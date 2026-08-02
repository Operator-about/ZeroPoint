#include<exFAT.h>

int CurrentIndex = 0;
FileInfo CurrentFolder;

void open(char _name[]){
    uint8_t _current_name[260];
    for(int _clear = 0; _clear < 260; _clear++){
        _current_name[_clear] = 0x0;
    }
    FileInfo _info;
    
    if(_name[0] == '/'){ //Если путь файла начинается с точки монтирования
        //Обнуление текущей папки
        CurrentFolder.FirstCluster = exFAT_attribute.FirstRootCluster;
        CurrentFolder.FileAttribute = 0x0;
        CurrentFolder.NoFATChain = 0x0;
        CurrentIndex = 1; //Пропуск /
        for(int _clear = 0; _clear < 260; _clear++){
            CurrentFolder.Name[_clear] = 0x0;
        }
    }
    else{ //Если нет
        CurrentIndex = 0;
    }

    read_cluster(CurrentFolder.FirstCluster);
    if(_name[1] == '\0'){ //Если дальше нет пути
        display_folder();
        clear_file_buffer();
        return;
    }

    parser(_name, _current_name); //Взятие следующей части пути

    for(int _index = 0; _index <= get_count_file(); _index++){
        _info = get_file_info();
        if(compare_u16_to_ASCII(_info.Name, _current_name) == 1){ //Если найден(-а) нужный(-ая) файл/директория
            clear_file_buffer();

            read_cluster(_info.FirstCluster);
            clear_buffer(_current_name);

            if(_info.FileAttribute & (1ULL << 4)){ //Если директорий
                if(_name[CurrentIndex] == '\0'){ //Если дальше нет пути
                    CurrentFolder = _info;
                    display_folder(); //Отображение папки
                    break;
                }
                else{
                    _index = 0; //Если да, то продолжается поиск файла/директории
                }
            }
            else{ //Если файл
                print(File.Buffer);
                print("\r\n");
                break;
            }
            clear_buffer(_current_name);
            parser(_name, _current_name);
        }
        else if(_index == get_count_file()){
            print("File not found\r\n");
            break;
        }
    }
    
    clear_file_buffer();
    clear_buffer(_current_name);
    clear_buffer(_name);
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