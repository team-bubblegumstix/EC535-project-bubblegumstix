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
 
int main(int argc, char **argv)
{
	int fh;
	char buff[ARDUINO_I2C_BUFFER_LIMIT + 4];
  char msg[5];
	int len, sent, rcvd,player,angle;

	fh = open("/dev/i2c-0", O_RDWR);

	if (fh < 0) {
		perror("open");
		return 1;
	}

	if (ioctl(fh, 0x0703, ARDUINO_I2C_ADDRESS) < 0) {
		perror("ioctl");
		return 1;
	}

	if (argc > 1) {
		memset(buff, 0, sizeof(buff));
		strncpy(buff, argv[1], ARDUINO_I2C_BUFFER_LIMIT);
	}
	else {
    while(1) {
      angle = (angle + 90) % 180;
      if (angle == 0) {
        angle = 1;
      }
      for (player = 3; player < 5; player++) {
        sprintf(msg,"%c%c",player,angle);
        strcpy(buff,msg); 
        printf("Sending ..."); 
        
        len = strlen(buff);
        sent = write(fh, buff, len);

        if (sent != len) {
          perror("write");
          return 1;
        }
        usleep(100000); // sleep for 1 sec
      }
      usleep(100000); // sleep for 1 sec
    }
  }
}
