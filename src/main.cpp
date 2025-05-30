#include <Arduino.h>

#include <abc_pin_assign.hpp>

void setup() {
    // Initialize serial communication for debugging
    Serial.begin(115200);
    while (!Serial) {
        ;  // Wait for serial port to connect. Needed for native USB port only
    }
    ledcSetup(0, 12800, 8);  // Set up LEDC channel 0 with a frequency of 5000
                             // Hz and 8-bit resolution
    ledcAttachPin(SPEAKER_PIN, 0);  // Attach the built-in LED to channel 0
    Serial.println("Setup complete.");
}

void loop() {
    // Main loop does nothing in this example
    ledcWrite(0, 128);  // Set the duty cycle to 50% (128 out of 255)
    delay(1000);        // Wait for a second
    ledcWrite(0, 0);    // Turn off the LED
    delay(1000);        // Wait for a second
    ledcWrite(0, 255);  // Turn on the LED at full brightness
    delay(1000);        // Wait for a second before the next loop iteration
    Serial.println("Looping...");
}

// #include <Arduino.h>
// #include <HardwareSerial.h>
// #include <PS4Controller.h>
// #include <SCServo.h>

// #define RDATA_LENGTH 6
// #define DEGREE_OF_FREEDOM 19
// #define TORQUE_ON 1
// #define TORQUE_OFF 0

// SMS_STS STS3215;

// HardwareSerial Debug_Serial(0);  // Use USB
// HardwareSerial Servo_Serial(2);  // Use IO16, & IO17

// struct Servo {
//     uint8_t ID;
//     int16_t angle;
//     uint16_t speed;
//     int16_t min_angle;
//     int16_t max_angle;
// };

// const int16_t initial_angle = 2047;
// const uint16_t initial_speed = 3400;

// bool Servo_Torque_State = TORQUE_OFF;
// bool angle_safety = 0;

// bool walk_mode = false;

// long randNum = 0;

// struct Servo waist = {1, initial_angle, initial_speed, 1024, 3072};

// struct Servo L_shoulder_pitch = {11, initial_angle, initial_speed, 1024,
// 4095}; struct Servo L_shoulder_roll = {12, initial_angle, initial_speed,
// 2047, 4095}; struct Servo L_elbow_pitch = {13, initial_angle, initial_speed,
// 2047, 4000};

// struct Servo R_shoulder_pitch = {21, initial_angle, initial_speed, 0, 3072};
// struct Servo R_shoulder_roll = {22, initial_angle, initial_speed, 0, 2047};
// struct Servo R_elbow_pitch = {23, initial_angle, initial_speed, 94, 2047};

// struct Servo R_hip_roll = {31, initial_angle, initial_speed, 1024, 2303};
// struct Servo R_hip_pitch = {32, initial_angle, initial_speed, 512, 2500};
// struct Servo R_hip_yaw = {33, initial_angle, initial_speed, 1024, 3072};
// struct Servo R_knee_pitch = {34, initial_angle, initial_speed, 256, 2186};
// struct Servo R_ankle_roll = {35, initial_angle, initial_speed, 1024, 3072};
// struct Servo R_ankle_pitch = {36, initial_angle, initial_speed, 1024, 3072};

// struct Servo L_hip_roll = {41, initial_angle, initial_speed, 1791, 3072};
// struct Servo L_hip_pitch = {42, initial_angle, initial_speed, 1594, 3582};
// struct Servo L_hip_yaw = {43, initial_angle, initial_speed, 1024, 3072};
// struct Servo L_knee_pitch = {44, initial_angle, initial_speed, 1913, 3838};
// struct Servo L_ankle_roll = {45, initial_angle, initial_speed, 1024, 3072};
// struct Servo L_ankle_pitch = {46, initial_angle, initial_speed, 1024, 3072};

// uint8_t ID_array[DEGREE_OF_FREEDOM] = {
//     waist.ID,         L_shoulder_pitch.ID, L_shoulder_roll.ID,
//     L_elbow_pitch.ID, R_shoulder_pitch.ID, R_shoulder_roll.ID,
//     R_elbow_pitch.ID, R_hip_roll.ID,       R_hip_pitch.ID,
//     R_hip_yaw.ID,     R_knee_pitch.ID,     R_ankle_roll.ID,
//     R_ankle_pitch.ID, L_hip_roll.ID,       L_hip_pitch.ID,
//     L_hip_yaw.ID,     L_knee_pitch.ID,     L_ankle_roll.ID,
//     L_ankle_pitch.ID};
// int16_t Angle_array[DEGREE_OF_FREEDOM] = {};
// uint16_t Speed_array[DEGREE_OF_FREEDOM] = {};
// uint8_t ACC_array[DEGREE_OF_FREEDOM] = {};

