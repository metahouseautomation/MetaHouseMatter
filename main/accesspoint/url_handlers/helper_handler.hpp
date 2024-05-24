#ifndef HELPER_HANDLER_HPP
#define HELPER_HANDLER_HPP

#include <esp_err.h>
#include <esp_http_server.h>

#define BUFFER_STREAM_SIZE 4096 // Buffer size for streaming files
#define MAX_PATH_SIZE 1024 // Maximum path size

/**
 * @brief Check if a file exists
 *
 * @param filename The filename to check
 * @return esp_err_t ESP_OK if the file exists, ESP_ERR_NOT_FOUND if the file does not exist
 */
esp_err_t check_file_exist(const char *filename);

/**
 * @brief Set the content type of the response based on the filename
 *
 * @param req The request object
 * @param filename The filename to determine the content type
 * @return esp_err_t ESP_OK on success, ESP_ERR_INVALID_ARG if req or filename is NULL
 */
esp_err_t set_content_type(httpd_req_t *req, const char *filename);

/**
 * @brief Stream a file to the response
 *
 * @param req The request object
 * @param filename The filename to stream
 * @return esp_err_t ESP_OK on success, ESP_ERR_INVALID_ARG if req or filename is NULL, ESP_ERR_NOT_FOUND if the file
 * does not exist, ESP_FAIL if an error occurs while reading the file, ESP_ERR_HTTPD_RESP_SEND if an error occurs while
 * sending the response
 */
esp_err_t stream_file(httpd_req_t *req, const char *filename);

/**
 * @brief Mount the filesystem
 *
 * @param base_path The base path to mount the filesystem
 * @param partition_label The partition label of the filesystem
 * @return esp_err_t ESP_OK on success, ESP_ERR_INVALID_ARG if base_path or partition_label is NULL, ESP_FAIL if an
 * error occurs while mounting the filesystem
 */
esp_err_t mount_fs(char *base_path, char *partition_label);

/**
    * @brief Get the content of the accessory database as a JSON string
    *
    * @param response The buffer to store the JSON string
    * @param response_size The size of the buffer
    * @return esp_err_t ESP_OK on success, ESP_ERR_INVALID_ARG if response is NULL

    @note The DB is stored in a JSON string format in the NVS
*/
esp_err_t get_accessory_DB_JSON(char *response, size_t response_size);

/**
    * @brief Set the content of the accessory database from a JSON string
    *
    * @param new_accessory_json The new JSON string to set the DB
    * @return esp_err_t ESP_OK on success, ESP_ERR_INVALID_ARG if new_accessory_json is NULL

    @note The DB is stored in a JSON string format in the NVS
*/
esp_err_t set_accessory_DB_JSON(const char *new_accessory_json);

/**
 * @brief Erase all data in the NVS
 *
 * @param erase_accessories Whether to erase the accessories or not
 * @return esp_err_t ESP_OK on success, ESP_FAIL if an error occurs while erasing the data
 */
esp_err_t erase_all_data(bool erase_accessories);

/**
 * @brief Update the firmware
 *
 * @return esp_err_t ESP_OK on success, ESP_FAIL if an error occurs while updating the firmware
 */
esp_err_t update_firmware();

/**
 * @brief Restart the device
 *
 * @return esp_err_t ESP_OK on success, ESP_FAIL if an error occurs while restarting the device
 */
esp_err_t restart_device();

#endif // HELPER_HANDLER_HPP