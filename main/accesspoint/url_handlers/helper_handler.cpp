#include <esp_log.h>
#include <esp_spiffs.h>
#include <nvs.h>
#include <nvs_flash.h>

#include "esp_err.h"
#include "esp_http_server.h"
#include "helper_handler.hpp"

#define BUFFER_ACCESSORY_DB_SIZE 2048 // Buffer size for the accessory database

#define IS_FILE_EXT(filename, ext) (strcasecmp(&filename[strlen(filename) - sizeof(ext) + 1], ext) == 0)

esp_err_t check_file_exist(const char *filename)
{
    // Check if the file exists
    if (!filename) {
        return ESP_ERR_INVALID_ARG;
    }

    FILE *file = fopen(filename, "r");
    if (!file) {
        return ESP_ERR_NOT_FOUND;
    }

    fclose(file);
    return ESP_OK;
}

esp_err_t set_content_type(httpd_req_t *req, const char *filename)
{
    if (!filename || !req) {
        return ESP_ERR_INVALID_ARG;
    }
    if (IS_FILE_EXT(filename, ".htm"))
        return httpd_resp_set_type(req, "text/html");
    if (IS_FILE_EXT(filename, ".html"))
        return httpd_resp_set_type(req, "text/html");
    if (IS_FILE_EXT(filename, ".css"))
        return httpd_resp_set_type(req, "text/css");
    if (IS_FILE_EXT(filename, ".js"))
        return httpd_resp_set_type(req, "application/javascript");
    if (IS_FILE_EXT(filename, ".png"))
        return httpd_resp_set_type(req, "image/png");
    if (IS_FILE_EXT(filename, ".gif"))
        return httpd_resp_set_type(req, "image/gif");
    if (IS_FILE_EXT(filename, ".jpg"))
        return httpd_resp_set_type(req, "image/jpeg");
    if (IS_FILE_EXT(filename, ".ico"))
        return httpd_resp_set_type(req, "image/x-icon");
    if (IS_FILE_EXT(filename, ".xml"))
        return httpd_resp_set_type(req, "text/xml");
    if (IS_FILE_EXT(filename, ".pdf"))
        return httpd_resp_set_type(req, "application/x-pdf");
    if (IS_FILE_EXT(filename, ".zip"))
        return httpd_resp_set_type(req, "application/x-zip");
    if (IS_FILE_EXT(filename, ".gz"))
        return httpd_resp_set_type(req, "application/x-gzip");

    return httpd_resp_set_type(req, "text/plain");
}

esp_err_t stream_file(httpd_req_t *req, const char *filename)
{
    if (!filename || !req) {
        return ESP_ERR_INVALID_ARG;
    }

    // Open the file
    FILE *file = fopen(filename, "r");
    if (!file) {
        return ESP_FAIL;
    }

    // Stream the file
    char buffer[BUFFER_STREAM_SIZE];
    size_t read_bytes;
    do {
        read_bytes = fread(buffer, 1, BUFFER_STREAM_SIZE, file);
        ESP_LOGI("stream_file", "Read %zu bytes", read_bytes);
        if (read_bytes > 0) {
            esp_err_t err = httpd_resp_send_chunk(req, buffer, read_bytes);
            if (err != ESP_OK) {
                fclose(file);
                /* Abort sending file */
                httpd_resp_sendstr_chunk(req, NULL); // TODO:  httpd_resp_send_chunk(req, NULL, 0);
                return err;
            }
        }
    } while (read_bytes > 0);

    // Close the file
    fclose(file);

    // Finish the response
    return httpd_resp_send_chunk(req, NULL, 0);
}

esp_err_t mount_fs(char *base_path, char *partition_label)
{
    if (!base_path || !partition_label) {
        return ESP_ERR_INVALID_ARG;
    }

    esp_vfs_spiffs_conf_t conf = {.base_path = base_path,
                                  .partition_label = partition_label,
                                  .max_files =
                                      5, // This sets the maximum number of files that can be open at the same time
                                  .format_if_mount_failed = false};

    esp_err_t err = esp_vfs_spiffs_register(&conf);
    if (err != ESP_OK) {
        return err;
    }

    return ESP_OK;
}

esp_err_t get_accessory_DB_JSON(char *response, size_t response_size)
{
    if (!response) {
        return ESP_ERR_INVALID_ARG;
    }

    // Get the accessory database from the NVS
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open("accessory", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        return err;
    }

    size_t required_size;
    err = nvs_get_str(nvs_handle, "accessory_db", NULL, &required_size);
    if (err != ESP_OK) {
        nvs_close(nvs_handle);
        return err;
    }

    if (required_size > response_size) {
        nvs_close(nvs_handle);
        return ESP_ERR_NO_MEM;
    }

    err = nvs_get_str(nvs_handle, "accessory_db", response, &required_size);
    if (err != ESP_OK) {
        nvs_close(nvs_handle);
        return err;
    }

    nvs_close(nvs_handle);
    return ESP_OK;
}

esp_err_t set_accessory_DB_JSON(const char *new_accessory_json)
{
    if (!new_accessory_json) {
        return ESP_ERR_INVALID_ARG;
    }

    // Set the accessory database to the NVS
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open("accessory", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        return err;
    }

    err = nvs_set_str(nvs_handle, "accessory_db", new_accessory_json);
    if (err != ESP_OK) {
        nvs_close(nvs_handle);
        return err;
    }

    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        nvs_close(nvs_handle);
        return err;
    }

    nvs_close(nvs_handle);
    return ESP_OK;
}

/* All data included in nvs default partition and also the accessories in default nvs partition.*/
esp_err_t erase_all_data(bool erase_accessories)
{
    char accessory_db[BUFFER_ACCESSORY_DB_SIZE];
    if (!erase_accessories) {
        // temporary store the accessories from nvs partition to restore it later
        if (get_accessory_DB_JSON(accessory_db, BUFFER_ACCESSORY_DB_SIZE) != ESP_OK) {
            return ESP_FAIL;
        }
    }

    // Flash erase nvs partition then re init nvs
    esp_err_t err = nvs_flash_erase();
    if (err != ESP_OK) {
        return err;
    }

    err = nvs_flash_init();
    if (err != ESP_OK) {
        return err;
    }

    if (!erase_accessories) {
        // restore the accessories from the temporary store
        if (set_accessory_DB_JSON(accessory_db) != ESP_OK) {
            return ESP_FAIL;
        }
    }

    restart_device();
    return ESP_OK;
}

esp_err_t restart_device()
{
    esp_restart();
    return ESP_OK;
}

esp_err_t update_firmware()
{
    esp_restart();
    return ESP_OK;
}