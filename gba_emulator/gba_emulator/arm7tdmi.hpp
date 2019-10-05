#pragma once

#include <fstream>
#include <stdio.h>
#include "opcode.hpp"


typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned int   U32;



//memory map

// General Internal Memory
// 00000000 - 00003FFF   BIOS - System ROM(16 KBytes)
// 00004000 - 01FFFFFF   Not used
// 02000000 - 0203FFFF   WRAM - On - board Work RAM(256 KBytes) 2 Wait
// 02040000 - 02FFFFFF   Not used
// 03000000 - 03007FFF   WRAM - On - chip Work RAM(32 KBytes)
// 03008000 - 03FFFFFF   Not used
// 04000000 - 040003FE   I / O Registers
// 04000400 - 04FFFFFF   Not used
// Internal Display Memory
// 05000000 - 050003FF   BG / OBJ Palette RAM(1 Kbyte)
// 05000400 - 05FFFFFF   Not used
// 06000000 - 06017FFF   VRAM - Video RAM(96 KBytes)
// 06018000 - 06FFFFFF   Not used
// 07000000 - 070003FF   OAM - OBJ Attributes(1 Kbyte)
// 07000400 - 07FFFFFF   Not used
// External Memory(Game Pak)
// 08000000 - 09FFFFFF   Game Pak ROM / FlashROM(max 32MB) - Wait State 0
// 0A000000 - 0BFFFFFF   Game Pak ROM / FlashROM(max 32MB) - Wait State 1
// 0C000000 - 0DFFFFFF   Game Pak ROM / FlashROM(max 32MB) - Wait State 2
// 0E000000 - 0E00FFFF   Game Pak SRAM(max 64 KBytes) - 8bit Bus width
// 0E010000 - 0FFFFFFF   Not used



// General Internal Memory
#define BIOS_BASE_LOG                            (0x00000000)
#define BIOS_SIZE                                (0x00004000)
#define ON_BOARD_WRAM_BASE_LOG                   (0x02000000)
#define ON_BOARD_WRAM_SIZE                       (0x00040000)
#define ON_CHIP_WRAM_BASE_LOG                    (0x03000000)
#define ON_CHIP_WRAM_SIZE                        (0x00008000)
#define IO_REGISTER_BASE_LOG                     (0x04000000)
#define IO_REGISTER_SIZE                         (0x00000400)   //actually (0x000003FF)
// internal display memory
#define PALETTE_RAM_BASE_LOG                     (0x05000000)
#define PALETTE_RAM_SIZE                         (0x00000400)
#define VIDEO_RAM_BASE_LOG                       (0x06000000)
#define VIDEO_RAM_SIZE                           (0x00018000)
#define OBJ_ATTR_RAM_BASE_LOG                    (0x07000000)
#define OBJ_ATTR_RAM_SIZE                        (0x00000400)
//external memory(game pak)
#define CARTRIDGE_ROM_WAIT_STATE_0_BASE_LOG      (0x08000000)
#define CARTRIDGE_ROM_WAIT_STATE_0_SIZE          (0x01000000)


#define BIOS_BASE_PHY                            (0x00000000)
#define ON_BOARD_WRAM_BASE_PHY                   (BIOS_BASE_PHY          + BIOS_SIZE)
#define ON_CHIP_WRAM_BASE_PHY                    (ON_BOARD_WRAM_BASE_PHY + ON_BOARD_WRAM_SIZE)
#define IO_REGISTER_BASE_PHY                     (ON_CHIP_WRAM_BASE_PHY  + ON_CHIP_WRAM_SIZE)
#define PALETTE_RAM_BASE_PHY                     (IO_REGISTER_BASE_PHY   + IO_REGISTER_SIZE)
#define VIDEO_RAM_BASE_PHY                       (PALETTE_RAM_BASE_PHY   + PALETTE_RAM_SIZE)
#define OBJ_ATTR_RAM_BASE_PHY                    (VIDEO_RAM_BASE_PHY     + VIDEO_RAM_SIZE)
#define CARTRIDGE_ROM_WAIT_STATE_0_BASE_PHY      (OBJ_ATTR_RAM_BASE_PHY  + OBJ_ATTR_RAM_SIZE)


