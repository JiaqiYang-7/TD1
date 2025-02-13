#include "movement.h"
#include "config.h"
#include "mbed.h"
#include "pid_controller.h"


MovementController::MovementController(EncoderSystem& e) 
    : encoder(e),
{}

void MovementController::move_forward(float distance) {
    const float target_ticks = (distance / (M_PI*WHEEL_DIAMETER)) * 624;
    int32_t start_left = encoder.get_left_ticks();
    int32_t start_right = encoder.get_right_ticks();
    
    while(true) {
        if (start_left < target_ticks - 10){
            
        }

    }
    stop();
}

void MovementController::turn_degrees(float degrees) {
    const float rad = degrees * M_PI / 180.0f;
    const float target_diff = (WHEEL_BASE * rad) / (WHEEL_DIAMETER * M_PI) * EFFECTIVE_CPR;
    
    int32_t start_left = encoder.get_left_ticks();
    int32_t start_right = encoder.get_right_ticks();
    
    while(true) {
        int32_t delta_left = encoder.get_left_ticks() - start_left;
        int32_t delta_right = encoder.get_right_ticks() - start_right;
        float error = target_diff - (delta_left - delta_right);
        
        float control = left_pid.calculate(error, 0.01f);
        motor.set_left_speed(TURN_PWM + control);
        motor.set_right_speed(TURN_PWM - control);
        
        if(fabs(error) < 100) break;
        wait_ms(10);   // 10ms
    }
    stop();
}

void MovementController::stop() {
    motor.stop();
    left_pid.reset();
    right_pid.reset();
}