#include<Custom-type.h>
#include<Kernel-modules.h>

alignas(4096) extern uint64_t L0_table[512];
alignas(4096) extern uint64_t L1_table[512];
alignas(4096) extern uint64_t L2_table[512];
extern uint64_t L1_index_address;
extern uint64_t L2_index_address;

void MMU_init();
void L0_table_descriptor_init();
void L1_block_descriptor_NORMAL_init(int _index);
void L1_block_descriptor_DEVICE_init(int _index);
void L1_table_descriptor_init(int _index);
void L2_block_descriptor_DEVICE_init(int _index);