#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <errno.h> 
#include <netdb.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <netinet/in.h> 
#include <arpa/inet.h>

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
    unsigned len = strlen(url);

    for (unsigned i = 6; i < len; ++i) {
        char c = url[i];

        // TODO switch case
        if (type == USER) {
            if (c == ':') {
                info->user[j] = '\0';
                type = PASSWORD;
                j = 0;
            }
            else {
                info->user[j++] = c;
            }
        }
        else if (type == PASSWORD) {
            if (c == '@') {
                info->password[j] = '\0';
                type = HOST;
                j = 0;
            }
            else {
                info->password[j++] = c;
            }
        }
        else if (type == HOST) {
            if (c == '/') {
                info->host[j] = '\0';
                type = PATH;
                j = 0;
            }
            else {
                info->host[j++] = c;
            }
        }
        else {
            info->path[j++] = c;
        }
    }

    info->path[j] = '\0';

    printf("%s\n", info->user);
    printf("%s\n", info->password);
    printf("%s\n", info->host);
    printf("%s\n", info->path);
}

char* getServerAddr(char* host) {
    struct hostent* h;
    if ((h = gethostbyname(host)) == NULL) {
        herror("gethostbyname");
        exit(3); // TODO add macro error
    }

    return inet_ntoa(*((struct in_addr*)h->h_addr));
}

int connectServer(ftp_info info) {
    int	sockfd;
    struct	sockaddr_in server_addr;

    /*server address handling*/
    bzero((char*)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(getServerAddr(info.host));	/*32 bit Internet address network byte ordered*/
    server_addr.sin_port = htons(21);		/*server TCP port must be network byte ordered */
    // TODO 21 bad MACRO good

    /*open an TCP socket*/
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        exit(4); // TODO macro
    }

    /*connect to the server*/
    if (connect(sockfd,
        (struct sockaddr*)&server_addr,
        sizeof(server_addr)) < 0) {
        perror("connect()");
        exit(5); // TODO macro ihih
    }

    return sockfd;
}

int send_string(char* string, int fd) {
    int bytes = write(fd, string, strlen(string));

    if (bytes >= 0)
        printf("Written bytes %d\n", bytes);

    return bytes;
}

int close_connection(int fd) {
    return close(fd);
}

int main(int argc, char** argv) {
    ftp_info info;
    if (argc != 2 || !parseURL(argv[1], &info)) {
        printf("Usage:\tdownload ftp://[<user>:<password>@]<host>/<url-path>\n");
        exit(ARGS_ERROR);
    }

    int fd = connectServer(info);

    send_string("macaco", fd);

    close_connection(fd);
}