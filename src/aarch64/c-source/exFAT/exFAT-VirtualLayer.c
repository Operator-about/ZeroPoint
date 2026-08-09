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

    read_cluster(exFAT_attribute.FirstRootCluster);
    display_folder();
    //display_folder();
    //clear_file_buffer();
}

void open_folder(){

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
    //clear_file_buffer();
}