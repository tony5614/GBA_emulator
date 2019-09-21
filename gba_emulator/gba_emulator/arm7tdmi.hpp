#pragma once

typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned int   U32;


//memory map

//external memory(game pak)
#define CARTRIDGE_ROM_WAIT_STATE_0_BASE      (0x08000000)
#define CARTRIDGE_ROM_WAIT_STATE_0_SIZE      (0x02000000)


#define NUM_OF_REGISTER                      (16)
#define USR_MODE                             (0x10)    // 10000b
#define FIQ_MODE                             (0x11)    // 10001b
#define IRQ_MODE                             (0x12)    // 10010b
#define SVC_MODE                             (0x13)    // 10011b
#define ABT_MODE                             (0x17)    // 10111b
#define UND_MODE                             (0x1B)    // 11011b
#define SYS_MODE                             (0x1F)    // 11111b


#pragma pack(1)
typedef struct cartridge_rom_header
{
    U32 rom_entry_point;
    U8  nintendo_logo[156];
    U8  game_title[12];
    U8  game_code[4];
    U8  maker_code[2];
    U8  fixed_value;                //must be 096h
    U8  main_unit_code;             //00h for current gba models
    U8  device_type;
    U8  reserved0[7];
    U8  software_version;
    U8  complement_check;           //checksum
    U8  reserved1[2];
    // 192 bytes
    U32 ram_entry_point;            //additional multiboot header entries
    U8  boot_mode;
    U8  slave_id_number;
    U8  reserved3[26];
}
ROM_HEADER;


typedef struct cartridge_rom_header2 //192 bytes
{
    U32 rom_entry_point;
    U8  nintendo_logo[156];
    U8  game_title[12];
    U8  game_code[4];
    U8  maker_code[2];

}
ROM_HEADER2;
#pragma pack()

typedef struct program_status_register 
{
    U32 mode : 5;
    U32 T    : 1;     //reflect the processor is executing in THUMB or ARM state, the software must never change the state of this bit
    U32 F    : 1;     //disable FIQ
    U32 I    : 1;     //disable IRQ
    U32 rsv  : 20;
    U32 V    : 1;
    U32 C    : 1;
    U32 Z    : 1;
    U32 N    : 1;
}CPSR, SPSR;

class ARM7TDMI
{
public:
    //ARM state general egister and program counter
    //R13 : SP
    //R14 : LR
    //R15 : PC
    //system and user mode
    U32 R[NUM_OF_REGISTER];
    U32 R08_fiq, R09_fiq, R10_fiq, R11_fiq, R12_fiq, R13_fiq, R14_fiq;
    U32 R13_svc, R14_svc;
    U32 R13_abt, R14_abt;
    U32 R13_irq, R14_irq;
    U32 R13_und, R14_und;

    //ARM state program status registers
    CPSR CPSR_usr;      
    SPSR SPSR_fiq;
    SPSR SPSR_svc;
    SPSR SPSR_abt;
    SPSR SPSR_irq;
    SPSR SPSR_und;


    void run() 
    {
    }
};
