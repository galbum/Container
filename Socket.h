#ifndef CONTAINER_SOCKET_H
#define CONTAINER_SOCKET_H

#include <iostream>
#include <cstring>
#include <strings.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFFER_LENGTH 256
#define CLIENT_ARG_NUM 4
#define SERVER_ARG_NUM 3
#define SUCCESS 0
#define MAX_CONNECTIONS 5
#define SERVER_TYPE_ARG "server"
#define CLIENT_TYPE_ARG "client"

#define USAGE_ERR "system error: Usage: ./sockets client <port> <terminal_command_to_run>.\nServer Usage: ./sockets server <port>\n"
#define WRITE_ERR "system error: Writing data failed.\n"
#define CONNECT_ERR "system error: Connecting to socket failed.\n"
#define SOCKET_ERR "system error: Failed to create socket.\n"
#define HOSTNAME_ERR "system error: Failed to obtain hostname.\n"
#define BIND_ERR "system error: Failed to bind socket.\n"
#define ACCEPT_ERR "system error: Failed to accept connection.\n"
#define LISTEN_ERR "system error: Failed to listen.\n"
#define SOCKNAME_ERR "system error: Failed to get socketname.\n"
#define RUN_CMD_ERR "system error: Failed to run client's command.\n"

/**
 * called if a system call was resulted in an error. Prints a relevant error msg
 * from errno and exits with EXIT_FAILURE.
 * @param errorMessage the msg to print.
 */
void errorHandler(const char* errorMessage);

/**
 * establishes a connection to a server
 * @param port The port to connect to
 * @return The connected socket
 */
int establish(unsigned short port);

/**
 * reads data from a socket
 * @param hostname Hostname of the servera
 * @param port  The port to connect to
 * @return  The connected socket
 */
int call_socket(char *hostname, unsigned short port);

/**
 * reads data from a socket
 * @param sock  The socket to read from
 * @param buf The buffer to read into
 * @param n Amount of bytes to read
 * @return How many bytes were read
 */
int read_data(int sock, char *buf, int n);

/**
 * writes data to a socket
 * @param sock The socket to write to
 * @param buf The buffer to write from
 * @param n Amount of bytes to write
 * @return How many bytes were written
 */
int write_data(int sock, char* buf, int n);

/**
 * Get's connection if there is one
 * @param sock The socket to get connection from
 * @return File descriptor of the accepted socket
 */
int getConnection(int sock);

/**
 * Runs a command on a server
 * @param port The port to connect to
 * @return doesn't return (keep running continuously)
 */
[[noreturn]] int runServer(unsigned short port);

/**
 * Runs a command on a client
 * @param port The port to connect to
 * @param cmdToRun The command to run
 * @return returns 0 if successful
 */
int runClient(unsigned short port, char* cmdToRun);

#endif //CONTAINER_SOCKET_H
