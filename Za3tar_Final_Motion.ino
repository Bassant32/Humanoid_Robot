#include <WiFi.h>
#include <Adafruit_PWMServoDriver.h>

const char* ssid = "Mahmoud's Galaxy A24";
const char* password = "narnar04";

uint8_t cmd = 0;
float depth = 0 ;
WiFiServer server(1234);  // Port Number used by the ESP32

//PCA9685 I2C address (default 0x40)
Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver(0x40);


//ESP32 I2C pins
#define SDA_PIN 21
#define SCL_PIN 22

//Servo min/max pulse lengths
#define servoMin 205
#define servoMax 410

//Servo channels
int channels[8] = {0, 1, 2, 3, 4, 5, 6, 7};  

//store current angles
int currentAngles[8] = {90, 90, 90, 90, 90, 90, 90, 90};  

//Standing angles
int initial_pos[8] = {85, 95, 90, 90, 70, 100, 95, 85};       // R0, L1, R2, L3, R4, L5, R6, L7


/**************************** convert angle → PWM ****************************************/
void moveServo(int ch, int angle) {
  int pulse = map(angle, 0, 180, servoMin, servoMax);
  pca.setPWM(ch, 0, pulse);
}

/************************* smooth movement (increment) **********************************/
void smoothMove(int ch, int targetAngle)
{
  int current = currentAngles[ch];

  if (targetAngle > current) {
    for (int a = current; a <= targetAngle; a++) {
      moveServo(ch, a);
      delay(5);
    }
  }
  else {
    for (int a = current; a >= targetAngle; a--) {
      moveServo(ch, a);
      delay(5);
    }
  }

  currentAngles[ch] = targetAngle;
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  pca.begin();
  pca.setPWMFreq(50);

  delay(500);

  stand();


  // Connecting over WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("Server started, waiting for clients...");
}

void loop() {
  WiFiClient client = server.available(); // Checking whether a client is connected or not

  if (client) {
    Serial.println("Client connected!");
    
    while (client.connected()) { // As long asA the client is still connecting
      while (client.available()) { // Data is being sent by the client
        cmd = client.read();   // Reads only one byte
        Serial.print("Received int: ");
        Serial.println(cmd);

        Serial.print("Depth = ");
        Serial.println(depth);
        

        switch (cmd){

          case 0 :
            stand();
            break ;

          // case 1 : // Right
          //   // turnRight();
          //   stand();
          //   break;

          case 2 : // left
            turnLeft();  
            stand();
            break;

          case 3 : // Center
            walkForwardRight();
            walkForwardLeft();  
            break;
        }
      }
    }

    client.stop();
    // Serial.println("Client disconnected.");


  }
  
  

}

//******************************************************************************************************




/**************************************** Initial stand ******************************************/


void stand() {

  // sync current angles to standing
  for(int i=0; i<8; i++){
    smoothMove(channels[i], initial_pos[i]);
  }

  delay(5000);
}



