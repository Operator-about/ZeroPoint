#include<Interrput.h>

void GIC_common_configure(volatile struct GIC_registers_data* _Registers_data){
    
    //Получение полного Aff для IROUTER
    volatile uint64_t _aff_local; //Локальная переменная для хранения временного Aff значения
    __asm__ volatile("MRS %0, MPIDR_EL1" : "=r"(_aff_local)); //Получение из MPDIR_EL1 параметров ядра
    __asm__ volatile("AND %0, %0, 0xFF" : "=r"(_aff_local) : "r"(_aff_local)); //Извлечение
    _Registers_data->Aff_data.Aff0 = _aff_local; //Получение ядра для Aff
    _aff_local = (volatile uint64_t)0;

    __asm__ volatile("MRS %0, MPIDR_EL1" : "=r"(_aff_local)); //Получение из MPDIR_EL1 параметров ядра
    __asm__ volatile("LSR %0, %0, #8" : "=r"(_aff_local) : "r"(_aff_local)); //Сдвиг от начало регистра на 8, чтобы узнать про Aff1
    __asm__ volatile("AND %0, %0, 0xFF" : "=r"(_aff_local) : "r"(_aff_local)); //Извлечение
    _Registers_data->Aff_data.Aff1 = _aff_local; //Получение ядра для Aff
    _aff_local = (volatile uint64_t)0;

    __asm__ volatile("MRS %0, MPIDR_EL1" : "=r"(_aff_local)); //Получение из MPDIR_EL1 параметров ядра
    __asm__ volatile("LSR %0, %0, #16" : "=r"(_aff_local) : "r"(_aff_local)); //Сдвиг от начало регистра на 16, чтобы узнать про Aff2
    __asm__ volatile("AND %0, %0, 0xFF" : "=r"(_aff_local) : "r"(_aff_local)); //Извлечение
    _Registers_data->Aff_data.Aff2 = _aff_local; //Получение ядра для Aff
    _aff_local = (volatile uint64_t)0;

    __asm__ volatile("MRS %0, MPIDR_EL1" : "=r"(_aff_local)); //Получение из MPDIR_EL1 параметров ядра
    __asm__ volatile("LSR %0, %0, #32" : "=r"(_aff_local) : "r"(_aff_local)); //Сдвиг от начало регистра на 32, чтобы узнать про Aff3
    __asm__ volatile("AND %0, %0, 0xFF" : "=r"(_aff_local) : "r"(_aff_local)); //Извлечение
    _Registers_data->Aff_data.Aff1 = _aff_local; //Получение ядра для Aff
    _aff_local = (volatile uint64_t)0;

    _Registers_data->PMR = (0xFF << 0); //Установление самого низкого порога приоритета для прерывания
    _Registers_data->GROUP1ENABLE = (1 << 0); //Включение группы 1(Non-Secure)
    _Registers_data->SRE = (1 << 2) | (1 << 0); //По порядку: выключение обхода IRQ, включение регистров GICv3 для взаимодействвия
    _Registers_data->SRE &= (0 << 1); //включение обхода FIQ
    _Registers_data->CTLR = (1 << 0); //Конфигурация GICv3
    _Registers_data->CTLR &= (0 << 2) | (0 << 14);

    __asm__ volatile("MSR ICC_CTLR_EL1, %0" : : "r"(_Registers_data->CTLR)); //Включение GIC моодуля для MRS/MSR
    __asm__ volatile("ISB");

    __asm__ volatile("MSR ICC_SRE_EL1, %0" : : "r"(_Registers_data->SRE)); //Включение GIC моодуля для MRS/MSR
    __asm__ volatile("ISB");

    __asm__ volatile("MSR ICC_IGRPEN1_EL1, %0" : : "r"(_Registers_data->GROUP1ENABLE)); //Включение группы 1 прерываний
    __asm__ volatile("ISB");

    __asm__ volatile("MSR ICC_PMR_EL1, %0" : : "r"(_Registers_data->PMR)); //Установление порога
    __asm__ volatile("ISB");


    //Вычисление адреса для определённого прерывания. Для разрешения данного прерывания. Формула: ПРЕРЫВАНИЕ / 32 * 4. Почему / на 32 -> см. в Custom-type.h
    GIC.GICD_ISENABLER[33 / 32] = (volatile uint32_t*)(0x08000000 + 0x0100 + (33 / 32) * 4); 
    //Вычисление адреса определённого прерывания. Для указания его группы.
    GIC.GICD_IGROUPR[33 / 32] = (volatile uint32_t*)(0x08000000 + 0x0080 + (33 / 32) * 4);
    //Вычисление адреса для определённого прерывания. Для указания приоритета для определённого прерывания. Формула: ПРЕРЫВАНИЯ / 4 * 4. 
    //Почему / происходит на 4 -> т.к. в IPRIORITYR в одном 32-битном регистре 4 ячейки по одному байту. * на 4 нужно для вычесление адреса до этой ячейки.
    //Т.к. идёт "перешагивание" ячеек до этой.
    GIC.GICD_IPRIORITY[33 / 4] = (volatile uint32_t*)(0x08000000 + 0x0400 + (33 / 4) * 4);
    //Вычисление адреса для определённого прерывания. Для указание, на какое ядро пойдёт данное прерывание при обработке. Формула: ПРЕРЫВАНИЕ * 8
    //Умножение на 8 происходит -> т.к. это 64-битный регистр каждое прерывание расположено друг от друга в одном байте
    GIC.GICD_IROUTER[33] = (volatile uint64_t*)(0x08000000 + 0x6000 + (33 * 8));
    GIC.GICD_ICFGR[33 / 16] = (volatile uint32_t*)(0x08000000 + 0x0C00 + (33 / 16) * 4);
    GIC.GICD_CTLR = (volatile uint32_t*)(0x08000000 + 0x0000 / 4);   

    *GIC.GICD_ISENABLER[33 / 32] |= (1 << (33 % 32)); //Взаимодейтсвие: 33 делится с остатком на 32 -> т.к. вычисляется конкретный бит с этим прерыванием в 32-битном регистре
    *GIC.GICD_IGROUPR[33 / 32] |= (1 << (33 % 32)); //Так-же как и в ISENABLER
    //Взаимодействие: 33 делится с остатком на 4 -> т.к. вычисляется конкретная ячейка в 32-битном регистре.
    //Умножение на 8 происходит, чтобы перевести данную ячейку в биты
    *GIC.GICD_IPRIORITY[33 / 4] |= (1 << (33 % 4) * 8); 
    *GIC.GICD_IROUTER[33] |= (_Registers_data->Aff_data.Aff0 << 0) | (_Registers_data->Aff_data.Aff1 << 8)
    | (_Registers_data->Aff_data.Aff2 << 16) | (_Registers_data->Aff_data.Aff3 << 32); //Указание на какое ядро пойдёт прерывание
    *GIC.GICD_IROUTER[33] &= (0 << 31);
    //0 - долгий сигнал / 1 - короткий
    //Умножение на 2 -> так как на каждое прерывание выделяется здесь 2 бита
    *GIC.GICD_ICFGR[33 / 16] &= (0 << ((33 % 16) * 2)); //Указание режима реагирования на прерывание
    //4 - обычный маршрут обработки
    *GIC.GICD_CTLR = (1 << 4) | (1 << 0);

    GIC.GICRS[0].GICR = (volatile uint32_t*)(0x080A0000);
    //GIC.GICRS[0].GICR_CTLR = (volatile uint32_t*)(GIC.GICRS[0].GICR + 0x0000);
    GIC.GICRS[0].GICR_WAKER = (volatile uint32_t*)(GIC.GICRS[0].GICR + (0x0014 / 4));

    *GIC.GICRS[0].GICR_WAKER &= (0 << 1);
    while(*GIC.GICRS[0].GICR_WAKER & (1 << 2)){
        *(volatile uint32_t*)0x09000000 = 'A';
        __asm__ volatile("NOP");
    }
    //*GIC.GICRS[0].GICR_CTLR |= (1 << 26) | (0 << 25) | (1 << 24);

    // volatile int _index_ENABLER = -1;
    // volatile int _index_PRIORITYR = -1;
    // for(int _ID = 32; _ID < 128; _ID++){
    //     if(_index_ENABLER != (_ID / 32)){
    //         _index_ENABLER = _ID / 32;
    //         GIC.GICD_ISENABLER[_index_ENABLER] = (volatile uint32_t*)(0x08000000 + 0x0100 + (_ID / 32) * 4);
    //         GIC.GICD_IGROUPR[_index_ENABLER] = (volatile uint32_t*)(0x08000000 + 0x0080 + (_ID / 32) * 4);
    //     } 
    //     if(_index_PRIORITYR != (_ID / 4)){
    //         _index_PRIORITYR = _ID / 4;
    //         GIC.GICD_IPRIORITY[_index_PRIORITYR] = (volatile uint32_t*)(0x08000000 + 0x0400 + (_ID / 4) * 4);
    //     }

    //     GIC.GICD_IROUTER[_ID] = (volatile uint64_t*)(0x08000000 + 0x6000 + (_ID * 8));

    //     *GIC.GICD_ISENABLER[_index_ENABLER] |= (1 << (_ID % 32));
    //     *GIC.GICD_IGROUPR[_index_ENABLER] |= (1 << (_ID % 32));
    //     *GIC.GICD_IPRIORITY[_index_PRIORITYR] |= (1 << (_ID % 4) * 8);
    //     *GIC.GICD_IROUTER[_ID] = 0x0;
    // }
    
}


