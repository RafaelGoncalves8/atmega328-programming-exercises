BIN  = main
OBJS = 03-finite-states_machine.o

CC         = avr-gcc
OBJCOPY    = avr-objcopy
OBJDUMP    = avr-objdump
AVRDUDE    = avrdude
PROGRAMMER = arduino

PGPORT   ?= /dev/ttyACM0
BAUDRATE ?= 57600
DEVICE    = atmega328p
OPTIMIZE  = -O0
FREQ      = 16000000

OPTIONS = -fpack-struct -fshort-enums
CFLAGS=-Wall -gdwarf-2 -DF_CPU=$(FREQ)  -mmcu=$(DEVICE) $(OPTIMIZE)

$(BIN).hex: $(BIN).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

$(BIN).elf: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

burn: ${BIN}.hex
	$(AVRDUDE) -c $(PROGRAMMER) -p $(DEVICE) -P $(PGPORT) -B $(BAUDRATE) -u -U flash:w:$<

clean:
	rm -f $(BIN).elf $(BIN).hex $(OBJS)
