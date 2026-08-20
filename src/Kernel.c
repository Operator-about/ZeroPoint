#include<Kernel-modules.h>
#include<MMU.h>
#include<Stringz.h>
#include<std.h>
#include<SD.h>
#include<exFAT.h>
#include<UART.h>
#include<MBR.h>
#include<ZeroShell.h>

GICCv2* GICv2;
JumpData* OutJump;
HALUART UART;
HALSD SD;

int main(void){
    __asm__("MOV %0, X10" : "=r"(OutJump));

    MMU_init();

    UART.UARTAddress = OutJump->UART;
    switch(OutJump->UART_Standart){
        case 0x504C00B0: //PL011
            UART.UARTF = &UARTPL011F;
            UART.UARTF->Register_init();
            break;
        case 0x10323200: //165050
            UART.UARTF = &UART165050F;
            UART.UARTF->Register_init();
        default:
            break;
    }

    GICv2 = (GICCv2*)OutJump->GICv2;

    switch(OutJump->SD_Standart){
        case 0x5354414E:
            SD.SDAddress = OutJump->SD;
            SD.SDF = &SDAF;
            SD.SDF->register_init();
            break;
        case 0x44455300: //Rock 4
            SD.SDAddress = OutJump->SD;
            SD.SDF = &SDRF;
            SD.SDF->register_init();    
            break;
        default:
            break;
    }

    Rx_buffer.head = -1;
    Rx_buffer.end = 0;
    Rx_buffer.tail = 0;

    __asm__("MOV X10, XZR");

    __asm__("ISB");

    __asm__("MSR DAIFClr, #2");
    char _keyboard_buffer_input[100];

    switch(OutJump->SD_Standart){
        case 0x00000000:
            print("FS init unsupported for this device. Because SD card not indeficator\r\n");
            break;
        default:
            init_MBR();
            get_LBA_for_exFAT();
            init_exFAT();
            print("[^]exFAT init stage done\r\n");
            break;
    }

    shell_init();
}

