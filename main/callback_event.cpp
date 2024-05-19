#include "callback_event.hpp"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_matter_attribute_utils.h"
#include "esp_matter_core.h"
#include <driver/gpio.h>
#include <iot_button.h>
#include "accessories/base_accessory/base_accessory.hpp"

static const char *TAG = "app_callback";
constexpr auto k_timeout_seconds = 300;

void metahouse::callback_event::event(const chip::DeviceLayer::ChipDeviceEvent *event, intptr_t arg)
{
    switch (event->Type) {
    case chip::DeviceLayer::DeviceEventType::kInterfaceIpAddressChanged:
        ESP_LOGI(TAG, "Interface IP Address changed");
        break;

    case chip::DeviceLayer::DeviceEventType::kCommissioningComplete:
        ESP_LOGI(TAG, "Commissioning complete");
        break;

    case chip::DeviceLayer::DeviceEventType::kFailSafeTimerExpired:
        ESP_LOGI(TAG, "Commissioning failed, fail safe timer expired");
        break;

    case chip::DeviceLayer::DeviceEventType::kCommissioningSessionStarted:
        ESP_LOGI(TAG, "Commissioning session started");
        break;

    case chip::DeviceLayer::DeviceEventType::kCommissioningSessionStopped:
        ESP_LOGI(TAG, "Commissioning session stopped");
        break;

    case chip::DeviceLayer::DeviceEventType::kCommissioningWindowOpened:
        ESP_LOGI(TAG, "Commissioning window opened");
        break;

    case chip::DeviceLayer::DeviceEventType::kCommissioningWindowClosed:
        ESP_LOGI(TAG, "Commissioning window closed");
        break;

    case chip::DeviceLayer::DeviceEventType::kFabricRemoved: {
        ESP_LOGI(TAG, "Fabric removed successfully");
        if (chip::Server::GetInstance().GetFabricTable().FabricCount() == 0) {
            chip::CommissioningWindowManager &commissionMgr =
                chip::Server::GetInstance().GetCommissioningWindowManager();
            constexpr auto kTimeoutSeconds = chip::System::Clock::Seconds16(k_timeout_seconds);
            if (!commissionMgr.IsCommissioningWindowOpen()) {
                /* After removing last fabric, this example does not remove the Wi-Fi
                 * credentials and still has IP connectivity so, only advertising on
                 * DNS-SD.
                 */
                CHIP_ERROR err = commissionMgr.OpenBasicCommissioningWindow(
                    kTimeoutSeconds, chip::CommissioningWindowAdvertisement::kDnssdOnly);
                if (err != CHIP_NO_ERROR) {
                    ESP_LOGE(TAG, "Failed to open commissioning window, err:%" CHIP_ERROR_FORMAT, err.Format());
                }
            }
        }
        break;
    }

    case chip::DeviceLayer::DeviceEventType::kFabricWillBeRemoved:
        ESP_LOGI(TAG, "Fabric will be removed");
        break;

    case chip::DeviceLayer::DeviceEventType::kFabricUpdated:
        ESP_LOGI(TAG, "Fabric is updated");
        break;

    case chip::DeviceLayer::DeviceEventType::kFabricCommitted:
        ESP_LOGI(TAG, "Fabric is committed");
        break;
    default:
        break;
    }
}

esp_err_t metahouse::callback_event::identification(esp_matter::identification::callback_type_t type,
                                                    uint16_t endpoint_id, uint8_t effect_id, uint8_t effect_variant,
                                                    void *priv_data)
{
    esp_err_t err = ESP_OK;

    switch (type) {
    case esp_matter::identification::callback_type_t::START:
        ESP_LOGI(TAG,
                 "Identification start for endpoint_id %d, effect_id %d, "
                 "effect_variant %d",
                 endpoint_id, effect_id, effect_variant);

        //        if (!priv_data)
        //            err = ((metahouse::endpoint::PrivateDataDelegate
        //            *)priv_data)->identification(endpoint_id);
        break;
    case esp_matter::identification::callback_type_t::STOP:
        ESP_LOGI(TAG,
                 "Identification stop for endpoint_id %d, effect_id %d, "
                 "effect_variant %d",
                 endpoint_id, effect_id, effect_variant);
        break;
    case esp_matter::identification::callback_type_t::EFFECT:
        ESP_LOGI(TAG,
                 "Identification effect for endpoint_id %d, effect_id %d, "
                 "effect_variant %d",
                 endpoint_id, effect_id, effect_variant);
        break;
    default:
        break;
    }
    return err;
}

esp_err_t metahouse::callback_event::attribute_update(esp_matter::attribute::callback_type_t type, uint16_t endpoint_id,
                                                      uint32_t cluster_id, uint32_t attribute_id,
                                                      esp_matter_attr_val_t *val, void *priv_data)
{
    if (type == esp_matter::attribute::callback_type_t::PRE_UPDATE) {
        if (priv_data != nullptr) {
            BaseAccessory *accessory = static_cast<BaseAccessory *>(priv_data);
            if (accessory != nullptr) {
                return accessory->attribute_update(endpoint_id, cluster_id, attribute_id, val);
            }
        }

        return ESP_OK;
    }
    return ESP_OK;
}

void metahouse::callback_event::enable_factory_reset(gpio_num_t reset_pin, uint16_t reset_hold_time_s)
{
    reset_hold_time_s = (reset_hold_time_s * 1000);
    gpio_set_direction(reset_pin, GPIO_MODE_INPUT);
    button_config_t config = {.type = BUTTON_TYPE_GPIO,
                              .long_press_time = reset_hold_time_s,
                              .gpio_button_config = {
                                  .gpio_num = reset_pin,
                                  .active_level = 1,
                              }};
    button_handle_t handle = iot_button_create(&config);
    iot_button_register_cb(
        handle, BUTTON_LONG_PRESS_START,
        [](void *button_handle, void *usr_data) {
            ESP_LOGI(TAG, "Factory reset triggered");
            esp_matter::factory_reset();
        },
        nullptr);
}

void metahouse::callback_event::enable_restart_device(gpio_num_t restart_pin)
{
    gpio_set_direction(restart_pin, GPIO_MODE_INPUT);
    button_config_t config = {.type = BUTTON_TYPE_GPIO,
                              .gpio_button_config = {
                                  .gpio_num = restart_pin,
                                  .active_level = 1,
                              }};
    button_handle_t handle = iot_button_create(&config);
    iot_button_register_cb(
        handle, BUTTON_SINGLE_CLICK,
        [](void *button_handle, void *usr_data) {
            ESP_LOGI(TAG, "Restart device triggered");
            esp_restart();
        },
        nullptr);
}