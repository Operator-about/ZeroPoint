.section ".text.boot"
.global start
.global main_EL3
.global EL1h_SP_reserve_top


start:
    
    LDR X0, =EL3_SP_reserve_top //Указание верхушки стека
    MOV SP, X0 //Задавание стека
    //Настройка таблицы-векторов
    ADR X0, vector_table_center //Взятие адреса таблицы векторов
    LSL X0, X0, #11 //Сдвиг адреса
    MSR VBAR_EL3, X0 //Загрузка таблицы векторов
    MOV X0, XZR

    BL main_EL3 //Переход в код


//Стек должен быть выравнен по 16 байт!
.section .bss
.align 15
EL3_SP_reserve_bottom:
    .SPACE 32768
EL3_SP_reserve_top:

.align 15
EL1h_SP_reserve_bottom:
    .SPACE 32768
EL1h_SP_reserve_top:
    


    