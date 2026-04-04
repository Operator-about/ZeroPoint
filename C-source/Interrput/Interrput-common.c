#include<Interrput.h>
#include<Kernel-modules.h>

void GIC_common_configure(volatile struct GIC_registers_data* _Registers_data, struct GIC _GIC){
    
    //Получение полного Aff для IROUTER
    volatile uint64_t _aff_local; //Локальная переменная для хранения временного Aff значения
    __asm__ volatile("MRS %0, MPIDR_EL1" : "=r"(_aff_local)); //Получение из MPDIR_EL1 параметров ядра
    __asm__ volatile("AND %0, %0, 0xFF" : "=r"(_aff_local) : "r"(_aff_local)); //Извлечение
    _Registers_data->AFF[0] = _aff_local; //Получение ядра для Aff
    _aff_local = (volatile uint64_t)0;

    __asm__ volatile("MRS %0, MPIDR_EL1" : "=r"(_aff_local)); //Получение из MPDIR_EL1 параметров ядра
    __asm__ volatile("LSR %0, %0, #8" : "=r"(_aff_local) : "r"(_aff_local)); //Сдвиг от начало регистра на 8, чтобы узнать про Aff1
    __asm__ volatile("AND %0, %0, 0xFF" : "=r"(_aff_local) : "r"(_aff_local)); //Извлечение
    _Registers_data->AFF[1] = _aff_local; //Получение ядра для Aff
    _aff_local = (volatile uint64_t)0;

    __asm__ volatile("MRS %0, MPIDR_EL1" : "=r"(_aff_local)); //Получение из MPDIR_EL1 параметров ядра
    __asm__ volatile("LSR %0, %0, #16" : "=r"(_aff_local) : "r"(_aff_local)); //Сдвиг от начало регистра на 16, чтобы узнать про Aff2
    __asm__ volatile("AND %0, %0, 0xFF" : "=r"(_aff_local) : "r"(_aff_local)); //Извлечение
    _Registers_data->AFF[2] = _aff_local; //Получение ядра для Aff
    _aff_local = (volatile uint64_t)0;

    __asm__ volatile("MRS %0, MPIDR_EL1" : "=r"(_aff_local)); //Получение из MPDIR_EL1 параметров ядра
    __asm__ volatile("LSR %0, %0, #32" : "=r"(_aff_local) : "r"(_aff_local)); //Сдвиг от начало регистра на 32, чтобы узнать про Aff3
    __asm__ volatile("AND %0, %0, 0xFF" : "=r"(_aff_local) : "r"(_aff_local)); //Извлечение
    _Registers_data->AFF[3] = _aff_local; //Получение ядра для Aff
    _aff_local = (volatile uint64_t)0;

    _Registers_data->PMR |= (0xFF << 0); //Установление самого низкого порога приоритета для прерывания
    _Registers_data->GROUP1ENABLE |= (1ULL << 0); //Включение группы 1(Non-Secure)
    _Registers_data->SRE |= (1ULL << 2) | (1ULL << 0) | (1ULL << 3); //По порядку: выключение обхода IRQ, включение регистров GICv3 для взаимодействвия
    _Registers_data->SRE &= ~(1ULL << 1); //включение обхода FIQ
    _Registers_data->CTLR |= (1ULL << 0) | (1ULL << 6); //Конфигурация GICv3

    __asm__ volatile("MSR ICC_CTLR_EL3, %0" : : "r"(_Registers_data->CTLR)); //Включение GIC моодуля для MRS/MSR
    __asm__ volatile("ISB");

    __asm__ volatile("MSR ICC_SRE_EL3, %0" : : "r"(_Registers_data->SRE)); //Включение GIC моодуля для MRS/MSR
    __asm__ volatile("ISB");

    __asm__ volatile("MSR ICC_IGRPEN1_EL3, %0" : : "r"(_Registers_data->GROUP1ENABLE)); //Включение группы 1 прерываний
    __asm__ volatile("ISB");

    __asm__ volatile("MSR ICC_PMR_EL1, %0" : : "r"(_Registers_data->PMR)); //Установление порога
    __asm__ volatile("ISB");

    /*
        0x0100 - ISENABLER
        0x0080 - IGROUPR
        0x0D00 - IGRPMODR
        0x0400 - IPRIORITY
        0x6000 - IROUTER
        0x0C00 - ICFGR
        0x0000 - CTLR
        0x0D00 - ICFGR
    */



    _GIC.GICD = (struct GICD*)0x08000000;
    _GIC.GICR[0] = (struct GICR*)0x080A0000;
    GICD_common_configure_EL3(_Registers_data, _GIC);
}

void GICD_common_configure_EL3(struct GIC_registers_data* _registers_data, struct GIC _GIC){
    _GIC.GICD->GICD_CTLR |= (1ULL << 5) | (1ULL << 4) | (1ULL << 1) | (1ULL << 0); //5 - ARE_NS, 4 - ARE_S, 1 - NS1
    _GIC.GICD->GICD_IGROUPR[33 / 32] |= (1ULL << (33 % 32)); //Указание группы прерывания
    _GIC.GICD->GICD_IGRPMODR[33 / 32] &= ~(1ULL << (33 % 32)); //Указания под-группы прерывания по безопасности
    _GIC.GICD->GICD_ICFGR[33 / 16] &= ~(1ULL << (33 % 32 * 2 + 1)); //Указания режима реагирования на прерывания
    //Указание пути к ядру для прерывания
    _GIC.GICD->GICD_IROUTER[33] |= (_registers_data->AFF[0] << 0) | (_registers_data->AFF[1] << 8) | (_registers_data->AFF[2] << 16) |
    (_registers_data->AFF[3] << 32);
    _GIC.GICD->GICD_IROUTER[33] &= ~(1ULL << 31); //Указание того, что прерывания пойдёт только на конкретное ядро, указанное ранее

    _GIC.GICR[0]->GICR_WAKER &= (1ULL << 1);
    while(_GIC.GICR[0]->GICR_WAKER & (1ULL << 2)){
        debugf("Children check...");
    }
}

void GICD_common_configure_EL1(struct GIC _GIC){


    __asm__ volatile("MSR ICC_IGRPEN1_EL1, %0" : : "r"(1)); //Включение группы 1 прерываний
    __asm__ volatile("ISB");

    __asm__ volatile("MSR ICC_PMR_EL1, %0" : : "r"(0xFF)); //Установление порога
    __asm__ volatile("ISB");


    _GIC.GICD->GICD_IPRIORITYR[33 / 4] |= (1ULL << (33 % 4 * 8)); //Указание приоритета для прерывания
    _GIC.GICD->GICD_ISENABLER[33 / 32] |= (1ULL << (33 % 32)); //Разрешения прерывания
}

void UART_common_configure(volatile struct UART* _UART){
    
    _UART = (struct UART*)0x09000000;

    //Подсчёт IBRD и FBRD
    struct BRR_UART _BRR = calculate_BRR(9600, 48000000);

    //Настройка параметров
    _UART->UART_CR = 0;
    _UART->UART_IBRD = _BRR.IBRD; 
    _UART->UART_FBRD = _BRR.FBRD;       
    _UART->UART_LCR_H &= (1ULL << 4); //Настройка параметров передачи данных
                      //Разрешение Rx //Разрешение Tx
    _UART->UART_IMSC |= (1ULL << 5) | (1ULL << 4);
    //*_UART->UART_IFLS &= (1ULL << 0); //Установление срабатывания FIFO при его заполнении на 1/2
    _UART->UART_CR = (1ULL << 0) | (1ULL << 8) | (1ULL << 9);
}