// modes contains the sequence control of the motor for breathing guidance, used by mechanism.cpp

// linking external library dependancies
#include <Servo.h>

// linking local variable and function declarations
#include "headr.h"


void mode_basic() {

    int out_spd = out_spd_arr[1];
    int time = storage.getInt("1", 0);

    motor.write(out_spd);
    delay(time);

    motor.write(90);
    delay(2000);

    motor.write(180 - out_spd);
    delay(time);

    motor.write(90);
    delay(2000);
}


void mode_478() {

}


void mode_inhaler() {

}
