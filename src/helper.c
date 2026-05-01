#include "helper.h"

void compute_message(char *message, const char *line) {
    strcat(message, line);
    strcat(message, "\r\n");
}

int open_connection(char *host_ip, int port, int ip_type, int socket_type, int flag) {
    struct sockaddr_in addr;
    int sockfd = socket(ip_type, socket_type, flag);
    DIE(sockfd < 0, "Unable to open socket");

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = ip_type;
    addr.sin_port = htons(port);
    DIE(inet_aton(host_ip, &addr.sin_addr) == 0, "Invalid IP address");

    DIE(connect(sockfd, (struct sockaddr *) &addr, sizeof(addr)) < 0,\
        "Unable to connect to server");

    return sockfd;
}

void close_connection(int sockfd) {
    close(sockfd);
}

void send_to_server(int sockfd, char *message) {
    int bytes, sent = 0;
    int total = strlen(message);
    do
    {
        bytes = write(sockfd, message + sent, total - sent);
        DIE(bytes < 0, "Unable to send data to server");

        if (bytes == 0)
            break;

        sent += bytes;
    } while (sent < total);

}

char *receive_from_server(int sockfd) {
    char buffer[BUFFER_SIZE];
    char *response = calloc(BUFFER_SIZE, sizeof(char));
    int total_len = 0;
    int bytes;

    // read first packet of data
    bytes = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
    DIE(bytes < 0, "Error receiving data from server");
    if (bytes == 0) {
        free(response);
        return NULL;
    }

    buffer[bytes] = '\0';
    strcpy(response, buffer);
    total_len = bytes;

    char *headers_end = strstr(response, "\r\n\r\n");
    while (headers_end == NULL) {
        bytes = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
        DIE(bytes < 0, "Error receiving data from server");
        if (bytes == 0) break;

        buffer[bytes] = '\0';
        response = realloc(response, total_len + bytes + 1);
        memcpy(response + total_len, buffer, bytes + 1);
        total_len += bytes;
        headers_end = strstr(response, "\r\n\r\n");
    }

    if (headers_end == NULL) {
        fprintf(stderr, "ERROR: Headers incomplete\n");
        free(response);
        return NULL;
    }

    int headers_len = headers_end - response + 4;
    int content_length = 0;

    char *cl = strstr(response, "Content-Length:");
    if (cl != NULL) {
        sscanf(cl, "Content-Length: %d", &content_length);
    }

    int body_len = total_len - headers_len;

    // read leftover data
    while (body_len < content_length) {
        bytes = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
        DIE(bytes < 0, "Error receiving data from server");
        if (bytes == 0) break;

        buffer[bytes] = '\0';
        response = realloc(response, total_len + bytes + 1);
        memcpy(response + total_len, buffer, bytes + 1);
        total_len += bytes;
        body_len += bytes;
    }

    return response;
}


char *basic_extract_json_response(char *str) {
    return strstr(str, "{\"");
}

void add_cookie(char ***cookies, int *cookies_count, const char *new_cookie) {
    if (*cookies_count >= MAX_COOKIES) {
        fprintf(stderr, "ERROR: Limita de cookieuri atinsa\n");
        return;
    }
    strncpy((*cookies)[*cookies_count], new_cookie, COOKIE_LEN);
    (*cookies)[*cookies_count][COOKIE_LEN - 1] = '\0';
    (*cookies_count)++;
}

void remove_cookies(char ***cookies, int *cookies_count) {
    if (*cookies == NULL || *cookies_count == 0)
        return;

    for (int i = 0; i < *cookies_count; i++) {
        memset((*cookies)[i], 0, COOKIE_LEN);
    }

    *cookies_count = 0;
}

char *extract_jwt_token(char *response) {
    char *start = strstr(response, "\"token\":\"");
    if (!start) 
        return "";
    start += strlen("\"token\":\"");

    char *end = strchr(start, '\"');
    if (!end) 
        return "";

    *end = '\0';
    return start;
}

char *extract_error(char *response) {
    static char error_msg[256];
    char *status_line = strstr(response, "HTTP/1.1 ");
    char *error_field = strstr(response, "\"error\":\"");

    int code;
    char reason[64];
    char error_text[128];

    if (sscanf(status_line, "HTTP/1.1 %d %63[^\r\n]", &code, reason) != 2) {
        return "ERROR: Invalid status line";
    }

    error_field += strlen("\"error\":\"");
    char *end_quote = strchr(error_field, '\"');

    size_t len = end_quote - error_field;
    if (len >= sizeof(error_text)) len = sizeof(error_text) - 1;
    strncpy(error_text, error_field, len);
    error_text[len] = '\0';

    snprintf(error_msg, sizeof(error_msg), "ERROR: %d %s. %s", code, reason, error_text);
    return error_msg;
}
