// LineSensor.h
#pragma once
#include "mbed.h"
#include "config.h"
#include "C12832.h"

class LineSensor {
public:
    LineSensor(PinName pins[Config::SENSOR_COUNT], C12832& lcd);
    void update();
    float* getSensorValues();
    bool isOnTrack() const;
    float calculateDeviation();
    void drawSensorStatus();

private:
    AnalogIn* _sensors[Config::SENSOR_COUNT];
    float _sensorValues[Config::SENSOR_COUNT];
    C12832& _lcd;
    int _lostCount;
};
