#include<SD.h>

void SDC_init(){
    SDCMD _CMD0;
    _CMD0.arg &= ~(0xFFFFFFFFU << 0);
    _CMD0.CMD &= ~(0xFFFFFFFFU << 0);
    SD_CMD(_CMD0);

    SDCMD _CMD8;
    _CMD8.arg = (0xAA << 0);
    _CMD8.arg |= (1ULL << 8);
    _CMD8.CMD = (2ULL << 0);
    _CMD8.CMD |= (8ULL << 8);
    SD_CMD(_CMD8);
    SD_get_status(_CMD8, 8);
    if((SD_Registers->RESP_SD[0] & 0xFF) == 0xAA){
        print("Pattern correct\r\n");
    }

    if(SD_Registers->RESP_SD[0] & (1ULL << 8)){
        print("Voltage support(2.6v-3.3v)\r\n");
    }

    while((SD_Registers->PS_SD & (1ULL << 0)) != 0){
        print("Wait for finish job SD\r\n");
        __asm__("NOP");
    }
}

void SD_CMD(SDCMD _CMD){
    SD_Registers->ARG_SD = _CMD.arg;
    SD_Registers->CMD_SD = _CMD.CMD;
}

void SD_get_status(SDCMD _CMD, int _CMD_index){
    print("Status:\r\n");

    if(SD_Registers->NS_SD & (1ULL << 0)){
        print("CDM complet. Please see next log for more details:\r\n");
    }

    if(SD_Registers->ERS_SD & (1ULL << 0)){
        print("Timeout error!\r\n");
    }
    else{
        print("Error no detected\r\n");
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