/*************************************** Forward Right Step *****************************************/
void walkForwardRight(){
  int angle0 = initial_pos[0];
  int angle1 = initial_pos[1];
  int angle2 = initial_pos[2];
  int angle3 = initial_pos[3];
  int angle4 = initial_pos[4];
  int angle5 = initial_pos[5];
  int angle6 = initial_pos[6];
  int angle7 = initial_pos[7];

  /************************************** Weight Shifting  Loop-1 *******************************************/
  
  for(int i = 0 ; i < 9 ; i++){
    
    angle1 -= 5;
    angle7 += 5;
    angle0 -= 5;
    angle6 += 5;

    smoothMove(channels[1], angle1);    // Left hip in L1
    smoothMove(channels[7], angle7);    // Left foot in L7
    smoothMove(channels[0], angle0);    // Right Hip out R0
    smoothMove(channels[6], angle6);    // Right Foot out R6  
  }
  
  /*********************************** Right Leg steps forward  Loop-2 ***************************************/

  for(int i = 0 ; i < 30 ; i++){
    
    angle2 -= 3;
    angle4 -= 3;
    
    smoothMove(channels[2], angle2);    // Right Thigh forward R2
    smoothMove(channels[4], angle4);    // Right Knee backward R4
  }

  /********************************** Placing the Right Leg back on the ground  Loop-3 ***********************/

  for(int i = 0 ; i < 7 ; i++){
     
    
    angle2 += 5;
    angle3 -= 5;
    angle5 -= 5;
    angle4 += 5;

    smoothMove(channels[3], angle3);    // Left Thigh backward L3
    smoothMove(channels[2], angle2);    // Right Thigh backward R2
    smoothMove(channels[5], angle5);    // Left Knee forward L5
    smoothMove(channels[4], angle4);    // Right Knee forward R4
  }

  /************************************* Shifting the weight to the Right Leg **************************/

  for(int i = 0 ; i < 7 ; i++){

    angle0 += 5;
    angle1 += 5;
    angle6 -= 5;
    angle7 -= 5;


    smoothMove(channels[0], angle0);    // Right Hip in R0
    smoothMove(channels[1], angle1);    // Left Hip out L1
    smoothMove(channels[6], angle6);    // Right Foot out R6
    smoothMove(channels[7], angle7);    // Left Foot in L7
  }

  for(int i = 0 ; i < 4 ; i++){
    angle6 -= 3;
    angle7 -= 3;
    angle0 += 3;
    angle1 += 3;
    angle2 += 20;  
    angle4 += 10;
    angle5 -= 5;

    smoothMove(channels[6], angle6);    // Right Foot out R6
    smoothMove(channels[7], angle7);    // Right Foot in L7
    smoothMove(channels[0], angle0);    // Right Hip in R0
    smoothMove(channels[1], angle1);    // Left Hip out L1
    smoothMove(channels[2], angle2);    // Right Thigh backward R2
    smoothMove(channels[4], angle4);    // Right knee forward R4
    smoothMove(channels[5], angle5);    // Left knee forward L5
  }

  for(int i = 0 ; i < 9 ; i++){
    if(i >= 0 && i < 7){
      angle3 += 3;
      angle2 -= 3;
    }
    
    angle0 += 3;
    angle1 += 5;
    angle6 -= 5;
    angle7 -= 5;
    
    smoothMove(channels[2], angle2);    // Right Thigh in R2
    smoothMove(channels[3], angle3);    // Left Thigh in L3
    smoothMove(channels[0], angle0);    // Right Hip in R0
    smoothMove(channels[1], angle1);    // Left Hip out L1
    smoothMove(channels[6], angle6);    // Right Foot out R6
    smoothMove(channels[7], angle7);    // Left Foot in L7  
  }

  for(int i =0 ; i< 7 ; i++){
    angle3 += 2;
    angle5 += 8;
    angle4 += 2;
    smoothMove(channels[3], angle3);    // Left Thigh in L3
    smoothMove(channels[5], angle5);    // Left Knee in L5
    smoothMove(channels[4], angle4);    // Right Knee forward R4
    
  }

// End of the Right Step

//******************************************************************
// Performance is better without the following angle correction
//******************************************************************

// // Setup for the Left Step

// angle0 -= 2;
// angle1 -= 2;
// angle2 -= 4;
// angle4 += 1;
// angle5 -= 1;
// angle6 += 2;
// angle7 += 2;
// smoothMove(channels[0], angle0);    
// smoothMove(channels[1], angle1);    
// smoothMove(channels[2], angle2);    
// smoothMove(channels[5], angle5);    
// smoothMove(channels[6], angle6);    
// smoothMove(channels[7], angle7);
}



/***********************************************************************************************************/

/***********************************************************************************************************/

/***********************************************************************************************************/



void walkForwardLeft(){
/*********************************** Left Leg steps forward  Loop-2 ***************************************/
  int angle0 = 130;
  int angle1 = 140;
  int angle2 = 90;
  int angle3 = 90;
  int angle4 = 70;
  int angle5 = 100;
  int angle6 = 50;
  int angle7 = 40;
  for(int i = 0 ; i < 30 ; i++){
    
    angle3 += 3;
    angle5 += 3;
    
    smoothMove(channels[3], angle3);    // Left Thigh forward L3
    smoothMove(channels[5], angle5);    // Left Knee backward L5
  }

  /********************************** Placing the Left Leg back on the ground  Loop-3 ***********************/

  for(int i = 0 ; i < 7 ; i++){
     
    
    angle3 -= 5;
    angle2 += 2;
    angle4 += 2;
    angle5 -= 5;

    smoothMove(channels[3], angle3);    // Left Thigh forward L3
    smoothMove(channels[2], angle2);    // Right Thigh forward R2
    smoothMove(channels[5], angle5);    // Left Knee backward L5
    smoothMove(channels[4], angle4);    // Right Knee backward R4
  }

  /************************************* Shifting the weight to the Left Leg **************************/

  for(int i = 0 ; i < 7 ; i++){

    angle0 -= 5;
    angle1 -= 5;
    angle6 += 5;
    angle7 += 5;


    smoothMove(channels[0], angle0);    // Right Hip out R0
    smoothMove(channels[1], angle1);    // Left Hip in L1
    smoothMove(channels[6], angle6);    // Right Foot in R6
    smoothMove(channels[7], angle7);    // Left Foot out L7
  }

  for(int i = 0 ; i < 4 ; i++){
    angle6 += 3;
    angle7 += 3;
    angle0 -= 3;
    angle1 -= 3;
    angle3 -= 10;  
    angle4 += 5;
    angle5 -= 10;

    smoothMove(channels[6], angle6);    // Right Foot in R6
    smoothMove(channels[7], angle7);    // Right Foot out L7
    smoothMove(channels[0], angle0);    // Right Hip out R0
    smoothMove(channels[1], angle1);    // Left Hip in L1
    smoothMove(channels[3], angle3);    // Right Thigh forward R2
    smoothMove(channels[4], angle4);    // Right knee backward R4
    smoothMove(channels[5], angle5);    // Left knee backward L5
  }

  for(int i = 0 ; i < 9 ; i++){
     if(i >= 0 && i < 7){
      angle3 -= 3;
      angle2 += 3;
    }
    smoothMove(channels[2], angle2);    // Right Thigh out R2
    smoothMove(channels[3], angle3);    // Left Thigh out L3
  }

  for(int i =0 ; i< 7 ; i++){
    
    angle5 -= 2;

    smoothMove(channels[5], angle5);    // Left Knee out L5
  }


}

