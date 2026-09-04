#include "header.h"

int headers_done(const char* buf)
{
        return (buf[0] == '\r' && buf[1] == '\n') ? 1 : 0;
}

http_header parse_header(const char* line)
{
        (void)line; // TODO: This is just to hush Wextra
        http_header hdr = {0};

        return hdr;
}
