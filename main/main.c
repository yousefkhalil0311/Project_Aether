#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

void app(void *pvParameter)
{
    while(1){
        printf("Start App1 now\n");
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    vTaskDelete(NULL);
}

void app2(void *pvParameter)
{
    while(1){
        printf("Start App2 now\n");
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    vTaskDelete(NULL);
}

void app3(void *pvParameter)
{
    while(1){
        printf("Start App3 now\n");
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    vTaskDelete(NULL);
}

void app4(void *pvParameter)
{
    while(1){
        printf("Start App4 now\n");
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    vTaskDelete(NULL);
}

void app_main(void)
{
    xTaskCreate(app, "appTask", 8192, NULL, 2, NULL);
    xTaskCreate(app2, "appTask2", 8192, NULL, 3, NULL);
    xTaskCreate(app3, "appTask3", 8192, NULL, 4, NULL);
    xTaskCreate(app4, "appTask4", 8192, NULL, 5, NULL);
}

