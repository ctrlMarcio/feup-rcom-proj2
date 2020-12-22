#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>


#include "error.h"
#include "util.h"

/**
 * @brief Send string to file descriptor
 * 
 * @param string String to be sent
 * @param fd File descriptor where string should be sent
 * @return int number of bytes sent
 */
int sendString(char* string, int fd);

/**
 * @brief Reads reply information in file descriptor to buffer
 * 
 * @param fd File descriptor where information is to be read
 * @param buf Buffer to contain read reply
 * @return int number of byter read
 */
int readReply(int fd, char* buf);

/**
 * @brief Performs the login operation in FTP server
 * 
 * @param fd File descriptor used to recieve and send information to FTP server
 * @param info Structure containing login credentials
 * @return int 0 if success, error code otherwise
 */
int login(int fd, ftp_info* info);

/**
 * @brief Parses the URL and stores the information in info
 * 
 * @param url URL provided as an argument to the application
 * @param info Struct to store URL information
 * @return int 
 */
int parseURL(char* url, ftp_info* info);

/**
 * @brief Get Server Address 
 * 
 * @param host Host name
 * @return char* server address 
 */
char* getServerAddr(char* host);

/**
 * @brief Performs the connection to the FTP server
 * 
 * @param info Structure containing server information
 * @return int Socket file descriptor
 */
int connectServer(ftp_info info);

/**
 * @brief Calculates server port for file download
 * 
 * @param reply String containing port information
 * @return int Calculated port number
 */
int calculatePort(char * reply);

/**
 * @brief Performs the file download from FTP server
 * 
 * @param fd File descriptor to read information
 * @param path Path to file in FTP server
 * @return int 0 if success
 */
int downloadFile(int fd, char* path);

/**
 * @brief Closes connection to the FTP server
 * 
 * @param fd File descriptor to FTP server
 * @return int 0 if success
 */
int closeConnection(int fd);