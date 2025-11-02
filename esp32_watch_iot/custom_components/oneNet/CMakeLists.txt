idf_component_register(SRCS "mqtt.c" 
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES wifi_manager mqtt json dht11
                    REQUIRES)
