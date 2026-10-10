#include <Arduino.h>

#include "abc_function.hpp"
#include "basenoid.hpp"
#include "basenoid_sample_motion.hpp"

static bool Now_Walking_Flagment =
    false;  // Flag to indicate if walking is in progress
static bool Leg_flexed_state = false;  // Flag to indicate if the leg is flexed
static bool BaseNoid_Torque_Flagment = TORQUE_OFF;
static uint32_t Step_Count = 0;        // Initialize step count
static int16_t Walk_Command_x = 0;     // Forward(+)/backward(-) command
static int16_t Walk_Command_turn = 0;  // Turn left(+)/right(-) command
static volatile bool Random_Motion_Flagment =
    false;                                // Playing motions at random
static bool Square_Button_State = false;  // Square button is held down

void BaseNoid_Process(ControllerPtr ctl);

// Returns true only at the moment the Square button is pressed
bool Square_Button_Pressed(void) {
    using namespace ABC;
    bool pressed = false;
    for (auto myController : myControllers) {
        if (myController && myController->isConnected() &&
            myController->isGamepad() && (myController->buttons() & BUTTON_X)) {
            pressed = true;
        }
    }
    const bool pressed_now = pressed && !Square_Button_State;
    Square_Button_State = pressed;
    return pressed_now;
}

// Watcher task (works like an interrupt) : while a random motion is being
// played, the main task is blocked in the motion, so this task watches the
// controller instead. When the Square button is pressed (or the controller
// is disconnected), it wakes up the main task to stop the motion immediately.
static SemaphoreHandle_t Controller_Mutex = nullptr;
static volatile bool Random_Motion_Playing = false;

