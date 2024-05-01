#include <esp_matter.h>
#include <esp_matter_cluster.h>

#include "bridged_device_basic_information.hpp"
#include "checker.hpp"
#include "esp_matter_attribute.h"

namespace metahouse::clusters::bridged_device_basic_information {

esp_matter::cluster_t *create(esp_matter::endpoint_t *endpoint, config_t *config, esp_matter::cluster_flags_t flags)
{
    esp_matter::cluster::bridged_device_basic_information::config_t default_config;
    esp_matter::cluster_t *bridged_device_basic_information_cluster =
        esp_matter::cluster::bridged_device_basic_information::create(endpoint, &(default_config),
                                                                      esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
    _CHECK_NULL_RETURN(bridged_device_basic_information_cluster,
                       "Failed to create the bridged device basic information cluster", nullptr);

    esp_matter::cluster::bridged_device_basic_information::attribute::create_node_label(
        bridged_device_basic_information_cluster, config->node_label, strlen(config->node_label));

    return bridged_device_basic_information_cluster;
}

} // namespace metahouse::clusters::bridged_device_basic_information