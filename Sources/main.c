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

void USART2_Config(); // Configuring UART to send data to the computer

float GyroDataArr[3];  // Array to Store Gyroscope Data [Gx, Gy, Gz]
float AccelDataArr[3]; // Array to Store Accelerometer Data [Gx, Gy, Gz]

void pvGyroDataReading();  // Task For Reading Gyroscope Data
void pvAccelDataReading(); // Task For Reading Accelerometer Data
void pvDataLogging();      // Task For Sending Data to the Computer

int main(void)
{
    UART2_GPIO_Init(); // Initializing GPIO Pins for USART2
    USART2_Config();   // Initializing USART2
    I2C1_GPIO_Init();  // Initializing GPIO Pins for I2C1
    I2C1_Init();       // Initializing I2C1

    if (MPU_Whoami() == 0x68) // Checking Device ID
    {
        printf("MPU Connected Successfully.\n"); // If it matches print this message
    }
    else
    {
        printf("Check Your Connections or Address.\n"); // If it doesn't match print this message
    }

    MPU_Init(); // Initializing The MPU6050 for reading Data

    /**
     * @brief: Creating Task for GyroData
     */
    if (xTaskCreate(pvGyroDataReading,
                    "pvGyroDataReading",
                    configMINIMAL_STACK_SIZE,
                    NULL,
                    configMAX_PRIORITIES - 1U,
                    NULL) != pdPASS)
    {
        printf("Task Creation Failed!\n");
    }

    /**
     * @brief: Creating Task for AccelData
     */
    if (xTaskCreate(pvAccelDataReading,
                    "AccelDataReading",
                    configMINIMAL_STACK_SIZE,
                    NULL,
                    configMAX_PRIORITIES - 1U,
                    NULL) != pdPASS)
    {
        printf("Task Creation Failed!\n");
    }

    /**
     * @brief: Creating Task for Data logging
     */
    if (xTaskCreate(pvDataLogging,
                    "pvDataLogging",
                    configMINIMAL_STACK_SIZE,
                    NULL,
                    configMAX_PRIORITIES - 1U,
                    NULL) != pdPASS)
    {
        printf("Task Creation Failed!\n");
    }

    vTaskStartScheduler(); // Starting the Scheduler

    for (;;)
    {
    }

    return 0;
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) // Stack Overflow Hook
{
    /* Print or log stack overflow for task debugging */

    for (;;)
        ; // Halt system to debug
}

void USART2_Config()
{
    UARTConfig_t uart2;                      // Creating UART Instance
    uart2.pUARTx = USART2;                   // Adding USART peripheral to the instance
    uart2.Init.BaudRate = 115200U;           // Configuring Baud Rate
    uart2.Init.Mode = UART_MODE_TX_ONLY;     // Configuring Mode/Direction
    uart2.Init.Parity = UART_PARITY_NONE;    // Configuring Parity Control
    uart2.Init.Parity = UART_WORD_LEN_8BITS; // Configuring Word length
    UART_Init(&uart2);
}

void pvGyroDataReading()
{

    for (;;)
    {
        MPU_Read_Gyro(GyroDataArr);     // Reading Gyroscoped Data
        vTaskDelay(pdMS_TO_TICKS(10)); // delay for 500ms
    }
}

void pvAccelDataReading()
{

    for (;;)
    {
        MPU_Read_Accel(AccelDataArr);   // Reading Accelerometer Data
        vTaskDelay(pdMS_TO_TICKS(10)); // delay for 500ms
    }
}

void pvDataLogging()
{
    for (;;)
    {
        printf("%.4f %.4f %.4f %.4f %.4f %.4f\n",
               AccelDataArr[0], AccelDataArr[1], AccelDataArr[2],
               GyroDataArr[0], GyroDataArr[1], GyroDataArr[2]); // Log Accelerometer Data[Ax Ay Az, Gx Gy Gz]
        vTaskDelay(pdMS_TO_TICKS(10));                         // Delay for 500ms
    }
}

int __io_putchar(int ch) // Overwriting for use printf via UART
{
    UART_SendChar(USART2, ch);
    return ch;
}
