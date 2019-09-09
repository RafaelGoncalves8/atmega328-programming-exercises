.device ATmega328P

.org 0x0000     ; program starts at 0x0000 (value of PC after reset)
jmp main

.org 0x0034     ; first address after reserved portion for interruptions and vectors
main:

sbi 0x07,0      ; configures pin A0 as output
cbi 0x0A,7      ; configures pin 7 as input
cbi 0x08,0      ; turn off led initially

loop:

in r16, 0x09    ; read button
andi r16, 0x80  ; apply mask
brbs 1, led_on  ; branch if button on
cbi 0x08,0      ; turn off led
rjmp loop

led_on:

sbi 0x08,0      ; turn led on
rjmp loop
