#include "requests.h"

char *compute_get_request(char *host, char *url, char *query_params, 
                            char **cookies, int cookies_count, char *auth_token) {
    char *message = calloc(BUFFER_SIZE, sizeof(char));
    char *line = calloc(LINE_SIZE, sizeof(char));

    if (query_params != NULL) {
        sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }
    compute_message(message, line);

    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    if (auth_token != NULL) {
        sprintf(line, "Authorization: Bearer %s", auth_token);
        compute_message(message, line);
    }

    if (cookies != NULL && cookies_count > 0) {
       strcpy(line, "Cookie: ");
        for (int i = 0; i < cookies_count; i++) {
            strcat(line, cookies[i]);
            if (i < cookies_count - 1) {
                strcat(line, "; ");
            }
        }
        compute_message(message, line);
    }

    compute_message(message, "");
    return message;
}

char *compute_post_request(char *host, char *url, char* content_type, char **body_data,
                            int body_data_fields_count, char **cookies, 
                            int cookies_count, const char *auth_token) {
    char *message = calloc(BUFFER_SIZE, sizeof(char));
    char *line = calloc(LINE_SIZE, sizeof(char));
    char *body_data_buffer = calloc(BUFFER_SIZE, sizeof(char));

    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    for (int i = 0; i < body_data_fields_count; i++) {
        strcat(body_data_buffer, body_data[i]);
        if (i < body_data_fields_count - 1) {
            strcat(body_data_buffer, "&");
        }
    }

    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);

    sprintf(line, "Content-Length: %lu", strlen(body_data_buffer));
    compute_message(message, line);

    if (auth_token != NULL) {
        sprintf(line, "Authorization: Bearer %s", auth_token);
        compute_message(message, line);
    }

     if (cookies != NULL && cookies_count > 0) {
        strcpy(line, "Cookie: ");
        for (int i = 0; i < cookies_count; i++) {
            strcat(line, cookies[i]);
            if (i < cookies_count - 1) {
                strcat(line, "; ");
            }
        }
        compute_message(message, line);
    }

    compute_message(message, "");

    memset(line, 0, LINE_SIZE);
    compute_message(message, body_data_buffer);

    free(line);
    free(body_data_buffer);
    return message;
}

char *compute_delete_request(char *host, char *url, char *query_params, 
                             char **cookies, int cookies_count, const char *auth_token) {
    char *message = calloc(BUFFER_SIZE, sizeof(char));
    char *line = calloc(LINE_SIZE, sizeof(char));

    if (query_params != NULL) {
        sprintf(line, "DELETE %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "DELETE %s HTTP/1.1", url);
    }
    compute_message(message, line);

    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    if (auth_token != NULL) {
        sprintf(line, "Authorization: Bearer %s", auth_token);
        compute_message(message, line);
    }

    if (cookies != NULL && cookies_count > 0) {
        strcpy(line, "Cookie: ");
        for (int i = 0; i < cookies_count; i++) {
            strcat(line, cookies[i]);
            if (i < cookies_count - 1) {
                strcat(line, "; ");
            }
        }
        compute_message(message, line);
    }

    compute_message(message, "");
    return message;
}

char *compute_put_request(char *host, char *url, char* content_type, char **body_data,
                            int body_data_fields_count, char **cookies, 
                            int cookies_count, const char *auth_token) {
    char *message = calloc(BUFFER_SIZE, sizeof(char));
    char *line = calloc(LINE_SIZE, sizeof(char));
    char *body_data_buffer = calloc(BUFFER_SIZE, sizeof(char));

    sprintf(line, "PUT %s HTTP/1.1", url);
    compute_message(message, line);
    
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    for (int i = 0; i < body_data_fields_count; i++) {
        strcat(body_data_buffer, body_data[i]);
        if (i < body_data_fields_count - 1) {
            strcat(body_data_buffer, "&");
        }
    }

    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);

    sprintf(line, "Content-Length: %lu", strlen(body_data_buffer));
    compute_message(message, line);

    if (auth_token != NULL) {
        sprintf(line, "Authorization: Bearer %s", auth_token);
        compute_message(message, line);
    }

     if (cookies != NULL && cookies_count > 0) {
        strcpy(line, "Cookie: ");
        for (int i = 0; i < cookies_count; i++) {
            strcat(line, cookies[i]);
            if (i < cookies_count - 1) {
                strcat(line, "; ");
            }
        }
        compute_message(message, line);
    }

    compute_message(message, "");

    memset(line, 0, LINE_SIZE);
    compute_message(message, body_data_buffer);

    free(line);
    free(body_data_buffer);
    return message;
}

