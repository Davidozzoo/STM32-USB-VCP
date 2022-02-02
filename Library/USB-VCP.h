#include "stdint.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Registers definitions.

#define USB_CNTR 		(*(volatile uint32_t*)(0x40005C40))
#define USB_ISTR		(*(volatile uint32_t*)(0x40005C44))
#define USB_DADDR		(*(volatile uint32_t*)(0x40005C4C))
#define USB_BTABLE		(*(volatile uint32_t*)(0x40005C50))
#define USB_ENP0R 		(*(volatile uint32_t*)(0x40005C00))
#define USB_ENP1R 		(*(volatile uint32_t*)(0x40005C04))
#define USB_ENP2R 		(*(volatile uint32_t*)(0x40005C08))
#define USB_ENP3R 		(*(volatile uint32_t*)(0x40005C0C))
#define USB_ENP4R 		(*(volatile uint32_t*)(0x40005C10))
#define USB_ENP5R 		(*(volatile uint32_t*)(0x40005C14))
#define USB_ENP6R 		(*(volatile uint32_t*)(0x40005C18))
#define USB_ENP7R 		(*(volatile uint32_t*)(0x40005C1C))

#define USB_BASE_MEMORY (*(volatile uint32_t*)(0x40006000))
#define RCC_APB1ENR_USB	(*(volatile uint32_t*)(0x4002101C))
#define RCC_APB2ENR_USB	(*(volatile uint32_t*)(0x40021018))
#define RCC_CFGR_USB 	(*(volatile uint32_t*)(0x40021004))
#define GPIOA_CRH_USB	(*(volatile uint32_t*)(0x40010804))

#define USB_ADDR0_TX 	(*(volatile uint32_t*)(0x40006000))
#define USB_COUNT0_TX 	(*(volatile uint32_t*)(0x40006004))
#define USB_ADDR0_RX 	(*(volatile uint32_t*)(0x40006008))
#define USB_COUNT0_RX 	(*(volatile uint32_t*)(0x4000600C))

#define USB_ADDR1_TX 	(*(volatile uint32_t*)(0x40006010))
#define USB_COUNT1_TX 	(*(volatile uint32_t*)(0x40006014))
#define USB_ADDR1_RX 	(*(volatile uint32_t*)(0x40006018))
#define USB_COUNT1_RX 	(*(volatile uint32_t*)(0x4000601C))

#define USB_ADDR2_TX 	(*(volatile uint32_t*)(0x40006020))
#define USB_COUNT2_TX 	(*(volatile uint32_t*)(0x40006024))
#define USB_ADDR2_RX 	(*(volatile uint32_t*)(0x40006028))
#define USB_COUNT2_RX 	(*(volatile uint32_t*)(0x4000602C))


#define USB_RX0_BASE_32	0x40006080		//Endpoint 0 reception buffer initial address in RAM. 64 byte lenght (128 byte because the packet memory is 16 bit wide).
#define USB_RX0_BASE	0x0040			//Endpoint 0 reception buffer initial address in PMA. Packet memory is 16 bit wide.
#define USB_TX0_BASE_32	0x40006100		//Endpoint 0 transmission buffer initial address in RAM. 64 byte lenght (128 byte because the packet memory is 16 bit wide).
#define USB_TX0_BASE	0x0080			//Endpoint 0 transmission buffer initial address in PMA. Packet memory is 16 bit wide.

#define USB_RX1_BASE_32	0x40006180		//Endpoint 1 reception buffer initial address in RAM. 64 byte lenght (128 byte because the packet memory is 16 bit wide).
#define USB_RX1_BASE	0x00C0			//Endpoint 1 reception buffer initial address in PMA. Packet memory is 16 bit wide.
#define USB_TX1_BASE_32	0x40006200		//Endpoint 1 transmission buffer initial address in RAM. 64 byte lenght (128 byte because the packet memory is 16 bit wide).
#define USB_TX1_BASE	0x0100			//Endpoint 1 transmission buffer initial address in PMA. Packet memory is 16 bit wide.     
                        
#define USB_RX2_BASE_32	0x40006280		//Endpoint 2 reception buffer initial address in RAM. 8 byte lenght (16 byte because the packet memory is 16 bit wide).
#define USB_RX2_BASE	0x0140			//Endpoint 2 reception buffer initial address in PMA. Packet memory is 16 bit wide.
#define USB_TX2_BASE_32	0x40006290		//Endpoint 2 transmission buffer initial address in RAM. 8 byte lenght (16 byte because the packet memory is 16 bit wide).
#define USB_TX2_BASE	0x0148			//Endpoint 2 transmission buffer initial address in PMA. Packet memory is 16 bit wide.  

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Descriptors definitions.

extern const uint8_t 	Device_Descriptor[18];

