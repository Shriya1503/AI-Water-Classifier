# 💧 AI Water Quality Monitor

An intelligent IoT device that detects Safe, Tap, and Unsafe water using TinyML. For HackerEarth Edge Impulse Hackathon 2025.

## 🔗 Quick Links

- 📺 [View Demo Video](https://youtu.be/40XXZL1KhtI?si=IHx8mocMnkwHCQuq)

- 📱 [Launch Web App](https://shriya1503.github.io/AI-Water-Classifier/)

- 🧠 [View Model](https://studio.edgeimpulse.com/public/828724/live)

## 📸 Project Gallery

![Full Hardware Setup](https://github.com/Shriya1503/AI-Water-Classifier/blob/9e29376c744cb2d1c61d0d2ef325fcac651f3938/Assets/SetupLabelled.jpg)

![Real-time Analysis Dashboard](https://github.com/Shriya1503/AI-Water-Classifier/blob/e1b8ee0d3a5716288008ecc732d52cfcac9ad8fc/Assets/Default_Dashboard.jpg)

## 🚀 Project Overview

Water contamination is a major global health issue. Standard TDS meters only measure conductivity, missing dangerous non-conductive contaminants. This project solves that by using Machine Learning to analyze complex water quality "fingerprints."

It reads data from three sensors simultaneously and uses a neural network to classify water into three distinct categories:

- ✅ Safe: Filtered, RO, or Bottled water suitable for drinking.

- ℹ️ Tap: Standard municipal supply (potable but may need filtration).

- ⚠️ Unsafe: Contaminated with biological matter, soap, salt, or extreme pH levels.

  The datatset is collected from varies locations and for each samples few synthetic datapoints are added to make it more robust. Alsothere is some overlapping between Tap and Safe waters which is actually accurate as sometimes tap water are drinkable.This can be clearly seen in feature explorer graph on Edge Impulse. ![Feature Explorer](https://github.com/Shriya1503/AI-Water-Classifier/blob/19713001c579c1ae89c6bc9a82027c7b0be81898/Assets/FeatureExplorer.jpg)

## 🛠️ Hardware & Tech Stack

	Component				Specification					Function

	Microcontroller			Seeed XIAO nRF52840          Brain & Bluetooth LE
 
	TDS Sensor              Gravity Analog TDS           Measures dissolved solids

	pH Sensor               Gravity Analog pH            Measures acidity/alkalinity

	Turbidity               Custom Probe Hack            Measures suspended particles

	Power                   3.7V LiPo + Boost            Portable power supply


## Hardware Connections

	## 🔌 Hardware Connection Table

| **Component** | **Pin/Wire** | **Connection** | **Notes** |
| :--- | :--- | :--- | :--- |
| **Power Supply** | | | |
| **LiPo Battery (3.7V)** | Positive (+) | **BAT (+)** (XIAO) & **VIN+** (Boost) | Powers both MCU and Boost Converter |
| | Negative (-) | **GND (-)** (XIAO) & **VIN-** (Boost) | **Common Ground** |
| **Boost Converter** | **VOUT+** | **5V Power Rail** | Provides 5V for Turbidity & pH sensors |
| | **VOUT-** | **Common Ground** | Connect to global GND |
| **Sensors** | | | |
| **TDS Sensor** | VCC | **Pin D7** (XIAO) | **GPIO Powered** (Turned ON/OFF by code) |
| | GND | Common Ground | |
| | Signal (A) | **Pin A0** (XIAO) | Analog Input |
| **Turbidity Sensor** | VCC | **5V Power Rail** | Requires 5V for operation |
| | GND | Common Ground | |
| | Signal (OUT) | **Pin A1** (XIAO) | **⚠️ Via Voltage Divider** (See below) |
| **pH Sensor** | VCC | **5V Power Rail** | Requires 5V for operation |
| | GND | Common Ground | |
| | Signal (PO) | **Pin A2** (XIAO) | Analog Input |
| **Voltage Divider** | | | *Protects 3.3V Pin A1 from 5V Signal* |
| *(For Turbidity)* | **R1 (2.2kΩ)** | Sensor OUT <--> **Pin A1** | Top Resistor |
| | **R2 (4.7kΩ)** | **Pin A1** <--> GND | Bottom Resistor |
| **User Input** | | | |
| **Push Button** | Pin 1 | **Pin D3** (XIAO) | Digital Input |
| | Pin 2 | **GND** | Active Low Logic |

> **⚠️ Critical Wiring Notes:**
> 1.  **Common Ground:** All GND pins (Battery, Boost Converter, Xiao, Sensors) **must** be connected together.
> 2.  **TDS Power:** Do **not** connect the TDS sensor to the 5V rail. Connect it to **Pin D7**. This allows the software to turn it off during pH readings to prevent electrical interference.
> 3.  **Voltage Divider:** The Turbidity sensor outputs up to 4.5V. You **must** use the 2.2kΩ/4.7kΩ divider to step this down to < 3.3V to avoid damaging the XIAO.


## 📊 Model Performance

The TinyML model was trained using Edge Impulse on a balanced dataset of 270 samples.

Training Accuracy: **97.8%**

Testing Accuracy: **94.5%**

Inference Time: < 1 ms on device.

## 🔧 Engineering Challenges & Solutions

### 1. The Turbidity Sensor Hack

The standard driver board for the turbidity sensor failed to provide usable analog data from it's designated 'Out' pin.

_Solution_: We bypassed the driver board and tapped directly into the probe's phototransistor.

_Implementation_: Using a voltage divider, we scaled the raw 5V probe signal to the XIAO's 3.3V logic level. This required inverting the logic in software (Higher Voltage = Clearer Water).

### 2. Sensor Interference Fix

The TDS sensor injects current into the water, which corrupted the delicate pH readings.

Solution: Implemented Time-Division Multiplexing. The TDS sensor is powered by a GPIO pin 7 and is only turned ON for 100ms when needed. It remains OFF while pH and Turbidity are being sampled.

## ⚙️ How to Replicate

Flash Firmware: Upload [FinalBleAI_Water_Classifier.ino](https://github.com/Shriya1503/AI-Water-Classifier/blob/e1b8ee0d3a5716288008ecc732d52cfcac9ad8fc/HardwareCodes/FinalBleAI_Water_Classifier.ino) to the XIAO BLE.

Connect App: Open the Live Web App.

Pair: Click "Connect" and select WaterAI.( Need to be re-connected for every reading) You can also download a timestampped csv file for all reading all together for offline storage. A sample csv file has been added in Assets folder. 

Test: Dip sensors in water and press the physical button. The data is sent in 20 charater packets at once through BLE to web app. 

## 🔮 Future Roadmap

* **🛠️ Custom PCB Integration:** Transition from breadboard prototyping to a custom-designed, miniaturized PCB that integrates the XIAO BLE, power management, and sensor interfaces into a single board.
* **🖊️ "Smart Pen" Form Factor:** Design and 3D print a sleek, waterproof, pen-shaped enclosure. This "Probe-Pen" will house the battery, PCB, and sensors, making the device truly pocket-sized and field-deployable.
* **🔋 Solar Autonomy:** Add micro-solar harvesting to extend battery life indefinitely for remote monitoring stations.
* **📱 Native Mobile App:** Port the Web Bluetooth functionality to a native Flutter/Kotlin app for background monitoring and push notifications.

📜 License

This project is open-source under the MIT License.
