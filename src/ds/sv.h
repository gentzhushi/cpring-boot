#ifndef CPRING_SV_H_
#define CPRING_SV_H_

#include <stddef.h>

struct cpring_sv {
	const char	*data;
	size_t		len;
};

#define CPRING_SV_FMT		"%.*s"
#define CPRING_SV_ARG(sv)	(int)(sv).len, (sv).data
#define CPRING_SV_LIT_INIT(lit)	{ .data = ""lit , .len = sizeof(""lit) - 1}
#define CPRING_SV_LIT(lit)						\
	((struct cpring_sv) { .data = ""lit , .len = sizeof(""lit) - 1})

int cpring_sv_eq(struct cpring_sv sva, struct cpring_sv svb);

#endif // CPRING_SV_H_