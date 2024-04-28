#ifndef FAN_HPP
#define FAN_HPP

#include <esp_matter.h>
#include <esp_matter_cluster.h>
#include <esp_matter_core.h>
#include <esp_matter_endpoint.h>

// Main namespace for the project
namespace metahouse {
// Namespace for application endpoints
namespace endpoint {
// Namespace for the fan endpoint
namespace fan {
const uint32_t DEVICE_TYPE_ID = ESP_MATTER_FAN_DEVICE_TYPE_ID; // fan device type ID
const uint8_t DEVICE_TYPE_VERSION = ESP_MATTER_FAN_DEVICE_TYPE_VERSION; // fan device type version

// Configuration structure for the fan
typedef struct config {
    esp_matter::cluster::bridged_device_basic_information::config_t
        bridged_device_basic_information; // used only if the device is a bridged device
    esp_matter::cluster::descriptor::config_t descriptor;
    esp_matter::cluster::identify::config_t identify;
    esp_matter::cluster::groups::config_t groups;
    esp_matter::cluster::scenes::config_t scenes;
    esp_matter::cluster::fan_control::config_t fan_control;
} config_t;

/** create
 * @brief Create the fan
 *
 * @param node Node to create the fan on
 * @param config Configuration for the fan
 * @param aggregator Aggregator as parent for the fan (optional)
 *
 * @return fan
 * @note If the aggregator is provided, the fan will be created as bridged device on the aggregator
 *
 */
esp_matter::endpoint_t *create(esp_matter::node_t *node, config_t *config,
                               esp_matter::endpoint_t *aggregator = nullptr);

} // namespace fan
} // namespace endpoint
} // namespace metahouse

#endif // FAN_HPP
