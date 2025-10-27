idf_component_register(SRCS "time_manager.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES lwip
                    REQUIRES wifi_manager)
