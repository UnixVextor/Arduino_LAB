#include <Arduino_FreeRTOS.h>
#include <semphr.h>
SemaphoreHandle_t xCountingSemaphore;

#define LED 7

void setup()
{
  Serial.begin(9600); // Enable serial communication library.
  Serial.println( "\n\n\n\n" );  
  pinMode(LED, OUTPUT);

  // Create task for Arduino led
  xTaskCreate(Task1, "Ledon", 128, NULL, 0, NULL );
  xTaskCreate(Task2, "Ledoff", 128, NULL, 0, NULL );
  xCountingSemaphore = xSemaphoreCreateCounting(1, 1);
  xSemaphoreGive(xCountingSemaphore);
}

void loop() {}


void Task1(void *pvParameters)
{
  (void) pvParameters;
  for (;;)
  {
    xSemaphoreTake(xCountingSemaphore, portMAX_DELAY);
    Serial.println("Inside Task1 and Serial monitor Resource Taken");
    digitalWrite(LED, HIGH);
    xSemaphoreGive(xCountingSemaphore);
    vTaskDelay(1);
  }
}

void Task2(void *pvParameters)
{
  (void) pvParameters;
  for (;;)
  {
    xSemaphoreTake(xCountingSemaphore, portMAX_DELAY);
    Serial.println("Inside Task2 and Serial monitor Resource Taken");
    digitalWrite(LED, LOW);
    xSemaphoreGive(xCountingSemaphore);
    vTaskDelay(1);

  }
}
