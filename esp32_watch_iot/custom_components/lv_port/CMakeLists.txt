idf_component_register(SRCS "lv_port.c" "cst816.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES freertos esp_timer esp_lcd driver log esp_common newlib
                    REQUIRES lvgl)
