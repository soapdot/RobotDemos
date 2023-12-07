/* ==================================================================
Robot Project Main: 
Robot_Demo3.ino edited by Sophie Robertson | 10/25/2023
=====================================================================
Pressing the green button will start gardening process.
Robot will start pointing to -y
=====================================================================
When a motor is moving, LED on (1s at a time)
When a motor is targeting 0 (stopping), LED off (0.1s may be too fast)
=====================================================================*/
#include <AFMotor.h> // https://learn.adafruit.com/adafruit-motor-shield/af-dcmotor-class
//#include <Gardener_Robot.h>
AF_DCMotor base(1, MOTOR12_1KHZ); //m1: base
AF_DCMotor elbow(2, MOTOR12_1KHZ); //m2: elbow
AF_DCMotor wrist(3, MOTOR34_1KHZ); //m3: wrist
//AF_DCMotor shoulder(4, MOTOR34_1KHZ); //m4: shoulder
AF_DCMotor grabber(4, MOTOR34_1KHZ); //m4: grab

//digital pins
#define ledPin = 13; // the number of the LED pin
#define startSW = 4;
//analog pins
#define basePotPin = A0; 
#define shoulderPotPin = A1;
#define elbowPotPin = A2; 
#define wristPotPin = A3;
#define grabberPotPin = A4;
#define DS_WL_Pin = A7; //wrist left
#define DS_EL_Pin = A8; //elbow left
#define DS_WT_Pin = A9; //wrist top
#define DS_ET_Pin = A10; //elbow top 

//limit switch
int SW1 = 0;
//infrared sensor = 5v, ground, analog pin
int Dist_WL = 0, Dist_WT = 0, Dist_EL = 0; //distances 

//pots rotate 300 degrees
int basePMax = 0, shoulderPMax = 0, elbowPMax = 0, wristPMax = 0, grabPMax = 0;
int basePVal = 0, shoulderPVal = 0, elbowPVal = 0, wristPVal = 0, grabPVal = 0;

void notsetup() { 
  Serial.begin(38400);
  pinMode(startSW,INPUT);
  pinMode(ledPin,OUTPUT); 
  pinMode(DS_WL_Pin,OUTPUT);
  pinMode(DS_EL_Pin,OUTPUT);
  pinMode(basePotPin,OUTPUT);
  base.setSpeed(255);
  //shoulder.setSpeed(255);
  elbow.setSpeed(255);
  wrist.setSpeed(255);
  grabber.setSpeed(255);
}
void notloop() {
  SW1.loop();
  SW2.loop();

  while ((SW1.isPressed() == false)&&(SW2.isPressed() == false)) {
    //update distance sensors
    if ((DS_WL == false) && (DS_EL == false)) {
      baseL(100);
    }
    else {
      while ((DS_WL == true)&&(elbowPVal < 50)) {
        elbowU(100); 
      }
      while ((DS_EL == true)&&(shoulderPVal < 50)) {
        shoulderU(100);
      }
    }
  }
  if ((SW1.isPressed() == true)) {
    //flip 180 degrees (need pots)
    while (shoulderPVal < 70) { //some 30% up on other side
      shoulderU(100);
    }
    while (elbowPVal < 90) { //some 10% up on other side (parallel w floor or so)
      elbowU(100);
    }
    //shuffle top/bottom sensors
    
  }
  else if ((SW2.isPressed() == true)) {
    //flip 180 degrees (need pots)
    while (shoulderPVal > 30) { //some 30% up from floor
      shoulderD(100);
    }
    while (elbowPVal > 10) { //some 10% up from floor
      elbowD(100);
    }
    //shuffle top/bottom sensors
  }

}

void setup() {
  Serial.begin(38400);
  pinMode(ledPin,OUTPUT); // initialize the LED pin as an output:
  pinMode()
}

void loop() {

}