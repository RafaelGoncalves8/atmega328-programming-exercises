.org 0x0000 ; vetor de reset
jmp inicio

.org 0x0002 ; vetor INT0
jmp rsi_int0

.org 0x0034 ; primeiro endereço livre fora da área dos vetores de interrupção
inicio:
; Inicialização SREG = 0 e pilha no final da SRAM endereço 0x08FF
clr R1 ; R1 = 0
out 0x3F,R1 ; SREG <- 0
ser R28 ; R28 = 0xFF
ldi R29,0x08 ; R29 = 0x08
; Carrega o endereço final da SRAM (0x08FF) no stack pointer
out 0x3E,R29 ; SPH <- 0x08
out 0x3D,R28 ; SPL <- 0xFF

sbi 0x07,0 ; configura o pino A0 como saida

; Configurações da interrupção externa INT0
ldi R16,0x02
sts 0x0069,R16 ; EICRA <- 0x02 (INT0 ativada pela borda de descida)
sbi 0x1D,0 ; habilita INT0 setando o bit 0 do EIMSK (máscara da interrupção)
sei ; habilita as interrupções: seta o flag I do SREG

loop: ; laço infinito
rjmp loop

; rotina de serviço de interrupção
rsi_int0:
sbi 0x06,0 ; altera o estado do LED no pino A0
reti ; restataura o PC e retorna ao ponto do programa em que ocorreu a interrupção
