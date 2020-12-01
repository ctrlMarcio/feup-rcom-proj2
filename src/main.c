#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "error.h"
#include "util.h"

int send_string(char* string, int fd) {
    char res[1024];
    sprintf(res, "%s\n", string);
    int bytes = write(fd, res, strlen(res));

    // if (bytes >= 0)
    //     printf("Written bytes %d\n", bytes);

    printf("> %s\n", string);

    return bytes;
}

int read_reply(int fd, char* buf) {
    char c;
    int bytes = 0;
    bool lastLine = false;
    while (read(fd, &c, 1) >= 0) {
        buf[bytes] = c;
        // printf("%c",c);      //TEST: read char by char and print
        if ((buf[bytes - 4] == '\n' || bytes < 4) && isdigit(buf[bytes - 3]) && isdigit(buf[bytes - 2]) && isdigit(buf[bytes - 1]) && buf[bytes] != '-') lastLine = true;
        if (lastLine && c == '\n') break;

        bytes++;
    }

    return bytes;
}

int login(int fd, ftp_info* info) {
    char user_cmd[2048], pass_cmd[2048];
    char server_reply[1024];

    // username
    sprintf(user_cmd, "USER %s",info->user);
    send_string(user_cmd, fd);

    if (read_reply(fd, server_reply) < 0) {
        printf("Error reading reply when logging in.");
        return (8);  // TODO: macro ihih
    }

    printf("< %s\n", server_reply);  //TEST

    memset(server_reply, 0, sizeof server_reply);

    // password
    sprintf(pass_cmd, "PASS %s", info->password);
    send_string(pass_cmd, fd);

    if (read_reply(fd, server_reply) < 0) {
        printf("Error reading reply when logging in.");
        return (8);  // TODO: macro ihih
    }

    printf("< %s\n", server_reply);  //TEST

    return 0;
}

int parseURL(char* url, ftp_info* info) {
    //confirm ftp beginning
    char res[1024];             // TODO macro
    substring(url, 0, 6, res);  // TODO check return

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
            } else {
                info->user[j++] = c;
            }
        } else if (type == PASSWORD) {
            if (c == '@') {
                info->password[j] = '\0';
                type = HOST;
                j = 0;
            } else {
                info->password[j++] = c;
            }
        } else if (type == HOST) {
            if (c == '/') {
                info->host[j] = '\0';
                type = PATH;
                j = 0;
            } else {
                info->host[j++] = c;
            }
        } else {
            info->path[j++] = c;
        }
    }

    info->path[j] = '\0';
}

char* getServerAddr(char* host) {
    struct hostent* h;
    if ((h = gethostbyname(host)) == NULL) {
        herror("gethostbyname");
        exit(3);  // TODO add macro error
    }

    return inet_ntoa(*((struct in_addr*)h->h_addr));
}

int connect_server(ftp_info info) {
    int sockfd;
    struct sockaddr_in server_addr;

    /*server address handling*/
    bzero((char*)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(getServerAddr(info.host)); /*32 bit Internet address network byte ordered*/
    server_addr.sin_port = htons(info.port);                                  /*server TCP port must be network byte ordered */
    // TODO 21 bad MACRO good

    /*open an TCP socket*/
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        exit(4);  // TODO macro
    }

    /*connect to the server*/
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect()");
        exit(5);  // TODO macro ihih
    }

    return sockfd;
}

int calculate_port(char * reply){
    int portMSB, portLSB;

    sscanf( reply, "227 Entering Passive Mode (%*d,%*d,%*d,%*d,%d,%d).", &portMSB, &portLSB );

    return portMSB * 256 + portLSB;
}

int download_file(int fd, char* path){
    char filename[1024];    // TODO: macro
    get_string_after(path, '/', filename);

    FILE *file;
    file = fopen(filename, "w+");
    
    char buffer[1024];
    int ret;
    while((ret = read(fd, buffer, 1024)) > 0){
        fwrite(buffer, sizeof(char), ret, file);
    }

    fclose(file);
    
    return 0;
}

int close_connection(int fd) {
    return close(fd);
}

int main(int argc, char** argv) {
    ftp_info info;
    info.port=21;       // TODO: 21 bad Macro good
    if (argc != 2 || !parseURL(argv[1], &info)) {
        printf("Usage:\tdownload ftp://[<user>:<password>@]<host>/<url-path>\n");
        exit(ARGS_ERROR);
    }

    int fd = connect_server(info);

    //welcome message
    char reply[1024];  // FIXME: this just a random number... ...
    if (read_reply(fd, reply) < 0) {
        printf("Error reading reply.");
        return (6);  // TODO: macro ihih
    }

    printf("< %s\n", reply);

    //login
    int error;
    if ((error = login(fd, &info)) > 0) return error;

    //passive mode
    send_string("pasv", fd);
    memset(reply, 0, sizeof reply);
    if(read_reply(fd, reply) < 0){
        printf("Error reading reply to passive_command.");
        return(7); // TODO: macro ihih
    }
    printf("< %s\n", reply);


    info.port = calculate_port(reply);

    int fd2 = connect_server(info);

    //retr
    char retrCmd[2048];
    sprintf(retrCmd, "retr %s", info.path);
    send_string(retrCmd, fd);

    memset(reply, 0, sizeof reply);
    if(read_reply(fd, reply) < 0){
        printf("Error reading reply to passive_command.");
        return(7); // TODO: macro ihih
    }
    printf("< %s\n", reply);

    download_file(fd2,info.path);

    close_connection(fd);
    close_connection(fd2);
}