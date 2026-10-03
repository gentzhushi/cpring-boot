#ifndef CPRING_HTTP_STATUS_H_
#define CPRING_HTTP_STATUS_H_

enum cpring_http_status {
#define X(CODE, NAME, REASON) CPRING_HTTP_STATUS_##NAME = CODE,
#	include "status.def"
#undef X
	CPRING_HTTP_STATUS_COUNT,
};

struct cpring_sv cpring_http_status_to_reason(enum cpring_http_status status);

#endif // CPRING_HTTP_STATUS_H_