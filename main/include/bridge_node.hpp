#ifndef BRIDGE_NODE_HPP
#define BRIDGE_NODE_HPP

#include <esp_matter.h>
#include <esp_matter_endpoint.h>

// Main namespace for the project
namespace metahouse {
// Namespace for application endpoints
namespace endpoint {
namespace bridge_node {
const uint32_t DEVICE_TYPE_ID = ESP_MATTER_BRIDGED_NODE_DEVICE_TYPE_ID; // Bridge node device type ID
const uint8_t DEVICE_TYPE_VERSION = ESP_MATTER_BRIDGED_NODE_DEVICE_TYPE_VERSION; // Bridge node device type version

/** create_bridged_endpoint
 * @brief Create a bridged endpoint
 *
 * @param node Node to create the bridged endpoint on
 * @param aggregator Aggregator as parent for the bridged endpoint
 *
 * @return Bridged endpoint
 *
 */
esp_matter::endpoint_t *create_bridged_endpoint(esp_matter::node_t *node, esp_matter::endpoint_t *aggregator);

} // namespace bridge_node
} // namespace endpoint
} // namespace metahouse
#endif // BRIDGE_NODE_HPP