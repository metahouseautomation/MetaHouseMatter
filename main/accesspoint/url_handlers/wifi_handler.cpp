#include "url_handlers.hpp"

esp_err_t wifi_handler(httpd_req_t *req)
{
    /* Send a simple response */
    const char resp[] = "URI: /uri zzzzzzzzzz";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}