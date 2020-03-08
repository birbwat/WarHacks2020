#include <AFMotor.h>
#include <NewPing.h>
#define TRIGGER_PIN  A3 
#define ECHO_PIN     A4 
#define MAX_DISTANCE 200 
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
AF_DCMotor left_motor(3);
AF_DCMotor right_motor(4);
int TrgPin = 2;
int EcoPin = 1;
int leftSensor = A1;
int rightSensor = A2;

// speed variables for easy access
int hs = 255; // high speed
int ls = 185; // low speed

// boundaries for switching modes
int lowb = 10;
int highb = 100;

// black or white boundary
int bw = 400;

// pass obstacle
int passObs = 0;

// init some sensors vars
int distance;
int leftValue;
int rightValue;

void setup()
{
  left_motor.setSpeed(255);
  right_motor.setSpeed(255);
  left_motor.run(RELEASE);
  right_motor.run(RELEASE);
  Serial.begin(9600);
}
void loop()
{
  //Beginning
  delay(50);                  
  useSensors();
  // Main logic ------------------------------------------------- 1
  while (distance > lowb || distance < highb) {
      mainLogic();
  } // while stop
  // Obstacle --------------------------------------------------- 2
  if ((distance <= lowb || distance >= highb) && passObs == 0) {
    turnLeft();
    while (leftValue>=bw && rightValue<bw) {
      useSensors();
    }
    if (leftValue<bw && rightValue>=bw) {
      passObs = 1;
      while (leftValue<bw && rightValue>=bw) {
        turnRight();
        useSensors();
      }
    }
  } // while 2 stop
  // Back to main ----------------------------------------------- 3
  if (leftValue>=bw && rightValue<bw) {
    delay(200); // arbitrary - to test
    while (distance > lowb || distance < highb) {
      mainLogic();
    }
  } 
} // void loop stop
//functions to make code less clunky

void driveForward()
{
    left_motor.run(FORWARD);
    right_motor.run(FORWARD);
    left_motor.setSpeed(hs);
    right_motor.setSpeed(hs);
}

void turnRight()
{
    left_motor.run(FORWARD);
    right_motor.run(FORWARD);
    left_motor.setSpeed(hs);
    right_motor.setSpeed(ls);
}

void turnLeft()
{
    left_motor.run(FORWARD);
    right_motor.run(FORWARD);
    left_motor.setSpeed(ls);
    right_motor.setSpeed(hs);
}

void useSensors()
{
  Serial.print("Ping: ");
  int distance = sonar.ping_cm();
  Serial.print(distance); 
  Serial.println("cm");
  int leftValue = analogRead(leftSensor);
  int rightValue = analogRead(rightSensor);
  Serial.print(leftValue);
  Serial.print(" L / R ") ;
  Serial.println(rightValue);
}

void mainLogic()
{
    useSensors();
    if (leftValue>=bw && rightValue>=bw) 
    {
      //forward
      driveForward();
    }
    if (leftValue>=bw && rightValue<bw)
    {
      // turn right
      turnRight();
    }
    if (leftValue<bw && rightValue>=bw)
    {
      // turn left
      turnLeft();
    }
    else
    {
      // go forward anyway
      driveForward();
    }
    delay(10);
} // voidmain stop
