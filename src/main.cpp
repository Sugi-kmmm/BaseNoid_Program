#include <Arduino.h>
#include <PS4Controller.h>

#include "abc_function.hpp"
#include "basenoid.hpp"

static bool Now_Walking_Flagment =
    false;                       // Flag to indicate if walking is in progress
static uint32_t Step_Count = 0;  // Initialize step count

void setup() {
    ABC::PIN_SETUP();
    ABC::Serial_Begin(115200, 115200, 1000000);
    BaseNoid::Servo_Setup();  // Initialize servo motors
    PS4.begin();              // Initialize PS4 controller

    BaseNoid::enable_servo_torque(TORQUE_OFF);  // Disable servo torque
    ABC::PW_Switch_LED(false, false, true);     // Blue LED on
    BaseNoid::Setting_Mode();                   // Set servos to setting mode

    Debug_Serial.println("Launching Servo Motor...");
    BaseNoid::enable_servo_torque(TORQUE_ON);  // Enable servo torque
    BaseNoid::set_initial_position(1.0);       // Set initial position of servos
    Debug_Serial.println("Setup complete.");
    Debug_Serial.println("Welcome to BaseNoid!");
    delay(2000);
}

void loop() {
    using namespace BaseNoid;
    // Check battery voltage
    if (ABC::Check_Battery_Voltage(1006, 154.6)) {
        ABC::battery_voltage_count++;
        if (ABC::battery_voltage_count > 10) {
            ABC::PW_Switch_LED(true, false, false);  // Red LED on
            Debug_Serial.println("[FATAL]:Battery voltage out of range!");
            while (1) {
                ABC::PW_Switch_LED(true, false, false);  // Red LED on
                delay(1000);
                ABC::PW_Switch_LED(false, false, false);  // All LEDs off
                delay(1000);
            };
        }
    } else {
        ABC::battery_voltage_count = 0;  // Reset battery voltage count
    }

    if (PS4.isConnected()) {
        ABC::PW_Switch_LED(false, true, false);  // Green LED on
        if (abs(PS4.LStickY()) > 30) {
            if (Now_Walking_Flagment == false) {
                Now_Walking_Flagment = true;
                Step_Count = 0;  // Reset step count
                // Get_Ready_to_Walk();
                Walk_algo_test(PS4.LStickX(), PS4.LStickY());
            }
            Walk(Step_Count, PS4.LStickY(), PS4.LStickX());
        } else {
            Now_Walking_Flagment = false;
            set_initial_position(0.2);
        }
    } else {
        set_initial_position(0.5);
        Debug_Serial.println("[ERROR]:PS4 controller not connected.");
        ABC::PW_Switch_LED(true, false, true);  // Purple LED on
    }
    delay(10);  // Delay for demonstration purposes
}