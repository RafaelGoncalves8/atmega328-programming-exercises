SRC ?= main.asm
BIN  = "$(basename $(SRC))"

AVRA       = avra
AVRDUDE    = avrdude
PROGRAMMER = arduino

PGPORT   ?= /dev/ttyACM0
BAUDRATE ?= 57600
DEVICE    = atmega328p
FREQ      = 16000000

$(BIN).hex: $(SRC)
	$(AVRA) -fI $< -o $@
	rm *.eep.hex *.obj *.cof

burn: $(BIN).hex
	$(AVRDUDE) -c $(PROGRAMMER) -p $(DEVICE) -P $(PGPORT) -B $(BAUDRATE) -u -U flash:w:$<

monitor:
	$(PICOCOM) --send-cmd "ascii_xfr -s -v -l10" --nolock $(PGPORT)

clean:
	rm -f *.hex

.PHONY: burn monitor clean