extern const uint8_t 	Configuration_Descriptor[67];
#define 				Configuration_Descriptor_0  &Configuration_Descriptor[0]	//lenght 32 byte.
#define 				Configuration_Descriptor_1  &Configuration_Descriptor[32]	//lenght 32 byte.
#define 				Configuration_Descriptor_2  &Configuration_Descriptor[64]	//lenght 3 byte.

extern uint8_t 			Line_Coding [7];
#define 				LC_DATA_RATE 	  	(*((uint32_t*)(&Line_Coding[0])))		//Represents the virtual serial port speed in bit/s. dwDTERate.
#define					LC_STOP_BITS 	  	(*(uint8_t*)(&Line_Coding[4]))			//Represents the number of stop bits of the virtual serial port. bCharFormat.
#define					LC_PARITY_TYPE 	  	(*(uint8_t*)(&Line_Coding[5]))			//Represents the type of parity of the virtual serial port. bParityType.
#define					LC_N_BITS_DATA 	  	(*(uint8_t*)(&Line_Coding[6]))			//Represents the number of bit transmitted for each frame on the virtual serial port. bDataBits.

extern  uint16_t 		Control_Line_State[2];

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Global variables definitions.

extern volatile uint32_t 	wait;							//Variable used to wait Tsetup = 2.5 us.
extern volatile uint8_t 	USB_RECEIVED_EP0;				//Variable set to 1 when a packet has been received on endpoint 0.
extern volatile uint8_t 	USB_TRANSMITTED_EP0;			//Variable set to 1 when a packet has been transmitted on endpoint 0.
extern volatile uint8_t 	USB_RECEIVED_EP1;				//Variable set to 1 when a packet has been received on endpoint 1.
extern volatile uint8_t 	USB_TRANSMITTED_EP1;            //Variable set to 1 when a packet has been transmitted on endpoint 1.
extern volatile uint8_t 	USB_RECEIVED_EP2;				//Variable set to 1 when a packet has been received on endpoint 2.
extern volatile uint8_t 	USB_TRANSMITTED_EP2;            //Variable set to 1 when a packet has been transmitted on endpoint 2.
extern volatile uint32_t  	N_JUMBO_SUBPACKETS;				//Number of 32 byte subpackets of the Jumbo packet that have to be transmitted.
extern volatile uint32_t  	N_JUMBO_SUBPACKETS_TXED;		//Number of 32 byte packets of the Jumbo packet already transmitted.
extern volatile uint8_t* 	JUMBO_BUFF_ADDR;				//Pointer to the Jumbo packet.
extern volatile uint32_t 	JUMBO_LAST_SUBPACKET_SIZE;		//Size of the last Jumbo subpacket.

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Functions declarations.

//Enpoint 0 functions.
void EN_RX_EP0();											//Set endpoint 0 reception to VALID.
void DIS_RX_EP0();											//Set endpoint 0 reception to NAK.
void STALL_RX_EP0();										//Set endpoint 0 reception to STALL.
void OFF_RX_EP0();			                                //Set endpoint 0 reception to DISABLED.
void EN_TX_EP0();											//Set endpoint 0 transmission to VALID.
void DIS_TX_EP0();											//Set endpoint 0 transmission to NAK.
void STALL_TX_EP0();										//Set endpoint 0 transmission to STALL.
void OFF_TX_EP0();											//Set endpoint 0 transmission to DISABLE.		
void SET_DATA0_EP0();										//Set transmission packet PID to DATA0.
void SET_DATA1_EP0();										//Set transmission packet PID to DATA1.
void Send_EP0(const uint8_t* data, uint32_t len);			//Send a packet on endpoint 0. Max packet lenght 64 byte.

//Enpoint 1 functions.
void EN_RX_EP1();											//Set endpoint 1 reception to VALID.
void DIS_RX_EP1();                                          //Set endpoint 1 reception to NAK.
void EN_TX_EP1();                                           //Set endpoint 1 transmission to VALID.
void DIS_TX_EP1();                                          //Set endpoint 1 transmission to NAK.
void Send_EP1(uint8_t* data, uint32_t len);			        //Send a packet on endpoint 1. Max packet lenght 64 byte.
void Send_EP1_Jumbo(uint8_t* data, uint32_t len);			//Send a packet on endpoint 1. Max packet lenght 4294967296 byte.
void Read_EP1(uint8_t* Received_data, uint32_t* len);		//Read a packet received on endpoint 1. The packet will be copied in the "Received_Data" array, and the leght of the packet will be written in the "len" variable.

//Enpoint 2 functions.
void EN_RX_EP2();											//Set endpoint 2 reception to VALID.
void DIS_RX_EP2();                                          //Set endpoint 2 reception to NAK.
void EN_TX_EP2();                                           //Set endpoint 2 transmission to VALID.
void DIS_TX_EP2();                                          //Set endpoint 2 transmission to NAK.

