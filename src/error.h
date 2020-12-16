
/**
 * @brief Defines the error code for errors related to the arguments
 * 
 */
#define ARGS_ERROR 1

/**
 * @brief Defines the error code for errors related to invalid URL
 * 
 */
#define INVALID_URL_ERROR 2

/**
 * @brief Defines the error called when the server address can't be found by name
 * 
 */
#define SERVER_ADDR_ERROR 3

/**
 * @brief Defines the error code for errors related to the opening of the TCP socket
 * 
 */
#define OPEN_SOCKET_ERROR 4

/**
 * @brief Defines the error code for errors related to the connection of the TCP socket to the server
 * 
 */
#define CONNECT_SOCKET_ERROR 5

/**
 * @brief Defines the error code for errors related to reading the FTP server's welcome message
 * 
 */
#define READ_WELCOME_ERROR 6

/**
 * @brief Defines the error code for errors related to sending the user to the FTP server when logging in
 * 
 */
#define SEND_LOGIN_USER_ERROR 7

/**
 * @brief Defines the error code for errors related to reading the server's reply after sending the user
 * 
 */
#define READ_LOGIN_USER_ERROR 8

/**
 * @brief Defines the error code for errors related to sending the password to the FTP server when logging in
 * 
 */
#define SEND_LOGIN_PASS_ERROR 9

/**
 * @brief Defines the error code for errors related to reading the server's reply after sending the password
 * 
 */
#define READ_LOGIN_PASS_ERROR 10

/**
 * @brief Defines the error code for errors related to sending the passive command
 * 
 */
#define SEND_PASV_ERROR 11

/**
 * @brief Defines the error code for errors related to reading the server's reply after sending the 'pasv' command
 * 
 */
#define READ_PASV_ERROR 12

/**
 * @brief Defines the error code for errors related to calculating the server's port
 * 
 */
#define CALCULATE_PORT_ERROR 13

/**
 * @brief Defines the error code for errors related to sending the 'retr' command
 * 
 */
#define SEND_RETR_ERROR 14

/**
 * @brief Defines the error code for errors related to reading the server's reply after sending the 'retr' command
 * 
 */
#define READ_RETR_ERROR 15

/**
 * @brief Defines the error code for errors related to closing connections to the sockets
 * 
 */
#define CLOSE_CONNECTION_ERROR 16