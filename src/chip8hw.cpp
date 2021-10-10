/*
* chip8hw.cpp
* Implementations and definitions for the prototypes defined in chip8hw.h
*/

// Includes
#include "../include/chip8hw.hpp"
#include <cstdlib>
#include <iostream>
#include <bits/stdc++.h>

#define DECODE_OP(mask, shift) ((opcode & mask) >> shift)

using namespace std;

void chip8hw::dispClear(void) {
    memset(disp, 0, sizeof(disp));
}

bool chip8hw::drawFlag(void) {
    return (V[15] ? true : false);
}

void chip8hw::emulateCycle(void) {
    // Fetch opcode
    u_int16_t opcode = (mem[pc] << 8) | mem[pc + 1];

    u_int8_t sep;
    u_int16_t regref, rev_regref;

    // Decode and Execute opcode
    switch (DECODE_OP(0xF000, 12)) {
        case 0x0:
            if (opcode == 0x00E0){
                dispClear();
                pc++;
            }
            else if (opcode == 0x00EE)
                pc = stack[sp--];
            else {
                stack[++sp] = (pc + 1);
                pc = DECODE_OP(0x0FFF, 0);
            }
            break;
        
        case 0x1:
            pc = DECODE_OP(0x0FFF, 0);
            pc++;
            break;
        
        case 0x2:
            stack[++sp] = pc;
            pc = DECODE_OP(0x0FFF, 0);
            pc++;
            break;
        
        case 0x3:
            pc = (V[DECODE_OP(0x0F00, 8)] == DECODE_OP(0x00FF, 0)) ?
                    pc + 2 : pc + 1;
            break;

        case 0x4:
            pc = (V[DECODE_OP(0x0F00, 8)] != DECODE_OP(0x00FF, 0)) ?
                    pc + 2 : pc + 1;
            break;
        
        case 0x5:
            pc = (V[DECODE_OP(0x0F00, 8)] == V[DECODE_OP(0x00F0, 4)]) ?
                    pc + 2 : pc + 1;
            break;
        
        case 0x6:
            V[DECODE_OP(0x0F00, 8)] = DECODE_OP(0x00FF, 0);
            break;
        
        case 0x7:
            V[DECODE_OP(0x0F00, 8)] += DECODE_OP(0x00FF, 0);
            break;

        case 0x8:
            switch (DECODE_OP(0x000F, 0))
            {
                case 0x0:
                    V[DECODE_OP(0x0F00, 8)] = V[DECODE_OP(0x00F0, 4)];
                    break;

                case 0x1:
                    V[DECODE_OP(0x0F00, 8)] |= V[DECODE_OP(0x00F0, 4)];
                    break;

                case 0x2:
                    V[DECODE_OP(0x0F00, 8)] &= V[DECODE_OP(0x00F0, 4)];
                    break;

                case 0x3:
                    V[DECODE_OP(0x0F00, 8)] ^= V[DECODE_OP(0x00F0, 4)];
                    break;

                case 0x4:
                    V[DECODE_OP(0x0F00, 8)] += V[DECODE_OP(0x00F0, 4)];
                    break;

                case 0x5:
                    V[DECODE_OP(0x0F00, 8)] -= V[DECODE_OP(0x00F0, 4)];
                    break;

                case 0x6:
                    V[15] = V[DECODE_OP(0x0F00, 8)] & 0x1;
                    V[DECODE_OP(0x0F00, 8)] >>= 1;
                    break;
                
                case 0x7:
                    V[DECODE_OP(0x0F00, 8)] = V[DECODE_OP(0x00F0, 4)] - V[DECODE_OP(0x0F00, 8)];
                    break;

                case 0xE:
                    V[15] = (V[DECODE_OP(0x0F00, 8)] & 0x8000) >> 15;
                    V[DECODE_OP(0x0F00, 8)] <<= 1;
                    break;

                default:
                    break;
            }
            break;

        case 0x9:
            pc = (V[DECODE_OP(0x0F00, 8)] == V[DECODE_OP(0x00F0, 4)]) ?
                    pc + 2 : pc + 1;
            break;

        case 0xA:
            V[DECODE_OP(0xF000, 12)] = DECODE_OP(0x0FFF, 0);
            break;

        case 0xB:
            pc = V[0] + DECODE_OP(0x0FFF, 0);
            break;
        
        case 0xC:
            srand(time(NULL));
            V[DECODE_OP(0x0F00, 8)] = rand() & DECODE_OP(0x00FF, 0);
            break;

        case 0xD:
            // Draw Graphics
            break;

        case 0xE:
            if (DECODE_OP(0x000F, 0) == 0xE)
                pc = (key[V[DECODE_OP(0x0F00, 8)]]) ? pc + 2 : pc + 1;
            else
                pc = (!key[V[DECODE_OP(0x0F00, 8)]]) ? pc + 2 : pc + 1;
            break;

        case 0xF:
            switch (DECODE_OP(0x000F, 0)) {
                case 0x7:
                    V[DECODE_OP(0x000F, 0)] = delay_timer;
                    break;
                
                case 0xA:
                    while(!(V[DECODE_OP(0x0F00, 8)] = (u_int16_t) getKeyPressed()));
                    break;

                case 0x5:
                    sep = DECODE_OP(0x00F0, 4);
                    if (sep == 0x1) {
                        delay_timer = V[DECODE_OP(0x0F00, 8)];
                    }
                    else {
                        u_int8_t X = DECODE_OP(0x0F00, 8);
                        for (u_int8_t offset = 0; offset <= X; offset++) {
                            (sep == 0x5) ?
                                mem[(I++)] = V[offset] :
                                V[offset] = mem[(I++)];
                        }
                    }
                    break;
                    
                case 0x8:
                    sound_timer = V[DECODE_OP(0x0F00, 8)];
                    break;

                case 0xE:
                    I += V[DECODE_OP(0x0F00, 8)];
                    break;

                case 0x9:
                    // Set I as sprite addr stored in Vx
                    break;

                case 0x3:
                    regref = V[DECODE_OP(0x0F00, 8)];
                    rev_regref = 0;
                    while (regref > 0) {
                        rev_regref = rev_regref * 10 + regref % 10;
                        regref = regref / 10;
                    }
                    for (u_int8_t off = 0; off < 3; off++) {
                        mem[(I+off)] = (rev_regref % 10);
                        rev_regref /= 10;
                    }
                    break;

                default:
                    cout << "ERROR: Unable to resolve opcode" << endl;
                    break;
            }

            if (delay_timer > 0) delay_timer--;
            if (sound_timer > 0) {
                if (sound_timer == 1) cout << "BEEP!" << endl;
                sound_timer--;
            }
    }

}

u_int8_t chip8hw::getKeyPressed(void) {
    return 0x1;
}