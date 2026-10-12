#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#include "cpring.h"
#include "ds/sv.h"
#include "http/method.h"
#include "http/request.h"
#include "http/response.h"

struct cpring_endpoint{
	enum cpring_http_method method;
	struct cpring_sv route;
	cpring_handler_t handler;
};

struct cpring_app {
	struct cpring_endpoint *endpoints;
	in_addr_t ip_addr;
	int sockfd;
	int port;
	int backlog_size;
};

/*
 * Setup the server, return the socket fd upon sucess. Crash on failure.
 */
struct cpring_app *cpring_new(int argc, char** argv)
{
	(void) argc; (void) argv;
	// Parse CLI arguments:
	//	--port 80,
	//	--ip_addr (to bind to) 0.0.0.0,
	//	--backlog_sz,

	struct cpring_app *app = calloc(1, sizeof(struct cpring_app));

	app->port = 80;

	app->sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (app->sockfd < 0) {
		perror("socket");
		exit(1); // Failed to create socket
	}

	int opt = 1;
	if (setsockopt(app->sockfd, SOL_SOCKET, SO_REUSEADDR,
		&opt, sizeof(opt)) < 0) {
		perror("socket");
	}

	struct sockaddr_in s_addr = {
		.sin_len		= sizeof(struct sockaddr_in),
		.sin_family		= AF_INET,
		.sin_port		= htons(app->port),
		.sin_addr.s_addr	= INADDR_ANY,
	};

	if(bind(app->sockfd, (struct sockaddr*) &s_addr, sizeof(s_addr)) < 0) {
		perror("bind");
		exit(2); // Failed to bind socket
	}

	if (listen(app->sockfd, app->backlog_size) < 0) {
		perror("listen");
		exit(3); // Failed to listen
	}

	return app;
}

void cpring_free(struct cpring_app *app)
{
	free(app);
}

int cpring_map_endpoint(struct cpring_app *app, enum cpring_http_method method,
			struct cpring_sv route, cpring_handler_t handler)
{
	if (!route.data || !handler)
		return -1;

	struct cpring_endpoint ep = {
		.method		= method,
		.route		= route,
		.handler	= handler,
	};

	arrput(app->endpoints, ep);
	return 0;
}

int cpring_run(struct cpring_app *app)
{
	printf("Listening on 0x%08X:%d\n", app->ip_addr, app->port);
	char req_buf[CPRING_BUFSIZ] = {0};
	for (;;) {
		int c_sock = accept(app->sockfd, NULL, NULL);
		if (c_sock < 0) {
			perror("accept");
			exit(4);
		}

		ssize_t n = recv(c_sock, req_buf, CPRING_BUFSIZ, 0);
		if (n <= 0) {
			cpring_respond(c_sock); // Default 500
			close(c_sock);
			continue;
		}

		struct cpring_request req = {0};
		if(cpring_parse_request(&req, req_buf, CPRING_BUFSIZ) < 0) {
			cpring_respond(c_sock); // Default 500
			close(c_sock);
			continue;
		}

		cpring_request_print(req);

		struct cpring_response res = {0};
		struct cpring_endpoint *ep;

		for (int i = 0; i < arrlen(app->endpoints); i++) {
			ep = &app->endpoints[i];

			if (ep->method == req.method && 
			    cpring_sv_eq(ep->route, req.route)) {
				ep->handler(&req, &res);
			}
		}

		cpring_response_print(res);
		cpring_respond_with(c_sock, res);
	}
}