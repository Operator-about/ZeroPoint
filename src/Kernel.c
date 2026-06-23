#include<Kernel-modules.h>
#include<Ella.h>
#include<MMU.h>
#include<Stringz.h>
#include<std.h>
#include<SD.h>
#include<FAT32.h>

UART0 UART;
GICCv2* GICv2;
SDR* SD_Registers;
uint32_t SD;
volatile uint32_t DAT_buffer[128];

int main(void){
    MMU_init();

    init_t_buffer();
    Rx_buffer.head = -1;
    Rx_buffer.end = 0;
    Rx_buffer.tail = 0;

    __asm__("MOV %0, X10" : "=r"(UART.UART_DR));
    __asm__("MOV %0, X11" : "=r"(UART.UART_FR));
    __asm__("MOV %0, X12" : "=r"(UART.UART_MIS));
    __asm__("MOV %0, X13" : "=r"(UART.UART_IMSC));
    __asm__("MOV %0, X14" : "=r"(UART.UART_RIS));
    __asm__("MOV %0, X15" : "=r"(UART.UART_ICR));
    if(GIC_version_check() == 2){
        __asm__("MOV %0, X16" : "=r"(GICv2));
    }
    __asm__("MOV %0, X17" : "=r"(SD_Registers));
    __asm__("ISB");

    __asm__("MOV X10, XZR");
    __asm__("MOV X11, XZR");
    __asm__("MOV X12, XZR");
    __asm__("MOV X13, XZR");
    __asm__("MOV X14, XZR");
    __asm__("MOV X15, XZR");
    __asm__("MOV X16, XZR");

    __asm__("ISB");

    __asm__("MSR DAIFClr, #2");
    char _keyboard_buffer_input[100];
    clear_buffer(_keyboard_buffer_input);
    char _info_buffer[100];
    clear_buffer(_info_buffer);
    print("SD card init\r\n");
    SDC_init();
    uint32_t _start_LBA = FS_init();
    read_block(_start_LBA);
    init_FAT32_BPB(_start_LBA);
    print("SD card - +\r\n");
    print("Welcome! Load OS success completed! Please type - help for get more information\r\n");
    print("Or input command - about. For get information about OS\r\n");
    while(1){
        print(">>");
        input(_keyboard_buffer_input);
        if(compare_s(_keyboard_buffer_input, "about") == 1){
            clear_buffer(_keyboard_buffer_input);
            print("=========================================================\r\n");
            print("_____  _____  _____  _____  _____  _____  +  _____  +    \r\n");
            print("   // ||     ||   ||||   ||||   ||||   || | ||   ||-|-   \r\n");
            print("  //  ||____ ||     ||   ||||___||||   || | ||   || |    \r\n");
            print(" //   ||     ||     ||   ||||     ||   || | ||   || |    \r\n");
            print("//___ ||____ ||     ||___||||     ||___|| | ||   || |___ \r\n");
            print("=========================================================\r\n");
            print("Kernel: v0.0.3\r\n");
        }
        else if(compare_s(_keyboard_buffer_input, "help") == 1){
            clear_buffer(_keyboard_buffer_input);
            print("Attention! This list command work only this terminal:\r\n");
            print("about - command for shows name OS and kernel versions\r\n");
            print("help - shows this list\r\n");
        }
        else if(compare_s(_keyboard_buffer_input, "sd-life") == 1){
            clear_buffer(_keyboard_buffer_input);
            print("Init test for SD card. Target: live the SD card?\r\n");
            read_block(2048);
        }
        else{
            clear_buffer(_keyboard_buffer_input);
            print("Unknow command. Please input command: help - for more information\r\n");
        }
        __asm__("NOP");
    }
}

