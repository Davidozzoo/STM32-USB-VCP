#define FLASH_ACR 		(*(volatile uint32_t*)(0x40022000))
#define RCC_CR 			(*(volatile uint32_t*)(0x40021000))
#define RCC_CFGR 		(*(volatile uint32_t*)(0x40021004))
#define RCC_CIR 		(*(volatile uint32_t*)(0x40021008))
#define RCC_APB2RSTR 	(*(volatile uint32_t*)(0x4002100C))
#define RCC_APB1RSTR 	(*(volatile uint32_t*)(0x40021010))
#define RCC_AHBENR 		(*(volatile uint32_t*)(0x40021014))
#define RCC_APB2ENR 	(*(volatile uint32_t*)(0x40021018))
#define RCC_APB1ENR 	(*(volatile uint32_t*)(0x4002101C))
#define RCC_BDCR 		(*(volatile uint32_t*)(0x40021020))
#define RCC_CSR 		(*(volatile uint32_t*)(0x40021024))

#define ClockInit()		{	/*TURN ON FLASH PREFETCH BUFFER FOR HIGH SPEED CLOCK.*/																																																\
							FLASH_ACR=0x12;											/*PREFETCH BUFFER ON. FLASH HALF CYCLE ACCESS OFF. FLASH LATENCY = 2.*/																														\
							while(!(FLASH_ACR & 0X20)) {};							/*Wait until PREFETCH BUFFER is enabled.*/																																					\
							/*CLEAR ALL RCC INTERRUPT FLAGS. DISABLE ALL RCC INTERRUPTS.*/																																														\
							RCC_CIR=0x009F0000;																																																									\
							/*TURN ON HSI AND HSE.*/																																																							\
							RCC_CR=0X00010081;										/*PLL OFF. CSS OFF. HSE BYP OFF. HSE ON. HSITRIM = 16. HSI ON.*/																															\
							while(!(RCC_CR & 0X00000002)) {};						/*Wait until HSI RDY.*/																																										\
							while(!(RCC_CR & 0X00020000)) {};						/*Wait until HSE RDY.*/																																										\
							/*SET HSI AS SYSTEM CLOCK.*/																																																						\
							RCC_CFGR=0X001D0400;									/*MCO OFF. USB PRESCALED = 1.5. PLLMUL = 9. HSE Prescaler = 1. PLL INPUT = HSE. ADC PRESCALER = PCLK2/2. APB2 PRESCALER = 1. APB1 PRESCALER = 2. AHB PRESCALER = 1. SYSTEM CLOCK = HSI.*/	\
							while(RCC_CFGR & 0X0000000F) {};						/*Wait until SYSTEM CLOCK = HSI.*/																																							\
							/*TURN ON PLL.*/																																																									\
							RCC_CR=0X01010081;										/*PLL ON. CSS OFF. HSE BYP OFF. HSE ON. HSITRIM = 16. HSI ON.*/																																\
							while(!(RCC_CR & 0X02000000)) {};						/*Wait until PLL RDY.*/																																										\
							/*SET PLL AS SYSTEM CLOCK.*/																																																						\
							RCC_CFGR=0X001D0402;									/*MCO OFF. USB PRESCALED = 1.5. PLLMUL = 9. HSE Prescaler = 1. PLL INPUT = HSE. ADC PRESCALER = PCLK2/2. APB2 PRESCALER = 1. APB1 PRESCALER = 2. AHB PRESCALER = 1. SYSTEM CLOCK = PLL.*/	\
							while((RCC_CFGR & 0X0000000F)!=0xA) {};					/*Wait until SYSTEM CLOCK = PLL.*/																																							\
							RCC_CIR=0x009F0000;		}								/*CLEAR ALL RCC INTERRUPT FLAGS. DISABLE ALL RCC INTERRUPTS.*/
