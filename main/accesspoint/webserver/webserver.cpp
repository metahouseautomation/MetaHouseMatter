#include <esp_http_server.h>
#include <esp_log.h>

#include "helper_handler.hpp"
#include "accesspoint/url_handlers/url_structure.hpp"
#include "accesspoint/webserver/webserver.hpp"

/* Function for starting the webserver */
httpd_handle_t metahouse::accesspoint::webserver::start_webserver(void)
{
    /* Generate default configuration */
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    /* Empty handle to esp_http_server */
    httpd_handle_t server = NULL;

    /* Mount the filesystem */
    esp_err_t err = mount_fs("", "frontend");
    if (err != ESP_OK) {
        ESP_LOGE("start_webserver", "Failed to mount filesystem"); // TODO: Change LOG Format.
        return NULL;
    }

    config.uri_match_fn = httpd_uri_match_wildcard;
    config.max_uri_handlers = 10;
    config.stack_size = 8192;

    /* Start the httpd server */
    if (httpd_start(&server, &config) == ESP_OK) {
        /* Register URI handlers */
        httpd_register_uri_handler(server, &uri_command_unpair);
        httpd_register_uri_handler(server, &uri_command_factory);
        httpd_register_uri_handler(server, &uri_command_update);
        httpd_register_uri_handler(server, &uri_command_restart);
        httpd_register_uri_handler(server, &uri_accessories_GET);
        httpd_register_uri_handler(server, &uri_accessories_POST);
        httpd_register_uri_handler(server, &uri_wifi_get_credentials);
        httpd_register_uri_handler(server, &uri_wifi_save_credentials);
        httpd_register_uri_handler(server, &uri_wifi_connect);

        httpd_register_uri_handler(server, &uri_file_read);
    }
    /* If server failed to start, handle will be NULL */
    return server;
}

/* Function for stopping the webserver */
void metahouse::accesspoint::webserver::stop_webserver(httpd_handle_t server)
{
    if (server) {
        /* Stop the httpd server */
        httpd_stop(server);
    }
}