# STM32-USB-VCP
STM32F103XX USB Device and Virtual Serial Port (VCP) library.

## DESCRIPTION

Welcome to my STM32F103XX USB Device and Virtual Serial Port (VCP) library.
This library allows to connect an STM32F103XX to a PC, through USB, be recognised as an USB Virtual Serial Port and
communicate as if it were a standard serial COM port.
The communication with the serial port can be handled with YAT, PuTTY, Arduino Serial Monitor and other serial terminals.

This library initialize and manage completely the USB peripheral, USB control transfers and packets exchange.

I designed the library to be light, independet and easy to use as much as possible.




## COMPILATION

I compiled the library with STM32CubeIde 1.3.0 on Windows 10, but it could be compiled with Arduino or gcc-arm-none-eabi stand alone because it doesn't have any
particular dependence.

The library was compiled and tested for STM32F103RB and STM32F103C8 (Blue Pill).





## HOW TO USE THE LIBRARY WITH STM32CubeIde

1. Create a new project for your STM32 MCU (you don't need to enable anything about the USB peripheral, the library will do everything).

2. Set SYSTEM CLOCK = 72Mhz. AHB CLOCK = 72Mhz. APB1 CLOCK = 36Mhz. APB2 CLOCK = 72Mhz. USB CLOCK = 48Mhz. (Alternatively use my function "ClockInit()" in the example code, you will find it's definition in "RCC_CLOCK.h", in folder Core/Inc).  

3. Copy "USB-VCP.h" to <projet folder>/Core/Inc.

4. Copy "USB-VCP.c" to <projet folder>/Core/Src.

5. #include "USB-VCP.h" in your "main.c" file.

6. Call the "USBInit();" function at the begginning of your main.

7. Call the "USB_MANAGEMENT();" function inside your while(1) loop in main.

8. Use the USER Functions to receive and transmit data on the Virtual Serial Port.

9. Compile.





## USER FUNCTIONS

#### USBInit();
USB clock, GPIO, PMA, transceiver and interrupts initialization.  
ATTENTION: The initialization uses Timer 4, once the initialization is terminated the Timer 4 is left free, so if you have to initializate the Timer 4 do it after calling the USBInit() function.

#### USB_MANAGEMENT();
Manage control tranfers on endpoint 0.  

#### Write_VCP(uint8_t* data, uint32_t len);
Send a packet of size "len", that contains the data stored in the "data" array, on the virtual serial port.  

#### VCP_Transmitted
Variable set to 1 when a packet has been completely transmitted on the virtual serial port.  

#### Wait_VCP_TX();
Wait until a packet has been completely transmitted on the virtual serial port.  
It's not always necessary to call this function after sending some data, but you have to wait until the variable "VCP_Transmitted" goes to 1 before sending new data, otherwise you will overwrite the old data.  

#### Read_VCP(uint8_t* Received_data, uint32_t* len);
Read a packet received from the virtual serial port and enable the port to receive a new packet. The packet will be copied in the "Received_Data" array, and the leght of the packet will be written in the variable pointed by "len".  

#### VCP_Received
Variable set to 1 when a packet is received from the virtual serial port.  




## EXAMPLE

You can find an example on how to use the library in the "Example" folder. The example was compiled for STM32F103C8 (blue pill).

Usually your main.c should be like this:


```
#include "USB-VCP.h"

...

int main(void)	{

...

USBInit();

...	
	
	while(1) {
		...
		USB_MANAGEMENT();
		...
		if(VCP_Received)	{
			Read_VCP(...)		 
		...
		}
	}
}
```





## PC DRIVERS

On Windows 10 you don't need to install any driver, because they are alreay installed by default, the same happens on linux (Tested UBUNTU 18.04).
For Windows 98SE, 2000, XP, Vista®, 7, and 8.x you can download drivers from here:
https://www.st.com/en/development-tools/stsw-stm32102.html
(Not tested).





## HOW TO USE THE SERIAL PORT WITH YAT

1.Download YAT https://sourceforge.net/projects/y-a-terminal/

2.Once compiled and uploaded your firmware to the STM32, connect it to the PC and check in "device manager" which COM port has been assigned to the device.

3.Open YAT.

4.Click Terminal -> Settings

5.Serial Port: insert the correct COM port.

6.Leave "Bits per second", "Data bits", "Parity" and "Stop Bits" as they are, because they are useless since this is a virtual serial port.

7.Flow Control: None.

8.Click Ok.

9.Click Terminal -> Open/Start.

10.Now you can send and receive your data.

11. If you have uploaded the example code you will receive the same data that you have sent.




## HOW IT WORKS

My library uses 3 endpoints.

EP0 for control transfers. 64 byte size.

EP1 for sending and receiving data through the virtual serial port. 64 byte size.

EP2 for the Protocol AT V.250 (actually not used).	8 byte size.

When an USB event happens a flag is set in interrupt routine and the USB_MANAGEMENT(), or an user function, will handle the event in main.
This is made to reduce the time spent in interrupt when an event occurs.

All the Descriptors are defined in "USB-VCP.c" and they are sent to the host, when requested, by the USB_MANAGEMENT() function.




## MORE INFORMATIONS ABOUT THE USB PROTOCOL

If you want to know more about the USB protocol here there are some documents that can be useful:

usb_20.pdf

CDC120-20101103-track.pdf

PSTN120.pdf

STM32_Reference_Manual.pdf

https://www.usbmadesimple.co.uk/ums_1.htm
https://www.beyondlogic.org/usbnutshell/usb1.shtml
https://bits4device.wordpress.com/2011/10/14/usb-protocol-device-framework/
https://en.m.wikipedia.org/wiki/USB
https://www.jungo.com/st/support/documentation/windriver/14.5.0/wdusb_manual.mhtml/USB_cfg.html
http://janaxelson.com/usbcenum.htm
https://www.perytech.com/USB-Enumeration.htm





## ABOUT ME

I'm Davide Demo, an electronic engeneer.
I spend my free time studying about electronic, i have always wanted to learn how USB works and developing this library was the occasion to do that.

I developed this library because i used the original one from ST, but i think it's too messy (like all ST libraries), not so easy to use, full of useless code and hard to understand.
I hope that my library will be useful for someone and help someone to understand more about USB protocol.

If you need some informations or you want to help me to solve any bug you can write me to: 97davide9736@gmail.com