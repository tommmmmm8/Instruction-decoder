
#include <stdint.h>
#include <stdio.h>

typedef enum {
    OP_HALT = 0,
    OP_LOAD = 1,
    OP_ADD = 2,
    OP_SUB = 3,
} OpCode;

typedef struct {
    OpCode op;
    uint8_t reg;
    uint8_t val;
} Instruction;

Instruction decode(uint16_t raw_data);
void printInBinary2(uint16_t x, char *name);

int main() {
    // Each 16-bit hex represents: [Opcode (4bit)][Register (4bit)][Value (8bit)]
    uint16_t program[] = {
        0x100A, // Instruction 1
        0x1114, // Instruction 2
        0x2005, // Instruction 3
        0x3102, // Instruction 4
        0x0000  // Instruction 5 (Halt)
    };
    size_t count = sizeof(program) / sizeof(program[0]);

    Instruction instructions[count];
    for (size_t i = 0; i < count; ++i) {
        instructions[i] = decode(program[i]);
    }

    for (size_t i = 0; i < count; ++i) {
        char* op_code = NULL;
        switch (instructions[i].op) {
            case OP_HALT:
                op_code = "HALT";
                break;
            case OP_LOAD:
                op_code = "LOAD";
                break;
            case OP_ADD:
                op_code = "ADD";
                break;
            case OP_SUB:
                op_code = "SUB";
                break;
        }
        printf("[DEC] Instruction: %s | Reg: %u | Val: %u \n", op_code, instructions[i].reg, instructions[i].val);
    }

    return 0;
}

Instruction decode(uint16_t raw_data) {
    // Bits 15–12 (4 bits): The Opcode (What to do: Add, Move, Halt).
    // Bits 11–8 (4 bits): The Register (Where to put it: R0, R1, R2, R3).
    // Bits 7–0 (8 bits): The Immediate Value (The actual number/data).

    uint16_t op_code_mask = UINT16_MAX << 12;
    uint16_t register_mask = ((1 << 4) - 1) << 8;
    uint16_t value_mask = (1 << 8) - 1;
    // printInBinary2(op_code_mask, "op_code_mask");
    // printInBinary2(register_mask, "register_mask");
    // printInBinary2(value_mask, "value_mask");

    Instruction instruction = {
        .op = (raw_data & op_code_mask) >> 12,
        .reg = (raw_data & register_mask) >> 8,
        .val = (raw_data & value_mask)
    };
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