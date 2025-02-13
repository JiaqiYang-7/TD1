#pragma once
#include "encoder.h"
#include "config.h"

class MovementController {
public:
    MovementController(EncoderSystem& encoder);
    
    void move_forward(float distance);  // meters
    void turn_degrees(float degrees);   // +: clockwise, -: CCW
    void stop();

private:

    EncoderSystem& encoder;
    void maintain_speed(float target_left, float target_right);
};