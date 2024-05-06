#include "url_handlers.hpp"

esp_err_t command_handler(httpd_req_t *req)
{
    /* Send a simple response */
    const char resp[] = "URI: /uri vvvvvvvvvv";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}