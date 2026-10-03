#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "stb_ds.h"

#include "cpring.h"
#include "method.h"
#include "response.h"
#include "status.h"

void cpring_respond_with(int c_sock, struct cpring_response res)
{
	struct cpring_sv hdr_buf = {0};
	hdr_buf.data = malloc(CPRING_BUFSIZ);

	if (res.reason.len == 0) res.reason = cpring_http_status_to_reason(res.status);
	if (res.reason.len == 0) res.reason = CPRING_SV_LIT("Unknown reason phrase");

	hdr_buf.len += snprintf(
		hdr_buf.data + hdr_buf.len,
		CPRING_BUFSIZ, 
		"HTTP/"CPRING_SV_FMT" %d "CPRING_SV_FMT"" CRLF, 
		CPRING_SV_ARG(res.version), res.status, CPRING_SV_ARG(res.reason)
	);

	hdr_buf.len += snprintf(
		hdr_buf.data + hdr_buf.len,
		CPRING_BUFSIZ,
		"Content-Length: %zu" CRLF, res.body.len
	);

	for (ssize_t i = 0; i < arrlen(res.headers); i++) {
		hdr_buf.len += snprintf(
			hdr_buf.data + hdr_buf.len,
			CPRING_BUFSIZ,
			""CPRING_SV_FMT": "CPRING_SV_FMT"" CRLF,
			CPRING_SV_ARG(res.headers[i].key),
			CPRING_SV_ARG(res.headers[i].value)
		);
	}

	hdr_buf.len += snprintf(
		hdr_buf.data + hdr_buf.len,
		CPRING_BUFSIZ,
		CRLF
	);

	send(c_sock, hdr_buf.data, hdr_buf.len, 0);
	// free(hdr_buf.data);

	return;
}

void cpring_response_print(struct cpring_response res)
{
	printf("response(\n");
	printf("  version="CPRING_SV_FMT",\n", CPRING_SV_ARG(res.version));
	printf("  status=%d,\n", res.status);
	printf("  reason="CPRING_SV_FMT",\n", CPRING_SV_ARG(res.reason));
	int i;
	printf("  headers=(\n");
	for (i = 0; i < arrlen(res.headers) - 1; i++) {
		printf("    header=(key=\""CPRING_SV_FMT
		       "\", value=\""CPRING_SV_FMT"\"),\n",
		       CPRING_SV_ARG(res.headers[i].key),
		       CPRING_SV_ARG(res.headers[i].value));
	}
	printf("  ),\n  body=\""CPRING_SV_FMT"\"\n)\n", CPRING_SV_ARG(res.body));
}