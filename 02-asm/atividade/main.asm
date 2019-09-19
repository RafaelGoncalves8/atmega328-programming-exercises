.device ATmega328P

.org 0x0000     ; program starts at 0x0000 (value of PC after reset)
jmp main

.org 0x0034     ; first address after reserved portion for interruptions and vectors
main:

; configures pin 13 as output
lds r24,0x24
ori r24,0b00100000
sts 0x24,r24

; configures pin A0 as input
lds r24,0x27
andi r24,0b1111110
sts 0x27,r24

; configures internal pullup resistor
lds r24,0x26
ori r24,0b00000001
sts 0x26,r24

; turn off led initially
lds r24,0x25
andi r24,0b11011111
sts 0x25,r24

loop:

; lds r16,0x26    ; read button
; andi r16,0x01  ; apply mask
; brbs 1, fast  ; branch if button on

;set led on
lds r24,0x25
ori r24,0b00100000
sts 0x25,r24

;set led off
lds r24,0x25
andi r24,0b11011111
sts 0x25,r24

rjmp loop

led_off:

lds r24,0x25
andi r24,0b11011111
sts 0x25,r24

rjmp loop

led_on:

lds r24,0x25
ori r24,0b00100000
sts 0x25,r24

rjmp loop
