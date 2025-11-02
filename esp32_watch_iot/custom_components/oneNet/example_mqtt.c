#include "mqtt.h"
#include "wifi_manager.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "esp_event.h"
#include "mqtt_client.h"
#include <cJSON.h>
#include "dht11.h"

#define PRODUCT_ID "YOUR_PRODUCT_ID"
#define PRODUCT_NAME "YOUR_PRODUCT_NAME"
#define MQTT_BROKER_URL "mqtt://mqtts.heclouds.com:1883"
#define PASSWORD "YOUR_PASSWORD"
static const char* TAG = "mqtt";

static esp_mqtt_client_handle_t mqtt_client;

/**
 *@brief 生成onenet 订阅set的topic
 *
 * @param url
 * @param size 必须大于等于40
 */
void onenet_concat_url_sub_set(char* url, uint32_t size, const char* product_id, const char* dev_name)
{
    //"$sys/{pid}/{device-name}/thing/property/set"
    snprintf(url, size, "$sys/%s/%s/thing/property/set", product_id, dev_name);
}


void onenet_concat_url_sub_post_reply(char* url, uint32_t size, const char* product_id, const char* dev_name)
{
    //"$sys/{pid}/{device-name}/thing/property/post/reply"
    snprintf(url, size, "$sys/%s/%s/thing/property/post/reply", product_id, dev_name);
}

/**
 *@brief 发布收到set后的reply
 *
 * @param url
 * @param size
 * @param product_id
 * @param dev_name
 * @param code
 */
void onenet_publish_set_reply(const char* product_id, const char* dev_name, int code, char* msg)
{
    //$sys/{pid}/{device-name}/thing/property/set_reply
    char topic[40] = { 0 };
    snprintf(topic, sizeof(topic), "$sys/%s/%s/thing/property/set_reply", product_id, dev_name);

    /* 生成回应的json */
  /*   {
        "id":"123",
            "code" : 200,
            "msg" : "xxxx"
    } */
    cJSON* root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "id", "123");
    cJSON_AddNumberToObject(root, "code", code);
    cJSON_AddStringToObject(root, "msg", msg);

    char* data = cJSON_PrintUnformatted(root);

    esp_mqtt_client_publish(mqtt_client, topic, data, strlen(data), 1, 0);

    cJSON_free(data);
    cJSON_Delete(root);
}

/**
 *@brief 生成属性上报所需的topic
 *
 * @param url
 * @param size
 * @param product_id
 * @param dev_name
 */
void onenet_concat_url_publish_post(char* url, uint32_t size, const char* product_id, const char* dev_name)
{
    //$sys/{pid}/{device-name}/thing/property/post
    snprintf(url, size, "$sys/%s/%s/thing/property/post", product_id, dev_name);
}

/**
 * @brief 创建发布属性所需的json
 *
 * @return cJSON*
 */
cJSON* onenet_generate_post_json(void)
{
    /*     {
            "id": "123",
                "version" : "1.0",
                "params" : {
                "room_temp": {
                    "value": 10,
                },
                    "humidity" : {
                    "value": 50,
                }
            }
        } */

    cJSON* json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "id", "123");

    cJSON* params = cJSON_CreateObject();

    cJSON* room_temp = cJSON_CreateObject();
    double temp = dth11_get_room_temp() / 10.0f;
    cJSON_AddNumberToObject(room_temp, "value", temp);
    cJSON* humidity = cJSON_CreateObject();
    cJSON_AddNumberToObject(humidity, "value", dht11_get_humidity());

    cJSON_AddItemToObject(params, "room_temp", room_temp);
    cJSON_AddItemToObject(params, "humidity", humidity);

    cJSON_AddItemToObject(json, "params", params);

    return json;
}

void onenet_property_post(const char* product_id, const char* dev_name)
{
    char topic[128] = { 0 };
    onenet_concat_url_publish_post(topic, sizeof(topic), PRODUCT_ID, PRODUCT_NAME);
    cJSON* json = onenet_generate_post_json();
    char* data = cJSON_PrintUnformatted(json);
    esp_mqtt_client_publish(mqtt_client, topic, data, strlen(data), 1, 0);

    /* 释放内存 */
    cJSON_free(data);
    cJSON_Delete(json);
}

static void mqtt_event_handler(void* handler_args, esp_event_base_t base, int32_t event_id, void* event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        /* 订阅onenet的set */
        char topic[128] = { 0 };
        onenet_concat_url_sub_set(topic, sizeof(topic), PRODUCT_ID, PRODUCT_NAME);
        esp_mqtt_client_subscribe_single(mqtt_client, topic, 1);
        memset(topic, 0, sizeof(topic));

        /* 订阅publish的reply */
        onenet_concat_url_sub_post_reply(topic, sizeof(topic), PRODUCT_ID, PRODUCT_NAME);
        esp_mqtt_client_subscribe_single(mqtt_client, topic, 1);
        memset(topic, 0, sizeof(topic));

        /* 发布属性 */
        onenet_property_post(PRODUCT_ID, PRODUCT_NAME);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;
    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        if (strstr(event->topic, "property/set"))
        {

        }
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

void onenet_post_property_task(void* param)
{
    while (1)
    {
        onenet_property_post(PRODUCT_ID, PRODUCT_NAME);
        vTaskDelay(pdMS_TO_TICKS(10 * 1000));
    }

}

void mqtt_init()
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = MQTT_BROKER_URL,
        .credentials.username = PRODUCT_ID,
        .credentials.client_id = PRODUCT_NAME,
        .credentials.authentication.password =PASSWORD
    };

    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);

    /* 确保wifi连接成功 */
    while (wifi_manager_is_connected() == false)
    {
        ;
    }
    esp_mqtt_client_start(mqtt_client);
    xTaskCreatePinnedToCore(onenet_post_property_task, "onenet_post_property_task", 4096, NULL, 5, NULL, APP_CPU_NUM);
}