#include "tui.h"

/// @brief The memory address to display.
int memory_address = 0;

/**
 * Creates a new window based on parameters.
 */
WINDOW *create_win(int height, int width, int starty, int startx)
{
    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx);
    // box(local_win, 0, 0);

    return local_win;
}

void clear_output(WINDOW *win)
{
    for (int i = 0; i < 6; i++)
    {
        wmove(win, OUTPUT_LINE + i, 1);
        wclrtoeol(win);
    }

    wrefresh(win);
}

/// @brief Checks if the address is valid and a multiple of 4.
/// @param win
/// @param address
/// @return
int address_check(WINDOW *win, int address)
{
    if (address % 4 != 0)
    {
        mvwprintw(win, OUTPUT_LINE, 1, "Address must be a multiple of 4");
        wrefresh(win);
        return -1;
    }
    if (address < 0 || address > MEM_SIZE)
    {
        mvwprintw(win, OUTPUT_LINE, 1, "Invalid address");
        wrefresh(win);
        return -1;
    }

    return address;
}

void jump_to_instruction(WINDOW *win, StateMIPS *state)
{
    int address;
    echo();
    mvwprintw(win, OUTPUT_LINE, 1, "Enter address (hex): ");
    wrefresh(win);
    wscanw(win, "%x", &address);
    noecho();

    address = address_check(win, address);

    if (address == -1)
        return;

    state->pc = address;
    mvwprintw(win, OUTPUT_LINE, 1, "Jumped to address 0x%08x", address);

    print_memory(win, state);
    wrefresh(win);
}

void jump_to_memory(WINDOW *win)
{
    int address;
    echo();
    mvwprintw(win, OUTPUT_LINE, 1, "Enter address (hex): ");
    wrefresh(win);
    wscanw(win, "%x", &address);
    noecho();

    address = address_check(win, address);

    if (address == -1)
        return;

    memory_address = address;

    mvwprintw(win, OUTPUT_LINE, 1, "Jumped to address 0x%08x", address);
    wrefresh(win);
}

void load_file(WINDOW *win, StateMIPS *state)
{
    char filename[100];
    int address;

    echo();
    mvwprintw(win, OUTPUT_LINE, 1, "Enter filename: ");
    wrefresh(win);
    wgetnstr(win, filename, 100);
    clear_output(win);

    mvwprintw(win, OUTPUT_LINE, 1, "Enter address (hex): ");
    wrefresh(win);
    wscanw(win, "%x", &address);
    noecho();
    clear_output(win);

    address = address_check(win, address);

    if (address == -1)
        return;

    int res = read_file_into_mem_at(state, filename, address);
    if (res == 0)
    {
        mvwprintw(win, OUTPUT_LINE, 1, "File %s loaded successfully at 0x%08x", filename, address);
    }
    else
    {
        mvwprintw(win, OUTPUT_LINE, 1, "Error loading file");
    }

    // Check if the memory address is within the view, if not, update it
    if (address > memory_address + MEM_VIEW_SIZE * 4 || address < memory_address)
    {
        memory_address = address;
    }

    print_memory(win, state);
    wrefresh(win);
}

void print_help(WINDOW *win)
{
    mvwprintw(win, OUTPUT_LINE, 1, "n: Next instruction");
    mvwprintw(win, OUTPUT_LINE + 1, 1, "l: Load file");
    mvwprintw(win, OUTPUT_LINE + 2, 1, "j: Jump to instruction");
    mvwprintw(win, OUTPUT_LINE + 3, 1, "m: Jump to memory");
    mvwprintw(win, OUTPUT_LINE + 4, 1, "h: Help");
    mvwprintw(win, OUTPUT_LINE + 5, 1, "q: Quit");
    wrefresh(win);
}

