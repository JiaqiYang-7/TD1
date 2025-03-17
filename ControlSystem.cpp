// ControlSystem.cpp
#include "ControlSystem.h"

ControlSystem::ControlSystem(MotorController& motor,
                           MotionFeedback& feedback,
                           LineSensor& sensor)
    : _motor(motor),
      _feedback(feedback),
      _sensor(sensor),
      _currentState(LINE_FOLLOWING),
      _linePID(Config::LINE_KP, Config::LINE_KI, Config::LINE_KD, Config::PID_INTERVAL)
{
    _linePID.setInputLimits(-1.0f, 1.0f);
    _linePID.setOutputLimits(-Config::MAX_SPEED/2, Config::MAX_SPEED/2);
    _linePID.setMode(1);// auto
}

void ControlSystem::update() {
    _sensor.update();
    _feedback.update();
    
    if(!_sensor.isOnTrack()) {
        emergencyStop();
        return;
    }
    
    switch(_currentState) {
        case LINE_FOLLOWING:
            lineFollowingControl();
            break;
        case TURNING:
            // 转弯控制逻辑
            break;
        case STOPPED:
            _motor.setLeftTargetSpeed(0);
            _motor.setRightTargetSpeed(0);
            break;
    }
    _motor.update();
}

void ControlSystem::lineFollowingControl() {
    float deviation = _sensor.calculateDeviation();
    _linePID.setProcessValue(deviation);
    _linePID.setSetPoint(0.0f);  
    float correction = _linePID.compute();
    
    float baseSpeed = Config::MAX_SPEED * Config::SLOPE_COMPENSATION;
    _motor.setLeftTargetSpeed(baseSpeed - correction);
    _motor.setRightTargetSpeed(baseSpeed + correction);
}

void ControlSystem::emergencyStop(){
    
}
