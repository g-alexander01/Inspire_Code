// link dependancies
#include "headr.h"

#include <Servo.h>
#include <Preferences.h>    // for flash memory
#include <BLEDevice.h>  // bluetooth
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Adafruit_NeoPixel.h> 

#include "MQ135.h"
#include "DHT.h"


// defining global variables - declared in header file
int motor_pin = 32;
int led_1_pin = 33; //gone  
int led_2_pin = 25; // gone
int pot_pin = 38;   // change
int button_pin = 22;
int PHOTOTRANSISTOR_PIN_1 = 27;
int PHOTOTRANSISTOR_PIN_2 = 12;
int LED_PIN = 13;
int NEOPIXEL_PIN = 2;


char DHTTYPE = DHT11;                                                                                                           
int DHTPIN = 14; 
int PIN_MQ135 = 4;

int pot_val = 0;

volatile bool initial_pos_set = false;
volatile bool button_state = HIGH;
volatile bool cal_execute = false;  // set to true to not overwrite the time values
volatile bool state_1 = false;
volatile bool state_2 = false;

int start_time = 0;
int current_time = 0;

int speed_maxI = 180;
int speed_maxO = 0; 
int speed_stop = 90;

int NUM_NEOPIXELS = 8;

int count = 0;
int direction = 0;

float temperature = 0; 
float humidity = 0;

const int out_spd_arr[3] = {60, 30, 0}; // constant low, med, high speed out

const char SERVICE_UUID[] = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
const char CHARACTERISTIC_UUID_1[] = "beb5483e-36e1-4688-b7f5-ea07361b26a8";
const char CHARACTERISTIC_UUID_2[] = "beb5483e-36e1-4688-b7f5-ea07361b26a9";


Preferences storage;    // create instance of preferences
Servo motor; 
Adafruit_NeoPixel neopixels(NUM_NEOPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

BLECharacteristic *pCharacteristic_1;
BLECharacteristic *pCharacteristic_2;

MQ135 mq135_sensor(PIN_MQ135);
DHT dht(DHTPIN, DHTTYPE);


void setup() {

    Serial.begin(115200);
    delay(1000);

    // initialize storage space in flash memory for mode times in read/write mode (true = read only)
    storage.begin("mode_times", false);

    motor.attach(motor_pin);

    pinMode(button_pin, INPUT);
    pinMode(led_1_pin, OUTPUT);
    pinMode(led_2_pin, OUTPUT);


    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);
    neopixels.begin();
    neopixels.show();  // Initialize all pixels to off
    pinMode(PHOTOTRANSISTOR_PIN_1, INPUT);
    attachInterrupt(digitalPinToInterrupt(PHOTOTRANSISTOR_PIN_1), handleInterrupt1, RISING);
    pinMode(PHOTOTRANSISTOR_PIN_2, INPUT);
    attachInterrupt(digitalPinToInterrupt(PHOTOTRANSISTOR_PIN_2), handleInterrupt2, RISING);

    dht.begin();    // begin dht sensor

    setupBLE();  // Initialize Bluetooth Low Energy


    motor.write(speed_maxI);


}


void loop() {
    // using boolean to prevent calibrating every time

    /*
    if (cal_execute == false) {

        // calibrating each mode of the motor
        for (int j = 0; j < 3; j++) {

            calibration(j, out_spd_arr[j]);
        }
        cal_execute = true;
    }
    */

    update_dht_mq(); // update sensor activity



    delay(1000);    // crucial for 1s spiro calc interval

    detachInterrupt(digitalPinToInterrupt(PHOTOTRANSISTOR_PIN_1));
    detachInterrupt(digitalPinToInterrupt(PHOTOTRANSISTOR_PIN_2));

    // digitalWrite(LED_PIN, HIGH);  // Turn LED on
    if (count != 0) {
    Serial.print("Flow Rate: ");
    Serial.println(count);
    Serial.print("Direction: ");
    Serial.println(direction > 0 ? "Exhale" : "Inhale");

    // Update Neopixels based on count
    updateNeopixels();

    // update spiro values
    update_spiro();

    }

    count = 0;
    direction = 0;
    state_1 = false;
    state_2 = false;
    attachInterrupt(digitalPinToInterrupt(PHOTOTRANSISTOR_PIN_1), handleInterrupt1, RISING);
    attachInterrupt(digitalPinToInterrupt(PHOTOTRANSISTOR_PIN_2), handleInterrupt2, RISING);

    /*
    for (int k = 0; k < 10; k++) {

        mode_basic();
    }
    */
}