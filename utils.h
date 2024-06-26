#pragma once

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/// @brief Enum to hold the type and template of the instruction.
// See: https://uweb.engr.arizona.edu/~ece369/Resources/spim/MIPSReference.pdf
typedef enum ITemplate
{
    R_RD_RS_RT,    // add rd, rs, rt
    R_RS_RT,       // div rs, rt
    R_RD_RT_SHAMT, // sll rd, rt, shamt   -- shift left logical
    R_RD_RT_RS,    // sllv rd, rt, rs     -- shift left logical variable
    R_RS,          // jr rs
    R_RD,          // mfhi rd

    I_RT_RS_I,     // addi rt, rs, i
    I_RT_IMM32,    // lhi rt, imm32
    I_RS_RT_LABEL, // bne rs, rt, label
    I_RS_LABEL,    // bgtz rs, label
    I_RT_I_RS,     // lw rt, i(rs)
    I_RS_RT_I,     // beq rs, rt, i

    J_LABEL, // j label
    J_I,      // j i

    UNKNOWN
} ITemplate;

/// @brief Struct to hold r-type instruction
typedef struct RArgs
{
    uint8_t rs;
    uint8_t rt;
    uint8_t rd;
    uint8_t shamt;
    uint8_t funct;
} RArgs;

/// @brief Struct to hold i-type instruction
typedef struct IArgs
{
    uint8_t rs;
    uint8_t rt;
    uint16_t imm;
} IArgs;

/// @brief Struct to hold j-type instruction
typedef struct JArgs
{
    uint32_t target;
} JArgs;

/// @brief Struct to hold instruction
typedef struct Instruction
{
    uint32_t instr;
    uint8_t opcode;
    ITemplate format;
    union
    {
        RArgs r;
        IArgs i;
        JArgs j;
    };
} Instruction;

/// @brief Decode an instruction from a 32 bit number.
/// @param instruction
/// @return
Instruction decode_instr(uint32_t instruction);

/// @brief Returns mnemonic for an instruction.
/// @param opcode
/// @return
const char *get_mnemonic_from_instr(uint32_t instruction);

/// @brief Parses a number from a string.
/// @param arg
/// @return Returns the parsed number, or returns -1 if the number is invalid.
long parse_number(const char *arg);

/// @brief Decode an R-type instruction
/// @param instruction
/// @return RType
RArgs decode_r_type(uint32_t instruction);

/// @brief Decode an I-type instruction
/// @param instruction
/// @return IType
IArgs decode_i_type(uint32_t instruction);

/// @brief Decode a J-type instruction
/// @param instruction
/// @return JType
JArgs decode_j_type(uint32_t instruction);

/// @brief Get the name of a register.
/// @param reg
/// @return
const char *get_reg_name(uint8_t reg);

/// @brief Get the the template of an instruction from the opcode.
/// @param opcode 
/// @return 
ITemplate get_template_from_opcode(uint8_t opcode);

/// @brief Get the opcode from a mnemonic.
/// @param mnemonic 
/// @return 
uint8_t get_opcode_from_mnemonic(const char *mnemonic);

/// @brief Gets the register number from the register name.
/// @param reg 
/// @return 
int get_register_number_from_name(const char *reg);

/// @brief Gets the opcode from the mnemonic.
/// @param mnemonic 
/// @return 
uint8_t get_opcode_from_mnemonic(const char *mnemonic);
