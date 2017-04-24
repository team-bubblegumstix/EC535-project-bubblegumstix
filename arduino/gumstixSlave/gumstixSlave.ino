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

Servo BlueArmLeft;          // Define left servo for Blue player
Servo BlueArmRight;         // Define right servo for Blue player
Servo RedArmLeft;           // Define left servo for Red player
Servo RedArmRight;          // Define right servo for Red player


/*
 * Create callback function when I2C data sent from Gumstix
 */
void onRecieveHandler(int numBytes)
{
    if(Wire.available() == 3) {
      int player_id = Wire.read();      // receive byte as an integer  (arm number to move)
      int leftAngle = Wire.read();      // receive byte as an integer  (left arm position)
      int rightAngle = Wire.read();     // receive byte as an integer  (right arm position)
      
      if (player_id == 1) {
        BlueArmLeft.write(leftAngle);
        BlueArmRight.write(rightAngle);
      } else {
        RedArmLeft.write(leftAngle);
        RedArmRight.write(rightAngle);
      }

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

    // The servos need to start at 90
    BlueArmLeft.write(90);
    BlueArmRight.write(90);
    RedArmLeft.write(90);
    RedArmRight.write(90);
    
    // Prepare the I2C communication
    begin_i2c();  
}

void loop()
{
}
