#pragma once

#include "chip8_t.h"

typedef void (*OpFuncPtr)(chip8_t*, uint16_t);

OpFuncPtr get_instruction(uint16_t opcode);

void op_00E0(chip8_t* cpu, uint16_t opcode);  // Clear Screen
void op_00EE(chip8_t* cpu, uint16_t opcode);  // Return from Subroutine
void op_1NNN(chip8_t* cpu, uint16_t opcode);  // Jump to Address
void op_2NNN(chip8_t* cpu, uint16_t opcode);  // Call Subroutine
void op_3XNN(chip8_t* cpu, uint16_t opcode);  // Skip if Equal
void op_4XNN(chip8_t* cpu, uint16_t opcode);  // Skip if Not Equal
void op_5XY0(chip8_t* cpu, uint16_t opcode);  // Skip if Registers Equal
void op_6XNN(chip8_t* cpu, uint16_t opcode);  // Load Value into Register
void op_7XNN(chip8_t* cpu, uint16_t opcode);  // Add Value to Register
void op_8XY0(chip8_t* cpu, uint16_t opcode);  // Copy Register
void op_8XY1(chip8_t* cpu, uint16_t opcode);  // OR Register
void op_8XY2(chip8_t* cpu, uint16_t opcode);  // AND Register
void op_8XY3(chip8_t* cpu, uint16_t opcode);  // XOR Register
void op_8XY4(chip8_t* cpu, uint16_t opcode);  // Add Registers
void op_8XY5(chip8_t* cpu, uint16_t opcode);  // Subtract Registers
void op_8XY6(chip8_t* cpu, uint16_t opcode);  // Shift Right
void op_8XY7(chip8_t* cpu, uint16_t opcode);  // Subtract Registers (Reverse)
void op_8XYE(chip8_t* cpu, uint16_t opcode);  // Shift Left
void op_9XY0(chip8_t* cpu, uint16_t opcode);  // Skip if Registers Not Equal
void op_ANNN(chip8_t* cpu, uint16_t opcode);  // Load Address into I
void op_BNNN(chip8_t* cpu, uint16_t opcode);  // Jump to Address + V0
void op_CXNN(chip8_t* cpu, uint16_t opcode);  // Generate Random Number
void op_DXYN(chip8_t* cpu, uint16_t opcode);  // Draw Sprite
void op_EX9E(chip8_t* cpu, uint16_t opcode);  // Skip if Key Pressed
void op_EXA1(chip8_t* cpu, uint16_t opcode);  // Skip if Key Not Pressed
void op_FX07(chip8_t* cpu, uint16_t opcode);  // Load Delay Timer into Register
void op_FX0A(chip8_t* cpu, uint16_t opcode);  // Wait for Key Press
void op_FX15(chip8_t* cpu, uint16_t opcode);  // Set Delay Timer
void op_FX18(chip8_t* cpu, uint16_t opcode);  // Set Sound Timer
void op_FX1E(chip8_t* cpu, uint16_t opcode);  // Add to I
void op_FX29(chip8_t* cpu, uint16_t opcode);  // Load Sprite Location
void op_FX33(chip8_t* cpu, uint16_t opcode);  // Store BCD
void op_FX55(chip8_t* cpu, uint16_t opcode);  // Store Registers
void op_FX65(chip8_t* cpu, uint16_t opcode);  // Read Registers
