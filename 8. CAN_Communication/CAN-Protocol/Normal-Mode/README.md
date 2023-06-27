# Communication-Between-STM32F407-Discovery-Kit-and-Arduino-UNO-using-CAN-Protocol

![CAN_Normal](https://user-images.githubusercontent.com/56625259/139848915-c17ec85b-61b3-45fc-ad2f-b631013c3dcc.jpg)

This project aims at establishing communication between an Arduino UNO and a STM32F407 Discovery Kit using the CAN protocol. The STM32F407 MCU has a in-built CAN controller,
but a CAN transceiver needs to be connected externally. The CAN transciever used here is SN65HVD230 module. A 120-ohm termination needs to be applied between CAN_H and CAN_L. This
also needs to be connected externally. The connections between the STM32F407 and the transceiver are as follows:

| STM32F407 | SN65HVD230 |
| ---- | ---- |
| 3V | VCC |
| GND | GND |
| PB8 | RX |
| PB9 | TX |

For the 120-ohm termination, connect the CAN_H from the SN65HVD230 module to a point on the breadboard. Then take a resistor or a combination of resistors that make up 120-ohms 
and connect the other end to CAN_L, which is brought onto the breadboard from the SN65HVD230 module.

For the Arduino, part the connections between the CAN Module and the Arduino UNO are as follows:

| Arduino UNO | CAN Module |
| ---- | ---- |
| 5V | VCC |
| GND | GND |
| CS | D10 |
| SO | D12 |
| SI | D11 |
| SCK | D13 |

The CAN breakout board has the 120-ohm termination which can be accessed by shorting the J1 jumper using a jumper wire.

The Directory named "Core" contains the files for the STM32F407 Discovery Board, and the directory named "Arduino" contains the files for the Arduino Uno Board.
