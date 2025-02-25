#pragma once
#include <cmath>

//math constant
#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif
// encoder pins
#define LEFT_ENC_A      PC_1
#define LEFT_ENC_B      PC_0
#define RIGHT_ENC_A     PA_2
#define RIGHT_ENC_B     PA_3
                 // encoder CPR

// Motion parameters (requires calibration based on actual testing)
#define PWM_FREQ        200000      // PWM frequency (Hz)
#define STRAIGHT_TIME   2.2f    // Time to move 0.5m straight (s)
#define TURN_90_TIME    0.8f    // Time for 90° turn (s)
#define TURN_180_TIME   1.6f    // Time for 180° turn (s)

// paras for movement control
const float WHEEL_DIAMETER = 0.08f;   // (m)
const float WHEEL_BASE = 0.185f;        // wheel distance(m)  
const float GEAR_RATIO = 18.75;             
