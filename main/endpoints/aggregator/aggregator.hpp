#ifndef AGGREGATOR_HPP
#define AGGREGATOR_HPP

#include <esp_matter.h>
#include <esp_matter_cluster.h>
#include <esp_matter_core.h>
#include <esp_matter_endpoint.h>

// Main namespace for the project
namespace metahouse {
// Namespace for application endpoints
namespace endpoint {
namespace aggregator {
const uint32_t DEVICE_TYPE_ID = ESP_MATTER_AGGREGATOR_DEVICE_TYPE_ID; // Aggregator device type ID
const uint8_t DEVICE_TYPE_VERSION = ESP_MATTER_AGGREGATOR_DEVICE_TYPE_VERSION; // Aggregator device type version

// Configuration structure for the aggregator
typedef struct config {
    esp_matter::cluster::descriptor::config_t descriptor;
} config_t;

/** create
 * @brief Create the aggregator
 *
 * @param node Node to create the aggregator on
 * @param config Configuration for the aggregator
 *
 * @return Aggregator
 *
 */
esp_matter::endpoint_t *create(esp_matter::node_t *node, config_t *config);

} // namespace aggregator
} // namespace endpoint
} // namespace metahouse

#endif // AGGREGATOR_HPP