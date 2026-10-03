#ifndef CPRING_REQUEST_H_
#define CPRING_REQUEST_H_

#include "ds/sv.h"

#include "method.h"
#include "header.h"

struct cpring_request {
	struct cpring_http_header	*headers;
	struct cpring_sv		route;
	struct cpring_sv		version;
	enum cpring_http_method		method;
};


int cpring_parse_request(struct cpring_request *req, char* buf, size_t bufsiz);
void cpring_request_print(struct cpring_request req);

#endif // CPRING_REQUEST_H_