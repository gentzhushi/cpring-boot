#include <stdio.h>

#include "stb_ds.h"
#include "cJSON.h"

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

	printf("    header=(key=\""CPRING_SV_FMT"\", value=\""CPRING_SV_FMT"\")"
	       "\n  )\n",
	       CPRING_SV_ARG(req.headers[arrlen(req.headers)-1].key),
	       CPRING_SV_ARG(req.headers[arrlen(req.headers)-1].value));

	printf("  body=(\n    %s\n  )\n", cJSON_PrintUnformatted(req.body));

	printf(")\n");
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

	char *buf_end = buf + bufsiz;

	char *next_token = buf;

	char *method	= buf;
	next_token	= (char*)(memchr(method, ' ', buf_end - next_token)) + 1;
	size_t mlen	= (size_t)(next_token - method - 1);
	struct cpring_sv method_str = {
		.data	= method,
		.len	= mlen,
	};
	req->method = cpring_http_method_from_str(method_str);

	//						  bufsiz(47)
	//						  ↓
	// GET / HTTP/1.1\r\nContent-Type: application/json
	//    ↑
	//    next_token(3)

	char *route	= next_token;
	next_token	= (char*)(memchr(route, ' ', buf_end - next_token - 1)) + 1;
	size_t rlen	= (size_t)(next_token - route - 1);
	req->route = (struct cpring_sv) {
		.data	= route,
		.len	= rlen,
	};

	char *version	= next_token;
	next_token	= (char*)(memchr(version, '\r', buf_end - next_token - 1)) + 1;
	size_t vlen	= (size_t)(next_token - version - 1);
	req->version = (struct cpring_sv) {
		.data	= version,
		.len	= vlen,
	};

	buf = next_token + 1; // next_token is already @ '\n', add 1 to skip current '\n'

	while (!headers_done(buf)) {
		char* line = buf;
		buf = (char*)(memchr(buf, '\n', buf_end - buf - 1)) + 1;
		// NOTE: May have an off-by-one error here:     ↑
		size_t line_len = (buf - line - 2);
		struct cpring_sv header_sv = {
			.data	= line,
			.len	= line_len,
		};

		struct cpring_http_header header;
		cpring_header_parse(&header, header_sv);

		arrput(req->headers, header);
	}

	// When the loop is done, we're pointing at the beginning the empty 
	// line. Next token is pointing here: "\r\n<body>"
	//					↑
	// 					next_token;
	//
	// So we add 2 to offset it to the start.

	buf += 2;
	printf("BODY_BUFFER: "CPRING_SV_FMT"\n", buf_end - buf, buf);

	// we assume content-type is application/json

	struct cJSON *body = cJSON_ParseWithLength(buf, (size_t)(buf_end - buf));
	req->body = body;

	return 0;
}