/* ==================================================================
Robot Final Project Main: 
Gardener_Robot.h edited by Sophie Robertson | 12/7/2023
=====================================================================

=====================================================================*/
#include <AFMotor.h> // https://learn.adafruit.com/adafruit-motor-shield/af-dcmotor-class
#include "I2Cdev.h"
#include "MPU6050.h"
#include "LinkedList.h"
#include "Math.h"

AF_DCMotor base(1, MOTOR12_1KHZ); //m1: base
AF_DCMotor shoulder(2, MOTOR12_1KHZ); //m2: shoulder
AF_DCMotor elbow(3, MOTOR34_1KHZ); //m3: elbow
AF_DCMotor wrist(4, MOTOR34_1KHZ); //m4: wrist
//AF_DCMotor grabber(4, MOTOR34_1KHZ); //m4: grab

//digital pins
#define ledPin 22
#define limitSW1_PIN 50
#define limitSW2_PIN 51
#define startSW_PIN 53
//analog pins
//pot pins not currently in use
#define basePotPin A0
// digital pins
#define US_L_TRIG_PIN 24 
#define US_L_ECHO_PIN 25
#define US_T_TRIG_PIN 26 
#define US_T_ECHO_PIN 27
#define US_B_TRIG_PIN 28
#define US_B_ECHO_PIN 29

// Anything over 400 cm (23200 us pulse) is "out of range"
#define MAX_DIST 23200

//limit switch (NO = gnd, NC = 5V, COM = digital pin)
int limitSW1 = 0, limitSW2 = 0, startSW = 0;
//infrared sensor (red = 5V, black = gnd, white = analog pin)
int Dist_WL = 0, Dist_WT = 0, Dist_EL = 0, Dist_ET = 0; //distances 
//pots rotate 300 degrees
int basePMax = 0, basePVal = 0; 
// IMU Sensors: I2C, 5V, GND; DI interrupt, AD0 link to 0/1
MPU6050 IMU_BaseElbow(0x69), IMU_ElbowWrist(0x68); // I2C addr: AD0 low = 0x68 (default) | AD0 high = 0x69
int16_t IMU_BaseElbow_ax, IMU_BaseElbow_ay, IMU_BaseElbow_az; //a = accelerometer value
int16_t IMU_ElbowWrist_ax, IMU_ElbowWrist_ay, IMU_ElbowWrist_az; 
int flipped = 0;
linkedlist<float> xVals0;
linkedlist<float> xVals1;
linkedlist<float> yVals0;
linkedlist<float> yVals1;

//movement functions
void baseR(int delayTime) {
  Serial.println("m1:base right"); base.setSpeed(255);
  base.run(FORWARD); digitalWrite(ledPin,HIGH); delay(delayTime); 
  base.setSpeed(0); digitalWrite(ledPin,LOW); delay(100); 
}
void baseL(int delayTime) {
  Serial.println("m1:base left"); base.setSpeed(255);
  base.run(BACKWARD); digitalWrite(ledPin,HIGH); delay(delayTime); 
  base.setSpeed(0); digitalWrite(ledPin,LOW); delay(100); 
}
void shoulderU(int delayTime) {
  Serial.println("m2:shoulder up"); shoulder.setSpeed(255);
  shoulder.run(FORWARD); digitalWrite(ledPin,HIGH); delay(delayTime);
  shoulder.setSpeed(0); digitalWrite(ledPin,LOW); delay(100);
}
void shoulderD(int delayTime) {
  Serial.println("m2:shoulder down"); shoulder.setSpeed(255);
  shoulder.run(BACKWARD); digitalWrite(ledPin,HIGH); delay(delayTime);
  shoulder.setSpeed(0); digitalWrite(ledPin,LOW); delay(100);
}
void elbowU(int delayTime) {
  Serial.println("m3:elbow up"); elbow.setSpeed(255);
  elbow.run(FORWARD); digitalWrite(ledPin,HIGH); delay(delayTime);
  elbow.setSpeed(0); digitalWrite(ledPin,LOW); delay(100);
}
void elbowD(int delayTime) {
  Serial.println("m3:elbow down"); elbow.setSpeed(255);
  elbow.run(BACKWARD); digitalWrite(ledPin,HIGH); delay(delayTime);
  elbow.setSpeed(0); digitalWrite(ledPin,LOW); delay(100);
}
void wristU(int delayTime) {
  Serial.println("m4:wrist up"); wrist.setSpeed(255);
  wrist.run(BACKWARD); digitalWrite(ledPin,HIGH); delay(delayTime);  //takes 8s for full movement i think?
  wrist.setSpeed(0); digitalWrite(ledPin,LOW); delay(100); 
}
void wristD(int delayTime) {
  Serial.println("m4:wrist down"); wrist.setSpeed(255);
  wrist.run(FORWARD); digitalWrite(ledPin,HIGH); delay(delayTime);  //takes 8s for full movement i think?
  wrist.setSpeed(0); digitalWrite(ledPin,LOW); delay(100); 
}

