#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void print_process_states(char status) {
    switch (status) {
        case 'R':
            printf("Running\n");
            break;
        case 'S':
            printf("Sleeping in an interruptible wait\n");
            break;
        case 'Z':
            printf("Zombie\n");
            break;
        default:
            printf("Unknown\n");
            break;
    }
}

void print_proclore_info(int pid) {
    char proc_path[128];
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/status", pid);

    FILE *fp = fopen(proc_path, "r");
    if (fp == NULL) {
        perror("Error opening proc file");
        exit(EXIT_FAILURE);
    }

    char line[128];
    char status;
    int pgrp, vm_size;
    char exec_path[128];
    
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "State:", 6) == 0) {
            sscanf(line, "State:\t%c", &status);
        } else if (strncmp(line, "VmSize:", 7) == 0) {
            sscanf(line, "VmSize:\t%d", &vm_size);
        } else if (strncmp(line, "PPid:", 5) == 0) {
            sscanf(line, "PPid:\t%d", &pgrp);
        }
    }

    fclose(fp);

    printf("pid : %d\n", pid);
    printf("process status : %c", status);
    if (status == 'R' || status == 'S') {
        printf("+");
    }
    printf("\n");
    printf("Process Group : %d\n", pgrp);
    printf("Virtual memory : %d\n", vm_size);

    // Note: This part needs to be adapted based on the process's actual executable path
    strcpy(exec_path, "/path/to/executable");

    printf("executable path : %s\n", exec_path);

    print_process_states(status);
}

int main(int argc, char *argv[]) {
    int pid = getpid();

    if (argc > 1) {
        pid = atoi(argv[1]);
    }

    print_proclore_info(pid);

    return 0;
}
