idf_component_register(SRCS "weather.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES wifi_manager json 
                    REQUIRES esp_http_client)
