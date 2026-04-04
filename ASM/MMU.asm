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
    MRS X0, MAIR_EL1 //Получения конфигурации атрибутов
    MOV X1, #(17ULL << 0)
    ORR X0, X0, X1 //Запись атрибута GRE
    MSR MAIR_EL1, X0 //Запись...

    MRS X0, TCR_EL1 //Получения начальной конфигурации виртуального мира
    MOV X1, #(25ULL << 0)
    ORR X0, X0, X1 //Запись размера виртуального мира
    MOV X1, #(0b00 << 14)
    ORR X0, X0, X1 //Запись гранулы для таблиц L(в данном случаи выставлено 4КБ)
    MSR TCR_EL1, X0 //Запись...

    MRS X0, TTBR0_EL1 //Получения конфигурации для настройки адреса первоначальной L-таблицы(L0, L1)
    ADR X1, L1_table //Получения адреса L1 таблицы
    LSL X1, X1, #1 //Сдвиг адреса в X1 от начала на 5 бит
    ORR X0, X0, X1 //Запись значения
    MSR TTBR0_EL1, X0 //Запись...
    MOV X1, XZR //Обнуления X1 для последущей с ним работы

    ADR X0, L1_table //Получения адреса L1 таблицы
    // ADR X1, L2_table //Получения адреса L2 таблицы
    // STR X1, [X0, #0] //Запись указателя на L2 в L1 таблицу
    // MOV X1, XZR //Обнуления X1 для последущей с ним работы

    MOV X1, #0x40000000
    MOV X2, XZR
    BL L1_block_descriptor

    MRS X0, SCTLR_EL1 //Получения конфигурации настройки MMU
    ORR X0, X0, #(1ULL << 0) //Включение MMU(M)
    AND X0, X0, #~(1ULL << 2) //Выключение кэширования MMU(C)
    MSR SCTLR_EL1, X0 //Запись

    ISB

    RET

L1_block_descriptor:
    /*
        Про использование регистров:
        X0 - сама L1 таблица
        X1 - OA(Output Address)
        X2 - сам дескриптор 
    */
    ORR X2, X2, #(1ULL << 0)
    AND X2, X2, #~(1ULL << 0)
    MOV X4, #(0b10001 << 2)
    ORR X2, X2, X4
    AND X2, X2, #~(1ULL << 5)
    ORR X2, X2, #(1ULL << 6)
    AND X2, X2, #~(1ULL << 7)
    AND X2, X2, #~(1ULL << 8)
    ORR X2, X2, #(1ULL << 10)
    AND X2, X2, #~(1ULL << 11)
    LSL X1, X1, #12
    ORR X2, X2, X1

    ADD X1, X1, #0x4
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