void post_json(int sockfd, char *url, char *json_string, char ***cookies, int *cookies_count, 
                const char *auth_token, char *success_code, char *success_text) {
    char *body_data[1];
    body_data[0] = json_string;

    char *request = compute_post_request(HOST, url, "application/json",
                    body_data, 1, *cookies, *cookies_count, auth_token);

    send_to_server(sockfd, request);
    char *response = receive_from_server(sockfd);
    if (strstr(response, success_code) != NULL) {
        printf("SUCCESS: %s\n", success_text);
        char *set_cookie = strstr(response, "Set-Cookie: ");
        if (set_cookie) {
            set_cookie += strlen("Set-Cookie: ");
            char *end = strchr(set_cookie, ';');
            if (end) {
                size_t len = end - set_cookie;
                char *cookie = calloc(len + 1, sizeof(char));
                strncpy(cookie, set_cookie, len);
                add_cookie(cookies, cookies_count, cookie);
            }
        }
    } else {
        printf("%s\n", extract_error(response));
    }
}

void login_admin(int sockfd, const char *username, const char *password, 
                    char ***cookies, int *cookies_count) {
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    json_object_set_string(root_object, "username", username);
    json_object_set_string(root_object, "password", password);

    char *json_string = json_serialize_to_string(root_value);

    post_json(sockfd, "/api/v1/tema/admin/login", json_string, cookies,
            cookies_count, NULL, "200 OK", "Admin autentificat cu succes");
    json_free_serialized_string(json_string);
    json_value_free(root_value);
}

void add_user(int sockfd, const char *username, const char *password, 
                    char ***cookies, int *cookies_count, const char *auth_token) {
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    json_object_set_string(root_object, "username", username);
    json_object_set_string(root_object, "password", password);

    char *json_string = json_serialize_to_string(root_value);

    post_json(sockfd, "/api/v1/tema/admin/users", json_string, cookies, 
            cookies_count, auth_token, "201 CREATED", "Utilizator adăugat cu succes");

    json_free_serialized_string(json_string);
    json_value_free(root_value);
}

void get_users(int sockfd, char ***cookies, int *cookies_count) {
    char *request = compute_get_request(HOST, "/api/v1/tema/admin/users", 
                                        NULL, *cookies, *cookies_count, NULL);
    send_to_server(sockfd, request);
    char *response = receive_from_server(sockfd);
   
    if (strstr(response, "200 OK") != NULL) {
        char *body = strstr(response, "\r\n\r\n");
            JSON_Value *val = json_parse_string(body);
            JSON_Object *root_obj = json_value_get_object(val);
            JSON_Array *users = json_object_get_array(root_obj, "users");

            for (size_t i = 0; i < json_array_get_count(users); i++) {
                JSON_Object *user_obj = json_array_get_object(users, i);
                const char *username = json_object_get_string(user_obj, "username");
                const char *password = json_object_get_string(user_obj, "password");
                printf("#%zu %s:%s\n", i + 1, username, password);
            }

            json_value_free(val);
    } else {
        printf("%s\n", extract_error(response));
    }
}

void logout_admin(int sockfd, char ***cookies, int *cookies_count, int *success) {
    char *request = compute_get_request(HOST, "/api/v1/tema/admin/logout", 
                                        NULL, *cookies, *cookies_count, NULL);
    send_to_server(sockfd, request);
    char *response = receive_from_server(sockfd);
   if (strstr(response, "200 OK") != NULL) {
        *success = 1;
        printf("SUCCESS: Admin delogat\n");
    } else {
        printf("%s\n", extract_error(response));
    }
}

void login(int sockfd, const char *admin_username, const char *username,
            const char *password, char ***cookies, int *cookies_count) {
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    json_object_set_string(root_object, "admin_username", admin_username);
    json_object_set_string(root_object, "username", username);
    json_object_set_string(root_object, "password", password);

    char *json_string = json_serialize_to_string(root_value);

    post_json(sockfd, "/api/v1/tema/user/login", json_string, cookies,
            cookies_count, NULL, "200 OK", "Autentificare reușită");
    json_free_serialized_string(json_string);
    json_value_free(root_value);
}