void check_UART_avaible_common(volatile struct UART* _UART){
    //Проверка на то: лежит в этом адресе, что-то
    _UART->UART = (volatile uint32_t*)0x09000000; //Уазание обычного адреса UART
    _UART->UART_CR = (volatile uint32_t*)(_UART->UART + 0x030 / 4); //Указание адреса для регистра конфигурации UART
    _UART->UART_DR = (volatile uint32_t*)(_UART->UART + 0x00 / 4); //Указание адреса для регистра отправки/прочтения данных(Tx и Rx линии) 
    _UART->UART_FR = (volatile uint32_t*)(_UART->UART + 0x18 / 4); //Указание адреса для регистра статуса UART
    //Указание адресов для стабилизации скорости передачи UART
    _UART->UART_FBRD = (volatile uint32_t*)(_UART->UART + 0x024 / 4); //Указание адреса для регистра корректировки IBRD
    _UART->UART_IBRD = (volatile uint32_t*)(_UART->UART + 0x028 / 4); //Указание адреса для регистра скорости передачи UART
    _UART->UART_LCR_H = (volatile uint32_t*)(_UART->UART + 0x02C / 4);
    _UART->UART_MIS = (volatile uint32_t*)(_UART->UART + 0x040 / 4);
    _UART->UART_ICR = (volatile uint32_t*)(_UART->UART + 0x044 / 4);
    _UART->UART_IFLS = (volatile uint32_t*)(_UART->UART + 0x034 / 4);
    _UART->UART_IMSC = (volatile uint32_t*)(_UART->UART + 0x38 / 4);
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
    *_UART->UART_IMSC = (1 << 5) | (1 << 4);
    *_UART->UART_CR = (1 << 0) | (1 << 8) | (1 << 9);
}