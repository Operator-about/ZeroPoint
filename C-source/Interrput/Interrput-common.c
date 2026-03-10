#include<Interrput.h>
#include<Kernel-modules.h>

void GIC_common_configure(volatile struct GIC_registers_data* _Registers_data, volatile struct GIC* _GIC){
    
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


    //Описание данных регистров см. в файле -> Custom-type.h
    _GIC->GICD_CTLR = (volatile uint32_t*)(0x08000000 + 0x0100);
    _GIC->GICD_IGROUPR[33 / 32] = (volatile uint32_t*)(0x08000000 + 0x0080 + (33 / 32 * 4));
    _GIC->GICD_IGRPMODR[33 / 32] = (volatile uint32_t*)(0x08000000 + 0x0D00 + (33 / 32 * 4));
    _GIC->GICD_ICFGR[33 / 16] = (volatile uint32_t*)(0x08000000 + 0x0C00 + (33 / 16 * 4));
    _GIC->GICD_IROUTER[33] = (volatile uint64_t*)(0x08000000 + 0x6000 + (33 * 8));
    _GIC->GICD_IPRIORITY[33 / 4] = (volatile uint32_t*)(0x08000000 + 0x0400 + (33 / 4 * 4));
    _GIC->GICD_ISENABLER[33 / 32] = (volatile uint32_t*)(0x08000000 + 0x0100 + (33 / 32 * 4));

    //Растёч GICR для определённого ядра, формула: 0x080A0000 + (N * 0x20000) -> где N - это ядро
    _GIC->GICRS[0].GICR = (volatile uint32_t*)(0x080A0000 + (0 * 0x20000));
    _GIC->GICRS[0].GICR_WAKER = (volatile uint32_t*)(0x080A0000 + 0x0014);

    *_GIC->GICRS[0].GICR_WAKER &= ~(1ULL << 1);

    GICD_common_configure_EL3(_Registers_data, _GIC);
}

void GICD_common_configure_EL3(volatile struct GIC_registers_data* _registers_data, volatile struct GIC* _GIC){
    *_GIC->GICD_CTLR |= (1ULL << 5) | (1ULL << 4) | (1ULL << 1); //5 - ARE_NS, 4 - ARE_S, 1 - NS1
    *_GIC->GICD_IGROUPR[33 / 32] |= (1ULL << (33 % 32)); //Указание группы прерывания
    *_GIC->GICD_IGRPMODR[33 / 32] &= ~(1ULL << (33 % 32)); //Указания под-группы прерывания по безопасности
    *_GIC->GICD_ICFGR[33 / 16] &= ~(1ULL << (33 % 32 * 2 + 1)); //Указания режима реагирования на прерывания
    //Указание пути к ядру для прерывания
    *_GIC->GICD_IROUTER[33] |= (_registers_data->AFF[0] << 0) | (_registers_data->AFF[1] << 8) | (_registers_data->AFF[2] << 16) |
    (_registers_data->AFF[3] << 32);
    *_GIC->GICD_IROUTER[33] &= ~(1ULL << 31); //Указание того, что прерывания пойдёт только на конкретное ядро, указанное ранее
}

void GICD_common_configure_EL1(volatile struct GIC* _GIC){
    *_GIC->GICD_IPRIORITY[33 / 4] |= (1ULL << (33 % 4 * 8)); //Указание приоритета для прерывания
    *_GIC->GICD_ISENABLER[33 / 32] |= (1ULL << (33 % 32)); //Разрешения прерывания
}

void check_UART_avaible_common(volatile struct UART* _UART){
    //Проверка на то: лежит в этом адресе, что-то
    _UART->UART = (volatile uint32_t*)0x09000000; //Уазание обычного адреса UART
    _UART->UART_CR = (volatile uint32_t*)(0x09000000 + 0x030 / 4); //Указание адреса для регистра конфигурации UART
    _UART->UART_DR = (volatile uint32_t*)(0x09000000 + 0x00 / 4); //Указание адреса для регистра отправки/прочтения данных(Tx и Rx линии) 
    _UART->UART_FR = (volatile uint32_t*)(0x09000000 + 0x18 / 4); //Указание адреса для регистра статуса UART
    //Указание адресов для стабилизации скорости передачи UART
    _UART->UART_FBRD = (volatile uint32_t*)(0x09000000 + 0x024 / 4); //Указание адреса для регистра корректировки IBRD
    _UART->UART_IBRD = (volatile uint32_t*)(0x09000000 + 0x028 / 4); //Указание адреса для регистра скорости передачи UART
    _UART->UART_LCR_H = (volatile uint32_t*)(0x09000000 + 0x02C / 4); //Указание адреса для регистра настройки передачи
    _UART->UART_MIS = (volatile uint32_t*)(0x09000000 + 0x040 / 4); //Указание адреса для регистра "оповещаещего" о прерывании в UART
    _UART->UART_ICR = (volatile uint32_t*)(0x09000000 + 0x044 / 4); //Указание адреса для регистра завершения прерывания в UART
    _UART->UART_IFLS = (volatile uint32_t*)(0x09000000 + 0x034 / 4); //Указание адреса для регистра реагирования на FIFO
    _UART->UART_IMSC = (volatile uint32_t*)(0x09000000 + 0x38 / 4); //Указание адреса для регистра разрешения определённых прерываний в UART
    _UART->interrput = (volatile uint32_t)33;
}

void UART_common_configure(volatile struct UART* _UART){
    
    //Подсчёт IBRD и FBRD
    struct BRR_UART _BRR = calculate_BRR(9600, 48000000);

    //Настройка параметров
    *_UART->UART_CR = 0;
    *_UART->UART_IBRD = _BRR.IBRD; 
    *_UART->UART_FBRD = _BRR.FBRD;       
    *_UART->UART_LCR_H = (1 << 4) | (3 << 5); //Настройка параметров передачи данных
                      //Разрешение Rx //Разрешение Tx
    //*_UART->UART_IMSC = (1 << 5) | (1 << 4);
    *_UART->UART_CR = (1 << 0) | (1 << 8) | (1 << 9);
}