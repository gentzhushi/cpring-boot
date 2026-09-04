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

typedef enum {
        F2CRTSK, // Failed to create socket
        F2BNDSK, // Failed to bind socket
        F2LISTN, // Failed to listen socket
        F2ACCPT, // Failed to accept connection
} exit_codes;

http_request parse_request(char *buf, size_t bufsiz)
{
        /**
         * Format for HTTP request:
         * Request Header -> METHOD /route/to/resource HTTP/1.1\r\n
         * General Header -> general_header\r\n
         * Request Header -> request_header\r\n
         * Entity  Header -> entity_header\r\n
         * \r\n
         * Message Body   ->
         */

        http_request req = {0};

        //while (!headers_done(buf)) {
        while (!headers_done(buf)) {
                char* line = buf;
                buf = memchr(buf, '\n', bufsiz) + 1;
                size_t len = (buf - line);

                printf("HEADER=%.*s\n", (int)len, line);
                //arrput(req.hdrs, parse_header(line));
        }

        buf += 1; // skip the '\n'

        printf("BODY=%s\n", buf);
        // parse_body()
        return req;
}

int handle_request(http_request *req)
{
        (void)req;
        return 0;
}

int main(void)
{
        int s_sock = 0, c_sock = 0;
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
                exit(F2CRTSK);
        }

        s_addr.sin_len         = sizeof(s_addr);
        s_addr.sin_family      = AF_INET;
        s_addr.sin_port        = htons(80);
        s_addr.sin_addr.s_addr = INADDR_ANY;

        if(bind(s_sock, (struct sockaddr*) &s_addr, sizeof(s_addr)) < 0) {
                perror("bind");
                exit(F2BNDSK);
        }

        if (listen(s_sock, 1) < 0) {
                perror("listen");
                exit(F2LISTN);
        }

        printf("Server started...\n");

        char buf[BUFSIZ];

        for (;;) {
                c_sock = accept(s_sock, NULL, NULL);
                if (c_sock < 0) {
                        perror("accept");
                        exit(F2ACCPT);
                }

                if (recv(c_sock, buf, BUFSIZ, 0) <= 0) continue;

//                printf("```\n%s\n```\n", buf);
                http_request req = parse_request(buf, BUFSIZ);
                handle_request(&req);
                close(c_sock);
        }

        return 0;
}
