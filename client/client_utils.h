#ifndef CLIENT_UTILS_H
#define CLIENT_UTILS_H

#include "../common/common.h"

int client_get(int data_sock, char* arg);
void client_put(int data_sock, char* arg);
int client_list(int sock_data, int sock_con);

#endif
