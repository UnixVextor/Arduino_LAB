#include <Arduino_FreeRTOS.h>
#include "queue.h"

#define RED     7
#define YELLOW  8
#define GREEN   9
#define sw1     2
#define sw2     3
#define sw3     4
int count = 0;
unsigned long pastTime = 0;
bool pressedsw2 = 0;
bool pressedsw3 = 0;
int Round = 0;
QueueHandle_t ledque;

void setup()
{
  Serial.begin(9600);
  ledque = xQueueCreate(10, sizeof(int32_t));
  xTaskCreate(sender, "sender1", 100, sw1, 1, NULL);
  xTaskCreate(sender, "sender2", 100, sw2, 1, NULL);
  xTaskCreate(sender, "sender3", 100, sw3, 1, NULL);
  xTaskCreate(reciever, "recieverRed", 100, NULL, 1, NULL);
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

    if (sw == 2 && digitalRead(sw) == 1)
    {
      valueToSend = sw;
    }
    else if (digitalRead(sw) == 0)
    {
      valueToSend = 0;
    }

    if (sw == 3 && digitalRead(sw) == 1)
    {
      valueToSend = sw;
    }
    else if (digitalRead(sw) == 0)
    {
      valueToSend = 0;
    }

    if (sw == 4 && digitalRead(sw) == 1)
    {
      valueToSend = sw;
    }
    else if (digitalRead(sw) == 0)
    {
      valueToSend = 0;
    }
    if (valueToSend != 0) {
      qStatus = xQueueSend(ledque, &valueToSend, xTicksToWait);
      vTaskDelay(10);
    }
  }
}

void reciever(void *pvParameters)
{
  int32_t valueReceived;
  unsigned long timeMax = 0;
  unsigned long passYellow = 0;
  unsigned long passGreen = 0;
  bool greenpressed = false;
  int countGreen = 0;
  BaseType_t qStatus;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  while (1)
  {
    qStatus = xQueueReceive(ledque, &valueReceived, xTicksToWait);
    if (qStatus == pdPASS) {
      Serial.println(valueReceived);
      if (valueReceived == sw1)
      {
        count++;
        timeMax = (3000 * count);
        digitalWrite(RED, HIGH);
      }

      if (valueReceived == sw2)
      {
        pressedsw2 = !pressedsw2;
        Serial.println(pressedsw2);
      }
      if (valueReceived == sw3 && greenpressed == false)
      {
        greenpressed = true;
      }
    }
    if (greenpressed == true && millis() - passGreen >= 500) {
      digitalWrite(GREEN, !digitalRead(GREEN));
      Serial.println("Check");
      countGreen++;
      passGreen = millis();
    }
    if (countGreen > 5) {
      countGreen = 0;
      greenpressed = false;
    }
    if (count > 0)
    {
      if (millis() - pastTime >= timeMax)
      {
        digitalWrite(RED, LOW);
        count = 0;
        pastTime = millis();
      }
    }
    if (pressedsw2 == 1)
    {
      if (millis() - passYellow >= 100) {
        digitalWrite(YELLOW, !digitalRead(YELLOW));
        passYellow = millis();
      }
    }
    else if (pressedsw2 == 0)
    {
      digitalWrite(YELLOW, LOW);
    }
  }
  vTaskDelay(10);
}

void loop() {}
