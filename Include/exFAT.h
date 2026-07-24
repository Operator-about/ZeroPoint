#include<Custom-type.h>
#include<Kernel-modules.h>
#include<Stringz.h>
#include<SD.h>
#include<Mathz.h>

extern FileBuffer File;
extern exFATAttrubute exFAT_attribute;

//Физический уровень
void init_exFAT();
uint32_t init_MBR();
void read_cluster(uint32_t _cluster);
uint32_t walk_FAT(uint32_t _cluster);
int get_count_file();
FileInfo get_file_info();
void clear_file_buffer();
void copy_current_folder();

//Виртуальный уровень
void open(char _name[]);
void open_file(char _file_name[]);
void create_file(uint8_t _file_name[]);
void create_folder(uint8_t _folder_name[]);
void change_root();
void parser(char _path[], uint8_t _name[]);
void read_mode(uint32_t _cluster, FileInfo _info);