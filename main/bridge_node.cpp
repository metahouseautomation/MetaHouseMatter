#include <esp_err.h>

#include "bridge_node.hpp"
#include "checker.hpp"

namespace metahouse::endpoint::bridge_node {

esp_matter::endpoint_t *create_bridged_endpoint(esp_matter::node_t *node, esp_matter::endpoint_t *aggregator)
{
    _CHECK_NULL_RETURN(node, "Node is null", nullptr);
    _CHECK_NULL_RETURN(aggregator, "Aggregator is null", nullptr);

    esp_matter::endpoint_t *endpoint = esp_matter::endpoint::create(
        node, esp_matter::endpoint_flags::ENDPOINT_FLAG_BRIDGE | esp_matter::endpoint_flags::ENDPOINT_FLAG_DESTROYABLE,
        nullptr);
    _CHECK_NULL_RETURN(endpoint, "Failed to create the endpoint", nullptr);

    esp_err_t err = esp_matter::endpoint::add_device_type(endpoint, DEVICE_TYPE_ID, DEVICE_TYPE_VERSION);
    _CHECK_ERROR_RETURN(err, "Failed to add the device type", nullptr);

    err = esp_matter::endpoint::set_parent_endpoint(endpoint, aggregator);
    _CHECK_ERROR_RETURN(err, "Failed to set the parent endpoint", nullptr);

    return endpoint;
}
} // namespace metahouse::endpoint::bridge_node