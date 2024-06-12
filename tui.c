#include "tui.h"

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

void jump_to_instruction(WINDOW *win, StateMIPS *state)
{
    int address;
    echo();
    mvwprintw(win, OUTPUT_LINE, 1, "Enter address: ");
    wrefresh(win);
    wscanw(win, "%x", &address);
    noecho();

    if (address < 0 || address > MEM_SIZE)
    {
        mvwprintw(win, OUTPUT_LINE, 1, "Invalid address");
        wrefresh(win);
        return;
    }

    state->pc = address;
    mvwprintw(win, OUTPUT_LINE, 1, "Jumped to address 0x%08x", address);

    memory_address = address;
    print_memory(win, state);
    wrefresh(win);
}

void jump_to_memory(WINDOW *win)
{
    int address;
    echo();
    mvwprintw(win, OUTPUT_LINE, 1, "Enter address: ");
    wrefresh(win);
    wscanw(win, "%x", &address);
    noecho();

    if (address < 0 || address > MEM_SIZE)
    {
        mvwprintw(win, OUTPUT_LINE, 1, "Invalid address");
        wrefresh(win);
        return;
    }

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

    mvwprintw(win, OUTPUT_LINE, 1, "Enter address: ");
    wrefresh(win);
    wscanw(win, "%x", &address);
    noecho();
    clear_output(win);

    if (address < 0 || address > MEM_SIZE)
    {
        mvwprintw(win, OUTPUT_LINE, 1, "Invalid address");
        wrefresh(win);
        return;
    }

    int res = ReadFileIntoMemoryAt(state, filename, address);
    if (res == 0)
    {
        mvwprintw(win, OUTPUT_LINE, 1, "File %s loaded successfully at %x", filename, address);
    }
    else
    {
        mvwprintw(win, OUTPUT_LINE, 1, "Error loading file");
    }

    memory_address = address;
    print_memory(win, state);
    wrefresh(win);
}

void print_help(WINDOW *win)
{
    mvwprintw(win, OUTPUT_LINE, 1, "n: Next instruction");
    mvwprintw(win, OUTPUT_LINE + 1, 1, "j: Jump to instruction");
    mvwprintw(win, OUTPUT_LINE + 2, 1, "m: Jump to memory");
    mvwprintw(win, OUTPUT_LINE + 3, 1, "l: Load file");
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
    case 'w':
        memory_address += MEM_VIEW_STEP;
        print_memory(win, state);
        mvwprintw(win, OUTPUT_LINE, 1, "Moved up, memory address: %d", memory_address);
        return 0;
    case 's':
        memory_address -= MEM_VIEW_STEP;
        print_memory(win, state);
        mvwprintw(win, OUTPUT_LINE, 1, "Moved down, memory address: %d", memory_address);
        return 0;
    }
}

void print_registers(WINDOW *win, StateMIPS *state)
{
    mvwprintw(win, REG_ROW_LOC, REG_COL_LOC, "Registers:");
    for (int i = 0; i < 32; i++)
    {
        mvwprintw(win, i + REG_ROW_LOC + 1, REG_COL_LOC, "$%02d: 0x%08x", i, state->regs[i]);
    }
}

void print_memory(WINDOW *win, StateMIPS *state)
{

    if (memory_address < 0)
    {
        memory_address = 0;
    }
    if (memory_address + MEM_VIEW_SIZE >= MEM_SIZE || memory_address > MEM_SIZE)
    {
        memory_address = MEM_SIZE - MEM_VIEW_SIZE;
    }

    mvwprintw(win, MEM_ROW_LOC, MEM_COL_LOC, "Memory:");

    for (int i = 0; i < MEM_VIEW_SIZE; i++)
    {
        if (i >= MEM_SIZE)
        {
            break;
        }
        if (memory_address + i == state->pc)
        {
            wattron(win, COLOR_PAIR(1));
            mvwprintw(win, i + MEM_ROW_LOC + 1, MEM_COL_LOC, "0x%08x: 0x%08x", memory_address + i*4, state->mem[memory_address + i]);
            wattroff(win, COLOR_PAIR(1));
        } else {
            mvwprintw(win, i + MEM_ROW_LOC + 1, MEM_COL_LOC, "0x%08x: 0x%08x", memory_address + i*4, state->mem[memory_address + i]);
        }
    }
}

void print_pc(WINDOW *win, StateMIPS *state)
{
    mvwprintw(win, 1, 1, "PC: 0x%08x", state->pc);
}