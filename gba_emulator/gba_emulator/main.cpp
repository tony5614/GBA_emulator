// gba_emulator.cpp : Defines the entry point for the console application.
//

#include "stdio.h"
#include "conio.h"
#include "arm7tdmi.hpp"

int main()
{
    printf("%x %u\n", sizeof(ROM_HEADER), sizeof(ROM_HEADER));
    getchar();
    return 0;
}

