#include<MMU.h>

alignas(4096) uint64_t L0_table[1][512];
alignas(4096) uint64_t L1_table[1][512];
alignas(4096) uint64_t L2_table[512][512];
uint64_t L1_index_address;


void MMU_init(){
    clear_TTBR();
    
    L1_index_address = (uint64_t)0x00000000; //Указание нулевого адреса для правильного расчёта
    uint64_t _buffer[] = {OutJump->UART, OutJump->SD, OutJump->GICv2};
    uint64_t _device_block_address = 0x0;

    switch(min_uint64_t(_buffer)){
        case 0x00000000 ... 0x3FFFFFFF:
            _device_block_address = 0x00000000;
            break;
        case 0x40000000 ... 0x7FFFFFFF:
            _device_block_address = 0x40000000;
            break;
        case 0x80000000 ... 0xBFFFFFFF:
            _device_block_address = 0x80000000;
            break;    
        case 0xC0000000 ... 0xFFFFFFFF:
            _device_block_address = 0xC0000000;
            break;
        default:
            _device_block_address = 0x0;
            break;
    }    

    MMU_registers _registers = {0};
    _registers.TCR = (16ULL << 0); //Указание T0SZ 
    int _max_descriptor_space = 0;
    switch(get_MMU_support_IPS()){
        case 32:
            _registers.TCR &= ~(3ULL << 32);
            _max_descriptor_space = 4;
            break;
        case 36:
            _registers.TCR &= ~(3ULL << 32);
            _registers.TCR |= (1ULL << 32);
            _max_descriptor_space = 64;
            break;
        case 40:
            _registers.TCR &= ~(3ULL << 32);
            _registers.TCR |= (2ULL << 32);
            _max_descriptor_space = 1024;
        default:
            return;
            break;
    }

    L_block_descriptor_LOW_init(0, 0, 0, L0_table, L1_table);
    for(int _L1 = 0; _L1 < _max_descriptor_space; _L1++){
        L_block_descriptor_LOW_init(0, _L1, _L1, L1_table, L2_table);
        if(L1_index_address >= _device_block_address && L1_index_address <= (_device_block_address + 0x40000000)){
            L_block_descriptor_DEVICE_init(_L1, L2_table, 0x00200000);
        }
        else{
            L_block_descriptor_NORMAL_init(_L1, L2_table, 0x00200000);
        }
    }

    _registers.MAIR = (4ULL << 0) | (68ULL << 8); //Указание nGnRE(0 индекс 0:7) и указание Normal N I/O(1 индекс 8:15)
    _registers.TTBR0 = ((uint64_t)L0_table << 0); //Указание того, что поиск в MMU будет начинаться с L0 таблицы

    switch(get_MMU_support_TG()){
        case 4:
            _registers.TCR &= ~(3ULL << 14);
            break;
        default:
            return;
            break;
    }

    _registers.SCTLR = (1ULL << 0) | (0ULL << 12) | (0ULL << 2); //Включение MMU

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

void L_block_descriptor_LOW_init(int _table_index, int _des_index, int _to_index, uint64_t _table[][512], uint64_t _to[][512]){
    _table[_table_index][_des_index] = (uint64_t)_to[_to_index];
    _table[_table_index][_des_index] |= (1ULL << 0) | (1ULL << 1);
}

void L_block_descriptor_DEVICE_init(int _table_index, uint64_t _table[512][512], uint64_t _plus){
    for(int _descriptor_index = 0; _descriptor_index <= 511; _descriptor_index++){
        _table[_table_index][_descriptor_index] = L1_index_address;
        _table[_table_index][_descriptor_index] |= (1ULL << 0);
        _table[_table_index][_descriptor_index] &= ~(1ULL << 1);
        _table[_table_index][_descriptor_index] &= ~(3ULL << 2);
        _table[_table_index][_descriptor_index] &= ~(3ULL << 6);
        _table[_table_index][_descriptor_index] &= ~(3ULL << 8);
        _table[_table_index][_descriptor_index] |= (1ULL << 10);
        _table[_table_index][_descriptor_index] |= (1ULL << 53);
        _table[_table_index][_descriptor_index] |= (1ULL << 54);
        
        L1_index_address+=_plus;
    }
}

void L_block_descriptor_NORMAL_init(int _table_index, uint64_t _table[512][512], uint64_t _plus){
    for(int _descriptor_index = 0; _descriptor_index < 512; _descriptor_index++){
        _table[_table_index][_descriptor_index] = L1_index_address; 
        _table[_table_index][_descriptor_index] |= (1ULL << 0); 
        _table[_table_index][_descriptor_index] &= ~(1ULL << 1); 
        _table[_table_index][_descriptor_index] &= ~(3ULL << 2); 
        _table[_table_index][_descriptor_index] |= (1ULL << 2); 
        _table[_table_index][_descriptor_index] &= ~(3ULL << 6); 
        _table[_table_index][_descriptor_index] &= ~(3ULL << 8);
        _table[_table_index][_descriptor_index] |= (1ULL << 10);
        _table[_table_index][_descriptor_index] &= ~(1ULL << 53);
        switch(L1_index_address){
            case 0x40000000 ... 0xC0000000:
                _table[_table_index][_descriptor_index] &= ~(1ULL << 54);
                _table[_table_index][_descriptor_index] |= (1ULL << 6);
                break;
            default:
                _table[_table_index][_descriptor_index] |= (1ULL << 54);
                break;
        }

        L1_index_address += _plus; //Прибавление для указания нового адреса.
    }
}

int get_MMU_support_IPS(){
    uint64_t _ips;
    __asm__("MRS %0, ID_AA64MMFR0_EL1" : "=r"(_ips));

    switch(_ips & 0xF){
        case 1:
            return 36;
        case 2:
            return 40;
        default:
            return 32;
    }
}

int get_MMU_support_TG(){
    uint64_t _tg;
    __asm__("MRS %0, ID_AA64MMFR0_EL1" : "=r"(_tg));

    switch((_tg >> 28) & 0xF){
        case 15:
            return 16;
        default:
            return 4;
    }
}