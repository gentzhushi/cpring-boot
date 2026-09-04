#define NOB_IMPLEMENTATION
#include "nob.h"

int main(int argc, char *argv[])
{
        GO_REBUILD_URSELF(argc, argv);
        Cmd cmd = {0};
        cmd_append(&cmd, "cc", "-Wall", "-Wextra", "-o", "server.obj",
                   "src/main.c");

        if (!cmd_run(&cmd)) return 1;

        cmd.count = 0;

        cmd_append(&cmd, "./server.obj");

        return cmd_run(&cmd);
}
