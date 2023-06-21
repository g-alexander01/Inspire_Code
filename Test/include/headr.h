// header file providing variables and function declaration and documentation
// outlines variables and functions of spiro.cpp, mechanism.cpp and data.cpp, for use by main file

// linking other header files for classes
#include <Preferences.h> 
#include <Servo.h>
#include <Adafruit_NeoPixel.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include "MQ135.h"
#include "DHT.h"



// declare pins
extern int motor_pin;   // pin for motor
extern int led_1_pin;   // yellow led
extern int led_2_pin;   // blue led 
extern int pot_pin; // potentiometer
extern int button_pin;  // button
extern int PHOTOTRANSISTOR_PIN_1;   // photoran 1 pin
extern int PHOTOTRANSISTOR_PIN_2;   // photoran 2 pin
extern int LED_PIN; // spirometer led pin
extern int NEOPIXEL_PIN;    // neopixel attachment pin

extern char DHTTYPE;   // DHT11 or DHT22, depends on your sensor
extern int DHTPIN; // DHT Digital Input Pin
extern int PIN_MQ135; // MQ135 Analog Input Pin

// declare pin inputs
extern int pot_val; // potentiometer reading

// declare booleans
extern volatile bool initial_pos_set;   // true if starting positon of motor is correct
extern volatile bool button_state;  // state whether button has been pressed
extern volatile bool cal_execute;   // true if calibration has taken place
extern volatile bool state_1;   // direction state 1 for spirometer interrupt (lead dependant)
extern volatile bool state_2;   // direction state 2 for spirometer interrupt (lead dependant)

// declare variables
extern int start_time;  // start time of the timer using millis()
extern int current_time;    // current time determined using millis()

extern int speed_maxI;  // maximum inward speed of the motor
extern int speed_maxO;  // maximum outward speed of the motor
extern int speed_stop;  // stopped speed - threshold values ± either side

extern int NUM_NEOPIXELS;   // number of neopixels to illuminate on a given blow

extern int count;   // number of low->transitions registered by phototransistor 
extern int direction;    // negative or positive to indicate turbine direction. detemine in/exhale

extern float temperature; // temperature reading from DHT
extern float humidity;  // humidity reading from DHT

extern const int out_spd_arr[3]; // array containing values for low, med, high speed. immutable

extern const char SERVICE_UUID[];   // BLE characteristic package
extern const char CHARACTERISTIC_UUID_1[]; // indidual datatype characteristic to send over BLE - spiro carrier
extern const char CHARACTERISTIC_UUID_2[];  // indidual datatype characteristic to send over BLE - enviro carriER

extern Preferences storage; // Prefernces instance for storage
extern Servo motor; // servo object for motor control
extern Adafruit_NeoPixel neopixels; // neopixel object for live spiro feedback

extern BLECharacteristic *pCharacteristic_1;  // for spirometer characteristic
extern BLECharacteristic *pCharacteristic_2;    // for environment sensor characteristic

extern MQ135 mq135_sensor;  // MQ135 sensor object
extern DHT dht; // DHT sensor object


/**
 * Calibrate the motor engagment time with its selected speeds.
 * 
 * The times for which the motor moves must be calibrated to the different speeds assigned to the 3 motions
 * This was implemented for easy transition between power sources throughout testing and prototyping, given that 
 * diffent motor input voltages will result in different motor speeds for a constant duty cycle.
 *
 * @param j Mode setting from 1-3 that controls motion.
 * @param out_spd Speed of the motor as servo angle.
 * @param interval_time The engagement time of the motor for the given speed.
 */
void calibration(int j, int out_spd);


/**
 * Blink yellow and blue LEDs set number of times to waypoint code execution
 * 
 * @param blink_num The number of cycles to blink the LEDs for
*/
void blink_both(int blink_num);


/**
 * Write the motor engagement times to ESP32 flash memory
 * 
 * The times to engage the motor at the 3 respective speeds must be stored between the event of device power-off
 * As such, the values determined through calibration are written to the memory as integers
 * 
 * @param mode_num The mode calibrated for
 * @param time The time value in ms that the motor must be engaged for to reach full extension at mode_num speed
*/
void store_time(int mode_num, int time);


/**
 * Basic rythmic breathing mode with constant, slow plate motion 
 * 
 * Used primarily to calm user and allow them to regain breath during/following attack
 * Or can be used in preparation for inhaler administering to get lungs 'working'
*/
void mode_basic();


/**
 * Breathing mode following 4-7-8 structure
 * 
 * Inhale for 4 seconds, hold for 7, exhale for 8
 * Proven to help with stress, develop diaphragm control and boost oxygen to brain
*/
void mode_478();


/**
 * Inhaler training mode
 * 
 * Sequence follows what the child would do if they were breathing in preparation for, 
 * and administering, a dose of their inhaler
*/
void mode_inhaler();


/**
 * Handles low -> high ISR of phototransistor 1 (of 2)
 * 
 * Order in which states of phototransistors change determines direction i.e. which one
 * leads the other based off their interrupt order and boolen values of state 1 and 2 
*/
void IRAM_ATTR handleInterrupt1();


/**
 * Handles low -> high ISR of phototransistor 2 (of 2)
 * 
 * (Repeated)
 * Order in which states of phototransistors change determines direction i.e. which one
 * leads the other based off their interrupt order and boolen values of state 1 and 2 
*/
void IRAM_ATTR handleInterrupt2();


/**
 * Initiate BLE connection
 * 
 * BLE used to communicate spirometer readings, environmental sensor data and mechanism feedback
 * data to the users and/or carers phone, which in turn can be relayed to a GP.
*/
void setupBLE();


/**
 * Read dht and mq sensor and write their values to bluetooth
 * 
 * The sensor values are used to communicate the environmental conditions to the user, allowing
 * them to correlate their trends in their spirometer readings with the environmental trends -
 * supporting better understanding.
*/
void update_dht_mq();


/**
 * Function updates neopixels based off spirometer readings of user. Proportional to power
 * 
 * The neopixels are used to visually indicate to the user the strength of their in/exhale
 * value read by the spirometer. The amount of neopixels that turn on is determined by the 
 * difference between the spiro reading and all-time max value the user has recorded, which is
 * written to the flash and updated if broken.
*/
void updateNeopixels();


/**
 * Update spirometer values and assign for bluetooth communication
 * 
 * Bluetooth comms of the environment sensor values is also important and takes place in the 
 * spirometer control file
*/
void update_spiro();
