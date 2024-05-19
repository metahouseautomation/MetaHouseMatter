#include "root_node.hpp"
#include "checker.hpp"

namespace metahouse::endpoint::root_node {
esp_matter::node_t *create(config_t *config)
{
    esp_matter::node_t *node = esp_matter::node::create_raw();
    _CHECK_NULL_RETURN(node, "Failed to create the root node", nullptr);

    esp_matter::endpoint_t *endpoint =
        esp_matter::endpoint::create(node, esp_matter::endpoint_flags::ENDPOINT_FLAG_NONE, nullptr);
    _CHECK_NULL_RETURN(endpoint, "Failed to create the endpoint", nullptr);

    esp_err_t err = esp_matter::endpoint::add_device_type(endpoint, DEVICE_TYPE_ID, DEVICE_TYPE_VERSION);
    _CHECK_ERROR_RETURN(err, "Failed to add the device type root node", nullptr);

    esp_matter::cluster_t *descriptor_cluster = esp_matter::cluster::descriptor::create(
        endpoint, &(config->descriptor), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
    _CHECK_NULL_RETURN(descriptor_cluster, "Failed to create the descriptor cluster", nullptr);

    esp_matter::cluster_t *access_control_cluster = esp_matter::cluster::access_control::create(
        endpoint, &(config->access_control), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
    _CHECK_NULL_RETURN(access_control_cluster, "Failed to create the access control cluster", nullptr);

    esp_matter::cluster_t *basic_information_cluster = esp_matter::cluster::basic_information::create(
        endpoint, &(config->basic_information), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
    _CHECK_NULL_RETURN(basic_information_cluster, "Failed to create the basic information cluster", nullptr);

    esp_matter::cluster_t *general_commissioning_cluster = esp_matter::cluster::general_commissioning::create(
        endpoint, &(config->general_commissioning), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
    _CHECK_NULL_RETURN(general_commissioning_cluster, "Failed to create the general commissioning cluster", nullptr);

    esp_matter::cluster_t *network_commissioning_cluster = esp_matter::cluster::network_commissioning::create(
        endpoint, &(config->network_commissioning), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
    _CHECK_NULL_RETURN(network_commissioning_cluster, "Failed to create the network commissioning cluster", nullptr);

    esp_matter::cluster_t *general_diagnostics_cluster = esp_matter::cluster::general_diagnostics::create(
        endpoint, &(config->general_diagnostics), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
    _CHECK_NULL_RETURN(general_diagnostics_cluster, "Failed to create the general diagnostics cluster", nullptr);

    esp_matter::cluster_t *administrator_commissioning_cluster =
        esp_matter::cluster::administrator_commissioning::create(endpoint, &(config->administrator_commissioning),
                                                                 esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
    _CHECK_NULL_RETURN(administrator_commissioning_cluster, "Failed to create the administrator commissioning cluster",
                       nullptr);

    esp_matter::cluster_t *operational_credentials_cluster = esp_matter::cluster::operational_credentials::create(
        endpoint, &(config->operational_credentials), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
    _CHECK_NULL_RETURN(operational_credentials_cluster, "Failed to create the operational credentials cluster",
                       nullptr);

    esp_matter::cluster_t *group_key_management_cluster =
        esp_matter::cluster::group_key_management::create(endpoint, esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
    _CHECK_NULL_RETURN(group_key_management_cluster, "Failed to create the group key management cluster", nullptr);

#if CHIP_DEVICE_CONFIG_ENABLE_WIFI
    esp_matter::cluster_t *diagnostics_network_wifi_cluster = esp_matter::cluster::diagnostics_network_wifi::create(
        endpoint, &(config->diagnostics_network_wifi), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
    _CHECK_NULL_RETURN(diagnostics_network_wifi_cluster, "Failed to create the diagnostics network wifi cluster",
                       nullptr);
#endif

#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
    esp_matter::cluster_t *diagnostics_network_thread_cluster = esp_matter::cluster::diagnostics_network_thread::create(
        endpoint, &(config->diagnostics_network_thread), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
    _CHECK_NULL_RETURN(diagnostics_network_thread_cluster, "Failed to create the diagnostics network thread cluster",
                       nullptr);
#endif

#if CHIP_DEVICE_CONFIG_ENABLE_ETHERNET
    // esp_matter::cluster_t *diagnostics_network_ethernet_cluster =
    //     esp_matter::cluster::diagnostics_network_ethernet::create(endpoint, &(config->diagnostics_network_ethernet),
    //                                                               esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
    // _CHECK_NULL_RETURN(diagnostics_network_ethernet_cluster, "Failed to create the diagnostics network ethernet
    // cluster",
    //              nullptr);
#endif

#if CHIP_CONFIG_ENABLE_ICD_SERVER
    esp_matter::cluster_t *icd_management_cluster = esp_matter::cluster::icd_management::create(
        endpoint, &(config->icd_management), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER,
        esp_matter::cluster::icd_management::feature::check_in_protocol_support::get_id());
    _CHECK_NULL_RETURN(icd_management_cluster, "Failed to create the icd management cluster", nullptr);
#endif

    return node;
}
} // namespace metahouse::endpoint::root_node