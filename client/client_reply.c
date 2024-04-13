#include "client_reply.h"

int read_reply(int sock_control){
	int retcode = 0;
	if (recv(sock_control, &retcode, sizeof retcode, 0) < 0) {
		perror("client: error reading message from server\n");
		return -1;
	}	
	return ntohl(retcode);
}

void print_reply(int rc) 
{
	switch (rc) {
		case 220:
			printf("Welcome, server ready.\n");
			break;
		case 221:
			printf("Goodbye!\n");
			break;
		case 226:
			printf("Requested file action successful.\n");
			break;
		case 550:
			printf("Requested action not taken. File unavailable.\n");
			break;
	}
	
}
