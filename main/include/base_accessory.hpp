#ifndef BASE_ACCESSORY_HPP
#define BASE_ACCESSORY_HPP

#include <esp_err.h>
#include <esp_matter_attribute_utils.h>

class BaseAccessory {
public:
    BaseAccessory();
    ~BaseAccessory();
    virtual esp_err_t attribute_update(uint16_t endpoint_id, uint32_t cluster_id, uint32_t attribute_id,
                                       esp_matter_attr_val_t *val) = 0;
    virtual esp_err_t identification(uint16_t endpoint_id) = 0;
};

#endif // BASE_ACCESSORY_HPP