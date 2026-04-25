.section .vectors, "ax", %progbits
.global vector_table_center
.global GIC_interrupts 

.align 11
vector_table_center:
    synchronous_elxt:
        ERET
    .align 7
    irq_handel_elxt:
        ERET
    .align 7
    fiq_handel_elxt:
        ERET
    .align 7
    serror_elxt:
        ERET
    .align 7
    synchronous_elxh:
        ERET
    .align 7
    irq_handel_elxh:
        STP X0, X1, [SP, #-16]!
        STP X2, X30, [SP, #-16]!

        BL GIC_interrupts

        LDP X2, X30, [SP], #16
        LDP X0, X1, [SP], #16

        ERET
