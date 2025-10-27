idf_component_register(SRCS "weather.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES wifi_manager 
                    REQUIRES esp_http_client)
