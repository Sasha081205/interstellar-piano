# 🎹 Interstellar Piano

A multi-microcontroller hardware synthesizer combining an **Arduino Mega 2560** and an **ESP32-S3**. It allows users to play notes, complex chords, and full pre-programmed melodies (including Hans Zimmer's *Cornfield Chase*) through both a physical keypad and a remote web dashboard.

🌐 **[View the Project Portfolio Page](https://sasha081205.github.io/interstellar-piano/)**

## 🚀 Features
* **Dual Interface**: Play via physical 4x4 matrix keypad or through the responsive HTTP dashboard.
* **Visual Feedback**: 8-LED array maps to different notes and octaves in real-time.
* **Chord Generation**: Maps single button presses to full major/minor triad chords.
* **Pre-loaded Melodies**: Features an extended 210-note arpeggio sequence of the *Interstellar* soundtrack, built with precision timings.
* **Custom UART Protocol**: Seamless bidirectional communication between the Mega (sound processing) and the ESP32 (web hosting).

## 🛠 Hardware Required
* Arduino Mega 2560
* ESP32-S3 (or standard ESP32)
* 4x4 Matrix Keypad
* Passive Buzzer
* 8x LEDs & 8x 220Ω Resistors
* Jumper wires & Breadboard

## 🔌 Quick Wiring Overview
* **UART Link**: Mega TX1 (18) -> ESP32 RX (16) | Mega RX1 (19) -> ESP32 TX (5). *A logic level converter is recommended for the 5V to 3.3V step-down.*
* **Common Ground**: Connect GND on both boards together.
* **Keypad**: Mega Pins A8-A11 (Rows), A12-A15 (Cols).
* **Audio/Visual**: Buzzer to Pin 8. LEDs to even pins from 22 to 36.

## 💻 Getting Started
1. **Flash the Mega**: Open `Mega_2560/Mega_Piano.ino` in the Arduino IDE and flash it to your Arduino Mega 2560. (Requires the `Keypad.h` library).
2. **Flash the ESP32**: Open `ESP32_S3/ESP32_Dashboard.ino` in the Arduino IDE and flash it to your ESP32-S3.
3. **Connect**: Power both boards. The ESP32 will create a Wi-Fi Access Point named **PIANO_GS** (Password: `interstellar`).
4. **Play**: Connect your phone or computer to the Wi-Fi network and navigate to `http://192.168.4.1` in your browser to open the dashboard.

## 📂 Repository Structure
* `/Mega_2560` - Contains the C++ logic for the Arduino Mega (Audio generation, keypad reading).
* `/ESP32_S3` - Contains the C++ logic for the ESP32 (Web server, HTML/CSS dashboard, Wi-Fi AP).
* `index.html` - GitHub Pages portfolio site code.
