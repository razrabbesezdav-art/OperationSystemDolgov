#define _GNU_SOURCE
#include "../include/proc_info.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

static char *read_cmdline(int pid) {
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "/proc/%d/cmdline", pid);

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

    if (bytes_read > 0 && buffer[bytes_read - 1] == ' ') {
        buffer[bytes_read - 1] = '\0';
    }

    return strdup(buffer);
}

static int count_fd(int pid) {
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "/proc/%d/fd", pid);

    DIR *dir = opendir(filepath);
    if (dir == NULL) {
        return -1;
    }

    int fd_count = 0;
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        fd_count++;
    }

    closedir(dir);
    return fd_count;
}

ProcessInfo proc_info_init(void) {
    ProcessInfo result = {0};
    result.PID = 0;
    result.ParentPID = -1;
    result.CommandLine = NULL;
    result.FDcount = 0;
    strcpy(result.Name, "Unknown");
    strcpy(result.State, "Unknown");
    return result;
}

int proc_info_fill(int pid, ProcessInfo *info) {
    if (info == NULL) {
        return -1;
    }

    info->PID = pid;

    char filepath[256];
    snprintf(filepath, sizeof(filepath), "/proc/%d/status", pid);

    FILE *fp = fopen(filepath, "r");
    if (fp == NULL) {
        perror("cannot open /proc/PID/status");
        return -1;
    }

    char line[256];
    int name_found = 0, state_found = 0, ppid_found = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strncmp(line, "Name:", 5) == 0) {
            sscanf(line, "Name: %63s", info->Name);
            name_found = 1;
        }
        else if (strncmp(line, "State:", 6) == 0) {
            sscanf(line, "State: %63[^\n]", info->State);
            state_found = 1;
        }
        else if (strncmp(line, "PPid:", 5) == 0) {
            sscanf(line, "PPid: %d", &info->ParentPID);
            ppid_found = 1;
            break;
        }
    }
    fclose(fp);

    if (!name_found || !state_found || !ppid_found) {
        fprintf(stderr, "Не удалось прочитать обязательные поля из /proc/%d/status\n", pid);
        return -1;
    }

    info->CommandLine = read_cmdline(pid);

    info->FDcount = count_fd(pid);
    if (info->FDcount < 0) {
        perror("cannot open /proc/PID/fd");
        return -1;
    }

    return 0;
}

void proc_info_destroy(ProcessInfo *info) {
    if (info == NULL) {
        return;
    }
    if (info->CommandLine != NULL) {
        free(info->CommandLine);
        info->CommandLine = NULL;
    }
}