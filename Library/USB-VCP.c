#include "USB-VCP.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Global variables definitions.

volatile uint8_t 	USB_RECEIVED_EP0 = 0;			//Variable set to 1 when a packet has been received on endpoint 0.
volatile uint8_t  	USB_TRANSMITTED_EP0 = 0;		//Variable set to 1 when a packet has been transmitted on endpoint 0.
volatile uint8_t  	USB_RECEIVED_EP1 = 0;			//Variable set to 1 when a packet has been received on endpoint 1.
volatile uint8_t  	USB_TRANSMITTED_EP1 = 0;		//Variable set to 1 when a packet has been transmitted on endpoint 1.
volatile uint8_t  	USB_RECEIVED_EP2 = 0;			//Variable set to 1 when a packet has been received on endpoint 2.
volatile uint8_t  	USB_TRANSMITTED_EP2 = 0;		//Variable set to 1 when a packet has been transmitted on endpoint 2.
volatile uint32_t  	N_JUMBO_SUBPACKETS = 0;			//Number of 32 byte subpackets of the Jumbo packet that have to be transmitted.
volatile uint32_t  	N_JUMBO_SUBPACKETS_TXED = 0;	//Number of 32 byte packets of the Jumbo packet already transmitted.
volatile uint8_t* 	JUMBO_BUFF_ADDR = 0;			//Pointer to the Jumbo packet.
volatile uint32_t 	JUMBO_LAST_SUBPACKET_SIZE = 0;	//Size of the last Jumbo subpacket.

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Descriptors definitions.

const uint8_t Device_Descriptor[18] = {				//Little Endian.
		0x12,										//Descriptor lenght = 18 byte.
		0x01,										//Descriptor type = Device.
		0x00,										//Desctiptor type USB 2.0.
		0x02,										//Desctiptor type USB 2.0.
		0x02,										//Class Communications and CDC Control.
		0x02,										//Subclass Abstract Control Model.
		0x00,										//Protocol No class specific protocol required.
		0x40,										//EP0 Max Packet Size 64 byte.
		0x83,										//Vendor ID 0483.
		0x04,										//Vendor ID 0483.
		0x40,										//Product ID 5740.
		0x57,										//Product ID 5740.
		0x00,										//bcdDevice 200.
		0x02,										//bcdDevice 200.
		0x00,										//iManufacter.
		0x00,										//iProduct.
		0x00,										//iSerialNumber.
		0x01   										//Number of possible configurations = 1.
};

