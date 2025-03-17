// MotionFeedback.cpp
#include "motionFeedback.h"

MotionFeedback::MotionFeedback(EncoderSystem& encoder) 
    : _encoder(encoder), _totalDistance(0.0f) {
    _distanceTimer.start();
}

void MotionFeedback::update() {
    float dt = _distanceTimer.read();
    _totalDistance += (_encoder.get_left_speed() + _encoder.get_right_speed()) / 2 * dt;
    _distanceTimer.reset();
}

float MotionFeedback::getTravelDistance() const {
    return _totalDistance;
}
