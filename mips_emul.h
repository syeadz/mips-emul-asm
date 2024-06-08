#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// Useful bitwise macros

#define BIT_MASK(bit_pos) (1 << (bit))
// Set the bit for a variable
#define SET_BIT(variable, bit_pos) ((variable) |= BIT_MASK(bit_pos))
// Clear the bit for a value
#define CLEAR_BIT(variable, bit_pos) ((variable) &= ~BIT_MASK(bit_pos))
// Test if a bit is set
#define TEST_BIT(variable, bit_pos) (((variable) & BIT_MASK(bit_pos)) ? 1 : 0)

// MIPS registers, use as index into the regs array in StateMIPS
typedef enum registers
{
    zero,
    at,
    v0,
    v1,
    a0,
    a1,
    a2,
    a3,
    t0,
    t1,
    t2,
    t3,
    t4,
    t5,
    t6,
    t7,
    s0,
    s1,
    s2,
    s3,
    s4,
    s5,
    s6,
    s7,
    t8,
    t9,
    k0,
    k1,
    gp,
    sp,
    fp,
    ra
} reg_t;

typedef enum exception_codes
{
    Int = 0, // interrupt
    AdEL = 4, // address error exception (load or instruction fetch)
    AdES = 5, // address error exception (store)
    IBE = 6,  // bus error on instruction fetch
    DBE = 7,  // bus error on load or store
    Sys = 8,  // syscall
    Bp = 9,   // breakpoint
    RI = 10,  // reserved instruction
    CpU = 11, // coprocessor unimplemented
    Ov = 12,  // arithmetic overflow
    Tr = 13,  // trap
    FPE = 15  // floating point exception
} exception_t;

/// @brief Struct to hold the state of the MIPS processor
typedef struct StateMIPS
{
    // MIPS registers
    uint32_t regs[32];

    // program counter
    uint32_t pc;

    // pointer to program in memory
    uint32_t *mem;
} StateMIPS;

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

/// @brief Read a file into memory at a specific offset
/// @param state
/// @param filename
/// @param offset
void ReadFileIntoMemoryAt(StateMIPS *state, char *filename, uint32_t offset);

/// @brief Initialize the MIPS processor
/// @param
/// @return
StateMIPS *InitMIPS(void);