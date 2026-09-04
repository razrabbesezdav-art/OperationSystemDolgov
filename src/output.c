#include "../include/output.h"
#include <stdio.h>

void OutputInfo(ProcessInfo InfoProc){
    printf("Process information\n\n");
    printf("PID: %d\n", InfoProc.PID);
    printf("Name: %s\n", InfoProc.Name);
    printf("State: %s\n", InfoProc.State);
    printf("Parent PID: %d\n", InfoProc.ParentPID);
    printf("Command line: %s\n", InfoProc.CommandLine);
    printf("Open file descriptors: %d\n", InfoProc.FDcount);
}