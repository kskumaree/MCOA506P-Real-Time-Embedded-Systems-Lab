/***********************************************************************
 * The following is the code for the communication between STM32F407
 * and Arduino UNO using the CAN Protocol.
 * 
 * The connection are as follows:
 * CS   - D10
 * MOSI - D11
 * MISO - D12
 * SCK  - D13
***********************************************************************/

#include <SPI.h>
#include<mcp2515.h>

struct can_frame canMsg;
MCP2515 mcp2515(10);


void setup() {
  Serial.begin(115200);
  SPI.begin(); 
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS,MCP_8MHZ);
  mcp2515.setNormalMode();
  Serial.println("------- CAN Read ----------");
  Serial.println("ID  DLC   DATA");
}

void loop() {
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    Serial.print(canMsg.can_id, HEX); // print ID
    Serial.print(" "); 
    Serial.print(canMsg.can_dlc, DEC); // print DLC
    Serial.print(" ");
    
    for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
      Serial.print(canMsg.data[i],DEC);
      Serial.print(" ");    
    }
    Serial.println();
  }
}
