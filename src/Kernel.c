#include<Interrupts.h>
#include<Kernel-modules.h>
#include<Ella.h>
#include<GIC.h>
#include<MMU.h>
#include<UART.h>
#include<Stringz.h>

struct UART* UARTPL011;
struct GICv3 GICv3_;
struct GICv2 GICv2_;
struct GICv3_registers_data GICv3_registers;

int main(void){
    Rx_buffer.head=-1;
    MMU_init();

    switch(GIC_version_check()){
        case 3:
            GICv3_registers_init();
            GICv3_.GICD = (struct GICDv3*)0x08000000; //Указание начало GICD
            GICv3_.GICR[0] = (struct GICRv3*)0x080A0000; //Указание начало GICR
            GICDv3_init(); //Настройка GIC для EL1
            GICRv3_init();
            break;
        case 2:
            GICv2_.GICC = (struct GICCv2*)0x08010000;
            GICv2_.GICD = (struct GICDv2*)0x08000000;
            GICDv2_init();
            GICCv2_init();
            break;
        default:
            break;
    }


    UARTPL011 = (struct UART*)0x09000000; //Указание начало UART
    UARTPL011_init(); //Настройка UART

    char _keyboard_buffer_input[100];
    char _info_buffer[100];
    while(1){
        write(">>");
        read(_keyboard_buffer_input);
        if(compare_s(_keyboard_buffer_input, "about") == 1){
            clear_buffer(_keyboard_buffer_input);
            write("=========================================================\r\n");
            write("_____  _____  _____  _____  _____  _____  +  _____  +    \r\n");
            write("   // ||     ||   ||||   ||||   ||||   || | ||   ||-|-   \r\n");
            write("  //  ||____ ||     ||   ||||___||||   || | ||   || |    \r\n");
            write(" //   ||     ||     ||   ||||     ||   || | ||   || |    \r\n");
            write("//___ ||____ ||     ||___||||     ||___|| | ||   || |___ \r\n");
            write("=========================================================\r\n");
            write("Kernel: v0.0.2\r\n");
        }
        else if(compare_s(_keyboard_buffer_input, "help") == 1){
            clear_buffer(_keyboard_buffer_input);
            write("Attention! This list command work only this terminal:\r\n");
            write("about - command for shows name OS and kernel versions\r\n");
            write("help - shows this list\r\n");
        }
        else{
            clear_buffer(_keyboard_buffer_input);
            write("Unknow command. Please input command: help - for more information\r\n");
        }
        __asm__("NOP");
    }
}

