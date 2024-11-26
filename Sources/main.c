
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

int main(void)
{
    char buffer[] = "Hello, From UART!\n";
    UART2_GPIO_Init();
    USART2_Config();
    I2C1_GPIO_Init();
    I2C1_Init();
    // uint8_t res = MPU_Whoami();
    // (void)res;
    MPU_Init();
    MPU_Read_Accel();
    UART_SendBuffer(USART2, (uint8_t *)buffer, sizeof(buffer));
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
