
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

// Bits 15–12 (4 bits): The Opcode (What to do: Add, Move, Halt).
// Bits 11–8 (4 bits): The Register (Where to put it: R0, R1, R2, R3).
// Bits 7–0 (8 bits): The Immediate Value (The actual number/data).

#define OPCODE_MASK ((uint16_t)0xF000)
#define REGISTER_MASK ((uint16_t)0x0F00)
#define VALUE_MASK ((uint16_t)0x00FF)
#define MAX_PROGRAM_LEN 16


typedef enum {
    OP_HALT = 0,
    OP_LOAD = 1,
    OP_ADD = 2,
    OP_SUB = 3,
    OPCODE_COUNT = 4,
} OpCode;

char *opcode_names[OPCODE_COUNT] = {
    [OP_HALT] = "HALT",
    [OP_LOAD] = "LOAD",
    [OP_ADD] = "ADD",
    [OP_SUB] = "SUB"
};

typedef struct {
    OpCode op;
    uint8_t reg;
    uint8_t val;
} Instruction;

Instruction decode(uint16_t raw_instruction);
void printInBinary2(uint16_t x, char *name);

int main() {
    // Each 16-bit hex represents: [Opcode (4bit)][Register (4bit)][Value (8bit)]
    uint16_t program[MAX_PROGRAM_LEN] = {
        0x100A, // Instruction 1
        0x1114, // Instruction 2
        0x2005, // Instruction 3
        0x3102, // Instruction 4
        0x0000  // Instruction 5 (Halt)
    };
    size_t count = 5;

    Instruction instructions[count];
    for (size_t i = 0; i < count; ++i) {
        instructions[i] = decode(program[i]);
    }

    for (size_t i = 0; i < count; ++i) {
        char* opcode_name = NULL;
        OpCode op_code = instructions[i].op;

        if (op_code < OPCODE_COUNT)
            opcode_name = opcode_names[op_code];
        else
            opcode_name = "UNKNOWN";

        printf("[DEC] Instruction: %s | Reg: %u | Val: %u \n", opcode_name, instructions[i].reg, instructions[i].val);
    }

    return 0;
}

Instruction decode(uint16_t raw_instruction) {
    Instruction instruction = {
        .op = (raw_instruction & OPCODE_MASK) >> 12,
        .reg = (raw_instruction & REGISTER_MASK) >> 8,
        .val = (raw_instruction & VALUE_MASK),
    };
    assert(instruction.op < OPCODE_COUNT && "Invalid opcode in decode()");
    return instruction;
}

void printInBinary2(uint16_t x, char *name) {
    printf("%s: ", name);
    for (int i = 15; i >= 0 ; i--) {
        if (i == 3 || i == 7 || i == 11) putchar(' ');
        putchar('0' + ((x >> i) & 1));
    }
    printf("\n");
}