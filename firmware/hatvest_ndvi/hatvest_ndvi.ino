// =============================================================
// Crop-Stress Hat: NDVI to RGB LED
//
// Board: Arduino Uno
//
// Sensor: Adafruit AS7343
//   VIN -> 5V
//   GND -> GND
//   SDA -> A4
//   SCL -> A5
//
// RGB LED (Red and Green only):
//   RED   -> D12 (through 220 ohm resistor)
//   GREEN -> D13 (through 220 ohm resistor)
//   BLUE  -> Not connected
//
// Common Cathode: COM -> GND
// Common Anode:   COM -> 5V
//
// =============================================================

#include <Adafruit_AS7343.h>
#include <EEPROM.h>
#include <math.h>

// =============================================================
// SETTINGS
// =============================================================

// RGB LED pins
const int RGB_RED_PIN = 12;
const int RGB_GREEN_PIN = 13;

// RGB LED type
// false = Common Cathode (COM connected to GND)
// true  = Common Anode (COM connected to 5V)

const bool COMMON_ANODE = false;

// NDVI threshold
const float NDVI_THRESHOLD = 0.35;

// =============================================================
// GLOBAL VARIABLES
// =============================================================

// AS7343 sensor
Adafruit_AS7343 sensor;

// Sensor readings
uint16_t readings[18];

// White reference values
float whiteRed = 0;
float whiteEdge = 0;
float whiteNir = 0;

// White reference status
bool haveWhite = false;

// Serial streaming
bool streaming = true;

// Measurement timing
unsigned long lastReadTime = 0;


// =============================================================
// RGB LED CONTROL
// =============================================================

// greenOn = true  -> Green LED ON
// redOn   = true  -> Red LED ON

void setLeds(bool greenOn, bool redOn) {

  if (COMMON_ANODE) {

    // Common Anode RGB LED
    // LOW = ON
    // HIGH = OFF

    digitalWrite(
      RGB_GREEN_PIN,
      greenOn ? LOW : HIGH
    );

    digitalWrite(
      RGB_RED_PIN,
      redOn ? LOW : HIGH
    );

  } else {

    // Common Cathode RGB LED
    // HIGH = ON
    // LOW = OFF

    digitalWrite(
      RGB_GREEN_PIN,
      greenOn ? HIGH : LOW
    );

    digitalWrite(
      RGB_RED_PIN,
      redOn ? HIGH : LOW
    );

  }
}


// =============================================================
// SENSOR READING
// =============================================================

bool readSensor(float &red, float &edge, float &nir) {

  // Read all spectral channels

  if (!sensor.readAllChannels(readings)) {

    Serial.println("Read error");

    return false;
  }

  // Extract required channels

  red = readings[AS7343_CHANNEL_F7];

  edge = readings[AS7343_CHANNEL_F8];

  nir = readings[AS7343_CHANNEL_NIR];

  // Check saturation

  if (
    red >= 65535 ||
    edge >= 65535 ||
    nir >= 65535
  ) {

    Serial.println(
      "WARNING: Sensor saturated. Reduce light intensity."
    );

    return false;
  }

  return true;
}


// =============================================================
// NDVI / NDRE CALCULATION
// =============================================================

void showReading(float red, float edge, float nir) {

  // Print raw readings

  Serial.print("raw red(F7)=");
  Serial.print(red, 0);

  Serial.print(" edge(F8)=");
  Serial.print(edge, 0);

  Serial.print(" nir=");
  Serial.print(nir, 0);


  // Check white reference

  if (!haveWhite) {

    setLeds(false, false);

    Serial.println(
      " | No white reference. Type w to calibrate."
    );

    return;
  }


  // ===========================================================
  // STEP 1: NORMALIZE USING WHITE REFERENCE
  // ===========================================================

  float r = red / whiteRed;

  float e = edge / whiteEdge;

  float n = nir / whiteNir;


  // Check for invalid measurement

  if (
    !isfinite(r) ||
    !isfinite(e) ||
    !isfinite(n) ||
    (n + r) <= 0 ||
    (n + e) <= 0
  ) {

    Serial.println(" | Invalid measurement");

    setLeds(false, false);

    return;
  }


  // ===========================================================
  // STEP 2: CALCULATE VEGETATION INDICES
  // ===========================================================

  float ndvi = (n - r) / (n + r);

  float ndre = (n - e) / (n + e);


  // Print results

  Serial.print(" | NDVI=");
  Serial.print(ndvi, 3);

  Serial.print(" NDRE=");
  Serial.print(ndre, 3);


  // ===========================================================
  // STEP 3: CONTROL RGB LED
  // ===========================================================

  if (ndvi >= NDVI_THRESHOLD) {

    // Healthy vegetation
    // Green ON, Red OFF

    setLeds(true, false);

    Serial.println(" -> GREEN (healthy)");

  } else {

    // Stressed vegetation
    // Red ON, Green OFF

    setLeds(false, true);

    Serial.println(" -> RED (stressed)");

  }
}


// =============================================================
// SETUP
// =============================================================

