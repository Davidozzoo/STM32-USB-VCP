//USB-VCP example.
//This firmware reads data received from USB-VCP and sends it back through the USB-VCP.

#include "main.h"
#include "RCC_CLOCK.h"
#include "USB-VCP.h"

int main(void)
{
	uint32_t	ii=0;
	uint8_t		BUFF_TX_VCP[128] = {0};				//Virtual Serial Port Tansmission Buffer.
	uint8_t		BUFF_RX_VCP[128] = {0};				//Virtual Serial Port Reception Buffer.
	uint32_t	VCP_RX_LEN = 0;						//Variable to store the size of received packets.
	
	NVIC->ICER[0] = 0xFFFFFFFF;						//Turn off all interrupts.	

	ClockInit();									//SYSTEM CLOCK = 72Mhz. AHB CLOCK = 72Mhz. APB1 CLOCK = 36Mhz. APB2 CLOCK = 72Mhz. USB CLOCK = 48Mhz.  
	USBInit();
	
	for(ii=0;ii<5;ii++) {BUFF_TX_VCP[ii]=ii+1;}


	while(1)	{
		
		USB_MANAGEMENT();							//Function that manage the USB peripheral and USB control transfers on endpoint 0.
		
		if(VCP_Received)	{						//If a packet has been received..
			
			Read_VCP(BUFF_RX_VCP,&VCP_RX_LEN);		//Read the packet and store it in the reception buffer "BUFF_RX_VCP". The lenght of the packet will be written inside the variable VCP_RX_LEN.
			
			for(ii=0;ii<VCP_RX_LEN;ii++)	{		//Copy the received data to the transmission buffer "BUFF_TX_VCP".
				BUFF_TX_VCP[ii]=BUFF_RX_VCP[ii]; }
			
			Write_VCP(BUFF_TX_VCP,VCP_RX_LEN);		//Send the packet through the USB VCP.
			
			Wait_VCP_TX();	}						//Wait until the packet has been completely sent.
			
	}												//END While(1).

}													//END main().
