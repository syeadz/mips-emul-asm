#pragma once

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/// @brief Enum to hold the type and template of the instruction.
// See: https://uweb.engr.arizona.edu/~ece369/Resources/spim/MIPSReference.pdf
typedef enum i_template
{
    R_rd_rs_rt,    // add rd, rs, rt
    R_rs_rt,       // div rs, rt
    R_rd_rt_shamt, // sll rd, rt, shamt   -- shift left logical
    R_rd_rt_rs,    // sllv rd, rt, rs     -- shift left logical variable
    R_rs,          // jr rs
    R_rd,          // mfhi rd

    I_rt_rs_i,     // addi rt, rs, i
    I_rt_imm32,    // lhi rt, imm32
    I_rs_rt_label, // bne rs, rt, label
    I_rs_label,    // bgtz rs, label
    I_rt_i_rs,     // lw rt, i(rs)
    I_rs_rt_i,     // beq rs, rt, i

    J_label, // j label
    J_i      // j i
} i_template;

/// @brief Struct to hold r-type instruction
typedef struct r_type
{
    uint8_t rs;
    uint8_t rt;
    uint8_t rd;
    uint8_t shamt;
    uint8_t funct;
} r_type;

/// @brief Struct to hold i-type instruction
typedef struct i_type
{
    uint8_t rs;
    uint8_t rt;
    uint16_t imm;
} i_type;

/// @brief Struct to hold j-type instruction
typedef struct j_type
{
    uint32_t target;
} j_type;

/// @brief Struct to hold instruction
typedef struct instr_t
{
    uint32_t instr;
    uint8_t opcode;
    i_template format;
    union
    {
        r_type r;
        i_type i;
        j_type j;
    };
} instr_t;

/// @brief Decode an instruction from a 32 bit number.
/// @param instruction
/// @return
instr_t DecodeInstruction(uint32_t instruction);

/// @brief Returns mnemonic for an instruction.
/// @param opcode
/// @return
const char *get_mnemonic(uint32_t instruction);

/// @brief Parses a number from a string.
/// @param arg
/// @return Returns the parsed number, or returns -1 if the number is invalid.
long parse_number(const char *arg);

/// @brief Decode an R-type instruction
/// @param instruction
/// @return r_type
r_type DecodeRType(uint32_t instruction);

/// @brief Decode an I-type instruction
/// @param instruction
/// @return i_type
i_type DecodeIType(uint32_t instruction);

/// @brief Decode a J-type instruction
/// @param instruction
/// @return j_type
j_type DecodeJType(uint32_t instruction);

/// @brief Get the name of a register.
/// @param reg
/// @return
const char *GetRegName(uint8_t reg);