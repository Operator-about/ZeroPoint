#include<exFAT.h>

FileBuffer File;
exFATAttrubute exFAT_attribute;

uint32_t init_MBR(){
    int _partion_index = 0;
    MBR _MBR;
    uint32_t _LBA = 0x0;
    exFAT* exFATp;

    SD.SDF->single_read(0);
    _MBR = *(MBR*)&File.Buffer;

    while(_partion_index < 4){
        _LBA = *(uint32_t*)&_MBR.PartionsRecords[_partion_index].StartLBA;

        SD.SDF->single_read(_LBA);
        exFATp = (exFAT*)&File.Buffer;
        if(compare_s(exFATp->FileSystemName, "EXFAT   ") == 1){
            break;
        }
        _LBA = 0x0;
        _partion_index++;
    }

    return _LBA;
}

void init_exFAT(){
    uint32_t _LBA = 0x0;
    _LBA = init_MBR();

    SD.SDF->single_read(_LBA);
    exFAT* _exFATp = (exFAT*)File.Buffer;

    exFAT_attribute.DATALBA = _LBA + _exFATp->ClusterHeapOffset;
    exFAT_attribute.FATLBA = _LBA + _exFATp->FATOffset;
    exFAT_attribute.BytsPerSector = (uint32_t)power_two((int)_exFATp->BytsPerSector);
    exFAT_attribute.SectorsPerCluster = (uint32_t)power_two((int)_exFATp->SectorsPerCluster);
    exFAT_attribute.FirstRootCluster = _exFATp->FirstRootCluster;
    clear_buffer(CurrentFolder.Name);

    CurrentFolder.FirstCluster = exFAT_attribute.FirstRootCluster;
    CurrentFolder.FileAttribute = (1ULL << 4);
}

void read_cluster(uint32_t _cluster){
    volatile uint32_t _LBA_for_cluster = exFAT_attribute.DATALBA + ((_cluster - 2) * exFAT_attribute.SectorsPerCluster);

    SD.SDF->multi_read(_LBA_for_cluster);
}

uint32_t walk_FAT(uint32_t _cluster){
    uint32_t _LBA_for_cluster_FAT = exFAT_attribute.FATLBA + ((_cluster * 4) / exFAT_attribute.BytsPerSector);
    SD.SDF->single_read(_LBA_for_cluster_FAT);

    uint32_t _tempory_cluster = 0x0;
    _tempory_cluster = *(uint32_t*)&File.Buffer[((_cluster * 4) % exFAT_attribute.BytsPerSector)];

    if(_tempory_cluster >= 0xFFFFFFF8 && _tempory_cluster <= 0xFFFFFFFF){
        return 0xFFFFFFFF;
    }
    else if(_tempory_cluster == 0x0){
        return 0x0;
    }
    else{
        return _tempory_cluster;
    }
}

uint32_t walk_allocationbitmap(){
    uint8_t _tempory_buffer[32];
    uint32_t _first_cluster = 0x0;
    read_cluster(exFAT_attribute.FirstRootCluster);
    for(int _build = 0; _build < 32; _build++){
        _tempory_buffer[_build] = File.Buffer[File.Current_index];
        File.Current_index++;
        if(((_build + 1) % 32) == 0){
            _build = -1;
            if(_tempory_buffer[0] == 0x81){
                _first_cluster = ((BitMapAllocationDescriptor*)_tempory_buffer)->FirstCluster;
                break;
            }
        }
    }

    uint32_t _current_cluster = exFAT_attribute.FirstRootCluster;
    uint8_t _test_byte = 0x0;
    uint8_t _mask = 0x0;
    read_cluster(_first_cluster);
    while(1){
        _test_byte = File.Buffer[(((int)_current_cluster - 2) / 8)];
        _mask = (1ULL << ((_current_cluster - 2) % 8));
        if(_test_byte & _mask){
            _current_cluster++;
        }
        else{
            //clear_file_buffer();
            print("Cluster find\r\n");
            return _current_cluster;
        }
    }
}


