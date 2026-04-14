.global L1_table
.global L2_table
.global MMU_preparing_for_EL1h
.global L1_block_descriptor
.global L1_L2_descriptor
/*
    ПРИМИЧАНИЯ!
    L1_write и L2_write - по сути дела являются лишь циклами for, для заполнения соответстующих их названию L-таблиц 
    используются L1_write_descriptor и L2_write_descriptor.
    Так же, изначально L1_table и L2_table нечего в себя не хранят со старта. Их заполнение происходит в L1_write и L2_write
    соответственно.
*/
MMU_preparing_for_EL1h:
    MRS X0, MAIR_EL1
    MOV X1, #(8ULL << 0)
    ORR X0, X0, X1
    MOV X1, #(68ULL << 8)
    ORR X0, X0, X1
    MSR MAIR_EL1, X0
    ISB

    MRS X0, TTBR0_EL1
    ADR X1, L1_table
    LSL X1, X1, #5
    ORR X0, X0, X1
    MSR TTBR0_EL1, X0
    ISB

    MRS X0, TCR_EL1
    MOV X1, #(5ULL << 32)
    ORR X0, X0, X1
    AND X0, X0, #~(1ULL << 14)
    MOV X1, #(26ULL << 0)
    ORR X0, X0, X1
    MSR TCR_EL1, X0
    ISB

    MOV X7, 0x41000000
    LSR X7, X7, #3
    ADR X0, L1_table
    //BL L1_block_descriptor

    RET

L1_block_descriptor:
    /*
        Про использование регистров:
        X0 - сама L1 таблица
        X1 - OA(Output Address)
        X2 - сам дескриптор 
    */
    ORR X2, X2, #(1ULL << 0)
    AND X2, X2, #~(1ULL << 1)
    AND X2, X2, #~(1ULL << 2)
    AND X2, X2, #~(1ULL << 6)
    AND X2, X2, #~(1ULL << 7)
    AND X2, X2, #~(1ULL << 8)
    ORR X2, X2, #(1ULL << 10)
    LSL X7, X7, #12
    ORR X2, X2, X1

    MOV X6, #0x40000000
    ADD X7, X7, X6
    STR X2, [X0], #8
    RET

//Выделение мест под Lx-таблицы
.section .bss
.align 12
L1_table:
    .SPACE 4096
.align 12
L2_table:
    .SPACE 4096
.align 12
L3_table:
    .SPACE 4096