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

int __io_putchar(int ch)
{
    UART_SendChar(USART2, ch);
    return ch;
}

void USART2_Config();

float GyroDataArr[3];

void pvAccelDataReading()
{

    for (;;)
    {
        MPU_Read_Gyro(GyroDataArr);
        printf("      Gx: %.4f\n\r      Gy: %.4f\n\r      Gz: %.4f\n\n\r", GyroDataArr[0], GyroDataArr[1], GyroDataArr[2]);
        vTaskDelay(500);
    }
}

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

    if (xTaskCreate(pvAccelDataReading,
                    "AccelDataReading",
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
