.section .vectors, "ax", %progbits
.global vector_table_center
.global GIC_interrput 
.global synchronous_elxh_handel

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
        MOV X5, #1ULL
        ERET
    .align 7
    irq_handel_elxh:
        BL GIC_interrput
        ERET


    
    




    




