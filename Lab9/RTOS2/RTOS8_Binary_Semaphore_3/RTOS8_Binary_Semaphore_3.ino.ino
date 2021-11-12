#include <Arduino_FreeRTOS.h> // tested on Uno
#include <task.h>
#include <semphr.h>

#define LED_ON     LOW
#define LED_OFF    HIGH
#define NUM_LEDS   (8)
const byte LED_PINS[] = { 5,6,7,8,9,10,11,12 };

TaskHandle_t       taskHandles[ NUM_LEDS ];
SemaphoreHandle_t  semaphores[ NUM_LEDS ];// task function prototypes

void task( void *pvParameters ); 
void sequencer(  void *pvParameters );

void setup() {
  char sbuf[4];
  int  priority = (tskIDLE_PRIORITY + 2);
  for ( int id=0; id < NUM_LEDS; id++ ) {
     semaphores[id] = xSemaphoreCreateBinary();
     sprintf( sbuf, "T%d", id );
     xTaskCreate(
       task, (const char *)sbuf, 60, 
       (void *)id, priority, &taskHandles[id] 
     );
  }
  xTaskCreate( sequencer, "sequencer", 60, NULL, priority, NULL );  
  // Note the task scheduler is started automatically.  
}void loop() {} // do nothings

void sequencer( void *pvParameters ) {
  int index = 0;
  while (1) {
      xSemaphoreGive( semaphores[ index ] );
      index = (index+1) % NUM_LEDS;
      vTaskDelay( pdMS_TO_TICKS(100) );
  }
}

void task( void *pvParameters ) {
   int id = (int)pvParameters;
   int ledPin = LED_PINS[id];
   pinMode( ledPin, OUTPUT );
   digitalWrite( ledPin, LED_OFF );    while(1) { 
      if (xSemaphoreTake( semaphores[id], portMAX_DELAY )==pdTRUE){
         digitalWrite( ledPin, LED_ON );
         vTaskDelay( pdMS_TO_TICKS(100) );
         digitalWrite( ledPin, LED_OFF );
         taskYIELD();
      }
   }
}
