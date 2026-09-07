#include "header.h"
#include "stb_ds.h"
#include <string.h>
#include <stdio.h>

static int _headers_done(const char* buf)
{
        return (buf[0] == '\r' && buf[1] == '\n') ? 1 : 0;
}

header parse_header(char* line, size_t len)
{
        char *value;

        header hdr = {0};

        value         = memchr(line, ' ', len);
        *(value - 1)  = '\0';   /* set column (:) to nul (create new string) */
        value        += 1;      /* move value to the start of the new string */
        line[len] = '\0';

        hdr.key = line;
        hdr.value = value;

        return hdr;
}

static const char *mstrs[] = {
#define X(METHOD) #METHOD,
        #include "methods.def"
#undef X
};

method strtom(const char *line, size_t line_len)
{
#define X(METHOD) if (strncmp(line, #METHOD, line_len) == 0) return HTTP_##METHOD;
        #include "methods.def"
#undef X
        return HTTP_METHOD_COUNT; /* Error */
}

request parse_request(char *buf, size_t bufsiz)
{
        /**
         * Format for HTTP request:
         * Request Line   -> METHOD /route/to/resource HTTP/1.1\r\n
         * General Header -> general_header\r\n
         * Request Header -> request_header\r\n
         * Entity  Header -> entity_header\r\n
         * \r\n
         * Message Body   ->
         */

        request req = {0};

        /* parse Request Line */
        char *next_token;

        char *method      = buf;
        next_token        = memchr(method, ' ', bufsiz) + 1;
        size_t mlen       = (int)(next_token - method - 1);

        char *route       = next_token;
        next_token        = memchr(route, ' ', bufsiz - mlen - 1) + 1;
        *(next_token - 1) = '\0';
        size_t rlen       = (int)(next_token - route - 1);

        char *version     = next_token;
        next_token        = memchr(version, '\r', bufsiz - rlen - 1) + 1;
        size_t vlen       = (int)(next_token - version - 1);
        (void) vlen;

        req.m = strtom(method, mlen);
        req.r = route;

        /* next_token is already @ '\n', add 1 to skip current '\n' */
        buf = next_token + 1;
        while (!_headers_done(buf)) {
                char* line = buf;
                buf = memchr(buf, '\n', bufsiz) + 1;
                size_t line_len = (buf - line - 2);

                arrput(req.hs, parse_header(line, line_len));
        }

        /* parse_body() */

        return req;
}

void req_print(request req)
{
        printf("request(\n");
        printf("  method=\"%s\",\n", mstrs[req.m]);
        printf("  route= \"%s\",\n", req.r);

        int i;
        printf("  headers=(\n");
        for (i = 0; i < arrlen(req.hs) - 1; i++)
                printf("    header=(key=\"%s\", value=\"%s\"),\n", req.hs[i].key, req.hs[i].value);

        printf("    header=(key=\"%s\", value=\"%s\")\n  )\n)\n",
               req.hs[arrlen(req.hs)-1].key,
               req.hs[arrlen(req.hs)-1].value);
}
