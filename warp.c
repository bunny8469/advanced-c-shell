#include "headers.h"
#include "commands.h"

char* getRelativePath(PATH_INFO* path){
    // Prints Absolute or Relative Path accordingly.
   
    char* relative = (char*) malloc(sizeof(char) * DEFAULT_CHARS);
    if(strncmp(path->HOME_DIR, path->CURR_DIR, strlen(path->HOME_DIR)) == 0){
    
        // relative if common path upto home directory
        strcpy(relative, &(path->CURR_DIR[strlen(path->HOME_DIR)]));
    }
    else{

        // otherwise, absolute path
        strcpy(relative, path->CURR_DIR);
    }
    return relative;
}

void warp(char* token, char delimiters[], PATH_INFO* path){
    while (token != NULL) {
        token = strtok(NULL, delimiters);
        if(token != NULL){

            if(strcmp("-", token) == 0){
                if(chdir(path->PREV_DIR) != 0){
                    printf("Shell: %s: %s\n", path->PREV_DIR, strerror(errno));
                }
                else{

                    // swapping previous and current directories
                    char temp[DEFAULT_CHARS];
                    strcpy(temp, path->PREV_DIR);
                    strcpy(path->PREV_DIR, path->CURR_DIR);
                    strcpy(path->CURR_DIR, temp);
                }
                continue;
            }
            else if(strcmp("~", token) == 0){
                if(chdir(path->HOME_DIR) != 0){
                    printf("Shell: %s: %s\n", path->HOME_DIR, strerror(errno));
                }
                else{
                    // pushing home directory path to current directory
                    strcpy(path->CURR_DIR, path->HOME_DIR);
                }
                continue;
            }
            
            if(chdir(token) == 0){
                // updating the current directory path
                getcwd(path->CURR_DIR, sizeof(char) * DEFAULT_CHARS);
            }
            else{
                printf("Shell: %s: %s\n", token, strerror(errno));
            }
        }
    }
    return ;
}