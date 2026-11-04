#include<ZeroShell.h>

char CurrentNameDir[256];
char KeyboardBufferInput[256];

void shell_init(){
    ZeroPoint_logo();

    clear_buffer(KeyboardBufferInput);
    clear_buffer(CurrentNameDir);

    while(1){
        clear_buffer(KeyboardBufferInput);
        clear_buffer(CurrentNameDir);
        utf16_to_ASCII(CurrentFolder.Name, CurrentNameDir);
        print(CurrentNameDir);
        print("@>>");

        input(KeyboardBufferInput);
        if(compare_s(KeyboardBufferInput, "about") == 1){
            ZeroPoint_logo();
        }
        else if(compare_s(KeyboardBufferInput, "help") == 1){
            help_command();
        }
        else if(compare_s(KeyboardBufferInput, "open") == 1){
            CurrentIndex = 5;
            open_mode();
            CurrentIndex = 0;
        }
        else{
            print("Unknow command. Please input command: help - for more information\r\n");
        }
    }
}

void ZeroPoint_logo(){
    print("========================================================\r\n");
    print("_____  _____  _____  _____  _____  _____  +  _____  +    \r\n");
    print("   // ||     ||   ||||   ||||   ||||   || | ||   ||-|-   \r\n");
    print("  //  ||____ ||     ||   ||||___||||   || | ||   || |    \r\n");
    print(" //   ||     ||     ||   ||||     ||   || | ||   || |    \r\n");
    print("//___ ||____ ||     ||___||||     ||___|| | ||   || |___ \r\n");
    print("========================================================\r\n");
    print("Kernel: v0.4.0\r\n");
    print("Build date: 26.08.2026\r\n");
}

void open_mode(){
    switch(OutJump->SD_Standart){
        case 0x00000000:
            print("FS operation unsupported for this device. Because SD card not indeficator\r\n");
            break;
        default:
            open(KeyboardBufferInput);
            break;
        }
}

void help_command(){
    print("Attention! This list command work only this terminal:\r\n");
    print("about - command for shows name OS and kernel versions\r\n");
    print("help - shows this list\r\n");
    print("open - for open file/folder\r\n");
}