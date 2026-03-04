#pragma once
#define SIZE 4096

#include<stdint.h>
#include<string.h>
#include<stdbool.h>

struct BRR_UART{
    uint32_t IBRD;
    uint32_t FBRD; 
};

struct Aff_registers_data{
    volatile uint64_t Aff0;
    volatile uint64_t Aff1;
    volatile uint64_t Aff2;
    volatile uint64_t Aff3;
};

struct GIC_registers_data{
    volatile uint64_t PMR; //Порог для прерывания
    volatile uint64_t SRE; //Доступность GICv3 в регистрах
    volatile uint64_t GROUP1ENABLE; //Разрешение первой группы прерываний(Non-Secure)
    volatile uint64_t CTLR;
    volatile struct Aff_registers_data Aff_data;
};

struct GICR{
    volatile uint32_t* GICR;
    volatile uint32_t* GICR_WAKER;
    volatile uint32_t* GICR_CTLR;
};

struct GIC{
    //4 элемента -> так как это кол-во 32-битных регистров, где 32 прерывания. Т.е. в каждом 32 битном по 32 прерывания
    volatile uint32_t* GICD_ISENABLER[4]; //Выставление доступных прерываний
    //128 элемента -> так как это для всех прерываний, а общее кол-во прерываний на данный момент 128
    volatile uint64_t* GICD_IROUTER[128]; //Указание ядров для прерываний
    //32 элемента -> так как в одном 32 битном регистре 4 ячейке по 1 байту(т.е. по 8 бит каждая). Это вычесляется так: 
    volatile uint32_t* GICD_IPRIORITY[32]; //Приоритет конкретному прерыванию
    //4 элемента -> так-же как и в ISENABLER
    volatile uint32_t* GICD_IGROUPR[4]; //Указание каждому прерыванию, в какой групе он находится
    //64 -> т.к. в данном случаи в один 32-битный регистр помещается по 16 ячеек с настройками для регистров(2 бита на каждую ячейку). 
    //Т.е. помещается 16 настроек, для 16 прерываний
    volatile uint32_t* GICD_ICFGR[64]; //Указание, как GIC будет реагировать на прерывания. Либо по level(долгому сигналу) или по edge(короткому одному сигналу)
    volatile struct GICR GICRS[1];
    volatile uint32_t* GICD_CTLR;
};



struct UART{
    volatile uint32_t* UART; //Регистер UART
    volatile uint32_t* UART_LCR_H; //Регистр настройки передачи
    volatile uint32_t* UART_DR; //Регистр Rx, Tx линий 
    volatile uint32_t* UART_CR; //Регистр конфигурации
    volatile uint32_t* UART_FR; //Регистр состояния UART
    volatile uint32_t* UART_IBRD; //Коэффициент замедления скорости(int)
    volatile uint32_t* UART_FBRD; //Коэффициент замедление скорости(float)
    volatile uint32_t* UART_MIS; //Хранит, какое имено прерывание из 33 ID(Tx или Rx)
    volatile uint32_t* UART_ICR; //Обнуление прерывания
    volatile uint32_t* UART_IFLS; //Параметр срабатывания FIFO
    volatile uint32_t* UART_IMSC; //Разрешает определённые прерывания
    int interrput;
};

struct UART_buffer{
    uint8_t buffer[SIZE]; //Буфер
    int head; //Размер буфера
    int tail; //Текущая позиция
};