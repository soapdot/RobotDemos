/* ==================================================================
Robot Project Main: 
Robot_Demo2.ino edited by Sophie Robertson | 9/28/2023
=====================================================================
Code will grab, pick up wrist, pick up elbow, move to the right
then put down elbow, put down wrist, and release. delay 10s. (repeat 5x)

Then, code will grab and move left 5 times to reset to beginning. delay 10s.
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
  //start at down ungrabbed pointing left
  for (int i=0;i<5;i++) {
    //move elbow up (2s)
    elbowU(2000);
    //move wrist up/down
    wristU(3000);
    wristD(3000);
    //move elbow down 
    elbowD(2000);
    //try 4th motor
    ungrab(1000);
    grab(1000);
    //move base R (1s)
    baseR(1000);
    delay(15000); //wait 15s for user to mark block
  }
  //move base L 5s (back to start)
  //i know i can do delay(5000), but i want to move same as forward & see if that makes it go back exactly/consistent
  for (int j=0;j<5;j++) { 
    baseL(1000);
  }
  delay(10000);
}