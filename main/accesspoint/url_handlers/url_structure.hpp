#ifndef URL_STRUCTURE_HPP
#define URL_STRUCTURE_HPP

#include <esp_http_server.h>

#include "http_parser.h"
#include "url_handlers.hpp"

// // index.html
// httpd_uri_t uri_file_read_HTML = {.uri = "/", .method = HTTP_GET, .handler = file_read_handler, .user_ctx = NULL};

// image, css, js, etc.
httpd_uri_t uri_file_read = {.uri = "/\*", .method = HTTP_GET, .handler = file_read_handler, .user_ctx = NULL};

// Command Handler {Unpair, Reset Factory, Restart}
httpd_uri_t uri_command_unpair = {
    .uri = "/command/unpair", .method = HTTP_GET, .handler = command_handler, .user_ctx = NULL};
httpd_uri_t uri_command_factory = {
    .uri = "/command/factory", .method = HTTP_GET, .handler = command_handler, .user_ctx = NULL};
httpd_uri_t uri_command_update = {
    .uri = "/command/updatefirmware", .method = HTTP_GET, .handler = command_handler, .user_ctx = NULL};
httpd_uri_t uri_command_restart = {
    .uri = "/command/restart", .method = HTTP_GET, .handler = command_handler, .user_ctx = NULL};

// Accessories Handler {GET, POST}
// GET: Get all accessories
httpd_uri_t uri_accessories_GET = {
    .uri = "/accessories/stored", .method = HTTP_GET, .handler = accessories_handler, .user_ctx = NULL};

// POST: Save new accessories list as JSON
httpd_uri_t uri_accessories_POST = {
    .uri = "/accessories/save", .method = HTTP_POST, .handler = accessories_handler, .user_ctx = NULL};

// WiFi Handler {GET, POST, PUT}
// GET: Get WiFi Credentials
httpd_uri_t uri_wifi_get_credentials = {
    .uri = "/wifi/stored", .method = HTTP_GET, .handler = wifi_handler, .user_ctx = NULL};

// PUT: Save WiFi Credentials
httpd_uri_t uri_wifi_save_credentials = {
    .uri = "/wifi/save", .method = HTTP_POST, .handler = wifi_handler, .user_ctx = NULL};

// POST:  Try to Connect to WiFi
httpd_uri_t uri_wifi_connect = {.uri = "/wifi/connect", .method = HTTP_PUT, .handler = wifi_handler, .user_ctx = NULL};

// Not Found Handler

#endif // URL_STRUCTURE_HPP