#define ALLOCATED_MEMORY_SIZE                (BIOS_SIZE + ON_BOARD_WRAM_SIZE + ON_CHIP_WRAM_SIZE + IO_REGISTER_SIZE + PALETTE_RAM_SIZE + VIDEO_RAM_SIZE + OBJ_ATTR_RAM_SIZE + CARTRIDGE_ROM_WAIT_STATE_0_SIZE)

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
    U32 joybus_entry_point;
}ROM_HEADER;

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

typedef struct instruction_format 
{
    //Data Processing/PSR Transfer
    union
    {
        U32 operand2 : 12;
        U32 Rd       : 4;
        U32 Rm       : 4;
        U32 S        : 1;
        U32 opc      : 4;
        U32 I        : 1;
        U32 rsv      : 2;
        U32 cond     : 4;
    }data_proc;

    //Multiply
    union
    {
        U32 Rm   : 4;
        U32 rsv0  : 4;
        U32 Rs   : 4;
        U32 Rn   : 4;
        U32 Rd   : 4;
        U32 S    : 1;
        U32 A    : 1;
        U32 rsv1  : 6;
        U32 cond : 4;
    }multply;

    //Multiply Long
    union
    {
        U32 Rm   : 4;
        U32 rsv0  : 4;
        U32 Rn   : 4;
        U32 RdLo : 4;
        U32 RdHi : 4;
        U32 S    : 1;
        U32 A    : 1;
        U32 U    : 1;
        U32 rsv1  : 5;
        U32 cond : 4;
    }multply_long;

    //Single Data Swap
    union
    {
        U32 Rm   : 4;
        U32 rsv1 : 8;
        U32 Rd   : 4;
        U32 Rn   : 4;
        U32 rsv2 : 2;
        U32 B    : 1;
        U32 rsv  : 5;
        U32 cond : 4;
    }single_data_swap;
       
    //Branch and Exchange
    union
    {
        U32 Rn   : 4;
        U32 rsv  : 24;
        U32 cond : 4;
    }branch_exchange;

    //Halfword Data Transfer : register offset
    union
    {
        U32 Rm   : 4;
        U32 rsv1 : 1;
        U32 H    : 1;
        U32 S    : 1;
        U32 rsv2 : 5;
        U32 Rd   : 4;
        U32 Rn   : 4;
        U32 L    : 1;
        U32 W    : 1;
        U32 rsv3 : 1;
        U32 U    : 1;
        U32 P    : 1;
        U32 rsv4 : 3;
        U32 cond : 4;
    }hlf_data_tsf_reg_ofst;

    //Halfword Data Transfer : immediate offset
    union
    {
        U32 offset0 : 4;
        U32 rsv1    : 1;
        U32 H       : 1;
        U32 S       : 1;
        U32 rsv2    : 1;
        U32 offset1 : 4;
        U32 Rd      : 4;
        U32 Rn      : 4;
        U32 L       : 1;
        U32 W       : 1;
        U32 rsv3    : 1;
        U32 U       : 1;
        U32 P       : 1;
        U32 rsv4    : 3;
        U32 cond    : 4;
    }hlf_data_tsf_imm_ofst;

    //Single Data Transfer
    union
    {
        U32 offset : 12;
        U32 Rd     : 4;
        U32 Rn     : 4;
        U32 L      : 1;
        U32 W      : 1;
        U32 B      : 1;
        U32 U      : 1;
        U32 P      : 1;
        U32 rsv    : 3;
        U32 cond   : 4;
    }single_data_tsf;

    //Undefined
    union
    {
        U32 rsv1 : 28;
        U32 cond : 4;
    }undefined;

    //Block Data Transfer
    union
    {
        U32 reg_list : 16;
        U32 Rn       : 4;
        U32 L        : 1;
        U32 W        : 1;
        U32 B        : 1;
        U32 U        : 1;
        U32 P        : 1;
        U32 rsv      : 3;
        U32 cond     : 4;
    }blk_data_tsf;

    //Branch
    union
    {
        U32 offset : 24;
        U32 L      : 1;
        U32 rsv    : 3;
        U32 cond   : 4;
    }branch;

    //Coprocessor Data Transfer
    union
    {
        U32 offset : 8;
        U32 CPn    : 4;
        U32 CRd    : 4;
        U32 Rn     : 4;
        U32 L      : 1;
        U32 W      : 1;
        U32 N      : 1;
        U32 U      : 1;
        U32 P      : 1;
        U32 rsv    : 3;
        U32 cond   : 4;
    }cop_data_tfr;

    //Coprocessor Data Operation
    union
    {
        U32 CRm    : 4;
        U32 rsv0   : 1;
        U32 CP     : 3;
        U32 CPn    : 4;
        U32 CPd    : 4;
        U32 CRn    : 4;
        U32 CP_opc : 4;
        U32 rsv1   : 4;
        U32 cond   : 4;

    }cop_data_op;

    //Coprocessor Register Transfer
    union
    {
        U32 CRm    : 4;
        U32 rsv0   : 1;
        U32 CP     : 3;
        U32 CPn    : 4;
        U32 CPd    : 4;
        U32 CRn    : 4;
        U32 rsv1   : 1;
        U32 CP_opc : 3;
        U32 rsv2   : 4;
        U32 cond   : 4;
    }cop_reg_tfr;


    //Software Interrrupt
    union
    {
        U32 rsv  : 28;
        U32 cond : 4;
    }sw_int;


}INSTRUCTION_FORMAT;