void setup() {

  // Start serial communication

  Serial.begin(115200);


  // ===========================================================
  // RGB LED INITIALIZATION
  // ===========================================================

  // Set the OFF level before enabling output

  if (COMMON_ANODE) {

    digitalWrite(RGB_GREEN_PIN, HIGH);
    digitalWrite(RGB_RED_PIN, HIGH);

  } else {

    digitalWrite(RGB_GREEN_PIN, LOW);
    digitalWrite(RGB_RED_PIN, LOW);

  }

  pinMode(RGB_GREEN_PIN, OUTPUT);

  pinMode(RGB_RED_PIN, OUTPUT);


  // ===========================================================
  // RGB LED SELF TEST
  // ===========================================================

  Serial.println("Testing RGB LED...");

  // Green ON

  setLeds(true, false);

  delay(400);


  // Red ON

  setLeds(false, true);

  delay(400);


  // Both OFF

  setLeds(false, false);

  Serial.println("RGB LED test complete");


  // ===========================================================
  // SENSOR INITIALIZATION
  // ===========================================================

  Serial.println("Initializing AS7343...");

  if (!sensor.begin()) {

    Serial.println(
      "Sensor not found. Check A4, A5, 5V, GND."
    );

    // Flash red LED continuously

    while (true) {

      setLeds(false, true);

      delay(200);

      setLeds(false, false);

      delay(200);
    }
  }


  Serial.println("AS7343 connected");


  // ===========================================================
  // SENSOR CONFIGURATION
  // ===========================================================

  sensor.setGain(AS7343_GAIN_64X);

  sensor.setATIME(29);

  sensor.setASTEP(599);


  // ===========================================================
  // LOAD WHITE REFERENCE FROM EEPROM
  // ===========================================================

  EEPROM.get(0, whiteRed);

  EEPROM.get(4, whiteEdge);

  EEPROM.get(8, whiteNir);


  // Validate saved values

  if (
    isfinite(whiteRed) &&
    isfinite(whiteEdge) &&
    isfinite(whiteNir) &&
    whiteRed > 0 &&
    whiteEdge > 0 &&
    whiteNir > 0 &&
    whiteRed < 65535 &&
    whiteEdge < 65535 &&
    whiteNir < 65535
  ) {

    haveWhite = true;

    Serial.println(
      "White reference loaded from EEPROM"
    );

  } else {

    haveWhite = false;

    Serial.println(
      "No valid white reference found"
    );
  }


  // ===========================================================
  // READY
  // ===========================================================

  Serial.println();

  Serial.println("================================");

  Serial.println("Crop-Stress Hat Ready");

  Serial.println("================================");

  Serial.println("Commands:");

  Serial.println("w = Calibrate white reference");

  Serial.println("r = Take one measurement");

  Serial.println("s = Pause / Resume streaming");

  Serial.println("================================");

  Serial.println();
}


// =============================================================
// MAIN LOOP
// =============================================================

void loop() {

  float red, edge, nir;


  // ===========================================================
  // SERIAL COMMANDS
  // ===========================================================

  while (Serial.available() > 0) {

    char c = Serial.read();


    // Ignore spaces and line endings

    if (
      c == '\n' ||
      c == '\r' ||
      c == ' '
    ) {

      continue;
    }


    // =========================================================
    // COMMAND W: WHITE REFERENCE CALIBRATION
    // =========================================================

    if (c == 'w') {

      Serial.println("Calibrating white reference...");

      if (readSensor(red, edge, nir)) {

        if (
          red > 0 &&
          edge > 0 &&
          nir > 0
        ) {

          whiteRed = red;

          whiteEdge = edge;

          whiteNir = nir;

          haveWhite = true;


          // Save to EEPROM

          EEPROM.put(0, whiteRed);

          EEPROM.put(4, whiteEdge);

          EEPROM.put(8, whiteNir);


          Serial.println(
            "White reference saved to EEPROM"
          );


          Serial.print("WHITE REFERENCE STORED ");

          showReading(red, edge, nir);

        } else {

          Serial.println(
            "Too dark for calibration. Add light."
          );
        }
      }
    }


    // =========================================================
    // COMMAND R: MANUAL MEASUREMENT
    // =========================================================

    else if (c == 'r') {

      if (readSensor(red, edge, nir)) {

        Serial.print("SAMPLE ");

        showReading(red, edge, nir);
      }
    }


    // =========================================================
    // COMMAND S: STREAMING ON / OFF
    // =========================================================

    else if (c == 's') {

      streaming = !streaming;

      if (streaming) {

        Serial.println("Live stream ON");

      } else {

        Serial.println("Live stream PAUSED");

      }
    }


    // =========================================================
    // UNKNOWN COMMAND
    // =========================================================

    else {

      Serial.println(
        "Unknown command. Use w, r or s."
      );
    }
  }


  // ===========================================================
  // AUTOMATIC MEASUREMENT EVERY SECOND
  // ===========================================================

  if (
    streaming &&
    millis() - lastReadTime >= 1000
  ) {

    lastReadTime = millis();


    if (readSensor(red, edge, nir)) {

      showReading(red, edge, nir);
    }
  }
}