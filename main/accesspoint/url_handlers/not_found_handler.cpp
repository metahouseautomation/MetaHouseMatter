#include "url_handlers.hpp"

esp_err_t not_found_handler(httpd_req_t *req)
{
    const char *resp_str = "URI not found\n";
    httpd_resp_set_status(req, HTTPD_404);
    httpd_resp_send(req, resp_str, strlen(resp_str));
    return ESP_OK;
}