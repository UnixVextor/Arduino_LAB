#include <Arduino_FreeRTOS.h> // tested on Uno
#include <task.h>
#include <semphr.h>

#define LED_ON     LOW
#define LED_OFF    HIGH
#define NUM_LEDS   (3)
#define NUM_ACTIVE_TASKS (2)
const byte LED_PINS[] = {7,8,9,};

TaskHandle_t taskHandles[ NUM_LEDS ];
SemaphoreHandle_t  semaphore;// task function prototypes

void task( void *pvParameters ); 
void sequencer(  void *pvParameters );

void setup() {
  Serial.begin(9600);
  char sbuf[4];
  int  priority = (tskIDLE_PRIORITY + 2);  
  randomSeed( analogRead(0) ); // initialize seed value  
  semaphore = xSemaphoreCreateCounting( 
                NUM_ACTIVE_TASKS, NUM_ACTIVE_TASKS );
  for ( int id=0; id < NUM_LEDS; id++ ) {
     sprintf( sbuf, "T%d", id );
     xTaskCreate(
       task, (const char *)sbuf, 60, 
       (void *)id, priority, &taskHandles[id] 
     );
  } 
}

void loop() {} // do nothings

void task( void *pvParameters ) {
   byte id = (int)pvParameters;
   byte ledPin = LED_PINS[id];
   pinMode( ledPin, OUTPUT );
   digitalWrite( ledPin, LED_OFF ); 
   while(1) { 
      if (xSemaphoreTake( semaphore, portMAX_DELAY )==pdTRUE){
         digitalWrite( ledPin, LED_ON );
         vTaskDelay( pdMS_TO_TICKS( random(500,1000) ) );
         digitalWrite( ledPin, LED_OFF );
         xSemaphoreGive( semaphore );
         taskYIELD();
      }
   }
}
