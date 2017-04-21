#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#define MAX_VALID_Z 550     // will correspond to full punch

int bluetooth_client(int player, int hip_data, int y_data, int z_data) 
{
    struct sockaddr_rc addr = { 0 };
    int s, status;
    char dest[18] = "00:80:37:2E:31:20";

    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( dest, &addr.rc_bdaddr );

    // connect to server
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    printf("%d\n", status); 

    // send a message
    if( status == 0 ) {
	   status = write(s, "hello!", 6);
    }
    
    if( status < 0 ) perror("uh oh");

    close(s);
    return 0;
}

int main(int argc, char **argv)
{
    int hip_data, y_data, z_data, player;
    player = 1;
    y_data = 1;
    z_data = 1;
    hip_data = 5; // the set value for the hip

	while(1){
        if(player++ >= 5) {
            player = 1;
        }
        z_data = (z_data + 20) % (MAX_VALID_Z + 50);
        if (z_data == 0) {
            z_data = 1;
        }
        y_data = (y_data + 20) % 200;
        if (y_data == 0) {
            y_data = 1;
        }
		bluetooth_client(player, hip_data, y_data, z_data);
		sleep(1);
	}

	return 0;
}
