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
// RIGHT ARMS GO POSITIVE ANGLES, LEFT ARMS NEGATIVE (relative to 90 degrees)
#define BASE_POSITION 90    // 90 is starting Servo Angle
#define MAX_JAB_L_POS 120   // 120 is MAX Servo Angle in clockwise direction (30 degrees)
#define MAX_JAB_R_POS 60    // 60 is MAX Servo Angle in counter-clockwise direction (30 degrees)

// Define base values for the kinect readings
#define MIN_VALID_Y 0       // will correspond to arm above the stomach
#define MIN_VALID_Z 30      // will correspond to raised arm, against body
#define MAX_VALID_Z 70      // will correspond to full punch

int determine_angle(char arm, int y_data, int z_data){
  int angle;
  
  // Check if the arm is raised up above hip at least MIN_VALID_Y
  if(y_data > MIN_VALID_Y) {
    if (z_data > MIN_VALID_Z) {
      // TODO! Calculate the appropriate angle for servo based on data
      if (z_data >= MAX_VALID_Z) {
        if(arm == 'l') {
          return MAX_JAB_L_POS;
        } else {
          return MAX_JAB_R_POS;
        }
      } else {
        // Calculate proportional angle
        if(arm == 'l') {
          angle = (((z_data - MIN_VALID_Z) * (MAX_JAB_L_POS - BASE_POSITION)) / (MAX_VALID_Z - MIN_VALID_Z));
        } else {
          angle = (((z_data - MIN_VALID_Z) * (BASE_POSITION - MAX_JAB_R_POS)) / (MAX_VALID_Z - MIN_VALID_Z));
        }
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

int send_to_arduino(int fh, char*buff, int player, int leftAngle, int rightAngle) {
  int len, sent;
  char msg[3];
  // Put the data in the Gumstix-Arduino predetermined format
  sprintf(msg, "%c%c%c", player, leftAngle, rightAngle); // cast ints to chars
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
  int y_data_l, z_data_l, y_data_r, z_data_r, leftAngle, rightAngle;
  int k_player_id, last_k_id, player, skipped_player_count, sent;    // after parsing the Kinect data
  char arduino_buff[ARDUINO_I2C_BUFFER_LIMIT + 4]; // For sending the data to the arduino
  int num_players_tracked = 0;      // For tracking the state of known skeletons
  int player_id_map[2] = {-1, -1};  // For tracking the kinect IDs of the two players

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
      // Parse the data
      if(bytes_read == 5) {
        k_player_id = (int)buf[0]; // skel.TrackingId from kinect
        y_data_l    = (int)buf[1]; // leftWrist_Y
        z_data_l    = (int)buf[2]; // leftWrist_Z
        y_data_r    = (int)buf[3]; // rightWrist_Y
        z_data_r    = (int)buf[4]; // rightWrist_Z

        printf("KINECT ID: %d\n", k_player_id);
        printf("NUM PLYRS: %d\n", num_players_tracked);
        printf("MAP PLYR1: %d\n", player_id_map[0]);
        printf("MAP PLYR2: %d\n", player_id_map[1]);
        // Update player ID map
        if (num_players_tracked == 0) {
          // We are adding a new player 1
          player = 1;
          player_id_map[0] = k_player_id;
          num_players_tracked++;

        } else if (num_players_tracked == 1) {
          // We are only aware of 1 skeleton at the moment
          if(last_k_id == k_player_id) {
            // The data is an existing id
            if(player_id_map[0] == k_player_id) {
              // Data for solely-tracked player 1
              player = 1;

            } else {
              // Data for solely-tracked player 2
              player = 2;

            }
          } else {
            // This is a new id (and only tracking 1 player, so this is a new player)
            if (player_id_map[1] == -1) {
              // Add a new player 2
              player = 2;
              player_id_map[1] = k_player_id;
            
            } else if (player_id_map[0] == -1) {
              // Add a new player 1
              player = 1;
              player_id_map[0] = k_player_id;

            } 
            num_players_tracked++;

          }
        } else if (num_players_tracked == 2) {
          // We are aware of 2 skeletons
          if(last_k_id == k_player_id) {
            // Saw the same id 2x in a row, we must have lost a player
            if(player_id_map[0] == k_player_id) {
              // Data for now solely-tracked player 1
              player = 1;
              // Remove player 2 id
              player_id_map[1] = -1;

            } else {
              // Data for now solely-tracked player 2
              player = 2;
              // Remove player 1 id
              player_id_map[0] = -1;

            }
            num_players_tracked--;
          } else {
            // This is a different id than we saw last time
            if(player_id_map[0] == last_k_id) {
              if(player_id_map[1] == k_player_id) {
                // We saw the known player 1 last time, and this is the known player 2
                player = 2;

              } else {
                // We saw the known player 1 last time, but this is not player 2's id
                player = -1;
                k_player_id = 1;  // This way, we just ignore this 3rd player

              }
            } else if(player_id_map[1] == last_k_id) {
              if(player_id_map[0] == k_player_id) {
                // We saw the known player 2 last time, and this is the known player 1
                player = 1;

              } else {
                // We saw the known player 2 last time, but this is not player 1's id
                player = -1;      // This way, we just ignore this 3rd players data
                k_player_id = 2;  // This way, we may move back to the 

              }
            }
          }
        }
        last_k_id = k_player_id;

        if(player >= 1 && player <= 4) {
          printf("PLAYER ID: %d\n", player);
          // No players skipped this round (we were able to recognize/handle ids)
          skipped_player_count = 0;
          
          // Calculate the angles
          leftAngle = determine_angle('l', y_data_l, z_data_l);
          rightAngle = determine_angle('r', y_data_r, z_data_r);

          // Send the data to the arduino
          sent = send_to_arduino(fh, arduino_buff, player, leftAngle, rightAngle);
          
        } else {
          if(++skipped_player_count > 4) {
            // Both players left the screen at the same time and returned at the same time (w new ids)
            skipped_player_count = 0;

            // Reset map and num_players_tracked
            player_id_map[0] = -1;
            player_id_map[1] = -1;
            num_players_tracked = 0;

            printf("reset both ids\n");
          } else {
            printf("skipped 3rd player\n");
            
          }
        }
      } else {
        printf("bytes_read != 5 %d\n", bytes_read);
      }
    }
    // close connection
    close(client);
  }
  close(s);
}
