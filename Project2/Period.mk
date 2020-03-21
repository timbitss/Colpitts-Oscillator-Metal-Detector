SHELL=cmd
PORTN=$(shell type COMPORT.inc)
OBJS=Period.o usart.o
CPU=-mmcu=atmega328p

Period.elf: $(OBJS)
	avr-gcc $(CPU) $(OBJS) -Wl,-Map,Period.map -Wl,-u,vfprintf -lprintf_flt -lm -o Period.elf
	avr-objcopy -j .text -j .data -O ihex Period.elf Period.hex
	@echo done!
	
Period.o: Period.c
	avr-gcc -g -Os -mmcu=atmega328 -c Period.c

usart.o: usart.c usart.h
	avr-gcc -g -Os -Wall -mmcu=atmega328p -c usart.c
	
clean:
	@del *.hex *.elf *.o 2>nul

FlashLoad:
	@Taskkill /IM putty.exe /F 2>nul | wait 500
	spi_atmega -rc -p -v Period.hex
	c:\putty\putty.exe -serial $(PORTN) -sercfg 19200,8,n,1,N -v

putty:
	@Taskkill /IM putty.exe /F 2>nul | wait 500
	c:\putty\putty.exe -serial $(PORTN) -sercfg 19200,8,n,1,N -v

dummy: Period.hex Period.map
	@echo Hello from dummy!
	