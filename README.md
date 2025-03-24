# 🚗 Parking System — Arduino Uno

## 🧠 Project Overview

This repository contains a simple yet functional implementation of a smart parking system developed using an Arduino Uno. The system is designed to manage the availability of parking slots in real-time using sensors and actuators.

It consists of:
- **4 photoresistors** that monitor individual parking spaces.
- A **7-segment display** that shows the number of available slots.
- A **HC-SR04 ultrasonic sensor** that detects incoming vehicles at the entrance.
- A **servo motor** that controls a parking gate (barrier).
- **LED indicators** that show slot availability status.

---

## ⚙️ How It Works

1. Each photoresistor detects whether a car is occupying a specific slot.
2. The number of free slots is calculated and displayed on a 7-segment display.
3. When a vehicle is detected by the ultrasonic sensor at the entrance:
   - If there are available slots, the servo motor lifts the barrier.
   - If the parking lot is full, the barrier remains down.
4. LEDs provide a visual indication of availability per slot.

---

## 🔧 Components Used

- Arduino Uno
- 4 Photoresistors
- HC-SR04 Ultrasonic Sensor
- 1 Servo Motor (for the gate)
- 7-Segment Display
- LEDs + Resistors
- Breadboard & Jumper Wires

---


