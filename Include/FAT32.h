#include<Custom-type.h>
#include<Kernel-modules.h>
#include<Stringz.h>
#include<std.h>

extern FAT32_BPB* FAT32_INFO;

FAT32_BPB* init_FAT32_BPB(uint32_t _sector);
void get_file();
void search_next_free_cluster(int* _current_cluster);
void get_all_cluster(int _cluster, uint8_t _buffer[4096]);
void search_init_free_cluster(int* _current_cluster);
void get_cluster_data(int _cluster_number, uint8_t _buffer[]);
void build_structure_cluster_for_file(char _name[], uint32_t _start_sector);
void read_sector(int _sector, uint8_t _buffer[]);
void get_file_from_folder(int _folder_cluster);
FAT32_FILE create_new_file(char _name, uint8_t _attr);
