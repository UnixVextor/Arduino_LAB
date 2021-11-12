#include <Arduino_FreeRTOS.h>
#include <task.h>
void Task1( void *pvParameters );
void Task2( void *pvParameters );

TaskHandle_t TaskHandle_1; // handler for Task1
TaskHandle_t TaskHandle_2; // handler for Task2

void setup()
{
  Serial.begin(9600); // Enable serial communication library.

  xTaskCreate(Task1, "LED1", 100, NULL, 3, &TaskHandle_1);
  xTaskCreate(Task2, "LED2", 100, NULL, 2, &TaskHandle_2);
}

void loop(){}

//definition of Task1
void Task1(void* pvParameters)
{
  UBaseType_t uxPriority = uxTaskPriorityGet( NULL );
  while (1)
  {
    Serial.print("Task1 is running and about to raise Task2 Priority (");
    int TaskPriority = uxTaskPriorityGet(TaskHandle_2);
    Serial.print(TaskPriority);
    Serial.println(")");  
    vTaskPrioritySet( TaskHandle_2, ( uxPriority + 2 ) );
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second        
  }
}
void Task2(void* pvParameters)
{
  UBaseType_t   uxPriority = uxTaskPriorityGet( NULL );
  while (1)
  {
    Serial.print("Task2 is running and about to lower Task2 Priority (");
    int TaskPriority = uxTaskPriorityGet(TaskHandle_2);
    Serial.print(TaskPriority);
    Serial.println(")");  
    vTaskPrioritySet( TaskHandle_2, ( uxPriority - 2 ) );
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second       
  }
}
