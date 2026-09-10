#ifndef HTTP_RESPOND_H_
#define HTTP_RESPOND_H_

#include "request.h"

typedef struct {
        const char *reason;     // Http response reason phrase
        header     *hdrs;       // Http headers
        const char *body;       // Content after the empty line
        size_t      body_len;
        int         status;     // Http status code
} response;

#define respond(c_sock, ...)                                                   \
_respond(c_sock, (response){                                                   \
            .status = 500,                                                     \
            .reason = "Internal Server Error",                                 \
            .body   = "The server encountered an unexpected condition which "  \
                       "prevented it from fulfilling the request.",            \
            .hdrs   = NULL,                                                    \
            __VA_ARGS__                                                        \
            })

void _respond(int c_sock, response args);

#endif // HTTP_RESPOND_H_
