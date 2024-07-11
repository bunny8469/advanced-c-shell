#include "headers.h"
#include "queue.h"
#include <sys/wait.h>

void background_function() {
    printf("Background function started\n");
    sleep(5); // Simulate some work
    printf("Background function finished\n");
}

void bgprocess(char* string){
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork error");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        background_function();
    } else {
        // Parent process
        printf("Parent process continues\n");
    }


}

int execute_command(char* input, char* delimiters, PATH_INFO* path, Queue* que, char* compiled_command, char* bin_file){                        
    

    int should_store = 0;

    char* outPtr;
    char* token = strtok_r(input, delimiters, &outPtr);

    if(strcmp(que->items[que->rear], token) != 0){
        should_store = 1;
    }

    while(token != NULL){   

        if(strstr(token, "pastevents") == NULL){
            strcat(compiled_command, token);
        }

        char* delimiters_command = " /\n";
        
        char* token_command;

        int exec_past = 0;

        // tokenizing command
        char* copy_token = strdup(token);
        token_command = strtok(token, delimiters_command);
        
        if(strcmp(token_command, "warp") == 0){
            char temp[DEFAULT_CHARS];
            strcpy(temp, path->CURR_DIR);

            warp(token_command, delimiters_command, path);

            if(strcmp(temp, path->CURR_DIR) != 0){
                strcpy(path->PREV_DIR, temp);
            }
            printf("%s\n", path->CURR_DIR);
        }
        else if(strcmp(token_command, "peek") == 0){
            delimiters_command = " \n";
            peek(token_command, delimiters_command, path);
        }
        else if(strcmp(token_command, "pastevents") == 0){
            should_store = 0;
            exec_past = pastevents(token_command, delimiters_command, que, path, compiled_command, bin_file);
            if(exec_past) should_store = 1;
        }
        else if(strcmp(token_command, "seek") == 0){
            seek(token_command, delimiters_command, path);
        }
        else if(strcmp(token_command, "\n") != 0){
            pid_t pid = fork(); 

            if (pid == 0) {
                char *program = strdup(token_command);  // Program to execute
                char* args[] = {program, "5", NULL};
                char* ptr;

                args[0] = strdup(program);
                int n = 1;
                token_command = strtok(NULL, delimiters_command);
                while(token_command != NULL){
                    args[n++] = token_command;
                    token_command = strtok(NULL, delimiters_command);
                }
                args[n] = NULL;

                execvp(program, args); // Execute the program   
                
                // If execvp fails, print an error
                perror("execvp");
            } else if (pid > 0) {
                // This is the parent process
                wait(NULL); // Wait for the child process to finish
                // printf("Child process executed.\n");
            } else {
                // Forking failed
                perror("fork");
            }

        }
        token = strtok_r(NULL, delimiters, &outPtr);
        if(token == NULL || strcmp(token, "\n") == 0){
            break;
        }
        char* delim_used = ";";
        if(token != NULL){
            strcat(compiled_command, delim_used);
            // strcat(compiled_command, token);
        }
    }
    return should_store;
}

int main()
{
    // Keep accepting commands

    // Get home directory path
    PATH_INFO* path = (PATH_INFO*) malloc(sizeof(struct PATH_INFO));
    path->HOME_DIR = (char*) malloc(sizeof(char) * DEFAULT_CHARS);
    getcwd(path->HOME_DIR, sizeof(char) * DEFAULT_CHARS);
    printf("Current directory is set to %s\n", path->HOME_DIR);
    
    // Current directory
    path->CURR_DIR = (char*) malloc(sizeof(char) * DEFAULT_CHARS);
    strcpy(path->CURR_DIR, path->HOME_DIR);

    // previous directory
    path->PREV_DIR = (char*) malloc(sizeof(char) * DEFAULT_CHARS);
    strcpy(path->PREV_DIR, path->CURR_DIR);

    char bin_file[DEFAULT_CHARS];
    strcpy(bin_file, path->HOME_DIR);

    // initializing queue
    Queue* que = (Queue*) malloc(sizeof(struct Queue)); 
    initQueue(que);
    strcat(bin_file, "/pastevents.bin");
    char status = loadQueueFromFile(que, bin_file);

    while (1)
    {
        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt(path);
        char input[4096];
        fgets(input, 4096, stdin);
        
        char str[strlen(input)];
        strcpy(str, input);

        char* delimiters = ";";
        char compiled_command[DEFAULT_CHARS];
        strcpy(compiled_command,"");
        int should_store = execute_command(input, delimiters, path, que, compiled_command, bin_file);
        if(should_store){
            if(isFull(que)){
                char str[DEFAULT_CHARS];
                dequeue(que, str);
            }
            enqueue(que, compiled_command);
            saveQueueToFile(que, bin_file);
        }
    }
}
