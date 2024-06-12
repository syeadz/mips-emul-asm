#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "utils.h"

// Useful bitwise macros

#define BIT_MASK(bit_pos) (1 << (bit))
// Set the bit for a variable
#define SET_BIT(variable, bit_pos) ((variable) |= BIT_MASK(bit_pos))
// Clear the bit for a value
#define CLEAR_BIT(variable, bit_pos) ((variable) &= ~BIT_MASK(bit_pos))
// Test if a bit is set
#define TEST_BIT(variable, bit_pos) (((variable) & BIT_MASK(bit_pos)) ? 1 : 0)

// This is the size of the memory in bytes
#define MEM_SIZE 0x1000

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
    Int = 0,  // interrupt
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
    // NOTE: This is the index in the memory array, not the address in memory
    uint32_t pc;

    // pointer to program in memory
    uint32_t *mem;
} StateMIPS;

/// @brief Read a file into memory at a specific offset
/// NOTE: This function assumes the file is a binary file
/// Also, the offset is used like mem[offset] = file[0], mem[offset+1] = file[1], etc.
/// As such, the offset does not mean address in memory, but rather the index in the memory array.
/// @param state
/// @param filename
/// @param offset
/// @return returns 0 on success, 1 on failure
int ReadFileIntoMemoryAt(StateMIPS *state, char *filename, uint32_t offset);

/// @brief Initialize the MIPS processor
/// @param pc_start
/// @return StateMIPS*
StateMIPS *InitMIPS(uint32_t pc_start);

/// @brief Free the MIPS processor
/// @param state
void FreeMIPS(StateMIPS *state);

/// @brief Emulate the MIPS processor
/// @param state
/// @return
int EmulateMIPSp(StateMIPS *state);
