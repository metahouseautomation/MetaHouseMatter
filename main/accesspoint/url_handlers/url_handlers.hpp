#ifndef URL_HANDLERS_HPP
#define URL_HANDLERS_HPP

#include <esp_http_server.h>

esp_err_t file_read_handler(httpd_req_t *req);

esp_err_t not_found_handler(httpd_req_t *req);

esp_err_t command_handler(httpd_req_t *req);

esp_err_t accessories_handler(httpd_req_t *req);

esp_err_t wifi_handler(httpd_req_t *req);

#endif // URL_HANDLERS_HPP