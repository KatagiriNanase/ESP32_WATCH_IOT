idf_component_register(SRCS "wifi_manager.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES esp_wifi  nvs_flash 
                    REQUIRES freertos log esp_netif)
