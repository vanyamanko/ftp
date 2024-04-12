#ifndef CLIENT_INPUT_H
#define CLIENT_INPUT_H

#include "../common/common.h"
#include "client_network.h"
#include "client_reply.h"

void read_input(char* buffer, int size);
int client_read_command(char* buf, int size, struct command *cstruct);
void client_login(int sock_control);

#endif
