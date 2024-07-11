#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <math.h> 

#include "structs.h"
#include "commands.h"

#define BLUE "\x1b[34m"
#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define COLOR_RESET "\x1b[0m"
#define COLOR_BOLD  "\e[1m"
#define COLOR_OFF   "\e[m"

#endif