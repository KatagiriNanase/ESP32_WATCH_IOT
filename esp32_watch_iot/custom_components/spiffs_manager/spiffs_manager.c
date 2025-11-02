#include "spiffs_manager.h"
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"

static const char* TAG = "spiffs manager";

void spiffs_manager_init(void)
{
    esp_vfs_spiffs_conf_t config = {
        .base_path = "/img",
        .partition_label = NULL,
        .max_files = 10,
        .format_if_mount_failed = true,
    };

    esp_err_t ret = esp_vfs_spiffs_register(&config);

    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        else if (ret == ESP_ERR_NOT_FOUND)
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        else
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK)
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    else
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);

    /* The following calls demonstrate reading files from the generated SPIFFS
     * image. The images should contain the same files and contents as the spiffs_image directory.
     */

}
