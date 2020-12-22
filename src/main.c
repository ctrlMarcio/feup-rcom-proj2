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

#include "connection.h"


int main(int argc, char** argv) {
    ftp_info info;
    info.port=SERVER_PORT;

    if (argc != 2 || !parseURL(argv[1], &info)) {
        printf("Usage:\tdownload ftp://[<user>:<password>@]<host>/<url-path>\n");
        exit(ARGS_ERROR);
    }

    int fd = connectServer(info);

    //welcome message
    char reply[ARRAY_SIZE];  
    if (readReply(fd, reply) < 0) {
        printf("Error reading welcome message.\n");
        return (READ_WELCOME_ERROR);
    }

    printf("< %s\n", reply);

    //login
    int error;
    if ((error = login(fd, &info)) > 0) return error;

    //passive mode
    if(sendString("pasv", fd)<0){
        printf("Error sending passive command.\n");
        return(SEND_PASV_ERROR);
    }
    memset(reply, 0, sizeof reply);
    if(readReply(fd, reply) < 0){
        printf("Error reading reply to passive command.\n");
        return(READ_PASV_ERROR);
    }
    printf("< %s\n", reply);

    info.port = calculatePort(reply);
    if(info.port < 0){
        printf("Error calculating port.\n");
        return(CALCULATE_PORT_ERROR);
    }

    int fd2 = connectServer(info);

    //retr
    char retrCmd[COMMAND_SIZE];
    sprintf(retrCmd, "retr %s", info.path);
    if(sendString(retrCmd, fd) < 0){
        printf("Error sending 'retr' command.\n");
        return(SEND_RETR_ERROR);
    }
    memset(reply, 0, sizeof reply);
    if(readReply(fd, reply) < 0){
        printf("Error reading reply to 'retr' command.\n");
        return(READ_RETR_ERROR);
    }
    printf("< %s\n", reply);

    downloadFile(fd2,info.path);

    if(closeConnection(fd) < 0){
        printf("Error closing connection to socket.\n");
        return(CLOSE_CONNECTION_ERROR);
    }
    if(closeConnection(fd2) < 0){
        printf("Error closing connection to socket.\n");
        return(CLOSE_CONNECTION_ERROR);
    }
}