void logout(int sockfd, char ***cookies, int *cookies_count, int *success) {
    char *request = compute_get_request(HOST, "/api/v1/tema/user/logout", 
                                        NULL, *cookies, *cookies_count, NULL);
    send_to_server(sockfd, request);
    char *response = receive_from_server(sockfd);
   if (strstr(response, "200 OK") != NULL) {
        *success = 1;
        printf("SUCCESS: Utilizator delogat\n");
    } else {
        printf("%s\n", extract_error(response));
    }
}

void delete_user(int sockfd, const char *username, char ***cookies, 
                 int *cookies_count, const char *auth_token) {
    char body[LINE_SIZE];
    sprintf(body, "username=%s", username);

    char url[LINE_SIZE];
    sprintf(url, "/api/v1/tema/admin/users/%s", username);

    char *request = compute_delete_request(HOST, url, body, *cookies, 
                                            *cookies_count, auth_token);

    send_to_server(sockfd, request);
    char *response = receive_from_server(sockfd);
    if (strstr(response, "200 OK") != NULL) {
        printf("SUCCESS: Utilizator șters\n");
    } else {
        printf("%s\n", extract_error(response));
    }
}

void get_access(int sockfd, char ***cookies, int *cookies_count) {
    char *request = compute_get_request(HOST, "/api/v1/tema/library/access", NULL, *cookies, *cookies_count, NULL);
    send_to_server(sockfd, request);

    char *response = receive_from_server(sockfd);
    if (strstr(response, "200 OK") != NULL) {
        char jwt_token[COOKIE_LEN];
        memmove(jwt_token, extract_jwt_token(response), COOKIE_LEN);
        add_cookie(cookies, cookies_count, jwt_token);
        printf("SUCCESS: Token JWT primit\n");
    } else {
        printf("%s\n", extract_error(response));
    }
}

void get_movies(int sockfd, char ***cookies, int *cookies_count, char *auth_token) {
    char *request = compute_get_request(HOST, "/api/v1/tema/library/movies", 
                                        NULL, *cookies, *cookies_count, auth_token);
    send_to_server(sockfd, request);

    char *response = receive_from_server(sockfd);
    if (strstr(response, "200 OK") != NULL) {
        printf("SUCCESS: Lista filmelor\n");
        char *body = strstr(response, "{");
        JSON_Value *root_value = json_parse_string(body);
        JSON_Object *root_object = json_value_get_object(root_value);
        JSON_Array *movies = json_object_get_array(root_object, "movies");

        size_t n = json_array_get_count(movies);
        for (size_t i = 0; i < n; i++) {
            JSON_Object *movie = json_array_get_object(movies, i);
            int id = (int) json_object_get_number(movie, "id");
            const char *title = json_object_get_string(movie, "title");
            printf("#%d: %s\n", id, title);
        }

        json_value_free(root_value);
    } else {
        printf("%s\n", extract_error(response));
    }
}

void get_movie(int sockfd, char *id, char ***cookies, int *cookies_count, char *access_token) {
    char url[LINE_SIZE];
    sprintf(url, "/api/v1/tema/library/movies/%s", id);

    char *request = compute_get_request(HOST, url, NULL, *cookies, *cookies_count, access_token);
    send_to_server(sockfd, request);

    char *response = receive_from_server(sockfd);
    if (strstr(response, "200 OK") != NULL) {
        printf("SUCCESS: Detalii film\n");
        char *body = strstr(response, "{");
        JSON_Value *root_value = json_parse_string(body);
        JSON_Object *root_object = json_value_get_object(root_value);

        const char *title = json_object_get_string(root_object, "title");
        int year = (int) json_object_get_number(root_object, "year");
        const char *description = json_object_get_string(root_object, "description");
        double rating = atof(json_object_get_string(root_object, "rating"));

        printf("title: %s\n", title);
        printf("year: %d\n", year);
        printf("description: %s\n", description);
        printf("rating: %.1f\n", rating);

        json_value_free(root_value);
    } else {
        printf("%s\n", extract_error(response));
    }
}

