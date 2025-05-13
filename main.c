#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include "freertos/queue.h"

QueueHandle_t queue1; // Create handle

void measureTime(void *pvParameters){
    while(1){
        static unsigned long startTime = 0;
        startTime += 1;
        xQueueSend(queue1, &startTime, 0);  // Send queue
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
void printTime(void *pvParameters){
    while(1){
        unsigned long buf;
        xQueueReceive(queue1, &buf, 0); // Receive queue
        printf("Time since start: ");
        printf("%ld\n",buf);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}


void app_main(void)
{
    esp_task_wdt_deinit();
    queue1 = xQueueCreate(1, sizeof(unsigned long));  // Create queue
    xTaskCreate(
        measureTime,    // Function name of the task
        "Measure Time", // Name of the task (e.g. for debugging)
        2048,           // Stack size (bytes)
        NULL,           // Parameter to pass
        1,              // Task priority
        NULL            // Task handle
    );
    xTaskCreate(
        printTime,    // Function name of the task
        "Print time", // Name of the task (e.g. for debugging)
        2048,         // Stack size (bytes)
        NULL,         // Parameter to pass
        1,            // Task priority
        NULL          // Task handle
    );
    while(1);
}
