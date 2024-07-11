#include "headers.h"
#include "queue.h"

int pastevents(char* token, char delimiters[], Queue* que, PATH_INFO* path, char* compiled_command, char* bin_file){
    while(token != NULL){
        token = strtok(NULL, delimiters);

        if(token != NULL){
            if(strcmp(token, "execute") == 0){
                token = strtok(NULL, delimiters);
                if(token != NULL){
                    int num = atoi(token);
                    if(num >= 1 && num <= que->filled){
                        if(!isEmpty(que)){
                            char* command = strdup(que->items[(que->front + num - 1) % MAX_SIZE]);
                            delimiters = ";&";
                            execute_command(command, delimiters, path, que, compiled_command, bin_file);
                            return 1;
                        }
                        else{
                            printf("Shell: pastevents: No events recorded.\n");
                        }
                    }
                    else{
                        printf("Shell: pastevents: index out of range\n");
                    }
                }  
                else{
                    printf("Shell: pastevents: argument missing.\n");
                }
                break;
            }
            else if(strcmp(token, "purge") == 0) {
                FILE* fp = fopen(bin_file, "w");
                fclose(fp);
                initQueue(que);
                break;
            }
        }
        else{
            display(que);
        }
    }
    return 0;
}

