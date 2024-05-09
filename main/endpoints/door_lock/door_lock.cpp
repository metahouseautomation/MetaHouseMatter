#include "endpoints/door_lock/door_lock.hpp"
#include "checker.hpp"
#include "esp_log.h"
#include "esp_matter_attribute.h"
#include "esp_matter_core.h"
#include "app-common/zap-generated/ids/Attributes.h"
#include "endpoints/bridge_node/bridge_node.hpp"

namespace metahouse::endpoint::door_lock {
esp_matter::endpoint_t *create(esp_matter::node_t *node, config_t *config, esp_matter::endpoint_t *aggregator,
                               DoorAccessory *priv_data)
{
    _CHECK_NULL_RETURN(node, "Node is null", nullptr);
    esp_matter::endpoint_t *endpoint;
    // If the aggregator is not null, create a bridged endpoint
    if (aggregator != nullptr) {
        endpoint = metahouse::endpoint::bridge_node::create_bridged_endpoint(node, aggregator, priv_data);
        _CHECK_NULL_RETURN(endpoint, "Failed to create the bridged endpoint", nullptr);

        esp_matter::cluster_t *bridged_device_basic_information_cluster =
            esp_matter::cluster::bridged_device_basic_information::create(
                endpoint, &(config->bridged_device_basic_information), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
        _CHECK_NULL_RETURN(bridged_device_basic_information_cluster,
                           "Failed to create the bridged device basic information cluster", nullptr);
    } else { // If the aggregator is null, create a standalone endpoint
        endpoint = esp_matter::endpoint::create(node, esp_matter::endpoint_flags::ENDPOINT_FLAG_NONE, priv_data);
        _CHECK_NULL_RETURN(endpoint, "Failed to create the endpoint", nullptr);
    }

    priv_data->setEndpointId(esp_matter::endpoint::get_id(endpoint));

    esp_err_t err = esp_matter::endpoint::add_device_type(endpoint, DEVICE_TYPE_ID, DEVICE_TYPE_VERSION);
    _CHECK_ERROR_RETURN(err, "Failed to add the device type", nullptr);

    esp_matter::cluster_t *descriptor_cluster = esp_matter::cluster::descriptor::create(
        endpoint, &(config->descriptor), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
    _CHECK_NULL_RETURN(descriptor_cluster, "Failed to create the descriptor cluster", nullptr);

    esp_matter::cluster_t *identify_cluster = esp_matter::cluster::identify::create(
        endpoint, &(config->identify), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
    _CHECK_NULL_RETURN(identify_cluster, "Failed to create the identify cluster", nullptr);
    esp_matter::command_t *trigger_effect_command =
        esp_matter::cluster::identify::command::create_trigger_effect(identify_cluster);
    _CHECK_NULL_RETURN(trigger_effect_command, "Failed to create the trigger effect command", nullptr);

    esp_matter::cluster_t *door_lock_cluster = esp_matter::cluster::door_lock::create(
        endpoint, &(config->door_lock), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
    _CHECK_NULL_RETURN(door_lock_cluster, "Failed to create the door lock cluster", nullptr);

    esp_matter::cluster::door_lock::attribute::create_auto_relock_time(door_lock_cluster, 5000);

    esp_matter::attribute_t *lock_state_attribute =
        esp_matter::attribute::get(door_lock_cluster, chip::app::Clusters::DoorLock::Attributes::LockState::Id);

    esp_matter_attr_val_t val;
    esp_matter::attribute::get_val(lock_state_attribute, &val);

    val.val.u8 = priv_data->getState();

    esp_matter::attribute::set_val(lock_state_attribute, &val);

    return endpoint;
}
} // namespace metahouse::endpoint::door_lock