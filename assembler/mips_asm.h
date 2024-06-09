#pragma once

#include <stdlib.h>
#include <stdint.h>

#include "tokenizer.h"
#include "parser.h"
#include "utility.h"

/// @brief Reads the contents of a file into a buffer.
/// @param filename
/// @return
char *read_file(const char *filename);

/// @brief Generates the MIPS assembly code and writes it to a file.
/// @param output_filename
void generate_code(AST *ast, const char *output_filename);