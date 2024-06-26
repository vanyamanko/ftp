#include "client_input.h"

void read_input(char* buffer, int size)
{
	char *nl = NULL;
	memset(buffer, 0, size);

	if ( fgets(buffer, size, stdin) != NULL ) {
		nl = strchr(buffer, '\n');
		if (nl) *nl = '\0';
	}
}

int client_read_command(char* buf, int size, struct command *cstruct, int* dir_flag)
{
	memset(cstruct->code, 0, sizeof(cstruct->code));
	memset(cstruct->arg, 0, sizeof(cstruct->arg));

	printf("\x1b[35m");
	printf("\n");
	system("pwd");

	printf("\x1b[32m");
	printf("client> ");	

	printf("\x1b[0m");
	fflush(stdout); 	

	read_input(buf, size);

	char terminal_cmd[MAXSIZE];
	strcpy(terminal_cmd, buf);

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
	else if (strcmp(buf, "put") == 0) {
		strcpy(cstruct->code, "STOR");
		if(is_directory(arg)) {
			strcat(cstruct->arg, ".zip");
			*dir_flag = 1;
		}
	}
	else if (strcmp(buf, "cwd") == 0) {
		strcpy(cstruct->code, "CWD");
	}
	else if (strcmp(buf, "quit") == 0) {
		strcpy(cstruct->code, "QUIT");		
	}
	else if (strcmp(buf, "delete") == 0) {
		strcpy(cstruct->code, "DELE");		
	}
	else if (strcmp(buf, "help") == 0) {
		client_info();
		system("less info.txt");
		system("rm info.txt");
		return 1;
	}
	else {
		system(terminal_cmd);
		if (strcmp(buf, "cd") == 0) {
			chdir(arg);
			if(arg == NULL) {
				chdir("/");
			}
		}
		return 1;
	}

	memset(buf, 0, 512);
	strcpy(buf, cstruct->code);

	if (arg != NULL) {
		strcat(buf, " ");
		strncat(buf, cstruct->arg, strlen(cstruct->arg));
	}
	
	return 0;
}

void client_login(int sock_control)
{
	struct command cmd;
	char user[256];
	memset(user, 0, 256);

	printf("Name: ");	
	fflush(stdout); 		
	read_input(user, 256);

	strcpy(cmd.code, "USER");
	strcpy(cmd.arg, user);
	client_send_cmd(&cmd, sock_control);
	
	int wait;
	recv(sock_control, &wait, sizeof wait, 0);

	fflush(stdout);	
	char *pass = getpass("Password: ");	

	strcpy(cmd.code, "PASS");
	strcpy(cmd.arg, pass);
	client_send_cmd(&cmd, sock_control);
	
	int retcode = read_reply(sock_control);
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
	}
}
