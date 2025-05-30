#pragma once

#include <Arduino.h>
#include <HardwareSerial.h>

#include "abc_pin_assign.hpp"
#include "battery_info.hpp"

extern HardwareSerial Debug_Serial;  // Use USB
extern HardwareSerial RS485_Serial;  // Use IO32, & IO33
extern HardwareSerial Servo_Serial;  // Use IO16, & IO17

class ABC {
   public:
    void PIN_SETUP(void);
    void PW_Switch_LED(uint8_t R, uint8_t G, uint8_t B);
    bool Check_Battery_Voltage(uint8_t R10, uint8_t R11);
    void Serial_Begin(uint32_t DS_rate, uint32_t RS_rate, uint32_t SS_rate);

   private:
    static constexpr uint8_t pwmch_R = 0;
    static constexpr uint8_t pwmch_G = 1;
    static constexpr uint8_t pwmch_B = 2;
    double battery_voltage = 0;
    double max_battery_boltage = 4.2 * BATTERY_S_NO;
    double min_battery_voltage = 3.5 * BATTERY_S_NO;
};