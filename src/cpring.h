#ifndef CPRING_H_
#define  CPRING_H_

#define CPRING_BUFSIZ 8192
#define CRLF "\r\n"

#include "http/method.h"
#include "http/request.h"
#include "http/response.h"


struct cpring_app;

struct cpring_app *cpring_new(int argc, char **argv);

void cpring_free(struct cpring_app *app);

typedef void (*cpring_handler_t)(struct cpring_request *req,
				 struct cpring_response *res);

int cpring_map_endpoint(struct cpring_app *app, enum cpring_http_method method,
			struct cpring_sv route, cpring_handler_t handler);

int cpring_run(struct cpring_app *app);

#endif // CPRING_H_