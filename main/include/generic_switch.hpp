#ifndef GENERIC_SWITCH_HPP
#define GENERIC_SWITCH_HPP

#include <esp_matter.h>
#include <esp_matter_cluster.h>
#include <esp_matter_command.h>
#include <esp_matter_core.h>
#include <esp_matter_endpoint.h>

// Main namespace for the project
namespace metahouse {
// Namespace for application endpoints
namespace endpoint {
// Namespace for the generic switch endpoint
namespace generic_switch {
const uint32_t DEVICE_TYPE_ID = ESP_MATTER_GENERIC_SWITCH_DEVICE_TYPE_ID; // Generic switch device type ID
const uint8_t DEVICE_TYPE_VERSION = ESP_MATTER_GENERIC_SWITCH_DEVICE_TYPE_VERSION; // Generic switch device type version

// Configuration structure for the generic switch
typedef struct config {
    esp_matter::cluster::bridged_device_basic_information::config_t
        bridged_device_basic_information; // used only if the device is a bridged device
    esp_matter::cluster::descriptor::config_t descriptor;
    esp_matter::cluster::identify::config_t identify;
    esp_matter::cluster::switch_cluster::config_t switch_cluster;
} config_t;

/** create
 * @brief Create the generic switch
 *
 * @param node Node to create the generic switch on
 * @param config Configuration for the generic switch
 * @param aggregator Aggregator as parent for the generic switch (optional)
 *
 * @return Generic switch
 * @note If the aggregator is provided, the generic switch will be created as bridged device on the aggregator
 *
 */
esp_matter::endpoint_t *create(esp_matter::node_t *node, config_t *config,
                               esp_matter::endpoint_t *aggregator = nullptr);

} // namespace generic_switch
} // namespace endpoint
} // namespace metahouse

#endif // GENERIC_SWITCH_HPP