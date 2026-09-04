#ifndef PROC_INFO_H
#define PROC_INFO_H

typedef struct {
    int PID;
    int ParentPID;
    char Name[64];
    char State[64];
    char *CommandLine;
    int FDcount;
} ProcessInfo;

ProcessInfo proc_info_init(void);

int proc_info_fill(int pid, ProcessInfo *info);

void proc_info_destroy(ProcessInfo *info);

#endif