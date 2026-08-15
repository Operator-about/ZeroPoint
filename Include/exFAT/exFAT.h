#include<Custom-type.h>
#include<Kernel-modules.h>
#include<Stringz.h>
#include<SD.h>
#include<Mathz.h>

extern FileBuffer File;
extern exFATAttrubute exFAT_attribute;
extern FileInfo CurrentFolder;

//Физический уровень
void init_exFAT();
uint32_t init_MBR();
void read_cluster(uint32_t _cluster);
uint32_t walk_FAT(uint32_t _cluster);
int get_count_file();
void create_descriptors_file();
FileInfo get_file_info();
uint32_t walk_allocationbitmap();
uint16_t name_compare_hash(uint8_t _name[]);
uint16_t name_hash(uint8_t _name[]);
uint16_t summ(uint8_t _buffer[]);

//Виртуальный уровень
void open(char _name[]);
void open_folder();
void open_file(FileInfo _info);
void display_folder();
void current_folder_name_clear();
void parser(char _path[], uint8_t _name[]);
void read_mode(uint32_t _cluster, FileInfo _info);