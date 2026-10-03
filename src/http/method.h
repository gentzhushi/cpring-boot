#ifndef CPRING_METHODS_H_
#define  CPRING_METHODS_H_

#include "ds/sv.h"

enum cpring_http_method {
#define X(METHOD) CPRING_HTTP_METHOD_##METHOD,
#	include "method.def"
#undef X
	CPRING_HTTP_METHOD_COUNT,
};

enum cpring_http_method cpring_http_method_from_str(struct cpring_sv str);
struct cpring_sv cpring_http_method_to_str(enum cpring_http_method method);

#endif // CPRING_METHODS_H_