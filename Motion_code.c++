//*******************************LIBRARIES INCLUDED SECTION************************************

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

//*******************************MACROS SECTION************************************************

#define SERVO_MIN 205
#define SERVO_MAX 410
// Values used above are from the datasheet of the MG996R servo

//*******************************FUNCTION DECLARATION SECTION**********************************

void setServoAngle(int channel, float angle);
void stand1();
void stand2();
void stand3();
void stand_right();
void stand_rightfoot();
void stand_rightfoot_leftfoot_f();
void stand_leftfoot_rightfoot_f();
void stand_RfootFront();
void stand_RfootFront_right();
void stand_left();
void stand_leftfoot();
void stand_lfootFront();
void stand_LfootFront_left();
void stand_rightfoot_leftfoot_b();
void stand_leftfoot_rightfoot_b();
void stand_RfootFront_left();
void stand_LfootFront_right();
void stand_rfoot2();
void stand_left2();
void walk();
void turn_left();
void turn_right();
void move_back();

// Initialize PCA9685
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Servo setup
const int servoCount = 13;               // Total number of servos used

void iad(){
  stand1(); 
  delay(200); 
  stand2(); 
  delay(200); 
}

//**************************************MAIN SECTION*******************************************
void setup() { 

  Serial.begin(115200);          // Baud rate for data transmission
  pwm.begin();                   // Initiates sending PWM signal to the PCA9685 
  pwm.setPWMFreq(50);            // Sets frequency to 50 Hz for servos

  // Set initial position
  stand1();
}

void loop(){
  
}


//****************************** FUNCTION DEFIFINTION SECTION ************************************

/*
  The following function converts the angle given into a PWM signal 
  for the desired channel required to be used
  it uses the map() function to get the actual value of the PWM signal based on the 
  max and min values for the used ServoMotor
*/

void setServoAngle(int channel, float angle) {
  int pulse = map(angle, 0, 180, SERVO_MIN, SERVO_MAX);
  pwm.setPWM(channel, 0, pulse);
}

// Standing Mechanism 

/*
  Arrangement of Servos: 1) Right Foot     , 2) Right Knee , 3) Right Thigh    , 4) Right Hip
                          5) Left Hip       , 6) Left Thigh , 7) Left Knee      , 8) Left Foot
                          9) Right Shoulder , 10) Right Arm , 11) Left Shoulder , 12) Left Arm , 13) Head
*/

// Option 1

void stand1() {
  int angles[] = {(90 - 40), (90 - 40 + 3), (90 + 40 - 3), (90 - 40),
                  (90 + 40), (90 - 40 + 3), (90 + 40 - 3), (90 + 40),
                  (90 + 40), (90 + 40), (90 - 40), (90 - 40), 90};

    // Sets the values of the servos
  for (int i = 0; i < servoCount; i++) {
    setServoAngle(i, angles[i]);
  }
}

// Option 2

void stand2(){
  int angles[] = {(90 - 40), ( 90 - 40 + 20), (90 + 40 - 20), (90 - 40),
                  (90 + 40), (90 - 40 + 20), (90 + 40 - 20), (90 + 40),
                  (90 + 40 - 20), (90 + 40 + 20), (90 - 40 + 20), (90 - 40 - 20), 90};

  for (int i = 0; i < servoCount; i++) {
    setServoAngle(i, angles[i]);
  }
}

// Option 3 

void stand3(){
  int angles[]={60, 70, 110, 60,
                120, 70, 110, 120,
                110, 150, 70, 30, 90};
  for (int i = 0; i < servoCount; i++) {
    setServoAngle(i, angles[i]);
  }
}

//********************************************************************************************************************************

// Steps Mechanism
// Step 1

void stand_right() {
  int angles[] = {(90 - 40 - 15), (90 - 40 + 20), (90 + 40 - 20), (90 - 40 - 15),
                  (90 + 40 - 15), (90 - 40 + 20), (90 + 40 - 20), (90 + 40 - 15), 
                  (90 + 40 - 20), (90 + 40 + 20), (90 - 40 + 20), (90 - 40 - 20)};
          
  for(int i = 0; i < servoCount; i++) {
    setServoAngle(i, angles[i]);
  }
}

//********************************************************************************************************************************
// Step 2

void stand_rightfoot() {
  int angles[] = {(90 - 40 - 15), (90 - 40 + 20), (90 + 40 - 20), (90 - 40 - 15),
                  (90 + 40 - 15), (90 - 40 + 70), (90 + 40 - 70), (90 + 40 - 15), 
                  (90 + 40 - 20), (90 + 40 + 20), (90 - 40 + 20), (90 - 40 -20)};
              
  for(int i = 0; i < servoCount; i++) {
     setServoAngle(i, angles[i]);
  }
}

