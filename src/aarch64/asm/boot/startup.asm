.section ".text.boot", "ax"
.global start
start:

    LDR X0, =0xFE201000
    MOV W4, 0x58
    STR W4, [X0]

    ADR X0, vector_table_center
    MSR VBAR_EL1, X0

    ADR X0, EL1h_SP_bottom
    MSR SP_EL1, X0

    BL main
    
.section .text
.global main

    