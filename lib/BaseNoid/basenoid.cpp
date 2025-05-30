#include "BaseNoid/basenoid.hpp"

bool BaseNoid::check_range(Servo SERVO_ARRAY[Servo::DEGREE_OF_FREEDOM]) {
    for (int i = 0; i < Servo::DEGREE_OF_FREEDOM; ++i) {
        if (SERVO_ARRAY[i].angle < SERVO_ARRAY[i].min_angle ||
            SERVO_ARRAY[i].angle > SERVO_ARRAY[i].max_angle) {
            return 1;
        }
    }
    return 0;
}

void BaseNoid::enable_servo_torque(bool torque_enable) {
    for (int i = 0; i < Servo::DEGREE_OF_FREEDOM; ++i) {
        STS3215.EnableTorque(SERVO_ARRAY[i].ID, torque_enable);
    }
}

int16_t BaseNoid::angle(double rad) { return 4095 * rad / (2 * PI); }