# **Gesture Data Collection with MPU-6050 and STM32 using FreeRTOS**

This project collects motion data from an MPU-6050 sensor using an STM32 microcontroller. The system leverages FreeRTOS to manage tasks like sensor data acquisition, processing, and communication. The collected data is intended for training gesture recognition models such as identifying gestures like circles or squares.

---

## **Features**
- **Real-Time Data Acquisition**: Captures accelerometer and gyroscope data from the MPU-6050.
- **Task Scheduling with FreeRTOS**:
  - Sensor reading task.
  - Data processing task (optional filtering or transformations).
  - Communication task to send collected data to a PC via UART/USB.
- **Flexible Data Collection**: Logs labeled motion data for offline analysis and model training.

---

## **Hardware Requirements**
1. **STM32 Microcontroller**: Any STM32 board with I2C and UART capabilities (e.g., STM32F4, STM32F103).
2. **MPU-6050 Sensor**: IMU sensor with a 3-axis accelerometer and gyroscope.
3. **USB-to-Serial Adapter**: For transmitting data to a PC.
4. **Power Supply**: USB or battery-powered STM32 board.

---

## **Software Requirements**
1. **Development Tools**: Sode IDE or code Editor
2. **FreeRTOS**: For real-time task management.
3. **Serial Monitoring Software**: Tools like PuTTY, Tera Term, or CoolTerm to log data on a PC.

---

## **Getting Started**
### 1. **Hardware Setup**
- **Connect the MPU-6050** to the STM32:
  - **I2C Pins**:
    - SDA → STM32 I2C SDA pin.
    - SCL → STM32 I2C SCL pin.
  - **Power Pins**:
    - VCC → 3.3V or 5V.
    - GND → Ground.
- Connect the STM32 UART pins to the USB-to-Serial Adapter.

### 2. **FreeRTOS Task Overview**
| Task Name            | Description                                      |
|----------------------|--------------------------------------------------|
| **SensorTask**       | Reads accelerometer and gyroscope data from MPU-6050. |
| **ProcessingTask**   | (Optional) Filters or processes raw data.        |
| **CommunicationTask**| Sends data to the PC via UART/USB.               |

---

## **Workflow**
1. **Set Up and Flash Firmware**:
   - Use STM32CubeMX to configure:
     - I2C for communicating with the MPU-6050.
     - UART for serial communication.
     - FreeRTOS for task scheduling.
   - Write code for the tasks listed above.
   - Compile and flash the code onto the STM32 board using STM32CubeIDE.

2. **Data Collection**:
   - Label gestures (e.g., "circle," "square") as you perform them.
   - The data format transmitted via UART will typically include:
     - Timestamp
     - Accelerometer (Ax, Ay, Az)
     - Gyroscope (Gx, Gy, Gz)
     - Gesture label (if labeling is manual or pre-defined in the code).
   - Example data output:
     ```
     100, 0.02, 0.03, 0.98, 0.4, 0.3, -0.1, circle
     110, 0.05, 0.02, 0.96, 0.3, 0.2, -0.2, square
     ```

3. **Logging Data**:
   - Use a serial monitor to log the transmitted data to a file (e.g., CSV format) for offline analysis.
---

## **Example Output**
| Timestamp (ms) | Ax (g) | Ay (g) | Az (g) | Gx (°/s) | Gy (°/s) | Gz (°/s) | Label  |
|----------------|--------|--------|--------|----------|----------|----------|--------|
| 0              | 0.01   | 0.02   | 0.98   | 0.5      | 0.3      | -0.1     | Circle |
| 10             | 0.02   | 0.03   | 0.95   | 0.4      | 0.4      | -0.2     | Square |

---

## **References**
- [MPU-6050 Datasheet](https://invensense.tdk.com/products/motion-tracking/6-axis/mpu-6050/)
- [STM32 FreeRTOS Guide](https://www.freertos.org/RTOS.html)
- [Serial Communication with STM32](https://www.st.com/en/microcontrollers-microprocessors/stm32.html)

