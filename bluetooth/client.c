#include <stdio.h>
#include <sys/socket.h>
#include <error.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
int main()
{
	struct sockaddr_rc addr;
	int listenfd, status;
	//XX:XX:XX:XX:XX:XX IS BLUETOOTH ADDRESS
	char dest[18] = "XX:XX:XX:XX:XX:XX";
	int cc = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	if(cc < 0){
		perror(“Socket failed”);
		exit(1);
	}
	// set the connection parameters (who to connect to)
	addr.rc_family = AF_BLUETOOTH;
	addr.rc_channel = (uint8_t) 1;
	str2ba( dest, &addr.rc_bdaddr );
	// connect to server
	status = connect(listenfd, (struct sockaddr *)&addr, sizeof(addr));
	// send a message
	if( status == 0 ) {
		status = write(listenfd, "hello world!", 12);
	}
	if( status < 0 ){
		perror("write failed");
		exit(1);
	}
	return 0;
}
