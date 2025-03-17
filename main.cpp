#include "mbed.h"
#include "config.h"
#include "sensor.h"
#include "motionFeedback.h"
#include "ControlSystem.h"
#include "motor_controller.h"

C12832 lcd(D11,D13,D12,D7,D10);
PinName sensors[Config::SENSOR_COUNT] = {A5, A4, A3, A2, PC_3, PB_1};

DigitalOut enable(PC_5);   // Motor enable
DigitalOut left_mode(PB_13);// Left motor direction
DigitalOut right_mode(PB_15);// Right motor direction
InterruptIn joy_fire(D4);
DigitalIn joy_up(A2);
void setup_motors() {
    enable = 1;                 // Enable motor driver
    left_mode.write(0);
    right_mode.write(0);
}
int main() {
    EncoderSystem encoder;
    MotorController motor(PC_6, PC_8, PB_14, PB_1);
    LineSensor lineSensor(sensors, lcd);
    MotionFeedback feedback(encoder);
    ControlSystem controller(motor, feedback, lineSensor);
    setup_motors();

    encoder.init();
    motor.setLeftTargetSpeed(0.5);
    motor.setRightTargetSpeed(0.5);

    // 主循环
    while(1) {
        controller.update();
        // lineSensor.drawSensorStatus();
        
        // 显示实时速度
        lcd.locate(0,0);
        lcd.printf("L:%.2f R:%.2f", 
                  encoder.get_left_speed(),
                  encoder.get_right_speed());
        
        lcd.locate(0,10);
        lcd.printf("Ld:%.2f Rd:%.2f", 
                  motor.test(),
                  encoder.get_right_speed());
        wait(0.05);
    }
}

