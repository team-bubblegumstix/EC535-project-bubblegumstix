#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

int bluetooth_client() 
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
	while(1){

		bluetooth_client();
		sleep(2);


	}

	return 0;
}
