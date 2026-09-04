#ifndef PROC_INFO_H
#define PROC_INFO_H
typedef struct{
    int PID;
    int ParentPID;
    char Name[64];
    char State[64];
    char *CommandLine;
    int FDcount;
}ProcessInfo;

ProcessInfo proc_info_init();
ProcessInfo proc_info_fill(int IntPID, ProcessInfo result);
void proc_info_destroy(ProcessInfo);
static char *read_cmdline(int IntPID);
int count_fd(int IntPID);

#endif