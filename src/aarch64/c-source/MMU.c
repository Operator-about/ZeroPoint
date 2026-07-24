#include<MMU.h>

alignas(4096) uint64_t L0_table[512];
alignas(4096) uint64_t L1_table[512];
alignas(4096) uint64_t L2_table[512][512];
alignas(4096) uint64_t L3_table[512][512];
uint64_t L1_index_address;
uint64_t L2_index_address;


void MMU_init(){
    L1_index_address = (uint64_t)0x00000000; //Указание нулевого адреса для правильного расчёта
    int _current_L1 = 0;
    L0_table_descriptor_init();
    for(int _L1 = 0; _L1 < 3; _L1++){
        L1_table_descriptor_init(_L1);
        for(int _L2 = 0; _L2 <= 511; _L2++){
            L2_block_descriptor_NORMAL_init(_L2, _L1);
        }
        _current_L1++;
    }

    for(int _L2 = 0; _L2 <= 511; _L2++){
        L2_table_descriptor_init(_L2, _current_L1);
        L3_block_descriptor_DEVICE_init(_L2);
    }
    L1_table_descriptor_init(3);
    MMU_registers _registers = {0};

    _registers.MAIR |= (4ULL << 0) | (68ULL << 8); //Указание nGnRE(0 индекс 0:7) и указание Normal N I/O(1 индекс 8:15)
    _registers.TTBR0 |= ((uint64_t)L0_table << 0); //Указание того, что поиск в MMU будет начинаться с L0 таблицы
    _registers.TCR |= (16ULL << 0); //Указание T0SZ 
    _registers.TCR &= ~(3ULL << 32); //IPS
    /*
        T0SZ - указывает на то, с какой таблицы искать для VA - OA адрес.
        В данном случаи 16 записано, т.к. для 48-бит при 4КБ грануле 64-14 = 47 - с данного бита начинается осмотр MMU с L0 таблице
        IPS - указывает битность дескриптора для таблиц L 
    */
    _registers.TCR &= ~(3ULL << 14); //Указание, что используется гранула в 4КБ для страниц
    _registers.TCR &= ~(3ULL << 12);
    _registers.TCR &= ~(3ULL << 10);
    _registers.TCR &= ~(3ULL << 8);

    _registers.SCTLR |= (1ULL << 0); //Включение MMU
    _registers.SCTLR &= ~(1ULL << 2); //Выключение кэширования для данных
    _registers.SCTLR &= ~(1ULL << 12); //Выключение кэширования для инструкций

    __asm__("MSR MAIR_EL1, %0" : : "r"(_registers.MAIR));
    __asm__("MSR TTBR0_EL1, %0" : : "r"(_registers.TTBR0));
    __asm__("MSR TCR_EL1, %0" : : "r"(_registers.TCR));
    __asm__("ISB");

    __asm__("DSB SY");
    __asm__("ISB");

    __asm__("MSR SCTLR_EL1, %0" : :"r"(_registers.SCTLR));
    __asm__("ISB");
    VBAR_set();
}

void L1_block_descriptor_DEVICE_init(int _index){
    L1_table[_index] = L1_index_address;
    L1_table[_index] |= (1ULL << 0);
    L1_table[_index] &= ~(1ULL << 1);
    L1_table[_index] &= ~(1ULL << 2);
    L1_table[_index] &= ~(1ULL << 7);
    L1_table[_index] &= ~(3ULL << 8);
    L1_table[_index] |= (1ULL << 10);

    L1_index_address += 0x40000000;
}

void L1_block_descriptor_NORMAL_init(int _index){
    L1_table[_index] = L1_index_address;
    L1_table[_index] |= (1ULL << 0);
    L1_table[_index] &= ~(1ULL << 1);
    L1_table[_index] |= (1ULL << 2);
    L1_table[_index] &= ~(1ULL << 7);
    L1_table[_index] &= ~(3ULL << 8);
    L1_table[_index] |= (1ULL << 10);

    L1_index_address += 0x40000000;
}

