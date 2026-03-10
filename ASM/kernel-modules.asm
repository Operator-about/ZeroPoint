.global EL3_to_EL1
.global main_EL1
.global zero_PSTATE

EL3_to_EL1:

    //Конфигурация
    MRS X0, SCR_EL3
    //ORR X0, X0, #1 //NS
    ORR X0, X0, #(1 << 10) //RW
    MSR SCR_EL3, X0
    ISB

    //Сохранение значение регистра: сохранения статуса программы
    MOV X0, 0x3c5
    MSR SPSR_EL3, X0

    MOV X0, SP
    MSR SP_EL1, X0

    //Маршрут куда возвращаться
    MRS X0, ELR_EL3
    ADR X0, main_EL1 //Куда идти
    MSR ELR_EL3, X0
    ISB

    ERET

zero_PSTATE:
    MSR DAIF, X0 //Обнуление PSTATE
    MSR DAIFCLR, #2

    RET