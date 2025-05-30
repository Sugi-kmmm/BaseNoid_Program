#pragma once

#include <Arduino.h>
#include <HardwareSerial.h>
#include <SCServo.h>

#include <vector>

#include "ABC/abc_function.hpp"
#include "BaseNoid/dof_definition.hpp"

#ifndef TORQUE_ON
#define TORQUE_ON 1
#endif

#ifndef TORQUE_OFF
#define TORQUE_OFF 0
#endif

SMS_STS STS3215;

class BaseNoid {
   public:
    bool check_range(Servo SERVO_ARRAY[Servo::DEGREE_OF_FREEDOM]);
    void enable_servo_torque(bool torque_enable);
    int16_t angle(double rad);

   private:
};