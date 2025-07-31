#include <Arduino.h>
#include <PS4Controller.h>

#include "abc_function.hpp"
#include "basenoid.hpp"
#include "basenoid_sample_motion.hpp"

static bool Now_Walking_Flagment =
    false;  // Flag to indicate if walking is in progress
static bool Leg_flexed_state = false;  // Flag to indicate if the leg is flexed
static bool BaseNoid_Torque_Flagment = TORQUE_OFF;
static uint32_t Step_Count = 0;  // Initialize step count

void setup() {
    ABC::PIN_SETUP();
    ABC::Serial_Begin(115200, 115200, 1000000);
    BaseNoid::Servo_Setup();  // Initialize servo motors
    PS4.begin();              // Initialize PS4 controller

    BaseNoid_Torque_Flagment = TORQUE_OFF;  // Ensure torque is off initially
    BaseNoid::enable_servo_torque(
        BaseNoid_Torque_Flagment);           // Disable servo torque
    ABC::PW_Switch_LED(false, false, true);  // Blue LED on
    BaseNoid::Setting_Mode();                // Set BaseNoid to setting mode

    Debug_Serial.println("Launching Servo Motor...");
    BaseNoid_Torque_Flagment = TORQUE_ON;  // Enable servo torque
    BaseNoid::enable_servo_torque(
        BaseNoid_Torque_Flagment);        // Enable servo torque
    BaseNoid::set_initial_position(1.0);  // Set initial position of servos
    Debug_Serial.println("Setup complete.");
    Debug_Serial.println("Welcome to BaseNoid!");
    ABC::PW_Switch_LED(false, true, false);  // Green LED on
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
        if (PS4.PSButton()) {
            if (BaseNoid_Torque_Flagment == TORQUE_ON) {
                BaseNoid_Torque_Flagment = TORQUE_OFF;  // Disable torque
                Debug_Serial.println(
                    "[INFO]:PS Button pressed, resetting BaseNoid.");
                BaseNoid::enable_servo_torque(
                    BaseNoid_Torque_Flagment);           // Disable servo torque
                ABC::PW_Switch_LED(false, false, true);  // Blue LED on
                delay(1000);  // Wait for 1 second before resetting
            } else {
                BaseNoid_Torque_Flagment = TORQUE_ON;  // Enable torque
                Debug_Serial.println(
                    "[INFO]:PS Button pressed, resetting BaseNoid.");
                BaseNoid::enable_servo_torque(
                    BaseNoid_Torque_Flagment);           // Disable servo torque
                ABC::PW_Switch_LED(false, true, false);  // Green LED on
                delay(1000);  // Wait for 1 second before resetting
            }
        }

        if (PS4.Circle()) {
            if (Leg_flexed_state == false) {
                Leg_flexed_state = true;
                Get_Ready_to_Walk();
                delay(1000);  // Wait for 1 second before starting to walk
            } else {
                Leg_flexed_state = false;
                set_initial_position(0.2);
                delay(1000);  // Wait for 1 second before stopping
            }
        }

        if (Leg_flexed_state == true) {
            if (abs(PS4.LStickY()) > 30) {
                if (Now_Walking_Flagment == false) {
                    Now_Walking_Flagment = true;
                    Step_Count = 0;  // Reset step count
                }
                Walk(Step_Count, PS4.LStickY(), PS4.LStickX());
                Step_Count++;  // Increment step count
            } else {
                if (Now_Walking_Flagment == true) {
                    Walk(Step_Count, 0.0, 0.0);  // Stop walking
                    delay(1000);  // Wait for 1 second before stopping
                    Now_Walking_Flagment = false;
                }
            }

            if (PS4.Cross()) {
                // If Cross button is pressed, perform a punch action
                BaseNoid_Sample_Motion::punch();
            }
        }
    } else {
        set_initial_position(0.1);
        Debug_Serial.println("[ERROR]:PS4 controller not connected.");
        ABC::PW_Switch_LED(true, false, true);  // Purple LED on
    }
    delay(10);  // Delay for demonstration purposes
}