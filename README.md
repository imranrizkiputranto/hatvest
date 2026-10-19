# Hatvest: a crop-stress hat

**Tech Canopies · Group 4 · MSc Cleantech Innovation, Imperial College London · October 2026**

Hatvest (working name) is a conical field hat with a spectral sensor on the front brim. When the wearer looks at a plant, the sensor measures how the leaf reflects red and near-infrared light, the Arduino calculates NDVI, and an LED on the brim glows **green** (healthy) or **red** (possibly stressed). The idea is to give small-scale farmers a consistent, hands-free check on crop health while they walk their field.

This repository is the handover package for the team taking the project forward.

## In plain English

**Hatvest is a farmer's hat that checks how healthy a plant is just by looking at it, and shows the answer as a green or red light on the brim.**

**How it works.** Plants reflect light in two ways that matter here:

- **Healthy leaves soak up red light.** The green stuff in leaves (chlorophyll) uses red light as food to grow, so very little red bounces back.
- **Healthy leaves bounce back lots of "invisible" light.** Just beyond red is near-infrared light, which our eyes can't see. A healthy leaf's insides are full of tiny air pockets that scatter it back out, like a mirror maze.

When a plant struggles (too dry, too hot, hungry or sick), it loses some of its green stuff, so more red light bounces back. A small sensor on the front of the hat measures both kinds of light and turns them into one score, called NDVI. High score: healthy, the light glows green. Low score: struggling, it glows red.

**Why it matters.**

- Climate change is making droughts and heatwaves more common and more severe, and that is already hurting harvests [1].
- Most of the world's farms are tiny: about 84% are smaller than 2 hectares, roughly three football pitches [3]. Those farmers can't afford drones or expensive equipment.
- Many of these farmers already wear a conical hat in the field, so the sensor goes on something they already use and their hands stay free.

**Why use it if a farmer can just look?** Honestly, our hat does not (yet) see problems before a farmer's eyes do; the research does not support that claim for this kind of sensor. What it does offer:

1. **A number instead of a guess.** How green a leaf looks changes with sunshine, cloud and who is looking. A score is the same every time. IRRI gave out over 250,000 colour cards to Asian rice farmers to make this judgement more consistent [g]. Our hat does the same job automatically.
2. **Spotting slow decline.** Eyes catch obvious yellowing, but are worse at noticing that one corner of a field is slightly weaker than the rest.
3. **A record.** Later versions could save each reading with its location to map the whole field. Farmers who don't want that just use the light.
4. **No extra effort.** It works while the farmer walks the field anyway.

**The research behind it.**

| What we claim | Who showed it |
|---|---|
| Plants soak up red light and bounce back invisible light, and the NDVI score measures this | Rouse et al. 1974 (first used the score) [4]; Tucker 1979 [5] |
| Stressed plants reflect light differently, so light can be used to check plant health | Peñuelas & Filella 1998 [6] |
| Catching stress *before* it is visible is possible, but with more advanced sensors (such as leaf temperature), not this simple score | Zarco-Tejada et al. 2018 [7] |

**What we found.** We tested six plants on campus: three healthy and three struggling. Every healthy leaf scored higher than every struggling one, with a clear gap between the groups.

**What is not proven yet.**

- It was a small test: six plants, one afternoon.
- Changing light (sun, cloud, shade) throws readings off. For now we fix that with a sheet of white paper under the sensor, which isn't practical in a field.
- We haven't asked farmers whether they would wear it or find it useful.
- Catching problems earlier than the eye is the big question for the next team. Our sensor already has extra colour channels to start testing it, and a cheap temperature sensor could be added.

> **In one line:** Hatvest turns a farmer's "this plant looks a bit off" into a consistent score, hands-free, and is a first step towards spotting problems before anyone can see them.

> **Start here:** open [`docs/index.html`](docs/index.html) in a browser for the full handover guide (background, research, build, results and next steps). On GitHub, enable Pages from the `/docs` folder to view it online.

---

## Contents

| Path | What it is |
|---|---|
| [`docs/index.html`](docs/index.html) | Full handover guide (single page, works offline) |
| [`docs/bench_test_guide.html`](docs/bench_test_guide.html) | Step-by-step guide we used to run the leaf test |
| [`firmware/hatvest_ndvi/hatvest_ndvi.ino`](firmware/hatvest_ndvi/hatvest_ndvi.ino) | Final Arduino sketch running on the hat (originally `sketch_oct16a_rgb.ino`) |
| [`tools/ndvi_logger.py`](tools/ndvi_logger.py) | Python logger: saves labelled readings to CSV |
| [`data/ndvi_log_2026-10-19.csv`](data/ndvi_log_2026-10-19.csv) | Raw data from our first leaf test |