// char rdata[RDATA_LENGTH];

// bool check_range(uint8_t Servo_ID, int16_t Position);
// void enable_servo_torque(bool torque_enable);
// bool angle_array_update(void);
// void speed_array_update(void);
// void initial_posture(void);
// int16_t angle(double rad);

// void walk_ready(void);
// void walk_straight_motion(void);
// void walk_finish(void);
// void shake_R_hands(void);
// void shake_L_hands(void);
// void punch(void);
// void banzai(void);
// void tukkomi(void);

// void setup() {
//     Debug_Serial.begin(115200);
//     Servo_Serial.begin(1000000);
//     PS4.begin();
//     STS3215.pSerial = &Servo_Serial;

//     srand(1);

//     for (int i = 0; i < DEGREE_OF_FREEDOM; ++i) {
//         ACC_array[i] = 50;
//     }
//     speed_array_update();
//     angle_safety = angle_array_update();

//     while (1) {
//         if (PS4.isConnected()) {
//             if (PS4.PSButton()) break;
//         }
//     }

//     Servo_Torque_State = TORQUE_ON;
//     enable_servo_torque(Servo_Torque_State);

//     if (angle_safety == 1) {
//         Debug_Serial.println("Fatal:out of angle range.");
//         PS4.setLed(255, 0, 0);
//         PS4.sendToController();
//         while (1);
//     } else
//         STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                Speed_array, ACC_array);

//     delay(3000);
// }

// void loop() {
//     if (PS4.isConnected()) {
//         if (PS4.PSButton()) {  // 緊停処理
//             Servo_Torque_State = TORQUE_OFF;
//             enable_servo_torque(Servo_Torque_State);
//             Debug_Serial.println("Fatal:emergency stop activated.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         }
//         // ここにどんどん書いてく．
//         if (PS4.Circle()) {
//             shake_R_hands();
//         }
//         if (PS4.Square()) {
//             shake_L_hands();
//         }
//         if (PS4.Triangle()) {
//             punch();
//         }
//         if (PS4.Cross()) {
//             banzai();
//         }
//         if (PS4.L1()) {
//             tukkomi();
//         }
//         if (PS4.Up()) {
//             if (walk_mode == false) {
//                 walk_mode = true;
//                 walk_ready();
//             } else {
//                 walk_straight_motion();
//             }
//         } else {
//             if (walk_mode == true) {
//                 walk_mode = false;
//                 walk_finish();
//             }
//             initial_posture();
//         }

//     } else {
//         initial_posture();
//         randNum = random(4);
//         if (randNum == 0) {
//             shake_L_hands();
//         } else if (randNum == 1) {
//             shake_R_hands();
//         } else if (randNum == 2) {
//             punch();
//         } else {
//             banzai();
//         }
//     }
//     delay(100);
// }

