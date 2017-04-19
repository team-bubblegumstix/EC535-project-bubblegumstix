// Wire Master Writer
// https://www.arduino.cc/en/Tutorial/MasterWriter
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

#define I2C_SLAVE_ADDRESS 0x10
#define DES_BAUD_RATE 9600

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(DES_BAUD_RATE);
  while (!Serial);             // wait for serial monitor
  Serial.println("\nI2C Master");
}

byte x = 0;

void loop() {
  Serial.println("Master send");
  Wire.beginTransmission(I2C_SLAVE_ADDRESS);  // transmit to slave device
  Wire.write("x is ");        // sends five bytes
  Wire.write(x);              // sends one byte
  Wire.endTransmission();     // stop transmitting

  x++;
  delay(500);
}
