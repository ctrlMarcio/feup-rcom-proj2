#include <stdbool.h> 
#include <stdio.h>
#include <string.h>

#include "info.h"


/**
 * @brief Structure containing ftp connection information
 * 
 */
typedef struct ftp_info {
    char user[ARRAY_SIZE];               
    char password[ARRAY_SIZE];           
    char host[ARRAY_SIZE];               
    char path[ARRAY_SIZE];               
    int port;
} ftp_info;

/**
 * @brief Structure used to parse arguments
 * 
 */
typedef enum type {
    USER, PASSWORD, HOST, PATH
} type;

/**
 * @brief Obtain substring from original string, starting on first with pre-determined length 
 * 
 * @param string Original string
 * @param first Index where substring must begin
 * @param length Length of desired substring
 * @param res Obtained substring
 * @return true On success
 * @return false When length exceeds total remaining size of the String string
 */
bool substring(char* string, unsigned first, unsigned length, char* res);

/**
 * @brief Get the string after delimitor occurence
 * 
 * @param original Original string 
 * @param delimiter Character which marks the beginning of string result
 * @param result Obtained string after delimitor
 */
void get_string_after(char *original, char delimiter, char *result);