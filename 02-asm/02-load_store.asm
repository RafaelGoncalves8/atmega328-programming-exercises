.device ATmega328P

.org 0x0000     ; program starts at 0x0000 (value of PC after reset)
jmp main

.org 0x0034     ; first address after reserved portion for interruptions and vectors
main:

; configures pin A0 as output
lds r24,0x27
ori r24,0x01
sts 0x27,r24

; configures pin 7 as input
lds r24,0x2A
andi r24,0b0111111
sts 0x2A,r24

; configures internal pullup resistor
lds r24,0x29
ori r24,0b10000000
sts 0x29,r24

; turn off led initially
lds r24,0x28
andi r24,0b11111110
sts 0x28,r24

loop:

lds r16,0x29    ; read button
andi r16,0x80  ; apply mask
brbs 1, led_on  ; branch if button on

; turn off led
lds r24,0x28
andi r24,0b11111110
sts 0x28,r24

rjmp loop

led_on:

; turn on led
lds r24,0x28
ori r24,0x01
sts 0x28,r24

rjmp loop