const uint8_t Configuration_Descriptor[67] = {		//Little Endian.
		0x09,	//Descriptor lenght = 9 byte.
		0x02,  	//Descriptor Type = Configuration.
		0x43,  	//Total lenght of this configuration descriptor (included Interface_Descriptor end Endpoint_Descriptor) = 67.
		0x00,  	//Total lenght of this configuration descriptor (included Interface_Descriptor end Endpoint_Descriptor) = 67.
		0x02,  	//Number of interfaces that belong to this configuration = 2.
		0x01,  	//Index of this configuration = 1.
		0x00,  	//iConfiguration.
		0xC0,  	//Self/Powered.
		0x32,  	//Maximum current consumption = 100 mA.
		/////////////Interface_Descriptor///////////
		0x09,	//Descriptor lenght = 9 byte.
		0x04,	//Descriptor Type = Interface.
		0x00,	//Interface number = 0.
		0x00,	//Alternative interface = 0.
		0x01,	//Endopoints used by this interface = 1.
		0x02,	//Class Communications and CDC Control.
		0x02,	//Subclass Abstract Control Model.
		0x01,	//Protocol AT Commands: V.250 etc.
		0x00,	//iInterface.
		/////////////Class/Specific_Descriptor///////////
		0x05,	//Descriptor lenght = 5 byte.
		0x24,	//Descriptor Type = "CS_INTERFACE".
		0x00,	//Descriptor Subtype = "Header".
		0x10,	//USB 1.1.
		0x01,  	//USB 1.1.
		/////////////Class/Specific_Descriptor///////////
		0x05,	//Descriptor lenght = 5 byte.
		0x24,	//Descriptor Type = "CS_INTERFACE".
		0x01,	//Descriptor SubType = "Call Management Functional Descriptor".
		0x00, 	//bmCapabilities. 
		0x01,	//Indicates that multiplexed commands are handled via data interface 01.
		/////////////Class/Specific_Descriptor///////////
		0x04,	//Descriptor lenght = 5 byte.
		0x24,  	//Descriptor Type = "CS_INTERFACE".
		0x02,	//Descriptor SubType = "Abstract Control Management functional descriptor".
		0x02,	//bmCapabilities.
		/////////////Class/Specific_Descriptor///////////
		0x05,	//Descriptor lenght = 5 byte.
		0x24,  	//Descriptor Type = "CS_INTERFACE".
		0x06,  	//Descriptor SubType = "Union Descriptor Functional Descriptor".
		0x00,  	//bControlInterface. Interface number of the control.
		0x01,  	//bSubordinateInterface0. Interface number of the subordinate (Data Class) interface.
		/////////////Endpoint_Descriptor///////////
		0x07,	//Descriptor lenght = 7 byte.
		0x05,	//Descriptor Type = "Endpoint".
		0x82,	//In endpoint. Endpoint 2.
		0x03,	//Transfer Type = "Interrupt".
		0x08,	//Endpoint size = 8 byte.
		0x00,	//Endpoint size = 8 byte.
		0x10,	//Interval for polling endpoint = 16 * 1ms.
		/////////////Interface_Descriptor///////////
		0x09,	//Descriptor lenght = 9 byte.
		0x04,  	//Descriptor Type = Interface.
		0x01,  	//Interface number = 1.
		0x00,  	//Alternative interface = 0.
		0x02,  	//Endopoints used by this interface = 2.
		0x0A,  	//Class "Data Interface".
		0x00,  	//Subclass.
		0x00,  	//Protocol "Non specified".
		0x00,  	//iInterface.
		/////////////Endpoint_Descriptor///////////
		0x07,	//Descriptor lenght = 7 byte.
		0x05,  	//Descriptor Type = "Endpoint".
		0x01,  	//OUT endpoint. Endpoint 1.
		0x02,  	//Transfer Type = "Bulk".
		0x40,  	//Endpoint size = 64 byte.
		0x00,  	//Endpoint size = 64 byte.
		0x00,  	//Interval for polling endpoint.
		/////////////Endpoint_Descriptor///////////
		0x07,	//Descriptor lenght = 7 byte.
		0x05,  	//Descriptor Type = "Endpoint".
		0x81,  	//In endpoint. Endpoint 1.
		0x02,  	//Transfer Type = "Bulk".
		0x40,  	//Endpoint size = 64 byte.
		0x00,  	//Endpoint size = 64 byte.
		0x00  	//Interval for polling endpoint.
};

uint8_t 	Line_Coding [7]   = {0};

uint16_t 	Control_Line_State[2] = {0};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Functions declarations.

//Enpoint 0 functions.

//Set endpoint 0 reception to VALID.
void EN_RX_EP0()	{
	USB_ENP0R = ((~USB_ENP0R) & 0x3000) | 0x8280;	}

//Set endpoint 0 reception to NAK.
void DIS_RX_EP0()	{
	USB_ENP0R = ((~USB_ENP0R) & 0x2000) | ((USB_ENP0R) & 0x1000) | 0x8280;	}

//Set endpoint 0 reception to STALL.
void STALL_RX_EP0()	{
	USB_ENP0R = ((USB_ENP0R) & 0x2000) | ((~USB_ENP0R) & 0x1000) | 0x8280;	}
	
//Set endpoint 0 reception to DISABLED.
void OFF_RX_EP0()	{
	USB_ENP0R = ((USB_ENP0R) & 0x3000) | 0x8280;	}

//Set endpoint 0 transmission to VALID.
void EN_TX_EP0()	{
	USB_ENP0R = ((~USB_ENP0R) & 0x0030) | 0x8280;	}
	
//Set endpoint 0 transmission to NAK.
void DIS_TX_EP0()	{
	USB_ENP0R = ((~USB_ENP0R) & 0x0020) | ((USB_ENP0R) & 0x0010) | 0x8280;	}
	
//Set endpoint 0 transmission to STALL.
void STALL_TX_EP0()	{
	USB_ENP0R = ((USB_ENP0R) & 0x0020) | ((~USB_ENP0R) & 0x0010) | 0x8280;	}

//Set endpoint 0 transmission to DISABLE.		
void OFF_TX_EP0()	{
	USB_ENP0R = ((USB_ENP0R) & 0x0030) | 0x8280;	}

//Set transmission packet PID to DATA0.
void SET_DATA0_EP0()	{
	USB_ENP0R = ((USB_ENP0R) & 0x0040) | 0x8280;	}

