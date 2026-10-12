#include <stdio.h>

#include "core/cpring.h"
#include "http/status.h"

void get_test(struct cpring_request *req, struct cpring_response *res)
{
	(void) req;
	*res = (struct cpring_response) {
		.status = CPRING_HTTP_STATUS_ACCEPTED,
		.version = CPRING_SV_LIT("1.1"),
	};

	printf("INSIDE `get_test()`\n");
}

void get_ok(struct cpring_request *req, struct cpring_response *res)
{
	(void) req;
	*res = (struct cpring_response) {
		.status = CPRING_HTTP_STATUS_OK,
		.version = CPRING_SV_LIT("1.1"),
	};

	printf("INSIDE `get_ok()`\n");
}

int main(int argc, char **argv)
{

	struct cpring_app *app = cpring_new(argc, argv);

	cpring_map_endpoint(app, CPRING_HTTP_METHOD_GET,
			    CPRING_SV_LIT("/test"), get_test);
	cpring_map_endpoint(app, CPRING_HTTP_METHOD_GET,
			    CPRING_SV_LIT("/ok"), get_ok);

	return cpring_run(app);
}
