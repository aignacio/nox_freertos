/* Standard includes. */
#include <string.h>
#include <unistd.h>

/* Kernel includes. */
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>

#include "riscv_csr_encoding.h"
#include "nox_soc.h"
#include "uart.h"

void main( void );

static void print_task_1(void *pvParameters){
  uint8_t test = 0;
  while(1){
    uart0_printf("\nTask - 1 / Test: %d",test);
    test += 2;
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

static void print_task_2(void *pvParameters){
  uint8_t test = 0;
  while(1){
    uart0_printf("\nTask - 2 / Test: %d",test);
    test += 4;
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

static void print_task_3(void *pvParameters){
  uint8_t test = 0;
  while(1){
    uart0_printf("\nTask - 3 / Test: %d",test);
    test += 10;
    vTaskDelay(pdMS_TO_TICKS(300));
  }
}

static void print_task_4(void *pvParameters){
  uint8_t test = 0;
  while(1){
    uart0_printf("\nTask - 4 / Test: %d",test);
    test += 10;
    vTaskDelay(pdMS_TO_TICKS(600));
  }
}

void main(void){
  BaseType_t xReturned;
  TaskHandle_t xHandle1 = NULL,
               xHandle2 = NULL,
               xHandle3 = NULL,
               xHandle4 = NULL;

  xReturned = xTaskCreate(
    print_task_1,
    "T1",
    configMINIMAL_STACK_SIZE*2U,
    NULL,
    tskIDLE_PRIORITY+1,
    &xHandle1);

  xReturned = xTaskCreate(
    print_task_2,
    "I2",
    configMINIMAL_STACK_SIZE*2U,
    NULL,
    tskIDLE_PRIORITY+1,
    &xHandle2);

  xReturned = xTaskCreate(
    print_task_3,
    "T3",
    configMINIMAL_STACK_SIZE*2U,
    NULL,
    tskIDLE_PRIORITY+1,
    &xHandle3);

  xReturned = xTaskCreate(
    print_task_4,
    "T4",
    configMINIMAL_STACK_SIZE*2U,
    NULL,
    tskIDLE_PRIORITY+1,
    &xHandle4);

  vTaskStartScheduler();
  for(;;);
}
