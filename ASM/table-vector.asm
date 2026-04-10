.section .vectors, "ax", %progbits
.global vector_table_center
.global GIC_interrput 

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
        STR X0, [SP], #8
        STR X1, [SP], #8
        STR X2, [SP], #8
        STR X30, [SP], #8

        BL GIC_interrput

        LDR X30, [SP], #8
        LDR X2, [SP], #8
        LDR X1, [SP], #8
        LDR X0, [SP], #8

        ERET
