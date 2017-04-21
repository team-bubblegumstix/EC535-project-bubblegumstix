/*
  I2C code adapted from:
  https://github.com/scottellis/overo-arduino-i2c/blob/master/arduino/arduino_i2c_echo/arduino_i2c_echo.pde
  https://www.arduino.cc/en/Tutorial/MasterWriter
  The Arduino as an I2C slave to power PWM channels for servos.
*/

#include <Servo.h>
#include <Wire.h>

#define I2C_SLAVE_ADDRESS 0x10  // Set Arduino's address on I2C bus

#define SERVO_BLUE_PIN_LEFT 10  // Set Blue left servo to digital pin 10
#define SERVO_BLUE_PIN_RIGHT 9  // Set Blue right servo to digital pin 9
#define SERVO_RED_PIN_LEFT 8    // Set Red left servo to digital pin 8
#define SERVO_RED_PIN_RIGHT 7   // Set Red right servo to digital pin 7

// For defining motion of arm mapped to servo rotation values
#define BASE_POSITION 0           // 0 is MIN Servo Angle
#define MAX_JAB_POSITION 90       // 180 is MAX Servo Angle

Servo BlueArmLeft;          // Define left servo for Blue player
Servo BlueArmRight;         // Define right servo for Blue player
Servo RedArmLeft;           // Define left servo for Red player
Servo RedArmRight;          // Define right servo for Red player


// move forward to desired angle
void moveArm(Servo jabArm, int angle) {
  jabArm.write(angle);
}

// move forward to max position
void quickPunch(Servo jabArm) {
  moveArm(jabArm, MAX_JAB_POSITION);
}

// reset arm to base
void quickResetArm(Servo jabArm) {
  moveArm(jabArm, BASE_POSITION);
}

Servo getArmById(int armId) {
  switch(armId){
    case 1:
      return BlueArmLeft;
     case 2:
      return BlueArmRight;
     case 3:
      return RedArmLeft;
     case 4:
      return RedArmRight;
     default:
      return BlueArmLeft;
  }
}

/*
 * Create callback function when I2C data sent from Gumstix
 */
void onRecieveHandler(int numBytes)
{
    if(Wire.available() == 2) {
      int arm = Wire.read();       // receive byte as an integer  (arm number to move)
      int angle = Wire.read();     // receive byte as an integer  (Arm position)
      moveArm(getArmById(arm), angle);

    
    } else {
      for (int i = 0; i < numBytes; i++) {
        Wire.read();
      }
    }
    
}

/*
 * Setup the I2C communication
 */
void begin_i2c()
{
    Wire.begin(I2C_SLAVE_ADDRESS);    // Connect Arduino to I2C bus at I2C_SLAVE_ADDRESS
    Wire.onReceive(onRecieveHandler); // Create callback function for receiving data
}



void setup()
{
    // Attach the servos to their PINs
    BlueArmLeft.attach(SERVO_BLUE_PIN_LEFT);
    BlueArmRight.attach(SERVO_BLUE_PIN_RIGHT);
    RedArmLeft.attach(SERVO_RED_PIN_LEFT);
    RedArmRight.attach(SERVO_RED_PIN_RIGHT);
    
    // Prepare the I2C communication
    begin_i2c();  
}

void loop()
{
}
