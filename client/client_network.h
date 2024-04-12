#ifndef CLIENT_NETWORK_H
#define CLIENT_NETWORK_H

#include "../common/common.h"

int client_open_conn(int sock_con);
int client_send_cmd(struct command *cmd, int sock_control);

#endif