void add_movie(int sockfd, char *title, char *year, char *description, char *rating,
                char ***cookies, int *cookies_count, char *access_token) {
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    json_object_set_string(root_object, "title", title);
    json_object_set_number(root_object, "year", atof(year));
    json_object_set_string(root_object, "description", description);
    json_object_set_number(root_object, "rating", atof(rating));

    char *json_string = json_serialize_to_string(root_value);

    post_json(sockfd, "/api/v1/tema/library/movies", json_string, cookies, 
            cookies_count, access_token, "201 CREATED", "Film adăugat");

    json_free_serialized_string(json_string);
    json_value_free(root_value);
}

void delete_movie(int sockfd, char *id, char ***cookies, int *cookies_count, char *access_token) {
    char url[LINE_SIZE];
    sprintf(url, "/api/v1/tema/library/movies/%s", id);

    char *request = compute_delete_request(HOST, url, NULL, *cookies, 
                                            *cookies_count, access_token);
    send_to_server(sockfd, request);

    char *response = receive_from_server(sockfd);
    if (strstr(response, "200 OK") != NULL) {
        printf("SUCCESS: Film șters cu succes\n");
    } else {
        printf("%s\n", extract_error(response));
    }
}

void update_movie(int sockfd, char *id, char *title, char *year, char *description, char *rating,
                    char ***cookies, int *cookies_count, char *access_token) {
    char url[LINE_SIZE];
    sprintf(url, "/api/v1/tema/library/movies/%s", id);

    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    json_object_set_string(root_object, "title", title);
    json_object_set_number(root_object, "year", atof(year));
    json_object_set_string(root_object, "description", description);
    json_object_set_number(root_object, "rating", atof(rating));

    char *json_string = json_serialize_to_string(root_value);

    char *body_data[1];
    body_data[0] = json_string;

    char *request = compute_put_request(HOST, url, "application/json",
                    body_data, 1, *cookies, *cookies_count, access_token);

    send_to_server(sockfd, request);
    char *response = receive_from_server(sockfd);
    if (strstr(response, "200 OK") != NULL) {
        printf("SUCCESS: Film actualizat");
    } else {
        printf("%s\n", extract_error(response));
    }

    json_free_serialized_string(json_string);
    json_value_free(root_value);
}

void get_collections(int sockfd, char ***cookies, int *cookies_count, char *auth_token) {
    char *request = compute_get_request(HOST, "/api/v1/tema/library/collections", 
                                        NULL, *cookies, *cookies_count, auth_token);
    send_to_server(sockfd, request);

    char *response = receive_from_server(sockfd);
    if (strstr(response, "200 OK") != NULL) {
        printf("SUCCESS: Lista colecțiilor\n");

        char *body = strstr(response, "{");
        JSON_Value *root_value = json_parse_string(body);
        JSON_Object *root_object = json_value_get_object(root_value);
        JSON_Array *collections = json_object_get_array(root_object, "collections");

        size_t n = json_array_get_count(collections);
        for (size_t i = 0; i < n; i++) {
            JSON_Object *movie = json_array_get_object(collections, i);
            int id = (int) json_object_get_number(movie, "id");
            const char *title = json_object_get_string(movie, "title");
            printf("#%d: %s\n", id, title);
        }

        json_value_free(root_value);
    } else {
        printf("%s\n", extract_error(response));
    }
}

void get_collection(int sockfd, char *id, char ***cookies, int *cookies_count, char *access_token) {
    char url[LINE_SIZE];
    sprintf(url, "/api/v1/tema/library/collections/%s", id);

    char *request = compute_get_request(HOST, url, NULL, *cookies, *cookies_count, access_token);
    send_to_server(sockfd, request);
    char *response = receive_from_server(sockfd);
    if (strstr(response, "200 OK") != NULL) {
        printf("SUCCESS: Detalii colecție\n");

        char *body = strstr(response, "{");
        JSON_Value *root_value = json_parse_string(body);
        JSON_Object *root_object = json_value_get_object(root_value);


        const char *title = json_object_get_string(root_object, "title");
        printf("title: %s\n", title);

        const char *owner = json_object_get_string(root_object, "owner");
        printf("owner: %s\n", owner);

        JSON_Array *movies = json_object_get_array(root_object, "movies");
        size_t n = json_array_get_count(movies);
        for (size_t i = 0; i < n; i++) {
            JSON_Object *movie = json_array_get_object(movies, i);
            int movie_id = (int) json_object_get_number(movie, "id");
            const char *movie_title = json_object_get_string(movie, "title");
            printf("#%d: %s\n", movie_id, movie_title ? movie_title : "N/A");
        }

        json_value_free(root_value);
    } else {
        printf("%s\n", extract_error(response));
    }
}

