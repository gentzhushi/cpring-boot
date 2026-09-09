#ifndef HTTP_RESPOND_H_
#define HTTP_RESPOND_H_

#include "request.h"

struct respond500args {
        const char* title;
        const char* message;
};

#define respond500(c_sock, ...)                                                \
_respond500(c_sock, (struct respond500args){                                   \
            .title   = "Internal Server Error",                                \
            .message = "The server encountered an unexpected condition which " \
                       "prevented it from fulfilling the request.",            \
            __VA_ARGS__                                                        \
            })

void _respond500(int c_sock, struct respond500args args);
int   respond(int sock, request req);

#endif // HTTP_RESPOND_H_
