#include "mbed.h"
#include "encoder.h"
#include "config.h"
#include "C12832.h"
#include "potentiometer.h"

C12832 lcd(D11,D13,D12,D7,D10);
PwmOut right_pwm(PC_8);     // Left motor PWM
PwmOut left_pwm(PC_6);    // Right motor PWM
DigitalOut enable(PC_5);   // Motor enable
DigitalOut left_mode(PB_13);// Left motor direction
DigitalOut right_mode(PB_15);// Right motor direction
InterruptIn joy_fire(D4);
DigitalOut right_dir(PB_1);
DigitalOut left_dir(PB_14);
DigitalIn joy_up(A2);
EncoderSystem encoder;

SamplingPotentiometer leftPotentiometert(A0, 1.0, 10);
SamplingPotentiometer rightPotentiometer(A1, 1.0, 10);

volatile int modes = 0;
// void update_lcd() {
//     lcd.locate(10, 0);
//     lcd.printf("Left pulses: %i\n", encoder.get_left_ticks());
//     lcd.locate(10, 20);
//     lcd.printf("left speed: %f\n", left_speed);
//     lcd.locate(10, 10);
//     lcd.printf("Right pulses: %i\n", encoder.get_right_ticks());
// }
void fireISR()  //toggle motor mode                        //ISR executed each time fire is pressed. Used to advance program state.
    {
    modes = !modes;
    right_mode.write(modes);
    left_mode.write(modes);
    lcd.cls();
    }
void speed_control() {
    volatile float left_speed;
    volatile float right_speed;
    if (modes == 0){
        left_speed = 1 - leftPotentiometert.getCurrentSampleVolts();
        right_speed = 1 - rightPotentiometer.getCurrentSampleVolts();
    }
    if (modes ==  1) {
        left_speed = leftPotentiometert.getCurrentSampleVolts();
        right_speed = rightPotentiometer.getCurrentSampleVolts();
    }
    if (modes == 0){
        lcd.printf("M: Uni");
    }
    if (modes ==  1) {
        lcd.printf("M: Bi");
    }
    right_pwm.write(right_speed);
    left_pwm.write(left_speed);
    lcd.locate(0,0);
    lcd.printf("Set L: %.2f", left_speed);
    lcd.locate(55,0);
    lcd.printf("Set R: %.2f", right_speed);
    
    // lcd.printf("L Pul: %i", encoder.get_left_ticks());
    // lcd.locate(55,10);
    // lcd.printf("R Pul: %i", encoder.get_right_ticks());
    // lcd.locate(0,20);
    // lcd.printf("R l: %0.2f", left_pwm.read());
    // lcd.locate(55,20);
    // // lcd.printf("R R: %0.2f", right_pwm.read());
    // lcd.printf("M: %d", left_mode.read());
    // lcd.locate(90,20);
    lcd.locate(0,10);
    lcd.printf("l v: %.3f", encoder.get_left_speed());
    lcd.locate(40, 10);
    lcd.printf("r v: %.3f", encoder.get_right_speed());

    }

void move_forward(float distance) {
    int32_t initial_left = encoder.get_left_ticks();
    int32_t initial_right = encoder.get_right_ticks();
    int target_ticks = 1.1*((distance / (M_PI*WHEEL_DIAMETER)) * 624);
    int target_left = initial_left + target_ticks;
    int target_right = initial_right + target_ticks;
    left_mode.write(0);
    right_mode.write(0);
    left_dir.write(1);
    right_dir.write(1);//right.dir  = 1;

    while(1) {
        int current_l = encoder.get_left_ticks();
        int current_r = encoder.get_right_ticks();
        lcd.locate(0, 0);

        lcd.printf("target:%i", target_ticks);
        lcd.locate(0,10);
        lcd.printf("L Pul: %i",current_l);
        lcd.locate(55,10);
        lcd.printf("R Pul: %i", current_r);
        float left_error_f = abs(target_left - current_l);
        float right_error_f = abs(target_right - current_r);
        float kp_f = 0.00005f;

        if (current_l < target_left){
            float left_pwm_val = (0.60 - kp_f * left_error_f);
            left_pwm.write(left_pwm_val);
        } else {
            left_pwm.write(1);
        }

        if (current_r < target_right){
            float right_pwm_val = (0.5 - kp_f * right_error_f);
            right_pwm.write(right_pwm_val);
        } else {
            right_pwm.write(1);
        }
        if (current_l >= target_left && current_r >= target_right){
            wait(0.5);
            break;
            }
    }
}

