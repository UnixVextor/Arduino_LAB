#include <Arduino_FreeRTOS.h>
#include "queue.h"
#include <semphr.h>

#define RED      7
#define YELLOW   8
#define GREEN    9
#define SW1     2
#define SW2     3
#define SW3     4

QueueHandle_t ledque;
SemaphoreHandle_t  semaphore_count;
SemaphoreHandle_t  semaphore;

bool REDon = false;
bool YELLOWon = false;
bool GREENon = false;

void setup() {
  Serial.begin(9600);
  ledque = xQueueCreate(10, sizeof(int32_t));
  semaphore_count = xSemaphoreCreateCounting(2,0);
  semaphore = xSemaphoreCreateBinary();
  xTaskCreate(sender, "sender1", 90, SW1, 1, NULL);
  xTaskCreate(sender, "sender2", 90, SW2, 1, NULL);
  xTaskCreate(sender, "sender3", 90, SW3, 1, NULL);
  xTaskCreate(taskRED, "recieverRed", 90, NULL, 1, NULL);
  xTaskCreate(taskYELLOW, "recieverRed", 90, NULL, 1, NULL);
  xTaskCreate(taskGREEN, "recieverRed", 90, NULL, 1, NULL);
}

void sender(void *pvParameters)
{
  BaseType_t qStatus;
  int32_t valueToSend = 0;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  uint16_t *sw = pvParameters;
  pinMode(sw, INPUT_PULLUP);
  while (1)
  {

    if (sw == 2 && digitalRead(sw) == 1 && (REDon == false || GREENon == true))
    {
      valueToSend = sw;
      xSemaphoreGive(semaphore); 
    }
    else if (digitalRead(sw) == 0)
    {
      valueToSend = 0;
    }

    if (sw == 4 && digitalRead(sw) == 1 && REDon == false)
    {
      valueToSend = sw;
      xSemaphoreGive(semaphore_count); 
    }
    else if (digitalRead(sw) == 0)
    {
      valueToSend = 0;
    }

    if (sw == 3 && digitalRead(sw) == 1 && REDon == false && GREENon == false)
    {
      valueToSend = sw;
      xSemaphoreGive(semaphore); 
    }
    else if (digitalRead(sw) == 0)
    {
      valueToSend = 0;
    }
    if (valueToSend != 0) {
      qStatus = xQueueSend(ledque, &valueToSend, 0);
      vTaskDelay(11);
    }
  }
}

void taskRED(void *pvParameters) {
  int32_t valueReceived;
  BaseType_t qstatus;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  bool REDstate = true;
  bool LEDpress = false;
  unsigned long pastTime = 0;
  pinMode(RED, OUTPUT);
  digitalWrite(RED, REDstate);
  while (1) {
    qstatus = xQueueReceive(ledque, &valueReceived, xTicksToWait);
    if (qstatus == pdPASS && (xSemaphoreTake(semaphore_count, portMAX_DELAY) == pdTRUE)){ //&& xSemaphoreTake(semaphore, portMAX_DELAY) == pdTRUE
      if (valueReceived == SW1) {
        Serial.println("RED");
        LEDpress = !LEDpress; 
        digitalWrite(RED, !REDstate);
        REDon = true;
        taskYIELD();
      }
    }
    if (LEDpress && millis() - pastTime >= 3000) {
      Serial.println("JJJ");
      digitalWrite(RED, REDstate);
      LEDpress = false;
      pastTime = millis();
      REDon = false;
    }
    if (LEDpress == false) {
      digitalWrite(RED, REDstate);
      REDon = false;
    }
  }
}

void taskGREEN(void *pvParameters) {
  int32_t valueReceived;
  unsigned long pastTime = 0;
  BaseType_t qstatus;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  int LEDstate = 1;
  bool LEDpress = false;
  pinMode(GREEN, OUTPUT);
  digitalWrite(GREEN, LEDstate);
  while (1) {
    qstatus = xQueueReceive(ledque, &valueReceived, xTicksToWait);
    if (qstatus == pdPASS && xSemaphoreTake(semaphore_count, portMAX_DELAY) == pdTRUE) { // && xSemaphoreTake(semaphore_count, portMAX_DELAY) == pdTRUE
      if (valueReceived == SW3) {
        Serial.println("GREEN");
        LEDpress = !LEDpress;
        digitalWrite(GREEN, !LEDstate);
        GREENon = true;
        taskYIELD();
      }
    }
    if (LEDpress && millis() - pastTime >= 3000) {
      Serial.println("JJJ");
      digitalWrite(GREEN, LEDstate);
      LEDpress = false;
      pastTime = millis();
      GREENon = false;
    }
    if (LEDpress == false) {
      digitalWrite(GREEN, LEDstate);
      GREENon = false;
    } 
  }
}

void taskYELLOW(void *pvParameters) {
  int32_t valueReceived;
  BaseType_t qstatus;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  int LEDstate = 1;
  pinMode(YELLOW, OUTPUT);
  digitalWrite(YELLOW, LEDstate);
  while (1) {
    qstatus = xQueueReceive(ledque, &valueReceived, xTicksToWait);
    if (qstatus == pdPASS && xSemaphoreTake(semaphore, portMAX_DELAY) == pdTRUE) { //&& xSemaphoreTake(semaphore, portMAX_DELAY) == pdTRUE
      if (valueReceived == SW2) {
        Serial.println("YELLOW");
        digitalWrite(YELLOW, !LEDstate);

        delay(500);
        digitalWrite(YELLOW, LEDstate);

        vTaskDelay(pdMS_TO_TICKS(500));
        delay(500);

        delay(500);
        digitalWrite(YELLOW, LEDstate);
        delay(500);
        taskYIELD();
      }
    }
  }
}


void loop() {}
