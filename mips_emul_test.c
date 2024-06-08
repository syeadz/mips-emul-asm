#include "minunit.h"
#include "mips_emul.h"

void print_state();
void print_full();
void sr(reg_t reg, uint32_t val);
void sm(uint32_t index, uint32_t val);

// pointer to MIPS State
static StateMIPS *pState;

// ********* SETUP ********* //

/// @brief Initializes 8080State and gives its pointer to pState*. Is ran before every test.
void test_setup()
{
    pState = calloc(1,sizeof(StateMIPS));
    pState->mem = malloc(0x1000);  //4K
}

/// @brief Frees memory from pState.memory pointer. Is ran after every test.
void test_teardown() {
    free(pState->mem);
    free(pState);
}

// ********* function tests ********* //

// functions tests help verify functions like add_arith_flags are working properly

MU_TEST(test_decode_r_type) {
    // rs = 10, rt = 8, rd = 9, shamt = 0, funct = 0x20
    // 01010 01000 01001 00000 100000
    uint32_t instruction = 0x01484820;
    r_type r = DecodeRType(instruction);

    // check if flags are correct afterwards
    mu_assert(r.rs ==10, "RS was not set correctly");
    mu_assert(r.rt == 8, "RT was not set correctly");
    mu_assert(r.rd == 9, "RD was not set correctly");
    mu_assert(r.shamt == 0, "Shamt was not set correctly");
    mu_assert(r.funct == 0x20, "Funct was not set correctly");
}

MU_TEST(test_decode_i_type) {
    // rs = 10, rt = 8, imm = 0x1234
    // 01010 01000 0001001000110100
    uint32_t instruction = 0x01481034;
    i_type i = DecodeIType(instruction);

    // check if flags are correct afterwards
    mu_assert(i.rs == 10, "RS was not set correctly");
    mu_assert(i.rt == 8, "RT was not set correctly");
    mu_assert(i.imm == 0x1034, "Imm was not set correctly");
}

MU_TEST_SUITE(function_tests) {
    MU_RUN_TEST(test_decode_r_type);
    MU_RUN_TEST(test_decode_i_type);
}

int main() {
    MU_RUN_SUITE(function_tests);

	MU_REPORT();
	return MU_EXIT_CODE;
}

// ********* Helper functions ********* //

/// @brief Prints pState of each register
void print_state() {
    // TODO: Implement
}

/// @brief Prints pState of each register and condition cdoe
void print_full() {
    // TODO: Implement
}


/// @brief Sets the value of a register
/// @param reg the register to be changed
/// @param val the value to be set
void sr(reg_t reg, uint32_t val) {
    pState->regs[reg] = val;
}

/// @brief Sets a value at a memory location
/// @param index index in memory
/// @param val value to be set
void sm(uint32_t index, uint32_t val) {
    pState->mem[index] = val;
}