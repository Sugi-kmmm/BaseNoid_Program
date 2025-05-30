#pragma once

#include <stdint.h>

const int16_t initial_angle = 2047;
const uint16_t initial_speed = 3400;

struct Servo {
    enum Name {
        WAIST,             // 0番目
        L_SHOULDER_PITCH,  // 1番目
        L_SHOULDER_ROLL,   // 2番目
        L_ELBOW_PITCH,     // 3番目
        R_SHOULDER_PITCH,  // 4番目
        R_SHOULDER_ROLL,   // 5番目
        R_ELBOW_PITCH,     // 6番目
        R_HIP_ROLL,        // 7番目
        R_HIP_PITCH,       // 8番目
        R_HIP_YAW,         // 9番目
        R_KNEE_PITCH,      // 10番目
        R_ANKLE_ROLL,      // 11番目
        R_ANKLE_PITCH,     // 12番目
        L_HIP_ROLL,        // 13番目
        L_HIP_PITCH,       // 14番目
        L_HIP_YAW,         // 15番目
        L_KNEE_PITCH,      // 16番目
        L_ANKLE_ROLL,      // 17番目
        L_ANKLE_PITCH,     // 18番目
        DEGREE_OF_FREEDOM  // 配列のサイズを管理（19）
    };
    uint8_t ID;
    int16_t angle;
    uint16_t speed;
    int16_t min_angle;
    int16_t max_angle;
};

// 配列のサイズを自動管理
Servo SERVO_ARRAY[Servo::DEGREE_OF_FREEDOM] = {
    {1, initial_angle, initial_speed, 1024, 3072},
    {11, initial_angle, initial_speed, 1024, 4095},
    {12, initial_angle, initial_speed, 2047, 4095},
    {13, initial_angle, initial_speed, 2047, 4000},

    {21, initial_angle, initial_speed, 0, 3072},
    {22, initial_angle, initial_speed, 0, 2047},
    {23, initial_angle, initial_speed, 94, 2047},

    {31, initial_angle, initial_speed, 1024, 2303},
    {32, initial_angle, initial_speed, 512, 2500},
    {33, initial_angle, initial_speed, 1024, 3072},
    {34, initial_angle, initial_speed, 256, 2186},
    {35, initial_angle, initial_speed, 1024, 3072},
    {36, initial_angle, initial_speed, 1024, 3072},

    {41, initial_angle, initial_speed, 1791, 3072},
    {42, initial_angle, initial_speed, 1594, 3582},
    {43, initial_angle, initial_speed, 1024, 3072},
    {44, initial_angle, initial_speed, 1913, 3838},
    {45, initial_angle, initial_speed, 1024, 3072},
    {46, initial_angle, initial_speed, 1024, 3072}};
