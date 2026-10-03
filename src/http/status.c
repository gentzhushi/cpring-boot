#include "ds/sv.h"
#include "status.h"

static const struct cpring_sv status_strs[] = {
#define X(CODE, NAME, REASON) [CPRING_HTTP_STATUS_##NAME] = CPRING_SV_LIT_INIT(REASON),
#	include "status.def"
#undef X
};

static_assert(sizeof(status_strs)/sizeof(*status_strs)
	== CPRING_HTTP_STATUS_COUNT, "Status strs table out of sync");

struct cpring_sv cpring_http_status_to_reason(enum cpring_http_status status)
{
	return status_strs[status];
}