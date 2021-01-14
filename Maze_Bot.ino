#include <Servo.h>

//
// *****************************
// Authors: Angus Wu and Dylan Thai
// Date: June 2019
// Maze Bot Project
// Program instructs the boe bot for it to complete any maze
// *****************************
//

//Servos
Servo sensorServo;
const int sensorServoPin = 11;
Servo servoLeft;
Servo servoRight;

//Ultrasonic sensor
const int sensorPinT = 8;
const int sensorPinE = 7;

//Other variables
int leftDist; //The path distance on the left side of the robot
int rightDist; //The path distance on the right side of the robot
int frontDistance; //Distance between the robot (the ultrasonic sensor) and the wall ahead in milliseconds (the default unit)
const int wallDist = 600; //The maximum distance to stop when a wall is ahead (approcimately 8 cm)

//----------------------------------------------------------------------------------------------------

void setup() { //Set up of the ultrasonic sensor, IR sensors, and the servos
  Serial.begin(9600);

  //Ultrasonic sensor
  pinMode(sensorPinT, OUTPUT);
  pinMode(sensorPinE, INPUT);

  //Infrared sensor
  pinMode(3,INPUT);
  pinMode(10,INPUT);
  pinMode(2,OUTPUT);
  pinMode(9,OUTPUT);

  //Servos
  sensorServo.attach(sensorServoPin);
  servoLeft.attach(13);
  servoRight.attach(12);
}

//----------------------------------------------------------------------------------------------------

void loop(){

  //IR sensors check if the robot is to close to the walls
  int irLeft = irDetect(9, 10, 37000);
  int irRight = irDetect(2, 3, 37000);
  
  sensorServo.write(90); //Turns the standard servo so the ultrasonic sensor faces the front
  
  frontDistance = distance(); //Updates the distance between the robot and the wall ahead

  if(frontDistance == 0 || frontDistance > wallDist){ //If the ultrasonic sensor senses 0 (did not detect anything, it happens sometimes as an error), or the distance ahead is less than the minimum distance to stop
    forward();
    
    if(irRight == 0){ //If the robot is too close to the right wall, turn left slightly
      turnLeft(1);
    }
    if(irLeft == 0){ //If the robot is too close to the left wall, turn right slightly
      turnRight(1);
    }
    
  }else{
    //Stop the robot
    servoLeft.writeMicroseconds(1500);
    servoRight.writeMicroseconds(1500);
    
    sensorServo.write(0); //Turns the standard servo to 0 degrees (right)
    delay(750);
    rightDist = distance(); //Stores the distance to the right of the robot

    sensorServo.write(90); //Turns the standard servo to 90 degrees (front)
    delay(750);
    frontDistance = distance(); //Stores the distance ahead of the robot, used for double checking
  
    sensorServo.write(180); //Turns the standard servo to 180 degrees (left)
    delay(750);
    leftDist = distance(); // Stores the distance to the left of the robot

    if(frontDistance > wallDist){ //Double checks if there is room to move forward; checks if the distance ahead is above the minimum stop distance
      
      sensorServo.write(90); //Repositions the standard servo to face the front
      delay(500);
      
    }else{
    
      if(leftDist < 1000 && rightDist < 1000){ //If there is not enough distance to either side of the robot
        turnLeft(41); //Turns 180 degress
        delay(1000);
        sensorServo.write(90); //Repositions the standard servo to face the front
        delay(500);
        
      }else if(leftDist > rightDist || leftDist == 0){ //If there is more space to the left side, or if the sensor outputs 0 (as a rare error)
        turnLeft(21); //Turns left 90 degrees
        delay(1000);
        sensorServo.write(90); //Repositions the standard servo to face the front
        delay(500);
        
      }else{
        turnRight(21); //Turns right 90 degrees
        delay(1000);
        sensorServo.write(90); //Repositions the standard servo to face the front
        delay(500);
      }
      
    } //End if, turn direction
  } //End if, check turn direction distance
} //End loop()

//----------------------------------------------------------------------------------------------------

int irDetect(int irLedPin, int irReceiverPin, long frequency){ //Finds if any objects are in front of the IR sensors
  tone(irLedPin, frequency, 8);
  delay(1);
  int ir = digitalRead(irReceiverPin);
  delay(1);
  return ir;
}

long distance(){ //Finds the distance ahead
  digitalWrite(sensorPinT, LOW);
  delay(2);
  digitalWrite(sensorPinT, HIGH);
  delay(10);
  digitalWrite(sensorPinT, LOW);
  long distance = pulseIn(sensorPinE, HIGH, 25000);
  return distance;
}

void turnLeft(int num){ //Turns servos accordingly so the robot turns left
  for(int i=0; i<num; i++){
    servoLeft.writeMicroseconds(1340);
    servoRight.writeMicroseconds(1340);
    delay(20);
  }
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
}
  
void turnRight(int num){ //Turns servos accordingly so the robot turns right
  for(int i=0; i<num; i++){
    servoLeft.writeMicroseconds(1660);
    servoRight.writeMicroseconds(1660);
    delay(20);
  }
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
}

void forward(){ //Moves the robot forwards slightly
  for(int i = 0; i<5; i++){
    servoLeft.writeMicroseconds(1600);
    servoRight.writeMicroseconds(1413);
    delay(20);
  }
}
