#include "mips_emul.h"

int emulate_mips(StateMIPS *state)
{
    // If keeping track of cycles
    // int cycles = 1;

    // Get instruction from memory, divide by 4 to get index from address
    uint32_t instr = state->mem[state->pc / 4];
    uint8_t opcode = (instr >> 26) & 0x3F;

    RType r = decode_r_type(instr);
    JType j = decode_j_type(instr);
    IType i = decode_i_type(instr);

    // Increment by 4 bytes (32 bits) to point to next instruction
    state->pc += 4;

    switch (opcode)
    {
    case 0x00:               // arith/logic
        if (r.funct == 0x20) // add
            state->regs[r.rd] = state->regs[r.rs] + state->regs[r.rt];
        break;

    case 0x02: // j
        state->pc = j.target;
        break;

    case 0x0c: // beq
        if (state->regs[i.rs] == state->regs[i.rt])
        {
            state->pc += i.imm;
        }
        break;

    case 0x23: // lw
        state->regs[i.rt] = state->mem[(state->regs[i.rs] + i.imm) / 4];
        break;

    case 0x2b: // sw
        state->mem[(state->regs[i.rs] + i.imm) / 4] = state->regs[i.rt];
        break;
    }

    return 0;
}

int read_file_into_mem_at(StateMIPS *state, char *filename, uint32_t offset)
{
    FILE *f = fopen(filename, "rb");
    if (f == NULL)
    {
        printf("error: Couldn't open %s\n", filename);
        return 1;
    }

    fseek(f, 0L, SEEK_END);
    long unsigned fsize = ftell(f);
    fseek(f, 0L, SEEK_SET);

    // Allocate buffer for reading
    uint32_t *buffer = malloc(fsize);
    if (buffer == NULL)
    {
        printf("error: Couldn't allocate buffer for %s\n", filename);
        fclose(f);
        return 1;
    }

    // Read the file into the buffer
    fread(buffer, fsize, 1, f);
    fclose(f);

    // Convert endianness and copy to memory
    for (long unsigned i = 0; i < fsize / sizeof(uint32_t); i++)
    {
        state->mem[offset / 4 + i] = __builtin_bswap32(buffer[i]);
    }

    // Free the buffer
    free(buffer);
    return 0;
}

StateMIPS *init_mips(uint32_t pc_start)
{
    StateMIPS *state = calloc(1, sizeof(StateMIPS));
    state->mem = malloc(MEM_SIZE); // change to 2^32 for full 4GB address space
    state->pc = pc_start;
    return state;
}

void free_mips(StateMIPS *state)
{
    free(state->mem);
    free(state);
}
