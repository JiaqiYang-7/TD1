#pragma once
#include "mbed.h"
#include "QEI.h"
#include "config.h"

class EncoderSystem {
public:
    EncoderSystem();
    void init();
    int32_t get_left_ticks();
    int32_t get_right_ticks();

private:
    QEI left_encoder;
    QEI right_encoder;
    Ticker velocity_ticker;
    Timer speed_timer;
};
