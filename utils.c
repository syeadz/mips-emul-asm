#include "utils.h"

const char *get_mnemonic_from_instr(const uint32_t instr)
{
    uint8_t opcode = (instr >> 26) & 0x3F;
    const uint8_t funct = instr & 0x3F;

    switch (opcode)
    {
    case 0x00:
        if (funct == 0x20)
        {
            return "add";
        } else {
            return "unknown";
        }
    case 0x02:
        return "j";
    case 0x0C:
        return "beq";
    case 0x23:
        return "lw";
    case 0x2B:
        return "sw";
    default:
        return "unknown";
    }
}

long parse_number(const char *arg)
{
    char *endptr;
    long number = 0;

    if (strlen(arg) > 2 && arg[0] == '0')
    {
        if (arg[1] == 'x' || arg[1] == 'X')
        {
            // Hexadecimal
            number = strtol(arg, &endptr, 16);
        }
        else if (arg[1] == 'b' || arg[1] == 'B')
        {
            // Binary (using custom parsing as strtol doesn't support binary)
            number = strtol(arg + 2, &endptr, 2);
        }
        else
        {
            // Assume octal
            number = strtol(arg, &endptr, 8);
        }
    }
    else
    {
        // Decimal
        number = strtol(arg, &endptr, 10);
    }

    if (endptr == arg || *endptr != '\0')
    {
        return -1;
    }

    return number;
}

RArgs decode_r_type(uint32_t instruction)
{
    RArgs r;
    // Extracts bits 25-21, 20-16, 15-11, 10-6, 5-0
    r.rs = (instruction >> 21) & 0x1f;
    r.rt = (instruction >> 16) & 0x1f;
    r.rd = (instruction >> 11) & 0x1f;
    r.shamt = (instruction >> 6) & 0x1f;
    r.funct = instruction & 0x3f;
    return r;
}

IArgs decode_i_type(uint32_t instruction)
{
    IArgs i;
    // Extracts bits 25-21, 20-16, 15-0
    i.rs = (instruction >> 21) & 0x1f;
    i.rt = (instruction >> 16) & 0x1f;
    i.imm = instruction & 0xffff;
    return i;
}

JArgs decode_j_type(uint32_t instruction)
{
    JArgs j;
    // Extracts bits 25-0
    j.target = instruction & 0x3ffffff;
    return j;
}

ITemplate get_template_from_opcode(uint8_t opcode)
{
    switch (opcode)
    {
    // R-type
    case 0x00: // add
        return R_RD_RS_RT;


    // I-type
     // I_RS_RT_I
    case 0x0C: // beq
        return I_RS_RT_I;

    // I_RT_I_RS
    case 0x23: // lw
    case 0x2B: // sw
        return I_RT_I_RS;


    // J-type
    // J_I
    case 0x02: // j
        return J_I;
    default:
        return UNKNOWN;
    }
}

Instruction decode_instr(uint32_t instruction)
{
    Instruction instr;

    uint8_t opcode = instruction >> 26;
    ITemplate template = get_template_from_opcode(opcode);

    instr.instr = instruction;
    instr.opcode = opcode;
    instr.format = template;

    switch (template)
    {
    case R_RD_RS_RT:
        instr.r = decode_r_type(instruction);
        break;
    case I_RS_RT_I:
    case I_RT_I_RS:
        instr.i = decode_i_type(instruction);
        break;
    case J_I:
        instr.j = decode_j_type(instruction);
        break;
    default:
        break;
    }

    return instr;
}

uint8_t get_opcode_from_mnemonic(const char *mnemonic)
{
    if (strcmp(mnemonic, "add") == 0)
        return 0x00;
    if (strcmp(mnemonic, "j") == 0)
        return 0x02;
    if (strcmp(mnemonic, "beq") == 0)
        return 0x0c;
    if (strcmp(mnemonic, "lw") == 0)
        return 0x23;
    if (strcmp(mnemonic, "sw") == 0)
        return 0x2b;
    
    return 0xFF;
}

const char *get_reg_name(uint8_t reg)
{
    switch (reg)
    {
    case 0:
        return "zero";
    case 1:
        return "at";
    case 2:
        return "v0";
    case 3:
        return "v1";
    case 4:
        return "a0";
    case 5:
        return "1";
    case 6:
        return "a2";
    case 7:
        return "a3";
    case 8:
        return "t0";
    case 9:
        return "t1";
    case 10:
        return "t2";
    case 11:
        return "t3";
    case 12:
        return "t4";
    case 13:
        return "t5";
    case 14:
        return "t6";
    case 15:
        return "t7";
    case 16:
        return "s0";
    case 17:
        return "s1";
    case 18:
        return "s2";
    case 19:
        return "s3";
    case 20:
        return "s4";
    case 21:
        return "s5";
    case 22:
        return "s6";
    case 23:
        return "s7";
    case 24:
        return "t8";
    case 25:
        return "t9";
    case 26:
        return "k0";
    case 27:
        return "k1";
    case 28:
        return "gp";
    case 29:
        return "sp";
    case 30:
        return "fp";
    case 31:
        return "ra";
    default:
        return "unknown";
    }
}
int get_register_number_from_name(const char *reg)
{
    if (strcmp(reg, "$zero") == 0)
        return 0;
    if (strcmp(reg, "$at") == 0)
        return 1;
    if (strcmp(reg, "$v0") == 0)
        return 2;
    if (strcmp(reg, "$v1") == 0)
        return 3;
    if (strcmp(reg, "$a0") == 0)
        return 4;
    if (strcmp(reg, "$a1") == 0)
        return 5;
    if (strcmp(reg, "$a2") == 0)
        return 6;
    if (strcmp(reg, "$a3") == 0)
        return 7;
    if (strcmp(reg, "$t0") == 0)
        return 8;
    if (strcmp(reg, "$t1") == 0)
        return 9;
    if (strcmp(reg, "$t2") == 0)
        return 10;
    if (strcmp(reg, "$t3") == 0)
        return 11;
    if (strcmp(reg, "$t4") == 0)
        return 12;
    if (strcmp(reg, "$t5") == 0)
        return 13;
    if (strcmp(reg, "$t6") == 0)
        return 14;
    if (strcmp(reg, "$t7") == 0)
        return 15;
    if (strcmp(reg, "$s0") == 0)
        return 16;
    if (strcmp(reg, "$s1") == 0)
        return 17;
    if (strcmp(reg, "$s2") == 0)
        return 18;
    if (strcmp(reg, "$s3") == 0)
        return 19;
    if (strcmp(reg, "$s4") == 0)
        return 20;
    if (strcmp(reg, "$s5") == 0)
        return 21;
    if (strcmp(reg, "$s6") == 0)
        return 22;
    if (strcmp(reg, "$s7") == 0)
        return 23;
    if (strcmp(reg, "$t8") == 0)
        return 24;
    if (strcmp(reg, "$t9") == 0)
        return 25;
    if (strcmp(reg, "$k0") == 0)
        return 26;
    if (strcmp(reg, "$k1") == 0)
        return 27;
    if (strcmp(reg, "$gp") == 0)
        return 28;
    if (strcmp(reg, "$sp") == 0)
        return 29;
    if (strcmp(reg, "$fp") == 0)
        return 30;
    if (strcmp(reg, "$ra") == 0)
        return 31;
    return -1;
}
