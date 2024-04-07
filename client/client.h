#ifndef CLIENT_H
#define CLIENT_H

#include "../common/common.h"


int read_reply(void);

void print_reply(int rc);


int client_read_command(char* buf, int size, struct command *cstruct);

int client_get(int data_sock, char* arg);

void client_put(int sock_control, int data_sock, char* arg);

int client_open_conn(int sock_con);

int client_list(int sock_data, int sock_con);


int client_send_cmd(struct command *cmd);

void client_login(void);


#endif
