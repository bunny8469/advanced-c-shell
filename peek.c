#include "headers.h"
#include <pwd.h>
#include <grp.h>
#include <time.h>

int GetFileType(char* DIRNAME, char* curr_path){
    char temp[DEFAULT_CHARS*2];
    strcpy(temp, curr_path);
    strcat(temp, "/");
    strcat(temp, DIRNAME);
    
    struct stat entry_info;

    // stat() also takes relative / absolute paths, same for access()
    if (stat(temp, &entry_info) == 0) {

        if (S_ISDIR(entry_info.st_mode))
            return 0; 
        else if (S_ISREG(entry_info.st_mode)){
            if (access(temp, X_OK) == 0) 
                return 1; 
            else
                return 2;
        }
    }
    return -1;
}

void PrintWithColor(char* DIRNAME, int color){  
    switch(color){
        case 1:{
            printf(GREEN COLOR_BOLD "%s\n" COLOR_RESET, DIRNAME);
            break;
        }
        case 0:{
            printf(BLUE COLOR_BOLD "%s\n" COLOR_RESET, DIRNAME);
            break;
        }
        case -1:{
            printf(RED COLOR_BOLD "%s\n" COLOR_RESET, DIRNAME);
            break;
        }
        default:{
            printf("%s\n", DIRNAME);
            break;
        }
    }
}

int compare_strings(const void *a, const void *b) {
    return strcmp(a, b);
}

void print_permissions(mode_t mode) {
    printf((S_ISDIR(mode)) ? "d" : "-");
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}

void CalcBlocks(char* filename, char* curr_path, int* total_blocks){
    char temp[DEFAULT_CHARS*2];
    strcpy(temp, curr_path);
    strcat(temp, "/");
    strcat(temp, filename);

    struct stat entry_info;
    if (stat(temp, &entry_info) == 0) {
        
        // The st_blocks field in the struct stat structure represents the number of 512-byte blocks allocated to the file on disk
        // but when displaying using "ls -l", it shows using units of 1 kilo byte (1024 bytes) by convention. This is why we divide the number of blocks by 2.

        *total_blocks += ceil(entry_info.st_blocks / 2.0);
    }
}

void PrintDetails(char* filename, char* curr_path){
    char temp[DEFAULT_CHARS*2];
    strcpy(temp, curr_path);
    strcat(temp, "/");
    strcat(temp, filename);

    struct stat entry_info;
    if (stat(temp, &entry_info) == 0) {
        print_permissions(entry_info.st_mode);
        printf("%2lu", entry_info.st_nlink);
       
        // this struct contains user information
        struct passwd *pw = getpwuid(entry_info.st_uid);

        // this struct contains group information (collection of users)
        struct group *gr = getgrgid(entry_info.st_gid);

        // username as in the main.c
        if (pw != NULL) {
            printf(" %s", pw->pw_name);
        } 
        else {
            printf(" %d", entry_info.st_uid);
        }

        // group name
        if (gr != NULL) {
            printf(" %s", gr->gr_name);
        } 
        else {
            printf(" %d", entry_info.st_gid);
        }

        // size of file in bytes
        printf("%5ld", entry_info.st_size);

        // time info (time.h library)
        struct tm *time_info;
        char time_str[20];
        time_info = localtime(&entry_info.st_mtime);

        // String format time
        strftime(time_str, sizeof(time_str), "%b %d %H:%M", time_info);

        // prints the formatted string
        printf(" %s ", time_str);

    }
}

void SortedPrint(int file_count, char filenames[file_count][DEFAULT_CHARS], char* curr_path, int long_info){
    // sorting the strings
    
    qsort(filenames, file_count, sizeof(filenames[0]), compare_strings);
    int total_blocks = 0;
    for(int i = 0; i < file_count; i++){
        if(long_info)
            CalcBlocks(filenames[i], curr_path, &total_blocks);
    }
    if(long_info)
        printf("total %d\n", total_blocks);
    for(int i = 0; i < file_count; i++){
        if(long_info){
            PrintDetails(filenames[i], curr_path);
        }
        PrintWithColor(filenames[i], GetFileType(filenames[i], curr_path));
    }
}

void peek(char* token, char delimiters[], PATH_INFO* path){
    char all = 0;
    char long_info = 0;
    char arguments = 0;
    while (token != NULL) {
        token = strtok(NULL, delimiters);
        
        if(token != NULL){
            if(arguments == 1){
                // if already path argument is executed
                
                printf("Shell: %s: too many arguments\n", token);
                break;
            }
            if(strcmp(token, "-a") == 0){
                all = 1;
            }
            else if(strcmp(token, "-l") == 0){
                long_info = 1;
            }
            else if(strcmp(token, "-al") == 0 || strcmp(token, "-la") == 0){
                all = 1; long_info = 1;
            }
            else{
                char curr_path[DEFAULT_CHARS];

                if(strcmp(token, "~") == 0)
                    strcpy(curr_path, path->HOME_DIR);
                else if(strcmp(token, "-") == 0)
                    strcpy(curr_path, path->PREV_DIR);
                else
                    strcpy(curr_path, token);

                DIR *dir;
                struct dirent *entry;

                dir = opendir(curr_path);
                if (dir == NULL) {
                    printf("Shell: %s: %s\n", token, strerror(errno));
                }
                else{
                    
                    // temp directory for counting files in it.
                    DIR* temp_dir = opendir(curr_path);

                    // counting the files to store them in an array
                    int file_count = 0;
                    while ((entry = readdir(temp_dir)) != NULL) {
                        if(all || entry->d_name[0] != '.'){
                            file_count++;
                        }
                    }
                    
                    if(!long_info)
                        printf("%d files.\n", file_count);

                    // storing the files in an array, to sort them and print accordingly
                    char filenames[file_count][DEFAULT_CHARS];
                    int file_no = 0;
                    int total_blocks = 0;
                    while ((entry = readdir(dir)) != NULL) {

                        // get hidden files only when -a flag is used
                        if(all || entry->d_name[0] != '.'){
                            strcpy(filenames[file_no++], entry->d_name);
                        }
                    }
                    SortedPrint(file_count, filenames, curr_path, long_info);
                    arguments = 1;
                    closedir(temp_dir);
                }
                closedir(dir);
            }
        }
    }
}