/*
void grab(int delayTime) {
  Serial.println("m4:grab"); grabber.setSpeed(255);
  grabber.run(FORWARD); digitalWrite(ledPin,HIGH); delay(delayTime);
  grabber.setSpeed(0); digitalWrite(ledPin,LOW); delay(100);
}
void ungrab(int delayTime) {
  Serial.println("m4:ungrab"); grabber.setSpeed(255);
  grabber.run(BACKWARD); digitalWrite(ledPin,HIGH); delay(delayTime);
  grabber.setSpeed(0); digitalWrite(ledPin,LOW); delay(100); 
}*/

//DS
float loopUS_DS(int Tpin, int Epin, int printx) {
  unsigned long t1, t2, pulse_width;
  float cm;

  digitalWrite(Tpin, HIGH);
  delayMicroseconds(20);
  digitalWrite(Tpin, LOW);

  while (digitalRead(Epin) == 0);
  t1 = micros();
  while (digitalRead(Epin) == 1);
  t2 = micros();
  pulse_width = t2 - t1;
  if (pulse_width > MAX_DIST) {
    cm = 0.0;
  }
  else {
    cm = pulse_width / 58.0;
  }
  if (printx == 1) {
    if (pulse_width > MAX_DIST) {
      Serial.println("0.0");
    } 
    else {
      Serial.print(cm);
      Serial.print(" cm \t");
    }
  }
  delay(60);
  return cm;
}

//IMU x2
void setupIMU() {
  // join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
  #endif

  // initialize device
  Serial.println("Initializing I2C devices...");
  IMU_BaseElbow.initialize();
  IMU_ElbowWrist.initialize();

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(IMU_BaseElbow.testConnection() ? "MPU6050 IMU_BaseElbow connection successful" : "MPU6050 IMU_BaseElbow connection failed");
  Serial.println(IMU_ElbowWrist.testConnection() ? "MPU6050 IMU_ElbowWrist connection successful" : "MPU6050 IMU_ElbowWrist connection failed");

  // use the code below to change accel/gyro offset values
  Serial.println("Updating internal IMU_BaseElbow sensor offsets...");
  Serial.print("IMU_BaseElbow Accel Offset: "); //1122	-775	1704
  Serial.print(IMU_BaseElbow.getXAccelOffset()); Serial.print("\t"); // -76
  Serial.print(IMU_BaseElbow.getYAccelOffset()); Serial.print("\t"); // -2359
  Serial.print(IMU_BaseElbow.getZAccelOffset()); Serial.print("\t"); // 1688
  Serial.print("\n");
  
  Serial.println("Updating internal IMU_ElbowWrist sensor offsets...");
  Serial.print("IMU_ElbowWrist Accel Offset: "); //-5566	-1899	1414	
  Serial.print(IMU_ElbowWrist.getXAccelOffset()); Serial.print("\t"); 
  Serial.print(IMU_ElbowWrist.getYAccelOffset()); Serial.print("\t"); 
  Serial.print(IMU_ElbowWrist.getZAccelOffset()); Serial.print("\t"); 
  Serial.print("\n");
}