// bool check_range(uint8_t Servo_ID, int16_t Position) {
//     if (Servo_ID == waist.ID) {
//         if (Position < waist.min_angle || Position > waist.max_angle) {
//             return 1;
//         } else {
//             return 0;
//         }
//     } else if (Servo_ID == L_shoulder_pitch.ID) {
//         if (Position < L_shoulder_pitch.min_angle ||
//             Position > L_shoulder_pitch.max_angle) {
//             return 1;
//         } else {
//             return 0;
//         }
//     } else if (Servo_ID == L_shoulder_roll.ID) {
//         if (Position < L_shoulder_roll.min_angle ||
//             Position > L_shoulder_roll.max_angle) {
//             return 1;
//         } else {
//             return 0;
//         }
//     } else if (Servo_ID == L_elbow_pitch.ID) {
//         if (Position < L_elbow_pitch.min_angle ||
//             Position > L_elbow_pitch.max_angle) {
//             return 1;
//         } else {
//             return 0;
//         }
//     } else if (Servo_ID == R_shoulder_pitch.ID) {
//         if (Position < R_shoulder_pitch.min_angle ||
//             Position > R_shoulder_pitch.max_angle) {
//             return 1;
//         } else {
//             return 0;
//         }
//     } else if (Servo_ID == R_shoulder_roll.ID) {
//         if (Position < R_shoulder_roll.min_angle ||
//             Position > R_shoulder_roll.max_angle) {
//             return 1;
//         } else {
//             return 0;
//         }
//     } else if (Servo_ID == R_elbow_pitch.ID) {
//         if (Position < R_elbow_pitch.min_angle ||
//             Position > R_elbow_pitch.max_angle) {
//             return 1;
//         } else {
//             return 0;
//         }
//     } else if (Servo_ID == R_hip_roll.ID) {
//         if (Position < R_hip_roll.min_angle ||
//             Position > R_hip_roll.max_angle) {
//             return 1;
//         } else {
//             return 0;
//         }
//     } else if (Servo_ID == R_hip_pitch.ID) {
//         if (Position < R_hip_pitch.min_angle ||
//             Position > R_hip_pitch.max_angle) {
//             return 1;
//         } else {
//             return 0;
//         }
//     } else if (Servo_ID == R_hip_yaw.ID) {
//         if (Position < R_hip_yaw.min_angle || Position > R_hip_yaw.max_angle)
//         {
//             return 1;
//         } else {
//             return 0;
//         }
//     } else if (Servo_ID == R_knee_pitch.ID) {
//         if (Position < R_knee_pitch.min_angle ||
//             Position > R_knee_pitch.max_angle) {
//             return 1;
//         } else {
//             return 0;
//         }
//     } else if (Servo_ID == R_ankle_roll.ID) {
//         if (Position < R_ankle_roll.min_angle ||
//             Position > R_ankle_roll.max_angle) {
//             return 1;
//         } else {
//             return 0;
//         }
//     } else if (Servo_ID == R_ankle_pitch.ID) {
//         if (Position < R_ankle_pitch.min_angle ||
//             Position > R_ankle_pitch.max_angle) {
//             return 1;
//         } else {
//             return 0;
//         }
//     } else if (Servo_ID == L_hip_roll.ID) {
//         if (Position < L_hip_roll.min_angle ||
//             Position > L_hip_roll.max_angle) {
//             return 1;
//         } else {
//             return 0;
//         }
//     } else if (Servo_ID == L_hip_pitch.ID) {
//         if (Position < L_hip_pitch.min_angle ||
//             Position > L_hip_pitch.max_angle) {
//             return 1;
//         } else {
//             return 0;
//         }
//     } else if (Servo_ID == L_hip_yaw.ID) {
//         if (Position < L_hip_yaw.min_angle || Position > L_hip_yaw.max_angle)
//         {
//             return 1;
//         } else {
//             return 0;
//         }
//     } else if (Servo_ID == L_knee_pitch.ID) {
//         if (Position < L_knee_pitch.min_angle ||
//             Position > L_knee_pitch.max_angle) {
//             return 1;
//         } else {
//             return 0;
//         }
//     } else if (Servo_ID == L_ankle_roll.ID) {
//         if (Position < L_ankle_roll.min_angle ||
//             Position > L_ankle_roll.max_angle) {
//             return 1;
//         } else {
//             return 0;
//         }
//     } else if (Servo_ID == L_ankle_pitch.ID) {
//         if (Position < L_ankle_pitch.min_angle ||
//             Position > L_ankle_pitch.max_angle) {
//             return 1;
//         } else {
//             return 0;
//         }
//     }
// }

// void enable_servo_torque(bool torque_enable) {
//     for (int i = 0; i < DEGREE_OF_FREEDOM; ++i) {
//         STS3215.EnableTorque(ID_array[i], torque_enable);
//     }
// }

