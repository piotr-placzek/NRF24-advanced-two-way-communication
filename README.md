# NRF24-advanced-two-way-communication
A simple project realizing two-way communication for Arduino using NRF24 modules with an example implementation.

## Basics
The project allows to create a structure of devices: one MASTER, many SLAVEs. Communication between the master device and the slave device is based on a system of questions and answers between appropriately addressed devices.

## Communication protocol
Each frame is divided into three basic blocks:
1. a header,
2. a data block beginning with the address of the desired device,
3. ending the frame.

#### Content of communication frame elements
- The header always has the form **0xA5 0x5A**,
- The data block depends on the role of the transmitting device:
    - For the **master** device it contains **only the address of the slave device** (the whole frame has 4 bytes),
    - For a **slave** device, the data block starts with the **address of this device**, then it contains **one byte with information about the number of consecutive bytes of data**, and the declared number of **these bytes of data**,
- The end of the frame always has the form **0xED**.

#### Example:
Address of the slave device: 0x11.
The slave device sends two bytes of data with the value 0xFF in response.
> MASTER -> SLAVE [0xA5 0x5A 0x10 0xED]

> MASTER <- SLAVE *nothing*

> MASTER -> SLAVE [0xA5 0x5A 0x11 0xED]

> MASTER <- SLAVE [0xA5 0x5A 0x11 0x02 0xFF 0xFF 0xED]

## Sketch initialization
1. To specify the device role, define `DEV_ROLE` as 1 for the master device or as 0 for the slave device.
    - Do it in 35th line of sketch file.
2. If the device works as a master, define the maximum waiting time for the response from the slave device. Define the `MASTER_RCV_WAITING_TIME` time value expressed in milliseconds.
    - Do it in 39th line of sketch file.
3. If the device works as a slave, define its address as `PROTOCOL_ADDRESS` in line 13 of the `SlaveDeviceController` object.
4. If the device works as a slave, define the amount of data returned in the response frame as `DATA_COUNT` in line 16 of the `SlaveDeviceController` object. Remember that this value should also include the byte with the address of this device. For example, if the device in the response contains only one byte of significant data, the length (DATA_COUNT) should be 2, because DATA [0] == device address, DATA [1] = your data.
5. In the debug mode, the device sends various messages via the serial port. To disable debug mode, remove the definition `DEBUGMODE_ON` or change it to `DEBUGMODE_OFF`.
    - Do it in 31th line of sketch file.

## Wiring
NRF24 pin 01 <-> GND

NRF24 pin 02 <-> VCC (max 3.3V)

NRF24 pin 03 <-> ARDUINO UNO pin 10 (chip enable)

NRF24 pin 04 <-> ARDUINO UNO pin 09 (chip select)

NRF24 pin 05 <-> ARDUINO UNO pin 13 (SPI SCK)

NRF24 pin 06 <-> ARDUINO UNO pin 11 (SPI MOSI)

NRF24 pin 07 <-> ARDUINO UNO pin 12 (SPI MISO)

NRF24 pin 08 <-> N/U (IRQ)
