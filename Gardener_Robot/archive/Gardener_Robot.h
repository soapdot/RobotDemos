

void shmoving() {
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

void updatePots() {
  basePVal = analogRead(basePotPin);
  shoulderPVal = analogRead(shoulderPotPin);
  elbowPVal = analogRead(elbowPotPin);
  wristPVal = analogRead(wristPotPin);
  grabberPVal = analogRead(grabberPotPin);
}

float DS_update_process(pinNum) { // test distance vs output
  float temp0to5Vread = analogRead(pinNum);
  float convDist = temp0to5Vread;
  return convDist;
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