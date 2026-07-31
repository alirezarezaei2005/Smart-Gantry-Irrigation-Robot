# 🌱 Smart Gantry Irrigation & Monitoring System

An autonomous **2-axis (X-Y) Cartesian gantry robot** for precision irrigation, environmental monitoring, and plant growth documentation in indoor agriculture and research applications.

---

## 📌 Overview

This project presents a smart irrigation platform capable of monitoring and watering **25 individual plants** arranged in a **5 × 5 grid**. Instead of uniformly irrigating all pots, the system continuously measures the moisture level of each plant and waters only those requiring irrigation.

After watering, an **ESP32-CAM** captures an image of the corresponding plant, enabling long-term growth monitoring. Ambient temperature and humidity are also recorded to provide additional environmental data.

---

## ✨ Features

* Individual soil moisture monitoring for 25 plants
* Precision irrigation using an X-Y gantry mechanism
* Automatic navigation to target plant coordinates
* ESP32-CAM image capture after irrigation
* Temperature and humidity monitoring (DHT22)
* Multiplexed analog sensor acquisition
* Modular embedded firmware running on Arduino Mega 2560

---

## 🛠 Hardware

| Component                        | Quantity | Description                        |
| -------------------------------- | :------: | ---------------------------------- |
| Arduino Mega 2560                |     1    | Main controller                    |
| CD74HC4067 Analog Multiplexer    |     2    | Reading 25 analog moisture sensors |
| Capacitive Soil Moisture Sensors |    25    | Soil moisture measurement          |
| NEMA17 Stepper Motors            |     2    | X-Y gantry movement                |
| A4988 Stepper Drivers            |     2    | Stepper motor control              |
| 12V Water Pump                   |     1    | Irrigation system                  |
| Solenoid Nozzle                  |     1    | Water dispensing                   |
| ESP32-CAM                        |     1    | Plant image acquisition            |
| DHT22                            |     1    | Temperature & humidity monitoring  |
| 5V / 12V Power Supplies          |     2    | Logic and motor power              |

---

## ⚙️ System Architecture

```
Soil Moisture Sensors
        │
        ▼
Dual CD74HC4067 Multiplexers
        │
        ▼
Arduino Mega 2560
        │
 ┌──────┼─────────────┐
 ▼      ▼             ▼
Stepper Drivers    Water Pump    ESP32-CAM
(X & Y Motion)                   (Image Capture)
        │
        ▼
Cartesian Gantry Robot
        │
        ▼
Target Plant
```

---

## 🔄 Operating Workflow

1. Read all 25 soil moisture sensors through the multiplexers.
2. Compare each reading with its calibrated threshold.
3. Compute the corresponding X-Y coordinates.
4. Move the gantry to the target position.
5. Activate the irrigation pump for a predefined duration.
6. Trigger the ESP32-CAM to capture a new image.
7. Repeat the process for all plants.
8. Return to the home position after completing one scan cycle.

---

## 📂 Repository Structure

```text
hardware/
├── schematics.png
├── wiring_diagram.png
└── 3d_model.png

src/
└── main.ino

images/
├── prototype.jpg
├── gantry.jpg
└── demo.gif

README.md
LICENSE
```

---

## 🚀 Getting Started

Clone the repository:

```bash
git clone https://github.com/ALIREZA943/smart-gantry-irrigation-monitoring-system.git
```

Open the project using the Arduino IDE, install the required libraries, upload the firmware to the Arduino Mega 2560, and configure the moisture thresholds according to your sensor calibration.

---

## 🔮 Future Improvements

* Closed-loop positioning using limit switches or encoders
* Web dashboard for remote monitoring
* Automatic irrigation scheduling
* Machine-learning-based plant health analysis
* Cloud-based environmental data logging
* Integration with MQTT or Wi-Fi monitoring

---

## 👨‍💻 Author

**Alireza Rezaei Asl**

Electrical & Control Engineering Student
Isfahan, Iran

* 📧 Email: **[alirezarezaii943@gmail.com](mailto:alirezarezaii943@gmail.com)**
* 🔗 GitHub: **https://github.com/alirezarezaei2005**
* 🎓 Interested in: **Embedded Systems, Robotics, Control Engineering, and Smart Agriculture**

Feel free to connect with me for academic collaborations, research projects, or embedded systems discussions.

---
## 📜 License

This project is released under the MIT License.

