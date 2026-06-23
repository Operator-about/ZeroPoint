#include<SD.h>

void SDC_init(){
    SDCMD _CMD0;
    _CMD0.arg = 0;
    _CMD0.CMD = 0;
    SD_CMD(_CMD0);
    SD_barrier(10);

    SDCMD _CMD8;
    _CMD8.arg = (0xAA << 0);
    _CMD8.arg |= (1ULL << 8);
    _CMD8.CMD = 0x0;
    _CMD8.CMD = (2ULL << 16) | (1ULL << 19) | (1ULL << 20); //Тип ответа R*
    _CMD8.CMD |= (8ULL << 24);
    SD_CMD(_CMD8);
    SD_barrier(10);
    CMD8_check();

    SD_barrier(10);

    SDCMD _CMD55;
    _CMD55.arg = 0;
    _CMD55.CMD = 0x0;
    _CMD55.CMD |= (2ULL << 16) | (1ULL << 19) | (1ULL << 20); //Тип ответа R*
    _CMD55.CMD |= (55ULL << 24);

    SDCMD _ACMD41;
    _ACMD41.arg = (1ULL << 23) | (1ULL << 22) | (1ULL << 21) | (1ULL << 20) | (1ULL << 19) | (1ULL << 18) | (1ULL << 17) | (1ULL << 16) | (1ULL << 15);
    _ACMD41.arg &= ~(1ULL << 24);
    _ACMD41.arg &= ~(1ULL << 28);
    _ACMD41.arg |= (1ULL << 30);
    _ACMD41.CMD = 0x0;
    _ACMD41.CMD |= (2ULL << 16); //Тип ответа R*
    _ACMD41.CMD |= (41ULL << 24);
    SD_Registers->RESP_SD[0] = 0x0;
    ACMD41_check(_ACMD41, _CMD55);
    //Сброс CRC т.к. ACMD41 не проверяет CRC и CRC отсутствует у ACMD41
    SD_Registers->ERS_SD &= ~(1ULL << 1);
    SD_barrier(50);
    SD_Registers->RESP_SD[0] = 0x0;
    SD_Registers->RESP_SD[1] = 0x0;

    SD_barrier(1000);

    SDCMD _CMD2;
    _CMD2.arg = 0x0;
    _CMD2.CMD = 0x0;
    _CMD2.CMD |= (1ULL << 16) | (1ULL << 19);
    _CMD2.CMD |= (2ULL << 24);
    SD_CMD(_CMD2);
    SD_barrier(50);

    SD_barrier(1000);

    SDCMD _CMD3;
    SD_Registers->RESP_SD[0] = 0x0;
    _CMD3.arg = 0x0;
    _CMD3.CMD = 0x0;
    _CMD3.CMD |= (2ULL << 16) | (1ULL << 19) | (1ULL << 20);
    _CMD3.CMD |= (3ULL << 24);
    SD_CMD(_CMD3);
    SD_barrier(50);
    SD = SD_Registers->RESP_SD[0] & 0xFFFF0000;

    SD_barrier(1000);

    SDCMD _CMD7;
    _CMD7.arg = 0x0;
    _CMD7.arg |= SD;
    _CMD7.CMD = 0x0;
    _CMD7.CMD |= (3ULL << 16) | (1ULL << 19) | (1ULL << 20);
    _CMD7.CMD |= (7ULL << 24);
    SD_CMD(_CMD7);
    SD_get_status();

    SD_barrier(1000);

    SD_Registers->CC_SD &= ~(1ULL << 2);
    SD_Registers->CC_SD &= ~(1ULL << 0);

    uint32_t _base_mhz = ((SD_Registers->CB_SD >> 8) & 0xFF);
    uint32_t _result = (_base_mhz / (2 * 25)); //Формула для расчёта тактирования: базовая_скорость / 2 * желаемая
    uint32_t _up = ((_result & 0x300) >> 2); //Перенос 9:8 битов в 7:6
    uint32_t _bottom = ((_result & 0x7F) << 8); //Перенос 7:0 битов в 15:8
    SD_Registers->CC_SD = _up | _bottom;
    SD_Registers->CC_SD |= (1ULL << 0);
    while((SD_Registers->CC_SD & (1ULL << 1)) == 0){
        print("[^]Stable to 25mHz\r\n");
    }
    SD_Registers->CC_SD |= (1ULL << 2);
    SD_barrier(50);


    SD_Registers->NS_SD &= ~(1ULL << 1);
    SD_barrier(1000);

    SD_Registers->SR_SD |= (1ULL << 2);
    SD_barrier(1000);
}

void SD_CMD(SDCMD _CMD){
    SD_Registers->ARG_SD = _CMD.arg;
    SD_Registers->CMD_SD = _CMD.CMD;
}

