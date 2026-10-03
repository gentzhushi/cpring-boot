#include "sv.h"
#include "string.h"

int cpring_sv_eq(struct cpring_sv sva, struct cpring_sv svb)
{
	return sva.len == svb.len && !memcmp(sva.data, svb.data, sva.len);
}