#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Starting WiFi scan...");

  // Set WiFi to station mode and disconnect from any network
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Setup done");
}

void loop() {
  Serial.println("Scanning for WiFi networks...");

  int n = WiFi.scanNetworks();
  Serial.printf("Found %d networks\n", n);

  for (int i = 0; i < n; ++i) {
    // SSID (name), RSSI (signal strength), BSSID (MAC address)
    Serial.printf("%d: SSID=%s RSSI=%d dBm BSSID=%s\n",
                  i + 1,
                  WiFi.SSID(i).c_str(),
                  WiFi.RSSI(i),
                  WiFi.BSSIDstr(i).c_str());
  }

  Serial.println("Scan complete\n");

  delay(5000); // wait 5 seconds before next scan
}