// End of the Left Step


void turnLeft(){

  int angle0 = initial_pos[0];
  int angle1 = initial_pos[1];
  int angle2 = initial_pos[2];
  int angle3 = initial_pos[3];
  int angle4 = initial_pos[4];
  int angle5 = initial_pos[5];
  int angle6 = initial_pos[6];
  int angle7 = initial_pos[7];

  /************************************** Weight Shifting  Loop-1 *******************************************/
  
  for(int i = 0 ; i < 9 ; i++){
    
    angle1 -= 5;
    angle7 += 5;
    angle0 -= 5;
    angle6 += 5;

    smoothMove(channels[1], angle1);    // Left hip out
    smoothMove(channels[7], angle7);    // Left foot out
    smoothMove(channels[0], angle0);    // Right hip in
    smoothMove(channels[6], angle6);    // Right foot in  
  }

  /*********************************** Right Leg steps forward  Loop-2 ***************************************/

  for(int i = 0 ; i < 30 ; i++){
    
    angle2 -= 3;
    angle4 -= 3;
    
    smoothMove(channels[2], angle2);    // Left Thigh forward
    smoothMove(channels[4], angle4);    // Left Knee backward
  }

  /********************************** Placing the Right Leg back on the ground  Loop-3 ***********************/

  for(int i = 0 ; i < 7 ; i++){
     
    angle3 -= 5;
    angle2 += 5;
    angle4 += 5;
    angle5 -= 5;

    smoothMove(channels[3], angle3);    // Right Thigh backward
    smoothMove(channels[2], angle2);    // Left Thigh backward
    smoothMove(channels[4], angle4);    // Left Knee forward
    smoothMove(channels[5], angle5);    // Right Knee forward
  }

  /************************************* Shifting the weight to the Left Leg **************************/

  for(int i = 0 ; i < 7 ; i++){

    angle0 += 5;
    angle1 += 5;
    angle6 -= 5;
    angle7 -= 5;


    smoothMove(channels[0], angle0);    // Right Hip in R0
    smoothMove(channels[1], angle1);    // Left Hip out L1
    smoothMove(channels[6], angle6);    // Right Foot out R6
    smoothMove(channels[7], angle7);    // Left Foot in L7
  }

  for(int i = 0 ; i < 4 ; i++){
    angle6 -= 3;
    angle7 -= 3;
    angle0 += 3;
    angle1 += 3;
    angle2 += 20;  
    angle4 += 10;
    angle5 -= 5;

    smoothMove(channels[6], angle6);    // Right Foot out R6
    smoothMove(channels[7], angle7);    // Right Foot in L7
    smoothMove(channels[0], angle0);    // Right Hip in R0
    smoothMove(channels[1], angle1);    // Left Hip out L1
    smoothMove(channels[2], angle2);    // Right Thigh backward R2
    smoothMove(channels[4], angle4);    // Right knee forward R4
    smoothMove(channels[5], angle5);    // Left knee forward L5
  }

  for(int i = 0 ; i < 9 ; i++){
    if(i >= 0 && i < 7){
      angle3 += 3;
      angle2 -= 3;
    }
    
    angle0 += 5;
    angle1 += 5;
    angle6 -= 5;
    angle7 -= 5;
    
    smoothMove(channels[2], angle2);    // Right Thigh in R2
    smoothMove(channels[3], angle3);    // Left Thigh in L3
    smoothMove(channels[0], angle0);    // Right Hip in R0
    smoothMove(channels[1], angle1);    // Left Hip out L1
    smoothMove(channels[6], angle6);    // Right Foot out R6
    smoothMove(channels[7], angle7);    // Left Foot in L7  
  }

  for(int i =0 ; i< 7 ; i++){
    angle3 += 2;
    angle5 += 8;
    angle4 += 2;
    smoothMove(channels[3], angle3);    // Left Thigh in L3
    smoothMove(channels[5], angle5);    // Left Knee in L5
    smoothMove(channels[4], angle4);    // Right Knee forward R4
    
  }

  angle0 -= 2;
  angle1 -= 2;
  angle2 -= 4;
  angle4 += 1;
  angle5 -= 1;
  angle6 += 2;
  angle7 += 2;


  smoothMove(channels[0], angle0);    
  smoothMove(channels[1], angle1);    
  smoothMove(channels[2], angle2);    
  smoothMove(channels[5], angle5);    
  smoothMove(channels[6], angle6);    
  smoothMove(channels[7], angle7);    
  Serial.println(angle3);
  Serial.println(angle4);
  Serial.println(angle5);

}