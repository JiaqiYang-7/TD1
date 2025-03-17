#include "motor_controller.h"

EncoderSystem _encoder;

// PID构造函数参数修正
MotorController::MotorController(PinName left_pwm, PinName right_pwm,
                                 PinName left_dir, PinName right_dir)
    : _left_pwm(left_pwm),
      _right_pwm(right_pwm),
      _left_dir(left_dir),
      _right_dir(right_dir),
      // PID(Kc, taul, taulO, interval)
      _left_pid(0.5f, 0.05f, 0.01f, PID_INTERVAL),
      _right_pid(0.5f, 0.05f, 0.01f, PID_INTERVAL),
      _encoder(_encoder)
{
    // // 初始化硬件
    _left_pwm.period(1.0f/PWM_FREQ);
    _right_pwm.period(1.0f/PWM_FREQ);
    _left_pwm = 0.5f;
    _right_pwm = 0.5f;
    _left_dir = 1;
    _right_dir = 1;

    // 配置输入输出范围
    _left_pid.setInputLimits(0.0f, 2.0f);   // 输入速度范围 0-2 m/s
    _left_pid.setOutputLimits(0.0f, 1.0f);  // 输出PWM范围 0-1.0
    _right_pid.setInputLimits(0.0f, 2.0f);
    _right_pid.setOutputLimits(0.0f, 1.0f);

    // 设置自动模式
    _left_pid.setMode(1); // 1=Auto
    _right_pid.setMode(1);
}

void MotorController::setLeftTargetSpeed(float speed) {
    _left_pid.setSetPoint(speed);
}

void MotorController::setRightTargetSpeed(float speed) {
    _right_pid.setSetPoint(speed);
}


void MotorController::update() {
    _left_pid.setProcessValue(_encoder.get_left_speed());
    _right_pid.setProcessValue(_encoder.get_right_speed());
    
    // PID calc
    _left_pwm = _left_pid.compute();
    _right_pwm = _right_pid.compute();
    // _left_pwm = 0.5f;
    // _right_pwm = 0.5f;
    
}
float MotorController::test(){
    return _left_pwm;
}
