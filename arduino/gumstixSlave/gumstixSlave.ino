/*
  I2C code adapted from:
  https://github.com/scottellis/overo-arduino-i2c/blob/master/arduino/arduino_i2c_echo/arduino_i2c_echo.pde
  https://www.arduino.cc/en/Tutorial/MasterWriter
  The Arduino as an I2C slave to power PWM channels for servos.
*/

#include <Wire.h>

#define I2C_SLAVE_ADDRESS 0x10
#define DES_BAUD_RATE 9600

void onRecieveHandler(int numBytes)
{
    Serial.println("received:");
    if(Wire.available() == 4) {
      int arm = Wire.read();       // receive byte as an integer  (arm number to move)
      char delim = Wire.read();    // receive byte as a character (colon as a delimiter)
      int angle = Wire.read();     // receive byte as an integer  (Arm position)
      char term = Wire.read();     // receive byte as a character (null byte at end of string)
      Serial.println(arm);
      Serial.println(delim);
      Serial.println(angle);
      Serial.println(term);  
    
    } else {
      Serial.println("Wire > 4");
      Serial.println(Wire.available());
    }
    
}

/*
  Setup the I2C communication
*/
void begin_i2c()
{
    Wire.begin(I2C_SLAVE_ADDRESS);    // Connect Arduino to I2C bus at I2C_SLAVE_ADDRESS
    Wire.onReceive(onRecieveHandler); // Create callback function for receiving data
}

void setup()
{
    Serial.begin(DES_BAUD_RATE);
    while (!Serial);                  // Wait until Serial Monitor is open
    Serial.println("\nI2C Gumstix Slave");
    begin_i2c();  
}

void loop()
{
}
