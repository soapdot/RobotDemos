/* ==================================================================
Robot Project Main: 
Robot_Demo3.ino edited by Sophie Robertson | 10/25/2023
=====================================================================
Code will grab, pick up wrist, pick up elbow, move to the right 5cm
then put down elbow, put down wrist, and release. delay 10s. 
Move back to pickup location 
x5
=====================================================================
When a motor is moving, LED on (1s at a time)
When a motor is targeting 0 (stopping), LED off (0.1s may be too fast)
=====================================================================*/
#include <AFMotor.h> // https://learn.adafruit.com/adafruit-motor-shield/af-dcmotor-class
AF_DCMotor base(1, MOTOR12_1KHZ); //m1: base
AF_DCMotor elbow(2, MOTOR12_1KHZ); //m2: elbow
AF_DCMotor wrist(3, MOTOR34_1KHZ); //m3: wrist
//AF_DCMotor shoulder(4, MOTOR34_1KHZ); //m4: shoulder
AF_DCMotor grabber(4, MOTOR34_1KHZ); //m4: grab

const int ledPin = 53; // the number of the LED pin
const int basePotPin = A0; //analog pin
const int shoulderPotPin = A1; //analog pin
const int elbowPotPin = A2; //analog pin
const int wristPotPin = A3; //analog pin
const int grabPotPin = A4; //analog pin

//pots rotate 300 degrees
int basePMax = 0, shoulderPMax = 0, elbowPMax = 0, wristPMax = 0, grabPMax = 0;
int basePVal = 0, shoulderPVal = 0, elbowPVal = 0, wristPVal = 0, grabPVal = 0;

void updatePots() {
  basePVal = analogRead(basePotPin);
  shoulderPVal = analogRead(shoulderPotPin);
  elbowPVal = analogRead(elbowPotPin);
  wristPVal = analogRead(wristPotPin);
  grabPVal = analogRead(grabPotPin);
}

void baseR(int delayTime) {
  Serial.println("m1:base right"); base.setSpeed(255);
  base.run(FORWARD); digitalWrite(ledPin,HIGH); delay(delayTime); 
  base.setSpeed(0); digitalWrite(ledPin,LOW); delay(100); 
  updatePots(); //update all 5 potentiometers 
}
void baseL(int delayTime) {
  Serial.println("m1:base left"); base.setSpeed(255);
  base.run(BACKWARD); digitalWrite(ledPin,HIGH); delay(delayTime); 
  base.setSpeed(0); digitalWrite(ledPin,LOW); delay(100); 
  updatePots(); //update all 5 potentiometers 
}
void elbowU(int delayTime) {
  Serial.println("m2:elbow up"); elbow.setSpeed(255);
  elbow.run(BACKWARD); digitalWrite(ledPin,HIGH); delay(delayTime);
  elbow.setSpeed(0); digitalWrite(ledPin,LOW); delay(100);
  updatePots(); //update all 5 potentiometers 
}
void elbowD(int delayTime) {
  Serial.println("m2:elbow down"); elbow.setSpeed(255);
  elbow.run(FORWARD); digitalWrite(ledPin,HIGH); delay(delayTime);
  elbow.setSpeed(0); digitalWrite(ledPin,LOW); delay(100);
  updatePots(); //update all 5 potentiometers 
}
void wristU(int delayTime) {
  Serial.println("m3:wrist up"); wrist.setSpeed(255);
  wrist.run(BACKWARD); digitalWrite(ledPin,HIGH); delay(delayTime);  //takes 8s for full movement i think?
  wrist.setSpeed(0); digitalWrite(ledPin,LOW); delay(100); 
  updatePots(); //update all 5 potentiometers 
}
void wristD(int delayTime) {
  Serial.println("m3:wrist down"); wrist.setSpeed(255);
  wrist.run(FORWARD); digitalWrite(ledPin,HIGH); delay(delayTime);  //takes 8s for full movement i think?
  wrist.setSpeed(0); digitalWrite(ledPin,LOW); delay(100); 
  updatePots(); //update all 5 potentiometers 
}
void grab(int delayTime) {
  Serial.println("m4:grab"); grabber.setSpeed(255);
  grabber.run(FORWARD); digitalWrite(ledPin,HIGH); delay(delayTime);
  grabber.setSpeed(0); digitalWrite(ledPin,LOW); delay(100); 
  updatePots(); //update all 5 potentiometers 
}
void ungrab(int delayTime) {
  Serial.println("m4:ungrab"); grabber.setSpeed(255);
  grabber.run(BACKWARD); digitalWrite(ledPin,HIGH); delay(delayTime);
  grabber.setSpeed(0); digitalWrite(ledPin,LOW); delay(100); 
  updatePots(); //update all 5 potentiometers 
}
void setup() { // put your setup code here, to run once:
  Serial.begin(38400);
  pinMode(ledPin,OUTPUT); // initialize the LED pin as an output:
  base.setSpeed(255);
  //shoulder.setSpeed(255);
  elbow.setSpeed(255);
  wrist.setSpeed(255);
  grabber.setSpeed(255);
}
void loop() { // put your main code here, to run repeatedly:
  //start at down ungrabbed @ starting point
  updatePots(); //update all 5 potentiometers 
  /*
  for (int i=0;i<5;i++) {
    //grab and move right 5 cm+, let go
    while (grabPVal > 100) { //until grabber is closed (ish)
      grab(100);             //grab 
    }
    while (wristPVal < 300) { //until wrist is 1/3rd way up
      wristU(100);            //move wrist up
    }
    while (elbowPVal < 300) { //until elbow is 1/3rd way up
      elbowU(100);            //move elbow up (2s)
    }
    while (basePVal < 600) {
      baseR(100);  //move base right 2/3 of the way
    }
    while (elbowPVal > 100) { //until elbow is 1/3rd way up
      elbowD(100); //move elbow down 
    }
    while (wristPVal > 100) { //until wrist is 1/3rd way up
      wristU(100);            //move wrist up
    }
    while (grabPVal < 600) { //until grabber is open
      ungrab(100); //ungrab
    }
    delay(10000); //wait 10s for user to mark block
    //move left 5cm+, ready for next grab
    while (basePVal > 100) {
      baseL(100);  //move base left (7s) 5cm ?
    }
  }*/
  while (basePVal < 600) {
    baseR(100);  //move base right 2/3 of the way
  }
  delay(10000);
}