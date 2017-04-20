# EC535-project-bubblegumstix
Final Project for EC535 Spring 2017 - Embedded Rock 'EM Sock 'EM

## Software/Hardware Versions
### Kinect
[TODO]
### Gumstix


### Arduino




## Gumstix Board - I2C Pin Layout

```
                 [PIN Header side]
              _______________________
             |     |     |     |     |
             | SDA | SCL |     |     |
 [SD side]   |-----|-----|-----|-----|   [USB side]
             | GND |     |     |     |
             |_____|_____|_____|_____|
             
                 [Serial port side]

```

- GND = I2C(1), connected to breadboard common ground
- SDA = GPIO_118 = I2C(2), connected to the Ardunio Uno SDA = A4
- SCL = GPIO_117 = I2C(4), connected to the Ardunio Uno SCL = A5
- Gumstix powered by power supply unit


## Arduino Uno - Pin Layout

```
Analog Pins 
 ___________________________________________________________________________________
|     |     |     |     |     |     |     |     |     |     |     |     |     |     |
| SCL | SDA |     |     |     |     |     |     | GND | PWR |     |     |     |     |
|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|
  A5    A4    A3    A2    A1    A0    Vin   GND   GND    5V   3.3V  RST  IOREF

Digital Pins
 ___________________________________________________________________________________
|     |     |     |     |     |     |     |     |     |     |     |     |     |     |
|     |     |     |     |     |     |     | RR  | RL  | BR  | BL  |     |     |     |
|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|
   0     1     2     3     4     5     6     7     8     9    10    11    12     13

```

- SCL = A5, connected to the Gumstix I2C(4)
- SDA = A4, connected to the Gumstix I2C(2)
- GND = connected to breadboard common ground
- PWR = connected to breadboard common power
- RR  = D7, (Red Right Arm)  connected to Servo 4 Data line
- RL  = D8, (Red Left Arm)   connected to Servo 3 Data line
- BR  = D9, (Blue Right Arm) connected to Servo 2 Data line
- BL  = D10,(Blue Left Arm)  connected to Servo 1 Data line
- Arduino powered by a USB port

## Servo Motors

- Servo 4 Data line = Red Right Arm,  connected to Arduino D7
- Servo 3 Data line = Red Left Arm,   connected to Arduino D8
- Servo 2 Data line = Blue Right Arm, connected to Arduino D9
- Servo 1 Data line = Blue Left Arm,  connected to Arduino D10
- Each servo's PWR line is connected to the common 5V breadboard power (supplied by Arduino)
- Each servo's GND line is connected to the common breadboard ground (supplied by Arduino)

