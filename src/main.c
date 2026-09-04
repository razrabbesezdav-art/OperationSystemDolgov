#include "args.h"
#include "proc_info.h"
#include "output.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <PID>\n", argv[0]);
        return 2;
    }

    int pid = CheckPID(argv[1]);
    if (pid < 0) {
        fprintf(stderr, "Ошибка: некорректный PID '%s'\n", argv[1]);
        return 2;
    }

    ProcessInfo info = proc_info_init();

    if (proc_info_fill(pid, &info) != 0) { 
        proc_info_destroy(&info);
        return 1;
    }


    OutputInfo(info);

    proc_info_destroy(&info);

    return 0;
}