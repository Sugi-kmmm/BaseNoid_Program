#include "abc_function.hpp"

extern HardwareSerial Debug_Serial(0);  // Use USB
extern HardwareSerial RS485_Serial(1);  // Use IO32, & IO33
extern HardwareSerial Servo_Serial(2);  // Use IO16, & IO17

void ABC::PIN_SETUP(void) {
    pinMode(BATTERY_VOLTAGE_PIN, INPUT);

    pinMode(SWITCH_LED_R_PIN, OUTPUT);
    // ledcSetup(pwmch_R, 12800, 8);
    // ledcAttachPin(SWITCH_LED_R_PIN, pwmch_R);
    // pinMode(SWITCH_LED_G_PIN, OUTPUT);
    // ledcSetup(pwmch_G, 12800, 8);
    // ledcAttachPin(SWITCH_LED_G_PIN, pwmch_G);
    // pinMode(SWITCH_LED_B_PIN, OUTPUT);
    // ledcSetup(pwmch_B, 12800, 8);
    // ledcAttachPin(SWITCH_LED_B_PIN, pwmch_B);
}

void ABC::PW_Switch_LED(uint8_t R, uint8_t G, uint8_t B) {
    ledcWrite(pwmch_R, R);
    ledcWrite(pwmch_G, G);
    ledcWrite(pwmch_B, B);
}

bool ABC::Check_Battery_Voltage(uint8_t R10, uint8_t R11) {
    battery_voltage =
        (3.3 * analogRead(BATTERY_VOLTAGE_PIN) / 4096) * (R10 + R11) / R11;
    if (max_battery_boltage < battery_voltage ||
        battery_voltage < min_battery_voltage) {
        return 1;
    } else {
        return 0;
    }
}

void ABC::Serial_Begin(uint32_t DS_rate, uint32_t RS_rate, uint32_t SS_rate) {
    Debug_Serial.begin(DS_rate);
    RS485_Serial.begin(RS_rate, SERIAL_8N1, U1RXD_PIN, U1TXD_PIN);
    Servo_Serial.begin(SS_rate);
}