#pragma pack()



class MEMORY 
{
public:
    U8 raw_data[ALLOCATED_MEMORY_SIZE];
    U8 operator[](U32 idx) 
    {
        //translate index
        switch (idx & 0x0F000000)
        {
            case BIOS_BASE_LOG:
                return raw_data[idx & 0x00FFFFFF + BIOS_BASE_PHY];
                break;
            case ON_BOARD_WRAM_BASE_LOG:
                return raw_data[idx & 0x00FFFFFF + ON_BOARD_WRAM_BASE_PHY];
                break;
            case ON_CHIP_WRAM_BASE_LOG:
                return raw_data[idx & 0x00FFFFFF + ON_CHIP_WRAM_BASE_PHY];
                break;
            case IO_REGISTER_BASE_LOG:
                return raw_data[idx & 0x00FFFFFF + IO_REGISTER_BASE_PHY];
                break;
            case PALETTE_RAM_BASE_LOG:
                return raw_data[idx & 0x00FFFFFF + PALETTE_RAM_BASE_PHY];
                break;
            case VIDEO_RAM_BASE_LOG:
                return raw_data[idx & 0x00FFFFFF + VIDEO_RAM_BASE_PHY];
                break;
            case OBJ_ATTR_RAM_BASE_LOG:
                return raw_data[idx & 0x00FFFFFF + OBJ_ATTR_RAM_BASE_PHY];
                break;
            case CARTRIDGE_ROM_WAIT_STATE_0_BASE_LOG:
                return raw_data[idx & 0x00FFFFFF + CARTRIDGE_ROM_WAIT_STATE_0_BASE_PHY];
                break;
            default:
                printf("unknown idx : %08X\n", idx);
                return 0x00;
                break;
        }
    }
};


class GBA_EMUALTOR_ARM7TDMI
{
public:
    //cpu part
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

    MEMORY   memory;
    void readROM(std::string filename) 
    {
        std::ifstream fin;
        fin.open(filename.c_str(), std::ios::binary | std::ios::in);
        fin.read((char*)&memory.raw_data[CARTRIDGE_ROM_WAIT_STATE_0_BASE_PHY], CARTRIDGE_ROM_WAIT_STATE_0_SIZE);
        fin.close();
    }
    void run() 
    {
        U8  bit_27_20 = memory[R[15]] & 0x0FF00000;
    }
};