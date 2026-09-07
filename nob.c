#define NOB_IMPLEMENTATION
#include "nob.h"

int main(int argc, char *argv[])
{
        GO_REBUILD_URSELF(argc, argv);
        Cmd cmd = {0};
        cmd_append(&cmd,
                   "cc", "-Wall", "-Wextra", "-Werror",
                   "-std=c99",
                   "-o", "server.obj",
                   "-I./third_party",
                   "src/main.c", "src/header.c"
                   );

        if (!cmd_run(&cmd)) return 1;

        cmd.count = 0;

        cmd_append(&cmd, "./server.obj");

        return cmd_run(&cmd);
}
