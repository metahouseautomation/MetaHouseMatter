#ifndef WEB_SERVER_HPP
#define WEB_SERVER_HPP

#include <esp_http_server.h>

namespace metahouse {
namespace accesspoint::webserver {
/* Function for starting the webserver */
httpd_handle_t start_webserver(void);

/* Function for stopping the webserver */
void stop_webserver(httpd_handle_t server);
} // namespace accesspoint::webserver
} // namespace metahouse

#endif // WEB_SERVER_HPP