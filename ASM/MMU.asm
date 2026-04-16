.global MMU_preparing_for_EL1h
.global L1_block_descriptor_NORMAL
.global L1_block_descriptor_DEVICE
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
    MOV X1, #(8ULL << 8)
    ORR X0, X0, X1
    MOV X1, #(68ULL << 16)
    ORR X0, X0, X1
    MSR MAIR_EL1, X0
    ISB

    MRS X0, TTBR0_EL1
    ADR X1, L1_table
    ORR X0, X0, X1
    MSR TTBR0_EL1, X0
    ISB

    MRS X0, TCR_EL1
    MOV X1, #(5ULL << 32)
    ORR X0, X0, X1
    AND X0, X0, #~(1ULL << 14)
    MOV X1, #(32ULL << 0)
    ORR X0, X0, X1
    MSR TCR_EL1, X0
    ISB

    STP X29, X30, [SP, #-16]!

    MOV X7, 0x00000000
    MOV X6, 0x3FFFFFFF
    ADR X0, L1_table
    MOV X2, XZR
    BL L1_block_descriptor_DEVICE
    MOV X2, XZR
    ADD X7, X7, 0x1
    BL L1_block_descriptor_NORMAL
    ADD X7, X7, 0x1
    MOV X2, XZR
    BL L1_block_descriptor_NORMAL

    DSB SY
    ISB

    LDP X29, X30, [SP], #16

    MRS X0, SCTLR_EL1
    ORR X0, X0, #(1ULL << 0)
    MSR SCTLR_EL1, X0

    DSB SY
    ISB

    RET

L1_block_descriptor_DEVICE:
    ORR X2, X2, #(1ULL << 0)
    AND X2, X2, #~(1ULL << 1)
    ORR X2, X2, #(0b001 << 2)
    AND X2, X2, #~(1ULL << 6)
    AND X2, X2, #~(1ULL << 7)
    AND X2, X2, #~(1ULL << 8)
    ORR X2, X2, #(1ULL << 10)
    ORR X2, X2, X7

    ADD X7, X7, X6
    STR X2, [X0], #8
    RET
L1_block_descriptor_NORMAL:
    ORR X2, X2, #(1ULL << 0)
    AND X2, X2, #~(1ULL << 1)
    ORR X2, X2, #(0b010 << 2)
    AND X2, X2, #~(1ULL << 6)
    AND X2, X2, #~(1ULL << 7)
    AND X2, X2, #~(1ULL << 8)
    ORR X2, X2, #(1ULL << 10)
    ORR X2, X2, X7

    ADD X7, X7, X6
    STR X2, [X0], #8
    RET
