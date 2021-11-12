#include <Arduino_FreeRTOS.h> // tested on Uno
#include <task.h>
#include <semphr.h>

#define LED_ON     LOW
#define LED_OFF    HIGH
#define NUM_LEDS   (3)

const byte LED_PINS[] = {7,8,9};

TaskHandle_t       taskHandles[ NUM_LEDS ];
SemaphoreHandle_t  mutexes[ NUM_LEDS ];
SemaphoreHandle_t  sem;

char acquired[ NUM_LEDS ]; // chopsticks// task function prototypes
void task( void *pvParameters );
void monitor( void *pvParameters ); 

void setup() {
  Serial.begin( 9600 );
  Serial.println( "\n\n\n\n" );
  randomSeed( analogRead(0) );  
  sem = xSemaphoreCreateBinary(); // used for the monitor task
  for ( int id = 0; id < NUM_LEDS; id++ ) {
    mutexes[id] = xSemaphoreCreateMutex();
    acquired[id] = '-'; // free
  }

  int priority = (tskIDLE_PRIORITY + 1);
  char name[4];
  for ( int id = 0; id < NUM_LEDS; id++ ) {
    sprintf( name, "T%d", id );
    xTaskCreate( // create a task (philosopher)
      task, (const char *)name, 80,
      (void *)id, priority, &taskHandles[id]
    );
  }
  xTaskCreate( monitor, "Monitor", 80,
               NULL, priority + 1, NULL );
} 

void loop() {} 

void monitor( void *pvParameters ) { // monitor function
  char sbuf[ NUM_LEDS + 1 ];
  memset( sbuf, '-', NUM_LEDS );
  sbuf[ NUM_LEDS ] = '\0';
  boolean flag = false;
  while (1) {
    if (flag || xSemaphoreTake( sem,800) == pdTRUE) {
      flag = false;
      for (int i = 0; i < NUM_LEDS; i++) {
        sbuf[i] = acquired[i];
      }
      Serial.println( sbuf );
      Serial.flush();
    } else {
      flag = true;
    }
  }
} 

void task( void *pvParameters ) { // task function
  byte id = (int)pvParameters;
  byte ledPin = LED_PINS[id];
  byte l = id, r = (id + 1) % NUM_LEDS;
  pinMode( ledPin, OUTPUT );
  digitalWrite( ledPin, LED_OFF );

  while (1) {
    // thinking
    vTaskDelay( pdMS_TO_TICKS( random(100, 500) ) );
    if (xSemaphoreTake(mutexes[l], portMAX_DELAY) == pdTRUE) {
      // acquired the left chopstick
      acquired[l] = '0' + id;
      xSemaphoreGive( sem );          // attempting to acquire the right chopstick
      if (xSemaphoreTake(mutexes[r], portMAX_DELAY) == pdTRUE) {
        // acquired the right chopstick
        acquired[r] = '0' + id;
        xSemaphoreGive( sem );                      // eating (turn on LED)
        digitalWrite( ledPin, LED_ON );
        vTaskDelay( pdMS_TO_TICKS( 200 ) );
        digitalWrite( ledPin, LED_OFF );           // release the right chopstick
        xSemaphoreGive( mutexes[r] );
        acquired[r] = '-';
        xSemaphoreGive( sem );

      }         // release the left chopstick
      xSemaphoreGive( mutexes[l] );
      acquired[l] = '-';
      xSemaphoreGive( sem );
    }
  }
}