//Management functions.
void USB_MANAGEMENT();										//Manage control tranfers on endpoint 0.

void Manage_EP1_Jumbo();									//Function used to send the remaining subpackets of the Jumbo packet. Automatically called in interrupt when the previous packet has been completely transmitted.

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Macros definitions.

#define		Write_VCP(pTX_Data,len)				Send_EP1_Jumbo(pTX_Data,len)	//Send a packet on the virtual serial port.
#define		Read_VCP(pReceived_Data,plen)		Read_EP1(pReceived_Data,plen)	//Read a packet received from the virtual serial port and enable the port to receive a new packet. The packet will be copied in the "Received_Data" array, and the leght of the packet will be written in the variable pointed by "len".
#define 	VCP_Received						USB_RECEIVED_EP1				//Variable set to 1 when a packet is received from the virtual serial port.
#define 	VCP_Transmitted						!N_JUMBO_SUBPACKETS				//Variable set to 1 when a packet has been transmitted on the virtual serial port.	
#define		Wait_VCP_TX()						while(!(VCP_Transmitted))		//Wait until a packet has been completely transmitted on the virtual serial port.


//USB clock, GPIO, PMA, transceiver and interrupts initialization. 
#define		USBInit()		{	RCC_CFGR_USB	&=0xFFBFFFFF; 					/*USB CLOCK PRESCALER = 1.5*/																			\
								RCC_APB1ENR_USB |= 0x00800000;					/*TURN ON USB CLOCK*/																					\
								RCC_APB2ENR_USB |= 0x00000005;					/*GPIOA ON. ALTERNATE FUNCTION IO ON.*/																	\
								GPIOA_CRH_USB	&= 0xFFF00FFF;					/*Reset GPIOA12 and GPIOA11.*/																			\
								GPIOA_CRH_USB	|= 0x00044000;;					/*GPIOA12 (USBDP). GPIOA11 (USBDM).*/																	\
								USB_BTABLE	  = 0x0000;							/*Allocation table address in the PMA.*/																\
								USB_ADDR0_TX  = USB_TX0_BASE;					/*Endpoint 0 transmission buffer initial address in PMA.*/												\
								USB_COUNT0_TX = 0x0040;							/*Endpoint 0 transmission buffer lenght 64 byte.*/														\
								USB_ADDR0_RX  = USB_RX0_BASE;					/*Endpoint 0 reception buffer initial address in PMA.*/													\
								USB_COUNT0_RX = 0x8400;							/*Endpoint 0 recetpion buffer block size = 32 byte, number of blocks = 2. Total dimension 64 byte.*/	\
								USB_ADDR1_TX  = USB_TX1_BASE;					/*Endpoint 1 transmission buffer initial address in PMA.*/												\
								USB_COUNT1_TX = 0x0040;							/*Endpoint 1 transmission buffer lenght 64 byte.*/														\
								USB_ADDR1_RX  = USB_RX1_BASE;					/*Endpoint 1 reception buffer initial address in PMA.*/													\
								USB_COUNT1_RX = 0x8400;							/*Endpoint 1 recetpion buffer block size = 32 byte, number of blocks = 2. Total dimension 64 byte.*/	\
								USB_ADDR2_TX  = USB_TX2_BASE;					/*Endpoint 2 transmission buffer initial address in PMA.*/												\
								USB_COUNT2_TX = 0x0008;							/*Endpoint 2 transmission buffer lenght 8 byte.*/														\
								USB_ADDR2_RX  = USB_RX2_BASE;					/*Endpoint 2 reception buffer initial address in PMA.*/                                     			\
								USB_COUNT2_RX = 0x1000;							/*Endpoint 2 recetpion buffer block size = 2 byte, number of blocks = 4. Total dimension 8 byte.*/		\
								USB_CNTR	  = 0x0003;							/*USB Interrupt OFF. USB Transceiver OFF. RESET ON.*/                                           		\
								USB_CNTR	  = 0x0001;							/*USB Interrupt OFF. USB Transceiver ON. RESET ON*/                                             		\
								for(wait=0;wait<30;wait++) {}					/*Wait Tsetup = 2.5 us.*/																					\
								USB_CNTR	  = 0x0000;							/*USB Interrupt OFF. USB Transceiver ON. RESET OFF.*/                                           		\
								USB_ISTR	  = 0x0000;							/*Clear interrupt flags.*/                                                                      		\
								USB_CNTR	  = 0xF400;							/*Correct Transfer Interrupt ON. PMAOVRM ON. ERROR ON. WAKEUP ON. Reset interrupt ON.*/         		\
								NVIC->ISER[0] = 0x00100000;						/*USB LP Interrupt ON.*/																				\
								USB_DADDR	  = 0x0080;		}					/*USB ON. ADDRESS = 0.*/
	