---

## Background

**Climate adaptation.** More frequent and intense droughts and heatwaves are already reducing food security, and climate change has slowed the growth of agricultural productivity over the past 50 years [1]. Each 1 °C of warming is estimated to cut global rice yields by about 3.2% [2]. Catching crop stress early, before leaves visibly change, gives farmers time to act.

**Who it is for.** Small-scale farmers, especially in South-East Asia, and the agronomists who advise them. Of the world's more than 608 million farms, about 84% are smaller than 2 ha [3]. Conical hats are already standard field wear in the region, so the sensor rides on something farmers already wear, leaving their hands free.

**Why NDVI.** The normalised red/NIR difference was first used by Rouse et al. (as a "band ratio parameter") [4] and its link to green vegetation established by Tucker [5]. Plant stress changes red and NIR reflectance [6]. Airborne hyperspectral and thermal imaging has detected infection before symptoms appeared, but in that study NDVI itself did not separate asymptomatic from symptomatic trees [7]. So "earlier than the eye" is a goal for this project to test, not a result.

### Similar technologies already in use

| Product / company | How it works | Gap for small-scale farmers |
|---|---|---|
| [GreenSeeker handheld](https://www.ptxtrimble.com/en/products/hardware/flow-application-control/greenseeker-handheld-crop-sensor) (Trimble / PTx Trimble) | Handheld NDVI meter with its own red and NIR LEDs, so readings are not affected by light intensity [b]. From US$495 [a]. | Occupies the farmer's hands; a separate tool to buy and carry. |
| Crop Circle CC-430 / CC-470 (Holland Scientific) | Active canopy sensors with their own LEDs; red, red-edge and NIR bands; used for nitrogen diagnosis [d]. | Specialist instrument aimed at agronomists and researchers. |
| MicaSense RedEdge (drone camera) | Five bands including red, red edge and NIR; calibrated with a reflectance panel plus an upward-facing light sensor (DLS 2), the same idea as our proposed sky sensor [e]. | Needs a drone, a pilot and image processing. |
| Sentinel-2 (ESA Copernicus satellites) | Red and NIR at 10 m resolution, revisit about every 5 days [f]. | A 10 m pixel can cover a whole smallholder plot; cloud cover. |
| Leaf Colour Chart (IRRI) | Card of green shades for judging rice nitrogen by eye; 250,000+ distributed in eight Asian countries, including Indonesia [g]. | Cheap, but relies on visible colour: late and subjective. |
| Plantix (AI phone app) | Diagnoses pests and diseases from a photo; 45 crops, 500+ damage types [h]. | Only sees symptoms once visible. |

**Our angle:** the same measurement as a handheld NDVI meter, built into a hat farmers already wear, hands-free and low-cost. The trade-off: a passive sensor depends on ambient light, which GreenSeeker and Crop Circle avoid with their own LEDs.

### Value proposition

NDVI is not proven to see stress earlier than an experienced eye (see Background). The honest selling point: **turning what a farmer sees into a consistent, recorded number, hands-free, on a platform that can add earlier-warning sensors.**

1. **Eyes are inconsistent; a number isn't.** IRRI's Leaf Colour Chart, given to 250,000+ Asian rice farmers, exists to standardise that judgement for fertiliser decisions [g].
2. **Gradual decline is easy to miss.** Commercial NDVI tools are widely used to check crop nitrogen status during the season, not to detect disease early [b, d].
3. **Data, not memory.** Logged readings become maps, trends and evidence.
4. **Coverage without extra effort.** It measures while the farmer walks the field.

Points 2 to 4 are reasoning, not yet tested with farmers.

**What detects stress early in the research:** leaf temperature (thermal), chlorophyll fluorescence, PRI (531/570 nm), red-edge and green-band indices, and the ~970 nm water band [6, 7]. Drone cameras such as MicaSense measure red, red edge and NIR (some add thermal) and map NDVI/NDRE across fields; resellers claim early detection but cite no trial data [i].

---

## How it works

```
 Leaf  ──reflected light──▶  AS7343 sensor (front brim)
                                  │  I2C (A4/A5)
                                  ▼
                            Arduino Uno (back of hat)
                            reflectance = leaf ÷ white card
                            NDVI = (NIR − Red) ÷ (NIR + Red)
                                  │  D12 / D13
                                  ▼
                            Red/green LED (front brim)
                            NDVI ≥ 0.35 → green, else red
```

- **Why NDVI works:** chlorophyll absorbs red light, and the internal structure of a healthy leaf reflects near-infrared (NIR). A stressed leaf absorbs less red (more is reflected), and across a canopy NIR often drops too, so NDVI falls. In our test the drop came mainly from red. (Whether it falls before damage is visible is still to be tested; see Background.)
- **Why the white card:** the sensor's raw counts depend on how bright the light is. Dividing the leaf reading by a reading of a white card in the same light turns counts into reflectance (a fraction "out of" the light available).
- **Channels used (AS7343):** F7 (~690 nm) as red, F8 (~745 nm) as red edge, NIR (~855 nm).

## Hardware

| Part | Notes |
|---|---|
| Adafruit AS7343 14-channel spectral sensor | Front brim, facing the leaf |
| Arduino Uno R3 | Back of the hat, on a 3D-printed ring |
| RGB LED, common cathode (red and green legs only) | Soldered on perfboard with two 220 Ω resistors |
| 9 V battery | Powers the Uno; back of the hat |
| Jumper wires | Sensor and LED extensions from brim to back |
| 3D-printed ring | Holds the electronics at the back of the cone |
| Conical hat (caping) | Traditional straight-cone field hat |

### Wiring

| From | To Uno |
|---|---|
| Sensor VIN | 5V |
| Sensor GND | GND |
| Sensor SDA | A4 |
| Sensor SCL | A5 |
| LED red leg (via 220 Ω) | D12 |
| LED green leg (via 220 Ω) | D13 |
| LED common leg (longest) | GND |

Pins 12 and 13 are on/off only (not PWM), so the LED cannot mix an amber "borderline" colour. Move to pins 9, 10 or 11 if you want that.

## Quick start

1. **Install:** [Arduino IDE](https://www.arduino.cc/en/software), then the **Adafruit AS7343** library (Library Manager). For the logger: `pip install pyserial`.
2. **Upload:** open `firmware/hatvest_ndvi/hatvest_ndvi.ino`, choose **Arduino Uno** and its port, upload. On start-up the LED flashes green, then red.
3. **Calibrate:** in daylight, hold a sheet of white paper under the sensor and send `w` in the Serial Monitor (115200 baud). The reference is saved to EEPROM and survives power cycles, but **retake it whenever the light changes**.
4. **Measure:** point the sensor at a leaf. It streams a reading every second; `r` takes one reading, `s` pauses or resumes the stream.
5. **Log (optional):** close the Serial Monitor, then run `python tools/ndvi_logger.py`. Commands: `label <text>`, `w`, `r`, `q`. Readings are appended to `tools/ndvi_log.csv`.

**Light matters.** Use daylight or a halogen lamp. Indoor LED lights have almost no NIR, so readings under them are meaningless. With the white card under the sensor, raw counts should sit roughly between 5,000 and 50,000; if not, change only the `setGain(...)` line.

## Results so far (19 Oct 2026)

One bench session, outdoors, six campus plants (three healthy, three visibly stressed), 20 readings.

| Group | Readings | Mean NDVI | Range |
|---|---|---|---|
| Healthy | 9 | 0.43 | 0.386 to 0.498 |
| Stressed | 11 | 0.26 | 0.186 to 0.313 |

- No overlap between groups. Repeat readings on the same leaf varied by about ±0.02.
- A threshold of **0.35** classified all 20 readings correctly; the original 0.25 missed 7 of the 11 stressed readings. The sketch now uses 0.35.
- NDRE (red-edge index) did **not** separate the groups in this test.
- **Caveats:** a very small sample from one session; some "stressed" campus leaves may simply be autumn senescence; white-card counts (about 2,300 to 3,600) were below the recommended range, so the gain should be raised next time.

## Known limitations

- **White-card calibration** only holds for the light at the moment it was taken. Clouds, shade or time of day change it.
- **Electronics are exposed**: the 3D-printed enclosures need resizing and fitting.
- **Balance**: the Uno and battery at the back make the hat tip backwards.
- **Only campus plants tested**, not crops under controlled stress.
- **Untested on non-green leaves** (red or purple, anthocyanin-rich), which may read as stressed. In purple sweet potato leaves, red-edge reflectance still tracked chlorophyll despite anthocyanins, but NDVI was not tested [8].

## Next steps (top priorities)

1. **Automatic light correction:** an upward-facing "sky" sensor, as drone cameras use [e] (in drone tests this was less accurate than a white panel, so keep occasional white-card checks [9]), or an active light source (own red and NIR LEDs, as GreenSeeker does [b]).
2. **Test earlier-warning signals:** green-band and red-edge indices from the AS7343 channels we already have, plus a low-cost infrared thermometer for leaf temperature, against NDVI on plants under controlled stress.
3. **Auto gain and exposure** so readings stay in range from shade to full sun.
4. **Validate on real crops** under controlled stress, with more plants and sessions, and set per-crop thresholds.
5. **Enclosures, wiring harness and balance**: fit the housings, replace jumper chains with a soldered harness, move weight forward.
6. **User testing** with farmers for comfort, heat and usability.

The full list of improvements, with reasons, is in the [handover guide](docs/index.html#improvements).

## References

1. IPCC (2022). *Climate Change 2022: Impacts, Adaptation and Vulnerability.* WGII Summary for Policymakers. https://www.ipcc.ch/report/ar6/wg2/chapter/summary-for-policymakers/
2. Zhao, C. et al. (2017). Temperature increase reduces global yields of major crops in four independent estimates. *PNAS* 114(35), 9326–9331. https://doi.org/10.1073/pnas.1701762114
3. Lowder, S.K., Sánchez, M.V. & Bertini, R. (2021). Which farms feed the world and has farmland become more concentrated? *World Development* 142, 105455. https://doi.org/10.1016/j.worlddev.2021.105455
4. Rouse, J.W., Haas, R.H., Schell, J.A. & Deering, D.W. (1974). Monitoring vegetation systems in the Great Plains with ERTS. *Third ERTS-1 Symposium*, NASA SP-351. https://ntrs.nasa.gov/citations/19740022614
5. Tucker, C.J. (1979). Red and photographic infrared linear combinations for monitoring vegetation. *Remote Sensing of Environment* 8(2), 127–150. https://ntrs.nasa.gov/citations/19790049033
6. Peñuelas, J. & Filella, I. (1998). Visible and near-infrared reflectance techniques for diagnosing plant physiological status. *Trends in Plant Science* 3(4), 151–156. https://doi.org/10.1016/S1360-1385(98)01213-8
7. Zarco-Tejada, P.J. et al. (2018). Previsual symptoms of *Xylella fastidiosa* infection revealed in spectral plant-trait alterations. *Nature Plants* 4, 432–439. https://doi.org/10.1038/s41477-018-0189-7
8. Huang, J. et al. (2014). Eliminating interference by anthocyanin in chlorophyll estimation of sweet potato (*Ipomoea batatas* L.) leaves. *Botanical Studies* 55, 11. https://doi.org/10.1186/1999-3110-55-11
9. Mamaghani, B. & Salvaggio, C. (2019). Comparative study of panel and panelless-based reflectance conversion techniques for agricultural remote sensing. https://arxiv.org/abs/1910.03734

### Product and market sources

- [a] PrecisionAg. GreenSeeker goes handheld. https://www.precisionag.com/market-watch/greenseeker-goes-handheld/
- [b] Oklahoma State University Extension. Use of optical sensors to monitor plant nitrogen status in horticultural plants. https://extension.okstate.edu/fact-sheets/use-of-optical-sensors-to-monitor-plant-nitrogen-status-in-horticultural-plants.html
- [c] PTx Trimble. GreenSeeker handheld crop sensor. https://www.ptxtrimble.com/en/products/hardware/flow-application-control/greenseeker-handheld-crop-sensor
- [d] Cao, Q. et al. (2018). Evaluating two Crop Circle active canopy sensors for in-season diagnosis of winter wheat nitrogen status. *Agronomy* 8(10), 201. https://doi.org/10.3390/agronomy8100201
- [e] NERC Field Spectroscopy Facility. MicaSense RedEdge-M. https://fsf.nerc.ac.uk/instruments/uav/rededge-m/
- [f] Yale Center for Earth Observation. What is Sentinel-2? https://yceo.yale.edu/what-sentinel-2
- [g] Farming First (2011). Leaf colour charts help Asian farmers manage nitrogen fertilizers. https://farmingfirst.org/2011/01/leaf-colour-charts-help-asian-farmers-manage-nitrogen-fertilizers/
- [h] Future Farming (2024). Plantix: more than a crop doctor driven by AI. https://www.futurefarming.com/smart-farming/plantix-more-than-a-crop-doctor-driven-by-ai/
- [i] Measur. Precision agriculture with MicaSense. https://measur.ca/pages/guides/precision-agriculture-micasense

Prices and product details change; check them before quoting.

## Team

Group 4: Yuxuan Li, Xinrui (Rita) Geng, Imran (Rizki) Putranto, Wan Shuen Siaw.
Project expert: *to be confirmed*.
