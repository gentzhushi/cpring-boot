#ifndef CPRING_HTTP_RESPOND_H_
#define CPRING_HTTP_RESPOND_H_

#include "core/cpring.h"
#include "request.h"

struct cpring_response {
	struct cpring_sv		version;
	struct cpring_http_header	*headers;	/* `stb_ds` array */
	struct cpring_sv		body;
	struct cpring_sv		reason;	/* `NULL` by default */
	int				status;
};

// Call `cpring_respond_with()` with default arguments
#define cpring_respond(c_sock, ...)					\
cpring_respond_with(c_sock, (struct cpring_response){			\
	.version	= CPRING_SV_LIT_INIT("1.1"),			\
	.status		= 500,						\
	.body		= CPRING_SV_LIT_INIT("The server encountered an unexpected condition which prevented it from fulfilling the request."), \
	__VA_ARGS__							\
})

void cpring_respond_with(int c_sock, struct cpring_response args);
void cpring_response_print(struct cpring_response res);

#endif // CPRING_HTTP_RESPOND_H_
