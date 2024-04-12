#include "client_network.h"

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


int client_send_cmd(struct command *cmd, int sock_control)
{
	char buffer[MAXSIZE];

	sprintf(buffer, "%s %s", cmd->code, cmd->arg);
	
	int rc = send(sock_control, buffer, strlen(buffer), 0);	
	if (rc < 0) {
		perror("Error sending command to server");
		return -1;
	}
	
	return 0;
}