//Set transmission packet PID to DATA1.
void SET_DATA1_EP0()	{
	USB_ENP0R = ((~USB_ENP0R) & 0x0040) | 0x8280;	}

//Send a packet on endpoint 0. Max packet lenght 64 byte.
void Send_EP0(const uint8_t* data, uint32_t len)		{
	uint16_t* p_mem_16 = (uint16_t*) USB_TX0_BASE_32;	//Point to trasmission buffer endpoint 0.
	uint16_t* data_16 = (uint16_t*) data;				//Point to data transmission buffer.
	uint32_t  ii=0;	
	for(ii=0;ii<len;)	{								//Copy data from data buffer to endpoint 0 buffer. Note: endpoint 0 buffer in PMA is 16 bit wide, not 32.
		*p_mem_16 = *data_16;
		p_mem_16 += 2;
		data_16 += 1;
		ii+=2;			}
	USB_COUNT0_TX = len;								//Set the number of byte to transmit.
	USB_TRANSMITTED_EP0=0;
	EN_TX_EP0();		}


//Enpoint 1 functions.

//Set endpoint 1 reception to VALID.
void EN_RX_EP1()	{
	USB_ENP1R = ((~USB_ENP1R) & 0x3000) | 0x8081;	}

//Set endpoint 1 reception to NAK.
void DIS_RX_EP1()	{
	USB_ENP1R = ((~USB_ENP1R) & 0x2000) | ((USB_ENP1R) & 0x1000) | 0x8081;	}

//Set endpoint 1 transmission to VALID.
void EN_TX_EP1()	{
	USB_ENP1R = ((~USB_ENP1R) & 0x0030) | 0x8081;	}
	
//Set endpoint 1 transmission to NAK.
void DIS_TX_EP1()	{
	USB_ENP1R = ((~USB_ENP1R) & 0x0020) | ((USB_ENP1R) & 0x0010) | 0x8081;	}

//Send a packet on endpoint 1. Max packet lenght 64 byte.
void Send_EP1(uint8_t* data, uint32_t len)		{
	uint16_t* p_mem_16 = (uint16_t*) USB_TX1_BASE_32;	//Point to trasmission buffer endpoint 1.
	uint16_t* data_16 = (uint16_t*) data;				//Point to data transmission buffer.
	uint32_t  ii=0;	
	for(ii=0;ii<len;)	{								//Copy data from data buffer to endpoint 1 buffer. Note: endpoint 1 buffer in PMA is 16 bit wide, not 32.
		*p_mem_16 = *data_16;
		p_mem_16 += 2;
		data_16 += 1;
		ii+=2;			}
	USB_COUNT1_TX = len;								//Set the number of byte to transmit.
	USB_TRANSMITTED_EP1=0;
	EN_TX_EP1();		}

//Send a packet on endpoint 1. Max packet lenght 4294967296 byte.
void Send_EP1_Jumbo(uint8_t* data, uint32_t len)	{
	if(len<32)	{											//If the packet is not bigger than 32 byte use the standard function Send_EP1.
		N_JUMBO_SUBPACKETS=1;
		Send_EP1(data,len);
		return;}	
	uint32_t len_32 = len%32;								//Variable to check if the Jumbo packet size is a multiple o 32 byte and to calculate the last subpacket size.
	N_JUMBO_SUBPACKETS = len/32;							//Calculation of number of packet to send.			
	if((len_32) != 0) 	{
		N_JUMBO_SUBPACKETS++;								//If the Jumbo packet size is not a multiple of 32 add 1 packet of the remaining data.
		JUMBO_LAST_SUBPACKET_SIZE=len_32;}
	else				{ 
		JUMBO_LAST_SUBPACKET_SIZE=32;	}
	JUMBO_BUFF_ADDR = (uint8_t*) data;						//Point JUMBO_BUFF_ADDR	to the data trasmission buffer.
	Send_EP1(data,32);										//Send first packet of the Jumpo packet.
	N_JUMBO_SUBPACKETS_TXED=1;		}						//Set the number of transmitted packets to 1.									

