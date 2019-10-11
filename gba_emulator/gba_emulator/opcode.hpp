#pragma once

typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned int   U32;

//instruction bit[31:28]
#define COND_EQ             (0x0)     //Z set 	                    equal
#define COND_NE             (0x1)     //Z clear                     not equal
#define COND_CS             (0x2)     //C set                       unsigned higher or same
#define COND_CC             (0x3)     //C clear                     unsigned lower
#define COND_MI             (0x4)     //N set                       negative
#define COND_PL             (0x5)     //N clear                     positive or zero
#define COND_VS             (0x6)     //V set                       overflow
#define COND_VC             (0x7)     //V clear                     no overflow
#define COND_HI             (0x8)     //C set and Z clear           unsigned higher
#define COND_LS             (0x9)     //C clear or Z set            unsigned lower or same
#define COND_GE             (0xA)     //N equals V                  greater or equal
#define COND_LT             (0xB)     //N not equal to V            less than
#define COND_GT             (0xC)     //Z clear AND (N equals V)    greater than
#define COND_LE             (0xD)     //Z set OR (N not equal to V) less than or equal
#define COND_AL             (0xE)     //(ignored)                   always

//instruction bit[6:5]
#define SHIFT_TYPE_LOGICAL_LEFT     (0x0)
#define SHIFT_TYPE_LOGICAL_RIGHT    (0x0)
#define SHIFT_TYPE_ARITHMETIC_RIGHT (0x0)
#define SHIFT_TYPE_ROTATE_RIGHT     (0x0)



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
                        U32 rsv0 : 1;         // = 0
                        U32 shift_type : 2;
                        U32 shift_amount : 5;  //5 bit unsigned integer
                    };
                    union
                    {
                        U32 rsv0 : 1;          // = 1
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


    //Software Interrupt
    union
    {
        U32 rsv : 28;
        U32 cond : 4;
    }sw_int;


}INSTRUCTION_FORMAT;

#pragma pack()


class GBA_EMUALTOR_ARM7TDMI;

//logical left immediate
void GBA_EMUALTOR_ARM7TDMI::AND_lli(INSTRUCTION_FORMAT *instruction_ptr)
{
    U32 Rn           = instruction_ptr->data_proc.Rn;
    U32 Rd           = instruction_ptr->data_proc.Rd;
    U32 Rm           = instruction_ptr->data_proc.operand2.Rm;
    //U8  shift_type = instruction_ptr->data_proc.operand2.shift.shift_type;
    U8  shift_amount = instruction_ptr->data_proc.operand2.shift.shift_amount;

    this->R[Rd] = this->R[Rn] & this->R[Rm];


}
//doc, p55 : The form of the shift field which might be expected to correspond to LSR #0 is used to
//encode LSR #32, which has a zero result with bit 31 of Rm as the carry output.Logical
//shift right zero is redundant as it is the same as logical shift left zero, so the assembler
//will convert LSR #0 (and ASR #0 and ROR #0) into LSL #0, and allow LSR #32 to be
//specified.

