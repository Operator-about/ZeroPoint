#include<FAT32.h>

FAT32_BPB init_FAT32_BPB(){
    uint8_t _BPB_buffer[512];
    read_sector(0, _BPB_buffer);
    
    FAT32_BPB _info = {};
    _info.BPB_BytsPerSector |= (_BPB_buffer[88] << 0) | (_BPB_buffer[89] << 8);
    _info.BPB_SectorsPerCluster = _BPB_buffer[90];
    _info.BPB_ReserverSectorCount |= (_BPB_buffer[91] << 0) | (_BPB_buffer[92] << 8);
    _info.BPB_FATsTableCount = _BPB_buffer[93];
    _info.BPB_FATsTableSize32 |= (_BPB_buffer[152] << 0) | (_BPB_buffer[153] << 8) | (_BPB_buffer[154] << 16) | (_BPB_buffer[155] << 24);
    
    return _info;
}

void search_init_free_cluster(int* _current_cluster){
    uint8_t _test_buffer[512];
    while(1){
        uint32_t _FAT_sector = FAT32_INFO.BPB_ReserverSectorCount + (*_current_cluster * 4 / FAT32_INFO.BPB_BytsPerSector);
        read_sector(_FAT_sector, _test_buffer);
        if(_test_buffer[*_current_cluster * 4 % FAT32_INFO.BPB_BytsPerSector] == 0x0){
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
        uint32_t _FAT_sector = FAT32_INFO.BPB_ReserverSectorCount + (_next_cluster * 4 / FAT32_INFO.BPB_BytsPerSector);
        read_sector(_FAT_sector, _test_buffer);
        if(_test_buffer[_next_cluster * 4 % FAT32_INFO.BPB_BytsPerSector] == 0x0){
            break;
        }
        else{
            _next_cluster++;
        }
    }
    *_current_cluster = _next_cluster;
    _next_cluster = 0;
}

void read_sector(int _sector, uint8_t _buffer[]){

}