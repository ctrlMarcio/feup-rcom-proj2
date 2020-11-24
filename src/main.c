#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 

#include "error.h"
#include "util.h"

int parseURL(char* url, ftp_info* info) {
    //confirm ftp beginning
    char res[1024]; // TODO macro
    substring(url, 0, 6, res); // TODO check return

    if (strcmp(res, "ftp://"))
        return INVALID_URL_ERROR;

    //divide and build struct
    type type = USER;
    unsigned j = 0;
    unsigned len = string_len(url);

    for (unsigned i = 6; i < len; ++i) {
        char c = url[i];
        if (c == ':') {
            info->user[j] = '\0';
            type = PASSWORD;
            j = 0;
        }
        else if (c == '@') {
            info->password[j] = '\0';
            type = HOST;
            j = 0;
        }
        else if (c == '/') {
            info->host[j] = '\0';
            type = PATH;
            j = 0;
        }
        else {
            if (type == USER)
                info->user[j++] = c;
            else if (type == PASSWORD)
                info->password[j++] = c;
            else if (type == HOST)
                info->host[j++] = c;
            else {
                info->path[j++] = c;
            }
        }
    }

    info->path[j] = '\0';
}

int main(int argc, char** argv) {
    ftp_info info;
    if (argc != 2 || !parseURL(argv[1], &info)) {
        printf("Usage:\tdownload ftp://[<user>:<password>@]<host>/<url-path>\n");
        exit(ARGS_ERROR);
    }

    printf("%s\n", info.user);
    printf("%s\n", info.password);
    printf("%s\n", info.host);
    printf("%s\n", info.path);
}