//********************************************************************************************************************************
// Step 3

void stand_rightfoot_leftfoot_f() {
  int angles[] = {(90 - 40 - 15), (90 - 40 + 20), (90 + 40 - 20), (90 - 40 - 15),
                  (90 + 40 - 15), (90 - 40 + 70), (90 + 40 - 0), (90 + 40 - 15), 
                  (90 + 40 - 20), (90 + 40 - 20), (90 - 40 + 20), (90 - 40 - 20)};
              
  for(int i = 0; i < servoCount; i++) {
     setServoAngle(i, angles[i]);
  }
}

//********************************************************************************************************************************
// Step 4

void stand_leftfoot_rightfoot_f() {
  int angles[] = {(90 - 40 + 15), (90 - 40 + 0), (90 + 40 - 70), (90 - 40 + 15),
                  (90 + 40 + 15), (90 - 40 + 20), (90 + 40 - 20), (90 + 40 + 15), 
                  (90 + 40 - 20), (90 + 40 + 20), (90 - 40 + 20), (90 - 40 +20)};
              
  for(int i = 0; i < servoCount; i++) {
     setServoAngle(i, angles[i]);
  }
}

//********************************************************************************************************************************
// Step 5

void stand_RfootFront() {
  int angles[] = {(90 - 40 + 0), (90 - 40 + 0), (90 + 40 - 30), (90 - 40 + 0),
                  (90 + 40 + 0), (90 - 40 + 0), (90 + 40 - 30), (90 + 40 + 0), 
                  (90 + 40 - 20), (90 + 40 + 20), (90 - 40 + 20), (90 - 40 + 10)};
              
  for(int i = 0; i < servoCount; i++) {
     setServoAngle(i, angles[i]);
  }
}

//********************************************************************************************************************************
// Step 6

void stand_RfootFront_right() {
  int angles[] = {(90 - 40 - 15), (90 - 40 + 0), (90 + 40 - 30),(90 - 40 - 15),
                  (90 + 40 - 15), (90 - 40 + 0), (90 + 40 - 30), (90 + 40 - 15), 
                  (90 + 40 - 20), (90 + 40 + 20), (90 - 40 + 20), (90 - 40 - 20)};
              
  for(int i = 0; i < servoCount; i++) {
     setServoAngle(i, angles[i]);
  }
}

//********************************************************************************************************************************
// Step 7

void stand_left() {
  int angles[] = {(90 - 40 + 15), (90 - 40 + 20), (90 + 40 - 20), (90 - 40 + 15),
                  (90 + 40 + 15), (90 - 40 + 20), (90 + 40 - 20), (90 + 40 + 15), 
                  (90 + 40 - 20), (90 + 40 + 20), (90 - 40 + 20), (90 - 40 - 20)};

  for(int i = 0; i < servoCount; i++) {
     setServoAngle(i, angles[i]);
  }
}

//********************************************************************************************************************************
// Step 8

void stand_leftfoot() {
  int angles[] = {(90 - 40 + 15), (90 - 40 + 70), (90 + 40 - 70), (90 - 40 + 15),
                  (90 + 40 + 15), (90 - 40 + 20), (90 + 40 - 20), (90 + 40 + 15), 
                  (90 + 40 - 20), (90 + 40 + 20), (90 - 40 + 20), (90 - 40 - 20)};
              
  for(int i = 0; i < servoCount; i++) {
     setServoAngle(i, angles[i]);
  }
}

//********************************************************************************************************************************
// Step 9

void stand_lfootFront() {
  int angles[] = {(90 - 40 - 0), (90 - 40 + 30), (90 + 40 - 0), (90 - 40 - 0),
                  (90 + 40 - 0), (90 - 40 + 30), (90 + 40 - 0), (90 + 40 - 0), 
                  (90 + 40 - 20), (90 + 40 - 10), (90 - 40 + 20), (90 - 40 - 20)};
              
  for(int i = 0; i < servoCount; i++) {
     setServoAngle(i, angles[i]);
  }
}

//********************************************************************************************************************************
// Step 10

void stand_LfootFront_left() {
  int angles[] = {(90 - 40 - 0), (90 - 40 + 30), (90 + 40 - 0), (90 - 40 - 0),
                  (90 + 40 - 0), (90 - 40 + 30), (90 + 40 - 0), (90 + 40 - 0), 
                  (90 + 40 - 20), (90 + 40 - 10), (90 - 40 + 20), (90 - 40 - 20)};
          
  for(int i = 0; i < servoCount; i++) {
     setServoAngle(i, angles[i]);
  }
}

