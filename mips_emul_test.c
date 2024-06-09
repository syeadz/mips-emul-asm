#include "minunit.h"
#include "mips_emul.h"

void print_state();
void print_full();
void sr(reg_t reg, uint32_t val);
void sm(uint32_t index, uint32_t val);
void pr(reg_t reg);
void pm(uint32_t index);

// pointer to MIPS State
static StateMIPS *pState;

// ********* SETUP ********* //

/// @brief Initializes 8080State and gives its pointer to pState*. Is ran before every test.
void test_setup()
{
    pState = calloc(1, sizeof(StateMIPS));
    pState->mem = malloc(sizeof(uint32_t) * 64);
    if (!pState->mem)
    {
        perror("Failed to allocate memory");
        exit(1);
    }
}

/// @brief Frees memory from pState.memory pointer. Is ran after every test.
void test_teardown()
{
    free(pState->mem);
    free(pState);
}

// ********* function tests ********* //

// functions tests help verify functions like add_arith_flags are working properly

MU_TEST(test_decode_r_type)
{
    // rs = 10, rt = 8, rd = 9, shamt = 0, funct = 0x20
    // 01010 01000 01001 00000 100000
    uint32_t instruction = 0x01484820;
    r_type r = DecodeRType(instruction);

    // check if flags are correct afterwards
    mu_assert(r.rs == 10, "RS was not set correctly");
    mu_assert(r.rt == 8, "RT was not set correctly");
    mu_assert(r.rd == 9, "RD was not set correctly");
    mu_assert(r.shamt == 0, "Shamt was not set correctly");
    mu_assert(r.funct == 0x20, "Funct was not set correctly");
}

MU_TEST(test_decode_i_type)
{
    // rs = 10, rt = 8, imm = 0x1234
    // 01010 01000 0001001000110100
    uint32_t instruction = 0x01481034;
    i_type i = DecodeIType(instruction);

    // check if flags are correct afterwards
    mu_assert(i.rs == 10, "RS was not set correctly");
    mu_assert(i.rt == 8, "RT was not set correctly");
    mu_assert(i.imm == 0x1034, "Imm was not set correctly");
}

MU_TEST(test_decode_j_type)
{
    // target = 0x2345678
    // 00010010001101010101100101111000
    uint32_t instruction = 0xA345678;
    j_type j = DecodeJType(instruction);

    // check if flags are correct afterwards
    mu_assert(j.target == 0x2345678, "Target was not set correctly");
}

MU_TEST_SUITE(function_tests)
{
    MU_RUN_TEST(test_decode_r_type);
    MU_RUN_TEST(test_decode_i_type);
    MU_RUN_TEST(test_decode_j_type);
}

// ********* opcode tests ********* //

// ADD $t1, $t2, $t3
MU_TEST(test_0x00_0x20_add)
{
    // add $t1, $t2, $t3
    // t1 = t2 + t3
    // instruction = (0, 10, 11, 9, 0, 0x20)
    uint32_t instruction = 0x14b4820;
    sm(0, instruction);

    sr(t2, 0x1234);
    sr(t3, 0x5678);

    EmulateMIPSp(pState);

    mu_assert(pState->regs[t1] == 0x1234 + 0x5678, "Add did not work correctly");
}

// J $addr
MU_TEST(test_0x02_j)
{
    // j 0x0A
    // instruction = (2, 0x0A)
    uint32_t instruction = 0x800000A;
    sm(0, instruction);

    EmulateMIPSp(pState);

    mu_assert(pState->pc == 0x0A, "J did not work correctly");
}

// BEQ $t1, $t2, addr
MU_TEST(test_0x0c_beq)
{
    // if t1 == t2, pc += 16
    // instruction = (0x0C, 9, 10, 0x10)
    uint32_t instruction = 0x312a0010;
    sm(0, instruction);

    sr(t1, 0x1234);
    sr(t2, 0x1234);

    EmulateMIPSp(pState);

    mu_assert(pState->pc == 17, "Beq did not work correctly");
}

// LW $t1, offset($t2)
MU_TEST(test_0x23_lw)
{
    // t1 = mem[t2 + 12]
    // instruction = (0x23, 10, 9, 12)
    uint32_t instruction = 0x8d49000c;
    sm(0, instruction);

    sr(t2, 0x10);
    sm(0x10 + 12, 0x9ABC);

    EmulateMIPSp(pState);

    mu_assert(pState->regs[t1] == 0x9ABC, "Lw did not work correctly");
}

// SW $t1, offset($t2)
MU_TEST(test_0x2b_sw)
{
    // mem[t2 + 12] = t1
    // instruction = (0x2B, 10, 9, 12)
    uint32_t instruction = 0xad49000c;
    sm(0, instruction);

    sr(t1, 0x9ABC);
    sr(t2, 0x01);

    EmulateMIPSp(pState);

    mu_assert(pState->mem[0x01 + 12] == 0x9ABC, "Sw did not work correctly");
}


MU_TEST_SUITE(opcode_tests)
{
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    MU_RUN_TEST(test_0x00_0x20_add);
    MU_RUN_TEST(test_0x02_j);
    MU_RUN_TEST(test_0x0c_beq);
    MU_RUN_TEST(test_0x23_lw);
    MU_RUN_TEST(test_0x2b_sw);
}

int main()
{
    MU_RUN_SUITE(function_tests);
    MU_RUN_SUITE(opcode_tests);

    MU_REPORT();
    return MU_EXIT_CODE;
}

// ********* Helper functions ********* //

/// @brief Prints pState of each register
void print_state()
{
    // TODO: Implement
}

/// @brief Prints pState of each register and condition cdoe
void print_full()
{
    // TODO: Implement
}

/// @brief Sets the value of a register
/// @param reg the register to be changed
/// @param val the value to be set
void sr(reg_t reg, uint32_t val)
{
    pState->regs[reg] = val;
}

/// @brief Sets a value at a memory location
/// @param index index in memory
/// @param val value to be set
void sm(uint32_t index, uint32_t val)
{
    pState->mem[index] = val;
}

/// @brief Prints the value of a register
/// @param reg the register to be printed
void pr(reg_t reg)
{
    printf("Register %d : %d (%x)\n", reg, pState->regs[reg], pState->regs[reg]);
}

/// @brief Prints the value at a memory location
/// @param index index in memory
void pm(uint32_t index)
{
    printf("Memory at %d: %d (%x)\n", index, pState->mem[index], pState->mem[index]);
}