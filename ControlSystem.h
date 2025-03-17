// ControlSystem.h
#pragma once
#include "motionFeedback.h"
#include "sensor.h"
#include "motor_controller.h"
#include "PID.h"

class ControlSystem {
public:
    enum State {LINE_FOLLOWING, TURNING, STOPPED};
    
    ControlSystem(MotorController& motor, 
                MotionFeedback& feedback,
                LineSensor& sensor);
    
    void update();
    void emergencyStop();
    void handleBluetoothCommand(const char* cmd);

private:
    MotorController& _motor;
    MotionFeedback& _feedback;
    LineSensor& _sensor;
    State _currentState;
    PID _linePID;
    
    void lineFollowingControl();
    void executeTurn(float degrees);
};