void add_collection(int sockfd, char *title, char *owner, int *movies, int num_movies,
                    char ***cookies, int *cookies_count, char *access_token) {
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);
    json_object_set_string(root_object, "title", title);
    json_object_set_string(root_object, "owner", owner);

    char *json_string = json_serialize_to_string(root_value);

    char *body_data[1];
    body_data[0] = json_string;
    char *request = compute_post_request(HOST, "/api/v1/tema/library/collections", "application/json",
                                         body_data, 1, *cookies, *cookies_count, access_token);
    send_to_server(sockfd, request);

    char *response = receive_from_server(sockfd);
    if (strstr(response, "201 CREATED") != NULL) {
        // extract id
        int id = -1;
        char *body = strstr(response, "{");
        sscanf(strstr(body, "\"id\":"), "\"id\":%d", &id);

        // add movies to the collection
        for (int i = 0; i < num_movies; i++) {
            JSON_Value *movie_val = json_value_init_object();
            JSON_Object *movie_obj = json_value_get_object(movie_val);
            json_object_set_number(movie_obj, "id", movies[i]);
            char *movie_json = json_serialize_to_string(movie_val);

            char url[LINE_SIZE];
            sprintf(url, "/api/v1/tema/library/collections/%d/movies", id);

            char *movie_body[1];
            movie_body[0] = movie_json;
            char *movie_request = compute_post_request(HOST, url, "application/json",
                                                        movie_body, 1, *cookies, *cookies_count, access_token);
            send_to_server(sockfd, movie_request);
            char *movie_response = receive_from_server(sockfd);

            free(movie_request);
            free(movie_response);
            json_free_serialized_string(movie_json);
            json_value_free(movie_val);
        }

        printf("SUCCESS: Colecție adăugată\n");
    } else {
        printf("%s\n", extract_error(response));
    }

    json_free_serialized_string(json_string);
    json_value_free(root_value);
}

void delete_collection(int sockfd, char *id, char ***cookies, 
                        int *cookies_count, char *access_token) {
    char url[LINE_SIZE];
    sprintf(url, "/api/v1/tema/library/collections/%s", id);

    char *request = compute_delete_request(HOST, url, NULL, *cookies, 
                                            *cookies_count, access_token);
    send_to_server(sockfd, request);

    char *response = receive_from_server(sockfd);
    if (strstr(response, "200 OK") != NULL) {
        printf("SUCCESS: Colecție ștearsă\n");
    } else {
        printf("%s\n", extract_error(response));
    }                        
}

void add_movie_to_collection(int sockfd, int collection_id, int movie_id,
                        char ***cookies, int *cookies_count, char *access_token) {
    char url[LINE_SIZE];
    sprintf(url, "/api/v1/tema/library/collections/%d/movies", collection_id);
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    json_object_set_number(root_object, "id", movie_id);

    char *json_string = json_serialize_to_string(root_value);

    post_json(sockfd, url, json_string, cookies, 
            cookies_count, access_token, "201 CREATED", "Film adăugat în colecție");

    json_free_serialized_string(json_string);
    json_value_free(root_value);                        
}

void delete_movie_from_collection(int sockfd, int collection_id, int movie_id,
                        char ***cookies, int *cookies_count, char *access_token) {
    char url[LINE_SIZE];
    sprintf(url, "/api/v1/tema/library/collections/%d/movies/%d", collection_id, movie_id);

    char *request = compute_delete_request(HOST, url, NULL, *cookies, 
                                            *cookies_count, access_token);
    send_to_server(sockfd, request);

    char *response = receive_from_server(sockfd);
    if (strstr(response, "200 OK") != NULL) {
        printf("SUCCESS: Film șters din colecție\n");
    } else {
        printf("%s\n", extract_error(response));
    }                                

}

