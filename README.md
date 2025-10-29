# 🌤️ ESP32 IoT Weather Station (ESP-IDF)

A simple and elegant **IoT weather station** built using the **ESP32**, **BME280 sensor**, and a **128x64 SSD1306 OLED display** — powered by the **ESP-IDF** framework.  
It continuously measures **temperature, humidity, and atmospheric pressure**, displaying the data in real-time on the OLED screen.

---

## 🧩 Hardware Components

| Component | Description |
|------------|-------------|
| **ESP32 Dev Board** | Main microcontroller (Wi-Fi + BLE) |
| **BME280** | Temperature, humidity, and pressure sensor |
| **SSD1306 OLED (128x64)** | Display module via I²C |
| **Jumper wires / Breadboard** | For prototyping connections |

---

## ⚙️ Features

- 🌡️ Real-time weather data (Temperature, Humidity, Pressure)
- 🖥️ 128x64 OLED display with clean, minimal UI
- ⚡ Built using the native **ESP-IDF** framework
- 🧩 Modular structure using ESP-IDF components
- 🔧 Easy to extend (Wi-Fi, MQTT, or cloud integration)

---

## 🪛 Wiring Diagram

| BME280 Pin | ESP32 Pin |
|-------------|------------|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

| SSD1306 Pin | ESP32 Pin |
|--------------|-----------|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

> 📝 You can modify these I²C pins in `menuconfig` or your source code.

---

## 🧠 How It Works

1. The **ESP32** communicates with the **BME280** sensor over I²C to retrieve temperature, humidity, and pressure data.  
2. The data is formatted and displayed on the **SSD1306 OLED** using the `esp_ssd1306` component.  
3. The system periodically updates readings, providing live environmental data.

---

## 🧰 Software Setup

### Prerequisites
- [ESP-IDF v5.x or later](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/)
- USB connection to ESP32 board
- `idf.py` toolchain properly installed

---

### 📥 Clone the Repository
```bash
git clone https://github.com/<your-username>/weather-station.git
cd weather-station
