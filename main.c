#include "mips_emul.h"
#include "tui.h"

int main()
{
    // ncurses options
    initscr(); /* Start curses mode 		  */
    curs_set(0);
    noecho();
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);

    StateMIPS *state = init_mips(0x0);

    WINDOW *win = create_win(50, 160, 0, 0);

    int op = 0;
    int ret = 0;

    print_help(win);
    do
    {
        if (op == 1)
        {
            ret = emulate_mips(state);
        }

        print_pc(win, state);
        print_current_instr(win, state);
        print_registers(win, state);
        print_memory(win, state);
        wrefresh(win);

        op = handle_input(win, state);
        wclear(win);
    } while (op != -1);

    free_mips(state);

    endwin(); /* End curses mode		  */
    return 0;
}