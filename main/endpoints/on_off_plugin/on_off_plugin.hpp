#ifndef ON_OFF_PLUGIN_HPP
#define ON_OFF_PLUGIN_HPP

#include <esp_matter.h>
#include <esp_matter_cluster.h>
#include <esp_matter_command.h>
#include <esp_matter_core.h>
#include <esp_matter_endpoint.h>

// Main namespace for the project
namespace metahouse {
// Namespace for application endpoints
namespace endpoint {
// Namespace for the on/off plugin endpoint
namespace on_off_plugin {
const uint32_t DEVICE_TYPE_ID = ESP_MATTER_ON_OFF_PLUGIN_UNIT_DEVICE_TYPE_ID; // On/off plugin device type ID
const uint8_t DEVICE_TYPE_VERSION =
    ESP_MATTER_ON_OFF_PLUGIN_UNIT_DEVICE_TYPE_VERSION; // On/off plugin device type version

// Configuration structure for the on/off plugin
typedef struct config {
    esp_matter::cluster::bridged_device_basic_information::config_t
        bridged_device_basic_information; // used only if the device is a bridged device
    esp_matter::cluster::descriptor::config_t descriptor;
    esp_matter::cluster::identify::config_t identify;
    esp_matter::cluster::groups::config_t groups;
    esp_matter::cluster::scenes::config_t scenes;
    esp_matter::cluster::on_off::config_t on_off;
} config_t;

/** create
 * @brief Create the on/off plugin
 *
 * @param node Node to create the on/off plugin on
 * @param config Configuration for the on/off plugin
 * @param aggregator Aggregator as parent for the on/off plugin (optional)
 *
 * @return On/off plugin
 * @note If the aggregator is provided, the on/off plugin will be created as bridged device on the aggregator
 *
 */
esp_matter::endpoint_t *create(esp_matter::node_t *node, config_t *config,
                               esp_matter::endpoint_t *aggregator = nullptr);

} // namespace on_off_plugin
} // namespace endpoint
} // namespace metahouse

#endif // ON_OFF_PLUGIN_HPP