void Random_Motion_Watcher(void*) {
    using namespace ABC;
    for (;;) {
        xSemaphoreTake(Controller_Mutex, portMAX_DELAY);
        if (Random_Motion_Playing) {
            BP32.update();
            bool controller_connected = false;
            for (auto myController : myControllers) {
                if (myController && myController->isConnected()) {
                    controller_connected = true;
                }
            }
            if (Square_Button_Pressed() || !controller_connected) {
                Random_Motion_Flagment = false;
                Random_Motion_Playing = false;
                BaseNoid_Sample_Motion::request_motion_stop();
            }
        }
        xSemaphoreGive(Controller_Mutex);
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

void setup() {
    using namespace ABC;
    using namespace BaseNoid;
    PIN_SETUP();
    Serial_Begin(115200, 115200, 1000000);
    Servo_Setup();  // Initialize servo motors

    // Setup the Bluepad32
    BluePad32_Controller_Setup(false);  // Enable touchpad and mouse support
    // Higher priority than loop() (priority 1), same core as loop()
    Controller_Mutex = xSemaphoreCreateMutex();
    xTaskCreatePinnedToCore(Random_Motion_Watcher, "motion_watcher", 4096,
                            nullptr, 2, nullptr, ARDUINO_RUNNING_CORE);

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

    apply_servo_gain(false,
                     true);  // Set servo PID gains before enabling torque
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

    // Stop walking if no controller is connected
    bool controller_connected = false;
    for (auto myController : myControllers) {
        if (myController && myController->isConnected()) {
            controller_connected = true;
        }
    }
    if (!controller_connected) {
        Walk_Command_x = 0;
        Walk_Command_turn = 0;
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

    // Walking : called every loop without waiting for controller updates,
    // so that the LIPM trajectory is continuous between steps
    if (Leg_flexed_state == true && BaseNoid_Torque_Flagment == TORQUE_ON) {
        if (Walk_Command_x != 0 || Walk_Command_turn != 0) {
            if (Now_Walking_Flagment == false) {
                Now_Walking_Flagment = true;
                Step_Count = 0;  // Reset step count
            }
            Walk(Step_Count, Walk_Command_x, 0, Walk_Command_turn);
            Step_Count++;  // Increment step count
        } else if (Now_Walking_Flagment == true) {
            Walk_Stop(Step_Count);  // Stop walking
            Now_Walking_Flagment = false;
        }
    }

    // Random motion : Square button starts / stops it (only while standing)
    const bool random_motion_available =
        Leg_flexed_state == false && BaseNoid_Torque_Flagment == TORQUE_ON &&
        controller_connected;
    // true : the random motions ended without being stopped halfway, so the
    // waist is still turned
    bool random_motion_ended = false;
    if (Square_Button_Pressed() && random_motion_available) {
        Random_Motion_Flagment = !Random_Motion_Flagment;
        random_motion_ended = !Random_Motion_Flagment;
        Debug_Serial.println(Random_Motion_Flagment
                                 ? "[INFO]:Random motion started."
                                 : "[INFO]:Random motion stopped.");
    }
    if (!random_motion_available && Random_Motion_Flagment) {
        Random_Motion_Flagment = false;
        random_motion_ended = true;
    }
    if (random_motion_ended && BaseNoid_Torque_Flagment == TORQUE_ON) {
        BaseNoid_Sample_Motion::face_front();
    }
    if (Random_Motion_Flagment) {
        // The watcher task stops the motion immediately when the Square
        // button is pressed again
        xSemaphoreTake(Controller_Mutex, portMAX_DELAY);
        Random_Motion_Playing = true;
        xSemaphoreGive(Controller_Mutex);

        const bool finished = BaseNoid_Sample_Motion::play_random_motion();

        xSemaphoreTake(Controller_Mutex, portMAX_DELAY);
        Random_Motion_Playing = false;
        xSemaphoreGive(Controller_Mutex);
        if (!finished) {
            Debug_Serial.println("[INFO]:Random motion stopped.");
        } else if (!Random_Motion_Flagment) {
            // Stop requested just after the motion finished
            Debug_Serial.println("[INFO]:Random motion stopped.");
            BaseNoid_Sample_Motion::face_front();
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
            Walk_Command_x = 0;
            Walk_Command_turn = 0;
            Now_Walking_Flagment = false;
            apply_servo_gain(false);  // Gains for non-walking mode
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

    if (Leg_flexed_state == true) {
        // Left stick up : forward, down : backward
        Walk_Command_x = (abs(ctl->axisY()) > 64) ? -ctl->axisY() : 0;
        // Left stick left : turn left, right : turn right
        Walk_Command_turn = (abs(ctl->axisX()) > 64) ? -ctl->axisX() : 0;
    } else {
        Walk_Command_x = 0;
        Walk_Command_turn = 0;
    }
    if (Now_Walking_Flagment == true) {
        return;  // Ignore other buttons while walking
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
        if (ctl->buttons() & BUTTON_A) {
            Debug_Serial.println("[INFO]:Cross button pressed.");
            // If Cross button is pressed, measure the servo delay
            Measure_Control_Delay();
        }
    }

    // Upper body motions (available in any leg posture)
    // A motion starts only at the moment the button is pressed, so a button
    // that stays pressed (held down, or a worn trigger) does not repeat it.
    // L2 / R2 : the analog value (0 - 1023) is used, because the digital
    // flag turns on even when the trigger is only slightly pressed.
    const int32_t Trigger_Threshold = 512;
    enum {
        MOTION_PUNCH = 1 << 0,
        MOTION_R1 = 1 << 1,
        MOTION_L1 = 1 << 2,
        MOTION_R2 = 1 << 3,
        MOTION_L2 = 1 << 4,
    };

    uint8_t motion_buttons = 0;
    if (ctl->buttons() & BUTTON_Y) motion_buttons |= MOTION_PUNCH;
    if (ctl->r1()) motion_buttons |= MOTION_R1;
    if (ctl->l1()) motion_buttons |= MOTION_L1;
    // D-pad left / right : substitutes for L1 / L2 (for a controller whose
    // L1 / L2 do not work, e.g. L2 analog value stuck at 1020)
    if (ctl->dpad() & DPAD_LEFT) motion_buttons |= MOTION_L1;
    if (ctl->dpad() & DPAD_RIGHT) motion_buttons |= MOTION_L2;
    if (ctl->throttle() > Trigger_Threshold) motion_buttons |= MOTION_R2;
    if (ctl->brake() > Trigger_Threshold) motion_buttons |= MOTION_L2;
    // 0xFF : buttons already pressed at startup are ignored until released
    static uint8_t last_motion_buttons = 0xFF;
    const uint8_t pressed_now = motion_buttons & ~last_motion_buttons;
    last_motion_buttons = motion_buttons;

    if (BaseNoid_Torque_Flagment == TORQUE_ON) {
        if (pressed_now & MOTION_PUNCH) {
            Debug_Serial.println("[INFO]:Triangle button pressed.");
            // If Triangle button is pressed, perform a punch action
            BaseNoid_Sample_Motion::punch();
        } else if (pressed_now & MOTION_R1) {
            Debug_Serial.println("[INFO]:R1 button pressed.");
            BaseNoid_Sample_Motion::shake_R_hands();
        } else if (pressed_now & MOTION_L1) {
            Debug_Serial.println("[INFO]:L1 (or D-pad left) pressed.");
            BaseNoid_Sample_Motion::shake_L_hands();
        } else if (pressed_now & MOTION_R2) {
            Debug_Serial.printf("[INFO]:R2 button pressed. (%d)\n",
                                (int)ctl->throttle());
            BaseNoid_Sample_Motion::banzai();
        } else if (pressed_now & MOTION_L2) {
            Debug_Serial.printf("[INFO]:L2 (or D-pad right) pressed. (%d)\n",
                                (int)ctl->brake());
            BaseNoid_Sample_Motion::tukkomi();
        }
    }
}