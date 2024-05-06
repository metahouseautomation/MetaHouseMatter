#include "esp_http_server.h"
#include "url_handlers.hpp"

esp_err_t file_read_handler(httpd_req_t *req)
{
    /* Send a simple response */
    const char resp[] = "URI: /uri gggggggggggggg";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}