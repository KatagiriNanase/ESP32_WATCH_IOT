idf_component_register(SRCS "dht11.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES driver freertos esp_rom soc log)
