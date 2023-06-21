// mechanism file contains functions for controlling the breathing mechanism

// linking external library dependancies
#include <Servo.h>


// linking local variable and function declarations
#include "headr.h"


void calibration(int j, int out_spd) {

    // set inward speed based off argument
    int in_spd = 180 - out_spd;

    // j flashes of both - in position setting mode for mode j
    delay(2000);
    blink_both(j);

    if (initial_pos_set == false) {

        // yellow on > time to move inward
        digitalWrite(led_1_pin, HIGH);
        motor.write(in_spd);
        delay(5000);
        digitalWrite(led_1_pin, LOW); 

        // blue on > time to move outward
        digitalWrite(led_2_pin, HIGH);
        motor.write(out_spd);
        delay(5000);
        digitalWrite(led_2_pin, LOW);

        motor.write(90);
        delay(1000);

        initial_pos_set = true;
    }

    int interval_time = 0;
    bool looper = true;

    // countdown light sequence
    for (int i = 0; i < 3; i++) {

        digitalWrite(led_1_pin, HIGH);
        delay(500);

        digitalWrite(led_1_pin, LOW);
        delay(500);  
    }

    // start inidicator light, motor and timer
    digitalWrite(led_2_pin, HIGH);
    delay(100);
    digitalWrite(led_2_pin, LOW);

    motor.write(out_spd);
    start_time = millis();

    while (looper == true) {

        // Serial.println("in looper loop");
        
        // poll for button press
        if (analogRead(pot_pin) != 0) {

            // stop motor
            motor.write(90);

            // record time
            current_time = millis();
            interval_time = current_time - start_time;

            store_time(j, interval_time);
            // Serial.print("Interval time: ");
            // Serial.println(interval_time);

            // motor is stopped - returning to origin
            delay(1000);
            motor.write(in_spd);
            // Serial.println(interval_time);
            delay(interval_time);
            motor.write(90);

            looper = false;
        }
    }
}


void blink_both(int blink_num) {

    for (int i = 0; i < blink_num; i++) {

        digitalWrite(led_1_pin, HIGH);
        digitalWrite(led_2_pin, HIGH);
        delay(500);

        digitalWrite(led_1_pin, LOW);
        digitalWrite(led_2_pin, LOW);
        delay(500);  
    }
}
