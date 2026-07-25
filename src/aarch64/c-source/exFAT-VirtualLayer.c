#include<exFAT.h>

int _current_index = 0;
FileInfo CurrentFolder;

void open(char _name[]){
    uint8_t _current_name[260];
    for(int _clear = 0; _clear < 260; _clear++){
        _current_name[_clear] = 0x0;
    }
    FileInfo _info;
    uint8_t _buffer[260];
    int _this_folder = 1;
    for(int _clear = 0; _clear < 260; _clear++){
        _buffer[_clear] = 0x0;
    }

    clear_buffer_uint8(_current_name);
    parser(_name, _current_name);
    if(compare_s(_current_name, "mnt") == 1){
        clear_buffer_uint8(_current_name);
        read_cluster(exFAT_attribute.FirstRootCluster);
        exFAT_attribute.CurrentRoot = 0;
        CurrentFolder.FirstCluster = 0x0;
    }
    else{
        clear_buffer_uint8(_current_name);
        clear_file_buffer();
        clear_buffer(_name);
        _current_index = 0;
        exFAT_attribute.CurrentRoot = 0;
        return;
    }

    parser(_name, _current_name);

    if(_current_name[0] != 0x0){
        for(int _file_index = 0; _file_index <= get_count_file(); _file_index++){
            _info = get_file_info();
            if(compare_u16_to_ASCII(_info.Name, _current_name) == 1){
                clear_buffer_uint8(_current_name);
                if(_info.FileAttribute & (1ULL << 4)){
                    parser(_name, _current_name);
                    clear_file_buffer();
                    read_mode(_info.FirstCluster, _info);
                    if(_current_name[0] != 0x0){
                        _file_index = 0;
                    }
                    else{
                        break;
                    }     
                }
                else{
                    clear_file_buffer();
                    _current_index = 0;

                    read_mode(_info.FirstCluster, _info);
                    print(File.Buffer);
                    print("\r\n");
                    clear_file_buffer();
                    return;
                }
            }
        }
    }

    for(int _file_index = 0; _file_index <= get_count_file(); _file_index++){
        _info = get_file_info();
        utf16_to_ASCII(_info.Name, _buffer);
        if(_buffer[0] == 0x0){
            break;
        }
        print(_buffer);
        print("\r\n");
        clear_buffer_uint8(_buffer);
    }
    _current_index = 0;
    clear_file_buffer();
    clear_buffer_uint8(_current_name);
}

void parser(char _path[], uint8_t _name[]){
    int _current_name_index = 0;
    while(1){
        if(_path[_current_index] == '/' || _path[_current_index] == '\0'){
            _current_index++;
            break;
        }
        _name[_current_name_index] = _path[_current_index];
        _current_index++;
        _current_name_index++;
    }
}

void read_mode(uint32_t _cluster, FileInfo _info){
    if(_info.NoFATChain & (1ULL << 1)){
        for(int _read_cluster = 0; _read_cluster <= (_info.FileLength / (exFAT_attribute.SectorsPerCluster * exFAT_attribute.BytsPerSector)); _read_cluster++){
            read_cluster(_cluster);
            _cluster++;
        }
    }
    else{
        while(_cluster != 0xFFFFFFFF){
            read_cluster(_cluster);
            _cluster = walk_FAT(_cluster);
        }
    }
}