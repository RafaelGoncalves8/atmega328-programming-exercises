.device ATmega328P

; Define registers
.def rCounter  = r16
.def rCounterIn = r25
; .def rCounterInIn = r26
.def rAux = r24

; Define constants
.equ cInner = 0xFF
.equ cOuter = 0xFF
; .equ cInIn  = 0x0F

; program starts at 0x0000 (value of PC after reset)
.org 0x0000
jmp main

; first address after reserved portion for interruptions and vectors
.org 0x0034
main:

; configures pin 13 as output
lds rAux,0x24
ori rAux,0b00100000
sts 0x24,rAux

; configures pin A0 as input
lds rAux,0x27
andi rAux,0b1111110
sts 0x27,rAux

; configures internal pullup resistor
lds rAux,0x26
ori rAux,0b00000001
sts 0x26,rAux

main_loop:
; lds r16,0x26    ; read button
; andi r16,0x01   ; apply mask
; brbs 1, fast    ; branch if button on

loop:

; Led off
lds rAux,0x25            ; 2 clock cycle
andi rAux,0b11011111     ; 1 clock cycle
sts 0x25,rAux            ; 2 clock cycle

ldi rCounter, cOuter    ; 1 clock cycle

loop1:
ldi rCounterIn, cInner  ; 1 clock cycle

loop1i:
dec rCounterIn          ; 1 clock cycle
nop                     ; 1 clock cycle
nop                     ; 1 clock cycle
nop                     ; 1 clock cycle
nop                     ; 1 clock cycle
nop                     ; 1 clock cycle
nop                     ; 1 clock cycle
nop                     ; 1 clock cycle
nop                     ; 1 clock cycle
nop                     ; 1 clock cycle
nop                     ; 1 clock cycle
brne loop1i             ; if not zero: jmp(2 cycle), else: 1cycle
dec rCounter            ; 1 clock cycle
brne loop1              ; if not zero: jmp(2 cycle), else: 1cycle
nop                     ; 1 clock cycle

set led on
lds rAux,0x25            ; 2 clock cycle
ori rAux,0b00100000      ; 1 clock cycle
sts 0x25,rAux            ; 2 clock cycle

ldi rCounter, cOuter    ; 1 clock cycle

loop2:
ldi rcounterIn, cInner  ; 1 clock cycle

loop2i:
dec rCounterIn          ; 1 clock cycle
brne loop2i             ; if not zero: jmp(2 cycle), else: 1cycle
dec rCounter            ; 1 clock cycle
brne loop2              ; if not zero: jmp(2 cycle), else: 1cycle
nop                     ; 1 clock cycle

rjmp loop               ; 2 clock cycle


; ;set led on
; lds rAux,0x25            ; 2 clock cycle
; ori rAux,0b00100000      ; 1 clock cycle
; sts 0x25,rAux            ; 2 clock cycle
;
; ;set led off
; lds rAux,0x25            ; 2 clock cycle
; andi rAux,0b11011111     ; 1 clock cycle
; sts 0x25,rAux            ; 2 clock cycle
;
; rjmp loop
;
; led_off:
;
; lds rAux,0x25            ; 2 clock cycle
; andi rAux,0b11011111     ; 1 clock cycle
; sts 0x25,rAux            ; 2 clock cycle
;
; rjmp loop
;
; led_on:
;
; lds rAux,0x25            ; 2 clock cycle
; ori rAux,0b00100000      ; 1 clock cycle
; sts 0x25,rAux            ; 2 clock cycle
;
; rjmp loop
