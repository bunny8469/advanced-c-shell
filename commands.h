#ifndef __COMMANDS_H
#define __COMMANDS_H

#include "structs.h"

void prompt(PATH_INFO* path);

int execute_command(char* input, char* delimiters, PATH_INFO* path, Queue* que, char* compiled_command,char* bin_file);

void warp(char* token, char delimiters[], PATH_INFO* path);
char* getRelativePath(PATH_INFO* path);

void peek(char* token, char delimiters[], PATH_INFO* path);
int pastevents(char* token, char delimiters[], Queue* que, PATH_INFO* path, char* compiled_command, char* bin_file);

void seek(char* token, char* delimiters, PATH_INFO* path);

#endif