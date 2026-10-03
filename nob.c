#include <stdio.h>

#define NOB_IMPLEMENTATION
#include "thirdparty/nob.h"

int main(int argc, char *argv[])
{
	GO_REBUILD_URSELF(argc, argv);

	Cmd cmd = {0};
	cmd_append(&cmd,
		"cc", "-Wall", "-Wextra", "-pedantic",
		/* "-fsanitize=address,undefined", */
		"-std=c23",
		"-I./src",
		"-I./thirdparty",

#ifndef NOB_DEBUG
		"-o", "app.out",
#endif // NOB_DEBUG
		
#ifdef NOB_DEBUG
		/* Debug */
		"-O0",
		"-g",
		"-o", "app-debug.out",
#endif // NOB_DEBUG

		"src/cpring.c",
		"src/ds/sv.c",
		"src/main.c",
		"src/http/request.c",
		"src/http/header.c",
		"src/http/response.c",
		"src/http/status.c",
		"src/http/method.c",
	);

	if (!cmd_run(&cmd)) {
		nob_log(ERROR, "Failed to build.");
		return 1;
	}

	cmd.count = 0; // clear command

#ifndef NOB_DEBUG
	printf("TEST outta debug\n");
	cmd_append(&cmd, "./app.out");
#endif // NOB_DEBUG
	
#ifdef NOB_DEBUG
	printf("TEST in debug\n");
	cmd_append(&cmd, "lldb ./app-debug.out");
#endif // NOB_DEBUG

	return cmd_run(&cmd);
}
