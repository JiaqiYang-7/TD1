#pragma once
#include "encoder.h"
#include "config.h"

class MotionFeedback {
public:
    explicit MotionFeedback(EncoderSystem& encoder);
    void update();
    float getTravelDistance() const;
    
private:
    EncoderSystem& _encoder;
    float _totalDistance;
    Timer _distanceTimer;
};