// bool angle_array_update(void) {
//     Angle_array[0] = waist.angle;
//     if (check_range(ID_array[0], Angle_array[0]) == 1) return 1;
//     Angle_array[1] = L_shoulder_pitch.angle;
//     if (check_range(ID_array[1], Angle_array[1]) == 1) return 1;
//     Angle_array[2] = L_shoulder_roll.angle;
//     if (check_range(ID_array[2], Angle_array[2]) == 1) return 1;
//     Angle_array[3] = L_elbow_pitch.angle;
//     if (check_range(ID_array[3], Angle_array[3]) == 1) return 1;
//     Angle_array[4] = R_shoulder_pitch.angle;
//     if (check_range(ID_array[4], Angle_array[4]) == 1) return 1;
//     Angle_array[5] = R_shoulder_roll.angle;
//     if (check_range(ID_array[5], Angle_array[5]) == 1) return 1;
//     Angle_array[6] = R_elbow_pitch.angle;
//     if (check_range(ID_array[6], Angle_array[6]) == 1) return 1;
//     Angle_array[7] = R_hip_roll.angle;
//     if (check_range(ID_array[7], Angle_array[7]) == 1) return 1;
//     Angle_array[8] = R_hip_pitch.angle;
//     if (check_range(ID_array[8], Angle_array[8]) == 1) return 1;
//     Angle_array[9] = R_hip_yaw.angle;
//     if (check_range(ID_array[9], Angle_array[9]) == 1) return 1;
//     Angle_array[10] = R_knee_pitch.angle;
//     if (check_range(ID_array[10], Angle_array[10]) == 1) return 1;
//     Angle_array[11] = R_ankle_roll.angle;
//     if (check_range(ID_array[11], Angle_array[11]) == 1) return 1;
//     Angle_array[12] = R_ankle_pitch.angle;
//     if (check_range(ID_array[12], Angle_array[12]) == 1) return 1;
//     Angle_array[13] = L_hip_roll.angle;
//     if (check_range(ID_array[13], Angle_array[13]) == 1) return 1;
//     Angle_array[14] = L_hip_pitch.angle;
//     if (check_range(ID_array[14], Angle_array[14]) == 1) return 1;
//     Angle_array[15] = L_hip_yaw.angle;
//     if (check_range(ID_array[15], Angle_array[15]) == 1) return 1;
//     Angle_array[16] = L_knee_pitch.angle;
//     if (check_range(ID_array[16], Angle_array[16]) == 1) return 1;
//     Angle_array[17] = L_ankle_roll.angle;
//     if (check_range(ID_array[17], Angle_array[17]) == 1) return 1;
//     Angle_array[18] = L_ankle_pitch.angle;
//     if (check_range(ID_array[18], Angle_array[18]) == 1) return 1;
//     return 0;
// }

// void speed_array_update(void) {
//     Speed_array[0] = waist.speed;
//     Speed_array[1] = L_shoulder_pitch.speed;
//     Speed_array[2] = L_shoulder_roll.speed;
//     Speed_array[3] = L_elbow_pitch.speed;
//     Speed_array[4] = R_shoulder_pitch.speed;
//     Speed_array[5] = R_shoulder_roll.speed;
//     Speed_array[6] = R_elbow_pitch.speed;
//     Speed_array[7] = R_hip_roll.speed;
//     Speed_array[8] = R_hip_pitch.speed;
//     Speed_array[9] = R_hip_yaw.speed;
//     Speed_array[10] = R_knee_pitch.speed;
//     Speed_array[11] = R_ankle_roll.speed;
//     Speed_array[12] = R_ankle_pitch.speed;
//     Speed_array[13] = L_hip_roll.speed;
//     Speed_array[14] = L_hip_pitch.speed;
//     Speed_array[15] = L_hip_yaw.speed;
//     Speed_array[16] = L_knee_pitch.speed;
//     Speed_array[17] = L_ankle_roll.speed;
//     Speed_array[18] = L_ankle_pitch.speed;
// }

// void initial_posture(void) {
//     waist.angle = initial_angle;

//     L_shoulder_pitch.angle = initial_angle;
//     L_shoulder_roll.angle = initial_angle;
//     L_elbow_pitch.angle = initial_angle;

//     R_shoulder_pitch.angle = initial_angle;
//     R_shoulder_roll.angle = initial_angle;
//     R_elbow_pitch.angle = initial_angle;

//     R_hip_roll.angle = initial_angle;
//     R_hip_pitch.angle = initial_angle;
//     R_hip_yaw.angle = initial_angle;
//     R_knee_pitch.angle = initial_angle;
//     R_ankle_roll.angle = initial_angle;
//     R_ankle_pitch.angle = initial_angle;

//     L_hip_roll.angle = initial_angle;
//     L_hip_pitch.angle = initial_angle;
//     L_hip_yaw.angle = initial_angle;
//     L_knee_pitch.angle = initial_angle;
//     L_ankle_roll.angle = initial_angle;
//     L_ankle_pitch.angle = initial_angle;

