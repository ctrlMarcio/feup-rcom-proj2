#include "connection.h"

int sendString(char* string, int fd) {
    char res[ARRAY_SIZE];
    sprintf(res, "%s\n", string);
    int bytes = write(fd, res, strlen(res));

    printf("> %s\n", string);

    return bytes;
}

int readReply(int fd, char* buf) {
    char c;
    int bytes = 0;
    bool lastLine = false;
    while (read(fd, &c, 1) >= 0) {
        buf[bytes] = c;
        if ((buf[bytes - 4] == '\n' || bytes < 4) && isdigit(buf[bytes - 3]) && isdigit(buf[bytes - 2]) && isdigit(buf[bytes - 1]) && buf[bytes] != '-') lastLine = true;
        if (lastLine && c == '\n') break;

        bytes++;
    }

    return bytes;
}

int login(int fd, ftp_info* info) {
    char user_cmd[COMMAND_SIZE], pass_cmd[COMMAND_SIZE];
    char server_reply[ARRAY_SIZE];

    // username
    sprintf(user_cmd, "USER %s",info->user);
    if(sendString(user_cmd, fd) < 0){
        printf("Error sending user command.");
        return (SEND_LOGIN_USER_ERROR);
    }

    if (readReply(fd, server_reply) < 0) {
        printf("Error reading reply when logging in: user command.");
        return (READ_LOGIN_USER_ERROR);
    }

    printf("< %s\n", server_reply);

    memset(server_reply, 0, sizeof server_reply);

    // password
    sprintf(pass_cmd, "PASS %s", info->password);
    if(sendString(pass_cmd, fd) < 0){
        printf("Error sending pass command.");
        return (SEND_LOGIN_PASS_ERROR);
    }

    if (readReply(fd, server_reply) < 0) {
        printf("Error reading reply when logging in: pass command");
        return (READ_LOGIN_PASS_ERROR);
    }

    printf("< %s\n", server_reply);

    memset(server_reply, 0, sizeof server_reply);

    return 0;
}

int parseURL(char* url, ftp_info* info) {
    char res[ARRAY_SIZE];

    if(!substring(url, 0, 6, res)){
        printf("Error parsing URL: wrong URL format.");
        return INVALID_URL_ERROR;
    }  
    
    if (strcmp(res, URL_BEGINNING))
        return INVALID_URL_ERROR;


    //check if user & password are provided
    memset(res, 0, sizeof res);

    substring(url, 6, strlen(url)-7, res);

    type type;
    if(strchr(res,':') != NULL) //user & password provided
        type = USER;
    else {
        type = HOST;
        strcpy(info->user, DEFAULT_USER);
        strcpy(info->password, DEFAULT_PASS);
    }

    unsigned j = 0;
    unsigned len = strlen(url);

    //divide and build struct
    for (unsigned i = 6; i < len; ++i) {
        char c = url[i];

        switch(type){
        case USER:
            if (c == ':') {
                info->user[j] = '\0';
                type = PASSWORD;
                j = 0;
            } else {
                info->user[j++] = c;
            }
            break;
        case PASSWORD:
            if (c == '@') {
                info->password[j] = '\0';
                type = HOST;
                j = 0;
            } else {
                info->password[j++] = c;
            }
            break;
        case HOST:
            if (c == '/') {
                info->host[j] = '\0';
                type = PATH;
                j = 0;
            } else {
                info->host[j++] = c;
            }
            break;
        default:
            info->path[j++] = c;
        }
    }

    info->path[j] = '\0';

}

char* getServerAddr(char* host) {
    struct hostent* h;
    if ((h = gethostbyname(host)) == NULL) {
        herror("gethostbyname");
        exit(SERVER_ADDR_ERROR);
    }

    return inet_ntoa(*((struct in_addr*)h->h_addr));
}

int connectServer(ftp_info info) {
    int sockfd;
    struct sockaddr_in server_addr;

    /*server address handling*/
    bzero((char*)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(getServerAddr(info.host)); /*32 bit Internet address network byte ordered*/
    server_addr.sin_port = htons(info.port);                                  /*server TCP port must be network byte ordered */

    /*open an TCP socket*/
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        exit(OPEN_SOCKET_ERROR);
    }

    /*connect to the server*/
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect()");
        exit(CONNECT_SOCKET_ERROR);
    }

    return sockfd;
}

int calculatePort(char * reply){
    int portMSB, portLSB, res;

    if((res = sscanf( reply, "227 Entering Passive Mode (%*d,%*d,%*d,%*d,%d,%d).", &portMSB, &portLSB)) < 2){
        return -1;
    }

    return portMSB * 256 + portLSB;
}

int downloadFile(int fd, char* path){ 
    char filename[ARRAY_SIZE];    
    get_string_after(path, '/', filename);

    FILE *file;
    file = fopen(filename, "w+");
    
    char buffer[ARRAY_SIZE];
    int ret;
    while((ret = read(fd, buffer, ARRAY_SIZE)) > 0){
        fwrite(buffer, sizeof(char), ret, file);
    }

    fclose(file);
    
    return 0;
}

int closeConnection(int fd) {
    return close(fd);
}
