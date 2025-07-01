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
    BaseNoid::Servo_Setup();                 // Initialize servo motors
    PS4.begin();                             // Initialize PS4 controller
    ABC::PW_Switch_LED(false, false, true);  // Blue LED on

    BaseNoid::enable_servo_torque(TORQUE_OFF);  // Disable servo torque
    BaseNoid::Setting_Mode();                   // Set servos to setting mode

    Debug_Serial.println("Launching Servo Motor...");
    BaseNoid::enable_servo_torque(TORQUE_ON);  // Enable servo torque
    // BaseNoid::set_initial_position();// Set initial position of servos
    Debug_Serial.println("Setup complete.");
    Debug_Serial.println("Welcome to BaseNoid!");
    delay(2000);
}

void loop() {
    // Check battery voltage
    if (ABC::Check_Battery_Voltage(1006, 154.6)) {
        ABC::PW_Switch_LED(true, false, false);  // Red LED on
        while (1) {
            Debug_Serial.println("[FATAL]:Battery voltage out of range!");
            delay(1000);
        };
    }

    if (PS4.isConnected()) {
        ABC::PW_Switch_LED(false, true, false);  // Green LED on
        if (abs(PS4.LStickY()) > 30) {
            if (Now_Walking_Flagment == false) {
                Now_Walking_Flagment = true;
                Step_Count = 0;  // Reset step count
                BaseNoid::Get_Ready_to_Walk();
            }
            BaseNoid::Walk(Step_Count, PS4.LStickY(), PS4.LStickX());
        }
        Now_Walking_Flagment = false;
    } else {
        // BaseNoid::set_initial_position();
        Debug_Serial.println("[ERROR]:PS4 controller not connected.");
        ABC::PW_Switch_LED(true, false, true);  // Purple LED on
    }

    // Add your main code here
    // BaseNoid::Walk_algo_test(1, 1);
    BaseNoid::Get_Ready_to_Walk();
    Debug_Serial.println("[INFO] END");
    while (1);

    delay(10);  // Delay for demonstration purposes
}