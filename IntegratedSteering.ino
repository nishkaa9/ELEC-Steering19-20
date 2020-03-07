  // Adding servo library
#include <Servo.h>
Servo servo1;
Servo servo2;          
Servo servo3;
Servo servo4;
// defining joystick pins (analog)
int joyX = 3; // this is for pitch and turn
int joyY = 1;
int joyRX = 7; // R for roll
int joyRY = 8;
int joyVal;
float xValue;
float yValue;
int rollXValue;
int rollYValue;
int avg;
float current1, current2, current3, current4, diff1, diff2, diff3, diff4;
int NeutralPos = 90;
int UpperLimit = 600;
int LowerLimit = 400;


void calibration();
void writeAngles(int one, int two, int three, int four);


void setup() {
  Serial.begin(115200);
  servo1.attach(3);
  servo2.attach(5);
  servo3.attach(6);
  servo4.attach(9);
  calibration();
}


// joystick:
//                  (512, 1023)   
//   (1023, 512)  <-  center -> (0, 512)
//                   (512, 0)


void loop() {
  xValue = analogRead(joyX);
  yValue = analogRead(joyY);
  rollXValue = analogRead(joyRX);
  rollYValue = analogRead(joyRY);
  Serial.print(xValue);
  Serial.print(" ");
  Serial.println(yValue);
  // To Pitch down
  if ((xValue > UpperLimit) && (LowerLimit <= yValue <= UpperLimit)) { // changed to y < lower
    yValue = map(yValue, 0, 1023, 0, 180);
    writeAngles(NeutralPos - yValue, NeutralPos + yValue, NeutralPos - yValue, NeutralPos + yValue);
    Serial.println("Going Down");
    delay(500);
  }
  // To Pitch Up
  else if ((xValue < LowerLimit) && (LowerLimit <= yValue <= UpperLimit)) { //changed to y > upper
    yValue = map(yValue, 0, 1023, 0, 180);
    writeAngles(NeutralPos + yValue, NeutralPos - yValue, NeutralPos + yValue, NeutralPos - yValue);
    Serial.println("Going Up");
    delay(500);
  }
  // To turn starboard (right)
  else if ((yValue > UpperLimit) && (LowerLimit <= xValue <= UpperLimit)) { // changed to x < lower
    xValue = map(xValue, 0, 1023, 0, 180);
    writeAngles(NeutralPos - xValue, NeutralPos - xValue, NeutralPos - xValue, NeutralPos - xValue);
    Serial.println("Turning Right");
    delay(500);
  }
  // To turn portside (left)
  else if ((yValue < LowerLimit) && (LowerLimit <= xValue <= UpperLimit)) { // changed to x > upper
    xValue = map(xValue, 0, 1023, 0, 180);
    writeAngles(NeutralPos + xValue, NeutralPos + xValue, NeutralPos + xValue, NeutralPos + xValue);
    Serial.println("Turning Left");
    delay(500);
  }
  // 45 degrees
  else if ((xValue < LowerLimit) && (yValue > UpperLimit)) {
    int squareRoot = sqrt(xValue * xValue + yValue * yValue);
    int actual = map(squareRoot, 0, 1023, 0, 180);
    writeAngles(NeutralPos, NeutralPos + actual, NeutralPos, NeutralPos + actual);
    Serial.println("45 degrees");
    delay(500);
  }
  // 135 degrees
  else if ((xValue > UpperLimit) && (yValue > UpperLimit)) {
    int squareRoot = sqrt(xValue * xValue + yValue * yValue);
    int actual = map(squareRoot, 0, 1023, 0, 180);
    writeAngles(NeutralPos - actual, NeutralPos, NeutralPos - actual, NeutralPos);
    Serial.println("135 degrees");
    delay(500);
  }
  // 225 degrees
  else if ((xValue > UpperLimit) && (LowerLimit > yValue)) {
    int squareRoot = sqrt(xValue * xValue + yValue * yValue);
    int actual = map(squareRoot, 0, 1023, 0, 180);
    writeAngles(NeutralPos, NeutralPos - actual, NeutralPos, NeutralPos - actual);
    Serial.println("225 degrees");
    delay(500);
  }
  // 315 degrees
  else if ((xValue < LowerLimit) && (LowerLimit > yValue)) {
    int squareRoot = sqrt(xValue * xValue + yValue * yValue);
    int actual = map(squareRoot, 0, 1023, 0, 180);
    writeAngles(NeutralPos + actual, NeutralPos, NeutralPos + actual, NeutralPos);
    Serial.println("315 degrees");
    delay(500);
  }
  // Resetting the motors if no input
  else if ((LowerLimit <= xValue <= UpperLimit) && (LowerLimit <= yValue <= UpperLimit)) {
    calibration();
  } 
  
  // roll right
  if (rollXValue <= LowerLimit) {
    int rollXValue = map(rollXValue, 0, 1023, 0, 180);
    writeAngles(NeutralPos + rollXValue, NeutralPos - rollXValue, NeutralPos - rollXValue, NeutralPos + rollXValue);
    delay(500);
  } 
  // roll left
  else if (rollXValue >= UpperLimit) {
    int rollXValue = map(rollXValue, 0, 1023, 0, 180);
    writeAngles(NeutralPos - rollXValue, NeutralPos + rollXValue, NeutralPos + rollXValue, NeutralPos - rollXValue);
    delay(500);
  } 
  else {
    calibration();
  }
}


// Helper Functions
void calibration() {
  current1 = servo1.read();
  current2 = servo2.read();
  current3 = servo3.read();
  current4 = servo4.read();
  diff1 = NeutralPos - current1;
  diff2 = NeutralPos - current2;
  diff3 = NeutralPos - current3;
  diff4 = NeutralPos - current4;
  for (int pos = 0; pos <= 90; pos += 1) {                   // goes from 0 degrees to 180 degrees 
    servo1.write(current1 + diff1 * pos / 90.0);              // tell servo to go to position in variable 'pos'
    servo2.write(current2 + diff2 * pos / 90.0); 
    servo3.write(current3 + diff3 * pos / 90.0); 
    servo4.write(current4 + diff4 * pos / 90.0);
    //delay(10);                                             // waits 15ms for the servo to reach the position
  }
}
void writeAngles(int one, int two, int three, int four) {
  current1 = servo1.read();
  current2 = servo2.read();
  current3 = servo3.read();
  current4 = servo4.read();
  diff1 = one - current1;
  diff2 = two - current2;
  diff3 = three - current3;
  diff4 = four - current4;
  for (int pos = 0; pos <= 90; pos += 1) {                   // goes from 0 degrees to 180 degrees 
    servo1.write(current1 + diff1 * pos / 90.0);              // tell servo to go to position in variable 'pos'
    servo2.write(current2 + diff2 * pos / 90.0); 
    servo3.write(current3 + diff3 * pos / 90.0); 
    servo4.write(current4 + diff4 * pos / 90.0);
    //delay(10);                                             // waits 15ms for the servo to reach the position
  }
}


