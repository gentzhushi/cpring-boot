#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "respond.h"
#include "common.h"

void _respond500(int c_sock, struct respond500args args)
{
        char *buf;

        asprintf(&buf,
                 "HTTP/1.1 500 Internal Server Error" CRLF
                 CRLF
                 "<html>" CRLF
                  "<head>" CRLF
                   "<title>%s</title>" CRLF
                   "<style>" CRLF
                    "* { background-color:white; color:black} " CRLF
                    "@media (prefers-color-scheme:dark) {" CRLF
                     "* {" CRLF
                      "background-color:black; color:white" CRLF
                     "}" CRLF
                    "}" CRLF
                   "</style>" CRLF
                  "</head>" CRLF
                  "<body style=\"display:flex;justify-content:center;" CRLF
                                "align-items:center\">" CRLF
                   "<div style=\"max-width:50%%;display:flex;"
                                "justify-content:start;align-items:center;" CRLF
                                "flex-direction:column\">" CRLF
                    "<h1 style=\"font-family:sans-serif;font-size:128px;"
                                "margin-bottom: 0\">"
                     "500"
                    "</h1>" CRLF
                    "<h1 style=\"font-family:sans-serif\">"
                     "%s"
                    "</h1>" CRLF
                    "<hr style=\"width:100%%\"/>" CRLF
                    "<p style=\"text-align:center\">" CRLF
                     "%s" CRLF
                    "</p>" CRLF
                    "<p style=\"font-style:italic;color:grey\">" CRLF
                     "Clang Boot 0.1" CRLF
                    "</p>" CRLF
                   "</div>" CRLF
                  "</body>" CRLF
                 "</html>" CRLF,
                 args.title,
                 args.title,
                 args.message);

        printf("SENDING:\"%s\"\n", buf);

        send(c_sock, buf, strlen(buf), 0);
        free(buf);
}

int respond(int sock, request req)
{
        if (!(strncmp(req.r, "/ok", 4) == 0)) {
                respond500(sock,
                   .message = "This is an internal server error. Lorem ipsum "
                              "dolor sit amet, consectetur adipiscing elit, "
                              "sed do eiusmod tempor incididunt ut labore et "
                              "dolore magna aliqua."
                );
                return 500;
        }

        const char *buf =
                "HTTP/1.1 200 OK\r\n"
                "\r\n"
                "<html>"
                "<h1 style=\"color:red\">Totally legit website</h1>"
                "</html>"
                ;

        /* validate_request(); */
        /* respond();          */

        send(sock, buf, strlen(buf), 0);

        return 200;
}
