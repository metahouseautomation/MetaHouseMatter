#include <esp_http_server.h>

#include "accesspoint/url_handlers/url_structure.hpp"
#include "accesspoint/webserver/webserver.hpp"

/* Function for starting the webserver */
httpd_handle_t metahouse::accesspoint::webserver::start_webserver(void)
{
    /* Generate default configuration */
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    /* Empty handle to esp_http_server */
    httpd_handle_t server = NULL;

    /* Start the httpd server */
    if (httpd_start(&server, &config) == ESP_OK) {
        /* Register URI handlers */
        httpd_register_uri_handler(server, &uri_file_read);
        httpd_register_uri_handler(server, &uri_not_found);
        httpd_register_uri_handler(server, &uri_command);
        httpd_register_uri_handler(server, &uri_accessories);
        httpd_register_uri_handler(server, &uri_wifi);
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