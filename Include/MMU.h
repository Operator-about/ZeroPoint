#include<Custom-type.h>
#include<Kernel-modules.h>

alignas(4096) extern uint64_t L0_table[1][512];
alignas(4096) extern uint64_t L1_table[1][512];
alignas(4096) extern uint64_t L2_table[512][512];
extern uint64_t L1_index_address;

void MMU_init();

void L_block_descriptor_DEVICE_init(int _table_index, uint64_t _table[512][512], uint64_t _plus);
void L_block_descriptor_NORMAL_init(int _table_index, uint64_t _table[512][512], uint64_t _plus);
void L_block_descriptor_LOW_init(int _table_index, int _des_index, int _to_index, uint64_t _table[][512], uint64_t _to[][512]);

int get_MMU_support_IPS();
int get_MMU_support_TG();
extern void clear_TTBR();