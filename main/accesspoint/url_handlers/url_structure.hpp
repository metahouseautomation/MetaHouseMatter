#ifndef URL_STRUCTURE_HPP
#define URL_STRUCTURE_HPP

#include <esp_http_server.h>

#include "http_parser.h"
#include "url_handlers.hpp"

httpd_uri_t uri_file_read = {.uri = "/a", .method = HTTP_GET, .handler = file_read_handler, .user_ctx = NULL};

httpd_uri_t uri_not_found = {.uri = "/b", .method = HTTP_GET, .handler = not_found_handler, .user_ctx = NULL};

httpd_uri_t uri_command = {.uri = "/c", .method = HTTP_GET, .handler = command_handler, .user_ctx = NULL};

httpd_uri_t uri_accessories = {.uri = "/d", .method = HTTP_GET, .handler = accessories_handler, .user_ctx = NULL};

httpd_uri_t uri_wifi = {.uri = "/e", .method = HTTP_GET, .handler = wifi_handler, .user_ctx = NULL};

#endif // URL_STRUCTURE_HPP