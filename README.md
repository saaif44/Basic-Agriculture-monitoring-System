# Basic Agriculture Monitoring System

## Overview

This project focuses on monitoring and managing basic agriculture parameters using an Arduino UNO and an ESP8266 module. Below are the key steps for configuring the system:

## Configuration Steps

1. **Default Baud Rate**: The default UART (Universal Asynchronous Receiver-Transmitter) baud rate for the ESP8266 module is initially set to 9600 and recent firmware is in 115200.

2. **Baud Rate Adjustment**: To adapt the ESP8266 UART to 38400, follow these steps:
   - Connect your ESP8266 module to your computer.
   - Use a terminal program like CoolTerm.
   - Enter the command: `AT+UART_DEF=38400,8,1,0,0`
   - This command sets the UART configuration to 38400 baud, 8 data bits, 1 stop bit, no parity, and no flow control.

3. **Communication Verification**: Before making the baud rate adjustment, it's essential to ensure the existing communication is working as expected. Execute the following command:
   - `AT`
   - This command checks the ESP8266's response at the current baud rate (9600). A response of "OK" indicates successful communication.

## Usage

This README provides a brief overview of adjusting the ESP8266 baud rate for your project. For more detailed information and project implementation, please refer to the project documentation or source code in the repository.

## License

This project is available under the [MIT License](LICENSE).
