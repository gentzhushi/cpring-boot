#include <stdio.h>

#include "stb_ds.h"

#include "core/cpring.h"
#include "http/status.h"
#include "cJSON.h"

// JSON body:
// {
//	"file": "path/to/file",
//	"mode": "mode"
//	"data": "data to be written",
// }
// Mode: "r", "w", "b" for read, write, or binary; these can be combined
void post_writeFile(struct cpring_request *req, struct cpring_response *res)
{
	int hdr_count = arrlen(req->headers); (void) hdr_count;

	*res = (struct cpring_response) {
		.status = CPRING_HTTP_STATUS_ACCEPTED,
		.version = CPRING_SV_LIT("1.1"),
	};

	// drop a level
	req->body = req->body->child;

	char* file_name = req->body->valuestring;
	req->body = req->body->next;
	
	char* mode = req->body->valuestring;
	req->body = req->body->next;

	FILE *log_file = fopen(file_name, mode);

	char* data = req->body->valuestring;
	int data_len = strlen(data);

	if (fwrite(req->body->valuestring, data_len, 1, log_file) < 0) {
		struct cJSON *error_response = cJSON_CreateObject();
		cJSON_AddBoolToObject(error_response, "ok", cJSON_False);
		char *body_buf = cJSON_PrintUnformatted(error_response);
		size_t body_len = strlen(body_buf);
		res->body = (struct cpring_sv) {
			.data = body_buf,
			.len = body_len
		};
		cJSON_free(error_response);
	} else {
		struct cJSON *error_response = cJSON_CreateObject();
		cJSON_AddBoolToObject(error_response, "ok", cJSON_True);
		char *body_buf = cJSON_PrintUnformatted(error_response);
		size_t body_len = strlen(body_buf);
		res->body = (struct cpring_sv) {
			.data = body_buf,
			.len = body_len
		};
		cJSON_free(error_response);
	}

	fclose(log_file);
}

void get_healthcheck(struct cpring_request *req, struct cpring_response *res)
{
	(void) req;
	*res = (struct cpring_response) {
		.status = CPRING_HTTP_STATUS_OK,
		.version = CPRING_SV_LIT("1.1"),
	};

	printf("INSIDE `get_healthcheck()`\n");
}

int main(int argc, char **argv)
{
	struct cpring_app *app = cpring_new(argc, argv);

	cpring_map_endpoint(app, CPRING_HTTP_METHOD_POST, CPRING_SV_LIT("/write-file"), post_writeFile);
	cpring_map_endpoint(app, CPRING_HTTP_METHOD_GET, CPRING_SV_LIT("/healthcheck"), get_healthcheck);

	return cpring_run(app);
}