void loopIMU(int printx) {
  // read raw accel measurements from device
  IMU_BaseElbow.getAcceleration(&IMU_BaseElbow_ax, &IMU_BaseElbow_ay, &IMU_BaseElbow_az);
  IMU_BaseElbow_ax = IMU_BaseElbow_ax/1000; IMU_BaseElbow_ay = IMU_BaseElbow_ay/1000; IMU_BaseElbow_az = IMU_BaseElbow_az/1000;
  IMU_ElbowWrist.getAcceleration(&IMU_ElbowWrist_ax, &IMU_ElbowWrist_ay, &IMU_ElbowWrist_az);
  IMU_ElbowWrist_ax = IMU_ElbowWrist_ax/1000; IMU_ElbowWrist_ay = IMU_ElbowWrist_ay/1000; IMU_ElbowWrist_az = IMU_ElbowWrist_az/1000;
  if (printx == 1) {
    Serial.print("IMU_BaEl x / y / z:\t");
    Serial.print(IMU_BaseElbow_ax); Serial.print(" / "); Serial.print(IMU_BaseElbow_ay); Serial.print(" / "); Serial.print(IMU_BaseElbow_az); Serial.print(" \t ");
    Serial.print("IMU_ElWr x / y / z:\t");
    Serial.print(IMU_ElbowWrist_ax); Serial.print(" / "); Serial.print(IMU_ElbowWrist_ay); Serial.print(" / "); Serial.println(IMU_ElbowWrist_az);
  }
  delay(50);
}
void levelElbow() {
  Serial.println("Levelling Elbow");
  loopIMU(1);
  if (flipped == 0) {
    while (IMU_ElbowWrist_ay > 0) { // looking down to floor (elbow 0)
      elbowU(50); // move toward straight
      loopIMU(1);
      if (IMU_ElbowWrist_ay < -12) {// 90 degree
        Serial.println("wrong way");
        while (IMU_ElbowWrist_az < 14) {
          elbowD(50); // move from 0 -> 14 (can try 15/16)
          loopIMU(0);
        }
      }
    }
    if (IMU_ElbowWrist_ay < -12) {// 90 degree
      while (IMU_ElbowWrist_az < 14) {
        elbowD(50); // move from 0 -> 14 (can try 15/16)
        loopIMU(0);
      }
    }
  }
  else if (flipped == 1) {
    while (IMU_ElbowWrist_ay > 0) { // looking down to floor (elbow 180)
      elbowD(50); // move from -16 -> -14 
      loopIMU(1);
      if (IMU_ElbowWrist_ay < -12) { //over 90 degrees
        Serial.println("wrong way");
        while (IMU_ElbowWrist_az > -15) {
          elbowU(50); // move from 0 -> -14
          loopIMU(0);
        }
      }
    }
    if (IMU_ElbowWrist_ay < -12) { //over 90 degrees
      while (IMU_ElbowWrist_az > -14) {
        elbowU(50); // move from 0 -> -14
        loopIMU(1);
      }
    }
  }
}
void RobotStraightUp() { //might need to flip to get out of
  Serial.println("RobotStraightUp: Moving to straight up");
  loopIMU(0);
  if (flipped == 0) {
    while (IMU_BaseElbow_ay < 15) {
      shoulderU(100);
      loopIMU(0);
    }
    shoulderU(100);
    while (IMU_ElbowWrist_ay > -16) {
      elbowU(100);
      loopIMU(0);
    }
  }
  else {
    while (IMU_BaseElbow_ay < 15) {
      shoulderD(100);
      loopIMU(0);
    }
    while (IMU_ElbowWrist_ay > -15) {
      elbowD(100);
      loopIMU(0);
    }
  }
}
void flipRobot() { 
  loopIMU(0);
  if (flipped == 0) { //shoulder 45 elbow 0 -> shoulder 135 elbow 180
    Serial.println("flipRobot: flipping to back side");
    //while (IMU_BaseElbow_ax < 11) { // 45 -> 135 degree
    while (IMU_BaseElbow_ay < 15) { // to 90 degree
      shoulderU(100);
      loopIMU(0);
    }
    shoulderU(200);
    while ((IMU_ElbowWrist_az > -14)||(IMU_ElbowWrist_ay < 0)) { // parallel to floor other way (0 -> 180)
      elbowU(100);
      loopIMU(0);
    }
    flipped = 1;
  }
  else if (flipped == 1) { //shoulder 135 elbow 180 -> shoulder 45 elbow 0
    Serial.println("flipRobot: flipping to front side");
    //while (IMU_BaseElbow_ax > -11) { // 135 -> 45 degree 
    while (IMU_BaseElbow_ay < 15) {  
      shoulderD(100);
      loopIMU(0);
    }
    while ((IMU_ElbowWrist_az < 14)||(IMU_ElbowWrist_ay < 0)) { //parallel to floor (180 -> 0)
      elbowD(100);
      loopIMU(1);
    }
    flipped = 0;
  }
}

