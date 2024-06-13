#include "mips_emul.h"

int EmulateMIPSp(StateMIPS *state)
{
    // int cycles = 1;
    uint32_t instr = state->mem[state->pc];
    uint8_t opcode = (instr >> 26) & 0x3F;

    r_type r = DecodeRType(instr);
    j_type j = DecodeJType(instr);
    i_type i = DecodeIType(instr);

    // TODO: consider using address for PC instead of index
    state->pc += 1;

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
        state->regs[i.rt] = state->mem[state->regs[i.rs] + i.imm];
        break;

    case 0x2b: // sw
        state->mem[state->regs[i.rs] + i.imm] = state->regs[i.rt];
        break;
    }

    return 0;
}

int ReadFileIntoMemoryAt(StateMIPS *state, char *filename, uint32_t offset)
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
        state->mem[offset/4 + i] = __builtin_bswap32(buffer[i]);
    }

    // Free the buffer
    free(buffer);
    return 0;
}

StateMIPS *InitMIPS(uint32_t pc_start)
{
    StateMIPS *state = calloc(1, sizeof(StateMIPS));
    state->mem = malloc(MEM_SIZE); // change to 2^32 for full 4GB address space
    state->pc = pc_start;
    return state;
}

void FreeMIPS(StateMIPS *state)
{
    free(state->mem);
    free(state);
}
