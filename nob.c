#include <stdio.h>

#define NOB_IMPLEMENTATION
#include "thirdparty/nob.h"

int main(int argc, char *argv[])
{
	GO_REBUILD_URSELF(argc, argv);

	shift(argv, argc); // skip the program's name
	String_View build_type = SVLIT("normal");

	if (argc > 0) 
		build_type = sv_from_cstr(shift(argv, argc));

	Cmd cmd = {0};

	cmd_append(&cmd,
		"cc", "-Wall", "-Wextra", "-pedantic",
		"-fsanitize=address,undefined",
		"-std=c23",
		"-I./src",
		"-I./thirdparty",

		"example.c",
		"src/core/cpring.c",
		"src/ds/sv.c",
		"src/http/request.c",
		"src/http/header.c",
		"src/http/response.c",
		"src/http/status.c",
		"src/http/method.c",

		"thirdparty/cJSON.out",
	);

	if (sv_eq(build_type, SVLIT("debug")))
		cmd_append(&cmd, "-O0", "-g");

	cmd_append(&cmd, "-o", "app.out");

	if (!cmd_run(&cmd))
		return 1;

	nob_log(INFO, "Built sucessfully. Running...");

	cmd.count = 0; // clear command

	if (sv_eq(build_type, SVLIT("debug")))
		cmd_append(&cmd, "lldb");

	cmd_append(&cmd, "./app.out");

	return cmd_run(&cmd);
}