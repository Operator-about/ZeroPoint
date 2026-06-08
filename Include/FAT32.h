#include<Custom-type.h>
#include<Kernel-modules.h>

FAT32_BPB FAT32_INFO;

FAT32_BPB init_FAT32_BPB();
void search_next_free_cluster(int* _current_cluster);
void search_init_free_cluster(int* _current_cluster);
void get_cluster_data(int _cluster_number, uint8_t _buffer[]);
void build_structure_cluster_for_file(char _name[], uint32_t _start_sector);
void read_sector(int _sector, uint8_t _buffer[]);
void write_sector(int _sector, uint8_t _buffer[]);