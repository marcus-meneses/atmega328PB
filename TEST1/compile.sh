#m328pb
export C_INCLUDE_PATH=.:/usr/lib/avr/include:../LIBS
export LIBRARY_PATH=.:/usr/lib/avr/lib
rm *.o
avr-gcc -g -Os -mmcu=$2 -c $1.c ../LIBS/uart/uart.c ../LIBS/spi/spi.c ../LIBS/sram/sram.c  ../LIBS/serial/serial.c
avr-gcc -g -mmcu=$2 -Wl,-Map,$1.map -o $1.elf $1.o uart.o spi.o sram.o serial.o
avr-objdump -d -h $1.elf > $1.lst

avr-objcopy -j .text -j .data -O ihex $1.elf $1.hex
avr-objcopy -j .eeprom --change-section-lma .eeprom=0 -O ihex $1.elf $1_eeprom.hex
