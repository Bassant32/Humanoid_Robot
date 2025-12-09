//  This code provides the initial settings for the servo motors.
//  The starting angles are based on the final code developed by last year’s (Metal Detection Humanoid Robot) team. 
//  These values serve only as initial settings and will be refined during upcoming testing sessions.
//  The remaining code responsible for the humanoid robot’s walking sequence will be uploaded tomorrow. 
//  You are welcome to review, modify, and improve the code as needed. 


//*******************************LIBRARIES INCLUDED SECTION**************************************

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

//*******************************MACROS SECTION**************************************

#define SERVO_MIN 205
#define SERVO_MAX 410
// Values used above are from the datasheet of the MG996R servo

//*******************************FUNCTION DECLARATION SECTION**************************************

// Initialize PCA9685
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Servo setup
const int servoCount = 13;               // Total number of servos used

// The following function converts the angle given into a PWM signal 
// for the desired channel required to be used
// it uses the map() function to get the actual value of the PWM signal based on the 
// max and min values for the used ServoMotor

void setServoAngle(int channel, float angle) {
  int pulse = map(angle, 0, 180, SERVO_MIN, SERVO_MAX);
  pwm.setPWM(channel, 0, pulse);
}

//*************************************SETUP SECTION********************************************
void setup() { 

  Serial.begin(115200);          // Baud rate for data transmission
  pwm.begin();
  pwm.setPWMFreq(50);            // Set frequency to 50 Hz for servos

  // Set initial position
  stand();

}

void loop() {}

// Arrangement of Servos : 1) Right Foot     , 2) Right Knee , 3) Right Thigh    , 4) Right Hip
//                         5) Left Hip       , 6) Left Thigh , 7) Left Knee      , 8) Left Foot
//                         9) Right Shoulder , 10) Right Arm , 11) Left Shoulder , 12) Left Arm


//****************************** Movement Functions Section ***************************************************
void stand() {
    int angles[] = {(90 - 40), (90 - 40 + 3), (90 + 40 - 3), (90 - 40), (90 + 40), (90 - 40 + 3), (90 + 40 - 3), (90 + 40),
                    (90 + 40), (90 + 40), (90 - 40), (90 - 40), 90};

    // Setting the desired values of the servos
    for (int i = 0; i < servoCount; i++) {
        setServoAngle(i, angles[i]);
    }
}
