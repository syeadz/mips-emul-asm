#include "mips_emul.h"

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

    uint32_t *buffer = &state->mem[offset];
    fread(buffer, fsize, 1, f);
    fclose(f);
}

StateMIPS *InitMIPS(void)
{
    StateMIPS *state = calloc(1, sizeof(StateMIPS));
    state->mem = malloc(0x10000); // change to 2^32 for full 4GB address space
    return state;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("usage: %s <filename>\n", argv[0]);
        exit(1);
    }
    
    int done = 0;
    StateMIPS *state = InitMIPS();

    ReadFileIntoMemoryAt(state, "invaders.h", 0);
    ReadFileIntoMemoryAt(state, "invaders.g", 0x800);
    ReadFileIntoMemoryAt(state, "invaders.f", 0x1000);
    ReadFileIntoMemoryAt(state, "invaders.e", 0x1800);

    while (done == 0)
    {
        done = EmulateMIPSp(state);
    }

    return 0;
}