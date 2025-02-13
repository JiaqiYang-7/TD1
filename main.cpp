#include "mbed.h"
#include "encoder.h"
#include "config.h"
// #include "C12832.h"
#include "mbed2/299/TARGET_NUCLEO_F401RE/TARGET_STM/TARGET_STM32F4/TARGET_NUCLEO_F401RE/PinNames.h"
#include "mbed2/299/platform/wait_api.h"
#include "potentiometer.h"

// C12832 lcd(D11,D13,D12,D7,D10);
PwmOut right_pwm(PC_8);     // Left motor PWM
PwmOut left_pwm(PC_6);    // Right motor PWM
DigitalOut enable(PC_5);   // Motor enable
DigitalOut left_mode(PA_4);// Left motor direction 
DigitalOut right_mode(PA_0);// Right motor direction
DigitalIn firebutton(D4);

// EncoderSystem encoder; 

SamplingPotentiometer leftPotentiometer(A0, 3.3, 10);
SamplingPotentiometer rightPotentiometer(A1, 3.3, 10);
float left_speed = 0;
float right_speed = 0;

// void update_lcd() {
//     lcd.locate(10, 0);
//     lcd.printf("Left pulses: %i\n", encoder.get_left_ticks());
//     lcd.locate(10, 20);
//     lcd.printf("left speed: %f\n", left_speed);
//     lcd.locate(10, 10);
//     lcd.printf("Right pulses: %i\n", encoder.get_right_ticks());
// }

void speed_control() {
    while (1) {
        left_mode = 1;
        right_mode = 1;
        left_speed = static_cast<float>(leftPotentiometer.getCurrentSampleVolts()/3.3f*1);
        right_speed = static_cast<float>(rightPotentiometer.getCurrentSampleVolts()/3.3f*1);
        left_pwm.write(left_speed);
        right_pwm.write(right_speed);
        // update_lcd();
        if (firebutton == 1){
            left_pwm.write(1);
            right_pwm.write(1);
            break;
            }
    }
}


void move_forward(float distance) {
    const float target_ticks = (distance / (M_PI*WHEEL_DIAMETER)) * 624;
    int32_t start_left = encoder.get_left_ticks();
    int32_t start_right = encoder.get_right_ticks();
    left_mode = 1;
    right_mode = 1;
    while(true) {
        if (start_left < target_ticks - 10){
            left_pwm.write(0.7);
        } else {
            left_pwm.write(0.5);
        }

        if (start_right < target_ticks - 10){
            right_pwm.write(0.7);
        } else {
            right_pwm.write(0.5);
        }
        if (start_left == start_right == target_ticks) break;
        wait_ms(5);
    }
}

void setup_motors() {
    enable = 1;                 // Enable motor driver
    left_pwm.period(1.0/PWM_FREQ);  // Set PWM period
    right_pwm.period(1.0/PWM_FREQ);
}


int main() {
    setup_motors();
    float left_speed;
    float right_speed;
    // encoder.init();
    // potentiometerTicker.attach(speed_control(), 0.1f);
    
    speed_control();
    // first square
    // for (int i = 0; i < 4; i++) {
    //     move_forward(STRAIGHT_TIME);  // forward 50 cm
    //     turn_right(TURN_90_TIME);     // turn right 90°
    // }

    // // 180° 
    // turn_right(TURN_180_TIME);

    // // inverse square
    // for (int i = 0; i < 4; i++) {
    //     move_forward(STRAIGHT_TIME);
    //     turn_left(TURN_90_TIME);
    // }

    while(1){}  // stop
}
