#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define HOST "63.32.125.183"
#define PORT 8081

#define BUFFER_SIZE 4096
#define LINE_SIZE 1000
#define MAX_COMMAND_LEN 100
#define MAX_COOKIES 50
#define COOKIE_LEN 300

// adds a line to a string message
void compute_message(char *message, const char *line);

// opens a connection with server host_ip on port portno, returns a socket
int open_connection(char *host_ip, int port, int ip_type, int socket_type, int flag);

// closes a server connection on socket sockfd
void close_connection(int sockfd);

// send a message to a server
void send_to_server(int sockfd, char *message);

// receives and returns the message from a server
char *receive_from_server(int sockfd);

// extracts and returns a JSON from a server response
char *basic_extract_json_response(char *str);

// adds a cookie to a cookie list
void add_cookie(char ***cookies, int *cookie_count, const char *new_cookie);

// removes all the cookies in the list
void remove_cookies(char ***cookies, int *cookies_count);

// extracts the JWT token from a server http response
char *extract_jwt_token(char *response);

// extracts the error from a server http response
char *extract_error(char *response);

#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			perror(call_description);			\
			exit(errno);					\
		}							\
	} while (0)

#endif