.section ".text.boot"
.global _start
.global main
.global switch_EL

_start:
    //Настройка таблицы-векторов
    LDR X0, =0x40101000
    MOV SP, X0
    ADR X0, vector_table
    MSR VBAR_EL1, X0 //Загрузка таблицы векторов
    MOV X0, #0
    MSR DAIF, X0 //Обнуление PSTATE
    MSR DAIFCLR, #2

    //MRS X0, MPIDR_EL1
    //AND X0, X0, #0xFF
    //CBZ X0, master
    BL main
hang:
    WFE
    B hang  
master: 
    LDR X0, =_stack_top
    MOV SP, X0
    LDR X0, =__bss_start
    LDR X1, =__bss_end
    BL main
bss_loop:
    CMP X0, X1
    B.GE setup_fpu
    STR XZR, [X0], #8
    B bss_loop
setup_fpu:
    MOV X0, #3 << 20
    MSR CPACR_EL1, X0
    ISB

switch_EL:
    MOV X1, X0

    //Конфигурация
    MRS X0, SCR_EL3
    ORR X0, X0, #1 //NS
    ORR X0, X0, #(1 << 10) //RW
    MSR SCR_EL3, X0
    ISB

    MRS X0, SPSR_EL3 //Сохранение значение регистра: сохранения статуса программы
    MSR SPSR_EL3, X0

    MRS X0, ELR_EL3
    MOV X0, X1
    MSR ELR_EL3, X0
    ISB

    ERET

    MSR DAIF, X0 //Обнуление PSTATE
    MSR DAIFCLR, #2
    


    