//Read a packet received on endpoint 1 and enable endpoint 1 to receive a new packet. The packet will be copied in the "Received_Data" array, and the leght of the packet will be written in the "len" variable.
void Read_EP1(uint8_t* Received_data, uint32_t* len)		{
	uint32_t N_Byte_Received = USB_COUNT1_RX & 0x03FF;	//Check the number of byte received
	uint32_t ii=0;	
	uint8_t* p_mem_8 = (uint8_t*) USB_RX1_BASE_32;
	USB_RECEIVED_EP1 = 0;								//Reset flag EP1 reception.
	for(ii=0;ii<N_Byte_Received;)	{					//Copy the received data from endpoint 1 reception buffer to "Received_data" buffer. Note: endpoint 1 buffer is 16 bit wide, not 32.
		Received_data[ii] = *p_mem_8;
		ii++;
		p_mem_8++;
		if(ii==N_Byte_Received) {break;}
		Received_data[ii] = *p_mem_8;
		ii++;
		p_mem_8+=3;					}
	*len=N_Byte_Received;								//Copy the lenght of the received packet in the variable *len.
	EN_RX_EP1();								}		//Enable the endpoint 1 reception buffer to receive any new packet.


//Enpoint 2 functions.

//Set endpoint 2 reception to VALID.
void EN_RX_EP2()	{
	USB_ENP2R = ((~USB_ENP2R) & 0x3000) | 0x8682;	}

//Set endpoint 2 reception to NAK.
void DIS_RX_EP2()	{
	USB_ENP2R = ((~USB_ENP2R) & 0x2000) | ((USB_ENP2R) & 0x1000) | 0x8682;	}

//Set endpoint 2 transmission to VALID.
void EN_TX_EP2()	{
	USB_ENP2R = ((~USB_ENP2R) & 0x0030) | 0x8682;	}
	
//Set endpoint 2 transmission to NAK.
void DIS_TX_EP2()	{
	USB_ENP2R = ((~USB_ENP2R) & 0x0020) | ((USB_ENP2R) & 0x0010) | 0x8682;	}


//Management functions.

