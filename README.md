# Project_Aether

Project Aether is a handheld remote logic controller. It implements an ESP32-S3 as the main processor along with an RP2350 coprocessor. The ESP32 handles LoRa, GPS, and sensor input, and controls the OLED display. The RP2350 has its IO brought out on a header for expandability, use as a logic analyzer, or signal injection.

Features include:

ESP32-S3
    WiFi
    BLE

MAX-M10S GPS
SX-1262 LoRa Module

RP2350 as flexible logic controller

3 Axis Accelerometer
3 Axis Gyroscope
3 Axis Magnetometer(Compass)
Barometric Pressure Sensor

Secure Element
Secure Coprocessor

Battery Management
    Fuel Guage
    Battery charger with OTG support

micro-SD card interface