float measHeight() {
  float height;
  if (flipped == 0) {
    while (limitSW1 == 0) {
      baseL(50);
      limitSW1 = digitalRead(limitSW1_PIN);
    }
    levelElbow();
    height = loopUS_DS(US_B_TRIG_PIN, US_B_ECHO_PIN, 0);
  }
  else {
    while (limitSW2 == 0) {
      baseR(50);
      limitSW2 = digitalRead(limitSW2_PIN);
    }
    levelElbow();
    height = loopUS_DS(US_T_TRIG_PIN, US_T_ECHO_PIN, 0);
  }
  return height;
}
void goToStart() { //moves base to opposite of lim sw 2
  RobotStraightUp();
  limitSW1 = digitalRead(limitSW1_PIN);
  if (limitSW1 == 1) {
    //at start don't do anything else
  }
  else if (flipped == 0) { //between starting & limitsw1
    while (limitSW1 == 0) { //go to limitsw1
      baseL(100);
      limitSW1 = digitalRead(limitSW1_PIN);
    }
    flipRobot(); //flip
    RobotStraightUp();
    while (limitSW2 == 0) { //travel whole of second half
      baseR(100);
      limitSW2 = digitalRead(limitSW2_PIN);
    }
    flipRobot(); //at start
    RobotStraightUp();
  }
  else if (flipped == 1) {
    while (limitSW2 == 0) { //travel second half
      baseR(100);
      limitSW2 = digitalRead(limitSW2_PIN);
    }
    flipRobot(); //at start
    RobotStraightUp();
  }
  Serial.println("At starting position");
}
void goToTop(int start) { //goes to position: shoulder 45 elbow 0 or shoulder 135 elbow 180 based on flipped 
  loopIMU(0);
  if (start == 0) { //shoulder 45 
    while (IMU_BaseElbow_ax < -11) { // 45 from floor (shoulder 45)
      shoulderU(50); //if under 45 degree from floor
      loopIMU(0);
      if (IMU_BaseElbow_ax > 0) { 
        Serial.println("wrong way");
        while (IMU_BaseElbow_ax > -11) {
          shoulderD(50);
          loopIMU(0);
        }
      }
    }
    //Serial.println("Shoulder good");
    levelElbow();
  }
  else if (start == 1) { //shoulder 135 
    while (IMU_BaseElbow_ax > 11) {
      shoulderD(50); //over 135, move from 16 -> 11
      loopIMU(0);
      if (IMU_BaseElbow_ax < 0) { //flipped wrong way 
        Serial.println("wrong way");
        while (IMU_BaseElbow_ax < 11) {
          shoulderU(50); // under 90, move from 0 -> 11
          loopIMU(0);
        }
      }
    }
    levelElbow();
  }
}

//for output to MATLAB graph
void createXVals() { //from yvals, get how far was travelled in int start from 0
  yVals0.goToHead();
  yVals1.goToHead();
  int currVal0 = round(yVals0.getCurrent());
  int currVal1 = round(yVals1.getCurrent());
  int timeTravL = 0;
  int timeTravR = 0;
  xVals0.append(0);
  xVals1.append(0);
  while (yVals0.next()) {
    int nextVal = round(yVals0.getCurrent());
    if (currVal0 == nextVal) {
      timeTravL = timeTravL + 1; // increase time travelled left
      xVals0.append(timeTravL);
    } 
    else {
      xVals0.append(timeTravL); // have not travelled left
    }
    currVal0 = nextVal;
  }
  while (yVals1.next()) {
    int nextVal = round(yVals1.getCurrent());
    if (currVal1 == nextVal) {
      timeTravR = timeTravR + 1; // increase time travelled left
      xVals1.append(timeTravR);
    } 
    else {
      xVals1.append(timeTravR); // have not travelled left
    }
    currVal1 = nextVal;
  }
}
//used for testing 
void createFakeVals() {
  yVals0.append(0); yVals0.append(0); 
  yVals0.append(3); yVals0.append(3); yVals0.append(3);
  yVals0.append(0); yVals0.append(0);
  yVals0.append(2); yVals0.append(2); yVals0.append(2);
  yVals0.append(0); yVals0.append(0);
  yVals0.append(1); yVals0.append(1);
  yVals0.append(0); yVals0.append(0);
  
  yVals1.append(0); yVals1.append(0);
  yVals1.append(3);  yVals1.append(3);  yVals1.append(3);
  yVals1.append(0);  yVals1.append(0);
  yVals1.append(2);  yVals1.append(2);  yVals1.append(2);
  yVals1.append(0);  yVals1.append(0);
  yVals1.append(1);  yVals1.append(1);
  yVals1.append(0);  yVals1.append(0);
  createXVals();
}