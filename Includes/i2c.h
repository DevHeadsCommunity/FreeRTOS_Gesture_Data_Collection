#ifndef __INC_I2C_H__
#define __INC_I2C_H__

#include "stm32f446xx.h"

#define __RCC_I2C1_CLK_EN() (RCC->APB1ENR |= RCC_APB1ENR_I2C1EN)
#define __RCC_I2C2_CLK_EN() (RCC->APB1ENR |= RCC_APB1ENR_I2C2EN)
#define __RCC_I2C3_CLK_EN() (RCC->APB1ENR |= RCC_APB1ENR_I2C3EN)

void I2C1_GPIO_Init();
void I2C1_Init();
void I2C1_GenStart();
void I2C1_GenStop();
void I2C1_SetACK();
void I2C1_ResetACK();
void I2C1_PEnable();
void I2C1_PDisable();
void I2C1_SendAddress(uint8_t address, uint8_t rw);
void I2C1_MasterSendData(uint8_t *pBuffer, uint8_t len);
void I2C1_MasterReceiveData(uint8_t *pBuffer, uint8_t len);

#endif