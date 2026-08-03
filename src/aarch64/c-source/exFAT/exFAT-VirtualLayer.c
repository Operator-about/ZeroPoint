#include<exFAT.h>

int CurrentIndex = 0;
FileInfo CurrentFolder;

void open(char _name[]){
    uint8_t _current_name[260];
    uint8_t _current_name_in_ASCII[260];
    for(int _clear = 0; _clear < 260; _clear++){
        _current_name[_clear] = 0x0;
        _current_name_in_ASCII[_clear] = 0x0;
    }
    FileInfo _info;

    clear_file_buffer();

    if(_name[0] == '/'){
        CurrentFolder.FirstCluster = exFAT_attribute.FirstRootCluster;
        CurrentFolder.FileAttribute = 0x0;
        CurrentFolder.NoFATChain = 0x0;
        read_mode(CurrentFolder.FirstCluster, CurrentFolder);
        display_folder();
        clear_file_buffer();
        for(int _clear = 0; _clear < 260; _clear++){
            CurrentFolder.Name[_clear] = 0x0;
        }
        return;
    }

    read_mode(CurrentFolder.FirstCluster, CurrentFolder);
    parser(_name, _current_name);
    
    for(int _index = 0; _index <= get_count_file(); _index++){
        _info = get_file_info();
        utf16_to_ASCII(_info.Name, _current_name_in_ASCII);

        if(name_compare_hash(_current_name) == name_compare_hash(_current_name_in_ASCII)){
            clear_file_buffer();
            if(_info.FileAttribute & (1ULL << 4)){
                read_mode(_info.FirstCluster, _info);
                if(_name[CurrentIndex] == 0x0){
                    display_folder();
                    for(int _clear = 0; _clear < 260; _clear++){
                        CurrentFolder.Name[_clear] = 0x0;
                    }
                    CurrentFolder = _info;
                    break;
                }
                else{
                    _index = 0;
                }
            }
            else{
                read_mode(_info.FirstCluster, _info);
                print(File.Buffer);
                print("\r\n");
                break;
            }
            clear_buffer(_current_name);
            parser(_name, _current_name);
        }
        clear_buffer(_current_name_in_ASCII);
    }

    clear_file_buffer();
    clear_buffer(_current_name);
    clear_buffer(_current_name_in_ASCII);
    CurrentIndex = 0;
}

void parser(char _path[], uint8_t _name[]){
    int _current_name_index = 0;
    while(1){
        if(_path[CurrentIndex] == '/' || _path[CurrentIndex] == '\0'){
            CurrentIndex++;
            break;
        }
        _name[_current_name_index] = _path[CurrentIndex];
        CurrentIndex++;
        _current_name_index++;
    }
}

void read_mode(uint32_t _cluster, FileInfo _info){
    if(_info.NoFATChain & (1ULL << 1)){ //Если файл/директория записан(-а) способом цепочки(NoFATChain)
        for(int _read_cluster = 0; _read_cluster <= (_info.FileLength / (exFAT_attribute.SectorsPerCluster * exFAT_attribute.BytsPerSector)); _read_cluster++){
            read_cluster(_cluster);
            _cluster++;
        }
    }
    else{
        while(_cluster != 0xFFFFFFFF){ //Если FAT таблицей
            read_cluster(_cluster);
            _cluster = walk_FAT(_cluster);
        }
    }
}

void display_folder(){
    FileInfo _info;
    uint8_t _name_ASCII[260];
    clear_buffer(_name_ASCII);
    for(int _display = 0; _display <= get_count_file(); _display++){
        _info = get_file_info();
        if(_info.Name[0] == 0x0){
            break;
        }
        utf16_to_ASCII(_info.Name, _name_ASCII);
        print("|-");
        print(_name_ASCII);
        switch(_info.FileAttribute){
            case (1ULL << 4):
                print(" - Directory\r\n");
                break;
            default:
                print(" - File\r\n");
                break;
        }
        clear_buffer(_name_ASCII);
    }
    clear_file_buffer();
}