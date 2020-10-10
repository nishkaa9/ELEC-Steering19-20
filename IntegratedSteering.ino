#include <Servo.h> // Adding servo library

Servo servo1; // Defines the 4 different servos used for the 4 fins
Servo servo2;          
Servo servo3;
Servo servo4;

// defining joystick pins (analog)
int joyX = 3; // Joysticks used for pitch and yaw + input pins
int joyY = 1;
int joyRX = 7; // Joysticks used for roll + input pins
int joyRY = 8;

int joyVal; // Wasn't used

float xValue; // The position of the joystick in the x-y coordinate system for pitch and yaw
float yValue;
int rollXValue; // The position of the joystick in the x-y coordinate systems for roll
int rollYValue;

int avg; // Wasn't used

float current1, current2, current3, current4, diff1, diff2, diff3, diff4; // Variable "current" stores the current position of the servo in degrees 
//"diff" stores the change in the servo position that needs to be written to the servo

int NeutralPos = 90; // Neutral position of the servo is initialised to 90 degrees
int UpperLimit = 600; // Defines upper limit for the safety box in the x and y direction (if the joystick is touched by mistake the sub won't move)
int LowerLimit = 400; // Defines lower limit for the safety box (Note: only works for small deflections of the joystick)


void calibration(); // Moves the servo back to its neutral position from its current one
void writeAngles(int one, int two, int three, int four); // Writes what angke the servo needs to turn to corresponding to the joystick


void setup() { 
  Serial.begin(115200); // Sets the speed at which data is recorded 
  
  servo1.attach(3); // Defines the output pin of each of the servos
  servo2.attach(5);
  servo3.attach(6);
  servo4.attach(9);
  
  calibration(); // Sets all servos to the neutral position initially
}


// joystick:
//                  (512, 1023)   
//   (1023, 512)  <-  center -> (0, 512)
//                   (512, 0)