//Manage control tranfers on endpoint 0.
void USB_MANAGEMENT()	{
		if(USB_RECEIVED_EP0)	{												//If a packet has been received on endpoint 0..
			USB_RECEIVED_EP0 = 0;												//Reset reception flag endpoint 0.
			
			uint8_t* p_mem_8 = (uint8_t*) USB_RX0_BASE_32;						//Point to reception buffer endpoint 0.
			
			if( p_mem_8[0]==0x80 && p_mem_8[1]==0x06 ) 	{						//If a descriptor has been requested..
				
				if(p_mem_8[5]==0x01)			{								//If REQUEST Device_Descriptor..
					Send_EP0(Device_Descriptor,18);								//Send the Device_Descriptor.
					while(USB_TRANSMITTED_EP0==0)	{}							//Wait that the packet has been completely transmitted.
					USB_TRANSMITTED_EP0=0;		}								//Reset flag trasmission complete. 
				
				else if(p_mem_8[5]==0x02)		{								//If REQUEST Configuration_Descriptor..
					if(p_mem_8[12]==0x09 && p_mem_8[13]==0x00)	{				//If requested the partial Configuration_Descriptor..
						Send_EP0(Configuration_Descriptor,9);					//Send the partial Configuration_Descriptor.
						while(USB_TRANSMITTED_EP0==0)	{}						//Wait that the packet has been completely transmitted.
						USB_TRANSMITTED_EP0=0;	}								//Reset flag trasmission complete. 
					else					{									//If requested the complete Configuration_Descriptor..
						SET_DATA1_EP0();										//Set transmission packet PID to DATA1.
						Send_EP0(Configuration_Descriptor_0,64);				//Send the first part of the Configuration_Descriptor.
						while(USB_TRANSMITTED_EP0==0)	{}						//Wait that the packet has been completely transmitted.
						USB_TRANSMITTED_EP0=0;									//Reset flag trasmission complete. 
						SET_DATA0_EP0();										//Set transmission packet PID to DATA0.
						Send_EP0(Configuration_Descriptor_2,3);					//Send the second part of the Configuration_Descriptor.
						while(USB_TRANSMITTED_EP0==0)	{}						//Wait that the packet has been completely transmitted.
						USB_TRANSMITTED_EP0=0;	}	}							//Reset flag trasmission complete.  
										
				else if(p_mem_8[5]==0x06)		{								//If REQUEST DEVICE_QUALIFIER_DESCIPTOR..
					STALL_TX_EP0();				}								//Send REQUEST_ERROR (Because USB1.1 doesn't support that).
				}	
						
			else if ( p_mem_8[0]==0x00 && p_mem_8[1]==0x05 )	{				//If REQUEST SET_ADDRESS..
				Send_EP0(0,0);													//Send ACK.
				while(USB_TRANSMITTED_EP0==0)	{}								//Wait that the ACK has been sent.
				USB_TRANSMITTED_EP0=0;											//Reset flag trasmission complete. 
				USB_DADDR |= (p_mem_8[4] & 0x7F);								//Set the received address in USB peripheral.
			}
			
			else if ( p_mem_8[0]==0x00 && p_mem_8[1]==0x09 )	{				//If REQUEST SET_CONFIGURATION..
				Send_EP0(0,0);													//Send ACK.
				while(USB_TRANSMITTED_EP0==0)	{}								//Wait that the ACK has been sent.
				USB_TRANSMITTED_EP0=0;							}				//Reset flag trasmission complete. 
				
			else if ( p_mem_8[0]==0xA1 && p_mem_8[1]==0x21 )	{				//If REQUEST GET_LINE_CODING..
				Send_EP0(Line_Coding,7);										//Send the LINE_CODING.
				while(USB_TRANSMITTED_EP0==0)	{}								//Wait that the packet has been completely transmitted.
				USB_TRANSMITTED_EP0=0;							}				//Reset flag trasmission complete.  
			
			else if ( p_mem_8[0]==0x21 && p_mem_8[1]==0x22 )	{				//If REQUEST SET_CONTROL_LINE_STATE..
				Control_Line_State[0] = p_mem_8[4];								//Set the received CONTROL_LINE_STATE in USB peripheral.
				Control_Line_State[1] = p_mem_8[5];								//Set the received CONTROL_LINE_STATE in USB peripheral.
				Send_EP0(0,0);													//Send ACK.
				while(USB_TRANSMITTED_EP0==0)	{}								//Wait that the ACK has been sent.
				USB_TRANSMITTED_EP0=0;							}				//Reset flag trasmission complete.
			
			else if ( p_mem_8[0]==0x21 && p_mem_8[1]==0x20 )	{				//If REQUEST SET_LINE_CODING..
				EN_RX_EP0();													//Enable reception on endpoint 0.
				while(USB_RECEIVED_EP0==0) {};									//Wait the reception of the DATA packet.
				USB_RECEIVED_EP0=0;												//Reset flag reception complete.
				Line_Coding[0] = p_mem_8[0];									//Set the received LINE_CODING in USB peripheral
				Line_Coding[1] = p_mem_8[1];                                    //Set the received LINE_CODING in USB peripheral
				Line_Coding[2] = p_mem_8[4];                                    //Set the received LINE_CODING in USB peripheral
				Line_Coding[3] = p_mem_8[5];                                    //Set the received LINE_CODING in USB peripheral
				Line_Coding[4] = p_mem_8[8];                                    //Set the received LINE_CODING in USB peripheral
				Line_Coding[5] = p_mem_8[9];                                    //Set the received LINE_CODING in USB peripheral
				Line_Coding[6] = p_mem_8[12];                                   //Set the received LINE_CODING in USB peripheral
				SET_DATA1_EP0();												//Set transmission packet PID to DATA1.
				Send_EP0(0,0);													//Send ACK.
				while(USB_TRANSMITTED_EP0==0)	{}								//Wait that the ACK has been sent.
				USB_TRANSMITTED_EP0=0;							}				//Reset flag trasmission complete.

			else if ( p_mem_8[0]==0x02 && p_mem_8[1]==0x01 )	{				//If REQUEST CLEAR_FEATURE..
				Send_EP0(0,0);                                                  //Send ACK.
				while(USB_TRANSMITTED_EP0==0)	{}								//Wait that the ACK has been sent.
				USB_TRANSMITTED_EP0=0;							}				//Reset flag trasmission complete.
			
			EN_RX_EP0();														//Enable reception on endpoint 0.
		}							//END EP0.
		
		if(USB_RECEIVED_EP2)	{												//If a packet has been received on endpoint 1..
			USB_RECEIVED_EP2 = 0;												//Reset flag reception endpoint 2.
			EN_RX_EP2();		}												//Enable reception on endpoint 0.
	
}

//Function used to send the remaining subpackets of the Jumbo packet. Automatically called in interrupt when the previous packet has been completely transmitted.
void Manage_EP1_Jumbo()			{
	JUMBO_BUFF_ADDR+=32;																			//Point to the next subpacket.
	if(N_JUMBO_SUBPACKETS>1)	{Send_EP1((uint8_t*)JUMBO_BUFF_ADDR,32);}							//If not the last packet send a subpacket of 32 bytes.
	else						{Send_EP1((uint8_t*)JUMBO_BUFF_ADDR,JUMBO_LAST_SUBPACKET_SIZE);}	//If the last packet send a packet with the remaining bytes.	
	N_JUMBO_SUBPACKETS_TXED++;	}																	//Report that a new packet will be sent.


