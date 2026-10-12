#include <stdio.h>

#include "stb_ds.h"

#include "method.h"
#include "request.h"
#include "header.h"

static int headers_done(const char* buf)
{
	return (buf[0] == '\r' && buf[1] == '\n') ? 1 : 0;
}

void cpring_request_print(struct cpring_request req)
{
	printf("request(\n");
	printf("  method=\""CPRING_SV_FMT"\",\n",
	       CPRING_SV_ARG(cpring_http_method_to_str(req.method)));
	printf("  route= \""CPRING_SV_FMT"\",\n", CPRING_SV_ARG(req.route));

	int i;
	printf("  headers=(\n");
	for (i = 0; i < arrlen(req.headers) - 1; i++)
		printf("    header=(key=\""CPRING_SV_FMT
		       "\", value=\""CPRING_SV_FMT"\"),\n",
		       CPRING_SV_ARG(req.headers[i].key),
		       CPRING_SV_ARG(req.headers[i].value));

	printf("    header=(key=\""CPRING_SV_FMT"\", value=\""CPRING_SV_FMT
	       "\")\n  )\n)\n",
	       CPRING_SV_ARG(req.headers[arrlen(req.headers)-1].key),
	       CPRING_SV_ARG(req.headers[arrlen(req.headers)-1].value));
}

int cpring_parse_request(struct cpring_request *req, char* buf, size_t bufsiz)
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

	char *next_token;

	char *method	= buf;
	next_token	= (char*)(memchr(method, ' ', bufsiz)) + 1;
	size_t mlen	= (size_t)(next_token - method - 1);
	struct cpring_sv method_str = {
		.data	= method,
		.len	= mlen,
	};
	req->method = cpring_http_method_from_str(method_str);

	char *route	= next_token;
	next_token	= (char*)(memchr(route, ' ', bufsiz - mlen - 1)) + 1;
	size_t rlen	= (size_t)(next_token - route - 1);
	req->route = (struct cpring_sv) {
		.data	= route,
		.len	= rlen,
	};

	char *version	= next_token;
	next_token	= (char*)(memchr(version, '\r', bufsiz - rlen - 1)) + 1;
	size_t vlen	= (size_t)(next_token - version - 1);
	req->version = (struct cpring_sv) {
		.data	= version,
		.len	= vlen,
	};

	/* next_token is already @ '\n', add 1 to skip current '\n' */
	buf = next_token + 1;
	while (!headers_done(buf)) {
		char* line = buf;
		buf = (char*)(memchr(buf, '\n', bufsiz)) + 1;
		size_t line_len = (buf - line - 2);
		struct cpring_sv header_sv = {
			.data	= line,
			.len	= line_len,
		};

		struct cpring_http_header header;
		cpring_header_parse(&header, header_sv);

		arrput(req->headers, header);
	}

	/* parse_body() */

	return 0;
}