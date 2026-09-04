#include "../include/proc_info.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *read_cmdline(int IntPID) {
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "/proc/%d/cmdline", IntPID);
    
    FILE *fp = fopen(filepath, "rb");
    if (fp == NULL) {
        return NULL;
    }
    
    char buffer[4096];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, fp);
    fclose(fp);
    
    if (bytes_read == 0) {
        return NULL;
    }
    
    buffer[bytes_read] = '\0';
    
    for (size_t i = 0; i < bytes_read; i++) {
        if (buffer[i] == '\0') {
            buffer[i] = ' ';
        }
    }
    
    if (buffer[bytes_read - 1] == ' ') {
        buffer[bytes_read - 1] = '\0';
    }
    
    return strdup(buffer);
}

int count_fd(int IntPID) {
    char command[256];
    char line[256];
    int fd_counter = 0;
    
    snprintf(command, sizeof(command), "ls -l /proc/%d/fd", IntPID);
    
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        return -1;
    }
    
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strstr(line, "lrwx") != NULL) {
            fd_counter++;
        }
    }
    
    pclose(fp);
    return fd_counter;
}

ProcessInfo proc_info_init(){
    ProcessInfo result = {};
    result.PID = 0;
    result.ParentPID = -1;
    result.CommandLine = NULL;
    result.FDcount = 0;
    strcpy(result.Name, "Unknown");
    strcpy(result.State, "Unknown");
    return result;
}

ProcessInfo proc_info_fill(int IntPID, ProcessInfo result) {
    result.PID = IntPID;
    char StateCommand[256];
    char Stateline[256];
    snprintf(StateCommand, sizeof(StateCommand), 
             "cat /proc/%d/status | head -n 15", IntPID);
    
    FILE *fp = popen(StateCommand, "r");
    if (fp == NULL) {
        perror("popen failed");
        return;
    }
    
    while (fgets(Stateline, sizeof(Stateline), fp) != NULL) {
        if (strncmp(Stateline, "Name:", 5) == 0) {
            sscanf(Stateline, "Name: %63s", result.Name);
        }
        else if (strncmp(Stateline, "State:", 6) == 0) {
            sscanf(Stateline, "State: %63[^\n]", result.State);
        }
        else if (strncmp(Stateline, "PPid:", 5) == 0) {
            sscanf(Stateline, "PPid: %d", &result.ParentPID);
            break;
        }
    }
    pclose(fp);
    
    result.CommandLine = read_cmdline(IntPID);
    result.FDcount = count_fd(IntPID);
    
    return result;
}

void proc_info_destroy(ProcessInfo info) {
    if (info.CommandLine != NULL) {
        free(info.CommandLine);
    }
}