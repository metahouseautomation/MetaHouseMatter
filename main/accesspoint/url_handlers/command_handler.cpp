#include <esp_log.h>

#include "helper_handler.hpp"
#include "url_handlers.hpp"

#define COMMAND_HANDLER_TAG "COMMAND_HANDLER"

esp_err_t command_handler(httpd_req_t *req)
{
    /* Check the method of the request */
    if (req->method == HTTP_GET) {
        /* Get the URI from the request */
        char *uri = (char *)req->uri;
        if (uri == NULL) {
            ESP_LOGE(COMMAND_HANDLER_TAG, "Failed to get the URI from the request");
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }

        /* Check the URI */
        if (strcmp(uri, "/command/unpair") == 0) {
            /* Erase all data but not the accessories, restart the device */
            if (erase_all_data(false) != ESP_OK) {
                ESP_LOGE(COMMAND_HANDLER_TAG, "Failed to erase all data but not the accessories");
                httpd_resp_send_500(req);
                return ESP_FAIL;
            }

        } else if (strcmp(uri, "/command/factory") == 0) {
            /* Erase all data including accessories, restart the device */
            if (erase_all_data(true) != ESP_OK) {
                ESP_LOGE(COMMAND_HANDLER_TAG, "Failed to erase all data including accessories");
                httpd_resp_send_500(req);
                return ESP_FAIL;
            }

        } else if (strcmp(uri, "/command/updatefirmware") == 0) {
            /* Update the firmware, restart the device */
            if (update_firmware() != ESP_OK) {
                ESP_LOGE(COMMAND_HANDLER_TAG, "Failed to update the firmware");
                httpd_resp_send_500(req);
                return ESP_FAIL;
            }

        } else if (strcmp(uri, "/command/restart") == 0) {
            /* Restart the device */
            if (restart_device() != ESP_OK) {
                ESP_LOGE(COMMAND_HANDLER_TAG, "Failed to restart the device");
                httpd_resp_send_500(req);
                return ESP_FAIL;
            }

        } else {
            /* Invalid URI */
            ESP_LOGE(COMMAND_HANDLER_TAG, "Invalid URI: %s", uri);
            httpd_resp_send_404(req);
            return ESP_FAIL;
        }

        /* Send the response */
        httpd_resp_send(req, "success", HTTPD_RESP_USE_STRLEN);

        return ESP_OK;

    } else {
        /* Invalid method */
        ESP_LOGE(COMMAND_HANDLER_TAG, "Invalid method: %d", req->method);
        httpd_resp_send_err(req, HTTPD_405_METHOD_NOT_ALLOWED, "Invalid method");
        return ESP_FAIL;
    }
}