#include<Interrput.h>
#include<Kernel-modules.h>

void GIC_common_configure_registers(struct GIC_registers_data* _registers_data){
    
    //Получение полного Aff для IROUTER
    volatile uint64_t _aff_local; //Локальная переменная для хранения временного Aff значения
    __asm__ volatile("MRS %0, MPIDR_EL1" : "=r"(_aff_local)); //Получение из MPDIR_EL1 параметров ядра
    __asm__ volatile("AND %0, %0, 0xFF" : "=r"(_aff_local) : "r"(_aff_local)); //Извлечение
    _registers_data->AFF[0] = _aff_local; //Получение ядра для Aff
    _aff_local = (volatile uint64_t)0;

    __asm__ volatile("MRS %0, MPIDR_EL1" : "=r"(_aff_local)); //Получение из MPDIR_EL1 параметров ядра
    __asm__ volatile("LSR %0, %0, #8" : "=r"(_aff_local) : "r"(_aff_local)); //Сдвиг от начало регистра на 8, чтобы узнать про Aff1
    __asm__ volatile("AND %0, %0, 0xFF" : "=r"(_aff_local) : "r"(_aff_local)); //Извлечение
    _registers_data->AFF[1] = _aff_local; //Получение ядра для Aff
    _aff_local = (volatile uint64_t)0;

    __asm__ volatile("MRS %0, MPIDR_EL1" : "=r"(_aff_local)); //Получение из MPDIR_EL1 параметров ядра
    __asm__ volatile("LSR %0, %0, #16" : "=r"(_aff_local) : "r"(_aff_local)); //Сдвиг от начало регистра на 16, чтобы узнать про Aff2
    __asm__ volatile("AND %0, %0, 0xFF" : "=r"(_aff_local) : "r"(_aff_local)); //Извлечение
    _registers_data->AFF[2] = _aff_local; //Получение ядра для Aff
    _aff_local = (volatile uint64_t)0;

    __asm__ volatile("MRS %0, MPIDR_EL1" : "=r"(_aff_local)); //Получение из MPDIR_EL1 параметров ядра
    __asm__ volatile("LSR %0, %0, #32" : "=r"(_aff_local) : "r"(_aff_local)); //Сдвиг от начало регистра на 32, чтобы узнать про Aff3
    __asm__ volatile("AND %0, %0, 0xFF" : "=r"(_aff_local) : "r"(_aff_local)); //Извлечение
    _registers_data->AFF[3] = _aff_local; //Получение ядра для Aff
    _aff_local = (volatile uint64_t)0;

    _registers_data->PMR |= (0xFF << 0); //Установление самого низкого порога приоритета для прерывания
    _registers_data->GROUP1ENABLE |= (1ULL << 0); //Включение группы 1(Non-Secure)
    _registers_data->SRE |= (1ULL << 2) | (1ULL << 0) | (1ULL << 3); //По порядку: выключение обхода IRQ, включение регистров GICv3 для взаимодействвия
    _registers_data->SRE &= ~(1ULL << 1); //включение обхода FIQ
    _registers_data->CTLR |= (1ULL << 0) | (1ULL << 6)  | (1ULL << 4); //Конфигурация GICv3
    _registers_data->GROUP0ENABLE |= (1ULL << 0); //Разрешение 0 группы

    __asm__ volatile("MSR ICC_CTLR_EL3, %0" : : "r"(_registers_data->CTLR)); //Включение GIC моодуля для MRS/MSR
    __asm__ volatile("ISB");

    __asm__ volatile("MSR ICC_SRE_EL3, %0" : : "r"(_registers_data->SRE)); //Включение GIC моодуля для MRS/MSR
    __asm__ volatile("ISB");

    __asm__ volatile("MSR ICC_IGRPEN1_EL3, %0" : : "r"(_registers_data->GROUP1ENABLE)); //Включение группы 1 прерываний
    __asm__ volatile("ISB");

    __asm__ volatile("MSR ICC_PMR_EL1, %0" : : "r"(_registers_data->PMR)); //Установление порога
    __asm__ volatile("ISB");

    __asm__ volatile("MSR ICC_IGRPEN0_EL1, %0" : : "r"(_registers_data->GROUP0ENABLE)); //Разрешение 0 группы прерываний
    __asm__ volatile("ISB");
}

void GICD_common_configure_EL3(struct GIC_registers_data* _registers_data){
    GIC.GICD->GICD_CTLR |= (1ULL << 5) | (1ULL << 4) | (1ULL << 1) | (1ULL << 0); //Включение типов прерываний
    GIC.GICD->GICD_IGROUPR[33 / 32] |= (1ULL << (33 % 32)); //Указание группы прерывания
    GIC.GICD->GICD_IGRPMODR[33 / 32] &= ~(1ULL << (33 % 32)); //Указания под-группы прерывания по безопасности
    GIC.GICD->GICD_ICFGR[33 / 16] &= ~(1ULL << (33 % 32 * 2 + 1)); //Указания режима реагирования на прерывания
    //Указание пути к ядру для прерывания
    GIC.GICD->GICD_IROUTER[33] |= (_registers_data->AFF[0] << 0) | (_registers_data->AFF[1] << 8) | (_registers_data->AFF[2] << 16) |
    (_registers_data->AFF[3] << 32);
    GIC.GICD->GICD_IROUTER[33] &= ~(1ULL << 31); //Указание того, что прерывания пойдёт только на конкретное ядро, указанное ранее

    GIC.GICR[0]->GICR_WAKER &= ~(1ULL << 1); //Запуск GICR 
    while(GIC.GICR[0]->GICR_WAKER & (1ULL << 2)){
        __asm__("NOP");
    }

    GIC.GICD->GICD_IPRIORITYR[33 / 4] |= (1ULL << (33 % 4 * 8)); //Указание приоритета для прерывания
    GIC.GICD->GICD_ISENABLER[33 / 32] |= (1ULL << (33 % 32)); //Разрешения прерывания
}

void GICD_common_configure_EL1(struct GIC_registers_data* _registers_data){
    __asm__ volatile("MSR ICC_IGRPEN1_EL1, %0" : : "r"(_registers_data->GROUP1ENABLE)); //Включение группы 1 прерываний
    __asm__ volatile("ISB");

    __asm__ volatile("MSR ICC_PMR_EL1, %0" : : "r"(_registers_data->PMR)); //Установление порога
    __asm__ volatile("ISB");
}

void UART_common_configure(){
    struct BRR_UART _BRR = calculate_BRR(9600, 480000000);

    UART->UART_IBRD = _BRR.IBRD; //Настройка скорости
    UART->UART_FBRD = _BRR.FBRD; //Настройка скорости
    UART->UART_LCR_H &= ~(1ULL << 4); //Отключение FIFO
    UART->UART_IMSC |= (1ULL << 5); //Включение прерывания на Tx линию
    UART->UART_CR |= (1ULL << 8); //Включение Tx линии
    UART->UART_CR |= (1ULL << 0); //Включение UART модуля
}