void GBA_EMUALTOR_ARM7TDMI::AND_llr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::AND_lri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::AND_lrr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::AND_ari(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::AND_arr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::AND_rri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::AND_rrr(INSTRUCTION_FORMAT *instruction_ptr) {}

void GBA_EMUALTOR_ARM7TDMI::ANDS_lli(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ANDS_llr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ANDS_lri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ANDS_lrr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ANDS_ari(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ANDS_arr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ANDS_rri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ANDS_rrr(INSTRUCTION_FORMAT *instruction_ptr) {}

//exclusive OR
void GBA_EMUALTOR_ARM7TDMI::EOR_lli(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::EOR_llr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::EOR_lri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::EOR_lrr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::EOR_ari(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::EOR_arr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::EOR_rri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::EOR_rrr(INSTRUCTION_FORMAT *instruction_ptr) {}

void GBA_EMUALTOR_ARM7TDMI::EORS_lli(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::EORS_llr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::EORS_lri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::EORS_lrr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::EORS_ari(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::EORS_arr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::EORS_rri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::EORS_rrr(INSTRUCTION_FORMAT *instruction_ptr) {}

void GBA_EMUALTOR_ARM7TDMI::SUB_lli(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::SUB_llr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::SUB_lri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::SUB_lrr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::SUB_ari(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::SUB_arr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::SUB_rri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::SUB_rrr(INSTRUCTION_FORMAT *instruction_ptr) {}

void GBA_EMUALTOR_ARM7TDMI::SUBS_lli(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::SUBS_llr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::SUBS_lri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::SUBS_lrr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::SUBS_ari(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::SUBS_arr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::SUBS_rri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::SUBS_rrr(INSTRUCTION_FORMAT *instruction_ptr) {}

void GBA_EMUALTOR_ARM7TDMI::RSB_lli(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::RSB_llr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::RSB_lri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::RSB_lrr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::RSB_ari(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::RSB_arr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::RSB_rri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::RSB_rrr(INSTRUCTION_FORMAT *instruction_ptr) {}

void GBA_EMUALTOR_ARM7TDMI::RSBS_lli(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::RSBS_llr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::RSBS_lri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::RSBS_lrr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::RSBS_ari(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::RSBS_arr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::RSBS_rri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::RSBS_rrr(INSTRUCTION_FORMAT *instruction_ptr) {}

void GBA_EMUALTOR_ARM7TDMI::ADD_lli(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ADD_llr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ADD_lri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ADD_lrr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ADD_ari(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ADD_arr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ADD_rri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ADD_rrr(INSTRUCTION_FORMAT *instruction_ptr) {}

void GBA_EMUALTOR_ARM7TDMI::ADDS_lli(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ADDS_llr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ADDS_lri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ADDS_lrr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ADDS_ari(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ADDS_arr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ADDS_rri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ADDS_rrr(INSTRUCTION_FORMAT *instruction_ptr) {}

void GBA_EMUALTOR_ARM7TDMI::ADC_lli(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ADC_llr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ADC_lri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ADC_lrr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ADC_ari(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ADC_arr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ADC_rri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ADC_rrr(INSTRUCTION_FORMAT *instruction_ptr) {}

void GBA_EMUALTOR_ARM7TDMI::ADCS_lli(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ADCS_llr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ADCS_lri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ADCS_lrr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ADCS_ari(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ADCS_arr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ADCS_rri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ADCS_rrr(INSTRUCTION_FORMAT *instruction_ptr) {}

void GBA_EMUALTOR_ARM7TDMI::SBC_lli(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::SBC_llr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::SBC_lri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::SBC_lrr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::SBC_ari(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::SBC_arr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::SBC_rri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::SBC_rrr(INSTRUCTION_FORMAT *instruction_ptr) {}

void GBA_EMUALTOR_ARM7TDMI::SBCS_lli(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::SBCS_llr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::SBCS_lri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::SBCS_lrr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::SBCS_ari(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::SBCS_arr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::SBCS_rri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::SBCS_rrr(INSTRUCTION_FORMAT *instruction_ptr) {}

void GBA_EMUALTOR_ARM7TDMI::RSC_lli(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::RSC_llr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::RSC_lri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::RSC_lrr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::RSC_ari(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::RSC_arr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::RSC_rri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::RSC_rrr(INSTRUCTION_FORMAT *instruction_ptr) {}

void GBA_EMUALTOR_ARM7TDMI::RSCS_lli(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::RSCS_llr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::RSCS_lri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::RSCS_lrr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::RSCS_ari(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::RSCS_arr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::RSCS_rri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::RSCS_rrr(INSTRUCTION_FORMAT *instruction_ptr) {}

void GBA_EMUALTOR_ARM7TDMI::TSTS_lli(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::TSTS_llr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::TSTS_lri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::TSTS_lrr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::TSTS_ari(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::TSTS_arr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::TSTS_rri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::TSTS_rrr(INSTRUCTION_FORMAT *instruction_ptr) {}

void GBA_EMUALTOR_ARM7TDMI::TEQS_lli(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::TEQS_llr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::TEQS_lri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::TEQS_lrr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::TEQS_ari(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::TEQS_arr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::TEQS_rri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::TEQS_rrr(INSTRUCTION_FORMAT *instruction_ptr) {}

void GBA_EMUALTOR_ARM7TDMI::CMPS_lli(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::CMPS_llr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::CMPS_lri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::CMPS_lrr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::CMPS_ari(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::CMPS_arr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::CMPS_rri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::CMPS_rrr(INSTRUCTION_FORMAT *instruction_ptr) {}

void GBA_EMUALTOR_ARM7TDMI::CMNS_lli(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::CMNS_llr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::CMNS_lri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::CMNS_lrr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::CMNS_ari(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::CMNS_arr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::CMNS_rri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::CMNS_rrr(INSTRUCTION_FORMAT *instruction_ptr) {}

void GBA_EMUALTOR_ARM7TDMI::ORR_lli(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ORR_llr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ORR_lri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ORR_lrr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ORR_ari(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ORR_arr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ORR_rri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ORR_rrr(INSTRUCTION_FORMAT *instruction_ptr) {}

void GBA_EMUALTOR_ARM7TDMI::ORRS_lli(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ORRS_llr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ORRS_lri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ORRS_lrr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ORRS_ari(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ORRS_arr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ORRS_rri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::ORRS_rrr(INSTRUCTION_FORMAT *instruction_ptr) {}

void GBA_EMUALTOR_ARM7TDMI::MOV_lli(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::MOV_llr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::MOV_lri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::MOV_lrr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::MOV_ari(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::MOV_arr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::MOV_rri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::MOV_rrr(INSTRUCTION_FORMAT *instruction_ptr) {}

void GBA_EMUALTOR_ARM7TDMI::MOVS_lli(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::MOVS_llr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::MOVS_lri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::MOVS_lrr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::MOVS_ari(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::MOVS_arr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::MOVS_rri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::MOVS_rrr(INSTRUCTION_FORMAT *instruction_ptr) {}

void GBA_EMUALTOR_ARM7TDMI::BIC_lli(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::BIC_llr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::BIC_lri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::BIC_lrr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::BIC_ari(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::BIC_arr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::BIC_rri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::BIC_rrr(INSTRUCTION_FORMAT *instruction_ptr) {}

void GBA_EMUALTOR_ARM7TDMI::BICS_lli(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::BICS_llr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::BICS_lri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::BICS_lrr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::BICS_ari(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::BICS_arr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::BICS_rri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::BICS_rrr(INSTRUCTION_FORMAT *instruction_ptr) {}

void GBA_EMUALTOR_ARM7TDMI::MVM_lli(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::MVM_llr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::MVM_lri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::MVM_lrr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::MVM_ari(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::MVM_arr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::MVM_rri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::MVM_rrr(INSTRUCTION_FORMAT *instruction_ptr) {}

void GBA_EMUALTOR_ARM7TDMI::MVMS_lli(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::MVMS_llr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::MVMS_lri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::MVMS_lrr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::MVMS_ari(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::MVMS_arr(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::MVMS_rri(INSTRUCTION_FORMAT *instruction_ptr) {}
void GBA_EMUALTOR_ARM7TDMI::MVMS_rrr(INSTRUCTION_FORMAT *instruction_ptr) {}

void GBA_EMUALTOR_ARM7TDMI::ANDS_imm(INSTRUCTION_FORMAT *instruction_ptr){}	
void GBA_EMUALTOR_ARM7TDMI::EOR_imm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::EORS_imm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::SUB_imm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::SUBS_imm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::RSB_imm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::RSBS_imm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::ADD_imm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::ADDS_imm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::ADC_imm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::ADCS_imm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::SBC_imm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::SBCS_imm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::RSC_imm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::RSCS_imm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::TSTS_imm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::MSR_ic(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::TEQS_imm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::CMPS_imm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::MSR_is(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::CMNS_imm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::ORR_imm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::ORRS_imm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::MOV_imm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::MOVS_imm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::BIC_imm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::BICS_imm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::MVN_imm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::MVNS_imm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STR_ptim(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDR_ptim(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STRT_ptim(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDRT_ptim(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STRB_ptim(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDRB_ptim(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STRBT_ptim(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDRBT_ptim(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STR_ptip(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDR_ptip(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STRT_ptip(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDRT_ptip(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STRB_ptip(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDRB_ptip(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STRBT_ptip(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDRBT_ptip(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STR_ofim(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDR_ofim(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STR_prim(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDR_prim(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STRB_ofim(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDRB_ofim(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STRB_prim(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDRB_prim(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STR_ofip(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDR_ofip(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STR_prip(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDR_prip(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STRB_ofip(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDRB_ofip(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STRB_prip(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDRB_prip(INSTRUCTION_FORMAT *instruction_ptr){}

void GBA_EMUALTOR_ARM7TDMI::STR_ptrmll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STR_ptrmlr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STR_ptrmar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STR_ptrmrr(INSTRUCTION_FORMAT *instruction_ptr){} 


void GBA_EMUALTOR_ARM7TDMI::LDR_ptrmll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDR_ptrmlr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDR_ptrmar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDR_ptrmrr(INSTRUCTION_FORMAT *instruction_ptr){} 


void GBA_EMUALTOR_ARM7TDMI::STRT_ptrmll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRT_ptrmlr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRT_ptrmar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRT_ptrmrr(INSTRUCTION_FORMAT *instruction_ptr){} 


void GBA_EMUALTOR_ARM7TDMI::LDRT_ptrmll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRT_ptrmlr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRT_ptrmar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRT_ptrmrr(INSTRUCTION_FORMAT *instruction_ptr){} 


void GBA_EMUALTOR_ARM7TDMI::STRB_ptrmll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRB_ptrmlr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRB_ptrmar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRB_ptrmrr(INSTRUCTION_FORMAT *instruction_ptr){} 


void GBA_EMUALTOR_ARM7TDMI::LDRB_ptrmll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRB_ptrmlr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRB_ptrmar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRB_ptrmrr(INSTRUCTION_FORMAT *instruction_ptr){} 


void GBA_EMUALTOR_ARM7TDMI::STRBT_ptrmll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRBT_ptrmlr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRBT_ptrmar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRBT_ptrmrr(INSTRUCTION_FORMAT *instruction_ptr){} 

          
void GBA_EMUALTOR_ARM7TDMI::LDRBT_ptrmll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRBT_ptrmlr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRBT_ptrmar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRBT_ptrmrr(INSTRUCTION_FORMAT *instruction_ptr){} 


void GBA_EMUALTOR_ARM7TDMI::STR_ptrpll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STR_ptrplr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STR_ptrpar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STR_ptrprr(INSTRUCTION_FORMAT *instruction_ptr){} 


void GBA_EMUALTOR_ARM7TDMI::LDR_ptrpll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDR_ptrplr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDR_ptrpar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDR_ptrprr(INSTRUCTION_FORMAT *instruction_ptr){} 


void GBA_EMUALTOR_ARM7TDMI::STRT_ptrpll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRT_ptrplr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRT_ptrpar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRT_ptrprr(INSTRUCTION_FORMAT *instruction_ptr){} 

         
void GBA_EMUALTOR_ARM7TDMI::LDRT_ptrpll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRT_ptrplr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRT_ptrpar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRT_ptrprr(INSTRUCTION_FORMAT *instruction_ptr){} 


void GBA_EMUALTOR_ARM7TDMI::STRB_ptrpll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRB_ptrplr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRB_ptrpar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRB_ptrprr(INSTRUCTION_FORMAT *instruction_ptr){} 

         
void GBA_EMUALTOR_ARM7TDMI::LDRB_ptrpll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRB_ptrplr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRB_ptrpar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRB_ptrprr(INSTRUCTION_FORMAT *instruction_ptr){} 


void GBA_EMUALTOR_ARM7TDMI::STRBT_ptrpll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRBT_ptrplr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRBT_ptrpar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRBT_ptrprr(INSTRUCTION_FORMAT *instruction_ptr){} 

          
void GBA_EMUALTOR_ARM7TDMI::LDRBT_ptrpll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRBT_ptrplr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRBT_ptrpar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRBT_ptrprr(INSTRUCTION_FORMAT *instruction_ptr){} 


void GBA_EMUALTOR_ARM7TDMI::STR_ofrmll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STR_ofrmlr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STR_ofrmar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STR_ofrmrr(INSTRUCTION_FORMAT *instruction_ptr){} 


void GBA_EMUALTOR_ARM7TDMI::LDR_ofrmll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDR_ofrmlr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDR_ofrmar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDR_ofrmrr(INSTRUCTION_FORMAT *instruction_ptr){} 


void GBA_EMUALTOR_ARM7TDMI::STR_prrmll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STR_prrmlr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STR_prrmar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STR_prrmrr(INSTRUCTION_FORMAT *instruction_ptr){} 


void GBA_EMUALTOR_ARM7TDMI::LDR_prrmll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDR_prrmlr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDR_prrmar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDR_prrmrr(INSTRUCTION_FORMAT *instruction_ptr){} 


void GBA_EMUALTOR_ARM7TDMI::STRB_ofrmll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRB_ofrmlr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRB_ofrmar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRB_ofrmrr(INSTRUCTION_FORMAT *instruction_ptr){} 

         
void GBA_EMUALTOR_ARM7TDMI::LDRB_ofrmll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRB_ofrmlr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRB_ofrmar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRB_ofrmrr(INSTRUCTION_FORMAT *instruction_ptr){} 

         
void GBA_EMUALTOR_ARM7TDMI::STRB_prrmll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRB_prrmlr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRB_prrmar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRB_prrmrr(INSTRUCTION_FORMAT *instruction_ptr){} 

         
void GBA_EMUALTOR_ARM7TDMI::LDRB_prrmll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRB_prrmlr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRB_prrmar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRB_prrmrr(INSTRUCTION_FORMAT *instruction_ptr){} 


void GBA_EMUALTOR_ARM7TDMI::STR_ofrpll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STR_ofrplr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STR_ofrpar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STR_ofrprr(INSTRUCTION_FORMAT *instruction_ptr){} 


void GBA_EMUALTOR_ARM7TDMI::LDR_ofrpll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDR_ofrplr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDR_ofrpar(INSTRUCTION_FORMAT *instruction_ptr){} 


void GBA_EMUALTOR_ARM7TDMI::STR_prrpll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STR_prrplr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STR_prrpar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STR_prrprr(INSTRUCTION_FORMAT *instruction_ptr){} 


void GBA_EMUALTOR_ARM7TDMI::LDR_prrpll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDR_prrplr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDR_prrpar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDR_prrprr(INSTRUCTION_FORMAT *instruction_ptr){} 


void GBA_EMUALTOR_ARM7TDMI::STRB_ofrpll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRB_ofrplr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRB_ofrpar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRB_ofrprr(INSTRUCTION_FORMAT *instruction_ptr){} 

         
void GBA_EMUALTOR_ARM7TDMI::LDRB_ofrpll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRB_ofrplr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRB_ofrpar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRB_ofrprr(INSTRUCTION_FORMAT *instruction_ptr){} 

         
void GBA_EMUALTOR_ARM7TDMI::STRB_prrpll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRB_prrplr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRB_prrpar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::STRB_prrprr(INSTRUCTION_FORMAT *instruction_ptr){} 

         
void GBA_EMUALTOR_ARM7TDMI::LDRB_prrpll(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRB_prrplr(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRB_prrpar(INSTRUCTION_FORMAT *instruction_ptr){} 
void GBA_EMUALTOR_ARM7TDMI::LDRB_prrprr(INSTRUCTION_FORMAT *instruction_ptr){} 


void GBA_EMUALTOR_ARM7TDMI::STMDA(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDMDA(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STMDA_w(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDMDA_w(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STMDA_u(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDMDA_u(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STMDA_uw(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDMDA_uw(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STMIA(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDMIA(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STMIA_w(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDMIA_w(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STMIA_u(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDMIA_u(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STMIA_uw(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDMIA_uw(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STMDB(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDMDB(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STMDB_w(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDMDB_w(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STMDB_u(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDMDB_u(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STMDB_uw(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDMDB_uw(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STMIB(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDMIB(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STMIB_w(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDMIB_w(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STMIB_u(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDMIB_u(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STMIB_uw(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDMIB_uw(INSTRUCTION_FORMAT *instruction_ptr){}

void GBA_EMUALTOR_ARM7TDMI::B(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::BL(INSTRUCTION_FORMAT *instruction_ptr){}

void GBA_EMUALTOR_ARM7TDMI::STC_ofm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDC_ofm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STC_prm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDC_prm(INSTRUCTION_FORMAT *instruction_ptr){}

void GBA_EMUALTOR_ARM7TDMI::STC_ofp(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDC_ofp(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STC_prp(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDC_prp(INSTRUCTION_FORMAT *instruction_ptr){}

void GBA_EMUALTOR_ARM7TDMI::STC_unm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDC_unm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STC_ptm(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDC_ptm(INSTRUCTION_FORMAT *instruction_ptr){}

void GBA_EMUALTOR_ARM7TDMI::STC_unp(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDC_unp(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::STC_ptp(INSTRUCTION_FORMAT *instruction_ptr){}
void GBA_EMUALTOR_ARM7TDMI::LDC_ptp(INSTRUCTION_FORMAT *instruction_ptr){}

