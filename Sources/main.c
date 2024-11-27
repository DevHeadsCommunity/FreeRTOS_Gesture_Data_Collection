#include <stdio.h>
/*-------------FreeRTOS Includes--------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

/*-------------Custom Driver Includes--------------*/
#include "gpio.h"
#include "uart.h"
#include "i2c.h"
#include "mpu6050.h"

void USART2_Config();

float GyroDataArr[3];
float AccelDataArr[3];

void pvGyroDataReading();
void pvAccelDataReading();
void pvDataLogging();

int main(void)
{
    UART2_GPIO_Init();
    USART2_Config();
    I2C1_GPIO_Init();
    I2C1_Init();
    if (MPU_Whoami() == 0x68)
    {
        printf("MPU Connected Successfully.\n");
    }
    else
    {
        printf("Check Your Connections or Address.\n");
    }

    MPU_Init();

    if (xTaskCreate(pvGyroDataReading,
                    "pvGyroDataReading",
                    configMINIMAL_STACK_SIZE,
                    NULL,
                    configMAX_PRIORITIES - 1U,
                    NULL) != pdPASS)
    {
        printf("Task Creation Failed!\n");
    }

    if (xTaskCreate(pvAccelDataReading,
                    "AccelDataReading",
                    configMINIMAL_STACK_SIZE,
                    NULL,
                    configMAX_PRIORITIES - 1U,
                    NULL) != pdPASS)
    {
        printf("Task Creation Failed!\n");
    }
    if (xTaskCreate(pvDataLogging,
                    "pvDataLogging",
                    configMINIMAL_STACK_SIZE,
                    NULL,
                    configMAX_PRIORITIES - 1U,
                    NULL) != pdPASS)
    {
        printf("Task Creation Failed!\n");
    }

    vTaskStartScheduler();

    for (;;)
    {
    }

    return 0;
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    /* Print or log stack overflow for task debugging */

    for (;;)
        ; // Halt system to debug
}

void USART2_Config()
{
    UARTConfig_t uart2;
    uart2.pUARTx = USART2;
    uart2.Init.BaudRate = 115200U;
    uart2.Init.Mode = UART_MODE_TX_ONLY;
    uart2.Init.Parity = UART_PARITY_NONE;
    uart2.Init.Parity = UART_WORD_LEN_8BITS;
    UART_Init(&uart2);
}

void pvGyroDataReading()
{

    for (;;)
    {
        MPU_Read_Gyro(GyroDataArr);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void pvAccelDataReading()
{

    for (;;)
    {
        MPU_Read_Accel(AccelDataArr);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void pvDataLogging()
{
    for (;;)
    {
        printf("Ax: %.4f\n\rYy: %.4f\n\rAz: %.4f\n\r", AccelDataArr[0], AccelDataArr[1], AccelDataArr[2]);
        printf("Gx: %.4f\n\rGy: %.4f\n\rGz: %.4f\n\n\r", GyroDataArr[0], GyroDataArr[1], GyroDataArr[2]);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

int __io_putchar(int ch)
{
    UART_SendChar(USART2, ch);
    return ch;
}
