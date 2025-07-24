# 🌡️ ESP32-C3 Temperature Monitor with Web Dashboard

A real-time web-based temperature monitoring system using an **ESP32-C3 Mini-1 DevKit V1**, a **DS18B20 digital temperature sensor**, and a **3-wire active buzzer**.  
It serves a live webpage with temperature readings, alert system, interactive chart, and historical log — auto-updating every 5 seconds.

---

## 🚀 Features

- Live temperature monitoring via built-in web server
- Auto-refreshing webpage every 5s
- Overheat alert with buzzer
- Dynamic temperature chart (Chart.js)
- Live table log of recent measurements with UTC timestamps
- No cloud dependencies – fully offline-capable

---

## 🔌 Hardware Connections


| Component           | ESP32 Pin      | Notes                                                    |
| **DS18B20**         |                |                                                          |
| • Data (Yellow)     | `GPIO18`       | Use 4.7 kΩ **pull-up resistor** between GPIO18 and 3.3V  |
| • VCC (Black)       | `3.3V`         | Powers the sensor                                        |
| • GND (Red)         | `GND`          | Ground connection                                        |
| **Buzzer (3-wire)** |                |                                                          |
| • Signal (Gray)     | `GPIO19`       | Controls the buzzer                                      |
| • VCC (Red)         | `5V`           | Connect to 5V output of ESP32 (if available)             |
| • GND (Black)       | `GND`          | Ground connection                                        |
| • Pull-down         | `GPIO19 → GND` | Add **1 kΩ resistor** between GPIO19 and GND             |

---

## 🧠 How It Works

- On startup, the ESP32 connects to Wi-Fi and starts a local HTTP server.
- It queries the DS18B20 sensor every 5 seconds.
- Temperature is logged and displayed on a hosted webpage.
- If the temperature exceeds a configurable threshold (default: 30 °C), the buzzer is triggered.
- Users can monitor the temperature chart and log history remotely by visiting the ESP32's IP address.

---

## 🖥️ Web Interface Preview

![Web Dashboard Screenshot](images/screenshot.png)

*(replace with your own screenshot if available)*

---

## 🛠️ Requirements

- **Board:** ESP32-C3 Mini-1 DevKit V1  
- **Sensor:** DS18B20 (digital)  
- **Libraries:**
  - WiFi
  - WebServer
  - OneWire
  - DallasTemperature

---

## 📂 Project Structure

