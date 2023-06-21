// spiro contains the code for spirometer functionality

#include <Adafruit_NeoPixel.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include "headr.h"


void IRAM_ATTR handleInterrupt1() {
  state_1 = true;
  if (state_2) {
    direction = -1;
  } else {
    direction = 1;
  }
  count += direction;
}
 
void IRAM_ATTR handleInterrupt2() {
  state_2 = true;
  if (state_1) {
    direction = 1;
  } else {
    direction = -1;
  }
  count += direction;
}

void updateNeopixels() {
  neopixels.clear();  // Turn off all Neopixels
  int numPixels = abs(count) / 10;  // Calculate the number of pixels based on count - CHANGE
  numPixels = min(numPixels, NUM_NEOPIXELS);  // Limit the number of pixels to the maximum
  if (count > 0) {
    for (int i = 0; i < numPixels; i++) {
      neopixels.setPixelColor(i, neopixels.Color(255, 255, 255));  // White color
    }
  } else {
    for (int i = NUM_NEOPIXELS - 1; i >= NUM_NEOPIXELS - numPixels; i--) {
      neopixels.setPixelColor(i, neopixels.Color(255, 255, 255));  // White color
    }
  }
  neopixels.show();
}

void update_spiro() {

    // Update BLE characteristic value for spiro
    String flowRateString = "Flow Rate: " + String(count) + "\n";
    String directionString = "Direction: " + String(direction > 0 ? "Exhale" : "Inhale") + "\n";
    String dataString_1 = flowRateString + directionString;
    pCharacteristic_1->setValue(dataString_1.c_str());
    pCharacteristic_1->notify();
    
}