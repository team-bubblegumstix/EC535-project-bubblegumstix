/*
  https://github.com/scottellis/overo-arduino-i2c/blob/master/arduino/arduino_i2c_echo/arduino_i2c_echo.pde
  The Arduino as an I2C slave.
  Echo back data received converted to uppercase.
*/

#include <Wire.h>

#define I2C_SLAVE_ADDRESS 0x10
#define DES_BAUD_RATE 9600

// BUFFER_LENGTH comes from Wire.h
// It is the default Arduino I2C size limit for a single transfer.
// Larger transfers need to be broken up.
// Not doing that here.
uint8_t buff[BUFFER_LENGTH];

int pos;

// If the buffer overflows, reset to zero ;-)
void onRecieveHandler(int numBytes)
{
    Serial.println("received:");
    for (int i = 0; i < numBytes; i++) {
        buff[pos] = Wire.read();
        Serial.println(buff[pos]);  // print each byte as an integer
        pos++;

        if (pos >= BUFFER_LENGTH)
            pos = 0; 
    }
}

void onRequestHandler()
{
    if (!pos)
        return;

    Serial.println("req:");
    for (int i = 0; i < pos; i++){
        Serial.println(buff[i]);
        buff[i] = toupper(buff[i]);
    }
    Wire.write(buff, pos);

    pos = 0; 
}

/*
  With power means, enable pins 2 and 3 for ground and
  power respectively. Puts power close to the 4,5 pins
  used for I2C. Set withPower=0 if you don't want this.
*/
void begin_i2c(int withPower)
{
    byte pwrpin = PORTC3;
    byte gndpin = PORTC2;
  
    if (withPower) {
        DDRC |= _BV(pwrpin) | _BV(gndpin);  // make outputs
        PORTC &=~ _BV(gndpin);
        PORTC |=  _BV(pwrpin);
        delay(100);
    }
  
    Wire.begin(I2C_SLAVE_ADDRESS);
    
    Wire.onReceive(onRecieveHandler);
    Wire.onRequest(onRequestHandler);
}

void setup()
{
    Serial.begin(DES_BAUD_RATE);
    while (!Serial);             // Leonardo: wait for serial monitor
    Serial.println("\nI2C Gumstix Slave");
    begin_i2c(0);  
}

void loop()
{
}
