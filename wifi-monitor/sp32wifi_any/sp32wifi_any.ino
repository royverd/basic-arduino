#include <WiFi.h>
#include <HTTPClient.h>

/* =======================
   WiFi (internet access)
   ======================= */
const char* ssid = "AsEmpenes";
const char* password = "oppai80085";

/* =======================
   Firebase Realtime DB
   ======================= */
String firebaseHost =
  "https://iot-p4-2025-default-rtdb.europe-west1.firebasedatabase.app";
String firebasePath = "/positions.json";

/* =======================
   Anchor BSSIDs (WiFi MAC)
   ======================= */
String anchorA = "4c:22:f3:13:0a:33";
String anchorB = "c0:94:ad:6f:f9:a0";
String anchorC = "c4:27:28:4e:c2:0c";

/* =======================
   Anchor coordinates (m)
   ======================= */
float Ax = 0,   Ay = 0;
float Bx = 1.0, By = 0;
float Cx = 0,   Cy = 1.0;

/* =======================
   RSSI → distance model
   ======================= */
float Cval = -60.674;
float nval = 2.4573;

/* =======================
   Helpers
   ======================= */
float rssiToDistance(float rssi) {
  return pow(10.0, (Cval - rssi) / (10.0 * nval));
}

void sendToFirebase(float x, float y) {
  HTTPClient http;
  http.begin(firebaseHost + firebasePath);
  http.addHeader("Content-Type", "application/json");

  String payload =
    "{\"x\":" + String(x,3) + ",\"y\":" + String(y,3) + "}";

  int code = http.POST(payload);
  Serial.printf("Sent (%.3f, %.3f) Firebase=%d\n", x, y, code);
  http.end();
}

/* =======================
   Robust trilateration
   Works for ANY triangle
   ======================= */
void trilateration(float dA, float dB, float dC) {
  float A = 2*(Bx - Ax);
  float B = 2*(By - Ay);
  float C = dA*dA - dB*dB - Ax*Ax + Bx*Bx - Ay*Ay + By*By;

  float D = 2*(Cx - Ax);
  float E = 2*(Cy - Ay);
  float F = dA*dA - dC*dC - Ax*Ax + Cx*Cx - Ay*Ay + Cy*Cy;

  float denom = (A*E - B*D);

  if (abs(denom) < 0.0001) {
    Serial.println("Trilateration failed (degenerate geometry)");
    return;
  }

  float x = (C*E - B*F) / denom;
  float y = (A*F - C*D) / denom;

  Serial.printf("Estimated Position: (%.3f, %.3f)\n", x, y);
  sendToFirebase(x, y);
}

/* =======================
   Setup
   ======================= */
void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.println(WiFi.localIP());
}

/* =======================
   Main loop
   ======================= */
void loop() {
  static float valsA[3], valsB[3], valsC[3];
  static int countA=0, countB=0, countC=0;

  int n = WiFi.scanNetworks(false, true);

  for (int i = 0; i < n; i++) {
    String bssid = WiFi.BSSIDstr(i);
    bssid.toLowerCase();
    int rssi = WiFi.RSSI(i);

    /* HARD RSSI SANITY FILTER */
    if (rssi < -88) continue;

    if (bssid == anchorA && countA < 3) {
      valsA[countA++] = rssi;
      Serial.printf("Anchor A RSSI=%d\n", rssi);
    }
    else if (bssid == anchorB && countB < 3) {
      valsB[countB++] = rssi;
      Serial.printf("Anchor B RSSI=%d\n", rssi);
    }
    else if (bssid == anchorC && countC < 3) {
      valsC[countC++] = rssi;
      Serial.printf("Anchor C RSSI=%d\n", rssi);
    }
  }

  WiFi.scanDelete();

  if (countA==3 && countB==3 && countC==3) {
    float meanA = (valsA[0]+valsA[1]+valsA[2])/3.0;
    float meanB = (valsB[0]+valsB[1]+valsB[2])/3.0;
    float meanC = (valsC[0]+valsC[1]+valsC[2])/3.0;

    Serial.printf(
      "Mean RSSI: A=%.2f B=%.2f C=%.2f\n",
      meanA, meanB, meanC
    );

    float dA = rssiToDistance(meanA);
    float dB = rssiToDistance(meanB);
    float dC = rssiToDistance(meanC);

    Serial.printf(
      "Distances: dA=%.2f dB=%.2f dC=%.2f\n",
      dA, dB, dC
    );

    trilateration(dA, dB, dC);

    countA = countB = countC = 0;
  }

  delay(500);
}
