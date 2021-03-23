::start cmd /k 
stm32flash -w ./USB-VCP_LL_C8.bin -b 115200 -v -g 0x0 COM10
PAUSE