void rotate_degrees(float degrees) {
    left_mode.write(0);
    right_mode.write(0);
    // calc arc length (arc_length = θ * (wheelbase/2))
    float radians = fabs(degrees) * M_PI / 180.0f;
    float arc_length = radians * (WHEEL_BASE / 2.0f);
   
    // calc pulse target
    int pulse_target = 1.3*((arc_length / (M_PI * WHEEL_DIAMETER)) * 624);
    // lcd.printf("target:%i", pulse_target);
    int32_t initial_left = encoder.get_left_ticks();
    int32_t initial_right = encoder.get_right_ticks();
   

    int left_target, right_target;
    if (degrees > 0) { // clockwise
        
        left_dir.write(1);   // left wheel front
        right_dir.write(0);  // right inverse
        left_target = initial_left + pulse_target;
        right_target = initial_right - pulse_target;
    } else {          // anticlockwise
        left_dir.write(0);   //  left inverse
        right_dir.write(1);  // right front
        left_target = initial_left - pulse_target;
        right_target = initial_right + pulse_target;
    }
    // control loop
    while (true) {
        int32_t current_left = encoder.get_left_ticks();
        int32_t current_right = encoder.get_right_ticks();
        // lcd.locate(0, 0);
        // lcd.printf("l tar: %i", left_target);
        // lcd.locate(0, 10);
        // lcd.printf("r tar: %i", right_target);
        // lcd.locate(60, 0);
        // lcd.printf("l du: %f", left_pwm.read());
        // lcd.locate(60, 10);
        // lcd.printf("r du: %f", right_pwm.read());
        // lcd.locate(0, 20);
        // lcd.printf("l cur: %i", current_left);
        // lcd.locate(60, 20);
        // lcd.printf("r cur: %i", current_right);
        float left_error = abs(left_target - current_left);
        float right_error = abs(right_target - current_right);
        float kp = 0.0003f;
        // adjusting pwm
        float left_pwm_val = (0.65 - kp * left_error);
        float right_pwm_val = (0.65 - kp * right_error);
        left_pwm.write(left_pwm_val);
        right_pwm.write(right_pwm_val);


        bool left_done = (degrees > 0) ? (current_left >= left_target - 5) 
                                      : (current_left <= left_target + 5);
        bool right_done = (degrees > 0) ? (current_right <= right_target + 5)
                                       : (current_right >= right_target - 5);
        
        if (left_done && right_done) {
            left_pwm.write(1);
            right_pwm.write(1);
            wait(1);
            break;
        }
        
    }
}
void setup_motors() {
    enable = 1;                 // Enable motor driver
    left_pwm.period(1.0/PWM_FREQ);  // Set PWM period
    right_pwm.period(1.0/PWM_FREQ);
    left_mode.write(0);
    right_mode.write(0);
}


int main() {
    setup_motors();
    encoder.init();
    joy_fire.rise(&fireISR);
    // speed_control();
    lcd.cls();
    while (1) {
        speed_control();
    if (joy_up == 1) {
        lcd.cls();
        setup_motors();
        break;}
    }
    // first square
    for (int i = 0; i < 4; i++) {
        lcd.locate(0, 20);
        lcd.printf("i = %i", i);
        move_forward(0.5);  // forward 50 cm
        rotate_degrees(90);
    }
    // turn 180
    rotate_degrees(130);

    // inverse square
    for (int i = 0; i < 4; i++) {
        move_forward(0.5);
        rotate_degrees(-110);
    }

    while(1){}  // stop
}



// #include "mbed.h"
// #include "C12832.h"  

// C12832 lcd(D11,D13,D12,D7,D10);
// const int SENSOR_COUNT = 6;
// AnalogIn sensors[SENSOR_COUNT] = {A5, A4, A3, A2, PC_3, PC_2};

// // display param
// const int CIRCLE_RADIUS = 8;         
// const int CIRCLE_SPACING = 21;       
// const int START_X = 10;              // first circle x location
// const int CENTER_Y = 16;             // circle center location

// const float BLACK_THRESHOLD = 0.3f;
// const float WHITE_THRESHOLD = 2.0f;

// void draw_sensor_status() {   
//     lcd.cls();
//     for(int i=0; i<SENSOR_COUNT; i++) {
//         // palce circle center location
//         int x = START_X + i*CIRCLE_SPACING;
        
//         // read sensor value
//         float voltage = sensors[i].read()*5;
        
//         // plot circle
//         if(voltage < WHITE_THRESHOLD && voltage > BLACK_THRESHOLD) {
//             lcd.fillcircle(x, CENTER_Y, CIRCLE_RADIUS, 1); // black circle
//         } else if(voltage > WHITE_THRESHOLD) {
//             lcd.circle(x, CENTER_Y, CIRCLE_RADIUS, 1);     // white circle
//         } else {
//             lcd.rect(x-CIRCLE_RADIUS, CENTER_Y-CIRCLE_RADIUS,
//                      x+CIRCLE_RADIUS, CENTER_Y+CIRCLE_RADIUS, 1); // gray square
//         }
//     }
//     lcd.copy_to_lcd(); // refresh display
// }

// int main() {
//     lcd.cls();
//     while(1) {
//         draw_sensor_status();
//         wait(0.1); 
//     }
// }