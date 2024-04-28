#include "callback_event.hpp"

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
    if (type == esp_matter::attribute::callback_type_t::POST_UPDATE) {
        if (priv_data) {
            // metahouse::endpoint::PrivateDataDelegate *private_data =
            // (metahouse::endpoint::PrivateDataDelegate *)priv_data;
            // private_data->attribute_update(endpoint_id, cluster_id, attribute_id,
            // val);
        }
    }

    return ESP_OK;
}
