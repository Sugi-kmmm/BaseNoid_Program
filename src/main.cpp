#include <Arduino.h>

#include "abc_function.hpp"
#include "basenoid.hpp"
#include "basenoid_sample_motion.hpp"

static bool Now_Walking_Flagment =
    false;  // Flag to indicate if walking is in progress
static bool Leg_flexed_state = false;  // Flag to indicate if the leg is flexed
static bool BaseNoid_Torque_Flagment = TORQUE_OFF;
static uint32_t Step_Count = 0;  // Initialize step count

void BaseNoid_Process(ControllerPtr ctl);

void setup() {
    using namespace ABC;
    using namespace BaseNoid;
    PIN_SETUP();
    Serial_Begin(115200, 115200, 1000000);
    Servo_Setup();  // Initialize servo motors

    // Setup the Bluepad32
    BluePad32_Controller_Setup(false);  // Enable touchpad and mouse support

    BaseNoid_Torque_Flagment = TORQUE_OFF;  // Ensure torque is off initially
    enable_servo_torque(BaseNoid_Torque_Flagment);  // Disable servo torque
    PW_Switch_LED(false, false, true);              // Blue LED on

    Debug_Serial.println(
        "Press 's' to enter configuration mode, or wait 3 s to start BaseNoid. "
        "You can also start BaseNoid by pressing 'c'.");
    uint32_t start = millis();
    while (millis() - start < 3000) {  // 3 secだけ待つ
        if (Debug_Serial.available()) {
            char ch1 = Debug_Serial.read();
            if (ch1 == 's' || ch1 == 'S') {
                Configuration_Mode();  // Set BaseNoid to setting mode
            } else if (ch1 == 'c' || ch1 == 'C') {
                break;  // Exit setting mode and start BaseNoid
            } else {
                Debug_Serial.println("Invalid selection. Please try again.");
            }
        }
        delay(100);
    }

    Debug_Serial.println("Launching Servo Motor...");
    BaseNoid_Torque_Flagment = TORQUE_ON;           // Enable servo torque
    enable_servo_torque(BaseNoid_Torque_Flagment);  // Enable servo torque
    set_initial_position(1.0);  // Set initial position of servos
    Debug_Serial.println("Setup complete.");
    Debug_Serial.println("Welcome to BaseNoid!");
    PW_Switch_LED(false, true, false);  // Green LED on
    delay(2000);
}

void loop() {
    using namespace ABC;
    using namespace BaseNoid;

    // Check for input from Controller
    bool dataUpdated = BP32.update();

    // Check battery voltage
    if (Check_Battery_Voltage(1006, 154.6)) {
        battery_voltage_count++;
        if (battery_voltage_count > 10) {
            PW_Switch_LED(true, false, false);  // Red LED on
            Debug_Serial.println("[FATAL]:Battery voltage out of range!");
            while (1) {
                PW_Switch_LED(true, false, false);  // Red LED on
                delay(1000);
                PW_Switch_LED(false, false, false);  // All LEDs off
                delay(1000);
            };
        }
    } else {
        battery_voltage_count = 0;  // Reset battery voltage count
    }

    if (BaseNoid_Torque_Flagment == TORQUE_OFF) {
        PW_Switch_LED(false, false, true);  // Blue LED on
    } else {
        PW_Switch_LED(false, true, false);  // Green LED on
    }

    if (dataUpdated) {
        for (auto myController : myControllers) {
            if (myController && myController->isConnected() &&
                myController->hasData()) {
                if (myController->isGamepad()) {
                    BaseNoid_Process(myController);
                } else {
                    Debug_Serial.println("[WARNIG]:Unsupported controller");
                    PW_Switch_LED(true, false, true);  // Purple LED on
                    delay(1000);
                }
            }
        }
    }

    delay(1);
}

void BaseNoid_Process(ControllerPtr ctl) {
    using namespace ABC;
    using namespace BaseNoid;

    if (ctl->miscButtons() & MISC_BUTTON_SYSTEM) {
        if (BaseNoid_Torque_Flagment == TORQUE_ON) {
            BaseNoid_Torque_Flagment = TORQUE_OFF;  // Disable torque
            Debug_Serial.println(
                "[INFO]:PS Button pressed, turned off torque on BaseNoid.");
            enable_servo_torque(
                BaseNoid_Torque_Flagment);      // Disable servo torque
            PW_Switch_LED(false, false, true);  // Blue LED on
            delay(1000);  // Wait for 1 second before resetting
        } else {
            BaseNoid_Torque_Flagment = TORQUE_ON;  // Enable torque
            Debug_Serial.println(
                "[INFO]:PS Button pressed, turned on torque on BaseNoid.");
            enable_servo_torque(
                BaseNoid_Torque_Flagment);      // Disable servo torque
            PW_Switch_LED(false, true, false);  // Green LED on
            set_initial_position(0.2);
            delay(1000);  // Wait for 1 second before resetting
        }
    }

    if (BaseNoid_Torque_Flagment == TORQUE_OFF &&
        ctl->miscButtons() & MISC_BUTTON_START) {
        Configuration_Mode();  // Set BaseNoid to setting mode
    }

    // Press Circle and basenoid get ready for walking
    if (ctl->buttons() & BUTTON_B) {
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
        if (abs(ctl->axisX()) > 64) {
            if (Now_Walking_Flagment == false) {
                Now_Walking_Flagment = true;
                Step_Count = 0;  // Reset step count
            }
            Walk(Step_Count, ctl->axisX(), 0);
            Step_Count++;  // Increment step count
        } else {
            if (Now_Walking_Flagment == true) {
                Walk(Step_Count, 0.0, 0.0);  // Stop walking
                delay(1000);  // Wait for 1 second before stopping
                Now_Walking_Flagment = false;
            }
        }

        if (ctl->buttons() & BUTTON_Y) {
            Debug_Serial.println("[INFO]:Triangle button pressed.");
            // If Triangle button is pressed, perform a punch action
            BaseNoid_Sample_Motion::punch();
        }

        if (ctl->dpad() & DPAD_DOWN) {
            Debug_Serial.println("[INFO]:Down button pressed.");
            // If Down button is pressed, perform a sit down action
            BaseNoid_Sample_Motion::sit_down();
        }
        if (ctl->dpad() & DPAD_UP) {
            Debug_Serial.println("[INFO]:Up button pressed.");
            // If Up button is pressed, perform a wake up action
            BaseNoid_Sample_Motion::stand_up();
        }
        if (ctl->buttons() & BUTTON_X) {
            BaseNoid_Sample_Motion::kataashidachi();
        }
    }
}