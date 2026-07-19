# 🎹 Interstellar Piano

A multi-microcontroller hardware synthesizer combining an **Arduino Mega 2560** and an **ESP32-S3**. It allows users to play notes, complex chords, and full pre-programmed melodies (including Hans Zimmer's *Cornfield Chase*) through both a physical keypad and a remote web dashboard.

🌐 **[View the Project Portfolio Page](https://YOUR_GITHUB_USERNAME.github.io/YOUR_REPO_NAME/)** *(Remember to update this link)*

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

## 📚 Software & Libraries Required
To compile the code in the Arduino IDE, you need to configure the following:

**For Arduino Mega 2560:**
* **Keypad** library by Mark Stanley, Alexander Brevig. 
  *(Install via Arduino IDE: Sketch > Include Library > Manage Libraries... search for "Keypad")*

**For ESP32-S3:**
* **esp32** Board Package by Espressif Systems.
  *(Install via Arduino IDE: Tools > Board > Boards Manager... search for "esp32")*
* *Note: The `WiFi.h`, `WebServer.h`, and `HardwareSerial.h` libraries are included automatically with the ESP32 core.*

## 🔌 Detailed Wiring Guide (Cable by Cable)

To build the Interstellar Piano, use a breadboard to share the Ground (GND) connections. Here is exactly where every single cable goes.

### 1. Power & Ground (The Foundation)
If the boards do not share the same Ground, the UART communication will fail and read as garbage data.
* **Cable 1:** Connect any **GND** pin on the Arduino Mega to the negative (blue) rail of your breadboard.
* **Cable 2:** Connect any **GND** pin on the ESP32-S3 to the same negative rail of your breadboard.

### 2. UART Communication (Mega ↔ ESP32)
* **Cable 3:** Connect **Mega Pin 19 (RX1)** to **ESP32 GPIO 5 (TX)**.
* **Cable 4:** Connect **Mega Pin 18 (TX1)** to **ESP32 GPIO 16 (RX)**.
  > *⚠️ Important Voltage Note:* The Mega sends a 5V signal on its TX pin, but the ESP32 pins are designed for 3.3V. For a safe long-term build, put a Logic Level Converter or a simple voltage divider (e.g., a 1kΩ and a 2kΩ resistor) on Cable 4 to drop the 5V to 3.3V.

### 3. Audio (Passive Buzzer)
* **Cable 5:** Connect the **Positive pin** (usually longer leg or marked with a '+') of the buzzer to **Mega Pin 8**.
* **Cable 6:** Connect the **Negative pin** to the shared **GND** rail on the breadboard.

### 4. Visual Feedback (8x LEDs)
* **Cables 7 to 14 (Anodes):** Connect the long legs of your 8 LEDs directly to the Mega digital pins:
  * LED 1 (DO) ➔ Mega Pin 22
  * LED 2 (RE) ➔ Mega Pin 24
  * LED 3 (MI) ➔ Mega Pin 26
  * LED 4 (FA) ➔ Mega Pin 28
  * LED 5 (SOL) ➔ Mega Pin 30
  * LED 6 (LA) ➔ Mega Pin 32
  * LED 7 (SI) ➔ Mega Pin 34
  * LED 8 (Chord/Star) ➔ Mega Pin 36
* **Resistors (Cathodes):** Connect a 220Ω resistor to the short leg of *every single LED*. Connect the other end of all 8 resistors to the shared **GND** rail on your breadboard.

### 5. Input (4x4 Matrix Keypad)
Place the keypad face up. The 8 pins at the bottom are usually ordered Left-to-Right: Row 1, Row 2, Row 3, Row 4, Col 1, Col 2, Col 3, Col 4.
* **Rows (Left 4 pins):**
  * Pin 1 (Row 1) ➔ Mega Pin 62 (Analog A8)
  * Pin 2 (Row 2) ➔ Mega Pin 63 (Analog A9)
  * Pin 3 (Row 3) ➔ Mega Pin 64 (Analog A10)
  * Pin 4 (Row 4) ➔ Mega Pin 65 (Analog A11)
* **Columns (Right 4 pins):**
  * Pin 5 (Col 1) ➔ Mega Pin 66 (Analog A12)
  * Pin 6 (Col 2) ➔ Mega Pin 67 (Analog A13)
  * Pin 7 (Col 3) ➔ Mega Pin 68 (Analog A14)
  * Pin 8 (Col 4) ➔ Mega Pin 69 (Analog A15)

## 💻 Getting Started
1. **Prepare Arduino IDE**: Ensure you have installed the required libraries and the ESP32 board package mentioned in the Software section.
2. **Flash the Mega**: Open `Mega_2560/Mega_Piano.ino` and upload it to your Arduino Mega 2560.
3. **Flash the ESP32**: Open `ESP32_S3/ESP32_Dashboard.ino` and upload it to your ESP32-S3.
4. **Connect**: Power both boards. The ESP32 will create a Wi-Fi Access Point named **PIANO_GS** (Password: `interstellar`).
5. **Play**: Connect your phone or computer to the Wi-Fi network and navigate to `http://192.168.4.1` in your browser to open the dashboard.

## 📂 Repository Structure
* `/Mega_2560` - Contains the C++ logic for the Arduino Mega (Audio generation, keypad reading).
* `/ESP32_S3` - Contains the C++ logic for the ESP32 (Web server, HTML/CSS dashboard, Wi-Fi AP).
* `index.html` - GitHub Pages portfolio site code.
