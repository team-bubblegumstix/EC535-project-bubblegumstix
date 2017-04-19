#include <Servo.h>

#define SERVO_BLUE_PIN_LEFT 10  // Set Blue left servo to digital pin 10
#define SERVO_BLUE_PIN_RIGHT 9  // Set Blue right servo to digital pin 9
#define SERVO_RED_PIN_LEFT 8    // Set Red left servo to digital pin 8
#define SERVO_RED_PIN_RIGHT 7   // Set Red right servo to digital pin 7

// For defining motion of arm mapped to servo rotation values
#define BASE_POSITION 0           // 0 is MIN Servo Angle
#define MAX_JAB_POSITION 90       // 180 is MAX Servo Angle
#define DELAY_TIME 50             // If we want to hold positions for a certain amount of time
#define SERVO_DELAY_PER_DEGREE 5  // To give time for the servo to reach its end position

#define RESET_TO_BASE 0

Servo BlueArmLeft;          // Define left servo for Blue player
Servo BlueArmRight;         // Define right servo for Blue player
Servo RedArmLeft;           // Define left servo for Red player
Servo RedArmRight;          // Define right servo for Red player

void setup() { 
  BlueArmLeft.attach(SERVO_BLUE_PIN_LEFT);
  BlueArmRight.attach(SERVO_BLUE_PIN_RIGHT);
  RedArmLeft.attach(SERVO_RED_PIN_LEFT);
  RedArmRight.attach(SERVO_RED_PIN_RIGHT);
} 

void loop() { // Loop through motion tests

  if(RESET_TO_BASE) {
      moveArm(BlueArmLeft, BASE_POSITION);
      moveArm(BlueArmRight, BASE_POSITION);
      moveArm(RedArmLeft, BASE_POSITION);
      moveArm(RedArmRight, BASE_POSITION);
      
  } else {
    quickPunch(BlueArmLeft);
    delay(DELAY_TIME);      // Wait DELAY_TIME in milliseconds
    quickResetArm(BlueArmLeft);
    
    quickPunch(RedArmLeft);
    delay(DELAY_TIME);      // Wait DELAY_TIME in milliseconds
    quickResetArm(RedArmLeft);

    quickPunch(BlueArmRight);
    delay(DELAY_TIME);      // Wait DELAY_TIME in milliseconds
    quickResetArm(BlueArmRight);
    
    quickPunch(RedArmRight);
    delay(DELAY_TIME);      // Wait DELAY_TIME in milliseconds
    quickResetArm(RedArmRight);
    
    delay(DELAY_TIME*2);      // Wait DELAY_TIME in milliseconds
  }
  
}

// Motion routines
void moveArm(Servo jabArm, int angle) {
  int delta = jabArm.read() - angle;
  jabArm.write(angle);                                // move forward to desired angle
  delay(SERVO_DELAY_PER_DEGREE* abs(delta));          // Wait SERVO_DELAY_PER_DEGREE * delta milliseconds for servo to change position

}

void quickPunch(Servo jabArm) {
  moveArm(jabArm, MAX_JAB_POSITION);                  // move forward to max position
}

// reset arm to base
void quickResetArm(Servo jabArm) {
  moveArm(jabArm, BASE_POSITION);                     // reset back to base position
}



