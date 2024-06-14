#pragma once

#include "mips_emul.h"

// Include the correct curses header based on the operating system
#ifdef _WIN32
#include <curses.h>
#include <panel.h>
#else
#include <ncurses.h>
#endif

// Define the locations of the registers and memory in the window
#define REG_ROW_LOC 3
#define MEM_ROW_LOC 3
#define REG_COL_LOC 1
#define MEM_COL_LOC 40

// Size of the memory view, these many memory locations will be displayed
#define MEM_VIEW_SIZE 32
// Move by 4 bytes at a time when scrolling through memory
#define MEM_VIEW_STEP 4

// Output line for messages
#define OUTPUT_LINE MEM_ROW_LOC + MEM_VIEW_SIZE + 2

/// @brief Creates a new window based on parameters.
/// @param height
/// @param width
/// @param starty
/// @param startx
/// @return WINDOW*
WINDOW *create_win(int height, int width, int starty, int startx);

/// @brief Prints the MIPS registers.
/// @param win
/// @param state
void print_registers(WINDOW *win, StateMIPS *state);

/// @brief Prints the memory from a memory location.
/// @param win
/// @param state
void print_memory(WINDOW *win, StateMIPS *state);

/// @brief Prints the program counter.
/// @param win
/// @param state
void print_pc(WINDOW *win, StateMIPS *state);

/// @brief Prints the current instruction.
/// @param win
/// @param state
void print_current_instr(WINDOW *win, StateMIPS *state);

/// @brief Displays the help menu.
/// @param win
void print_help(WINDOW *win);

/// @brief Prints the instruction at a specific memory location.
/// @param win
/// @param instr
/// @param y
/// @param x
void print_instr_at(WINDOW *win, uint32_t instr, int y, int x);

// /// @brief Jumps to a specific instruction.
// /// @param win
// /// @param state
// void jump_to_instruction(WINDOW *win, StateMIPS *state);

// /// @brief Jumps to a specific memory location.
// /// @param win
// /// @param state
// void jump_to_memory(WINDOW *win, StateMIPS *state);

// /// @brief Loads a file into memory.
// /// @param win
// /// @param state
// void load_file(WINDOW *win, StateMIPS *state);

/// @brief Handles input from the user.
/// @param win
/// @param state
/// @return Returns 1 if the user wants to emulate the MIPS, -1 if the user wants to exit, and 0 otherwise.
int handle_input(WINDOW *win, StateMIPS *state);

// /// @brief Prints the help menu.
// /// @param win
// void print_help(WINDOW *win);