.section ".text.boot", "ax"
.global start
start:
    MOV X0, XZR
    ORR X0, X0, #(1ULL << 31)
    AND X0, X0, #~(1ULL << 7)
    AND X0, X0, #~(1ULL << 4)
    AND X0, X0, #~(1ULL << 27)
    MSR HCR_EL2, X0

    MOV X0, XZR
    MOV X1, #(5ULL << 0)
    ORR X0, X0, X1
    AND X0, X0, #~(1ULL << 4)
    ORR X0, X0, #(1ULL << 7)
    MSR SPSR_EL2, X0

    MOV X0, XZR

    LDR X0, =main
    MSR ELR_EL2, X0

    LDR X0, =EL1h_SP_bottom
    MSR SP_EL1, X0

    LDR X0, =vector_table_center
    MSR VBAR_EL1, X0

    ERET
.section .text
.global main
.global go_to_EL0
.global go_to_EL1
.global shell_init
.global clear_TTBR

go_to_EL0:
    MRS X0, SPSR_EL1
    AND X0, X0, #~(0xF << 0)
    AND X0, X0, #~(1ULL << 4)
    MSR SPSR_EL1, X0

    LDR X0, =0x40010250
    MSR ELR_EL1, X0

    LDR X0, =EL0t_SP_top
    MSR SP_EL0, X0

    ERET

clear_TTBR:
    TLBI VMALLE1IS
    DSB ISH

    ISB

    RET

.section .bss
.align 4
EL1h_SP_top:
    .SPACE 32768
EL1h_SP_bottom:
.align 4
EL0t_SP_top:
    .SPACE 32768
EL0t_SP_bottom:

    