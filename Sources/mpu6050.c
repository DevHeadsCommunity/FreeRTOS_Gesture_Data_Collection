#include "mpu6050.h"

/**
 * @brief Writes a value to a specified register.
 *
 * @param reg The address of the target register.
 * @param d   The data value to be written into the register.
 *
 * @return void
 */

static void MPU_WriteReg(uint8_t reg, uint8_t d)
{
    uint8_t Data[] = {reg, d};        // Keeping Register Address and Data in a single array [reg_addr, data]
    I2C1_GenStart();                  // Generating start condition in I2C bus
    I2C1_SendAddress(MPU_ADDRESS, 0); // Sending Address
    I2C1_MasterSendData(Data, 2);     // Sending formated data [reg_addr, data]
    I2C1_GenStop();                   // Giving Stop signal to end the communication
}

/**
 * @brief Returns the Chip ID
 *
 * @param none
 *
 * @return uint8_t Chip ID of MPU6050
 */

uint8_t MPU_Whoami()
{
    uint8_t data = 0x75;                           // WHOAMI Register Address
    uint8_t check;                                 // Variable for the Return value
    I2C1_GenStart();                               // Generating start condition in I2C bus
    I2C1_SendAddress(MPU_ADDRESS, 0);              // Sending Address
    I2C1_MasterSendData(&data, sizeof(data));      // Sending WHOAMI reg Address
    I2C1_GenStart();                               // Repeated start
    I2C1_SendAddress(MPU_ADDRESS, 1);              // Sending Address with Raed Flag
    I2C1_MasterReceiveData(&check, sizeof(check)); // Storing the value into check after reading
    return check;                                  // returning the check value
}

/**
 * @brief Initializes the MPU6050 for Measurement
 *
 * @param none
 *
 * @return void
 */
void MPU_Init()
{
    MPU_WriteReg(MPU_REG_PWR_MGMT_1, 0); // Setting all the bit to 0 so that sensor wakeup

    MPU_WriteReg(MPU_REG_SMPLRT_DIV, 0x7U); // Setting sampling rate to 7 to make sampling rate to 1KHz

    MPU_WriteReg(MPU_REG_ACCEL_CONFIG, 0x0U); // Setting Scale Range to 250degree/s

    MPU_WriteReg(MPU_REG_GYRO_CONFIG, 0x0U); // Setting Scale Range to 2g
}

/**
 * @brief Writes Accelerometer Data into Given array
 *
 * @param arr a preallocated float array of length 3
 *
 * @return void
 */
void MPU_Read_Accel(float *arr)
{
    int16_t RawAccelX, RawAccelY, RawAccelZ; // Varaiable to Store Raw data We Read From the sensor after formating

    uint8_t reg = MPU_REG_ACCEL_XOUT_H; // storing register address in a vairable to send it by reference
    uint8_t AccelData[6];               // Array to store 6 byte unsigned Accel data from MPU6050

    I2C1_GenStart();                      // Generating start condition in I2C bus
    I2C1_SendAddress(MPU_ADDRESS, 0);     // sending Address with Write Flag
    I2C1_MasterSendData(&reg, 1);         // Writing the the register address for MPU_REG_ACCEL_XOUT_H *1* is the length of the data in byte
    I2C1_GenStart();                      // Repeated start
    I2C1_SendAddress(MPU_ADDRESS, 1);     // sending Address with Read Flag
    I2C1_MasterReceiveData(AccelData, 6); // Doing Brust Read of 6 byte for better measurement

    // Processing RAW data for uint8_t to int16_t since 2 byte = 1 Axis data X= arr[0] and arr[1]
    RawAccelX = (int16_t)(AccelData[0] << 8 | AccelData[1]);
    RawAccelY = (int16_t)(AccelData[2] << 8 | AccelData[3]);
    RawAccelZ = (int16_t)(AccelData[4] << 8 | AccelData[5]);

    /*Doing the Final Calculation and Storing the Data
    since we are using full range scaling 2g so our scale factor is 16384
    Accel_n = RAW_Axix / Scale factor
    */
    arr[0] = (float)(RawAccelX / 16384.0);
    arr[1] = (float)(RawAccelY / 16384.0);
    arr[2] = (float)(RawAccelZ / 16384.0);
}

/**
 * @brief Writes Gyroscope Data into Given array
 *
 * @param arr a preallocated float array of length 3
 *
 * @return void
 */
void MPU_Read_Gyro(float *arr)
{
    int16_t RawGyroX, RawGyroY, RawGyroZ; // Varaiable to Store Raw data We Read From the sensor after formating

    uint8_t reg = MPU_REG_GYRO_XOUT_H; // storing register address in a vairable to send it by reference

    uint8_t GyroData[6]; // Array to store 6 byte unsigned Gyro data from MPU6050

    I2C1_GenStart();                     // Generating start condition in I2C bus
    I2C1_SendAddress(MPU_ADDRESS, 0);    // sending Address with Write Flag
    I2C1_MasterSendData(&reg, 1);        // Writing the the register address for MPU_REG_GYRO_XOUT_H *1* is the length of the data in byte
    I2C1_GenStart();                     // Repeated start
    I2C1_SendAddress(MPU_ADDRESS, 1);    // sending Address with Read Flag
    I2C1_MasterReceiveData(GyroData, 6); // Doing Brust Read of 6 byte for better measurement

    // Processing RAW data for uint8_t to int16_t since 2 byte = 1 Axis data X= arr[0] and arr[1]
    RawGyroX = (int16_t)(GyroData[0] << 8 | GyroData[1]);
    RawGyroY = (int16_t)(GyroData[2] << 8 | GyroData[3]);
    RawGyroZ = (int16_t)(GyroData[4] << 8 | GyroData[5]);

    /*
    Doing the Final Calculation and Storing the Data
    since we are using full range scaling 250 deg/s so our scale factor is 131
    Gyro_n = RAW_Axix / Scale factor
    */
    arr[0] = (float)RawGyroX / 131.0;
    arr[1] = (float)RawGyroY / 131.0;
    arr[2] = (float)RawGyroZ / 131.0;
}