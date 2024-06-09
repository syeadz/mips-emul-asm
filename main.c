#include "mips_emul.h"

int main(int argc, char **argv)
{
    // if (argc != 2)
    // {
    //     printf("usage: %s <filename>\n", argv[0]);
    //     exit(1);
    // }
    
    int done = 0;
    StateMIPS *state = InitMIPS(0x10000, 0x0);

    ReadFileIntoMemoryAt(state, "add.bin", 0);
    ReadFileIntoMemoryAt(state, "mem.bin", 0xc);

    while (done == 0)
    {
        done = EmulateMIPSp(state);
    }

    FreeMIPS(state);
    return 0;
}