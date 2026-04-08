.section ".text.boot"
.global start
.global main_EL3
.global EL1h_SP_reserve_top


start:
    
    LDR X0, =EL3_SP_reserve_top
    MOV SP, X0
    //Настройка таблицы-векторов
    ADR X0, vector_table_center
    LSL X0, X0, #11
    MSR VBAR_EL3, X0 //Загрузка таблицы векторов
    //MSR VBAR_EL1, X0
    MOV X0, XZR

    BL main_EL3


.section .bss
.align 15
EL3_SP_reserve_bottom:
    .SPACE 32768
EL3_SP_reserve_top:

.align 15
EL1h_SP_reserve_bottom:
    .SPACE 32768
EL1h_SP_reserve_top:
    


    