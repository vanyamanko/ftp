#include "client.h"
	

int sock_control; 

int read_reply(void){
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
			printf("220 Welcome, server ready.\n");
			break;
		case 221:
			printf("221 Goodbye!\n");
			break;
		case 226:
			printf("226 Closing data connection. Requested file action successful.\n");
			break;
		case 550:
			printf("550 Requested action not taken. File unavailable.\n");
			break;
	}
	
}

int client_read_command(char* buf, int size, struct command *cstruct)
{
	memset(cstruct->code, 0, sizeof(cstruct->code));
	memset(cstruct->arg, 0, sizeof(cstruct->arg));
	
	printf("client> ");	
	fflush(stdout); 	

	read_input(buf, size);
		
	char *arg = NULL;
	arg = strtok (buf," ");
	arg = strtok (NULL, " ");

	if (arg != NULL){
		strncpy(cstruct->arg, arg, strlen(arg));
	}

	if (strcmp(buf, "list") == 0) {
		strcpy(cstruct->code, "LIST");		
	}
	else if (strcmp(buf, "get") == 0) {
		strcpy(cstruct->code, "RETR");		
	}
	else if (strcmp(buf, "quit") == 0) {
		strcpy(cstruct->code, "QUIT");		
	}
	else {
		return -1;
	}

	memset(buf, 0, 400);
	strcpy(buf, cstruct->code);

	if (arg != NULL) {
		strcat(buf, " ");
		strncat(buf, cstruct->arg, strlen(cstruct->arg));
	}
	
	return 0;
}


int client_get(int data_sock, char* arg)
{
    char data[MAXSIZE];
    int size;
    FILE* fd = fopen(arg, "w");
    
    while ((size = recv(data_sock, data, MAXSIZE, 0)) > 0) {
        fwrite(data, 1, size, fd);
    }

    if (size < 0) {
        perror("error\n");
    }

    fclose(fd);
    return 0;
}


int client_open_conn(int sock_con)
{
	int sock_listen = socket_create(CLIENT_PORT_ID);

	int ack = 1;
	if ((send(sock_con, (char*) &ack, sizeof(ack), 0)) < 0) {
		printf("client: ack write error :%d\n", errno);
		exit(1);
	}		

	int sock_conn = socket_accept(sock_listen);
	close(sock_listen);
	return sock_conn;
}


int client_list(int sock_data, int sock_con)
{
	size_t num_recvd;		
	char buf[MAXSIZE];		
	int tmp = 0;

	if (recv(sock_con, &tmp, sizeof tmp, 0) < 0) {
		perror("client: error reading message from server\n");
		return -1;
	}
	
	memset(buf, 0, sizeof(buf));
	while ((num_recvd = recv(sock_data, buf, MAXSIZE, 0)) > 0) {
        	printf("%s", buf);
		memset(buf, 0, sizeof(buf));
	}
	
	if (num_recvd < 0) {
	        perror("error");
	}

	if (recv(sock_con, &tmp, sizeof tmp, 0) < 0) {
		perror("client: error reading message from server\n");
		return -1;
	}
	return 0;
}


int client_send_cmd(struct command *cmd)
{
	char buffer[MAXSIZE];
	int rc;

	sprintf(buffer, "%s %s", cmd->code, cmd->arg);
	
	rc = send(sock_control, buffer, (int)strlen(buffer), 0);	
	if (rc < 0) {
		perror("Error sending command to server");
		return -1;
	}
	
	return 0;
}




void client_login(void)
{
	struct command cmd;
	char user[256];
	memset(user, 0, 256);

	printf("Name: ");	
	fflush(stdout); 		
	read_input(user, 256);

	strcpy(cmd.code, "USER");
	strcpy(cmd.arg, user);
	client_send_cmd(&cmd);
	
	int wait;
	recv(sock_control, &wait, sizeof wait, 0);

	fflush(stdout);	
	char *pass = getpass("Password: ");	

	strcpy(cmd.code, "PASS");
	strcpy(cmd.arg, pass);
	client_send_cmd(&cmd);
	
	int retcode = read_reply();
	switch (retcode) {
		case 430:
			printf("Invalid username/password.\n");
			exit(0);
		case 230:
			printf("Successful login.\n");
			break;
		default:
			perror("error reading message from server");
			exit(1);		
			break;
	}
}


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
	
	sock_control = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

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
	print_reply(read_reply()); 
	

	client_login();

	while (1) { 

		if (client_read_command(buffer, sizeof buffer, &cmd) < 0) {
			printf("Invalid command\n");
			continue;	
		}

		if (send(sock_control, buffer, (int)strlen(buffer), 0) < 0 ) {
			close(sock_control);
			exit(1);
		}

		retcode = read_reply();		
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
			else if (strcmp(cmd.code, "RETR") == 0) {
				if (read_reply() == 550) {
					print_reply(550);		
					close(data_sock);
					continue; 
				}
				client_get(data_sock, cmd.arg);
				print_reply(read_reply()); 
			}
			close(data_sock);
		}

	} 
	close(sock_control);
    return 0;  
}
