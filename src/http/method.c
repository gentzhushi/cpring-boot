#include "ds/sv.h"
#include "method.h"

static const struct cpring_sv method_strs[] = {
#define X(METHOD) [CPRING_HTTP_METHOD_##METHOD] = CPRING_SV_LIT_INIT(#METHOD),
#	include "method.def"
#undef X
};

/* Compile-time check; it's a keyword in C23, which this project is using */
static_assert(sizeof(method_strs) / sizeof(*method_strs)
	== CPRING_HTTP_METHOD_COUNT, "Http method table out of sync.");

enum cpring_http_method cpring_http_method_from_str(struct cpring_sv str)
{
#define X(METHOD)							\
	if (cpring_sv_eq(str, CPRING_SV_LIT(#METHOD))) {		\
		return CPRING_HTTP_METHOD_##METHOD;			\
	}
#	include "method.def"
#undef X
	return CPRING_HTTP_METHOD_COUNT; /* Error */
}

struct cpring_sv cpring_http_method_to_str(enum cpring_http_method method)
{
	return method_strs[method];
}