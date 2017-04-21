/*
 * Simple I2C communication test with an Arduino as the slave device.
 * With bluetooth comms from Kinect
 * ble adapted from: https://people.csail.mit.edu/albert/bluez-intro/x502.html
 */

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#define ARDUINO_I2C_ADDRESS 0x10
#define ARDUINO_I2C_BUFFER_LIMIT 32

// For defining motion of arm mapped to servo rotation values
#define BASE_POSITION 1           // 0 is MIN Servo Angle
#define MAX_JAB_POSITION 180       // 180 is MAX Servo Angle
#define MIN_VALID_Y 0       // will correspond to arm above the stomach
#define MIN_VALID_Z 30      // will correspond to raised arm, against body
#define MAX_VALID_Z 70      // will correspond to full punch

int determine_angle(int y_data, int z_data){
  int angle, y_dist_from_hip, z_dist_from_hip;

  y_dist_from_hip = y_data; // - hip_data;
  z_dist_from_hip = z_data; // this should be (+) val
  
  // Check if the arm is raised up above hip at least MIN_VALID_Y
  if(y_dist_from_hip > MIN_VALID_Y) {
    if (z_dist_from_hip > MIN_VALID_Z) {
      // TODO! Calculate the appropriate angle for servo based on data
      if (z_dist_from_hip >= MAX_VALID_Z) {
        return MAX_JAB_POSITION;
      
      } else {
        // Calculate proportional angle
        angle = (((z_dist_from_hip - MIN_VALID_Z) * (MAX_JAB_POSITION - BASE_POSITION)) / (MAX_VALID_Z - MIN_VALID_Z));
        if (angle == 0) {
          return BASE_POSITION;
        }
        else {
          return angle;
        }
      }
    } 
  }
  // If we haven't returned, just set to BASE_POSITION
  return BASE_POSITION;
}

int send_to_arduino(int fh, char*buff, int player, int angle) {
  int len, sent;
  char msg[2];
  // Put the data in the Gumstix-Arduino predetermined format
  sprintf(msg, "%c%c", player, angle); // cast ints to chars
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
  int fh;               // to hold the i2c file descriptor
  int player, angle, hip_data, y_data, z_data, sent;    // after parsing the Kinect data
  char kinect_msg[10];  // receiving the kinect data
  char arduino_buff[ARDUINO_I2C_BUFFER_LIMIT + 4]; // For sending the data to the arduino

  // For bluetooth socket
  struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
  char buf[1024] = { 0 };
  int s, client, bytes_read;
  socklen_t opt = sizeof(rem_addr);

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

  // allocate socket
  s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

  // bind socket to port 1 of the first available local bluetooth adapter
  loc_addr.rc_family = AF_BLUETOOTH;
  loc_addr.rc_bdaddr = *BDADDR_ANY;
  loc_addr.rc_channel = (uint8_t) 2;// originally channel 1
  bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

  // Wait for data from Kinect
  while(1) {
    // put socket into listening mode
    listen(s, 1);
    // accept one connection (blocking)
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);
    ba2str( &rem_addr.rc_bdaddr, buf );
    memset(buf, 0, sizeof(buf));
    // read data from the client
    bytes_read = read(client, buf, sizeof(buf));
    if(bytes_read > 0) {
      printf("received [%s]\n", buf);
      // Parse the data
      if(bytes_read == 3) {
        player    = (int)buf[0];
        //hip_data  = (int)buf[1];
        y_data    = (int)buf[1];
        z_data    = (int)buf[2];
        
        // Check that the player was valid
        if (player >= 1 && player <= 4) {
          // Calculate the angle
          angle = determine_angle(y_data, z_data); // hip_data

          // Send the data to the arduino
          sent = send_to_arduino(fh, arduino_buff, player, angle);
        } else {
          printf("Player id invalid: %d\n", player);
        }
      } else {
        printf("bytes_read != 4 %d\n", bytes_read);
      }
    }
    // close connection
    close(client);
  }
  close(s);
}
