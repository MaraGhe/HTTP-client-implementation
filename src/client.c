#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"

char *read_word(char *field) {
    char *word = malloc(LINE_SIZE);
    int i = 0;
    do {
        printf("%s=", field);
        fgets(word, LINE_SIZE, stdin);
        word[strcspn(word, "\n")] = 0;
        if (strchr(word, ' ') != NULL) {
            printf("%s must not contain spaces\n", field);
        }
        i++;
        DIE(i > MAX_INPUT_TRIES, "Too many failed attempts. Closing the program...");
    } while (strchr(word, ' ') != NULL);
    return word;
}

char *read_sentence(char *field) {
    char *sentence = malloc(LINE_SIZE);
    printf("%s=", field);
    fgets(sentence, LINE_SIZE, stdin);
    sentence[strcspn(sentence, "\n")] = 0;
    return sentence;
}

char *read_nat_number(char *field) {
    char *n_str = malloc(LINE_SIZE);
    int n = -1;
    int i = 0;
    do {
        printf("%s=", field);
        fgets(n_str, LINE_SIZE, stdin);
        n_str[strcspn(n_str, "\n")] = 0;
        if ((atoi(n_str) <= 0 && strcmp(n_str, "0") != 0) 
            || strchr(n_str, '.') != NULL)
            printf("Invalid %s\n", field);
        else 
            n = atoi(n_str);
        i++;
        DIE(i > MAX_INPUT_TRIES, "Too many failed attempts. Closing the program...");    
    } while (n == -1);
    return n_str;
}

char *read_float_number(char *field) {
    int valid = 0, dots = 0;
    char *n_str;
    int i = 0;
    do {
        n_str = read_word(field);
        valid = 1;
        for (int i = 0; i < (int)strlen(n_str); i++) {
            if (n_str[i] == '.') {
                dots++;
                if (dots > 1) {
                    valid = 0;
                    printf("Invalid %s\n", field);
                    break;
                }
            } else if (strchr("0123456789", n_str[i]) == NULL) {
                    valid = 0;
                    printf("Invalid %s\n", field);
                    break;
                }
        }
        i++;
        DIE(i > MAX_INPUT_TRIES, "Too many failed attempts. Closing the program...");
    } while (!valid);
    return n_str;
}

