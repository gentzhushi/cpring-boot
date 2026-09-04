#ifndef HTTP_HEADER_H_
#define HTTP_HEADER_H_

#include <stdint.h>

typedef enum {
        OPTIONS,
        GET,
        HEAD,
        POST,
        PUT,
        DELETE,
        TRACE,
        CONNECT,
} http_method;

// TODO: Implement this
typedef const char* http_route;

typedef struct {
        http_method method;
        char* route;
        uint8_t vmaj; // HTTP version major
        uint8_t vmin; // HTTP version minor
} http_header;

typedef enum {
        CACHE_CONTROL,
        CONNECTION,
        DATE,
        PRAGMA,
        TRAILER,
        TRANSFER_ENCODING,
        UPGRADE,
        VIA,
        WARNING,
} general_header;

typedef enum {
        ACCEPT,
        ACCEPT_CHARSET,
        ACCEPT_ENCODING,
        ACCEPT_LANGUAGE,
        AUTHORIZATION,
        EXPECT,
        FROM,
        HOST,
        IF_MATCH,
} request_header;

typedef enum {
        ALLOW,
        CONTENT_ENCODING,
        CONTENT_LANGUAGE,
        CONTENT_LENGTH,
        CONTENT_LOCATION,
        CONTENT_MD5,
        CONTENT_RANGE,
        CONTENT_TYPE,
        EXPIRES,
        LAST_MODIFIED,
} entity_header;

typedef struct {
        http_header *hdrs;
        http_route   route;
        http_method  method;
} http_request;

int headers_done(const char* buf);
http_header parse_header(const char* line);

#endif // HTTP_HEADER_H_
