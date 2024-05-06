/** IMPORTANT: change source file in connectedhomeip
 * connectedhomeip\connectedhomeip\src\app\clusters\door-lock-server\door-lock-server-callback.cpp
 *
bool __attribute__((weak)) emberAfPluginDoorLockOnDoorLockCommand(chip::EndpointId endpointId,
                                                                  const Nullable<chip::FabricIndex> & fabricIdx,
                                                                  const Nullable<chip::NodeId> & nodeId,
                                                                  const Optional<ByteSpan> & pinCode, OperationErrorEnum
& err)
{
    DoorLockServer::Instance().SetLockState(endpointId, DlLockState::kLocked);
    return true;
}

bool __attribute__((weak)) emberAfPluginDoorLockOnDoorUnlockCommand(chip::EndpointId endpointId,
                                                                    const Nullable<chip::FabricIndex> & fabricIdx,
                                                                    const Nullable<chip::NodeId> & nodeId,
                                                                    const Optional<ByteSpan> & pinCode,
OperationErrorEnum & err)
{
    DoorLockServer::Instance().SetLockState(endpointId, DlLockState::kUnlocked);
    return true;
}
 *
 **/

#ifndef DOOR_LOCK_HPP
#define DOOR_LOCK_HPP

#include <esp_matter.h>
#include <esp_matter_cluster.h>
#include <esp_matter_command.h>
#include <esp_matter_core.h>
#include <esp_matter_endpoint.h>

#include "accessories/door_accessory/door_accessory.hpp"

// Main namespace for the project
namespace metahouse {
// Namespace for application endpoints
namespace endpoint {
// Namespace for the door lock endpoint
namespace door_lock {
const uint32_t DEVICE_TYPE_ID = ESP_MATTER_DOOR_LOCK_DEVICE_TYPE_ID; // Door lock device type ID
const uint8_t DEVICE_TYPE_VERSION = ESP_MATTER_DOOR_LOCK_DEVICE_TYPE_VERSION; // Door lock device type version

// Configuration structure for the door lock
typedef struct config {
    esp_matter::cluster::bridged_device_basic_information::config_t
        bridged_device_basic_information; // used only if the device is a bridged device
    esp_matter::cluster::descriptor::config_t descriptor;
    esp_matter::cluster::identify::config_t identify;
    esp_matter::cluster::door_lock::config_t door_lock;
} config_t;

/** create
 * @brief Create the door lock
 *
 * @param node Node to create the door lock on
 * @param config Configuration for the door lock
 * @param aggregator Aggregator as parent for the door lock (optional)
 *
 * @return Door lock
 * @note If the aggregator is provided, the door lock will be created as bridged device on the aggregator
 *
 */
esp_matter::endpoint_t *create(esp_matter::node_t *node, config_t *config, esp_matter::endpoint_t *aggregator = nullptr,
                               DoorAccessory *priv_data = nullptr);

} // namespace door_lock
} // namespace endpoint
} // namespace metahouse
#endif // DOOR_LOCK_HPP