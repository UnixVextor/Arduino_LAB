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
SemaphoreHandle_t  MutexA, MutexB;

void setup() {
  Serial.begin(9600);
  ledque = xQueueCreate(10, sizeof(int32_t));
  xTaskCreate(sender, "sender1", 100, SW1, 1, NULL);
  xTaskCreate(sender, "sender2", 100, SW2, 1, NULL);
  xTaskCreate(sender, "sender3", 100, SW3, 1, NULL);
  xTaskCreate(taskRED, "recieverRed", 100, NULL, 1, NULL);
  xTaskCreate(taskYELLOW, "recieverRed", 100, NULL, 1, NULL);
  xTaskCreate(taskGREEN, "recieverRed", 100, NULL, 1, NULL);
  MutexA = xSemaphoreCreateBinary();
  MutexB = xSemaphoreCreateBinary();
  xSemaphoreGive(MutexA);
  xSemaphoreGive(MutexB);
}

void sender (void *pvParameters) {
  BaseType_t qstatus;
  int32_t valueToSend = 0;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  uint16_t *sw = pvParameters;
  pinMode(sw, INPUT_PULLUP);
  while (1) {
   if (sw == SW1 && !digitalRead(sw) == 1)
    {
      valueToSend = sw;
    }
    else if (!digitalRead(sw) == 0)
    {
      valueToSend = 0;
    }

    if (sw == SW2 && !digitalRead(sw) == 1 && xQueuePeek(MutexA, nullptr,0) && xQueuePeek(MutexB,nullptr,0))
    {
      valueToSend = sw;
    }
    else if (!digitalRead(sw) == 0)
    {
      valueToSend = 0;
    }

    if (sw == SW3 && !digitalRead(sw) == 1 && xQueuePeek(MutexA, nullptr,0))
    {
      valueToSend = sw;
    }
    else if (!digitalRead(sw) == 0)
    {
      valueToSend = 0;
    }
    if (valueToSend) {
      qstatus = xQueueSend(ledque, &valueToSend, xTicksToWait);
      vTaskDelay(10);
    }
  }
}

void taskRED(void *pvParameters) {
  int32_t valueReceived;
  BaseType_t qstatus;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  bool REDstate = false;
  bool LEDpress = false;
  unsigned long pastTime = 0;
  pinMode(RED, OUTPUT);
  digitalWrite(RED, REDstate);
  while (1) {
    qstatus = xQueueReceive(ledque, &valueReceived, xTicksToWait);
    if (qstatus == pdPASS) {
      if (valueReceived == SW1) {
        Serial.println("RED");
        LEDpress = !LEDpress;
        digitalWrite(RED, !REDstate);
      }
    }
    
    if (LEDpress && millis() - pastTime >= 3000) {
      Serial.println("JJJ");
      digitalWrite(RED, REDstate);
      LEDpress = false;
      pastTime = millis();
    }

    
    if (LEDpress == false) {
      digitalWrite(RED, REDstate);
    }
    
    if(digitalRead(RED))
    {
      xSemaphoreTake(MutexA,0);
    }
    else
    {
      xSemaphoreGive(MutexA);
    }
  }
}

void taskGREEN(void *pvParameters) {
  int32_t valueReceived;
  unsigned long pastTime = 0;
  BaseType_t qstatus;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  int LEDstate = 0;
  bool LEDpress = false;
  pinMode(GREEN, OUTPUT);
  digitalWrite(GREEN, LEDstate);
  while (1) {
    qstatus = xQueueReceive(ledque, &valueReceived, xTicksToWait);
    if (qstatus == pdPASS) {
      if (valueReceived == SW3) {
        Serial.println("GREEN");
        LEDpress = !LEDpress;
        digitalWrite(GREEN, !LEDstate);
      }
    }
    if (LEDpress && millis() - pastTime >= 3000) {
      Serial.println("JJJ");
      digitalWrite(GREEN, LEDstate);
      LEDpress = false;
      pastTime = millis();
    }
    if (LEDpress == false) {
      digitalWrite(GREEN, LEDstate);
    }
    
    if(digitalRead(GREEN))
    {
      xSemaphoreTake(MutexB,0);
    }
    else
    {
      xSemaphoreGive(MutexB);
    }
  }
}

void taskYELLOW(void *pvParameters) {
  int32_t valueReceived;
  BaseType_t qstatus;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  int LEDstate = 0;
  pinMode(YELLOW, OUTPUT);
  digitalWrite(YELLOW, LEDstate);
  while (1) {
    qstatus = xQueueReceive(ledque, &valueReceived, xTicksToWait);
    if (qstatus == pdPASS) {
      if (valueReceived == SW2) {
        Serial.println("YELLOW");
        digitalWrite(YELLOW, !LEDstate);

        vTaskDelay(pdMS_TO_TICKS(500));
        digitalWrite(YELLOW, LEDstate);

        vTaskDelay(pdMS_TO_TICKS(500));
        digitalWrite(YELLOW, !LEDstate);

        vTaskDelay(pdMS_TO_TICKS(500));
        digitalWrite(YELLOW, LEDstate);
        vTaskDelay(pdMS_TO_TICKS(500));
      }
    }
  }
}

void loop() {}
