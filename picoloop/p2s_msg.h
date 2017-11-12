//
// Created by cpasjuste on 21/06/17.
//

#ifndef PROJECT_P2S_MSG_H
#define PROJECT_P2S_MSG_H

//#include "../psp2shell_k/psp2shell_k.h"
#include "psp2shell_k.h"

#define P2S_ERR_SOCKET      0x80000001
#define P2S_ERR_INVALID_MSG 0x80000002

typedef struct P2S_MSG {
    int color;
    char buffer[P2S_KMSG_SIZE];
} P2S_MSG;
#define P2S_SIZE_MSG    (sizeof(P2S_MSG) + 2)

enum p2s_colors_t {
    COL_NONE = 10,
    COL_RED,
    COL_YELLOW,
    COL_GREEN,
    COL_HEX
};

int p2s_msg_receive(int sock, P2S_MSG *msg);

void p2s_msg_send(int sock, int color, const char *msg);

int p2s_msg_send_msg(int sock, P2S_MSG *msg);

int p2s_msg_to_string(char *buffer, P2S_MSG *cmd);

int p2s_msg_to_msg(P2S_MSG *msg, const char *buffer);

#endif //PROJECT_P2S_MSG_H
