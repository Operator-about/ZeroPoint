#include<Kernel-modules.h>
#include<MMU.h>
#include<Stringz.h>
#include<std.h>
#include<SD.h>
#include<exFAT.h>
#include<PL011.h>
#include<16550.h>
#include<SD-Standart.h>

GICCv2* GICv2;
JumpData* OutJump;
HALUART UART;
HALSD SD;

int main(void){
    __asm__("MOV %0, X10" : "=r"(OutJump));

    MMU_init();

    UART.UARTAddress = OutJump->UART;
    switch(OutJump->UART_Standart){
        case 0x504C00B0:
            UART.UARTF = &UARTPL011F;
            UART.UARTF->Register_init();
            break;
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

    init_exFAT();
    print("[^]exFAT init stage done\r\n");

    char _info_buffer[100];
    uint8_t _current_name_dir[260];
    clear_buffer(_info_buffer);
    clear_buffer(_current_name_dir);

    while(1){
        clear_buffer(_keyboard_buffer_input);
        utf16_to_ASCII(CurrentFolder.Name, _current_name_dir);
        print(_current_name_dir);
        print("@>>");

        input(_keyboard_buffer_input);
        if(compare_s(_keyboard_buffer_input, "about") == 1){
            print("=========================================================\r\n");
            print("_____  _____  _____  _____  _____  _____  +  _____  +    \r\n");
            print("   // ||     ||   ||||   ||||   ||||   || | ||   ||-|-   \r\n");
            print("  //  ||____ ||     ||   ||||___||||   || | ||   || |    \r\n");
            print(" //   ||     ||     ||   ||||     ||   || | ||   || |    \r\n");
            print("//___ ||____ ||     ||___||||     ||___|| | ||   || |___ \r\n");
            print("=========================================================\r\n");
            print("Kernel: v0.0.3\r\n");
            print("Build date: 15.08.2026\r\n");
        }
        else if(compare_s(_keyboard_buffer_input, "help") == 1){
            print("Attention! This list command work only this terminal:\r\n");
            print("about - command for shows name OS and kernel versions\r\n");
            print("help - shows this list\r\n");
            print("open - for open file/folder\r\n");
        }
        else if(compare_s(_keyboard_buffer_input, "open") == 1){
            clear_buffer(_current_name_dir);

            print("Input full name:");
            clear_buffer(_keyboard_buffer_input);
            
            input(_keyboard_buffer_input);
            open(_keyboard_buffer_input);
        }
        else{
            print("Unknow command. Please input command: help - for more information\r\n");
        }
    }
}

