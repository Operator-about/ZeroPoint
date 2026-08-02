#include<exFAT.h>

FileBuffer File;
exFATAttrubute exFAT_attribute;

uint32_t init_MBR(){
    int _partion_index = 0;
    MBR* _MBR;
    uint32_t _LBA = 0x0;
    exFAT* exFATp;

    print("[^]MBR init\r\n");
    while(_partion_index < 4){
        read_single_sector(0);
        _MBR = (MBR*)DAT_buffer;
        for(int _LBA_build = 0; _LBA_build < 4; _LBA_build++){
            _LBA |= ((uint32_t)_MBR->PartionsRecords[_partion_index].StartLBA[_LBA_build] << (8 * _LBA_build));
        }

        read_single_sector(_LBA);
        exFATp = (exFAT*)DAT_buffer;
        if(compare_s(exFATp->FileSystemName, "EXFAT   ") == 1){
            print("[^]exFAT found\r\n");
            break;
        }

        _partion_index++;
        _LBA = 0;
    }

    return _LBA;
}

void init_exFAT(){
    SD_Registers->SR_SD |= (1ULL << 2);
    sec_barrier(50);

    uint32_t _LBA = 0x0;
    _LBA = init_MBR();

    read_single_sector(_LBA);
    exFAT* _exFATp = (exFAT*)DAT_buffer;

    exFAT_attribute.DATALBA = _LBA + _exFATp->ClusterHeapOffset;
    exFAT_attribute.FATLBA = _LBA + _exFATp->FATOffset;
    exFAT_attribute.BytsPerSector = (uint32_t)power_two((int)_exFATp->BytsPerSector);
    exFAT_attribute.SectorsPerCluster = (uint32_t)power_two((int)_exFATp->SectorsPerCluster);
    exFAT_attribute.FirstRootCluster = _exFATp->FirstRootCluster;
    exFAT_attribute.CurrentRoot = 0;

    File.Current_index = 0;
    File.Buffer_index = 0;

    clear_buffer(CurrentFolder.Name);

    CurrentFolder.FirstCluster = exFAT_attribute.FirstRootCluster;
    CurrentFolder.FileAttribute = (1ULL << 4);

    SD_Registers->BC_SD = exFAT_attribute.SectorsPerCluster & 0x0000FFFF;
}

void read_cluster(uint32_t _cluster){
    volatile uint32_t _LBA_for_cluster = exFAT_attribute.DATALBA + ((_cluster - 2) * exFAT_attribute.SectorsPerCluster);

    sec_barrier(10);
    read_multi_sector(_LBA_for_cluster);
    sec_barrier(10);

    for(int _data = 0; _data < (exFAT_attribute.BytsPerSector * exFAT_attribute.SectorsPerCluster); _data++){
        File.Buffer[File.Buffer_index] = DAT_buffer[_data];
        File.Buffer_index++;
    }
}

uint32_t walk_FAT(uint32_t _cluster){
    if(exFAT_attribute.CurrentRoot == exFAT_attribute.FirstRootCluster){
        exFAT_attribute.CurrentRoot = 0;
    }
    uint32_t _LBA_for_cluster_FAT = exFAT_attribute.FATLBA + (((exFAT_attribute.CurrentRoot + _cluster) * 4) / exFAT_attribute.BytsPerSector);
    read_single_sector(_LBA_for_cluster_FAT);

    uint32_t _tempory_cluster = 0x0;
    for(int _build = 0; _build < 4; _build++){
        _tempory_cluster |= ((uint32_t)DAT_buffer[((((exFAT_attribute.CurrentRoot + _cluster) * 4) % exFAT_attribute.BytsPerSector) + _build)] << (8 * _build));
    }

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
                clear_file_buffer();
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
            clear_file_buffer();
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

int get_count_file(){
    int _count = 0;

    for(int _index = 0; _index < File.Buffer_index; _index++){
        if(File.Buffer[_index] == 0x85){
            _count++;
        }
    }

    return _count;
}

void clear_file_buffer(){
    for(int _clear = 0; _clear <= File.Buffer_index; _clear++){
        File.Buffer[_clear] = 0x0;
    }
    File.Current_index = 0;
    File.Buffer_index = 0;
}