//     angle_safety = angle_array_update();
//     if (angle_safety == 1) {
//         Debug_Serial.println("Fatal:out of angle range.");
//         PS4.setLed(255, 0, 0);
//         PS4.sendToController();
//         while (1);
//     } else
//         STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                Speed_array, ACC_array);
//     delay(500);
// }

// int16_t angle(double rad) { return 4095 * rad / (2 * PI); }

// void walk_ready(void) {
//     for (int i = 1; i < 17; ++i) {
//         R_hip_pitch.angle = 2047 - 16 * i;
//         R_knee_pitch.angle = 2047 - 32 * i;
//         R_ankle_pitch.angle = 2047 - 16 * i;

//         L_hip_pitch.angle = 2047 + 16 * i;
//         L_knee_pitch.angle = 2047 + 32 * i;
//         L_ankle_pitch.angle = 2047 + 16 * i;
//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
// }

// void walk_straight_motion(void) {
//     for (int i = 1; i < 17; ++i) {
//         R_hip_roll.angle -= 10;
//         R_ankle_roll.angle -= 10;

//         L_hip_roll.angle -= 10;
//         L_ankle_roll.angle -= 10;
//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     delay(1000);
//     for (int i = 1; i < 17; ++i) {
//         R_hip_pitch.angle -= 8;
//         R_knee_pitch.angle -= 16;
//         R_ankle_pitch.angle -= 8;

//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     delay(1000);
//     for (int i = 1; i < 17; ++i) {
//         R_hip_pitch.angle -= 16;
//         R_ankle_pitch.angle += 16;

//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     delay(1000);
//     for (int i = 1; i < 17; ++i) {
//         R_hip_pitch.angle += 24;
//         R_knee_pitch.angle += 48;
//         R_ankle_pitch.angle += 24;

//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     delay(1000);
//     for (int i = 1; i < 33; ++i) {
//         R_hip_roll.angle += 10;
//         R_knee_pitch.angle -= 16;
//         R_ankle_roll.angle += 10;
//         R_ankle_pitch.angle -= 16;

//         L_hip_roll.angle += 10;
//         L_hip_pitch.angle -= 16;
//         L_knee_pitch.angle -= 16;
//         L_ankle_roll.angle += 10;

//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     delay(1000);
//     for (int i = 1; i < 16; ++i) {
//         L_knee_pitch.angle += 48;
//         L_ankle_pitch.angle += 48;

//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     delay(1000);
//     for (int i = 1; i < 16; ++i) {
//         L_hip_pitch.angle += 40;
//         L_ankle_pitch.angle -= 40;

//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     delay(1000);
//     for (int i = 1; i < 16; ++i) {
//         L_hip_pitch.angle -= 8;
//         L_knee_pitch.angle -= 16;
//         L_ankle_pitch.angle -= 8;

//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     delay(1000);
//     for (int i = 1; i < 16; ++i) {
//         R_hip_roll.angle -= 10;
//         R_ankle_roll.angle -= 10;

//         L_hip_roll.angle -= 10;
//         L_ankle_roll.angle -= 10;

//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     delay(1000);
// }

// void walk_finish(void) {
//     for (int i = 1; i < 17; ++i) {
//         R_hip_pitch.angle = 1791 + 16 * i;
//         R_knee_pitch.angle = 1535 + 32 * i;
//         R_ankle_pitch.angle = 1791 + 16 * i;

//         L_hip_pitch.angle = 2303 - 16 * i;
//         L_knee_pitch.angle = 2559 - 32 * i;
//         L_ankle_pitch.angle = 2303 - 16 * i;
//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
// }

// void shake_R_hands(void) {
//     for (int i = 1; i < 33; ++i) {
//         R_shoulder_roll.angle -= 63;
//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     for (int i = 1; i < 33; ++i) {
//         R_shoulder_roll.angle += 16;
//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     delay(100);
//     for (int i = 1; i < 33; ++i) {
//         R_shoulder_roll.angle -= 16;
//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     delay(100);
//     for (int i = 1; i < 33; ++i) {
//         R_shoulder_roll.angle += 16;
//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     delay(100);
//     for (int i = 1; i < 33; ++i) {
//         R_shoulder_roll.angle -= 16;
//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     delay(100);
//     for (int i = 1; i < 33; ++i) {
//         R_shoulder_roll.angle += 16;
//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     delay(100);
//     for (int i = 1; i < 33; ++i) {
//         R_shoulder_roll.angle -= 16;
//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     delay(100);
//     for (int i = 1; i < 33; ++i) {
//         R_shoulder_roll.angle += 63;
//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
// }

