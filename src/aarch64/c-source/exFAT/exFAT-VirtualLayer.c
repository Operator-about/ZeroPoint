#include<exFAT.h>

int CurrentIndex = 0;
FileInfo CurrentFolder;
uint32_t* AppBuffer;
int AppIndexBuffer;

void open(char _name[]){
    if(_name[CurrentIndex] == '\0' || _name[CurrentIndex] == ' '){
        print("Name invalid\r\n");
        return;
    }

    uint8_t _current_name[260];
    for(int _clear = 0; _clear < 260; _clear++){ 
        _current_name[_clear] = 0x0;
    }
    FileInfo _info;


    uint32_t _current_cluster = CurrentFolder.FirstCluster;
    int _cluster_count = 0;
    if(_name[CurrentIndex] == '/'){
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

        if(name_compare_hash(_current_name) == name_compare_hash(_info.Name)){
            switch(_info.FileAttribute){
                case (1ULL << 4):
                    CurrentFolder = _info;
                    if(_name[CurrentIndex] == '\0'){
                        open_file(&CurrentFolder);
                        clear_buffer(_current_name);
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
                    open_file(&_info);
                    clear_buffer(_current_name);
                    print("\r\n");
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
    }
}

void open_file(FileInfo* _info){
    uint32_t _current_cluster = _info->FirstCluster;
    
    if(check_app(_info) == 1){
        AppBuffer = (uint32_t*)0x40010250;
        AppIndexBuffer = 0;
    }
    
    if(_info->NoFATChain & (1ULL << 1)){
        if(_info->FileLength <= (exFAT_attribute.BytsPerSector * exFAT_attribute.SectorsPerCluster)){
            read_cluster(_current_cluster);
            file_or_dir(_info);
            UART.UARTF->wait_transmition();
            return;
        }
        
        for(int _get = 0; _get <= _info->FileLength / (exFAT_attribute.BytsPerSector * exFAT_attribute.SectorsPerCluster); _get++){
            read_cluster(_current_cluster);
            file_or_dir(_info);
            UART.UARTF->wait_transmition();
            _current_cluster++;
        }
    }
    else{
        do{
            read_cluster(_current_cluster);
            file_or_dir(_info);
            UART.UARTF->wait_transmition();
            _current_cluster = walk_FAT(_current_cluster);
        }while(_current_cluster != 0xFFFFFFFF);
    }

    if(check_app(_info) == 1){
        go_to_EL0();
    }
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
    if(get_count_file() == 0){
        print("Folder empety\r\n");
        return;
    }

    for(int _display = 0; _display < get_count_file(); _display++){
        _info = get_file_info();
        if(_info.Name[0] == 0x0){
            break;
        }
        print("|-");
        print(_info.Name);
        switch(_info.FileAttribute){
            case (1ULL << 4):
                print(" - Directory\r\n");
                break;
            default:
                print(" - File\r\n");
                break;
        }
    }
}

void current_folder_name_clear(){
    for(int _clear = 0; _clear < 260; _clear++){
        CurrentFolder.Name[_clear] = 0x0;
    }
}

void file_or_dir(FileInfo* _info){
    switch(_info->FileAttribute){
        case (1ULL << 4):
            display_folder();
            break;
        default:
            if(check_app(_info) == 1){
                to_app();
            }
            print(File.Buffer);
            break;
    }
}

int check_app(FileInfo* _info){
    int _current_index_name = 0;

    while(_info->Name[_current_index_name] != '.'){
        _current_index_name++;
    }

    _current_index_name++;

    if(_info->Name[_current_index_name] == 'b' && _info->Name[_current_index_name+1] == 'i' && _info->Name[_current_index_name+2] == 'n'){
        return 1;
    }
    else{
        return 0;
    }
}

void to_app(){
    for(int _get = 0; _get < File.Buffer_index; _get+=4){
        AppBuffer[AppIndexBuffer] = *(uint32_t*)&File.Buffer[_get];
        AppBuffer++;
    }
}