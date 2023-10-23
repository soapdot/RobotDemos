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
AF_DCMotor base(1, MOTOR12_1KHZ); //m1: base
AF_DCMotor elbow(2, MOTOR12_1KHZ); //m2: elbow
AF_DCMotor wrist(3, MOTOR34_1KHZ); //m3: wrist
AF_DCMotor grabber(4, MOTOR34_1KHZ); //m4: grab

const int ledPin = 53; // the number of the LED pin
const int basePotPin = A0; //analog pin
const int shoulderPotPin = A1; //analog pin
const int elbowPotPin = A2; //analog pin
const int wristPotPin = A3; //analog pin
const int grabberPotPin = A4; //analog pin

int basePVal = 0, shoulderPVal = 0, elbowPVal = 0, wristPVal = 0, grabberPVal = 0;

void updatePots() {
  basePVal = analogRead(basePotPin);
  shoulderPVal = analogRead(shoulderPotPin);
  elbowPVal = analogRead(elbowPotPin);
  wristPVal = analogRead(wristPotPin);
  grabberPVal = analogRead(grabberPotPin);
}

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
void elbowU(int delayTime) {
  Serial.println("m2:elbow up"); elbow.setSpeed(255);
  elbow.run(BACKWARD); digitalWrite(ledPin,HIGH); delay(delayTime);
  elbow.setSpeed(0); digitalWrite(ledPin,LOW); delay(100);
}
void elbowD(int delayTime) {
  Serial.println("m2:elbow down"); elbow.setSpeed(255);
  elbow.run(FORWARD); digitalWrite(ledPin,HIGH); delay(delayTime);
  elbow.setSpeed(0); digitalWrite(ledPin,LOW); delay(100);
}
void wristU(int delayTime) {
  Serial.println("m3:wrist up"); wrist.setSpeed(255);
  wrist.run(BACKWARD); digitalWrite(ledPin,HIGH); delay(delayTime);  //takes 8s for full movement i think?
  wrist.setSpeed(0); digitalWrite(ledPin,LOW); delay(100); 
}
void wristD(int delayTime) {
  Serial.println("m3:wrist down"); wrist.setSpeed(255);
  wrist.run(FORWARD); digitalWrite(ledPin,HIGH); delay(delayTime);  //takes 8s for full movement i think?
  wrist.setSpeed(0); digitalWrite(ledPin,LOW); delay(100); 
}
void grab(int delayTime) {
  Serial.println("m4:grab"); grabber.setSpeed(255);
  grabber.run(FORWARD); digitalWrite(ledPin,HIGH); delay(delayTime);
  grabber.setSpeed(0); digitalWrite(ledPin,LOW); delay(100); 
}
void ungrab(int delayTime) {
  Serial.println("m4:ungrab"); grabber.setSpeed(255);
  grabber.run(BACKWARD); digitalWrite(ledPin,HIGH); delay(delayTime);
  grabber.setSpeed(0); digitalWrite(ledPin,LOW); delay(100); 
}
void setup() { // put your setup code here, to run once:
  Serial.begin(38400);
  pinMode(ledPin,OUTPUT); // initialize the LED pin as an output:
  base.setSpeed(255);
  elbow.setSpeed(255);
  wrist.setSpeed(255);
  grabber.setSpeed(255);
}
void loop() { // put your main code here, to run repeatedly:
  //start at down ungrabbed @ starting point
  updatePots(); //update all 5 potentiometers 
  for (int i=0;i<5;i++) {
    //grab and move right 5 cm+, let go
    grab(1000);   //grab
    wristU(3000); //move wrist up
    elbowU(2000); //move elbow up (2s)
    baseR(7000);  //move base right (7s) 5cm ?
    wristD(3000); //move wrist down
    elbowD(2000); //move elbow down 
    ungrab(1000); //ungrab
    delay(10000); //wait 10s for user to mark block
    //move left 5cm+, ready for next grab
    baseL(7500);  //move base left (7s) 5cm ?
  }
  delay(10000);
}