//Interrupt functions.

//Function called when an USB reset, error or transmission complete happens. Automatically manage USB reset, error or transmission complete events.
void USB_LP_CAN1_RX0_IRQHandler(void)	{
	if(USB_ISTR & 0x0400) 	{						//If received a USB_RESET..
		USB_ISTR  = 0xFBFF;							//Clear flag USB_RESET.
		USB_ENP0R = 0x8280;							//Set endpoint 0 control register to default value. EP0 type = CONTROL.
		USB_ENP1R = 0x8081;							//Set endpoint 1 control register to default value. EP1 type = BULK. 
		USB_ENP2R = 0x8682;							//Set endpoint 2 control register to default value. EP2 type = INTERRUPT.
		EN_RX_EP0();								//Set endpoint 0 reception to VALID.
		DIS_TX_EP0();								//Set endpoint 0 transmission to NAK.
		EN_RX_EP1();								//Set endpoint 1 reception to VALID.
		DIS_TX_EP1();								//Set endpoint 1 transmission to NAK.
		EN_RX_EP2();								//Set endpoint 2 reception to VALID.
		DIS_TX_EP2();								//Set endpoint 2 transmission to NAK.
		USB_DADDR = 0x0080;							//USB ON. ADDRESS = 0.
		}
	
	if(USB_ISTR & 0x2000)	{						//If an error occurred..
		USB_ISTR	= 0xDFFF;		}				//Clear error flag.
		
	if(USB_ISTR & 0x8000)	{						//If a transmission has been completed correctly..
		if(USB_ENP0R & 0x8000)	{					//If the transmission is EP0 OUT..
			USB_RECEIVED_EP0=1;						//Set flag received data on EP0.
			USB_ENP0R = 0x0280; 					//Clear flag CTR_RX. EP0 Control. EP0 ADDR=0.
			DIS_RX_EP0();							//EP0 RX NAK.
			}
		
		if(USB_ENP0R & 0x0080)	{					//If the transmission is EP0 IN..
			USB_TRANSMITTED_EP0=1;					//Set flag transmitted data on EP0.
			USB_ENP0R = 0x8200; 					//EP0 Control. Clear flag CTR_TX. EP0 ADDR=0.
			DIS_TX_EP0();							//EP0 TX NAK.
			}
		
		if(USB_ENP1R & 0x8000)	{					//If the transmission is EP1 OUT..
			USB_RECEIVED_EP1=1;						//Set flag received data on EP1.
			USB_ENP1R = 0x0081; 					//Clear flag CTR_RX. EP1 Bulk. EP1 ADDR=1.
			DIS_RX_EP1();							//EP1 RX NAK.
			}
		
		if(USB_ENP1R & 0x0080)	{					//If the transmission is è EP1 IN..
			USB_TRANSMITTED_EP1=1;					//Set flag transmitted data on EP1.
			USB_ENP1R = 0x8001; 					//EP1 Bulk. Clear flag CTR_TX. EP1 ADDR=1.
			DIS_TX_EP1();							//EP1 TX NAK.
			if(N_JUMBO_SUBPACKETS>1)		{		//If any jumbo subpacket still need to be transmitted..
				N_JUMBO_SUBPACKETS--;				//Report that a new packet has been transmitted.
				Manage_EP1_Jumbo();			}		//Manage the transmission of the new jumbo subpacket.
			else if (N_JUMBO_SUBPACKETS==1)	{		//If the last jumbo subpacket has been completely transmitted..
				N_JUMBO_SUBPACKETS=0;		}	}	//Report that there isn't any jumbo subpacket to be transmitted.
		
		if(USB_ENP2R & 0x8000)	{					//If the transmission is EP2 OUT..
			USB_RECEIVED_EP2=1;						//Set flag transmitted data on EP2.
			USB_ENP2R = 0x0682; 					//Clear flag CTR_RX. EP2 Interrupt. EP2 ADDR=2.
			DIS_RX_EP2();							//EP2 RX NAK.
			}
		
		if(USB_ENP2R & 0x0080)	{					//If the transmission is EP2 IN..
			USB_TRANSMITTED_EP2=1;					//Set flag transmitted data on EP1.
			USB_ENP2R = 0x8602; 					//EP2 Interrupt. Clear flag CTR_TX. EP2 ADDR=2.
			DIS_TX_EP2();							//EP2 TX NAK.
			}
		
	
	
	}												//End CTR.

}
