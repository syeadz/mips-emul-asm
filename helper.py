def r_instruction_to_binary(opcode, rs, rt, rd, shamt, funct):
    # Convert the opcode to binary
    opcode_binary = f"{opcode:06b}"

    # Convert the rs register to binary
    rs_binary = f"{rs:05b}"

    # Convert the rt register to binary
    rt_binary = f"{rt:05b}"

    # Convert the rd register to binary
    rd_binary = f"{rd:05b}"

    # Convert the shamt to binary
    shamt_binary = f"{shamt:05b}"

    # Convert the funct to binary
    funct_binary = f"{funct:06b}"

    # Concatenate the binary strings
    binary_str = (
        opcode_binary + rs_binary + rt_binary + rd_binary + shamt_binary + funct_binary
    )

    # Return the binary string
    return binary_str


def i_instruction_to_binary(opcode, rs, rt, immediate):
    # Convert the opcode to binary
    opcode_binary = f"{opcode:06b}"

    # Convert the rs register to binary
    rs_binary = f"{rs:05b}"

    # Convert the rt register to binary
    rt_binary = f"{rt:05b}"

    # Convert the immediate to binary
    immediate_binary = f"{immediate:016b}"

    # Concatenate the binary strings
    binary_str = opcode_binary + rs_binary + rt_binary + immediate_binary

    # Return the binary string
    return binary_str


def j_instruction_to_binary(opcode, address):
    # Convert the opcode to binary
    opcode_binary = f"{opcode:06b}"

    # Convert the address to binary
    address_binary = f"{address:026b}"

    # Concatenate the binary strings
    binary_str = opcode_binary + address_binary

    # Return the binary string
    return binary_str


def binary_to_r_instruction(binary_str):
    # Get the opcode from the binary string
    opcode = int(binary_str[0:6], 2)

    # Get the rs register from the binary string
    rs = int(binary_str[6:11], 2)

    # Get the rt register from the binary string
    rt = int(binary_str[11:16], 2)

    # Get the rd register from the binary string
    rd = int(binary_str[16:21], 2)

    # Get the shamt from the binary string
    shamt = int(binary_str[21:26], 2)

    # Get the funct from the binary string
    funct = int(binary_str[26:32], 2)

    # Return the decoded instruction
    return opcode, rs, rt, rd, shamt, funct


def binary_to_i_instruction(binary_str):
    # Get the opcode from the binary string
    opcode = int(binary_str[0:6], 2)

    # Get the rs register from the binary string
    rs = int(binary_str[6:11], 2)

    # Get the rt register from the binary string
    rt = int(binary_str[11:16], 2)

    # Get the immediate from the binary string
    immediate = int(binary_str[16:32], 2)

    # Return the decoded instruction
    return opcode, rs, rt, immediate


def binary_to_j_instruction(binary_str):
    # Get the opcode from the binary string
    opcode = int(binary_str[0:6], 2)

    # Get the address from the binary string
    address = int(binary_str[6:32], 2)

    # Return the decoded instruction
    return opcode, address
