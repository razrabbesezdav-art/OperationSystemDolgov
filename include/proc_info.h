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

ProcessInfo proc_info_init(int IntPID);
void proc_info_destroy(ProcessInfo);

#endif