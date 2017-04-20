/*
  Simple I2C communication test with an Arduino as the slave device.
*/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>

#define ARDUINO_I2C_ADDRESS 0x10

#define ARDUINO_I2C_BUFFER_LIMIT 32


int send_to_arduino(char*buff, int player, int angle) {
  int len, sent;
  char msg[2];
  // Put the data in the Gumstix-Arduino predetermined format
  sprintf(msg,"%c%c",player,angle); // cast ints to chars
  // Copy that msg to the buffer
  strcpy(buff, msg); 
  
  len = strlen(buff);
  sent = write(fh, buff, len);

  if (sent != len) {
    perror("write");
    return 0;
  }
  return 1;
}
 

int main(int argc, char **argv)
{
	int fh;            // to hold the i2c file descriptor
  int player, angle; // after parsing the Kinect data
	char arduino_buff[ARDUINO_I2C_BUFFER_LIMIT + 4]; // For sending the data to the arduino

  // open the i2c dev file as read/write
	fh = open("/dev/i2c-0", O_RDWR);
  // check that file opened
	if (fh < 0) {
		perror("open");
		return 1;
	}
  // set the I2C_SLAVE (0x0703) equal to the ARDUINO_I2C_ADDRESS (set by arduino)
	if (ioctl(fh, 0x0703, ARDUINO_I2C_ADDRESS) < 0) {
		perror("ioctl");
		return 1;
	}

  // Wait for data from Kinect
  while(1) {
    // Take data, and calculate the desired servo/angle
    angle = (angle + 90) % 180;
    if (angle == 0) {
      angle = 1;
    }
    for (player = 3; player < 5; player++) {
      if(send_to_arduino(arduino_buff, player, angle) == 0) {
        return 1; // There was an error in sending the data
      }
      usleep(100000); // sleep for 1 sec
    }
    usleep(100000); // sleep for 1 sec
  }
}
