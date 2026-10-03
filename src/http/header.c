#include <string.h>

#include "header.h"

int cpring_header_parse(struct cpring_http_header *hdr,
			struct cpring_sv header_str)
{
	char *column_character = memchr(header_str.data, ':', header_str.len); 
	if (!column_character) 
		return -1;
	
	size_t key_len = (size_t)(column_character - header_str.data);

	hdr->key.data	= header_str.data;
	hdr->key.len	= key_len;
	
	/* 
	 * An HTTP header looks like this: `HEADER_KEY: HEADER_VALUE`;
	 * So, in order to extract the header value, we need to point
	 * `hdr.value.data` to `hdr.key.data + hdr.key.len + 2` (`+2` to offset 
	 * `: `). We also need to set the value length to the following:
	 * `header_str.len - hdr.key.len - 2`. Here's an explanation:
	 * > 1 2 3 4 5 6 7 8	< Length of `header_str`
	 * > K E Y : _ V A L	< Content of it
	 * > 8 - 3 - 2 = 3	< Length of `hdr.value.len` 
	 * > `header_str.len - hdr.key.len - 2` < after substituting
	*/

	hdr->value.data	= hdr->key.data + hdr->key.len + 2;
	hdr->value.len	= header_str.len - hdr->key.len - 2;

	return 0;
}