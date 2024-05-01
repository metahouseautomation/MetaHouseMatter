#include "endpoints/aggregator/aggregator.hpp"
#include "checker.hpp"
#include "esp_matter_cluster.h"

namespace metahouse::endpoint::aggregator {
esp_matter::endpoint_t *create(esp_matter::node_t *node, config_t *config)
{
    _CHECK_NULL_RETURN(node, "Node is null", nullptr);

    esp_matter::endpoint_t *endpoint =
        esp_matter::endpoint::create(node, esp_matter::endpoint_flags::ENDPOINT_FLAG_NONE, nullptr);
    _CHECK_NULL_RETURN(endpoint, "Failed to create the endpoint", nullptr);

    esp_err_t err = esp_matter::endpoint::add_device_type(endpoint, DEVICE_TYPE_ID, DEVICE_TYPE_VERSION);
    _CHECK_ERROR_RETURN(err, "Failed to add the device type", nullptr);

    esp_matter::cluster_t *descriptor_cluster = esp_matter::cluster::descriptor::create(
        endpoint, &(config->descriptor), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
    _CHECK_NULL_RETURN(descriptor_cluster, "Failed to create the descriptor cluster", nullptr);

    return endpoint;
}
} // namespace metahouse::endpoint::aggregator