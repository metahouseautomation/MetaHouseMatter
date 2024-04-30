#ifndef WINDOW_COVERING_HPP
#define WINDOW_COVERING_HPP

#include <esp_matter.h>
#include <esp_matter_cluster.h>
#include <esp_matter_core.h>
#include <esp_matter_endpoint.h>

#include "accessories/window_accessory/window_accessory.hpp"

// Main namespace for the project
namespace metahouse {
// Namespace for application endpoints
namespace endpoint {
// Namespace for the window covering endpoint
namespace window_covering {
const uint32_t DEVICE_TYPE_ID = ESP_MATTER_WINDOW_COVERING_DEVICE_TYPE_ID; // window covering device type ID
const uint8_t DEVICE_TYPE_VERSION =
    ESP_MATTER_WINDOW_COVERING_DEVICE_TYPE_VERSION; // window covering device type version

// Configuration structure for the window covering
typedef struct config {
    esp_matter::cluster::bridged_device_basic_information::config_t
        bridged_device_basic_information; // used only if the device is a bridged device
    esp_matter::cluster::descriptor::config_t descriptor;
    esp_matter::cluster::identify::config_t identify;
    esp_matter::cluster::groups::config_t groups;
    esp_matter::cluster::scenes::config_t scenes;
    esp_matter::cluster::window_covering::config_t window_covering;
} config_t;

/** create
 * @brief Create the window covering
 *
 * @param node Node to create the window covering on
 * @param config Configuration for the window covering
 * @param aggregator Aggregator as parent for the window covering (optional)
 * @param priv_data Private data for the window covering (optional)
 *
 * @return window covering
 * @note If the aggregator is provided, the window covering will be created as bridged device on the aggregator
 *
 */
esp_matter::endpoint_t *create(esp_matter::node_t *node, config_t *config, esp_matter::endpoint_t *aggregator = nullptr,
                               WindowAccessory *priv_data = nullptr);

} // namespace window_covering
} // namespace endpoint
} // namespace metahouse

#endif // WINDOW_COVERING_HPP