//********************************************************************************************************************************
// Step 11

void stand_rightfoot_leftfoot_b(){
  int angles[] = {(90 - 40 - 10), (90 - 40 + 20), (90 + 40 - 20), (90 - 40 - 10),
                  (90 + 40 - 10), (90 - 40 + 0), (90 + 40 - 70), (90 + 40 - 10),
                  (90 + 40 - 20), (90 + 40 + 20), (90 - 40 + 20), (90 - 40 - 20)};

  for(int i = 0; i < servoCount; i++) {
    setServoAngle(i, angles[i]);
    }
}
//********************************************************************************************************************************
// Step 12

void stand_leftfoot_rightfoot_b(){
  int angles[] = {(90 - 40 + 10), (90 - 40 + 70), (90 + 40 - 0), (90 - 40 + 10),
                  (90 + 40 + 10), (90 - 40 + 20), (90 + 40 - 20), (90 + 40 + 10),
                  (90 + 40 - 20), (90 + 40 + 20), (90 - 40 + 20),(90 - 40 - 20)};
  
  for(int i = 0; i < servoCount; i++) {
    setServoAngle(i, angles[i]);
  }
}

//********************************************************************************************************************************
// Step 13

void stand_RfootFront_left(){
  int angles[] = {(90 - 40 + 10), (90 - 40 + 0), (90 + 40 - 30), (90 - 40 + 10),
                  (90 + 40 + 10), (90 - 40 + 0), (90 + 40 - 30), (90 + 40 + 10), 
                  (90 + 40 - 20), (90 + 40 + 20), (90 - 40 + 20), (90 - 40 - 20)};

  for(int i = 0; i < servoCount; i++) {
    setServoAngle(i, angles[i]);
  }
}

//********************************************************************************************************************************
// Step 14

void stand_LfootFront_right(){
  int angles[] = {(90 - 40 - 10), (90 - 40 + 30), (90 + 40 - 0), (90 - 40 - 10),
                  (90 + 40 - 10), (90 - 40 + 30), (90 + 40 - 0), (90 + 40 - 10),
                  (90 + 40 - 20), (90 + 40 + 20), (90 - 40 + 20), (90 - 40 - 20)};
    
  for (int i = 0; i < 16; i++) {
    setServoAngle(i, angles[i]);
  }
}

//********************************************************************************************************************************
// Step 15

void stand_rfoot2(){
  int angles[]={40, 70, 110, 40,
                110, 50, 130, 110,
                110, 150, 70, 30};
  
  for (int i = 0; i < 16; i++) {
    setServoAngle(i, angles[i]);
  }
}

//********************************************************************************************************************************
// Step 16

void stand_left2(){
  int angles[]={70, 50, 130, 70,
                140, 70, 110, 140,
                110, 150, 70, 30};

  for (int i = 0; i < 16; i++) {
    setServoAngle(i, angles[i]);
  }
}

//********************************************************************************************************************************

void walk(){
  delay(100);
  stand_right();
  delay(100);
  stand_rightfoot();
  delay(100);
  stand_rightfoot_leftfoot_f ();
  delay(100);
  stand_lfootFront();
  delay(100);
  stand_LfootFront_left();
  delay(100);
  stand_leftfoot();
  delay(100);
  stand_leftfoot_rightfoot_f ();
  delay(100);
  stand_RfootFront();
  delay(100);
  stand_RfootFront_right();
  delay(100);
}
//********************************************************************************************************************************

void turn_left(){
  stand2();
  delay(100);
  stand_right();
  delay(100);
  stand_rightfoot();
  delay(100);
  stand_rfoot2();
  delay(100);
  stand3();
  delay(100);
  stand_left2();
  delay(100);
  stand_leftfoot();
  delay(100);
  stand1();
  delay(300);
 }

//********************************************************************************************************************************
void turn_right(){
  stand2();
  delay(100);
  stand_left();
  delay(100);
  stand_leftfoot();
  delay(100);
  stand_left2();
  delay(100);
  stand3();
  delay(100);
  stand_rfoot2();
  delay(100);
  stand_rightfoot();
  delay(100);
  stand1();
  delay(300);
}
//********************************************************************************************************************************
void move_back(){
  delay(100);
  stand_right();
  delay(100);
  stand_rightfoot();
  delay(100);
  stand_rightfoot_leftfoot_b ();
  delay(100);
  stand_RfootFront();
  delay(100);
  stand_RfootFront_left();
  delay(100);
  stand_leftfoot();
  delay(100);
  stand_leftfoot_rightfoot_b ();
  delay(100);
  stand_lfootFront();
  delay(100);
  stand_LfootFront_right();
  delay(100);
}
