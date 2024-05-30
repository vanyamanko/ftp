#ifndef SERVER_FUNC_H
#define SERVER_FUNC_H

#include "../common/common.h"

void server_cwd(int sock_data, int sock_control, char* arg); 

void server_get(int sock_control, int sock_data, char* filename);

int server_list(int sock_data, int sock_control);

int server_start_data_conn(int sock_control);

int server_check_user(char*user, char*pass);

int server_put(int sock_data, char* filename);

int server_delete(int sock_control, char* arg);

int server_login(int sock_control);

int server_recv_cmd(int sock_control, char*cmd, char*arg);

int socket_connect(int port, char*host);

int recv_data(int sockfd, char* buf, int bufsize);

int send_response(int sockfd, int rc);

void trimstr(char *str, int n);

void server_process(int sock_control);

#endif
