// #include <SPIFFS.h>
#include <cstring>
#include <esp_http_server.h>
#include <esp_log.h>

#include "helper_handler.hpp"
#include "url_handlers.hpp"

esp_err_t file_read_handler(httpd_req_t *req)
{
    /* Get the URI from the request */
    const char *uri = req->uri;

    /* Check if the URI is NULL */
    if (!uri || strlen(uri) == 0) {
        ESP_LOGE("file_read_handler", "URI is NULL");
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "URI is NULL");
        return ESP_OK;
    }

    /* Check if the URI is a file */
    if (uri[strlen(uri) - 1] == '.') {
        ESP_LOGE("file_read_handler", "URI is INVALID");
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "URI is INVALID");
        return ESP_OK;
    }

    char filename[MAX_PATH_SIZE];
    /* Check if the URI is a directory*/
    if (uri[0] == '/' && uri[1] == '\0') {
        strncpy(filename, "/index.html", MAX_PATH_SIZE);
    } else {
        if (uri[strlen(uri) - 1] == '/') {
            strncpy(filename, uri, strlen(uri) - 1);
        } else {
            strncpy(filename, uri, MAX_PATH_SIZE);
        }
    }

    ESP_LOGI("file_read_handler", "Filename: %s", filename);

    /* Check if the file exists */
    esp_err_t err = check_file_exist(filename);
    if (err != ESP_OK) {
        ESP_LOGE("file_read_handler", "File does not exist");
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "File does not exist");
        return ESP_OK;
    }

    /* Set the content type of the response */
    err = set_content_type(req, filename);
    if (err != ESP_OK) {
        ESP_LOGE("file_read_handler", "Failed to set content type");
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to set content type");
        return ESP_OK;
    }

    /* Stream the file to the response */
    err = stream_file(req, filename);
    if (err != ESP_OK) {
        ESP_LOGE("file_read_handler", "Failed to stream file");
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to stream file");
        return ESP_OK;
    }

    return ESP_OK;
}