#ifndef CHECKER_HPP
#define CHECKER_HPP

#include <esp_err.h>
#include <esp_log.h>

#define _CHECK_ERROR_RETURN(err_m, msg_m, ret_m) \
    do {                                         \
        if (err_m != ESP_OK) {                   \
            ESP_LOGE(__FILE__, msg_m);           \
            return ret_m;                        \
        }                                        \
    } while (0)

#define _CHECK_ERROR_(err_m, msg_m)    \
    do {                               \
        if (err_m != ESP_OK) {         \
            ESP_LOGE(__FILE__, msg_m); \
        }                              \
    } while (0)

#define _CHECK_NULL_RETURN(ptr_m, msg_m, ret_m) \
    do {                                        \
        if (ptr_m == nullptr) {                 \
            ESP_LOGE(__FILE__, msg_m);          \
            return ret_m;                       \
        }                                       \
    } while (0)

#define _CHECK_NULL_(ptr_m, msg_m)     \
    do {                               \
        if (ptr_m == nullptr) {        \
            ESP_LOGE(__FILE__, msg_m); \
        }                              \
    } while (0)

#endif // CHECKER_HPP