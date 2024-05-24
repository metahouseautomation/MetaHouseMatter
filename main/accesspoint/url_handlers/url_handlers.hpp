#ifndef URL_HANDLERS_HPP
#define URL_HANDLERS_HPP

#include <esp_http_server.h>

esp_err_t file_read_handler(httpd_req_t *req);

esp_err_t not_found_handler(httpd_req_t *req);

/**
    * @brief Handler for the Command URI
    *
    * @param req Request object
    * @return esp_err_t

    * @note This handler is used to handle the Command URI requests from the client.
    * @note The handler is used to handle the following requests:
    *     - GET: /command/unpair - Erase all data but not the accessories, restart the device.
    *     - GET: /command/factory - Erase all data including accessories, restart the device.
    *     - GET: /command/updatefirmware - Update the firmware, restart the device.
    *     - GET: /command/restart - Restart the device

*/
esp_err_t command_handler(httpd_req_t *req);

/**
    * @brief Handler for the Accessories URI
    *
    * @param req Request object
    * @return esp_err_t

    * @note This handler is used to handle the Accessories URI requests from the client.
    * @note The handler is used to handle the following requests:
    *     - GET: Get all accessories list as JSON from storage
    *     - POST: Save new accessories list as JSON

*/
esp_err_t accessories_handler(httpd_req_t *req);

/**
    * @brief Handler for the WiFi URI
    *
    * @param req Request object
    * @return esp_err_t

    * @note This handler is used to handle the WiFi URI requests from the client.
    * @note The handler is used to handle the following requests:
    *     - GET: Get the stored WiFi credentials
    *     - POST: Save the WiFi credentials
    *     - PUT: Try to connect to WiFi
*/
esp_err_t wifi_handler(httpd_req_t *req);

#endif // URL_HANDLERS_HPP