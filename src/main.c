#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"
#include "header.h"

#define SERVER_PORT 80

typedef enum {
        F2CRTSK, /* Failed to create socket     */
        F2BNDSK, /* Failed to bind socket       */
        F2LISTN, /* Failed to listen socket     */
        F2ACCPT, /* Failed to accept connection */
} exit_codes;

static int setup();

int handle_request(request *req)
{
        (void)req;

        /* validate_request(); */
        /* respond();          */

        return 0;
}

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

                if (recv(c_sock, buf, BUFSIZ, 0) <= 0) continue;

                request req = parse_request(buf, BUFSIZ);
                printf("-----REQUEST--------------------------------------\n");
                req_print(req);
                printf("--------------------------------------------------\n");
                handle_request(&req);

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
                       &opt, sizeof(opt)) < 0)
                perror("socket");

        s_addr.sin_len         = sizeof(s_addr);
        s_addr.sin_family      = AF_INET;
        s_addr.sin_port        = htons(SERVER_PORT); /* 80 */
        s_addr.sin_addr.s_addr = INADDR_ANY;

        if(bind(s_sock, (struct sockaddr*) &s_addr, sizeof(s_addr)) < 0) {
                perror("bind");
                exit(F2BNDSK);
        }

        if (listen(s_sock, 1) < 0) {
                perror("listen");
                exit(F2LISTN);
        }

        return s_sock;
}
