#ifndef ROOT_NODE_HPP
#define ROOT_NODE_HPP

#include <esp_err.h>
#include <esp_matter.h>
#include <esp_matter_cluster.h>
#include <esp_matter_core.h>
#include <esp_matter_endpoint.h>

// Main namespace for the project
namespace metahouse {
// Namespace for application endpoints
namespace endpoint {
// Namespace for the root node
namespace root_node {

const uint32_t DEVICE_TYPE_ID = ESP_MATTER_ROOT_NODE_DEVICE_TYPE_ID; // Root node device type ID
const uint8_t DEVICE_TYPE_VERSION = ESP_MATTER_ROOT_NODE_DEVICE_TYPE_VERSION; // Root node device type version

// Configuration structure for the root node
typedef struct config {
    esp_matter::cluster::descriptor::config_t descriptor;
    esp_matter::cluster::access_control::config_t access_control;
    esp_matter::cluster::basic_information::config_t basic_information;
    esp_matter::cluster::general_commissioning::config_t general_commissioning;
    esp_matter::cluster::network_commissioning::config_t network_commissioning;
    esp_matter::cluster::general_diagnostics::config_t general_diagnostics;
    esp_matter::cluster::administrator_commissioning::config_t administrator_commissioning;
    esp_matter::cluster::operational_credentials::config_t operational_credentials;
    esp_matter::cluster::diagnostics_network_wifi::config_t diagnostics_network_wifi;
    esp_matter::cluster::diagnostics_network_thread::config_t diagnostics_network_thread;
    // esp_matter::cluster::diagnostics_network_ethernet::config_t diagnostics_network_ethernet;
    esp_matter::cluster::icd_management::config_t icd_management;
} config_t;

/** create
 * @brief Create the root node
 *
 * @param config Configuration for the root node
 *
 * @return Root node
 *

*/
esp_matter::node_t *create(config_t *config);
} // namespace root_node
} // namespace endpoint
} // namespace metahouse

#endif // ROOT_NODE_HPP