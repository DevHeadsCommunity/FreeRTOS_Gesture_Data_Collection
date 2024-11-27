#include "i2c.h"

void I2C1_GPIO_Init()
{
    // Enabling Clock for GPIOB
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    GPIOB->MODER |= GPIO_MODER_MODE8_1 | GPIO_MODER_MODE9_1;
    GPIOB->OTYPER |= GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9;
    GPIOB->PUPDR |= GPIO_PUPDR_PUPD8_0 | GPIO_PUPDR_PUPD9_0;
    GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9;
    GPIOB->AFR[1] |= (4U << GPIO_AFRH_AFSEL8_Pos) | (4U << GPIO_AFRH_AFSEL9_Pos);
}

void I2C1_PEnable()
{
    I2C1->CR1 |= I2C_CR1_PE;
}
void I2C1_PDisable()
{
    I2C1->CR1 &= ~I2C_CR1_PE;
}

void I2C1_Init()
{
    // Enabling Clock for I2C1
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    // Resetting I2C
    I2C1->CR1 |= I2C_CR1_SWRST;
    I2C1->CR1 &= ~I2C_CR1_SWRST;

    // Disabling I2C1 Peripheral
    I2C1_PDisable();

    // Setting Frequency in I2C_CR2
    I2C1->CR2 |= 16U << I2C_CR2_FREQ_Pos;

    // Setting CCR value
    I2C1->CCR = 80;

    // Setting Rise Time
    I2C1->TRISE = 16 + 1;

    I2C1_SetACK();

    I2C1_PEnable();
}

/**
 * @brief: Generating Start Condition
 */
void I2C1_GenStart()
{
    I2C1->CR1 |= I2C_CR1_START;

    while (!(I2C1->SR1 & I2C_SR1_SB))
        ;
}

/**
 * @brief: Generating Stop Condition
 */
void I2C1_GenStop()
{
    I2C1->CR1 |= I2C_CR1_STOP;
}

/**
 * @brief: Disbling ACK
 */
void I2C1_SetACK()
{
    I2C1->CR1 |= I2C_CR1_ACK;
}

/**
 * @brief: Disbling ACK
 */
void I2C1_ResetACK()
{
    I2C1->CR1 &= ~I2C_CR1_ACK;
}

/**
 * @brief: Sending Address with R/W bit
 */
void I2C1_SendAddress(uint8_t address, uint8_t rw)
{
    I2C1->DR = ((address << 1U) | rw);
    while (!(I2C1->SR1 & I2C_SR1_ADDR))
        ;
    (void)I2C1->SR1;
    (void)I2C1->SR2;
}

/**
 * @brief: Sending Data
 */
void I2C1_MasterSendData(uint8_t *pBuffer, uint8_t len)
{
    while (len--)
    {
        I2C1->DR = (*pBuffer);
        pBuffer++;
        while (!(I2C1->SR1 & I2C_SR1_TXE))
            ;
    }
}

/**
 * @brief: Receiving Data
 */
void I2C1_MasterReceiveData(uint8_t *pBuffer, uint8_t len)
{
    I2C1_SetACK();
    while (len)
    {
        if (len == 1)
        {
            I2C1_ResetACK();
        }
        while (!(I2C1->SR1 & I2C_SR1_RXNE))
            ;
        *pBuffer = I2C1->DR;
        pBuffer++;
        len--;
    }
    I2C1_GenStop();
}