#include "client_utils.h"

void client_info(void)
{
    FILE *file = fopen("info.txt", "a");  
	    if (file == NULL) {
        printf("Failed to open info.txt\n");
        return;
    }

    fprintf(file, "HELP(1)\n\n");
    fprintf(file, "NAME\n");
    fprintf(file, "     help - get information about utils\n\n");
    fprintf(file, "DESCRIPTION\n");
    fprintf(file, "     The help utility displays documentation about all utils.\n\n\n\n");

    fprintf(file, "RETR(2)\n\n");
    fprintf(file, "NAME\n");
    fprintf(file, "     get - get a file from the server\n\n");
	fprintf(file, "SYNOPSIS\n");
	fprintf(file, "     get <file_name>\n\n");
    fprintf(file, "DESCRIPTION\n");
    fprintf(file, "     The get command gets a file from the server and saves it locally in the current directory.\n\n\n\n");

    fprintf(file, "STOR(3)\n\n");
    fprintf(file, "NAME\n");
    fprintf(file, "     put - put a file on the server\n\n");
	fprintf(file, "SYNOPSIS\n");
	fprintf(file, "     put <file_name>\n\n");
    fprintf(file, "DESCRIPTION\n");
    fprintf(file, "     The put command uploads a file to the server. If the file is not found, an empty file will be created on the server.\n\n\n\n");

    fprintf(file, "LIST(4)\n\n");
    fprintf(file, "NAME\n");
    fprintf(file, "      list - list files on the server\n\n");
    fprintf(file, "DESCRIPTION\n");
    fprintf(file, "     The list command displays the contents of the server files.\n\n\n\n");
	
    fprintf(file, "DELE(5)\n\n");
    fprintf(file, "NAME\n");
    fprintf(file, "     delete - delete a file from the server\n\n");
	fprintf(file, "SYNOPSIS\n");
	fprintf(file, "     delete <file_name>\n\n");
    fprintf(file, "DESCRIPTION\n");
    fprintf(file, "     The delete command removes a file from the server.\n\n\n\n");

	fprintf(file, "QUIT(6)\n\n");
    fprintf(file, "NAME\n");
    fprintf(file, "      quit - exit the program\n\n");
    fprintf(file, "DESCRIPTION\n");
    fprintf(file, "     Terminate the client process.\n\n\n\n");

	fprintf(file, "You can also use terminal commands.\n");

    fclose(file);
}

int client_get(int data_sock, char* arg)
{
	printf("Waiting for server response...\n");	
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


void client_put(int sock_data, char* filename)
{	
	FILE* fd = NULL;
	char data[MAXSIZE];
	size_t num_read;							
    int rm_flag = 0;

    if (is_directory(filename)) {
        char zip_cmd[MAXSIZE] = "zip -r ";
        strcat(zip_cmd, filename);
        strcat(zip_cmd, ".zip "); 
        strcat(zip_cmd, filename);
        strcat(zip_cmd, " > temp.txt; rm temp.txt");
        system(zip_cmd);
        strcat(filename, ".zip");
        rm_flag = 1;
    }
	fd = fopen(filename, "r");
	
	if (!fd) {	
		printf("File not fount! You created empty file.\n");
		
	} else {	

		printf("Waiting for server response...\n");
		do {
			num_read = fread(data, 1, MAXSIZE, fd);

			if (send(sock_data, data, num_read, 0) < 0)
				perror("error sending file\n");

		} while (num_read > 0);													

		printf("Requested file action successful.\n");
		fclose(fd);
	}
    
    if(rm_flag) {
        char rm_file_cmd[MAXSIZE] = "rm ";
        strcat(rm_file_cmd, filename);
        system(rm_file_cmd);
    }
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

	if (recv(sock_con, &tmp, sizeof tmp, 0) < 0) {
		perror("client: error reading message from server\n");
		return -1;
	}
	return 0;
}
