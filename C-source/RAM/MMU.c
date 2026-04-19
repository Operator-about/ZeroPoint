#include<MMU.h>

alignas(4096) uint64_t L0_table[512];
alignas(4096) uint64_t L1_table[512];
uint64_t L1_index_address;

void MMU_init(){
    struct MMU_registers _registers;
    uintptr_t _pointer_L0 = (uintptr_t)&L0_table;
    L1_index_address = (uint64_t)0x00000000;
    _registers.MAIR |= (4ULL << 0) | (68ULL << 8);
    _registers.TTBR0 |= ((uint64_t)_pointer_L0 << 0);
    _registers.TCR |= (25ULL << 0) | (5ULL << 32);
    _registers.TCR &= ~(1ULL << 14);

    __asm__("MSR MAIR_EL1, %0" : : "r"(_registers.MAIR));
    __asm__("MSR TTBR0_EL1, %0" : : "r"(_registers.TTBR0));
    __asm__("MSR TCR_EL1, %0" : : "r"(_registers.TCR));
    __asm__("ISB");

    L0_table_descriptor_init();
    L1_block_descriptor_DEVICE_init(0);
    L1_block_descriptor_NORMAL_init(1);

    MMU_active();
}

void L1_block_descriptor_DEVICE_init(int _index){
    L1_table[_index] |= (1ULL << 0);
    L1_table[_index] &= (1ULL << 1);
    L1_table[_index] &= ~(1ULL << 2);
    L1_table[_index] &= (1ULL << 6);
    L1_table[_index] &= (1ULL << 7);
    L1_table[_index] &= (1ULL << 8);
    L1_table[_index] |= (1ULL << 10);
    L1_table[_index] |= (L1_index_address << 12);

    L1_index_address += 0x40000000;
}

void L1_block_descriptor_NORMAL_init(int _index){
    L1_table[_index] |= (1ULL << 0);
    L1_table[_index] &= (1ULL << 1);
    L1_table[_index] |= (1ULL << 2);
    L1_table[_index] &= (1ULL << 6);
    L1_table[_index] &= (1ULL << 7);
    L1_table[_index] &= (1ULL << 8);
    L1_table[_index] |= (1ULL << 10);
    L1_table[_index] |= (L1_index_address << 12);

    L1_index_address += 0x40000000;
}

void L0_table_descriptor_init(){
    uintptr_t _pointer_L1 = (uintptr_t)&L1_table;
    L0_table[0] |= ((uint64_t)_pointer_L1 << 12);
    L0_table[0] |= (1ULL << 0) | (1ULL << 1);
    __asm__("MOV X26, %0" : : "r"(&L0_table));
    __asm__("MOV X23, %0" : : "r"(L0_table[0]));
    __asm__("MOV X25, %0" : : "r"(&L1_table));
}
