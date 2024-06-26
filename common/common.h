#ifndef COMMON_H
#define COMMON_H
#define _DEFAULT_SOURCE

#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>	
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define DEBUG				1
#define MAXSIZE 			512 
#define CLIENT_PORT_ID		30020

struct command {
	char arg[255];
	char code[5];
};

int socket_create(int port);

int socket_accept(int sock_listen);

int is_directory(const char *dir_name);

#endif