// void shake_L_hands(void) {
//     for (int i = 1; i < 33; ++i) {
//         L_elbow_pitch.angle += 32;
//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     delay(1000);
//     for (int i = 1; i < 17; ++i) {
//         L_elbow_pitch.angle += 16;
//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     for (int i = 1; i < 33; ++i) {
//         L_elbow_pitch.angle -= 16;
//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     for (int i = 1; i < 33; ++i) {
//         L_elbow_pitch.angle += 16;
//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     for (int i = 1; i < 33; ++i) {
//         L_elbow_pitch.angle -= 16;
//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     for (int i = 1; i < 33; ++i) {
//         L_elbow_pitch.angle += 16;
//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     for (int i = 1; i < 33; ++i) {
//         L_elbow_pitch.angle -= 40;
//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
// }

// void punch(void) {
//     for (int i = 1; i < 33; ++i) {
//         R_elbow_pitch.angle -= 32;
//         L_elbow_pitch.angle += 32;
//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     delay(1000);
//     for (int i = 1; i < 17; ++i) {
//         waist.angle -= 32;
//         R_shoulder_pitch.angle -= 64;
//         R_shoulder_roll.angle -= 32;
//         R_elbow_pitch.angle += 32;

//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     delay(2000);
//     for (int i = 1; i < 17; ++i) {
//         waist.angle += 64;
//         R_shoulder_pitch.angle += 64;
//         R_shoulder_roll.angle += 32;
//         R_elbow_pitch.angle -= 32;

//         L_shoulder_pitch.angle += 64;
//         L_shoulder_roll.angle += 32;
//         L_elbow_pitch.angle -= 32;

//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     delay(2000);
//     for (int i = 1; i < 32; ++i) {
//         waist.angle -= 16;

//         R_elbow_pitch.angle += 16;

//         L_shoulder_pitch.angle -= 32;
//         L_shoulder_roll.angle -= 16;

//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
// }

// void banzai(void) {
//     for (int i = 1; i < 33; ++i) {
//         R_shoulder_roll.angle -= 63;
//         L_shoulder_roll.angle += 63;
//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     delay(2000);
//     for (int i = 1; i < 33; ++i) {
//         R_shoulder_roll.angle += 32;
//         R_shoulder_pitch.angle -= 32;
//         R_elbow_pitch.angle -= 32;
//         L_shoulder_roll.angle -= 32;
//         L_shoulder_pitch.angle += 32;
//         L_elbow_pitch.angle += 32;
//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     delay(3000);
//     for (int i = 1; i < 33; ++i) {
//         R_shoulder_roll.angle += 31;
//         R_shoulder_pitch.angle += 32;
//         R_elbow_pitch.angle += 32;

//         L_shoulder_roll.angle -= 31;
//         L_shoulder_pitch.angle -= 32;
//         L_elbow_pitch.angle -= 32;
//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
// }

// void tukkomi(void) {
//     for (int i = 1; i < 33; ++i) {
//         L_shoulder_roll.angle += 32;
//         L_elbow_pitch.angle += 61;
//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     delay(1000);
//     for (int i = 1; i < 9; ++i) {
//         L_elbow_pitch.angle -= 244;
//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     delay(200);
//     for (int i = 1; i < 9; ++i) {
//         L_elbow_pitch.angle += 244;
//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
//     delay(1000);
//     for (int i = 1; i < 33; ++i) {
//         L_shoulder_roll.angle -= 32;
//         L_elbow_pitch.angle -= 61;
//         angle_safety = angle_array_update();
//         if (angle_safety == 1) {
//             Debug_Serial.println("Fatal:out of angle range.");
//             PS4.setLed(255, 0, 0);
//             PS4.sendToController();
//             while (1);
//         } else
//             STS3215.SyncWritePosEx(ID_array, DEGREE_OF_FREEDOM, Angle_array,
//                                    Speed_array, ACC_array);
//         delay(50);
//     }
// }
