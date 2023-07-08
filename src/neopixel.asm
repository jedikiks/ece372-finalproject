; export symbols
            XDEF Entry, _Startup            ; export 'Entry' symbol
            ABSENTRY Entry        ; for absolute assembly: mark this as application entry point



; Include derivative-specific definitions 
		INCLUDE 'derivative.inc' 

ROMStart    EQU  $4000  ; absolute address to place my code/constant data

PIXELS      EQU 96*8 ; Number of pixels in the string
T1H         EQU 900 ; Width of a 1 bit in ns
T1L         EQU 600 ; Width of a 1 bit in ns
T0H         EQU 400 ; Width of a 0 bit in ns
T0L         EQU 900 ; Width of a 0 bit in ns
RES         EQU 250000 ; Width of the low gap between bits to cause a frame to latch

;; variable/data section
            ORG RAMStart
R1:         DS.B 1
R2:         DS.B 1
R3:         DS.W 1


;; code section

; Interupt vectors
            ORG   $FFFE
            DC.W  Entry           ; Reset Vector

            ORG   ROMStart
Entry:
_Startup:
            LDS   #RAMEnd+1       ; initialize the stack pointer

            JSR main

us_delay:                         ; this is actually a 10us delay
            MOVW wait,R3          ; divide params by 10

        L_ud_3:
            MOVB #1,R2

        L_ud_2:
            MOVB #24,R1

        L_ud_1:
            NOP
            NOP
            NOP
            DEC R1
            BNE L_ud_1
            DEC R2
            BNE L_ud_2
            DEC (R3 + 1)
            BNE L_ud_3

            RTS

show:
            MOVW #(((RES / 1000) + 1) / 10),wait
            JSR us_delay

            RTC

show_color:                     ; Lights all LEDS with 1 color
            CLI
            PSHX
            LDX #PIXELS

        L_sc_1:
            JSR send_pixel
            DEX
            BNE L_sc_1

            SEI
            CALL show
            PULX

            RTS

send_pixel:
            MOVB RGB_g,byte
            JSR send_byte
            MOVB RGB_r,byte
            JSR send_byte
            MOVB RGB_b,byte
            JSR send_byte

            RTS

send_byte:
            PSHA
            PSHB
            MOVB #8,sB_count

        L_sB_1:
            CLRA
            LSL byte
            ROLA
            JSR send_bit

            DEC sB_count
            BNE L_sB_1

            PULB
            PULA

            RTS

send_bit:
            PSHA
            TSTA
            BNE sb_is_one

    ;; 0 bit:
            MOVW #((T0H - 2) / 10),wait
            BSET PORTA,%00000001
            JSR us_delay

            MOVW #((T0L - 2) / 10),wait
            BCLR PORTA,%00000001
            JSR us_delay

            BRA sb_done

    ;; 1 bit:
        sb_is_one:
            MOVW #((T1H - 2) / 10),wait
            BSET PORTA,%00000001
            JSR us_delay

            MOVW #((T1L - 2) / 10),wait
            BCLR PORTA,%00000001
            JSR us_delay

        sb_done:
            PULA
            RTS

main:
            BSET DDRA,%00000001

        main_loop:
            MOVB #$FF,RGB_r                 ; Make LEDs all white
            MOVB #$FF,RGB_g
            MOVB #$FF,RGB_b
            JSR show_color

            LDAB #16                        ; 1 second delay
            MOVW #(62500 / 10),wait
        L_main_1:
            JSR us_delay
            DECB
            BNE L_main_1

            MOVB #$00,RGB_r                 ; Turn off LEDs
            MOVB #$00,RGB_g
            MOVB #$00,RGB_b
            JSR show_color

            BRA main_loop

byte:       DS.B 1
RGB_r:      DS.B 1
RGB_g:      DS.B 1
RGB_b:      DS.B 1
wait:       DS.W 1
sB_count:   DS.B 1

            END
