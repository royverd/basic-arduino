/*
  ESP32 BLE RSSI Logger
  ----------------------
  This sketch scans for BLE devices and prints RSSI values
  only for specific MAC addresses (anchors).
  Output format: Node,<Label>,<RSSI>
*/

// Multiple BLE Device for Data

#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

// Create a BLE scanner object
BLEScan* pBLEScan;

// Define MAC addresses of Anchors
// Each anchor is mapped to a label (A, B, C)
String anchorA = "72:67:cc:eb:c6:0f";  // JBL Headphones
String anchorB = "d8:d6:68:33:67:ce";  // PC
String anchorC = "58:39:dc:59:d7:98";  // PS5 Controller

// Helper function: normalize MAC address to lowercase for comparison
String normalize(const BLEAddress& addr) {
  String s = addr.toString().c_str();
  s.toLowerCase();
  return s;
}

// Callback class: runs every time a BLE advertisement is received
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    // Get normalized MAC and RSSI
    String addr = normalize(advertisedDevice.getAddress());
    int rssi = advertisedDevice.getRSSI();

    // Check which anchor this MAC belongs to and print in desired format
    if (addr.equalsIgnoreCase(anchorA)) {
      Serial.print("Node,A,");
      Serial.println(rssi);
    } else if (addr.equalsIgnoreCase(anchorB)) {
      Serial.print("Node,B,");
      Serial.println(rssi);
    } else if (addr.equalsIgnoreCase(anchorC)) {
      Serial.print("Node,C,");
      Serial.println(rssi);
    }
    // Other devices are ignored
  }
};

void setup() {
  Serial.begin(9600);              // Start serial monitor
  BLEDevice::init("");               // Initialize BLE stack
  pBLEScan = BLEDevice::getScan();   // Create scanner
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);     // Active scan = more data, higher power use
}

void loop() {
  // Perform a scan for 2 seconds
  pBLEScan->start(2, false);

  // Clear results to free memory before next scan
  pBLEScan->clearResults();
}


void setup() {
  Serial.begin(9600);
  BLEDevice::init("");                 // Initialize BLE
  pBLEScan = BLEDevice::getScan();     // Create scanner
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);       // Active scan = more data
}

void loop() {
  // Scan for 5 seconds
  pBLEScan->start(5, false);   // no need to store results
  pBLEScan->clearResults();    // free memory
}
*/

/*

// Only One for Pathloss

#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

BLEScan* pBLEScan;

// normalize to lowercase for comparison
String normalize(const BLEAddress& addr) {
  String s = addr.toString().c_str();
  s.toLowerCase();
  return s;
}

class FilterCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice dev) {
    if (normalize(dev.getAddress()).equalsIgnoreCase("72:67:cc:eb:c6:0f")) {
      // Print only the numeric RSSI value
      Serial.println(dev.getRSSI());
    }
  }
};

void setup() {
  Serial.begin(9600);
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new FilterCallbacks());
  pBLEScan->setActiveScan(true);
}

void loop() {
  pBLEScan->start(1, false);   // short scan window for faster refresh
  pBLEScan->clearResults();    // free memory
}

*/
