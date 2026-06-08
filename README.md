# Gesture Controlled Floor Cleaning Car

An innovative, Human-Robot Interaction (HRI) prototype engineered to execute touch-free, wireless floor sanitation. The system translates physical smartphone coordinate tilts into localized movement instructions using a NodeMCU.

## 🛠️ System Architecture & Hardware Stack
* **Microcontroller Platform:** NodeMCU (ESP8266 Module hosting integrated Wi-Fi stack)
* **Motor Driver Module:** L298N Dual H-Bridge Motor Driver
* **Kinematics & Actuation:** Four 12V 100RPM center-shaft geared DC motors 
* **Sanitation Subsystem:** Dedicated high-torque 12V DC floor-sweeping motor
* **Controller Interface:** Android Gyroscopic Sensor Array

## ⚙️ Software Optimization Details
* **Asynchronous Web Daemon:** Developed a local HTTP web server on the ESP8266 core that intercepts remote gyroscope vectors via REST query arguments without dropping performance.
* **Granular PWM Tuning:** Utilized pulse-width modulation (`analogWrite`) logic to balance current distribution across four motors, mitigating mechanical drift and reducing battery drain.
* **Low-Latency Drive State Machine:** Configured precise discrete direction logic to coordinate instantaneous chassis adjustments (Forward, Backward, Left, Right, Stop).
