#pragma once
#include "mbed.h"
#include "QEI.h"
#include "config.h"

class EncoderSystem {
public:
    EncoderSystem();
    void init();
    float get_left_speed();  // m/s
    float get_right_speed();
    int32_t get_left_ticks();
    int32_t get_right_ticks();

private:
    QEI left_encoder;
    QEI right_encoder;
    Ticker velocity_ticker;
    Timer speed_timer;

    volatile int32_t prev_left;
    volatile int32_t prev_right;
    volatile float left_velocity;
    volatile float right_velocity;

    void calculate_velocity();
};
