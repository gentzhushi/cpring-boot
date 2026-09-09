#ifndef HTTP_REQUEST_H_
#define HTTP_REQUEST_H_

#include <stdint.h>
#include <stddef.h>

typedef enum {
#define X(METHOD) HTTP_##METHOD,
        #include "methods.def"
#undef X
        HTTP_METHOD_COUNT
} method;

method strtom( const char *str, size_t str_len);

typedef struct {
        char* key;
        char* value;
} header;

typedef struct {
        const char *r;
        method      m;
        header     *hs; /* STB dynamic array */
} request;

void    print_request(request req);
request parse_request(char *line, size_t len);
header  parse_header(char *line, size_t len);

#endif /* HTTP_REQUEST_H_ */