FileInfo get_file_info(){
    uint8_t _tempory_buffer[32];
    FileInfo _file_info;
    _file_info.SecondaryCount = 0;
    int _index_secondary = -1;
    int _tempory_index = 0;
    _file_info.NameIndex = 0;

    for(int _clear = 0; _clear < 260; _clear++){
        _file_info.Name[_clear] = 0x0;
    }

    for(int _index = 0; _index < File.Buffer_index; _index++){
        _tempory_buffer[_tempory_index] = File.Buffer[File.Current_index];
        File.Current_index++;
        _tempory_index++;
        if(_tempory_index == 32){
            _tempory_index = 0;
            switch(_tempory_buffer[0]){
                case 0x85:
                    _index_secondary = 0;
                    _file_info.SecondaryCount = ((FileEntryDescriptor*)_tempory_buffer)->SecondaryCount;
                    _file_info.FileAttribute = ((FileEntryDescriptor*)_tempory_buffer)->FileAttributes;
                    break;
                case 0xC0:
                    _file_info.FirstCluster = ((StreamExtensionsDescriptor*)_tempory_buffer)->FirstCluster;
                    _file_info.NoFATChain = ((StreamExtensionsDescriptor*)_tempory_buffer)->GenralSecondaryFlags;
                    _file_info.FileLength = ((StreamExtensionsDescriptor*)_tempory_buffer)->DataLength;
                    _index_secondary++;
                    break;
                case 0xC1:
                    for(int _get_name = 0; _get_name < 30; _get_name++){
                        _file_info.Name[_file_info.NameIndex + _get_name] = _tempory_buffer[2+_get_name];
                    }
                    _file_info.NameIndex+=30;
                    _index_secondary++;
                    break;
                default:
                    break;
            }
            
            if(_index_secondary >= _file_info.SecondaryCount){
                break;
            }
        }
    }

    return _file_info;
}

uint16_t name_compare_hash(uint8_t _name[]){
    uint16_t _summ = _name[0];
    int _index = 1;

    while(_name[_index] != 0x00){
        if((_summ % 2) == 0){
            _summ = ((_summ / 2) + _name[_index]);
        }
        else{
            _summ = (((_summ / 2) + 32768) + _name[_index]);
        }
        _index++;
    }

    return _summ;
}

uint16_t summ(uint8_t _buffer[]){
    uint16_t _summ = _buffer[0];

    for(int _index = 0; _index < 96; _index++){
        if(_index == 2 || _index == 3){
            continue;
        }
        else{
            if((_summ % 2) == 0){
                _summ = ((_summ / 2) + _buffer[_index]);
            }
            else{
                _summ = (((_summ / 2) + 32768) + _buffer[_index]);
            }
        }
    }

    return _summ;
}

uint16_t name_hash(uint8_t _name[]){
    uint16_t _summ = _name[0];
    
    for(int _index = 0; _index < 30; _index++){
        if((_summ % 2) == 0){
            _summ = ((_summ / 2) + _name[_index]);
        }
        else{
            _summ = (((_summ / 2) + 32768) + _name[_index]);
        }
    }
}

void create_descriptors_file(){
    uint8_t _buffer[96];
    uint8_t _name[96];
    for(int _clear = 0; _clear < 96; _clear++){
        _buffer[_clear] = 0x0;
        _name[_clear] = 0x0;
    }

    _buffer[0] = 0x85;
    _buffer[1] = 2;

    _buffer[33] = 0xC0;
    _buffer[35] = 5;
    *(uint16_t*)&_buffer[36] = name_hash(_name);

    _buffer[65] = 0xC1;
    for(int _copy = 0; _copy < 30; _copy++){
        _buffer[67 + _copy] = _name[_copy];
    }
}

int get_count_file(){
    int _count = 0;

    for(int _index = 0; _index < File.Buffer_index; _index++){
        if(File.Buffer[_index] == 0x85){
            _count++;
        }
    }

    return _count;
}