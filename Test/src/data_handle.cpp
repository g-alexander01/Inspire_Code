// data handle contains functionality for comitting data to ESP flash memory and comms with user devices

// linking library dependancies
#include <Preferences.h> 

// linking header file with variable and function declarations
#include "headr.h"


void store_time(int mode_num, int time) {

    if (mode_num == 0) {

        // Serial.println("Time stored");
        storage.putInt("1", time);
    }
    else if (mode_num == 1) {

        // Serial.println("Time stored");
        storage.putInt("2", time);
    }
    else if (mode_num == 2) {

        // Serial.println("Time stored");
        storage.putInt("3", time);
    }
}


void setupBLE() {
    BLEDevice::init("Inspire");
    BLEServer *pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService(SERVICE_UUID);
    pCharacteristic_1 = pService->createCharacteristic(
                        CHARACTERISTIC_UUID_1,
                        BLECharacteristic::PROPERTY_READ |
                        BLECharacteristic::PROPERTY_WRITE |
                        BLECharacteristic::PROPERTY_NOTIFY
                    );

    pCharacteristic_2 = pService->createCharacteristic(
                        CHARACTERISTIC_UUID_2,
                        BLECharacteristic::PROPERTY_READ |
                        BLECharacteristic::PROPERTY_WRITE |
                        BLECharacteristic::PROPERTY_NOTIFY
                    );
    
    pCharacteristic_1->setValue("Spirometer");
    pCharacteristic_2->setValue("Environment Sensing");

    pService->start();

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
    Serial.println("Characteristic defined! Now you can read it in your phone!");
}