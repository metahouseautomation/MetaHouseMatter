#include "url_handlers.hpp"

esp_err_t wifi_handler(httpd_req_t *req)
{
    /* Check the method of the request */
    if (req->method == HTTP_GET) {
        /* Get the stored WiFi credentials */
        char wifi_stored_SSID[] = "No Need For Matter";
        char wifi_stored_password[] = "No Need For Matter";

        /* Send the stored WiFi credentials as JSON */
        httpd_resp_set_type(req, "application/json");
        char response[100];
        // {data: {SSID: "MetaHouse", Password: "MetaHouse123"}}
        sprintf(response, "{\"data\": {\"SSID\": \"%s\", \"PASSWORD\": \"%s\"}}", wifi_stored_SSID,
                wifi_stored_password);

        httpd_resp_send(req, response, strlen(response));

        return ESP_OK;
    } else if (req->method == HTTP_POST) {
        /* Save the WiFi credentials */
        /* Respond success as JSON*/
        httpd_resp_set_type(req, "application/json");
        char response[100];
        sprintf(response, "{\"message\": \"success\"}");
        httpd_resp_send(req, response, strlen(response));
        return ESP_OK;
    } else if (req->method == HTTP_PUT) {
        /* Try to connect to WiFi */
        /* Respond success as JSON data "" and message success*/
        httpd_resp_set_type(req, "application/json");
        char response[100];
        sprintf(response, "{\"data\": \"\", \"message\": \"success\"}");
        httpd_resp_send(req, response, strlen(response));
        return ESP_OK;
    }
    return ESP_OK;
}