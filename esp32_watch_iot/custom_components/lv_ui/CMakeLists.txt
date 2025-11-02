file(GLOB_RECURSE srcs *.c
                    font/*.c
                    img/*.c
                    ui/*.c
                    ui_driver/*.c
                    )
 
set(include_dirs 
    font
    img
    ui
    ui_driver
    )
 
idf_component_register(
    SRCS ${srcs}
    INCLUDE_DIRS ${include_dirs}
    PRIV_REQUIRES main lvgl freertos time_manager wifi_manager dht11
    )
 