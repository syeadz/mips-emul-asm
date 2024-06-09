#include "utility.h"

int get_register_number(const char *reg)
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
}

int get_opcode(const char *mnemonic)
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
}