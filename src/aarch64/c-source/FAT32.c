#include<FAT32.h>

FAT32_BPB* FAT32_INFO;

FAT32_BPB* init_FAT32_BPB(uint32_t _sector){
    FAT32_INFO = (FAT32_BPB*)DAT_buffer;
    
    char _info[100];
    itos((int)FAT32_INFO->BPB_BytsPerSector, _info);
    print(_info);
    print("\r\n");

    clear_buffer(_info);
    itos((int)FAT32_INFO->BPB_SectorsPerCluster, _info);
    print(_info);
    print("\r\n");
}

void search_init_free_cluster(int* _current_cluster){
    uint8_t _test_buffer[512];
    while(1){
        uint32_t _FAT_sector = FAT32_INFO->BPB_ReserverSectorCount + (*_current_cluster * 4 / FAT32_INFO->BPB_BytsPerSector);
        read_sector(_FAT_sector, _test_buffer);
        if(_test_buffer[*_current_cluster * 4 % FAT32_INFO->BPB_BytsPerSector] == 0x0){
            break;
        }
        else{
            *_current_cluster++;
        }
    }
}

void search_next_free_cluster(int* _current_cluster){
    uint8_t _test_buffer[512];
    int _next_cluster = *_current_cluster;
    _next_cluster++;
    while(1){
        uint32_t _FAT_sector = FAT32_INFO->BPB_ReserverSectorCount + (_next_cluster * 4 / FAT32_INFO->BPB_BytsPerSector);
        read_sector(_FAT_sector, _test_buffer);
        if(_test_buffer[_next_cluster * 4 % FAT32_INFO->BPB_BytsPerSector] == 0x0){
            break;
        }
        else{
            _next_cluster++;
        }
    }
    *_current_cluster = _next_cluster;
    _next_cluster = 0;
}

void get_all_cluster(int _cluster, uint8_t _buffer[4096]){
    uint8_t _out_buffer[512];
    int _buffer_data_index = 0;
    int _sector_number = (FAT32_INFO->BPB_ReserverSectorCount + (FAT32_INFO->BPB_FATsTableCount * FAT32_INFO->BPB_FATsTableSize32)) + ((_cluster - 2) * FAT32_INFO->BPB_SectorsPerCluster);
    for(int _index = 0; _index <= FAT32_INFO->BPB_SectorsPerCluster; _index++){
        read_sector(_sector_number, _out_buffer);
        //_buffer[_buffer_data_index] = _out_buffer;
        _buffer_data_index += FAT32_INFO->BPB_BytsPerSector;
    }
}

void read_sector(int _sector, uint8_t _buffer[]){

}