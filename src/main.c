#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"
#include "request.h"
#include "respond.h"

#define SERVER_PORT  80
#define BACKLOG_SIZE 1

typedef enum {
        F2CRTSK, /* Failed to create socket     */
        F2BNDSK, /* Failed to bind socket       */
        F2LISTN, /* Failed to listen socket     */
        F2ACCPT, /* Failed to accept connection */
} exit_codes;

/* Setup the server, return the socket fd upon sucess. Crash on failure. */
static int setup();

int main(void)
{
        int s_sock, c_sock;
        s_sock = setup();

        char buf[BUFSIZ];
        for (;;) {
                c_sock = accept(s_sock, NULL, NULL);
                if (c_sock < 0) {
                        perror("accept");
                        exit(F2ACCPT);
                }

                if (recv(c_sock, buf, BUFSIZ, 0) <= 0) {
                        respond500(c_sock);
                        close(c_sock);
                        continue;
                }

                request req = parse_request(buf, BUFSIZ);
                print_request(req);
                respond(c_sock, req);

                close(c_sock);
        }

        return 0;
}

static int setup()
{
        int s_sock = 0;
        struct sockaddr_in s_addr = {0};

        s_sock = socket(PF_INET, SOCK_STREAM, 0);
        if (s_sock < 0) {
                perror("socket");
                exit(F2CRTSK);
        }

        int opt = 1;
        if (setsockopt(s_sock, SOL_SOCKET, SO_REUSEADDR,
                       &opt, sizeof(opt)) < 0) {
                perror("socket");
        }

        s_addr.sin_len         = sizeof(s_addr);
        s_addr.sin_family      = AF_INET;
        s_addr.sin_port        = htons(SERVER_PORT); /* 80 */
        s_addr.sin_addr.s_addr = INADDR_ANY;

        if(bind(s_sock, (struct sockaddr*) &s_addr, sizeof(s_addr)) < 0) {
                perror("bind");
                exit(F2BNDSK);
        }

        if (listen(s_sock, BACKLOG_SIZE) < 0) {
                perror("listen");
                exit(F2LISTN);
        }

        return s_sock;
}
