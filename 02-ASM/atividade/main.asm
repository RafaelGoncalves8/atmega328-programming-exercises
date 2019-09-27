.device ATmega328P

; Blink implemented trough 3 loops mediated by the variebles (registers)
; cCounterOut, cCounter and cCounterIn mediated by the values in
; rTmp, cOuter and cInner. The value in rTmp is determined by the
; values cFast and cSlow controlled by the variable rState that 
; indicates if the button is pressed (1) or not (0). The result is
; a blinking system that resonates at 1Mhz (slow) when the button
; is not pressed and in 5MHz (fast) when it is.

; Define registers
.def rCounter  = r16
.def rCounterIn = r23
.def rCounterOut = r24
.def rState = r25
.def rAux = r26
.def rTmp = r27

; Define constants
.equ cInner = 0xFF
.equ cOuter = 0xC4
.equ cFast = 0x04    ; 5.0 Hz
.equ cSlow = 0x14    ; 1.0 Hz
.equ cIsSlow = 0x00
.equ cIsFast = 0x01

; Expressão calculada para o tempo
; t = 1/(16 000 000)
; T = (((8*t)*cInner + 11*t)*cFast + 14*t)*cOut
; T = (((8*t)*cInner + 11*t)*cSlow + 14*t)*cOut
; 
; Obs: o ajuste fino da frequência foi feita utilizando um
; osciloscópio para medir o período.

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

fast:
ldi rTmp, cFast           ; 1 clock cycle
ldi rState, cIsFast       ; 1 clock cycle
jmp loop                  ; 2 clock cycle

slow:
ldi rTmp, cSlow           ; 1 clock cycle
ldi rState, cIsSlow       ; 1 clock cycle

loop:

; Led offfast
lds rAux,0x25            ; 2 clock cycle
andi rAux,0b11011111     ; 1 clock cycle
sts 0x25,rAux            ; 2 clock cycle

mov rCounterOut, rTmp    ; 1 clock cycle

loop1o:

ldi rCounter, cOuter    ; 1 clock cycle

loop1:
nop                     ; 1 clock cycle

ldi rCounterIn, cInner  ; 1 clock cycle

loop1i:
lds rAux,0x26           ; 2 clock cycle
andi rAux, 0x01         ; 1 clock cycle
cp rAux, rState         ; 1 clock cycle
breq swap               ; if not equal: jmp(2 cycle), else: 1cycle

dec rCounterIn          ; 1 clock cycle
brne loop1i             ; if not zero: jmp(2 cycle), else: 1cycle
dec rCounter            ; 1 clock cycle
brne loop1              ; if not zero: jmp(2 cycle), else: 1cycle
dec rCounterOut         ; 1 clock cycle
brne loop1o             ; if not zero: jmp(2 cycle), else: 1cycle

; set led on
lds rAux,0x25            ; 2 clock cycle
ori rAux,0b00100000      ; 1 clock cycle
sts 0x25,rAux            ; 2 clock cycle

mov rCounterOut, rTmp   ; 1 clock cycle

loop2o:
ldi rCounter, cOuter    ; 1 clock cycle

loop2:
nop                     ; 1 clock cycle
ldi rCounterIn, cInner  ; 1 clock cycle

loop2i:
lds rAux,0x26           ; 2 clock cycle
andi rAux, 0x01         ; 1 clock cycle
cp rAux, rState         ; 1 clock cycle
breq swap               ; if not zero: jmp(2 cycle), else: 1cycle

dec rCounterIn          ; 1 clock cycle
brne loop2i             ; if not zero: jmp(2 cycle), else: 1cycle
dec rCounter            ; 1 clock cycle
brne loop2              ; if not zero: jmp(2 cycle), else: 1cycle
dec rCounterOut         ; 1 clock cycle
brne loop2o             ; if not zero: jmp(2 cycle), else: 1cycle

rjmp loop               ; 2 clock cycle

swap:
andi rState, cIsFast    ; 1 clock cycle
brbs 1, fast            ; if not zero: jmp(2 cycle), else: 1cycle
rjmp slow               ; 2 clock cycle
