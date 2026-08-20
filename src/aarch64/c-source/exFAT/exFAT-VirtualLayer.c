#include<exFAT.h>

int CurrentIndex = 0;
FileInfo CurrentFolder;

void open(char _name[]){
    CurrentIndex = 5;

    if(_name[5] == '\0' || _name[5] == ' '){
        print("Name invalid\r\n");
        return;
    }

    uint8_t _current_name[260];
    uint8_t _current_name_in_ASCII[260];
    for(int _clear = 0; _clear < 260; _clear++){ 
        _current_name[_clear] = 0x0;
        _current_name_in_ASCII[_clear] = 0x0;
    }
    FileInfo _info;


    uint32_t _current_cluster = CurrentFolder.FirstCluster;
    int _cluster_count = 0;
    if(_name[5] == '/'){
        CurrentFolder.FirstCluster = exFAT_attribute.FirstRootCluster;
        CurrentFolder.NoFATChain = 0x0;
        _current_cluster = CurrentFolder.FirstCluster;
        while(_current_cluster != 0xFFFFFFFF){
            read_cluster(_current_cluster);
            display_folder();
            _current_cluster = walk_FAT(_current_cluster);
        }
        
        current_folder_name_clear();
        return;
    }

    read_cluster(_current_cluster);
    parser(_name, _current_name);
    _cluster_count = CurrentFolder.FileLength / (exFAT_attribute.BytsPerSector * exFAT_attribute.SectorsPerCluster);

    for(int _get = 0; _get <= get_count_file(); _get++){
        _info = get_file_info();
        utf16_to_ASCII(_info.Name, _current_name_in_ASCII);

        if(name_compare_hash(_current_name) == name_compare_hash(_current_name_in_ASCII)){
            switch(_info.FileAttribute){
                case (1ULL << 4):
                    CurrentFolder = _info;
                    if(_name[CurrentIndex] == '\0'){
                        open_folder();
                        clear_buffer(_current_name);
                        clear_buffer(_current_name_in_ASCII);
                        return;
                    }
                    else{
                        current_folder_name_clear();

                        _current_cluster = _info.FirstCluster;
                        CurrentFolder.FirstCluster = exFAT_attribute.FirstRootCluster;

                        read_cluster(_current_cluster);

                        _cluster_count = _info.FileLength / (exFAT_attribute.BytsPerSector * exFAT_attribute.SectorsPerCluster);
                        _get = 0;
                    }
                    break;
                default:
                    open_file(_info);
                    clear_buffer(_current_name);
                    clear_buffer(_current_name_in_ASCII);
                    return;
            }
            clear_buffer(_current_name);
            parser(_name, _current_name);
        }
        else if(_cluster_count > 0 && _get >= get_count_file()){
            switch(CurrentFolder.NoFATChain){
                case (1ULL << 1):
                    _current_cluster++;
                    read_cluster(_current_cluster);
                    break;
                default:
                    _current_cluster = walk_FAT(_current_cluster);
                    read_cluster(_current_cluster);
                    break;
            }
            _cluster_count--;
        }
        else if(_cluster_count == 0 && _get >= get_count_file()){
            print("File not found\r\n");
            return;
        }
        clear_buffer(_current_name_in_ASCII);
    }
}

void open_folder(){
    uint32_t _current_cluster = CurrentFolder.FirstCluster;
    if((CurrentFolder.NoFATChain & (1ULL << 1)) != 0){
        for(int _get = 0; _get <= CurrentFolder.FileLength / (exFAT_attribute.BytsPerSector * exFAT_attribute.SectorsPerCluster); _get++){
            read_cluster(_current_cluster);
            display_folder();
            UART.UARTF->wait_transmition();
            _current_cluster++;
        }
    }
    else{
        do{
            read_cluster(_current_cluster);
            display_folder();
            UART.UARTF->wait_transmition();
            _current_cluster = walk_FAT(_current_cluster);
        }while(_current_cluster != 0xFFFFFFFF);
    }
}

void open_file(FileInfo _info){
    uint32_t _current_cluster = _info.FirstCluster;
    if(_info.NoFATChain & (1ULL << 1)){
        for(int _get = 0; _get <= _info.FileLength / (exFAT_attribute.BytsPerSector * exFAT_attribute.SectorsPerCluster); _get++){
            read_cluster(_current_cluster);
            print(File.Buffer);
            UART.UARTF->wait_transmition();
            _current_cluster++;
        }
    }
    else{
        do{
            read_cluster(_current_cluster);
            print(File.Buffer);
            UART.UARTF->wait_transmition();
            _current_cluster = walk_FAT(_current_cluster);
        }while(_current_cluster != 0xFFFFFFFF);

    }
    print("\n\r");
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

void display_folder(){
    FileInfo _info;
    uint8_t _name_ASCII[260];
    clear_buffer(_name_ASCII);
    for(int _display = 0; _display <= get_count_file(); _display++){
        _info = get_file_info();
        if(_info.Name[0] == 0x0){
            clear_buffer(_name_ASCII);
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
}

void current_folder_name_clear(){
    for(int _clear = 0; _clear < 260; _clear++){
        CurrentFolder.Name[_clear] = 0x0;
    }
}