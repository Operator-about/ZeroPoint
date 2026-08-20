#include<MBR.h>

MBR MBRM;
uint32_t LBA;

void init_MBR(){
    SD.SDF->single_read(0);
    MBRM = *(MBR*)&File.Buffer;
}

void get_LBA_for_exFAT(){
    int _partion_index = 0;
    exFAT* _exFATp;
    
    while(_partion_index < 4){
        LBA = *(uint32_t*)&MBRM.PartionsRecords[_partion_index].StartLBA;

        SD.SDF->single_read(LBA);
        _exFATp = (exFAT*)&File.Buffer;
        if(compare_s(_exFATp->FileSystemName, "EXFAT   ") == 1){
            break;
        }
        LBA = 0x0;
        _partion_index++;
    }
}