# 📊 GT7-UDP: Traction Loss Simulation System for PS5 + GT7

## 📌 Credits

This project is based on the excellent work by [MacManley in the gt7-udp repository](https://github.com/MacManley/gt7-udp), which enables real-time data reception via UDP from the PlayStation 5 while playing **Gran Turismo 7**.  
The original code handles data reception sent by the console over the local network (router), decodes this data in real time, and makes it available for various uses such as telemetry and hardware integrations.

All credit for the original data reception and processing logic belongs to the original author.  
My contribution starts from this solid foundation.

---

## 🚀 About My Project

My name is César Roncon and using this base project, I developed an additional traction loss simulation system for my racing simulator — something I personally missed, especially in the physical feedback of the cockpit.

### ✅ What I Added:
I implemented two feedback channels (two bass shakers) designed to simulate:

- **Rear wheel traction loss** during acceleration  
- **Front wheel traction loss** during braking (ABS activation)

> ⚠️ Note: This system is designed exclusively for **PS5** running **Gran Turismo 7**. It is not compatible with other games.

---

## 🛠️ How It Works

1. **Required Hardware**
   - 2x Bass Shakers
   - 1x Amplifiers (with power supply)
   - 1x 3.5mm male jack connectors (for signal input)
   - 1x ESP32 controller (to receive and process the data)

2. **Data Reception**
   - The **PS5** sends UDP data to a specific IP address on the local network.
   - The modified code, based on MacManley's project, captures this data.

3. **Processing**
   - The ESP32 receives the data from the code.
   - It filters and identifies in real time:
     - When traction is lost under acceleration (rear wheels)
     - When traction is lost under braking (front wheels / ABS)

4. **Physical Feedback**
   - Depending on the situation, the ESP32 sends an analog signal to the corresponding amplifier:
     - **Gas Bass Shaker** → To simulate traction loss under acceleration
     - **Brake Bass Shaker** → To simulate ABS/traction loss under braking

---

## 🎯 Purpose

This project was created to enhance the immersion of my simulator rig by adding a missing physical sensation: the ability to feel real-time traction loss from both the rear and front wheels while driving in GT7 on PS5.

---

## 📣 Final Note

If you're on **PS5** and a **Gran Turismo 7** fan like me, I hope you enjoy this project and adapt it to your own sim racing setup!

---
