#include <esp_log.h>

#include "helper_handler.hpp"
#include "url_handlers.hpp"

#define ACCESSORY_JSON_SIZE 2048 // Buffer size for the accessory database JSON
#define ACCESSORIES_HANDLER_TAG "ACCESSORIES_HANDLER"

esp_err_t accessories_handler(httpd_req_t *req)
{
    /* Check the method of the request */
    if (req->method == HTTP_GET) {
        /* Get the accessory database as a JSON string */
        char response[ACCESSORY_JSON_SIZE];
        if (get_accessory_DB_JSON(response, sizeof(response)) != ESP_OK) {
            ESP_LOGE(ACCESSORIES_HANDLER_TAG, "Failed to get the accessory database");
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }

        /* Send the response in format :
            {"data": <accessory database JSON>, "message": "success"}
        */
        httpd_resp_set_type(req, "application/json");

        char response_buffer[ACCESSORY_JSON_SIZE + 64];
        snprintf(response_buffer, sizeof(response_buffer), "{\"data\": %s, \"message\": \"success\"}", response);
        httpd_resp_send(req, response_buffer, HTTPD_RESP_USE_STRLEN);

    } else if (req->method == HTTP_POST) {
        /* Get the content length of the request */
        size_t content_length = req->content_len;
        if (content_length == 0) {
            ESP_LOGE(ACCESSORIES_HANDLER_TAG, "No content in the request");
            httpd_resp_send_408(req);
            return ESP_FAIL;
        }

        /* Allocate a buffer to store the request */
        char *buffer = (char *)malloc(content_length + 1);
        if (buffer == NULL) {
            ESP_LOGE(ACCESSORIES_HANDLER_TAG, "Failed to allocate memory for the request");
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }

        /* Read the request */
        if (httpd_req_recv(req, buffer, content_length) <= 0) {
            ESP_LOGE(ACCESSORIES_HANDLER_TAG, "Failed to read the request");
            httpd_resp_send_500(req);
            free(buffer);
            return ESP_FAIL;
        }

        /* Null-terminate the buffer */
        buffer[content_length] = '\0';

        /* Set the accessory database from the request */
        if (set_accessory_DB_JSON(buffer) != ESP_OK) {
            ESP_LOGE(ACCESSORIES_HANDLER_TAG, "Failed to set the accessory database");
            httpd_resp_send_500(req);
            free(buffer);
            return ESP_FAIL;
        }

        /* Send the response
        {"data": <accessory database JSON>, "message": "success"}
        */
        httpd_resp_set_type(req, "application/json");

        char response_buffer[ACCESSORY_JSON_SIZE + 64];
        snprintf(response_buffer, sizeof(response_buffer), "{\"data\": %s, \"message\": \"success\"}", buffer);
        httpd_resp_send(req, response_buffer, HTTPD_RESP_USE_STRLEN);
        free(buffer);
    } else {
        /* Send the response */
        httpd_resp_send_404(req);
    }
    return ESP_OK;
}