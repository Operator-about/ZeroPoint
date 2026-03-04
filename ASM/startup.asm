.section ".text.boot"
.global _start
.global main

_start:
    //Настройка таблицы-векторов
    LDR X0, =0x40101000
    MOV SP, X0
    ADR X0, vector_table
    MSR VBAR_EL1, X0 //Загрузка таблицы векторов
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

    


    