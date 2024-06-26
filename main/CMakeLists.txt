
set(srcs
    "app_main.cpp"
    "callback_event.cpp"
)

list (APPEND srcs
    "accesspoint/url_handlers/accessories_handler.cpp"
    "accesspoint/url_handlers/command_handler.cpp"
    "accesspoint/url_handlers/file_read_handler.cpp"
    "accesspoint/url_handlers/not_found_handler.cpp"
    "accesspoint/url_handlers/wifi_handler.cpp"
    "accesspoint/webserver/webserver.cpp"
)

list (APPEND srcs
    "clusters/bridged_device_basic_information/bridged_device_basic_information.cpp"
)

list (APPEND srcs
    "accessories/light_accessory/light_accessory.cpp"
    "accessories/plugin_accessory/plugin_accessory.cpp"
    "accessories/switch_accessory/switch_accessory.cpp"
    "accessories/fan_accessory/fan_accessory.cpp"
    "accessories/window_accessory/window_accessory.cpp"
)
    
list (APPEND srcs
    "endpoints/aggregator/aggregator.cpp"
    "endpoints/bridge_node/bridge_node.cpp"
    "endpoints/door_lock/door_lock.cpp"
    "endpoints/fan/fan.cpp"
    "endpoints/generic_switch/generic_switch.cpp"
    "endpoints/on_off_light/on_off_light.cpp"
    "endpoints/on_off_plugin/on_off_plugin.cpp"
    "endpoints/root_node/root_node.cpp"
    "endpoints/window_covering/window_covering.cpp"
)

set(include_dirs "." "include")

list(APPEND include_dirs
    "accesspoint/webserver"
    "accesspoint/url_handlers"
)

list(APPEND include_dirs
    "clusters/bridged_device_basic_information"
)

list(APPEND include_dirs
    "accessories/base_accessory"
    "accessories/light_accessory"
    "accessories/plugin_accessory"
    "accessories/switch_accessory"
    "accessories/fan_accessory"
    "accessories/window_accessory"
)

list(APPEND include_dirs
    "endpoints/aggregator"
    "endpoints/bridge_node"
    "endpoints/door_lock"
    "endpoints/fan"
    "endpoints/generic_switch"
    "endpoints/on_off_light"
    "endpoints/on_off_plugin"
    "endpoints/root_node"
    "endpoints/window_covering"
)

idf_component_register(SRCS "${srcs}"
                      PRIV_INCLUDE_DIRS  "${include_dirs}")

set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 17)
target_compile_options(${COMPONENT_LIB} PRIVATE "-DCHIP_HAVE_CONFIG_H")