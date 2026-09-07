#ifndef HTTP_HEADER_H_
#define HTTP_HEADER_H_

#include <stdint.h>
#include <stddef.h>

typedef const char* route; /* TODO: Implement this */

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
        route   r;
        method  m;
        header *hs;
} request;

void    print_request(request req);
int     handle_request(request *req)
request parse_request(char *line, size_t len);
header  parse_header(char *line, size_t len);

#endif /* HTTP_HEADER_H_ */
