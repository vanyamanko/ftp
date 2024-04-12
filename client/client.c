#include "client_utils.h"
#include "client_input.h"
#include "client_reply.h"
#include "client_network.h"

int main(int argc, char* argv[]) 
{		
	int data_sock, retcode;
	char buffer[MAXSIZE];
	struct command cmd;	
	struct addrinfo hints, *res;

	if (argc != 3) {
		printf("usage: ./client hostname port\n");
		exit(0);
	}

	char *host = argv[1];
	char *port = argv[2];

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_socktype = SOCK_STREAM;
	
	if (getaddrinfo(host, port, &hints, &res)) {
		printf("getaddrinfo() error");
		exit(1);
	}

	int sock_control = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	if (sock_control < 0){
		perror("failed to create socket");
		exit(1);	
	}

	if(connect(sock_control, res->ai_addr, res->ai_addrlen)) {
		perror("connecting stream socket");
		exit(1);
	}

	freeaddrinfo(res);

	printf("Connected to %s.\n", host);
	print_reply(read_reply(sock_control)); 
	

	client_login(sock_control);

	while (1) { 

		if (client_read_command(buffer, sizeof buffer, &cmd) < 0) {
			printf("Invalid command\n");
			continue;	
		}

		if (send(sock_control, buffer, (int)strlen(buffer), 0) < 0 ) {
			close(sock_control);
			exit(1);
		}

		retcode = read_reply(sock_control);		
		if (retcode == 221) {
			print_reply(221);		
			break;
		}
		
		if (retcode == 502) {
			printf("%d Invalid command.\n", retcode);
		} else {			
		
			if ((data_sock = client_open_conn(sock_control)) < 0) {
				perror("Error opening socket for data connection");
				exit(1);
			}			
			
			if (strcmp(cmd.code, "LIST") == 0) {
				client_list(data_sock, sock_control);
			} 
			else if (strcmp(cmd.code, "CGET") == 0) {
				if (read_reply(sock_control) == 550) {
					print_reply(550);		
					close(data_sock);
					continue; 
				}
				client_get(data_sock, cmd.arg);
				print_reply(read_reply(sock_control)); 
			}
			else if (strcmp(cmd.code, "CPUT") == 0) {
				client_put(data_sock, cmd.arg);
			}
			close(data_sock);
		}

	} 
	close(sock_control);
    return 0;  
}
