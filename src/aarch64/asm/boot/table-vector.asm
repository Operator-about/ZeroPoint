.section .vectors, "ax", %progbits
.global vector_table_center
.global GIC_interrupts 

//Должна быть выровнена по 2КБ
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
        STP X2, X3, [SP, #-16]!
        STP X4, X5, [SP, #-16]!
        STP X6, X7, [SP, #-16]!
        STP X8, X9, [SP, #-16]! 
        STP X10, X30, [SP, #-16]!

        BL GIC_interrupts

        LDP X10, X30, [SP], #16
        LDP X9, X8, [SP], #16
        LDP X7, X6, [SP], #16
        LDP X5, X4, [SP], #16
        LDP X2, X3, [SP], #16
        LDP X0, X1, [SP], #16

        ERET
