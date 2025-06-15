#include <Arduino.h>
#include <PS4Controller.h>

#include "abc_function.hpp"
#include "basenoid.hpp"

void setup() {
    ABC::PIN_SETUP();
    ABC::Serial_Begin(115200, 115200, 1000000);
    PS4.begin();                             // Initialize PS4 controller
    ABC::PW_Switch_LED(false, false, true);  // Blue LED on

    Debug_Serial.println(
        "Press 'c' to calibration mode, or wait to start BaseNoid.");
    unsigned long start = millis();
    while (millis() - start < 10000) {  // 10秒間だけ待つ
        if (Debug_Serial.available()) {
            char ch1 = Debug_Serial.read();
            if (ch1 == 'c' || ch1 == 'C') {
                while (1) {
                    Debug_Serial.println("=== Calibration Mode ===");
                    ABC::PW_Switch_LED(true, true, false);  // Yellow LED on

                    Debug_Serial.println("Select the item to calibrate.");
                    Debug_Serial.println("1: Battery Voltage");
                    Debug_Serial.println("2: Exit Calibration Mode");
                    while (!Debug_Serial.available()) {
                        delay(100);
                    }

                    char ch2 = Debug_Serial.read();
                    if (ch2 == '2') {
                        Debug_Serial.println("Exiting Calibration Mode.");
                        ABC::PW_Switch_LED(false, false, true);  // Blue LED on
                        break;
                    } else if (ch2 == '1') {
                        Debug_Serial.println("Battery Voltage Calibration.");
                        ABC::Battery_Voltage_Calibration(1006, 154.6);
                    } else {
                        Debug_Serial.println(
                            "Invalid selection. Please try again.");
                    }
                }

                break;
            }
        }
        delay(100);
    }
    Debug_Serial.println("Setup complete.");
    Debug_Serial.println("Welcome to BaseNoid!");
    ABC::PW_Switch_LED(false, true, false);  // Green LED on
}

void loop() {
    // Check battery voltage
    if (ABC::Check_Battery_Voltage(1006, 154.6)) {
        ABC::PW_Switch_LED(true, false, false);  // Red LED on
        while (1) {
            Debug_Serial.println("Battery voltage out of range!");
            delay(1000);  // Wait for 1 second before checking again
        };
    }

    // Add your main code here
    delay(1000);  // Delay for demonstration purposes
}
