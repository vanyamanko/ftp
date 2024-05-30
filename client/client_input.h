#ifndef CLIENT_INPUT_H
#define CLIENT_INPUT_H

#include "../common/common.h"
#include "client_network.h"
#include "client_reply.h"
#include "client_utils.h"

void read_input(char* buffer, int size);
int client_read_command(char* buf, int size, struct command *cstruct, int* dir_flag);
void client_login(int sock_control);

#endif
