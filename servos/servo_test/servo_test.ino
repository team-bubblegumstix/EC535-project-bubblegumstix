#include <Servo.h>

#define SERVO_PIN_LEFT 10 // Set left servo to digital pin 10
#define SERVO_PIN_RIGHT 9 // Set right servo to digital pin 9

// For defining motion of arm mapped to servo rotation values
#define BASE_POSITION 90
#define MAX_JAB_POSITION 50
#define DELAY_TIME 2000

Servo armLeft;          // Define left servo
Servo armRight;         // Define right servo

void setup() { 
  armLeft.attach(SERVO_PIN_LEFT);
//  armRight.attach(SERVO_PIN_RIGHT);
} 

void loop() {            // Loop through motion tests

//  slowPunch(armLeft);
//  delay(DELAY_TIME);      // Wait DELAY_TIME milliseconds (2 seconds)
//
//  slowResetArm(armLeft);
//  delay(DELAY_TIME);      // Wait DELAY_TIME milliseconds (2 seconds)

  quickPunch(armLeft);
  delay(DELAY_TIME);      // Wait DELAY_TIME milliseconds (2 seconds)

  quickResetArm(armLeft);
  delay(DELAY_TIME);      // Wait DELAY_TIME milliseconds (2 seconds)
  
}

// Motion routines
void moveArm(Servo jabArm, int distance) {
  jabArm.write(distance);
}

void quickPunch(Servo jabArm) {
  moveArm(jabArm, MAX_JAB_POSITION); // move forward to max position
}

// reset arm to base
void quickResetArm(Servo jabArm) {
  moveArm(jabArm, BASE_POSITION);    // reset back to base position
}

void slowPunch(Servo jabArm) {
  moveArm(armLeft, (MAX_JAB_POSITION * .125) );
  delay(DELAY_TIME);
  moveArm(armLeft, (MAX_JAB_POSITION * .250) );
  delay(DELAY_TIME);
  moveArm(armLeft, (MAX_JAB_POSITION * .375) );
  delay(DELAY_TIME);
  moveArm(armLeft, (MAX_JAB_POSITION * .500) );
  delay(DELAY_TIME);
  moveArm(armLeft, (MAX_JAB_POSITION * .625) );
  delay(DELAY_TIME);
  moveArm(armLeft, (MAX_JAB_POSITION * .750) );
  delay(DELAY_TIME);
  moveArm(armLeft, (MAX_JAB_POSITION * .875) );
  delay(DELAY_TIME);
  moveArm(armLeft, (MAX_JAB_POSITION) );
  delay(DELAY_TIME);

}

void slowResetArm(Servo jabArm) {
  moveArm(armLeft, (MAX_JAB_POSITION * .875) );
  delay(DELAY_TIME);
  moveArm(armLeft, (MAX_JAB_POSITION * .750) );
  delay(DELAY_TIME);
  moveArm(armLeft, (MAX_JAB_POSITION * .625) );
  delay(DELAY_TIME);
  moveArm(armLeft, (MAX_JAB_POSITION * .500) );
  delay(DELAY_TIME);
  moveArm(armLeft, (MAX_JAB_POSITION * .375) );
  delay(DELAY_TIME);
  moveArm(armLeft, (MAX_JAB_POSITION * .250) );
  delay(DELAY_TIME);
  moveArm(armLeft, (MAX_JAB_POSITION * .125) );
  delay(DELAY_TIME);
  moveArm(armLeft, (BASE_POSITION) );
  delay(DELAY_TIME);
}



