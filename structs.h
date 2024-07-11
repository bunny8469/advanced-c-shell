#ifndef __STRUCTS_H_
#define __STRUCTS_H_

struct PATH_INFO{
    char* HOME_DIR;
    char* CURR_DIR;
    char* PREV_DIR;
};
typedef struct PATH_INFO PATH_INFO;

#define MAX_SIZE 15
#define DEFAULT_CHARS 512

typedef struct Queue{
    char items[MAX_SIZE][DEFAULT_CHARS];
    int filled;
    int front, rear;
} Queue;

#endif
