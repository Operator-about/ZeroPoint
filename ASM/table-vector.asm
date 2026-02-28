.section .vectors
.global vector_table
.global GIC_interrput

.macro vector_entry label
    .align 7
    b \label
.endm    

.align 11
vector_table:
    
    vector_entry sync_invalid_el1t
    vector_entry irq_invalid_el1t
    vector_entry fiq_invalid_el1t
    vector_entry error_invalid_el1t

    vector_entry sync_handler_el1h
    vector_entry irg_handler //Обычные прерывания

    irg_handler:
        SUB SP, SP, #160

        STP X0, X1, [SP, #0] //Сохранение регистров X0-X1 в стек(SP регистр). STP - аналог str. Отступ +16
        STP X2, X3, [SP, #16]
        STP X4, X5, [SP, #32]
        STP X6, X7, [SP, #48]
        STP X8, X9, [SP, #64]
        STP X10, X11, [SP, #80]
        STP X12, X13, [SP, #96]
        STP X14, X15, [SP, #112]
        STP X16, X17, [SP, #128]
        STP X18, X19, [SP, #144]
        STP X20, X21, [SP, #160]
        STP X22, X23, [SP, #176]
        STP X24, X25, [SP, #192]
        STP X26, X27, [SP, #208]
        STP X28, X29, [SP, #224]
        STR X30, [SP, #240]
    
        MRS X0, ELR_EL1 //Чтение PC регистра, через ELR_EL1
        MRS X1, SPSR_EL1

        STP X0, X1, [SP, #248] //Сохранение PC регистра в X0(в SP регистр)

        MOV X0, SP //Передаём указатель на всю сохранёную информацию в C функцую
        BL GIC_interrput //Обработка прерывания

        //Возвращение значение из SP регистра. LDP - аналог ldr 
        LDP X0, X1, [SP, #248]
        MSR ELR_EL1, X0
        MSR SPSR_EL1, X1
        
        LDR X30, [SP, #240]
        LDP X28, X29, [SP, #224]
        LDP X26, X27, [SP, #208]
        LDP X24, X25, [SP, #192]
        LDP X22, X23, [SP, #176]
        LDP X20, X21, [SP, #160]
        LDP X18, X19, [SP, #144]
        LDP X16, X17, [SP, #128]
        LDP X14, X15, [SP, #112]
        LDP X12, X13, [SP, #96]
        LDP X10, X11,[SP, #80]
        LDP X8, X9, [SP, #64]
        LDP X6, X7, [SP, #48]
        LDP X4, X5, [SP, #32]
        LDP X2, X3, [SP, #16]
        LDP X0, X1, [SP, #0] 

        ADD SP, SP, #160
        ERET

    sync_invalid_el1t:
    irq_invalid_el1t:
    fiq_invalid_el1t:
    error_invalid_el1t:
    sync_handler_el1h:



    




