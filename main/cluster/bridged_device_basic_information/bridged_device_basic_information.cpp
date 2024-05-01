#include <esp_matter.h>
#include <esp_matter_attribute.h>
#include <esp_matter_cluster.h>
#include <esp_matter_core.h>

#include <app-common/zap-generated/callback.h>
#include <app/PluginApplicationCallbacks.h>

#include "checker.hpp"
#include "esp_matter_event.h"
#include "cluster/bridged_device_basic_information/bridged_device_basic_information.hpp"

#define CALL_ONCE(cb)                  \
    []() {                             \
        static bool is_called = false; \
        if (!is_called) {              \
            cb();                      \
            is_called = true;          \
        }                              \
    }

namespace metahouse::cluster::bridged_device_basic_information {

const esp_matter::cluster::function_generic_t *function_list = nullptr;
const int function_flags = esp_matter::cluster_flags::CLUSTER_FLAG_NONE;

esp_matter::cluster_t *create(esp_matter::endpoint_t *endpoint, config_t *config, uint8_t flags)
{
    esp_matter::cluster_t *cluster = esp_matter::cluster::create(endpoint, CLUSTER_ID, flags);
    _CHECK_NULL_RETURN(cluster, "Failed to create the bridged device basic information cluster", nullptr);

    esp_err_t err;

    if (flags & esp_matter::cluster_flags::CLUSTER_FLAG_SERVER) {
        err = esp_matter::cluster::add_function_list(cluster, function_list, function_flags);
        _CHECK_ERROR_RETURN(err, "Failed to add the function list", nullptr);
    }

    if (flags & esp_matter::cluster_flags::CLUSTER_FLAG_SERVER) {
        /* Attributes managed internally */
        esp_matter::cluster::global::attribute::create_feature_map(cluster, 0);
#if CHIP_CONFIG_ENABLE_EVENTLIST_ATTRIBUTE
        esp_matter::cluster::global::attribute::create_event_list(cluster, NULL, 0, 0);
#endif
        // esp_matter::cluster::bridged_device_basic_information::attribute::create_vendor_name(
        //     cluster, config->vendor_name, strlen(config->vendor_name));
        esp_matter::cluster::bridged_device_basic_information::attribute::create_vendor_id(cluster, config->vendor_id);
        esp_matter::cluster::bridged_device_basic_information::attribute::create_product_name(
            cluster, config->product_name, strlen(config->product_name));
        esp_matter::cluster::bridged_device_basic_information::attribute::create_hardware_version(
            cluster, config->hardware_version);
        // esp_matter::cluster::bridged_device_basic_information::attribute::create_hardware_version_string(
        //     cluster, config->hardware_version_string, strlen(config->hardware_version_string));
        esp_matter::cluster::bridged_device_basic_information::attribute::create_software_version(
            cluster, config->software_version);
        // esp_matter::cluster::bridged_device_basic_information::attribute::create_software_version_string(
        //     cluster, config->software_version_string, strlen(config->software_version_string));
        esp_matter::cluster::bridged_device_basic_information::attribute::create_reachable(cluster, config->reachable);

        esp_matter::cluster::bridged_device_basic_information::attribute::create_node_label(cluster, config->node_label,
                                                                                            strlen(config->node_label));
        esp_matter::cluster::global::attribute::create_cluster_revision(cluster, config->cluster_revision);
    }
    esp_matter::cluster::bridged_device_basic_information::event::create_reachable_changed(cluster);

    return cluster;
}
} // namespace metahouse::cluster::bridged_device_basic_information