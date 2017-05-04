# EC535-project-bubblegumstix
Final Project for EC535 Spring 2017 - Embedded Rock 'EM Sock 'EM
https://github.com/team-bubblegumstix/EC535-project-bubblegumstix
https://www.youtube.com/watch?v=RtDk022GyY8

## Software/Hardware Versions
### Install Requirements for Kinect Sensor
Component           |    Version
------------------- | -------------
Xbox Kinect         |   v1 (USB 2.0)
OS                  |   Windows 7 / 10
IDE                 |   Microsoft Visual Studios 2015
Language            |   C#
SDK                 |   [Kinect for Windows v1.8](https://www.microsoft.com/en-us/download/details.aspx?id=40278)
Bluetooth Adapter   |   Targus 4.0
Bluetooth Library   |   32Feet (InTheHand.Net)

Steps:
1. Clone this repo
1. Download the SDK Package from provided link
1. Add the following .dll files to the Project References
   1. C:\Program Files\Microsoft SDKs\Kinect\v1.8\AssembliesC\Microsoft.Kinect.dll
   1. C:\Program Files\Microsoft SDKs\Kinect\Developer Toolkit v1.8.0\Assemblies\Microsoft.Kinect.Toolkit.dll
1. Add 32feet bluetooth package to references
   1. Use Nuget package manager 
1. Plug Kinect into any USB 2.0 / 3.0 port
      
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

- GND = I2C(1), connected to Arduino ground
- SDA = GPIO_118 = I2C(2), connected to the Ardunio Uno SDA = A4
- SCL = GPIO_117 = I2C(4), connected to the Ardunio Uno SCL = A5
- Gumstix powered by power supply unit
- Bluetooth module connected to Bluetooth receiver
- Gumstix Serial Communication over USB adapter to computer running minicom

Steps:
1. Copy the C program (gumstix_master) to the gumstix using minicom
1. Make sure the bluetooth, socket and I2C libraries have been loaded to the board

### Install Requirements for Arduino
Library      |    Link
------------ | -------------
Servo.h      |   [arduino-libraries/Servo](https://github.com/arduino-libraries/Servo)
Wire.h       |   [arduino/Arduino](https://github.com/arduino/Arduino/tree/master/hardware/arduino/avr/libraries/Wire)

Steps:
1. Download and add both libraries to the Arduino Libraries folder
1. Load the arduino script from this repo onto any board with I2C capabilities

## Arduino Uno - Pin Layout

```
Analog Pins 
 ___________________________________________________________________________________
|     |     |     |     |     |     |     |     |     |     |     |     |     |     |
| SCL | SDA |     |     |     |     |     | GND | GND |     |     |     |     |     |
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

## Ready to Run
Once you have managed to install all of the libraries and packages for each component you are now ready to run our project and throw your first punch (Towards the kinect of course. We do not in any way condone violence!).

Steps:
1. Plug the pre-loaded arduino (slave) servo controller into a power source and wire up the servos
1. Plug the gumstix into its power source and run ./gumstixmaster within minicom
1. Plug the kinect in and run its C# program within MVS
1. Step back in front of the camera and begin playing!

