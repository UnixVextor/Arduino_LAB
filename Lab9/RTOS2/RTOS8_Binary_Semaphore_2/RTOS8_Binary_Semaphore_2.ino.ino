#include <Arduino_FreeRTOS.h> // tested on Uno
#include <task.h>
#include <semphr.h>

#define LED_ON     LOW
#define LED_OFF    HIGH
#define NUM_LEDS   (8)
const byte LED_PINS[] = { 5,6,7,8,9,10,11,12 };

TaskHandle_t taskHandles[ NUM_LEDS ];
SemaphoreHandle_t  semaphores[ NUM_LEDS ];

void task( void *pvParameters ); // task function prototype

void setup() {
  randomSeed( analogRead(0) );
  char sbuf[4];
  int  priority = (tskIDLE_PRIORITY + 2);
  for ( int id=0; id < NUM_LEDS; id++ ) {
     semaphores[id] = xSemaphoreCreateBinary();
     if ( id == 0 ) { 
        xSemaphoreGive( semaphores[0] ); 
     }
     sprintf( sbuf, "T%d", id );
     xTaskCreate(
       task, (const char *)sbuf, 60, 
       (void *)id, priority, &taskHandles[id] 
     );
  }
  // Note the task scheduler is started automatically.  
}

void loop() {} // do nothings

void task( void *pvParameters ) {
   int id = (int)pvParameters;
   int ledPin = LED_PINS[id];
   pinMode( ledPin, OUTPUT );
   digitalWrite( ledPin, LED_OFF );    
   vTaskDelay( pdMS_TO_TICKS( random(100,1000) ) );   
   while(1) { 
      if (xSemaphoreTake( semaphores[id], portMAX_DELAY )==pdTRUE){
         digitalWrite( ledPin, LED_ON );
         vTaskDelay( pdMS_TO_TICKS(100) );
         digitalWrite( ledPin, LED_OFF );
         xSemaphoreGive( semaphores[ (id+1) % NUM_LEDS ] );
         taskYIELD();
      }
   }
}