int main() {
    char command[MAX_COMMAND_LEN];

    char **cookies = malloc(MAX_COOKIES * sizeof(char *));
    for (int i = 0; i < MAX_COOKIES; i++)
        cookies[i] = malloc(COOKIE_LEN * sizeof(char));
    int cookies_count = 0;
    char session_cookie[COOKIE_LEN], access_cookie[COOKIE_LEN], user[LINE_SIZE];

    while (1) {
        int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
        memset(command, 0, MAX_COMMAND_LEN);
        fgets(command, MAX_COMMAND_LEN, stdin);
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "exit") == 0) {
            close_connection(sockfd);
            break;
        }

        if (strcmp(command, "login_admin") == 0) {
            char *username = read_word("username");
            char *password = read_word("password");
            int pre_login_cookies = cookies_count;

            login_admin(sockfd, username, password, &cookies, &cookies_count);
            strcpy(user, username);
            free(username);
            free(password);

            if (cookies_count > pre_login_cookies) {
                memmove(session_cookie, cookies[cookies_count - 1], COOKIE_LEN);
                session_cookie[COOKIE_LEN - 1] = '\0';
            }
            continue;
        }

        if (strcmp(command, "add_user") == 0) {
            char *username = read_word("username");
            char *password = read_word("password");
            add_user(sockfd, username, password, &cookies, &cookies_count, session_cookie);
            free(username);
            free(password);
            continue;
        }

        if (strcmp(command, "get_users") == 0) {
            get_users(sockfd, &cookies, &cookies_count);
            continue;
        }

        if (strcmp(command, "logout_admin") == 0) {
            int success = 0;
            logout_admin(sockfd, &cookies, &cookies_count, &success);
            if (success == 1) {
                remove_cookies(&cookies, &cookies_count);
                memset(session_cookie, 0, COOKIE_LEN);
            }
            continue;
        }

        if (strcmp(command, "login") == 0) {
            char *admin_username = read_word("admin_username");
            char *username = read_word("username");
            char *password = read_word("password");

            int pre_login_cookies = cookies_count;
            login(sockfd, admin_username, username, password, &cookies, &cookies_count);
            if (cookies_count > pre_login_cookies) {
                memmove(session_cookie, cookies[cookies_count - 1], COOKIE_LEN);
                session_cookie[COOKIE_LEN - 1] = '\0';
                strcpy(user, username);
            }

            free(admin_username);
            free(username);
            free(password);
            continue;
        }

        if (strcmp(command, "logout") == 0) {
            int success = 0;
            logout(sockfd, &cookies, &cookies_count, &success);
            if (success == 1) {
                remove_cookies(&cookies, &cookies_count);
                memset(session_cookie, 0, COOKIE_LEN);
            }
            continue;
        }

        if (strcmp(command, "delete_user") == 0) {
            char *username = read_word("username");
            delete_user(sockfd, username, &cookies, &cookies_count, session_cookie);
            free(username);
            continue;
        }

        if (strcmp(command, "get_access") == 0) {
            int pre_login_cookies = cookies_count;
            get_access(sockfd, &cookies, &cookies_count);
            if (cookies_count > pre_login_cookies) {
                memmove(access_cookie, cookies[cookies_count - 1], COOKIE_LEN);
                access_cookie[COOKIE_LEN - 1] = '\0';
            }
            continue;
        }

        if (strcmp(command, "get_movies") == 0) {
            get_movies(sockfd, &cookies, &cookies_count, access_cookie);
            continue;
        }

        if (strcmp(command, "get_movie") == 0) {
            char *id = read_nat_number("id");
            get_movie(sockfd, id, &cookies, &cookies_count, access_cookie);
            free(id);
            continue;
        }

        if (strcmp(command, "add_movie") == 0) {
            char *title = read_sentence("title");
            char *year = read_nat_number("year");
            char *description = read_sentence("description");
            char *rating = read_float_number("rating");

            add_movie(sockfd, title, year, description, rating, 
                        &cookies, &cookies_count, access_cookie);

            free(title);
            free(year);
            free(description);
            free(rating);
            continue;
        }

        if (strcmp(command, "delete_movie") == 0) {
            char *id = read_nat_number("id");
            delete_movie(sockfd, id, &cookies, &cookies_count, access_cookie);
            free(id);
            continue;
        }

        if (strcmp(command, "update_movie") == 0) {
            char *id = read_nat_number("id"); 
            char *title = read_sentence("title");
            char *year = read_nat_number("year");
            char *description = read_sentence("description");
            char *rating = read_float_number("rating");

            update_movie(sockfd, id, title, year, description, rating, 
                        &cookies, &cookies_count, access_cookie);

            free(title);
            free(year);
            free(description);
            free(rating);
            continue;
        }
        
        if (strcmp(command, "get_collections") == 0) {
            get_collections(sockfd, &cookies, &cookies_count, access_cookie);
            continue;
        }

        if (strcmp(command, "get_collection") == 0) {
            char *id = read_nat_number("id");
            get_collection(sockfd, id, &cookies, &cookies_count, access_cookie);
            free(id);
            continue;
        }

        if (strcmp(command, "add_collection") == 0) {
            char *title = read_sentence("title");
            int num_movies = atoi(read_nat_number("num_movies"));
            int movies[num_movies];  
            char field[LINE_SIZE];
            for (int i = 0; i < num_movies; i++) {
                sprintf(field, "movie_id[%d]", i);
                movies[i] = atoi (read_nat_number(field));
            }

            add_collection(sockfd, title, user, movies, num_movies,
                            &cookies, &cookies_count, access_cookie);

            free(title);
            continue;
        }

        if (strcmp(command, "delete_collection") == 0) {
            char *id = read_nat_number("id");
            delete_collection(sockfd, id, &cookies, &cookies_count, access_cookie);
            free(id);
            continue;
        }

        if (strcmp(command, "add_movie_to_collection") == 0) {
            int collection_id = atoi(read_nat_number("collection_id"));
            int movie_id = atoi(read_nat_number("movie_id"));
            add_movie_to_collection(sockfd, collection_id, movie_id, 
                                    &cookies, &cookies_count, access_cookie);
            continue;
        }

        if (strcmp(command, "delete_movie_from_collection") == 0) {
            int collection_id = atoi(read_nat_number("collection_id"));
            int movie_id = atoi(read_nat_number("movie_id"));
            delete_movie_from_collection(sockfd, collection_id, movie_id, 
                                    &cookies, &cookies_count, access_cookie);
            continue;
        }

        printf("ERROR: Comanda invalida\n");
    }

    for (int i = 0; i < MAX_COOKIES; i++)
        free(cookies[i]);
    free(cookies);
    return 0;
}