void SD_get_status(){
    if(SD_Registers->NS_SD & (1ULL << 0)){
        print("[^]CDM complet. Please see next log for more details:\r\n");
    }

    if(SD_Registers->ERS_SD & (1ULL << 0)){
        print("[*]Timeout error!\r\n");
    }
    else if(SD_Registers->ERS_SD & (1ULL << 1)){
        print("[*]CRC error!\r\n");
    }
    else if(SD_Registers->ERS_SD & (1ULL << 4)){
        print("[*]Data timeout error\r\n");
    }
    else if(SD_Registers->ERS_SD & (1ULL << 5)){
        print("[*]CRC data error\r\n");
    }
    else{
        print("[^]Error no detected\r\n");
    }
}

void ACMD41_check(SDCMD _ACMD41, SDCMD _CMD55){
    while((SD_Registers->RESP_SD[0] & (1ULL << 31)) == 0){
        SD_CMD(_CMD55);
        SD_barrier(10);
        SD_CMD(_ACMD41);
        SD_barrier(10);
        print("[^]SD the busy. Please wait\r\n");
    }

    if(SD_Registers->RESP_SD[0] & (1ULL << 30)){
        print("[^]SDCH card support\r\n");
    }

    if(((SD_Registers->RESP_SD[0] >> 15) & 0xFF) == 0xFF){
        print("[^]2.7v ~ 3.3v in VDD support and enable\r\n");
    }
    else{
        print("[*]This SD card is not support current VDD\r\n");
        return;
    }
    print("[+]SD init ACMD41 done!\r\n");
}

void CMD8_check(){
    if(SD_Registers->RESP_SD[0] & (1ULL << 8)){
        print("[^]2.6v ~ 3.3v support\r\n");
    }

    if((SD_Registers->RESP_SD[0] & 0xFF) == 0xAA){
        print("[^]Pattern correct\r\n");
    }
}

void SD_barrier(int _seconds_wait){
    int _wait_time = ((60 * 60) * _seconds_wait);
    for(int _wait = 0; _wait < _wait_time; _wait++){
        __asm__("NOP");
    }
}

void SD_answer(){
    if(SD_Registers->RESP_SD[0] == 0x0){
        print("Recived 0x0\r\n");
        return;
    }
    else{
        print("Recived without 0x0\r\n");
    }   
}

void read_block(uint32_t _block_number){
    SD_barrier(1000);
    
    SDCMD _CMD17;
    _CMD17.arg = 0x0;
    _CMD17.arg = _block_number;
    _CMD17.CMD = 0x0;
    _CMD17.CMD |= (1ULL << 1) | (1ULL << 4);
    _CMD17.CMD &= ~(1ULL << 0);
    _CMD17.CMD |= (2ULL << 16) | (1ULL << 19) | (1ULL << 20) | (1ULL << 21) | (17ULL << 24); //21(5 бит) - Data present - за этой командой нужно ожидать данные
    SD_CMD(_CMD17);
    SD_barrier(1000);
}

uint32_t FS_init(){
    SD_barrier(1000);
    
    SDCMD _CMD17;
    _CMD17.arg = 0x0;
    _CMD17.arg = 0;
    _CMD17.CMD = 0x0;
    _CMD17.CMD |= (1ULL << 1) | (1ULL << 4);
    _CMD17.CMD &= ~(1ULL << 0);
    _CMD17.CMD |= (2ULL << 16) | (1ULL << 19) | (1ULL << 20) | (1ULL << 21) | (17ULL << 24); //21(5 бит) - Data present - за этой командой нужно ожидать данные
    SD_CMD(_CMD17);
    SD_barrier(1000);

    MBR* _MBR;
    _MBR->PartionRecord[0].StartLBA = 0x0;
    _MBR->Signature = 0x0;
    _MBR->UMBRDS = 0x0;
    _MBR = (MBR*)DAT_buffer;

    uint8_t* _test = (uint8_t*)DAT_buffer;
    if(_test[510] == 0x55 && _test[511] == 0xAA){
        print("[+]Signature ligel\r\n");
    }

    if(_MBR->PartionRecord[0].StartLBA != 0x0){
        print("[+]StartLBA reciving\r\n");
        char _info[100];
        itos((int)_MBR->PartionRecord[0].StartLBA, _info);
        print(_info);
        print("\r\n");
    }

    if(_MBR->Signature != 0x0){
        print("[+]Signature reciving\r\n");
        char _info[100];
        clear_buffer(_info);
        itos((int)_MBR->Signature, _info);
        print(_info);
        print("\r\n");
    }

    if(_MBR->UMBRDS != 0x0){
        print("[+]UMBRDS!\r\n");
    }

    return _MBR->PartionRecord[0].StartLBA;
}
