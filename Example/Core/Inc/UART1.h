#define RCC_APB2ENR_UART1 	(*(volatile uint32_t*)(0x40021018))
#define GPIOA_CRH_UART1 	(*(volatile uint32_t*)(0x40010804))
#define	USART1_SR			(*(volatile uint32_t*)(0x40013800))
#define	USART1_DR			(*(volatile uint32_t*)(0x40013804))
#define	USART1_BRR			(*(volatile uint32_t*)(0x40013808))
#define	USART1_CR1			(*(volatile uint32_t*)(0x4001380C))
#define	USART1_CR2			(*(volatile uint32_t*)(0x40013810))
#define	USART1_CR3			(*(volatile uint32_t*)(0x40013814))
#define	USART1_GTPR			(*(volatile uint32_t*)(0x40013818))



#define UART1Init()		{	RCC_APB2ENR_UART1 |= 0x00004005;				/*USART1 ON. PORTA ON. ALTERNATE FUNCION IO ON. */											\
							GPIOA_CRH_UART1	  &= 0xFFFFF00F;				/*GPIOA10 (USART1_RX) Floating Input. GPIOA9 (USART1_TX) Alternate Output PUSH-PULL.*/		\
							GPIOA_CRH_UART1	  |= 0x000004B0;				/*GPIOA10 (USART1_RX) Floating Input. GPIOA9 (USART1_TX) Alternate Output PUSH-PULL.*/		\
							/*USART1_BRR   = 0x0271;*/						/*Baudrate=115.2 kb/s.*/																	\
							USART1_BRR   = 0x0004E;							/*Baudrate=921.6 kb/s. 4.875. 4|14.*/														\
							USART1_CR1	  = 0x0000200C;						/*UART1 Attiva. TX attivo. RX attivo.*/														\
							USART1_CR2	  = 0x00000000;						/*No LIN. No Pin CLK.*/																		\
							USART1_CR3	  = 0x00000000;		}				/*No DMA. No IrDA. No Error Interrupt. No CTS. No RTS.*/	
	
#define	SendUART(data)	{ USART1->DR=data; while(!(USART1->SR&0x00000080)){}; }	