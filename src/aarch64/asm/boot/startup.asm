.section ".text.boot"
.global start
.global main

start:
    MRS X0, HCR_EL2
    ORR X0, X0, #(1ULL << 31) //RW - AArch64 всё ниже EL2
    AND X0, X0, #~(1ULL << 7) //Отключаем IRQ для EL2
    AND X0, X0, #~(1ULL << 4) //Запрещаем IRQ идти на EL2
    MSR HCR_EL2, X0

    MRS X0, SPSR_EL2
    MOV X1, #(5ULL << 0) //Указание того, что следующий уровень будет EL1h NS
    ORR X0, X0, X1
    AND X0, X0, #~(1ULL << 7)//Размаскирования бита I - это разрешает прерывания IRQ
    MSR SPSR_EL2, X0

    ADR X0, main
    MSR ELR_EL2, X0
    ISB

    ADR X0, EL1h_SP_top
    MSR SP_EL1, X0

    ADR X0, vector_table_center
    MSR VBAR_EL1, X0

    ERET


    