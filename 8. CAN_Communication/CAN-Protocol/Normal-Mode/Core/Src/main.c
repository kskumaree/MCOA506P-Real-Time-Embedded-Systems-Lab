/******************************************************************************
 * The following is a bare metal code written for CAN Normal Communication test
 * on the Arm Cortex-M4 based STM32F407 Discovery Kit.
 *
 * Configurations are made as follows:
 * PB8 - CAN1_RX (Need to be set in AF9 for CAN1/CAN2)
 * PB9 - CAN1_TX (Need to be set in AF9 for CAN1/CAN2)
 *
 *
 * @File   main.c
 *****************************************************************************/

#include "stm32f4xx.h"
#define ARM_MATH_CM4

void GPIO_Init(void);
void CAN1_Init(void);
void CAN1_Tx(uint8_t tr);
uint8_t CAN1_Rx(void);
void TIM4_ms_Delay(uint32_t delay);
uint8_t k = 0;
uint8_t rec = 0;

void GPIO_Init(){
	// Enable GPIOA clock signal (1UL<<1)
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	// Configuring PB8 and PB9 in alternate function mode (2UL<<(8*2) | 2UL<<(9*2))
	GPIOB->MODER |= (GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1);

	// Selecting AF9 for PB8 and PB9 (See Page 272 of dm00031020)
	// (8UL<<0 | 1UL<<0 | 1UL<<4 | 8UL<<4) = (9UL<<0 | 9UL<<4);
	GPIOB->AFR[1] |= (GPIO_AFRH_AFSEL8_3 | GPIO_AFRH_AFSEL8_0 |
					  GPIO_AFRH_AFSEL9_0 | GPIO_AFRH_AFSEL9_3);
}

void CAN1_Init(){
	/* 1. Setting Up the Baud Rate and Configuring CAN1 in
	 * Normal Mode -------------------------------------------------------*/

	// Enable clock for CAN1 (1UL<<25)
	RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;

	// Entering CAN Initialization Mode and wait for acknowledgment (1UL<<0)
	CAN1->MCR |= CAN_MCR_INRQ;
	while (!(CAN1->MSR & CAN_MSR_INAK)){}

	//Exit Sleep Mode (CAN1->MCR &= ~(1UL<<1))
	CAN1->MCR &= ~CAN_MCR_SLEEP;
	while (CAN1->MSR & CAN_MSR_SLAK){}

	//Set Loop back mode for CAN1
	//CAN1->BTR |= CAN_BTR_LBKM;

	//Setting the Re synchronization jump width to 1 (~(3UL<<24))
	CAN1->BTR &= ~CAN_BTR_SJW;

	//Setting the no. of time quanta for Time segment 2
	// TS2 = 4-1;
	CAN1->BTR &= ~(CAN_BTR_TS2); // ~(7UL<<20)
	CAN1->BTR |= (CAN_BTR_TS2_1 | CAN_BTR_TS2_0); // (2UL<<20 | 1UL<<20) = (3UL<<20)

	//Setting the no. of time quanta for Time segment 1
	// TS1 = 3-1;
	CAN1->BTR &= ~(CAN_BTR_TS1);
	CAN1->BTR |= (CAN_BTR_TS1_1); // (2UL<<16)

	//Setting the Baud rate Pre-scalar for CAN1
	// BRP[9:0] = 16-1
	CAN1->BTR |= ((16-1)<<0);

	/* 2. Configuring the Filters--------------------------------------------*/
	//Enter Filter Initialization mode to configure the filters (1UL<<0)
	CAN1->FMR |= CAN_FMR_FINIT;

	// Configuring the Number of Filters Reserved for CAN1
	// and also the start bank for CAN2
	CAN1->FMR |= 14<<8;

	// Select the single 32-bit scale configuration (1UL<<13)
	CAN1->FS1R |= CAN_FS1R_FSC13;

	// Set the receive ID
	CAN1->sFilterRegister[13].FR1 = 0x103<<21;

	// Select Identifier List mode
	CAN1->FM1R |= CAN_FM1R_FBM13; //(1UL<<13)

	//Activating filter 13
	CAN1->FA1R |= CAN_FA1R_FACT13; // (1UL<<13)

	//Exit filter Initialization Mode
	CAN1->FMR &= ~CAN_FMR_FINIT;

	/* 3. Setting up the Transmission----------------------------------------*/

	CAN1->sTxMailBox[0].TIR = 0;

	//Setting up the Std. ID
	CAN1->sTxMailBox[0].TIR = (0x245<<21);

	// We are only transmitting a single byte, so this register is set to 0.
	// It handles the data bytes from 4-7, from the range of 0-8
	CAN1->sTxMailBox[0].TDHR = 0;

	// Setting Data Length to 1 Byte.
	CAN1->sTxMailBox[0].TDTR = 1;

	// Exit the Initialization mode for CAN1
	// Wait until the INAK bit is cleared by hardware
	CAN1->MCR &= ~CAN_MCR_INRQ;
	while (CAN1->MSR & CAN_MSR_INAK){}

}

void CAN1_Tx(uint8_t tr){
	// Put the Data to be transmitted into Mailbox Data Low Register
	CAN1->sTxMailBox[0].TDLR = tr;

	// Request for Transmission
	CAN1->sTxMailBox[0].TIR |= 1;
}

uint8_t CAN1_Rx(){
	// Monitoring FIFO 0 message pending bits FMP0[1:0]
	while(!(CAN1->RF0R & 3)){}

	// Read the data from the FIFO 0 mailbox from Mailbox data low register
	uint8_t RxD = (CAN1->sFIFOMailBox[0].RDLR) & 0xFF;

	// Releasing FIFO 0 output mailbox
	CAN1->RF0R |= 1<<5;

	return RxD;
}

void TIM4_ms_Delay(uint32_t delay){
	RCC->APB1ENR |= 1<<2; //Start the clock for the timer peripheral
	TIM4->PSC = 16000-1; //Setting the clock frequency to 1kHz.
	TIM4->ARR = (delay); // Total period of the timer
	TIM4->CNT = 0;
	TIM4->CR1 |= 1; //Start the Timer
	while(!(TIM4->SR & TIM_SR_UIF)){} //Polling the update interrupt flag
	TIM4->SR &= ~(0x0001); //Reset the update interrupt flag
}

int main(void){
	GPIO_Init();
	CAN1_Init();
	while(1){
		CAN1_Tx(k);
		//rec = CAN1_Rx();
		k += 1;
		if (k>25)
			k = 0;
		TIM4_ms_Delay(1000);
	}
}
