.global vector_table
.global GIC_interrput

//Заглушка
.align 128
current_EL:

//Обработка ошибок(SVE)
.align 128
current_EL_spx: 
    

//Обработка прерываний
.align 128
lower_EL:
    STP X0, X1 [SP, #0] //Сохранение регистров X0-X1 в стек(SP регистр). STP - аналог str
    STP X2, X3 [SP, #16]
    STP X4, X5 [SP, #32]
    STP X6, X7 [SP, #48]
    STP X8, X9 [SP, #64]
    STP X10, X11[SP, #80]
    
    MRS X0, ELR_EL1 //Чтение PC регистра, через ELR_EL1

    STR X0, SP [#96] //Сохранение PC регистра в X0(в SP регистр)

    MOV X0, SP //Передаём указатель на всю сохранёную информацию в C функцую
    B GIC_interrput //Обработка прерывания

    //Возвращение значение из SP регистра. LDP - аналог ldr 
    LDP X0, X1 [SP, #0]
    LDP X2, X3 [SP, #16]
    LDP X3, X5 [SP, #32]


    




