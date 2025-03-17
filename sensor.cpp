#include "sensor.h"
#include "config.h"

LineSensor::LineSensor(PinName pins[Config::SENSOR_COUNT], C12832& lcd) 
    : _lcd(lcd), _lostCount(0) {
    for(int i = 0; i < Config::SENSOR_COUNT; i++) {
        _sensors[i] = new AnalogIn(pins[i]);
    }
    for(int i = 0; i < Config::SENSOR_COUNT; i++) {
        _sensorValues[i] = 0.0f;
    }
}

void LineSensor::update() {
    bool trackDetected = false;
    for(int i=0; i<Config::SENSOR_COUNT; i++) {
        _sensorValues[i] = _sensors[i]->read()*5; // 5V参考电压
        if(_sensorValues[i] > Config::BLACK_THRESHOLD) trackDetected = true;
    }
    _lostCount = trackDetected ? 0 : _lostCount+1;
}

bool LineSensor::isOnTrack() const {
    return _lostCount < Config::LINE_BREAK_TOLERANCE;
}

float LineSensor::calculateDeviation() {
    // Weighted average algorithm to calculate deviation values 
    float sum = 0, weight = 0;
    const float positions[6] = {-0.5f, -0.3f, -0.1f, 0.1f, 0.3f, 0.5f};
    for(int i=0; i<Config::SENSOR_COUNT; i++) {
        float activation = (_sensorValues[i] - Config::BLACK_THRESHOLD) / 
                          (Config::WHITE_THRESHOLD - Config::BLACK_THRESHOLD);
        activation = activation > 1.0f ? 1.0f : activation < 0.0f ? 0.0f : activation;
        sum += positions[i] * activation;
        weight += activation;
    }
    return weight > 0.1f ? sum/weight : 0.0f;
}

void LineSensor::drawSensorStatus(){
    // display param
const int CIRCLE_RADIUS = 8;         
const int CIRCLE_SPACING = 21;       
const int START_X = 10;              // first circle x location
const int CENTER_Y = 16;             // circle center location
  
    _lcd.cls();
    for(int i=0; i<Config::SENSOR_COUNT; i++) {
        // palce circle center location
        int x = START_X + i*CIRCLE_SPACING;
        
        // read sensor value
        float voltage = _sensors[i]->read()*5;
        
        // plot circle
        if(voltage < Config::WHITE_THRESHOLD && voltage > Config::BLACK_THRESHOLD) {
            _lcd.fillcircle(x, CENTER_Y, CIRCLE_RADIUS, 1); // black circle
        } else if(voltage > Config::WHITE_THRESHOLD) {
            _lcd.circle(x, CENTER_Y, CIRCLE_RADIUS, 1);     // white circle
        } else {
            _lcd.rect(x-CIRCLE_RADIUS, CENTER_Y-CIRCLE_RADIUS,
                     x+CIRCLE_RADIUS, CENTER_Y+CIRCLE_RADIUS, 1); // gray square
        }
    }
    _lcd.copy_to_lcd(); // refresh display

}