void L0_table_descriptor_init(){
    L0_table[0] = (uint64_t)L1_table; //Указние адреса L1 таблицы
    L0_table[0] |= (1ULL << 0) | (1ULL << 1); //Указание: валидности дескриптора. Указание типа. В данном случаи это дескриптор указание на следующею таблицу
}

void L1_table_descriptor_init(int _index){
    L1_table[_index] = (uint64_t)L2_table[_index]; //Указание адреса L2 таблицы
    L1_table[_index] |= (1ULL << 0) | (1ULL << 1); //Указание: валидности дескриптора. Указание типа. В данном случаи это дескриптор указание на следующею таблицу
}

void L2_table_descriptor_init(int _index, int _table_number){
    L2_table[_table_number][_index] = (uint64_t)L3_table[_index];
    L2_table[_table_number][_index] |= (1ULL << 0) | (1ULL << 1);
}


void L2_block_descriptor_DEVICE_init(int _index, int _table_number){
    L2_table[_table_number][_index] = L1_index_address; //Указание OA адреса
    L2_table[_table_number][_index] |= (1ULL << 0); //Указание валидности дескриптора
    L2_table[_table_number][_index] &= ~(1ULL << 1); //Указание типа дескриптора. В данном случаи block тип(т.е. указывает на OA)
    L2_table[_table_number][_index] &= ~(3ULL << 2); //Указание индекса в MAIR(т.е. указывается тип памяти: Device | Normal). В данном случаи: индекс 0, см. в MMU_init()
    L2_table[_table_number][_index] &= ~(1ULL << 7); //Указание флага доступа. В данном случаи RW Priv
    L2_table[_table_number][_index] &= ~(3ULL << 8); //Указание того, что между ядрами данный дескриптор не делится на чтение
    L2_table[_table_number][_index] |= (1ULL << 10); //Указание доступности дескриптора

    L1_index_address += 0x00200000; //Прибавление для указания нового адреса.
}

void L2_block_descriptor_NORMAL_init(int _index, int _table_number){
    L2_table[_table_number][_index] = L1_index_address; //Указание OA адреса
    L2_table[_table_number][_index] |= (1ULL << 0); //Указание валидности дескриптора
    L2_table[_table_number][_index] &= ~(1ULL << 1); //Указание типа дескриптора. В данном случаи block тип(т.е. указывает на OA)
    L2_table[_table_number][_index] &= ~(3ULL << 2); //Указание индекса в MAIR(т.е. указывается тип памяти: Device | Normal). В данном случаи: индекс 0, см. в MMU_init()
    L2_table[_table_number][_index] |= (1ULL << 2); //Указание индекса в MAIR(т.е. указывается тип памяти: Device | Normal). В данном случаи: индекс 1, см. в MMU_init()
    L2_table[_table_number][_index] &= ~(1ULL << 7); //Указание флага доступа. В данном случаи RW Priv
    L2_table[_table_number][_index] &= ~(3ULL << 8); //Указание того, что между ядрами данный дескриптор не делится на чтение
    L2_table[_table_number][_index] |= (1ULL << 10); //Указание доступности дескриптора

    L1_index_address += 0x00200000; //Прибавление для указания нового адреса.
}

void L3_block_descriptor_DEVICE_init(int _table_index){
    for(int _descriptor_index = 0; _descriptor_index <= 511; _descriptor_index++){
        L3_table[_table_index][_descriptor_index] = L1_index_address;
        L3_table[_table_index][_descriptor_index] |= (1ULL << 0);
        L3_table[_table_index][_descriptor_index] |= (1ULL << 1);
        L3_table[_table_index][_descriptor_index] &= ~(3ULL << 2);
        L3_table[_table_index][_descriptor_index] &= ~(3ULL << 6);
        L3_table[_table_index][_descriptor_index] &= ~(3ULL << 8);
        L3_table[_table_index][_descriptor_index] |= (1ULL << 10);
        
        L1_index_address+=0x00001000;
    }
}