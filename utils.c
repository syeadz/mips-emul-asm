#include "utils.h"

const char *get_mnemonic(const uint32_t instr)
{
    uint8_t opcode = (instr >> 26) & 0x3F;
    const uint8_t funct = instr & 0x3F;

    switch (opcode)
    {
    case 0x00:
        if (funct == 0x20)
        {
            return "add";
        }
        break;
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

Instruction decode_instr(uint32_t instruction)
{
    Instruction instr;

    instr.opcode = instruction >> 26;

    switch (instr.opcode)
    {
    case 0x00: // R-type
        instr.r = decode_r_type(instruction);
        switch (instr.r.funct)
        {
        case 0x20: // add
            instr.format = R_RD_RS_RT;
            break;
        default:
            break;
        }
        break;

    case 0x02: // jump
        instr.j = decode_j_type(instruction);
        instr.format = J_I;
        break;
    case 0x0C: // beq
        instr.i = decode_i_type(instruction);
        instr.format = I_RS_RT_I;
        break;
    case 0x23: // lw
        instr.i = decode_i_type(instruction);
        instr.format = I_RT_I_RS;
        break;
    case 0x2B:
        instr.i = decode_i_type(instruction);
        instr.format = I_RT_I_RS;
        break;
    default:
        break;
    }

    return instr;
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