void loop() {
  xValue = analogRead(joyX); // Reads input value from the joystick for pitch and yaw
  yValue = analogRead(joyY);
  
  rollXValue = analogRead(joyRX); // Reads input value from the joystick for roll
  rollYValue = analogRead(joyRY);
  
  Serial.print(xValue); // Prints x and y values of the joystick
  Serial.print(" ");
  Serial.println(yValue);
  
  // To Pitch down
  if ((xValue > UpperLimit) && (LowerLimit <= yValue <= UpperLimit)) { // changed to y < lower
    yValue = map(yValue, 0, 1023, 0, 180); // converts a value between 0 and 1023 to an angle between 0 and 180 to write to the servo
    writeAngles(NeutralPos - yValue, NeutralPos + yValue, NeutralPos - yValue, NeutralPos + yValue); // writes angles to the 4 servos
    Serial.println("Going Down"); // Prints direction of motion
    delay(500);
  }
  // To Pitch Up
  else if ((xValue < LowerLimit) && (LowerLimit <= yValue <= UpperLimit)) { //changed to y > upper
    yValue = map(yValue, 0, 1023, 0, 180); // converts a value between 0 and 1023 to an angle between 0 and 180 to write to the servo
    writeAngles(NeutralPos + yValue, NeutralPos - yValue, NeutralPos + yValue, NeutralPos - yValue); // writes angles to the 4 servos
    Serial.println("Going Up"); // Prints direction of motion
    delay(500);
  }
  // To turn starboard (right)
  else if ((yValue > UpperLimit) && (LowerLimit <= xValue <= UpperLimit)) { // changed to x < lower
    xValue = map(xValue, 0, 1023, 0, 180); // converts a value between 0 and 1023 to an angle between 0 and 180 to write to the servo
    writeAngles(NeutralPos - xValue, NeutralPos - xValue, NeutralPos - xValue, NeutralPos - xValue); // writes angles to the 4 servos
    Serial.println("Turning Right"); // Prints direction of motion
    delay(500);
  }
  // To turn portside (left)
  else if ((yValue < LowerLimit) && (LowerLimit <= xValue <= UpperLimit)) { // changed to x > upper
    xValue = map(xValue, 0, 1023, 0, 180); // converts a value between 0 and 1023 to an angle between 0 and 180 to write to the servo
    writeAngles(NeutralPos + xValue, NeutralPos + xValue, NeutralPos + xValue, NeutralPos + xValue); // writes angles to the 4 servos
    Serial.println("Turning Left"); // Prints direction of motion
    delay(500);
  }
  // 45 degrees
  else if ((xValue < LowerLimit) && (yValue > UpperLimit)) {
    int squareRoot = sqrt(xValue * xValue + yValue * yValue);
    int actual = map(squareRoot, 0, 1023, 0, 180); // converts a value between 0 and 1023 to an angle between 0 and 180 to write to the servo
    writeAngles(NeutralPos, NeutralPos + actual, NeutralPos, NeutralPos + actual); // writes angles to the 4 servos
    Serial.println("45 degrees"); // Prints direction of motion
    delay(500);
  }
  // 135 degrees
  else if ((xValue > UpperLimit) && (yValue > UpperLimit)) {
    int squareRoot = sqrt(xValue * xValue + yValue * yValue);
    int actual = map(squareRoot, 0, 1023, 0, 180); // converts a value between 0 and 1023 to an angle between 0 and 180 to write to the servo
    writeAngles(NeutralPos - actual, NeutralPos, NeutralPos - actual, NeutralPos); // writes angles to the 4 servos
    Serial.println("135 degrees"); // Prints direction of motion
    delay(500);
  }
  // 225 degrees
  else if ((xValue > UpperLimit) && (LowerLimit > yValue)) {
    int squareRoot = sqrt(xValue * xValue + yValue * yValue); 
    int actual = map(squareRoot, 0, 1023, 0, 180);// converts a value between 0 and 1023 to an angle between 0 and 180 to write to the servo
    writeAngles(NeutralPos, NeutralPos - actual, NeutralPos, NeutralPos - actual); // writes angles to the 4 servos
    Serial.println("225 degrees"); // Prints direction of motion
    delay(500);
  }
  // 315 degrees
  else if ((xValue < LowerLimit) && (LowerLimit > yValue)) {
    int squareRoot = sqrt(xValue * xValue + yValue * yValue);
    int actual = map(squareRoot, 0, 1023, 0, 180); // converts a value between 0 and 1023 to an angle between 0 and 180 to write to the servo
    writeAngles(NeutralPos + actual, NeutralPos, NeutralPos + actual, NeutralPos); // writes angles to the 4 servos
    Serial.println("315 degrees"); // Prints direction of motion
    delay(500);
  }
  // Resetting the motors if no input
  else if ((LowerLimit <= xValue <= UpperLimit) && (LowerLimit <= yValue <= UpperLimit)) {
    //calibration();
  } 
  
  // roll right
  if (rollXValue <= LowerLimit) {
    int rollXValue = map(rollXValue, 0, 1023, 0, 180); // converts a value between 0 and 1023 to an angle between 0 and 180 to write to the servo
    writeAngles(NeutralPos + rollXValue, NeutralPos - rollXValue, NeutralPos - rollXValue, NeutralPos + rollXValue); // writes angles to the 4 servos
    delay(500);
  } 
  // roll left
  else if (rollXValue >= UpperLimit) {
    int rollXValue = map(rollXValue, 0, 1023, 0, 180); // converts a value between 0 and 1023 to an angle between 0 and 180 to write to the servo
    writeAngles(NeutralPos - rollXValue, NeutralPos + rollXValue, NeutralPos + rollXValue, NeutralPos - rollXValue); // writes angles to the 4 servos
    delay(500);
  } 
  // Resetting the motors if no input
  else {
    // calibration();
  }
}


// Helper Functions
void calibration() {
  current1 = servo1.read(); // Reads the current position of the servos
  current2 = servo2.read();
  current3 = servo3.read();
  current4 = servo4.read();
  
  diff1 = NeutralPos - current1; // Angle that the servo needs to turn to return to the neutral position
  diff2 = NeutralPos - current2;
  diff3 = NeutralPos - current3;
  diff4 = NeutralPos - current4;
  
  for (int pos = 0; pos <= 90; pos += 1) { // goes from current position to neutral position in increments
    servo1.write(current1 + diff1 * pos / 90.0); // diff * pos/90 makes the servo shaft turn by that fraction till they reach the final position            
    servo2.write(current2 + diff2 * pos / 90.0); 
    servo3.write(current3 + diff3 * pos / 90.0); 
    servo4.write(current4 + diff4 * pos / 90.0);
    //delay(10);                                       
  }
}
void writeAngles(int one, int two, int three, int four) {
  current1 = servo1.read(); // Reads the current position of the servos
  current2 = servo2.read();
  current3 = servo3.read();
  current4 = servo4.read();
  
  diff1 = one - current1; // Angle that the servo needs to turn to move to the position corresponding to the joystick
  diff2 = two - current2;
  diff3 = three - current3;
  diff4 = four - current4;
  
  for (int pos = 0; pos <= 90; pos += 1) { // goes from current position to position corresponding to the joystick in increments               
    servo1.write(current1 + diff1 * pos / 90.0); // diff * pos/90 makes the servo shaft turn by that fraction till they reach the final position             
    servo2.write(current2 + diff2 * pos / 90.0); 
    servo3.write(current3 + diff3 * pos / 90.0); 
    servo4.write(current4 + diff4 * pos / 90.0);
    //delay(10);                                            
  }
}
