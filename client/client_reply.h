#ifndef CLIENT_REPLY_H
#define CLIENT_REPLY_H

#include "../common/common.h"

int read_reply(int sock_control);

void print_reply(int rc);

#endif