int handle_input(WINDOW *win, StateMIPS *state)
{
    int ch = wgetch(win);
    clear_output(win);
    switch (ch)
    {
    case 'n':
        return 1;
    case 'j':
        jump_to_instruction(win, state);
        return 0;
    case 'm':
        jump_to_memory(win);
        return 0;
    case 'l':
        load_file(win, state);
        return 0;
    case 'h':
        print_help(win);
        return 0;
    case 'q':
    case 'Q':
        return -1;
    case 's':
        // Change the memory address to the next 4 bytes if possible
        memory_address += MEM_VIEW_STEP;
        mvwprintw(win, OUTPUT_LINE, 1, "Moved down, memory address: 0x%08x", memory_address);
        return 0;
    case 'w':
        // Change the memory address to the previous 4 bytes if possible
        memory_address -= MEM_VIEW_STEP;
        mvwprintw(win, OUTPUT_LINE, 1, "Moved up, memory address: 0x%08x", memory_address);
        return 0;
    }

    return 0;
}

void print_registers(WINDOW *win, StateMIPS *state)
{
    mvwprintw(win, REG_ROW_LOC, REG_COL_LOC, "Registers:");
    for (int i = 0; i < 32; i++)
    {
        mvwprintw(win, i + REG_ROW_LOC + 1, REG_COL_LOC, "$%s:\t 0x%08x", get_reg_name(i), state->regs[i]);
    }
}

void print_memory(WINDOW *win, StateMIPS *state)
{

    if (memory_address < 0)
    {
        memory_address = 0;
    }
    else if (memory_address + MEM_VIEW_SIZE * 4 >= MEM_SIZE || memory_address > MEM_SIZE)
    {
        memory_address = MEM_SIZE - MEM_VIEW_SIZE * 4;
    }

    mvwprintw(win, MEM_ROW_LOC, MEM_COL_LOC, "Memory:");

    for (int i = 0; i < MEM_VIEW_SIZE; i++)
    {
        // Highlight the current instruction
        if (memory_address + i * 4 == state->pc)
        {
            wattron(win, COLOR_PAIR(1));
            mvwprintw(win, i + MEM_ROW_LOC + 1, MEM_COL_LOC, "0x%08x:\t 0x%08x", memory_address + i * 4, state->mem[memory_address / 4 + i]);
            wattroff(win, COLOR_PAIR(1));
        }
        else
        {
            mvwprintw(win, i + MEM_ROW_LOC + 1, MEM_COL_LOC, "0x%08x:\t 0x%08x", memory_address + i * 4, state->mem[memory_address / 4 + i]);
        }
    }
}

void print_current_instr(WINDOW *win, StateMIPS *state)
{
    uint32_t instr = state->mem[state->pc / 4];

    wmove(win, 1, MEM_COL_LOC);
    wclrtoeol(win);

    if (instr == 0)
    {
        mvwprintw(win, 1, MEM_COL_LOC, "Current instruction: not an instruction");
        return;
    }

    Instruction i = decode_instr(instr);

    const char *mnemonic = get_mnemonic(instr);

    switch (i.format)
    {
    case R_RD_RS_RT:
        mvwprintw(win, 1, MEM_COL_LOC, "Current instruction: %s $%s, $%s, $%s", mnemonic, get_reg_name(i.r.rd), get_reg_name(i.r.rs), get_reg_name(i.r.rt));
        break;
    case I_RS_RT_I:
        mvwprintw(win, 1, MEM_COL_LOC, "Current instruction: %s $%s, $%s, 0x%04x", mnemonic, get_reg_name(i.i.rs), get_reg_name(i.i.rt), i.i.imm);
        break;
    case I_RT_I_RS:
        mvwprintw(win, 1, MEM_COL_LOC, "Current instruction: %s $%s, 0x%04x($%s)", mnemonic, get_reg_name(i.i.rt), i.i.imm, get_reg_name(i.i.rs));
        break;
    case J_I:
        mvwprintw(win, 1, MEM_COL_LOC, "Current instruction: %s 0x%08x", mnemonic, get_reg_name(i.j.target));
        break;
    default:
        mvwprintw(win, 1, MEM_COL_LOC, "Current instruction: Invalid or unrecognized instruction");
        break;
    }
}

void print_pc(WINDOW *win, StateMIPS *state)
{
    mvwprintw(win, REG_COL_LOC, 1, "PC: 0x%08x", state->pc);
}