#ifndef SERVER_H
#define SERVER_H

#include "../common/common.h"

void server_get(int sock_control, int sock_data, char* filename);

int server_list(int sock_data, int sock_control);

int server_start_data_conn(int sock_control);

int server_check_user(char*user, char*pass);


int server_login(int sock_control);

int server_recv_cmd(int sock_control, char*cmd, char*arg);


void server_process(int sock_control);


#endif
