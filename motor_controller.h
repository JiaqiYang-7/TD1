#pragma once
#include "mbed.h"
#include "PID.h"
#include "encoder.h"
#include "config.h"

static  float PID_INTERVAL = 0.05f;

class MotorController {
public:
    MotorController(PinName left_pwm, PinName right_pwm, 
                    PinName left_dir, PinName right_dir);
    
    void setLeftTargetSpeed(float speed);
    void setRightTargetSpeed(float speed);
    void update();
    float test();
    float getLeftSpeed() const;
    float getRightSpeed() const;

private:
    PwmOut _left_pwm;
    PwmOut _right_pwm;
    DigitalOut _left_dir;
    DigitalOut _right_dir;
    
    PID _left_pid;  // 左轮PID
    PID _right_pid; // 右轮PID
    EncoderSystem& _encoder;

};

