#ifndef REQUESTS_H  
#define REQUESTS_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "helper.h"
#include "parson.h"

// creates a GET request ready to be sent to the server
char *compute_get_request(char *host, char *url, char *query_params, 
                            char **cookies, int cookies_count, char *auth_token);

// creates a POST request ready to be sent to the server
char *compute_post_request(char *host, char *url, char* content_type, char **body_data,
                            int body_data_fields_count, char **cookies, 
                            int cookies_count, const char *auth_token);

// creates a DELETE request ready to be sent to the server
char *compute_delete_request(char *host, char *url, char *query_params, 
                             char **cookies, int cookies_count, const char *auth_token);

// creates a PUT request ready to be sent to the server
char *compute_put_request(char *host, char *url, char* content_type, char **body_data,
                            int body_data_fields_count, char **cookies, 
                            int cookies_count, const char *auth_token);

// sends a JSON object to the server, extracts the new cookies and shows a SUCCESS/ERROR message                           
void post_json(int sockfd, char *url, char *json_string, char ***cookies, int *cookies_count, 
                const char *auth_token, char *success_code, char *success_text);
          
void login_admin(int sockfd, const char *username, const char *password, 
                    char ***cookies, int *cookies_count);

void add_user(int sockfd, const char *username, const char *password, 
                    char ***cookies, int *cookies_count, const char *auth_token);

void get_users(int sockfd, char ***cookies, int *cookies_count);

void logout_admin(int sockfd, char ***cookies, int *cookies_count, int *success);

void login(int sockfd, const char *admin_username, const char *username,
            const char *password, char ***cookies, int *cookies_count);

void logout(int sockfd, char ***cookies, int *cookies_count, int *success);

void delete_user(int sockfd, const char *username, char ***cookies, 
                 int *cookies_count, const char *auth_token);

void get_access(int sockfd, char ***cookies, int *cookies_count);

void get_movies(int sockfd, char ***cookies, int *cookies_count, char *access_token);

void get_movie(int sockfd, char *id, char ***cookies, int *cookies_count, char *access_token);

void add_movie(int sockfd, char *title, char *year, char *description, char *rating,
                char ***cookies, int *cookies_count, char *access_token);

void delete_movie(int sockfd, char *id, char ***cookies, int *cookies_count, char *access_token);

void update_movie(int sockfd, char *id, char *title, char *year, char *description, char *rating,
                    char ***cookies, int *cookies_count, char *access_token);

void get_collections(int sockfd, char ***cookies, int *cookies_count, char *auth_token);

void get_collection(int sockfd, char *id, char ***cookies, int *cookies_count, char *access_token);

void add_collection(int sockfd, char *title, char *owner, int *movies, int num_movies,
                    char ***cookies, int *cookies_count, char *access_token);

void delete_collection(int sockfd,char *id, char ***cookies, 
                        int *cookies_count, char *access_cookie);

void add_movie_to_collection(int sockfd, int collection_id, int movie_id,
                        char ***cookies, int *cookies_count, char *access_token);

void delete_movie_from_collection(int sockfd, int collection_id, int movie_id,
                        char ***cookies, int *cookies_count, char *access_token);

#endif