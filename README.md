این متن کامل، یکدست و آمادهٔ کپی برای فایل `README.md` شماست:

```markdown
# Smart Gantry Irrigation & Monitoring System

An automated 2-axis (X-Y) gantry robot designed for micro-irrigation and individual plant condition monitoring in small-scale indoor crops or research setups.

---

## Overview

In targeted agricultural setups or controlled indoor experiments, uniform irrigation often leads to over-watering or under-watering. This project implements a Cartesian gantry system over a 5x5 plant matrix (25 pots) to provide:
1. **Soil-Moisture-Driven Irrigation:** Selective watering based on individual sensor thresholds.
2. **Growth Logging:** Image capturing via an onboard ESP32-CAM right after watering.
3. **Environmental Monitoring:** Real-time logging of ambient temperature and relative humidity.

---

## Hardware Architecture

The system utilizes an **Arduino Mega 2560** as the central microcontroller due to its higher I/O pin count. 

### Key Components & Bill of Materials (BOM)

| Component | Quantity | Purpose |
| :--- | :---: | :--- |
| **Arduino Mega 2560** | 1 | Main system logic and sensor reading |
| **CD74HC4067 Multiplexer** | 2 | 16-Channel Analog MUX to read 25 sensors |
| **Capacitive Soil Moisture Sensors** | 25 | Analog soil moisture measurement |
| **NEMA 17 Stepper Motors** | 2 | X and Y axis linear movement |
| **A4988 Drivers** | 2 | Stepper motor drivers |
| **12V DC Pump & Solenoid Nozzle** | 1 | Water dispensing unit |
| **ESP32-CAM** | 1 | Image capture trigger upon watering |
| **DHT22** | 1 | Ambient temperature and humidity monitoring |
| **5V / 12V Power Supply Units** | 2 | Separate logic and inductive load power supply |

---

## System Logic & Workflow

1. **Sensor Multiplexing:**
   The Arduino cycles through channels on the dual CD74HC4067 multiplexers to query all 25 capacitive moisture sensors sequentially.

2. **Threshold Check & Navigation:**
   If a pot's reading drops below the pre-set calibrated threshold:
   * Target coordinates $(X, Y)$ are calculated based on the pot index ($1 \le \text{Pot} \le 25$).
   * Stepper drivers move the gantry head directly over the target pot.

3. **Irrigation & Imaging:**
   * The 12V water pump activates for a calibrated duration (10–15 seconds).
   * A digital signal is pulsed to the ESP32-CAM to record an updated snapshot of the plant.

4. **Home Positioning:**
   After completing a full scanning pass, the gantry head returns to the home position $(0, 0)$ and pauses until the next scheduled routine.

---

## Repository Structure

```text
├── hardware/
│   ├── schematics.png       # Circuit diagrams and MUX pinouts
│   └── 3d_model.png         # Structural render of the X-Y frame
├── src/
│   └── main.ino             # Main firmware for Arduino Mega 2560
└── README.md

```

---

## Setup & Flashing

1. Clone the repository:
```bash
git clone [https://github.com/alirezarezaei2005/Smart-Gantry-Irrigation-Robot.git](https://github.com/alirezarezaei2005/Smart-Gantry-Irrigation-Robot.git)

```


2. Open `src/main.ino` in Arduino IDE.
3. Install required dependencies:
* `DHT sensor library`
* `Adafruit Unified Sensor`


4. Select board **Arduino Mega 2560** and flash the firmware.

---

## 👨‍💻 Author

**Alireza Rezaei Asl** Electrical & Control Engineering Student

📍 Isfahan, Iran

📧 alirezarezaii943@gmail.com

🔗 GitHub: [@alirezarezaei2005](https://github.com/alirezarezaei2005)

---

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](https://www.google.com/search?q=LICENSE) file for details.

```

```
