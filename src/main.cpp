#include <Arduino.h>

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
    // PS3.begin();              // Initialize PS4 controller

    BaseNoid_Torque_Flagment = TORQUE_OFF;  // Ensure torque is off initially
    BaseNoid::enable_servo_torque(
        BaseNoid_Torque_Flagment);           // Disable servo torque
    ABC::PW_Switch_LED(false, false, true);  // Blue LED on

    Debug_Serial.println(
        "Press 's' to enter configuration mode, or wait 3 s to start BaseNoid. "
        "You can also start BaseNoid by pressing 'c'.");
    uint32_t start = millis();
    while (millis() - start < 3000) {  // 3 secだけ待つ
        if (Debug_Serial.available()) {
            char ch1 = Debug_Serial.read();
            if (ch1 == 's' || ch1 == 'S') {
                BaseNoid::Configuration_Mode();  // Set BaseNoid to setting mode
            } else if (ch1 == 'c' || ch1 == 'C') {
                break;  // Exit setting mode and start BaseNoid
            } else {
                Debug_Serial.println("Invalid selection. Please try again.");
            }
        }
        delay(100);
    }

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
    char ch1 = Debug_Serial.read();
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

    if (BaseNoid_Torque_Flagment == TORQUE_OFF) {
        ABC::PW_Switch_LED(false, false, true);  // Blue LED on
    } else {
        ABC::PW_Switch_LED(false, true, false);  // Green LED on
    }

    if (ch1 == 'q') {
        if (BaseNoid_Torque_Flagment == TORQUE_ON) {
            BaseNoid_Torque_Flagment = TORQUE_OFF;  // Disable torque
            Debug_Serial.println(
                "[INFO]:PS Button pressed, turned off torque on BaseNoid.");
            BaseNoid::enable_servo_torque(
                BaseNoid_Torque_Flagment);           // Disable servo torque
            ABC::PW_Switch_LED(false, false, true);  // Blue LED on
            delay(1000);  // Wait for 1 second before resetting
        } else {
            BaseNoid_Torque_Flagment = TORQUE_ON;  // Enable torque
            Debug_Serial.println(
                "[INFO]:PS Button pressed, turned on torque on BaseNoid.");
            BaseNoid::enable_servo_torque(
                BaseNoid_Torque_Flagment);           // Disable servo torque
            ABC::PW_Switch_LED(false, true, false);  // Green LED on
            set_initial_position(0.2);
            delay(1000);  // Wait for 1 second before resetting
        }
    }

    if (BaseNoid_Torque_Flagment == TORQUE_OFF && ch1 == 'p') {
        BaseNoid::Configuration_Mode();  // Set BaseNoid to setting mode
    }

    if (ch1 == 'e') {
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
        if (ch1 == 'w') {
            for (int i = 0; i < 10; i++) {
                if (Now_Walking_Flagment == false) {
                    Now_Walking_Flagment = true;
                    Step_Count = 0;  // Reset step count
                }
                Walk(Step_Count, 128, 0);
                Step_Count++;  // Increment step count
            }
        } else {
            if (Now_Walking_Flagment == true) {
                Walk(Step_Count, 0.0, 0.0);  // Stop walking
                delay(1000);  // Wait for 1 second before stopping
                Now_Walking_Flagment = false;
            }
        }

        if (ch1 == 'a') {
            Debug_Serial.println("[INFO]:Triangle button pressed.");
            // If Triangle button is pressed, perform a punch action
            BaseNoid_Sample_Motion::punch();
        }

        if (ch1 == 'b') {
            Debug_Serial.println("[INFO]:Down button pressed.");
            // If Down button is pressed, perform a sit down action
            BaseNoid_Sample_Motion::sit_down();
        }
    }
    ch1 = 0;
    Debug_Serial.printf("ch1: %c\n", ch1);
}