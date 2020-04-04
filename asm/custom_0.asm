    processor 6502
    org $e000
    BYTE "CUSTOM"

    ; Empty space :-)

    org $fce2

flipbit:
    lda #$55
    nop
    lda #$aa
    nop
    jmp flipbit

