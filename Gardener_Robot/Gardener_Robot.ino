/* ==================================================================
Robot Final Project Main: 
Gardener_Robot.ino edited by Sophie Robertson | 12/7/2023
=====================================================================
I believe another team is doing an actual watering system better,
so I focused on monitoring/mapping obstacles (plants) and their heights.

With this information, could figure out which plants are below avg &
make sure they get proper nutrition, or base nutrition on height.
=====================================================================
Import results into MATLAB to get 2d plot of obstacle heights
=====================================================================
Uses:
-OWI Robot arm 
-6-axis IMU Sensor x2:          Base-Elbow (BE) & Elbow-Wrist (EW)
-Ultrasonic Distance Sensor x2: Top_BE & Bot_BE 
=====================================================================
flipped = 0:  BE = -11,11,0    EW = 0,0,16
flipped = 1:  BE =  11,11,0    EW = 0,0,-16
=====================================================================*/
#include "Gardener_Robot.h"
float heightOfRobot = 0.0; 

void SafeTravels() {
  yVals0.goToHead(); // heights
  xVals0.goToHead(); // travel left time in sets of 50us 
  int currVal0 = xVals0.getCurrent();
  int currVal1 = xVals1.getCurrent();
  int matchedHeight0 = yVals0.getCurrent();
  int matchedHeight1 = yVals1.getCurrent();
  while (xVals0.next()) {
    matchedHeight0 = yVals0.getCurrent();
    int nextVal = xVals0.getCurrent();
    float distOfObs = 0;
    if (currVal0 == nextVal) { //no time travelled left, must've travelled up
      distOfObs = loopUS_DS(US_L_TRIG_PIN, US_L_ECHO_PIN,0);
      //TODO: could use height -> travel up time, some sort of map
      while (loopUS_DS(US_L_TRIG_PIN, US_L_ECHO_PIN,0) <= distOfObs + 1) { //while obstacle is still to the left
        shoulderU(50); // move up
        levelElbow(); // level elbow to keep reading consistent //TODO: maybe don't need 
      }
    }
    else {
      baseL(50);
      limitSW1 = digitalRead(limitSW1_PIN);
      if (limitSW1 == 1) {
        flipRobot();
      }
    }
    currVal0 = nextVal;
  }
  while (xVals1.next()) {
    matchedHeight1 = yVals1.getCurrent();
    int nextVal = xVals1.getCurrent();
    float distOfObs = 0;
    if (currVal1 == nextVal) { //no time travelled right, must've travelled up
      distOfObs = loopUS_DS(US_L_TRIG_PIN, US_L_ECHO_PIN,0);
      //TODO: could use height -> travel up time, some sort of map
      while (loopUS_DS(US_L_TRIG_PIN, US_L_ECHO_PIN,0) <= distOfObs + 1) { //while obstacle is still to the left
        shoulderU(50); // move up
        levelElbow(); // level elbow to keep reading consistent //TODO: maybe don't need 
      }
    }
    else {
      baseR(50);
      limitSW2 = digitalRead(limitSW2_PIN);
      if ((limitSW2 == 1)||!(xVals1.next())) {
        flipRobot();
      }
      xVals1.prev();
    }
    currVal1 = nextVal;
  }
}

void createYVals() { 
  Serial.print("Starting YVals0 Measurement");
  float heightOfRobot = loopUS_DS(US_B_TRIG_PIN, US_B_ECHO_PIN,0); //finds height
  while (flipped == 0) {
    float cmFromRobot = loopUS_DS(US_B_TRIG_PIN, US_B_ECHO_PIN,0); //cm from US sensor (cm)
    float cmObs_H = heightOfRobot - cmFromRobot; //height of obstacle 
    Serial.println(cmObs_H);
    yVals0.append(cmObs_H);
    baseL(100);
    delay(100);
    limitSW1 = digitalRead(limitSW1_PIN);
    if (limitSW1 == 1) {
      flipRobot(); //should escape loop
      RobotStraightUp();
      shoulderD(500);
      levelElbow();
      elbowU(800);
    }
  }
  Serial.print("Starting YVals1 Measurement");
  heightOfRobot = loopUS_DS(US_T_TRIG_PIN, US_T_ECHO_PIN,0); //finds height (flipped)
  while (flipped == 1) {
    float cmFromRobot = loopUS_DS(US_T_TRIG_PIN, US_T_ECHO_PIN,0); //cm from US sensor (cm)
    float cmObs_H = heightOfRobot - cmFromRobot; //height of obstacle 
    Serial.println(cmObs_H);
    yVals1.append(cmObs_H);
    baseR(50);
    delay(100);
    limitSW2 = digitalRead(limitSW2_PIN);
    if (limitSW2 == 1) {
      flipRobot(); //should escape loop
      RobotStraightUp();
      levelElbow();
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(limitSW1_PIN,INPUT);
  pinMode(limitSW2_PIN,INPUT);
  pinMode(startSW_PIN, INPUT);
  pinMode(US_L_ECHO_PIN, INPUT);
  pinMode(US_T_ECHO_PIN, INPUT);
  pinMode(US_B_ECHO_PIN, INPUT);
  pinMode(ledPin,OUTPUT);
  pinMode(basePotPin,OUTPUT);
  pinMode(US_L_TRIG_PIN, OUTPUT);
  pinMode(US_T_TRIG_PIN, OUTPUT);
  pinMode(US_B_TRIG_PIN, OUTPUT);
  digitalWrite(US_L_TRIG_PIN, LOW);
  digitalWrite(US_T_TRIG_PIN, LOW);
  digitalWrite(US_B_TRIG_PIN, LOW);
  setupIMU();
  elbowD(4000);
  flipped = 1;
  flipRobot();
  flipped = 0;
    goToStart(); //move robot to start position (arm will be up)
  levelElbow(); //move elbow to parallel to floor
  Serial.println("ready in 1");
  delay(1000);
}

void loop() {
  limitSW1 = digitalRead(limitSW1_PIN);
  limitSW2 = digitalRead(limitSW2_PIN);
  startSW = digitalRead(startSW_PIN);
  if (startSW == 1) {
    Serial.println("Start measuring obstacle heights");
    createYVals(); //move & get data on what's beneath
    createXVals(); //based on heights, create matrix of when movement occurred
    //arduino does not have a file system, tried to use outside libraries to no avail
    //will just copy from serial to matlab
    Serial.print("xVals0 = ["); xVals0.printList(); xVals0.clear();
    Serial.print("xVals1 = ["); xVals1.printList(); xVals1.clear();
    Serial.print("yVals0 = ["); yVals0.printList(); yVals0.clear();
    Serial.print("yVals1 = ["); yVals1.printList(); yVals1.clear();
    while (true) {
      //endless loop
    }
  }
  //baseR(500);
  //loopUS_DS(US_L_TRIG_PIN, US_L_ECHO_PIN);
  Serial.print("TOP USDS: ");
  loopUS_DS(US_T_TRIG_PIN, US_T_ECHO_PIN,1);
  Serial.print("BOT USDS: ");
  loopUS_DS(US_B_TRIG_PIN, US_B_ECHO_PIN,1);
  Serial.print("LEFT USDS: ");
  loopUS_DS(US_L_TRIG_PIN, US_L_ECHO_PIN,1);
  Serial.println();
  loopIMU(0);
  //delay(1000);
  
}