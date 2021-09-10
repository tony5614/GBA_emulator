#pragma once

#include <fstream>
#include <stdio.h>


typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned int   U32;
typedef          char  S8;
typedef          short S16;
typedef          int   S32;


#define BIT(n)         (1 << n)
#define BIT_MASK(e,s)  (((((0xFFFFFFFF << (31 - (e))) >> (31 - (e)))) >> (s)) << (s))

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

#define NUM_OF_REGISTER     (16)
#define USR_MODE            (0x10)    // 10000b
#define FIQ_MODE            (0x11)    // 10001b
#define IRQ_MODE            (0x12)    // 10010b
#define SVC_MODE            (0x13)    // 10011b
#define ABT_MODE            (0x17)    // 10111b
#define UND_MODE            (0x1B)    // 11011b
#define SYS_MODE            (0x1F)    // 11111b




#pragma pack(1)



typedef struct instruction_format
{
    //Data Processing/PSR Transfer
    union
    {
		struct
		{
            //operand2 is a register
			union 
			{
				U32 Rm : 4;

                struct // : 8         //apply to Rm
                {
                    union
                    {
                        U32 register_or_amount : 1;         // = 0
                        U32 shift_type : 2;
                        U32 shift_amount : 5;  //5 bit unsigned integer
                    };
                    union
                    {
                        U32 register_or_amount : 1;          // = 1
                        U32 shift_type : 2;
                        U32 rsv1 : 1;
                        U32 shift_reg : 4;     //shift amount specified in botom byte of Rs
                    };
                }shift;							
			};

            //operand2 is an immediate value
			union 
			{
				U32 imm : 8;
				U32 rotate : 4;				
			};
		}operand2;
		
        U32 Rd : 4;
        U32 Rn : 4;
        U32 S : 1;
        U32 opc : 4;
        U32 I : 1;
        U32 rsv : 2;
        U32 cond : 4;
    }data_proc;

    //Multiply
    union
    {
        U32 Rm : 4;
        U32 rsv0 : 4;
        U32 Rs : 4;
        U32 Rn : 4;
        U32 Rd : 4;
        U32 S : 1;
        U32 A : 1;
        U32 rsv1 : 6;
        U32 cond : 4;
    }multply;

    //Multiply Long
    union
    {
        U32 Rm : 4;
        U32 rsv0 : 4;
        U32 Rn : 4;
        U32 RdLo : 4;
        U32 RdHi : 4;
        U32 S : 1;
        U32 A : 1;
        U32 U : 1;
        U32 rsv1 : 5;
        U32 cond : 4;
    }multply_long;

    //Single Data Swap
    union
    {
        U32 Rm : 4;
        U32 rsv1 : 8;
        U32 Rd : 4;
        U32 Rn : 4;
        U32 rsv2 : 2;
        U32 B : 1;
        U32 rsv : 5;
        U32 cond : 4;
    }single_data_swap;

    //Branch and Exchange
    union
    {
        U32 Rn : 4;
        U32 rsv : 24;
        U32 cond : 4;
    }branch_exchange;

    //Halfword Data Transfer : register offset
    union
    {
        U32 Rm : 4;
        U32 rsv1 : 1;
        U32 H : 1;
        U32 S : 1;
        U32 rsv2 : 5;
        U32 Rd : 4;
        U32 Rn : 4;
        U32 L : 1;
        U32 W : 1;
        U32 rsv3 : 1;
        U32 U : 1;
        U32 P : 1;
        U32 rsv4 : 3;
        U32 cond : 4;
    }hlf_data_tsf_reg_ofst;

    //Halfword Data Transfer : immediate offset
    union
    {
        U32 offset0 : 4;
        U32 rsv1 : 1;
        U32 H : 1;
        U32 S : 1;
        U32 rsv2 : 1;
        U32 offset1 : 4;
        U32 Rd : 4;
        U32 Rn : 4;
        U32 L : 1;
        U32 W : 1;
        U32 rsv3 : 1;
        U32 U : 1;
        U32 P : 1;
        U32 rsv4 : 3;
        U32 cond : 4;
    }hlf_data_tsf_imm_ofst;

    //Single Data Transfer
    union
    {
        U32 offset : 12;
        U32 Rd : 4;
        U32 Rn : 4;
        U32 L : 1;
        U32 W : 1;
        U32 B : 1;
        U32 U : 1;
        U32 P : 1;
        U32 rsv : 3;
        U32 cond : 4;
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
        U32 Rn : 4;
        U32 L : 1;
        U32 W : 1;
        U32 B : 1;
        U32 U : 1;
        U32 P : 1;
        U32 rsv : 3;
        U32 cond : 4;
    }blk_data_tsf;

    //Branch
    union
    {
        U32 offset : 24;
        U32 L : 1;
        U32 rsv : 3;
        U32 cond : 4;
    }branch;

    //Coprocessor Data Transfer
    union
    {
        U32 offset : 8;
        U32 CPn : 4;
        U32 CRd : 4;
        U32 Rn : 4;
        U32 L : 1;
        U32 W : 1;
        U32 N : 1;
        U32 U : 1;
        U32 P : 1;
        U32 rsv : 3;
        U32 cond : 4;
    }cop_data_tfr;

    //Coprocessor Data Operation
    union
    {
        U32 CRm : 4;
        U32 rsv0 : 1;
        U32 CP : 3;
        U32 CPn : 4;
        U32 CPd : 4;
        U32 CRn : 4;
        U32 CP_opc : 4;
        U32 rsv1 : 4;
        U32 cond : 4;

    }cop_data_op;

    //Coprocessor Register Transfer
    union
    {
        U32 CRm : 4;
        U32 rsv0 : 1;
        U32 CP : 3;
        U32 CPn : 4;
        U32 CPd : 4;
        U32 CRn : 4;
        U32 rsv1 : 1;
        U32 CP_opc : 3;
        U32 rsv2 : 4;
        U32 cond : 4;
    }cop_reg_tfr;

    //PSR Transfer
    union
    {
        union 
        {
            struct 
            {
                union 
                {
                    struct
                    {
                        U32 Rm : 4;
                        U32 rsv0 : 8;
                    };
                    struct
                    {
                        U32 imm : 8;
                        U32 rotate : 4; //chaper 4.5.3
                    };
                };
            }source_operand;
        };
        
        U32 Rd : 4;
        U32 rsv1 : 6;
        union 
        {
            U32 Ps : 1;
            U32 Pd : 1;
        };
        U32 rsv2 : 2;
        U32 I : 1;
        U32 cond : 4;
    }psr_tsfr;

    //Software Interrupt
    union
    {
        U32 rsv : 28;
        U32 cond : 4;
    }sw_int;


}INSTRUCTION_FORMAT;




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
    union
    {
        struct 
        {
            U32 mode : 5;
            U32 T : 1;     //reflect the processor is executing in THUMB or ARM state, the software must never change the state of this bit
            U32 F : 1;     //disable FIQ
            U32 I : 1;     //disable IRQ
            U32 rsv : 20;
            U32 V : 1;
            U32 C : 1;
            U32 Z : 1;
            U32 N : 1;
        };
        U32 val;
    };
}CPSR, SPSR;

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
    //CPU part
    //ARM state general register and program counter
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

    U8 mode;

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
		U32 pc = R[15];
        U8  bit_27_20 = memory[pc] & 0x0FF00000;
    }
	
	


    //----------------------//
    //-- opcode functions --//
    //----------------------//	
    U32 get_shifted_operand2(INSTRUCTION_FORMAT *);
    
    void AND(INSTRUCTION_FORMAT*);


    void AND_lli(INSTRUCTION_FORMAT*);
    void AND_llr(INSTRUCTION_FORMAT*);
    void AND_lri(INSTRUCTION_FORMAT*);
    void AND_lrr(INSTRUCTION_FORMAT*);
    void AND_ari(INSTRUCTION_FORMAT*);
    void AND_arr(INSTRUCTION_FORMAT*);
    void AND_rri(INSTRUCTION_FORMAT*);
    void AND_rrr(INSTRUCTION_FORMAT*);
    
    void ANDS_lli(INSTRUCTION_FORMAT*);
    void ANDS_llr(INSTRUCTION_FORMAT*);
    void ANDS_lri(INSTRUCTION_FORMAT*);
    void ANDS_lrr(INSTRUCTION_FORMAT*);
    void ANDS_ari(INSTRUCTION_FORMAT*);
    void ANDS_arr(INSTRUCTION_FORMAT*);
    void ANDS_rri(INSTRUCTION_FORMAT*);
    void ANDS_rrr(INSTRUCTION_FORMAT*);
    
    //exclusive OR
    void EOR_lli(INSTRUCTION_FORMAT*);
    void EOR_llr(INSTRUCTION_FORMAT*);
    void EOR_lri(INSTRUCTION_FORMAT*);
    void EOR_lrr(INSTRUCTION_FORMAT*);
    void EOR_ari(INSTRUCTION_FORMAT*);
    void EOR_arr(INSTRUCTION_FORMAT*);
    void EOR_rri(INSTRUCTION_FORMAT*);
    void EOR_rrr(INSTRUCTION_FORMAT*);
    
    void EORS_lli(INSTRUCTION_FORMAT*);
    void EORS_llr(INSTRUCTION_FORMAT*);
    void EORS_lri(INSTRUCTION_FORMAT*);
    void EORS_lrr(INSTRUCTION_FORMAT*);
    void EORS_ari(INSTRUCTION_FORMAT*);
    void EORS_arr(INSTRUCTION_FORMAT*);
    void EORS_rri(INSTRUCTION_FORMAT*);
    void EORS_rrr(INSTRUCTION_FORMAT*);
    
    void SUB_lli(INSTRUCTION_FORMAT*);
    void SUB_llr(INSTRUCTION_FORMAT*);
    void SUB_lri(INSTRUCTION_FORMAT*);
    void SUB_lrr(INSTRUCTION_FORMAT*);
    void SUB_ari(INSTRUCTION_FORMAT*);
    void SUB_arr(INSTRUCTION_FORMAT*);
    void SUB_rri(INSTRUCTION_FORMAT*);
    void SUB_rrr(INSTRUCTION_FORMAT*);
    
    void SUBS_lli(INSTRUCTION_FORMAT*);
    void SUBS_llr(INSTRUCTION_FORMAT*);
    void SUBS_lri(INSTRUCTION_FORMAT*);
    void SUBS_lrr(INSTRUCTION_FORMAT*);
    void SUBS_ari(INSTRUCTION_FORMAT*);
    void SUBS_arr(INSTRUCTION_FORMAT*);
    void SUBS_rri(INSTRUCTION_FORMAT*);
    void SUBS_rrr(INSTRUCTION_FORMAT*);
    
    void RSB_lli(INSTRUCTION_FORMAT*);
    void RSB_llr(INSTRUCTION_FORMAT*);
    void RSB_lri(INSTRUCTION_FORMAT*);
    void RSB_lrr(INSTRUCTION_FORMAT*);
    void RSB_ari(INSTRUCTION_FORMAT*);
    void RSB_arr(INSTRUCTION_FORMAT*);
    void RSB_rri(INSTRUCTION_FORMAT*);
    void RSB_rrr(INSTRUCTION_FORMAT*);
    
    void RSBS_lli(INSTRUCTION_FORMAT*);
    void RSBS_llr(INSTRUCTION_FORMAT*);
    void RSBS_lri(INSTRUCTION_FORMAT*);
    void RSBS_lrr(INSTRUCTION_FORMAT*);
    void RSBS_ari(INSTRUCTION_FORMAT*);
    void RSBS_arr(INSTRUCTION_FORMAT*);
    void RSBS_rri(INSTRUCTION_FORMAT*);
    void RSBS_rrr(INSTRUCTION_FORMAT*);
    
    void ADD_lli(INSTRUCTION_FORMAT*);
    void ADD_llr(INSTRUCTION_FORMAT*);
    void ADD_lri(INSTRUCTION_FORMAT*);
    void ADD_lrr(INSTRUCTION_FORMAT*);
    void ADD_ari(INSTRUCTION_FORMAT*);
    void ADD_arr(INSTRUCTION_FORMAT*);
    void ADD_rri(INSTRUCTION_FORMAT*);
    void ADD_rrr(INSTRUCTION_FORMAT*);
    
    void ADDS_lli(INSTRUCTION_FORMAT*);
    void ADDS_llr(INSTRUCTION_FORMAT*);
    void ADDS_lri(INSTRUCTION_FORMAT*);
    void ADDS_lrr(INSTRUCTION_FORMAT*);
    void ADDS_ari(INSTRUCTION_FORMAT*);
    void ADDS_arr(INSTRUCTION_FORMAT*);
    void ADDS_rri(INSTRUCTION_FORMAT*);
    void ADDS_rrr(INSTRUCTION_FORMAT*);
    
    void ADC_lli(INSTRUCTION_FORMAT*);
    void ADC_llr(INSTRUCTION_FORMAT*);
    void ADC_lri(INSTRUCTION_FORMAT*);
    void ADC_lrr(INSTRUCTION_FORMAT*);
    void ADC_ari(INSTRUCTION_FORMAT*);
    void ADC_arr(INSTRUCTION_FORMAT*);
    void ADC_rri(INSTRUCTION_FORMAT*);
    void ADC_rrr(INSTRUCTION_FORMAT*);
    
    void ADCS_lli(INSTRUCTION_FORMAT*);
    void ADCS_llr(INSTRUCTION_FORMAT*);
    void ADCS_lri(INSTRUCTION_FORMAT*);
    void ADCS_lrr(INSTRUCTION_FORMAT*);
    void ADCS_ari(INSTRUCTION_FORMAT*);
    void ADCS_arr(INSTRUCTION_FORMAT*);
    void ADCS_rri(INSTRUCTION_FORMAT*);
    void ADCS_rrr(INSTRUCTION_FORMAT*);
    
    void SBC_lli(INSTRUCTION_FORMAT*);
    void SBC_llr(INSTRUCTION_FORMAT*);
    void SBC_lri(INSTRUCTION_FORMAT*);
    void SBC_lrr(INSTRUCTION_FORMAT*);
    void SBC_ari(INSTRUCTION_FORMAT*);
    void SBC_arr(INSTRUCTION_FORMAT*);
    void SBC_rri(INSTRUCTION_FORMAT*);
    void SBC_rrr(INSTRUCTION_FORMAT*);
    
    void SBCS_lli(INSTRUCTION_FORMAT*);
    void SBCS_llr(INSTRUCTION_FORMAT*);
    void SBCS_lri(INSTRUCTION_FORMAT*);
    void SBCS_lrr(INSTRUCTION_FORMAT*);
    void SBCS_ari(INSTRUCTION_FORMAT*);
    void SBCS_arr(INSTRUCTION_FORMAT*);
    void SBCS_rri(INSTRUCTION_FORMAT*);
    void SBCS_rrr(INSTRUCTION_FORMAT*);
    
    void RSC_lli(INSTRUCTION_FORMAT*);
    void RSC_llr(INSTRUCTION_FORMAT*);
    void RSC_lri(INSTRUCTION_FORMAT*);
    void RSC_lrr(INSTRUCTION_FORMAT*);
    void RSC_ari(INSTRUCTION_FORMAT*);
    void RSC_arr(INSTRUCTION_FORMAT*);
    void RSC_rri(INSTRUCTION_FORMAT*);
    void RSC_rrr(INSTRUCTION_FORMAT*);
    
    void RSCS_lli(INSTRUCTION_FORMAT*);
    void RSCS_llr(INSTRUCTION_FORMAT*);
    void RSCS_lri(INSTRUCTION_FORMAT*);
    void RSCS_lrr(INSTRUCTION_FORMAT*);
    void RSCS_ari(INSTRUCTION_FORMAT*);
    void RSCS_arr(INSTRUCTION_FORMAT*);
    void RSCS_rri(INSTRUCTION_FORMAT*);
    void RSCS_rrr(INSTRUCTION_FORMAT*);
    
    void TSTS_lli(INSTRUCTION_FORMAT*);
    void TSTS_llr(INSTRUCTION_FORMAT*);
    void TSTS_lri(INSTRUCTION_FORMAT*);
    void TSTS_lrr(INSTRUCTION_FORMAT*);
    void TSTS_ari(INSTRUCTION_FORMAT*);
    void TSTS_arr(INSTRUCTION_FORMAT*);
    void TSTS_rri(INSTRUCTION_FORMAT*);
    void TSTS_rrr(INSTRUCTION_FORMAT*);
    
    void TEQS_lli(INSTRUCTION_FORMAT*);
    void TEQS_llr(INSTRUCTION_FORMAT*);
    void TEQS_lri(INSTRUCTION_FORMAT*);
    void TEQS_lrr(INSTRUCTION_FORMAT*);
    void TEQS_ari(INSTRUCTION_FORMAT*);
    void TEQS_arr(INSTRUCTION_FORMAT*);
    void TEQS_rri(INSTRUCTION_FORMAT*);
    void TEQS_rrr(INSTRUCTION_FORMAT*);
    
    void CMPS_lli(INSTRUCTION_FORMAT*);
    void CMPS_llr(INSTRUCTION_FORMAT*);
    void CMPS_lri(INSTRUCTION_FORMAT*);
    void CMPS_lrr(INSTRUCTION_FORMAT*);
    void CMPS_ari(INSTRUCTION_FORMAT*);
    void CMPS_arr(INSTRUCTION_FORMAT*);
    void CMPS_rri(INSTRUCTION_FORMAT*);
    void CMPS_rrr(INSTRUCTION_FORMAT*);
    
    void CMNS_lli(INSTRUCTION_FORMAT*);
    void CMNS_llr(INSTRUCTION_FORMAT*);
    void CMNS_lri(INSTRUCTION_FORMAT*);
    void CMNS_lrr(INSTRUCTION_FORMAT*);
    void CMNS_ari(INSTRUCTION_FORMAT*);
    void CMNS_arr(INSTRUCTION_FORMAT*);
    void CMNS_rri(INSTRUCTION_FORMAT*);
    void CMNS_rrr(INSTRUCTION_FORMAT*);
    
    void ORR_lli(INSTRUCTION_FORMAT*);
    void ORR_llr(INSTRUCTION_FORMAT*);
    void ORR_lri(INSTRUCTION_FORMAT*);
    void ORR_lrr(INSTRUCTION_FORMAT*);
    void ORR_ari(INSTRUCTION_FORMAT*);
    void ORR_arr(INSTRUCTION_FORMAT*);
    void ORR_rri(INSTRUCTION_FORMAT*);
    void ORR_rrr(INSTRUCTION_FORMAT*);
    
    void ORRS_lli(INSTRUCTION_FORMAT*);
    void ORRS_llr(INSTRUCTION_FORMAT*);
    void ORRS_lri(INSTRUCTION_FORMAT*);
    void ORRS_lrr(INSTRUCTION_FORMAT*);
    void ORRS_ari(INSTRUCTION_FORMAT*);
    void ORRS_arr(INSTRUCTION_FORMAT*);
    void ORRS_rri(INSTRUCTION_FORMAT*);
    void ORRS_rrr(INSTRUCTION_FORMAT*);
    
    void MOV_lli(INSTRUCTION_FORMAT*);
    void MOV_llr(INSTRUCTION_FORMAT*);
    void MOV_lri(INSTRUCTION_FORMAT*);
    void MOV_lrr(INSTRUCTION_FORMAT*);
    void MOV_ari(INSTRUCTION_FORMAT*);
    void MOV_arr(INSTRUCTION_FORMAT*);
    void MOV_rri(INSTRUCTION_FORMAT*);
    void MOV_rrr(INSTRUCTION_FORMAT*);
    
    void MOVS_lli(INSTRUCTION_FORMAT*);
    void MOVS_llr(INSTRUCTION_FORMAT*);
    void MOVS_lri(INSTRUCTION_FORMAT*);
    void MOVS_lrr(INSTRUCTION_FORMAT*);
    void MOVS_ari(INSTRUCTION_FORMAT*);
    void MOVS_arr(INSTRUCTION_FORMAT*);
    void MOVS_rri(INSTRUCTION_FORMAT*);
    void MOVS_rrr(INSTRUCTION_FORMAT*);
    
    void BIC_lli(INSTRUCTION_FORMAT*);
    void BIC_llr(INSTRUCTION_FORMAT*);
    void BIC_lri(INSTRUCTION_FORMAT*);
    void BIC_lrr(INSTRUCTION_FORMAT*);
    void BIC_ari(INSTRUCTION_FORMAT*);
    void BIC_arr(INSTRUCTION_FORMAT*);
    void BIC_rri(INSTRUCTION_FORMAT*);
    void BIC_rrr(INSTRUCTION_FORMAT*);
    
    void BICS_lli(INSTRUCTION_FORMAT*);
    void BICS_llr(INSTRUCTION_FORMAT*);
    void BICS_lri(INSTRUCTION_FORMAT*);
    void BICS_lrr(INSTRUCTION_FORMAT*);
    void BICS_ari(INSTRUCTION_FORMAT*);
    void BICS_arr(INSTRUCTION_FORMAT*);
    void BICS_rri(INSTRUCTION_FORMAT*);
    void BICS_rrr(INSTRUCTION_FORMAT*);
    
    void MVN_lli(INSTRUCTION_FORMAT*);
    void MVN_llr(INSTRUCTION_FORMAT*);
    void MVN_lri(INSTRUCTION_FORMAT*);
    void MVN_lrr(INSTRUCTION_FORMAT*);
    void MVN_ari(INSTRUCTION_FORMAT*);
    void MVN_arr(INSTRUCTION_FORMAT*);
    void MVN_rri(INSTRUCTION_FORMAT*);
    void MVN_rrr(INSTRUCTION_FORMAT*);

    void MVNS_lli(INSTRUCTION_FORMAT*);
    void MVNS_llr(INSTRUCTION_FORMAT*);
    void MVNS_lri(INSTRUCTION_FORMAT*);
    void MVNS_lrr(INSTRUCTION_FORMAT*);
    void MVNS_ari(INSTRUCTION_FORMAT*);
    void MVNS_arr(INSTRUCTION_FORMAT*);
    void MVNS_rri(INSTRUCTION_FORMAT*);
    void MVNS_rrr(INSTRUCTION_FORMAT*);
    
    void AND_imm (INSTRUCTION_FORMAT*);	
    void ANDS_imm (INSTRUCTION_FORMAT*);
    void EOR_imm (INSTRUCTION_FORMAT*);
    void EORS_imm (INSTRUCTION_FORMAT*);
    void SUB_imm (INSTRUCTION_FORMAT*);
    void SUBS_imm (INSTRUCTION_FORMAT*);
    void RSB_imm (INSTRUCTION_FORMAT*);
    void RSBS_imm (INSTRUCTION_FORMAT*);
    void ADD_imm (INSTRUCTION_FORMAT*);
    void ADDS_imm (INSTRUCTION_FORMAT*);
    void ADC_imm (INSTRUCTION_FORMAT*);
    void ADCS_imm (INSTRUCTION_FORMAT*);
    void SBC_imm (INSTRUCTION_FORMAT*);
    void SBCS_imm (INSTRUCTION_FORMAT*);
    void RSC_imm (INSTRUCTION_FORMAT*);
    void RSCS_imm (INSTRUCTION_FORMAT*);
    void TSTS_imm (INSTRUCTION_FORMAT*);
    void MSR_ic (INSTRUCTION_FORMAT*);
    void TEQS_imm (INSTRUCTION_FORMAT*);
    void CMPS_imm (INSTRUCTION_FORMAT*);
    void MSR_is (INSTRUCTION_FORMAT*);
    void CMNS_imm (INSTRUCTION_FORMAT*);
    void ORR_imm (INSTRUCTION_FORMAT*);
    void ORRS_imm (INSTRUCTION_FORMAT*);
    void MOV_imm (INSTRUCTION_FORMAT*);
    void MOVS_imm (INSTRUCTION_FORMAT*);
    void BIC_imm (INSTRUCTION_FORMAT*);
    void BICS_imm (INSTRUCTION_FORMAT*);
    void MVN_imm (INSTRUCTION_FORMAT*);
    void MVNS_imm (INSTRUCTION_FORMAT*);
    void STR_ptim (INSTRUCTION_FORMAT*);
    void LDR_ptim (INSTRUCTION_FORMAT*);
    void STRT_ptim (INSTRUCTION_FORMAT*);
    void LDRT_ptim (INSTRUCTION_FORMAT*);
    void STRB_ptim (INSTRUCTION_FORMAT*);
    void LDRB_ptim (INSTRUCTION_FORMAT*);
    void STRBT_ptim (INSTRUCTION_FORMAT*);
    void LDRBT_ptim (INSTRUCTION_FORMAT*);
    void STR_ptip (INSTRUCTION_FORMAT*);
    void LDR_ptip (INSTRUCTION_FORMAT*);
    void STRT_ptip (INSTRUCTION_FORMAT*);
    void LDRT_ptip (INSTRUCTION_FORMAT*);
    void STRB_ptip (INSTRUCTION_FORMAT*);
    void LDRB_ptip (INSTRUCTION_FORMAT*);
    void STRBT_ptip (INSTRUCTION_FORMAT*);
    void LDRBT_ptip (INSTRUCTION_FORMAT*);
    void STR_ofim (INSTRUCTION_FORMAT*);
    void LDR_ofim (INSTRUCTION_FORMAT*);
    void STR_prim (INSTRUCTION_FORMAT*);
    void LDR_prim (INSTRUCTION_FORMAT*);
    void STRB_ofim (INSTRUCTION_FORMAT*);
    void LDRB_ofim (INSTRUCTION_FORMAT*);
    void STRB_prim (INSTRUCTION_FORMAT*);
    void LDRB_prim (INSTRUCTION_FORMAT*);
    void STR_ofip (INSTRUCTION_FORMAT*);
    void LDR_ofip (INSTRUCTION_FORMAT*);
    void STR_prip (INSTRUCTION_FORMAT*);
    void LDR_prip (INSTRUCTION_FORMAT*);
    void STRB_ofip (INSTRUCTION_FORMAT*);
    void LDRB_ofip (INSTRUCTION_FORMAT*);
    void STRB_prip (INSTRUCTION_FORMAT*);
    void LDRB_prip (INSTRUCTION_FORMAT*);
    
    void STR_ptrmll (INSTRUCTION_FORMAT*); 
    void STR_ptrmlr (INSTRUCTION_FORMAT*); 
    void STR_ptrmar (INSTRUCTION_FORMAT*); 
    void STR_ptrmrr (INSTRUCTION_FORMAT*); 
    
    void LDR_ptrmll (INSTRUCTION_FORMAT*); 
    void LDR_ptrmlr (INSTRUCTION_FORMAT*); 
    void LDR_ptrmar (INSTRUCTION_FORMAT*); 
    void LDR_ptrmrr (INSTRUCTION_FORMAT*); 
    
    void STRT_ptrmll (INSTRUCTION_FORMAT*); 
    void STRT_ptrmlr (INSTRUCTION_FORMAT*); 
    void STRT_ptrmar (INSTRUCTION_FORMAT*); 
    void STRT_ptrmrr (INSTRUCTION_FORMAT*); 
    
    void LDRT_ptrmll (INSTRUCTION_FORMAT*); 
    void LDRT_ptrmlr (INSTRUCTION_FORMAT*); 
    void LDRT_ptrmar (INSTRUCTION_FORMAT*); 
    void LDRT_ptrmrr (INSTRUCTION_FORMAT*); 
    
    void STRB_ptrmll (INSTRUCTION_FORMAT*); 
    void STRB_ptrmlr (INSTRUCTION_FORMAT*); 
    void STRB_ptrmar (INSTRUCTION_FORMAT*); 
    void STRB_ptrmrr (INSTRUCTION_FORMAT*); 
    
    void LDRB_ptrmll (INSTRUCTION_FORMAT*); 
    void LDRB_ptrmlr (INSTRUCTION_FORMAT*); 
    void LDRB_ptrmar (INSTRUCTION_FORMAT*); 
    void LDRB_ptrmrr (INSTRUCTION_FORMAT*); 
    
    void STRBT_ptrmll (INSTRUCTION_FORMAT*); 
    void STRBT_ptrmlr (INSTRUCTION_FORMAT*); 
    void STRBT_ptrmar (INSTRUCTION_FORMAT*); 
    void STRBT_ptrmrr (INSTRUCTION_FORMAT*); 
              
    void LDRBT_ptrmll (INSTRUCTION_FORMAT*); 
    void LDRBT_ptrmlr (INSTRUCTION_FORMAT*); 
    void LDRBT_ptrmar (INSTRUCTION_FORMAT*); 
    void LDRBT_ptrmrr (INSTRUCTION_FORMAT*); 
    
    void STR_ptrpll (INSTRUCTION_FORMAT*); 
    void STR_ptrplr (INSTRUCTION_FORMAT*); 
    void STR_ptrpar (INSTRUCTION_FORMAT*); 
    void STR_ptrprr (INSTRUCTION_FORMAT*); 
    
    void LDR_ptrpll (INSTRUCTION_FORMAT*); 
    void LDR_ptrplr (INSTRUCTION_FORMAT*); 
    void LDR_ptrpar (INSTRUCTION_FORMAT*); 
    void LDR_ptrprr (INSTRUCTION_FORMAT*); 
    
    void STRT_ptrpll (INSTRUCTION_FORMAT*); 
    void STRT_ptrplr (INSTRUCTION_FORMAT*); 
    void STRT_ptrpar (INSTRUCTION_FORMAT*); 
    void STRT_ptrprr (INSTRUCTION_FORMAT*); 
             
    void LDRT_ptrpll (INSTRUCTION_FORMAT*); 
    void LDRT_ptrplr (INSTRUCTION_FORMAT*); 
    void LDRT_ptrpar (INSTRUCTION_FORMAT*); 
    void LDRT_ptrprr (INSTRUCTION_FORMAT*); 
    
    void STRB_ptrpll (INSTRUCTION_FORMAT*); 
    void STRB_ptrplr (INSTRUCTION_FORMAT*); 
    void STRB_ptrpar (INSTRUCTION_FORMAT*); 
    void STRB_ptrprr (INSTRUCTION_FORMAT*); 
             
    void LDRB_ptrpll (INSTRUCTION_FORMAT*); 
    void LDRB_ptrplr (INSTRUCTION_FORMAT*); 
    void LDRB_ptrpar (INSTRUCTION_FORMAT*); 
    void LDRB_ptrprr (INSTRUCTION_FORMAT*); 
    
    void STRBT_ptrpll (INSTRUCTION_FORMAT*); 
    void STRBT_ptrplr (INSTRUCTION_FORMAT*); 
    void STRBT_ptrpar (INSTRUCTION_FORMAT*); 
    void STRBT_ptrprr (INSTRUCTION_FORMAT*); 
              
    void LDRBT_ptrpll (INSTRUCTION_FORMAT*); 
    void LDRBT_ptrplr (INSTRUCTION_FORMAT*); 
    void LDRBT_ptrpar (INSTRUCTION_FORMAT*); 
    void LDRBT_ptrprr (INSTRUCTION_FORMAT*); 
    
    void STR_ofrmll (INSTRUCTION_FORMAT*); 
    void STR_ofrmlr (INSTRUCTION_FORMAT*); 
    void STR_ofrmar (INSTRUCTION_FORMAT*); 
    void STR_ofrmrr (INSTRUCTION_FORMAT*); 
    
    void LDR_ofrmll (INSTRUCTION_FORMAT*); 
    void LDR_ofrmlr (INSTRUCTION_FORMAT*); 
    void LDR_ofrmar (INSTRUCTION_FORMAT*); 
    void LDR_ofrmrr (INSTRUCTION_FORMAT*); 
    
    void STR_prrmll (INSTRUCTION_FORMAT*); 
    void STR_prrmlr (INSTRUCTION_FORMAT*); 
    void STR_prrmar (INSTRUCTION_FORMAT*); 
    void STR_prrmrr (INSTRUCTION_FORMAT*); 
    
    void LDR_prrmll (INSTRUCTION_FORMAT*); 
    void LDR_prrmlr (INSTRUCTION_FORMAT*); 
    void LDR_prrmar (INSTRUCTION_FORMAT*); 
    void LDR_prrmrr (INSTRUCTION_FORMAT*); 
    
    void STRB_ofrmll (INSTRUCTION_FORMAT*); 
    void STRB_ofrmlr (INSTRUCTION_FORMAT*); 
    void STRB_ofrmar (INSTRUCTION_FORMAT*); 
    void STRB_ofrmrr (INSTRUCTION_FORMAT*); 
             
    void LDRB_ofrmll (INSTRUCTION_FORMAT*); 
    void LDRB_ofrmlr (INSTRUCTION_FORMAT*); 
    void LDRB_ofrmar (INSTRUCTION_FORMAT*); 
    void LDRB_ofrmrr (INSTRUCTION_FORMAT*); 
             
    void STRB_prrmll (INSTRUCTION_FORMAT*); 
    void STRB_prrmlr (INSTRUCTION_FORMAT*); 
    void STRB_prrmar (INSTRUCTION_FORMAT*); 
    void STRB_prrmrr (INSTRUCTION_FORMAT*); 
             
    void LDRB_prrmll (INSTRUCTION_FORMAT*); 
    void LDRB_prrmlr (INSTRUCTION_FORMAT*); 
    void LDRB_prrmar (INSTRUCTION_FORMAT*); 
    void LDRB_prrmrr (INSTRUCTION_FORMAT*); 
    
    void STR_ofrpll (INSTRUCTION_FORMAT*); 
    void STR_ofrplr (INSTRUCTION_FORMAT*); 
    void STR_ofrpar (INSTRUCTION_FORMAT*); 
    void STR_ofrprr (INSTRUCTION_FORMAT*); 
    
    void LDR_ofrpll (INSTRUCTION_FORMAT*); 
    void LDR_ofrplr (INSTRUCTION_FORMAT*); 
    void LDR_ofrpar (INSTRUCTION_FORMAT*);
    
    void STR_prrpll (INSTRUCTION_FORMAT*); 
    void STR_prrplr (INSTRUCTION_FORMAT*); 
    void STR_prrpar (INSTRUCTION_FORMAT*); 
    void STR_prrprr (INSTRUCTION_FORMAT*); 
    
    void LDR_prrpll (INSTRUCTION_FORMAT*); 
    void LDR_prrplr (INSTRUCTION_FORMAT*); 
    void LDR_prrpar (INSTRUCTION_FORMAT*); 
    void LDR_prrprr (INSTRUCTION_FORMAT*); 
    
    void STRB_ofrpll (INSTRUCTION_FORMAT*); 
    void STRB_ofrplr (INSTRUCTION_FORMAT*); 
    void STRB_ofrpar (INSTRUCTION_FORMAT*); 
    void STRB_ofrprr (INSTRUCTION_FORMAT*); 
             
    void LDRB_ofrpll (INSTRUCTION_FORMAT*); 
    void LDRB_ofrplr (INSTRUCTION_FORMAT*); 
    void LDRB_ofrpar (INSTRUCTION_FORMAT*); 
    void LDRB_ofrprr (INSTRUCTION_FORMAT*); 
             
    void STRB_prrpll (INSTRUCTION_FORMAT*); 
    void STRB_prrplr (INSTRUCTION_FORMAT*); 
    void STRB_prrpar (INSTRUCTION_FORMAT*); 
    void STRB_prrprr (INSTRUCTION_FORMAT*); 
             
    void LDRB_prrpll (INSTRUCTION_FORMAT*); 
    void LDRB_prrplr (INSTRUCTION_FORMAT*); 
    void LDRB_prrpar (INSTRUCTION_FORMAT*); 
    void LDRB_prrprr (INSTRUCTION_FORMAT*); 
    
    void STMDA(INSTRUCTION_FORMAT*);
    void LDMDA(INSTRUCTION_FORMAT*);
    void STMDA_w(INSTRUCTION_FORMAT*);
    void LDMDA_w(INSTRUCTION_FORMAT*);
    void STMDA_u(INSTRUCTION_FORMAT*);
    void LDMDA_u(INSTRUCTION_FORMAT*);
    void STMDA_uw(INSTRUCTION_FORMAT*);
    void LDMDA_uw(INSTRUCTION_FORMAT*);
    void STMIA(INSTRUCTION_FORMAT*);
    void LDMIA(INSTRUCTION_FORMAT*);
    void STMIA_w(INSTRUCTION_FORMAT*);
    void LDMIA_w(INSTRUCTION_FORMAT*);
    void STMIA_u(INSTRUCTION_FORMAT*);
    void LDMIA_u(INSTRUCTION_FORMAT*);
    void STMIA_uw(INSTRUCTION_FORMAT*);
    void LDMIA_uw(INSTRUCTION_FORMAT*);
    void STMDB(INSTRUCTION_FORMAT*);
    void LDMDB(INSTRUCTION_FORMAT*);
    void STMDB_w(INSTRUCTION_FORMAT*);
    void LDMDB_w(INSTRUCTION_FORMAT*);
    void STMDB_u(INSTRUCTION_FORMAT*);
    void LDMDB_u(INSTRUCTION_FORMAT*);
    void STMDB_uw(INSTRUCTION_FORMAT*);
    void LDMDB_uw(INSTRUCTION_FORMAT*);
    void STMIB(INSTRUCTION_FORMAT*);
    void LDMIB(INSTRUCTION_FORMAT*);
    void STMIB_w(INSTRUCTION_FORMAT*);
    void LDMIB_w(INSTRUCTION_FORMAT*);
    void STMIB_u(INSTRUCTION_FORMAT*);
    void LDMIB_u(INSTRUCTION_FORMAT*);
    void STMIB_uw(INSTRUCTION_FORMAT*);
    void LDMIB_uw(INSTRUCTION_FORMAT*);
    
    void B(INSTRUCTION_FORMAT*);
    void BL(INSTRUCTION_FORMAT*);
    
    void STC_ofm(INSTRUCTION_FORMAT*);
    void LDC_ofm(INSTRUCTION_FORMAT*);
    void STC_prm(INSTRUCTION_FORMAT*);
    void LDC_prm(INSTRUCTION_FORMAT*);
    
    void STC_ofp(INSTRUCTION_FORMAT*);
    void LDC_ofp(INSTRUCTION_FORMAT*);
    void STC_prp(INSTRUCTION_FORMAT*);
    void LDC_prp(INSTRUCTION_FORMAT*);
    
    void STC_unm(INSTRUCTION_FORMAT*);
    void LDC_unm(INSTRUCTION_FORMAT*);
    void STC_ptm(INSTRUCTION_FORMAT*);
    void LDC_ptm(INSTRUCTION_FORMAT*);
    
    void STC_unp(INSTRUCTION_FORMAT*);
    void LDC_unp(INSTRUCTION_FORMAT*);
    void STC_ptp(INSTRUCTION_FORMAT*);
    void LDC_ptp(INSTRUCTION_FORMAT*);
	
	

};
