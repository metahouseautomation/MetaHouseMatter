#ifndef CALLBACK_EVENT_HPP
#define CALLBACK_EVENT_HPP

#include <app/server/CommissioningWindowManager.h>
#include <app/server/Server.h>
#include <core/CHIPError.h>
#include <credentials/FabricTable.h>
#include <cstdint>
#include <esp_matter.h>
#include <esp_matter_attribute_utils.h>
#include <esp_matter_identify.h>
#include <hal/gpio_types.h>
#include <platform/CHIPDeviceEvent.h>

// Main namespace for the project
namespace metahouse {
// Namespace for application callback events
namespace callback_event {

/** event
 * @brief Event handler for CHIP device events
 *
 * @param event CHIP device event
 * @param arg Argument for the event
 *
 */
void event(const chip::DeviceLayer::ChipDeviceEvent *event, intptr_t arg);

/** identification
 * @brief Identification callback event
 *
 * @param type Identification callback type
 * @param endpoint_id Endpoint ID
 * @param effect_id Effect ID
 * @param effect_variant Effect variant
 * @param priv_data Private data
 *
 */
esp_err_t identification(esp_matter::identification::callback_type_t type, uint16_t endpoint_id, uint8_t effect_id,
                         uint8_t effect_variant, void *priv_data);

/** attribute_update
 * @brief Attribute update callback event
 *
 * @param type Attribute update callback type
 * @param endpoint_id Endpoint ID
 * @param cluster_id Cluster ID
 * @param attribute_id Attribute ID
 * @param val Attribute value
 * @param priv_data Private data
 *
 */
esp_err_t attribute_update(esp_matter::attribute::callback_type_t type, uint16_t endpoint_id, uint32_t cluster_id,
                           uint32_t attribute_id, esp_matter_attr_val_t *val, void *priv_data);

/** enable_factory_reset
 * @brief Enable factory reset callback event
 *
 * @param reset_pin Reset pin
 * @param reset_hold_time_s Reset hold time in seconds
 *
 */
void enable_factory_reset(gpio_num_t reset_pin, uint16_t reset_hold_time_s);
} // namespace callback_event
} // namespace metahouse

#endif // CALLBACK_EVENT_HPP