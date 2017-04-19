// Wire Slave Receiver
// https://www.arduino.cc/en/Tutorial/MasterWriter
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

#define I2C_SLAVE_ADDRESS 0x10
#define DES_BAUD_RATE 9600

void setup() {
  Wire.begin(I2C_SLAVE_ADDRESS);// join i2c bus at I2C_SLAVE_ADDRESS 
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(DES_BAUD_RATE);  // start serial for output
}

void loop() {
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int numBytes) {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read();     // receive byte as a character
    Serial.print(c);          // print the character
  }
  int x = Wire.read();        // receive byte as an integer
  Serial.println(x);          // print the integer
}
