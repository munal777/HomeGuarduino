# HomeGuarduino – IoT-Based Home Security System

HomeGuarduino is an IoT-based home security system project that integrates ESP32, sensors, and REST APIs to monitor and alert homeowners about potential security threats. It uses PostgreSQL for message storage and SendGrid for email notifications. I developed this project as the part of the **Cloud Computing and the Internet of Things** module to demonstrate how IoT devices can work with cloud technologies for smart home security.

## Features
✅ Real-time motion detection using PIR sensor  
✅ Door/window intrusion detection with electromagnetic reed switch  
✅ Gas leak detection using MQ-2 sensor  
✅ Wireless communication with ESP32  
✅ Automated email alerts via SendGrid  
✅ Secure backend API using Django Rest Framework (DRF)  
✅ Message storage using PostgreSQL  

## Technology Stack
### 🔹 Hardware Components
- **ESP32** – Microcontroller for IoT communication  
- **PIR Sensor** – Motion detection  
- **Electromagnetic Reed Switch** – Detects door/window intrusion  
- **MQ-2 Sensor** – Gas leakage detection  

### 🔹 Software & Tools
- **Django Rest Framework (DRF)** – Backend API development  
- **PostgreSQL** – Stores detected messages and logs  
- **SendGrid** – Sends email alerts  
- **Arduino IDE** – ESP32 programming  

## System Architecture
1️⃣ ESP32 collects sensor data (motion, intrusion, gas leak)  
2️⃣ Data is sent to the DRF backend via HTTP requests  
3️⃣ Backend stores alerts in PostgreSQL  
4️⃣ SendGrid sends email notifications if a security event is detected  
