#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "respond.h"
#include "common.h"
#include "stb_ds.h"

void _respond(int c_sock, response res)
{
        char buf[BUFFSZ];

        snprintf(buf, BUFFSZ, "HTTP/1.1 %d %s" CRLF, res.status, res.reason);
        send(c_sock, buf, strlen(buf), 0);

        for (int i = 0; i < arrlen(res.hdrs); i++) {
                snprintf(buf, BUFFSZ, "%s: %s" CRLF, res.hdrs[i].key, res.hdrs[i].value);
                send(c_sock, buf, strlen(buf), 0);
        }

        send(c_sock, "Content-Length: 26" CRLF, 20, 0);

        send(c_sock, CRLF, 2, 0); // Send empty line

        send(c_sock, "Welcome to Clang Boot 0.1!", 26, 0);

        printf("Sent, returning...\n");

        return;
}
