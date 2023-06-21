// sensor contains data collection and transfer from the AQ, humidity and temperature sensors

#include "headr.h"

#include "MQ135.h"
#include "DHT.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
 

void update_dht_mq() {

    // turning on LED to indicate code is running
    // this needs to be removed after testing as the delay prevents execution speed (4s)
    blink_both(4);

    humidity = dht.readHumidity();
    temperature = dht.readTemperature();

    Serial.println(humidity);
    Serial.println(temperature);

    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
    }

    float rzero = mq135_sensor.getRZero();
    float correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
    float resistance = mq135_sensor.getResistance();
    float ppm = mq135_sensor.getPPM();
    float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);

    // Update BLE characteristic value
    String dataString_2 = "Temp: " + String(temperature) + "\n" +
                        "Hum: " + String(humidity) + "\n" +
                        "Corrected RZero: " + String(correctedRZero) + "\n" +
                        "Corrected PPM: " + String(correctedPPM) + "\n";
    pCharacteristic_2->setValue(dataString_2.c_str());
    pCharacteristic_2->notify();
}
 