.section ".text.boot"
.global start
.global main_EL3


start:
    LDR X0, =EL3h_SP_top //Указание верхушки стека
    MOV SP, X0 //Задавание стека
    //Настройка таблицы-векторов
    ADR X0, vector_table_center //Взятие адреса таблицы векторов
    LSL X0, X0, #11 //Сдвиг адреса
    MSR VBAR_EL3, X0 //Загрузка таблицы векторов
    MOV X0, XZR

    BL main_EL3 //Переход в код


    