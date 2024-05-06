// #include <SPIFFS.h>
#include <esp_http_server.h>

#include "url_handlers.hpp"

// String getContentType(String filename)
// {
//     if (filename.endsWith(".htm"))
//         return "text/html";
//     if (filename.endsWith(".html"))
//         return "text/html";
//     if (filename.endsWith(".css"))
//         return "text/css";
//     if (filename.endsWith(".js"))
//         return "application/javascript";
//     if (filename.endsWith(".png"))
//         return "image/png";
//     if (filename.endsWith(".gif"))
//         return "image/gif";
//     if (filename.endsWith(".jpg"))
//         return "image/jpeg";
//     if (filename.endsWith(".ico"))
//         return "image/x-icon";
//     if (filename.endsWith(".xml"))
//         return "text/xml";
//     if (filename.endsWith(".pdf"))
//         return "application/x-pdf";
//     if (filename.endsWith(".zip"))
//         return "application/x-zip";
//     if (filename.endsWith(".gz"))
//         return "application/x-gzip";
//     return "text/plain";
// }

esp_err_t file_read_handler(httpd_req_t *req)
{
    /* Send a simple response */
    const char resp[] = "URI: /uri gggggggggggggg";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}