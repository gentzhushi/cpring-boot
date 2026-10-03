#ifndef CPRING_HEADER_H_
#define CPRING_HEADER_H_

#include "ds/sv.h"

struct cpring_http_header {
	struct cpring_sv key;
	struct cpring_sv value;
};

int cpring_header_parse(struct cpring_http_header *hdr,
			struct cpring_sv header_str);

#endif // CPRING_HEADER_H_