#include "encoder.h"
#include "config.h"

EncoderSystem::EncoderSystem() :
    left_encoder(PA_13, PA_14, NC, 624, QEI::X4_ENCODING),
    right_encoder(PB_12, PB_2, NC, 624, QEI::X4_ENCODING)

{}
void EncoderSystem::init() {
    speed_timer.start();
}
int32_t EncoderSystem::get_left_ticks() { return left_encoder.getPulses(); }
int32_t EncoderSystem::get_right_ticks() { return right_encoder.getPulses(); }