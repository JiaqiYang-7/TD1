#pragma once
#include <cmath>

//math constant
#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif
// // encoder pins
// #define LEFT_ENC_A      PC_1
// #define LEFT_ENC_B      PC_0
// #define RIGHT_ENC_A     PA_2
// #define RIGHT_ENC_B     PA_3
#define PWM_FREQ        200000      // PWM frequency (Hz)
namespace Config {
    //sensor
    const int SENSOR_COUNT = 6;
    const float BLACK_THRESHOLD = 0.3f;
    const float WHITE_THRESHOLD = 2.0f;
    // Motion parameters (requires calibration based on actual testing)


    // paras for movement control
    const float WHEEL_DIAMETER = 0.08f;   // (m)
    const float WHEEL_BASE = 0.185f;        // wheel distance(m)  
    const float GEAR_RATIO = 18.75;             
    const float MAX_SPEED = 0.8f; 
    // PID para
    const float PID_INTERVAL = 0.05f;
    const float SAFETY_TIMEOUT = 2.0f;
    const float LINE_KP = 0.35f;
    const float LINE_KI = 0.01f;
    const float LINE_KD = 0.05f;


    const int LINE_BREAK_TOLERANCE = 30;  // 允许的连续断线检测次数（6mm断线对应约30个采样点）  
    const float SLOPE_COMPENSATION = 1.15f; // 坡度补偿系数
    
}
