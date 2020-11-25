#include <stdbool.h> 

typedef struct ftp_info {
    char user[1024];       // TODO macro
    char password[1024];       // TODO macro
    char host[1024];               // TODO macro
    char path[1024];               // TODO macro
} ftp_info;

typedef enum type {
    USER, PASSWORD, HOST, PATH
} type;

bool substring(char* string, unsigned first, unsigned length, char* res);
