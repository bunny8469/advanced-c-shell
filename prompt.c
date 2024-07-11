#include "headers.h"
#include <pwd.h>

void prompt(PATH_INFO* path) {

    // Get username
    __uid_t uid = geteuid();
    struct passwd* pw = getpwuid(uid);

    // Get system name
    char systemName[64];
    gethostname(systemName, sizeof(systemName));

    // Prompt 
    printf(GREEN COLOR_BOLD "%s@%s:" COLOR_BOLD BLUE "~%s" COLOR_RESET "$ ", pw->pw_name, systemName, getRelativePath(path));
}
