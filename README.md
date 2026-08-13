# basic-arduino

A collection of standalone Arduino/ESP32 sketches — no shared build system, each `.ino` is self-contained and opened directly in the Arduino IDE.

## Sketches

### `submarine-code.ino`

Controller sketch for a submarine ROV. Reads a water pressure sensor on analog pin `A0` and converts it to depth, reads a DS18B20 temperature sensor over OneWire, and displays depth/temperature/pressure on a 20x4 I2C LCD. Also reads 4 joystick digital inputs (`J1`–`J4`) and drives 4 relay outputs (`R1`–`R4`) to control thrusters/motors.

**Hardware:** DS18B20 temp sensor, water pressure sensor, 20x4 I2C LCD (`LiquidCrystal_I2C`), 4-way joystick input, 4-channel relay module.

**Libraries:** `Wire`, `OneWire`, `LiquidCrystal_I2C`, `DallasTemperature`

### `wifi-monitor/wifi-scan/Wifi_Scan.ino` and `wifi-monitor/Bluetooth_Scan/Bluetooth_Scan.ino`

ESP32 WiFi scanner. Scans for nearby networks every loop and prints SSID, RSSI, and BSSID over serial (115200 baud). The two files are functionally identical aside from delay timing (1s vs 5s between scans) — despite the folder name, `Bluetooth_Scan.ino` scans WiFi, not Bluetooth.

**Libraries:** `WiFi` (ESP32 core)

### `wifi-monitor/sp32wifi/sp32wifi.ino` and `wifi-monitor/sp32wifi_any/sp32wifi_any.ino`

ESP32 indoor positioning via WiFi RSSI trilateration. Scans for 3 fixed anchor access points (identified by BSSID), converts each anchor's RSSI to an estimated distance using a log-distance path-loss model, trilaterates an (x, y) position from the three distances, and POSTs the result to a Firebase Realtime Database.

`sp32wifi_any.ino` is the more robust variant: general trilateration that works for any anchor triangle (the base `sp32wifi.ino` version only holds for a right-angle anchor layout), plus a hard RSSI sanity filter (`rssi < -88` discarded) and 3-sample averaging per anchor per cycle (vs. 1 sample in the base version).

**Libraries:** `WiFi`, `HTTPClient` (ESP32 core)

**Anchor setup:** each sketch hardcodes 3 anchor BSSIDs and their (x, y) coordinates in meters, plus a path-loss model calibrated for those specific anchors (`Cval`, `nval`). Recalibrate `Cval`/`nval` and update the BSSIDs/coordinates for your own anchor placement.

## Before you push

`sp32wifi.ino` and `sp32wifi_any.ino` currently have a **WiFi password and a live Firebase database URL hardcoded in source**. Move these out before this repo goes public:

1. Create `secrets.h` (per sketch folder) holding the real values:
   ```cpp
   #define WIFI_SSID "your-network"
   #define WIFI_PASSWORD "your-password"
   #define FIREBASE_HOST "https://your-project.firebasedatabase.app"
   ```
2. `#include "secrets.h"` in the `.ino` file and replace the hardcoded `ssid`, `password`, and `firebaseHost` literals with the macros.
3. Add `secrets.h` to `.gitignore` so it's never committed.
4. Rotate the existing WiFi password and Firebase database rules/URL, since they're already exposed in this history.

Commit a `secrets.h.example` with placeholder values so the expected format is documented without leaking the real ones.

## Limitations

- No shared config — each sketch's constants (pins, anchor positions, calibration values) are edited directly in source.
- Trilateration accuracy depends entirely on RSSI-to-distance calibration (`Cval`, `nval`), which is environment-specific and will drift with interference, obstacles, or anchor movement.
- `sp32wifi.ino`'s trilateration math assumes a specific anchor geometry (B on the x-axis, C on the y-axis) and silently produces wrong results outside it — use `sp32wifi_any.ino` for arbitrary anchor placement.

## License

[MIT](LICENSE)
