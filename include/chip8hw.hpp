/* 
* chip8hw.h
* Definition of the hardware
* Chip8hw allocates hardware out of the raspberry pi to run chip8 interpreter
*/

// Prevent multiple definitions
#ifndef __CHIP8HW_H__
#define __CHIP8HW_H__

// Includes
#include <string>
#include <cstdlib>
#include <cstring>
#include "chip8fontset.hpp"

// Defines: For use in chip8hw.cpp
#define CHIP8_MEMORY_SIZE       4096
#define CHIP8_REGISTER_COUNT    16
#define CHIP8_STACK_SIZE        16
#define CHIP8_DISPLAY_X         64
#define CHIP8_DISPLAY_Y         32
#define CHIP8_KEY_COUNT         16
#define CHIP8_FONTSET_ADDR      0x00

#define DECODE_OP(mask, shift) ((opcode & mask) >> shift)

using namespace std;

/* 
* Class chip8hw: defines the hardware for chip8 interpreter
* Private:
*   Data Members:
*       - mem: RAM for the chip8
*       - V: The 16 chip8 registers
*       - I: Index Register
*       - pc: Program Counter
*       - stack: Stack stores the return adresses to return from a procedure
*       - sp: Stack pointer stores the address up to which the stack is filled
*       - delay_timer, sound_timer: These timers count from set value to 0 at 60Hz
*       - disp: The chip8 sports a 64 * 32 display. Each array stores state of pixel
*       - key: Stores the state of keys pressed. Chip8 has a total of 16 keys
*   Member Functions:
*       - loadGame: load a game from the library
*       - emulateCycle: Fetch, decode, execute an opcode
*       - drawFlag: Gets the state of collision
*       - setKeys: At the end of a cycle, get state of each key
*/
class chip8hw {
    // Memory: RAM and registers
    u_int8_t mem[CHIP8_MEMORY_SIZE];
    u_int8_t V[CHIP8_REGISTER_COUNT];
    
    // Special Registers: Index register and program counter
    u_int16_t I;
    u_int16_t pc;

    // Stack: Stack memory and pointer
    u_int16_t stack[CHIP8_STACK_SIZE];
    u_int16_t sp;

    // Timers
    u_int8_t delay_timer;
    u_int8_t sound_timer;

    // Miscellaneous
    u_int8_t disp[CHIP8_DISPLAY_X * CHIP8_DISPLAY_Y];
    u_int8_t key[CHIP8_KEY_COUNT]; // u_int8_t key;

public:
    //Constructor
    chip8hw() {
        memset(mem, 0, sizeof(mem));
        memset(V, 0, sizeof(V));
        memset(disp, 0, sizeof(disp));
        memset(key, 0, sizeof(key));
        memset(stack, 0, sizeof(stack));

        pc = 0x200;
        I = 0;
        sp = 0;
        delay_timer = 0;
        sound_timer = 0;

        // Loading fonts into memory
        for (u_int8_t i = 0; i < 80; i++) {
            mem[i] = chip8_fontset[i];
        }    
    }

    void memDump(void);
    void dumpRegs(void);
    void loadGame(string gameLocation);
    void emulateCycle(void);
    void setKeys(u_int8_t key, int x, int y);
    void resetKeys(u_int8_t key, int x, int y);
    void dispClear(void);
    bool drawFlag(void);
    u_int8_t getKeyPressed(void);
};

#endif