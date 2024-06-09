#include "mips_emul.h"

int EmulateMIPSp(StateMIPS *state)
{
    // int cycles = 1;
    uint32_t instr = state->mem[state->pc];
    uint8_t opcode = (instr >> 26) & 0x3F;

    r_type r = DecodeRType(instr);
    j_type j = DecodeJType(instr);
    i_type i = DecodeIType(instr);

    state->pc += sizeof(uint32_t);

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

r_type DecodeRType(uint32_t instruction)
{
    r_type r;
    // Extracts bits 25-21, 20-16, 15-11, 10-6, 5-0
    r.rs = (instruction >> 21) & 0x1f;
    r.rt = (instruction >> 16) & 0x1f;
    r.rd = (instruction >> 11) & 0x1f;
    r.shamt = (instruction >> 6) & 0x1f;
    r.funct = instruction & 0x3f;
    return r;
}

i_type DecodeIType(uint32_t instruction)
{
    i_type i;
    // Extracts bits 25-21, 20-16, 15-0
    i.rs = (instruction >> 21) & 0x1f;
    i.rt = (instruction >> 16) & 0x1f;
    i.imm = instruction & 0xffff;
    return i;
}

j_type DecodeJType(uint32_t instruction)
{
    j_type j;
    // Extracts bits 25-0
    j.target = instruction & 0x3ffffff;
    return j;
}
void ReadFileIntoMemoryAt(StateMIPS *state, char *filename, uint32_t offset)
{
    FILE *f = fopen(filename, "rb");
    if (f == NULL)
    {
        printf("error: Couldn't open %s\n", filename);
        exit(1);
    }

    fseek(f, 0L, SEEK_END);
    int fsize = ftell(f);
    fseek(f, 0L, SEEK_SET);

    // Allocate buffer for reading
    uint32_t *buffer = malloc(fsize);
    if (buffer == NULL)
    {
        printf("error: Couldn't allocate memory\n");
        fclose(f);
        exit(1);
    }

    // Read the file into the buffer
    fread(buffer, fsize, 1, f);
    fclose(f);

    // Convert endianness and copy to memory
    for (int i = 0; i < fsize / sizeof(uint32_t); i++)
    {
        state->mem[offset + i] = __builtin_bswap32(buffer[i]);
    }

    // Free the buffer
    free(buffer);
}

StateMIPS *InitMIPS(uint32_t mem_size, uint32_t pc_start)
{
    StateMIPS *state = calloc(1, sizeof(StateMIPS));
    state->mem = malloc(mem_size); // change to 2^32 for full 4GB address space
    state->pc = pc_start;
    return state;
}

void FreeMIPS(StateMIPS *state)
{
    free(state->mem);
    free(state);
}
