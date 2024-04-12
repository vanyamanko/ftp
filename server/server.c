#include "server_func.h"

int main(int argc, char *argv[])
{	
	int sock_listen, sock_control, port, pid;

	if (argc != 2) {
		printf("usage: ./server port\n");
		exit(0);
	}

	port = atoi(argv[1]);

	if ((sock_listen = socket_create(port)) < 0 ) {
		perror("Error creating socket");
		exit(1);
	}		
	
	while(1) {	

		if ((sock_control = socket_accept(sock_listen))	< 0 )
			break;			
		
		if ((pid = fork()) < 0) { 
			perror("Error forking child process");
		} else if (pid == 0) { 
			close(sock_listen);
			server_process(sock_control);		
			close(sock_control);
			exit(0);
		}
			
		close(sock_control);
	}

	close(sock_listen);	

	return 0;
}