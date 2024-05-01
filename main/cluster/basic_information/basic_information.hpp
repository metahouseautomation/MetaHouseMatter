#ifndef BASIC_INFORMATION_HPP
#define BASIC_INFORMATION_HPP

#include <cstdint>
#include <esp_matter.h>
#include <esp_matter_core.h>

namespace metahouse {
namespace cluster {

namespace basic_information {
constexpr uint32_t CLUSTER_ID = chip::app::Clusters::BasicInformation::Id;
constexpr uint8_t MAX_NAME_LENGTH = 32;

typedef struct config {
    uint16_t cluster_revision; // mandatory
    char node_label[MAX_NAME_LENGTH + 1]; // mandatory

    uint16_t data_model_revision; // mandatory
    char vendor_name[MAX_NAME_LENGTH + 1]; // mandatory
    uint16_t vendor_id; // mandatory
    char product_name[MAX_NAME_LENGTH + 1]; // mandatory
    uint16_t product_id; // mandatory
    char location[MAX_NAME_LENGTH + 1]; // mandatory
    uint16_t hardware_version; // mandatory
    char hardware_version_string[MAX_NAME_LENGTH + 1]; // mandatory
    uint16_t software_version; // mandatory
    char software_version_string[MAX_NAME_LENGTH + 1]; // mandatory
    // char manufacturing_date[MAX_NAME_LENGTH + 1];
    // char part_number[MAX_NAME_LENGTH + 1];
    // char product_url[MAX_NAME_LENGTH + 1];
    // char product_label[MAX_NAME_LENGTH + 1];
    // char serial_number[MAX_NAME_LENGTH + 1];
    // bool local_config_disabled;
    // bool reachable;
    // char unique_id[MAX_NAME_LENGTH + 1];
    // uint8_t product_appearance;

    config(uint16_t cluster_revision, uint16_t data_model_revision, const char *vendor_name, uint16_t vendor_id,
           const char *product_name, uint16_t product_id, const char *node_label, const char *location,
           uint16_t hardware_version, const char *hardware_version_string, uint16_t software_version,
           const char *software_version_string)
        : cluster_revision(cluster_revision)
        , data_model_revision(data_model_revision)
        , vendor_id(vendor_id)
        , product_id(product_id)
        , hardware_version(hardware_version)
        , software_version(software_version)
    {
        strncpy(this->vendor_name, vendor_name, MAX_NAME_LENGTH);
        strncpy(this->product_name, product_name, MAX_NAME_LENGTH);
        strncpy(this->node_label, node_label, MAX_NAME_LENGTH);
        strncpy(this->location, location, MAX_NAME_LENGTH);
        strncpy(this->hardware_version_string, hardware_version_string, MAX_NAME_LENGTH);
        strncpy(this->software_version_string, software_version_string, MAX_NAME_LENGTH);
    }

} config_t;

esp_matter::cluster_t *create(esp_matter::endpoint_t *endpoint, config_t *config, uint8_t flags);
} // namespace basic_information
} // namespace cluster
} // namespace metahouse

#endif // BASIC_INFORMATION_HPP