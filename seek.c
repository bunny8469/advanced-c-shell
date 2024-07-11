#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include "headers.h"

#define RESET_COLOR "\033[0m"
#define GREEN_COLOR "\033[32m"
#define BLUE_COLOR "\033[34m"

// ... Other functions and definitions ...

void seek_recursive(const char *target, const char *dir_path, int is_directory, int is_file, int is_execute) {
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        perror("Directory open error");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char full_path[256];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

        struct stat entry_info;
        if (stat(full_path, &entry_info) == -1) {
            perror("stat error");
            continue;
        }

        int is_target_directory = S_ISDIR(entry_info.st_mode);
        int is_target_file = S_ISREG(entry_info.st_mode); 

        if (is_directory && !is_target_directory) {
            continue;
        }
        if (is_file && !is_target_file) {
            continue;
        }

        if (strcmp(target, entry->d_name) == 0) {
            char color_code[10];
            if (is_target_directory) {
                strcpy(color_code, BLUE_COLOR);
            } else {
                strcpy(color_code, GREEN_COLOR);
            }
            printf("%s%s/%s%s\n", color_code, dir_path, entry->d_name, RESET_COLOR);

            if (is_execute && !is_target_directory) {
                FILE *file = fopen(full_path, "r");
                if (file == NULL) {
                    perror("File open error");
                    continue;
                }

                char line[256];
                while (fgets(line, sizeof(line), file)) {
                    printf("%s", line);
                }

                fclose(file);
            }
        }

        if (is_target_directory) {
            seek_recursive(target, full_path, is_directory, is_file, is_execute);
        }
    }

    closedir(dir);
}

void seek(char *token, char *delimiters, PATH_INFO *path) {
    int is_directory = 0;
    int is_file = 0;
    int is_execute = 0;

    while (token != NULL) {
        if (strcmp(token, "-d") == 0) {
            is_directory = 1;
        } else if (strcmp(token, "-f") == 0) {
            is_file = 1;
        } else if (strcmp(token, "-e") == 0) {
            is_execute = 1;
        } else {
            seek_recursive(token, path->CURR_DIR, is_directory, is_file, is_execute);
        }
        token = strtok(NULL, delimiters);
    }
}
