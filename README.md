# 🚨 LPG/Butane Gas Leak Detector

An embedded systems project utilizing a PIC16F877A microcontroller and an MQ-02 analog gas sensor to monitor environmental gas levels in real-time. 

## 📌 Project Overview
This system is designed to detect the presence of combustible gases (such as LPG or butane). It continuously samples the environment using the microcontroller's internal ADC. As gas concentrations rise, the system triggers a multi-tiered safety protocol, providing visual feedback via an LCD and LEDs, and an audible alarm via a buzzer when critical thresholds are reached.

### ⚙️ Hardware Components
* **Microcontroller:** PIC16F877A (Running at 4 MHz)
* **Sensor:** MQ-02 Gas Sensor
* **Display:** 16x2 Character LCD
* **Indicators:** Yellow LED (Warning), Red LED (Danger), Active Buzzer (Critical)

### 🧠 System Logic & Thresholds
The MQ-02 sensor outputs an analog voltage proportional to the gas concentration. The PIC16F877A processes this via its AN0 channel:
* `Normal (ADC < 400):` System idle, real-time value displayed on LCD.
* `Warning (ADC >= 400):` Yellow LED activated.
* `Danger (ADC >= 700):` Red LED activated.
* `Critical (ADC >= 900):` Buzzer activated for immediate evacuation alert.

## 📸 Hardware Setup & Demonstration



## 🛠️ Software Details
* **Language:** Bare-metal C
* **Compiler/IDE:** MPLAB X / XC8
* **Key Implementations:** Direct register manipulation for ADC initialization and reading, custom 4-bit mode LCD driver functions.
