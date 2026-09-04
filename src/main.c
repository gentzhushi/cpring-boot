#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <unistd.h>

typedef enum {
        F2CRATSK,
        F2BINDSK,
        F2LISTEN, // Failed to listen socket
        F2ACCEPT, // Failed to accept connection
} exit_codes;

int handle_request(uint8_t *buf, size_t bufsiz)
{
        printf("requesti: %s!!!\n", buf);
        // Format for HTTP request:
        // METHOD /route/to/resource HTTP 1.3\r\n
        // 
}

int main(void)
{
        int s_sock = 0, c_sock = 0;
        struct sockaddr_in s_addr = {0};

        s_sock = socket(PF_INET, SOCK_STREAM, 0);
        if (s_sock < 0) {
                perror("socket");
                exit(F2CRATSK);
        }

        s_addr.sin_len         = sizeof(s_addr);
        s_addr.sin_family      = AF_INET;
        s_addr.sin_port        = htons(80);
        s_addr.sin_addr.s_addr = INADDR_ANY;

        if(bind(s_sock, (struct sockaddr*) &s_addr, sizeof(s_addr)) < 0) {
                perror("bind");
                exit(F2BINDSK);
        }

        if (listen(s_sock, 1) < 0) {
                perror("listen");
                exit(F2LISTEN);
        }

        printf("Server started...\n");

        uint8_t buf[BUFSIZ];

        for (;;) {
                c_sock = accept(s_sock, NULL, NULL);

                if (recv(c_sock, buf, BUFSIZ, 0) <= 0) continue;

                handle_request(buf, BUFSIZ);
        }

        return 0;
}
