/*
* chip8hw.cpp
* Implementations and definitions for the prototypes defined in chip8hw.h
*/

// Includes
#include "../include/chip8hw.hpp"
#include <cstdlib>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

void chip8hw::dispClear(void) {
    memset(disp, 0, sizeof(disp));
}

bool chip8hw::drawFlag(void) {
    return (V[15] ? true : false);
}

void chip8hw::setKeys(u_int8_t key, int x, int y) {
    if(key == 27)    // esc
		exit(0);

	if(key == '1')	    chip8hw::key[0x1] = 1;
	else if(key == '2')	chip8hw::key[0x2] = 1;
	else if(key == '3')	chip8hw::key[0x3] = 1;
	else if(key == '4')	chip8hw::key[0xC] = 1;

	else if(key == 'q')	chip8hw::key[0x4] = 1;
	else if(key == 'w')	chip8hw::key[0x5] = 1;
	else if(key == 'e')	chip8hw::key[0x6] = 1;
	else if(key == 'r')	chip8hw::key[0xD] = 1;

	else if(key == 'a')	chip8hw::key[0x7] = 1;
	else if(key == 's')	chip8hw::key[0x8] = 1;
	else if(key == 'd')	chip8hw::key[0x9] = 1;
	else if(key == 'f')	chip8hw::key[0xE] = 1;

	else if(key == 'z')	chip8hw::key[0xA] = 1;
	else if(key == 'x')	chip8hw::key[0x0] = 1;
	else if(key == 'c')	chip8hw::key[0xB] = 1;
	else if(key == 'v')	chip8hw::key[0xF] = 1;
}

void chip8hw::resetKeys(u_int8_t key, int x, int y) {

	if(key == '1')	    chip8hw::key[0x1] = 0;
	else if(key == '2')	chip8hw::key[0x2] = 0;
	else if(key == '3')	chip8hw::key[0x3] = 0;
	else if(key == '4')	chip8hw::key[0xC] = 0;

	else if(key == 'q')	chip8hw::key[0x4] = 0;
	else if(key == 'w')	chip8hw::key[0x5] = 0;
	else if(key == 'e')	chip8hw::key[0x6] = 0;
	else if(key == 'r')	chip8hw::key[0xD] = 0;

	else if(key == 'a')	chip8hw::key[0x7] = 0;
	else if(key == 's')	chip8hw::key[0x8] = 0;
	else if(key == 'd')	chip8hw::key[0x9] = 0;
	else if(key == 'f')	chip8hw::key[0xE] = 0;

	else if(key == 'z')	chip8hw::key[0xA] = 0;
	else if(key == 'x')	chip8hw::key[0x0] = 0;
	else if(key == 'c')	chip8hw::key[0xB] = 0;
	else if(key == 'v')	chip8hw::key[0xF] = 0;
}

void chip8hw::dumpRegs(void) {
    cout << "V Regs: " << endl;
    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < 8; i++)
            cout << ((j*8)+i) << ": " << hex << (int)V[(j*8)+i] << " " << dec;
        cout << endl;
    }
    cout << "I: " << hex << (int) I << dec << endl;
    cout << "pc: " << hex << (int) pc << dec << endl;
    cout << "delay_timer: " << hex << (int) delay_timer << dec << endl;
    cout << "sound_timer: " << hex << (int) sound_timer << dec << endl << endl;

    cout << "sp: " << hex << (int) sp << dec << endl;
    cout << "Stack: " << endl;
    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < 8; i++)
            cout << ((j*8)+i) << ": " << hex << (int)stack[(j*8)+i] << " " << dec;
        cout << endl;
    }
}

void chip8hw::memDump(void) {
    for (int j = 0; j < 128; j++) {
        for (int i = 0; i < 32; i++)
            cout << hex << (int)mem[(j*8)+i] << " " << dec;
        cout << endl;
    }
}

void chip8hw::emulateCycle(void) {
    // Fetch opcode
    u_int16_t opcode = (mem[pc] << 8) | mem[pc + 1];
    cout << hex << (int) opcode << endl;

    u_int8_t sep;
    u_int16_t regref, rev_regref;

    u_int16_t drawX, drawY;
    u_int8_t pixel;

    // Decode and Execute opcode
    switch (DECODE_OP(0xF000, 12)) {
        case 0x0:
            if (opcode == 0x00E0) {
                dispClear();
                pc += 2;
            }
            else if (opcode == 0x00EE) {
                pc = stack[--sp];
                pc += 2;
            }
            else {
                stack[sp++] = pc;
                pc = DECODE_OP(0x0FFF, 0);
            }
            break;
        
        case 0x1:
            pc = DECODE_OP(0x0FFF, 0);
            break;
        
        case 0x2:
            stack[sp++] = pc;
            pc = DECODE_OP(0x0FFF, 0);
            break;
        
        case 0x3:
            pc = (V[DECODE_OP(0x0F00, 8)] == DECODE_OP(0x00FF, 0)) ?
                    pc + 4 : pc + 2;
            break;

        case 0x4:
            pc = (V[DECODE_OP(0x0F00, 8)] != DECODE_OP(0x00FF, 0)) ?
                    pc + 4 : pc + 2;
            break;
        
        case 0x5:
            pc = (V[DECODE_OP(0x0F00, 8)] == V[DECODE_OP(0x00F0, 4)]) ?
                    pc + 4 : pc + 2;
            break;
        
        case 0x6:
            V[DECODE_OP(0x0F00, 8)] = DECODE_OP(0x00FF, 0);
            pc += 2;
            break;
        
        case 0x7:
            V[DECODE_OP(0x0F00, 8)] += DECODE_OP(0x00FF, 0);
            pc += 2;
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
                    V[0xF] = (V[DECODE_OP(0x0F00, 8)] > (0xFF - V[DECODE_OP(0x00F0, 4)])) ? 1 : 0;
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
                    cout << "ERROR: Unable to decode opcode " << opcode << endl;
                    break;
            }
            pc += 2;
            break;

        case 0x9:
            pc = (V[DECODE_OP(0x0F00, 8)] == V[DECODE_OP(0x00F0, 4)]) ?
                    pc + 4 : pc + 2;
            break;

        case 0xA:
            I = DECODE_OP(0x0FFF, 0);
            pc += 2;
            break;

        case 0xB:
            pc = V[0] + DECODE_OP(0x0FFF, 0);
            break;
        
        case 0xC:
            srand(time(NULL));
            V[DECODE_OP(0x0F00, 8)] = rand() & DECODE_OP(0x00FF, 0);
            pc += 2;
            break;

        case 0xD:
            drawX = V[DECODE_OP(0x0F00, 8)];
            drawY = V[DECODE_OP(0x00F0, 4)];
            V[0xF] = 0;

            for (u_int8_t yline = 0; yline < DECODE_OP(0x000F, 0); yline++) {
                pixel = mem[I+yline];

                for (u_int8_t xline = 0; xline < 8; xline++) {
                    if (pixel & (0x80 >> xline)) {
                        if (disp[drawX + xline + ((drawY + yline) * 64)] == 1)
                            V[0xF] = 1;
                        disp[drawX + xline + ((drawY + yline) * 64)] ^= 1;
                    }
                }
            }
            pc += 2;
            break;

        case 0xE:
            if (DECODE_OP(0x000F, 0) == 0xE)
                pc = (key[V[DECODE_OP(0x0F00, 8)]]) ? pc + 4 : pc + 2;
            else
                pc = (!key[V[DECODE_OP(0x0F00, 8)]]) ? pc + 4 : pc + 2;
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
                    I = CHIP8_FONTSET_ADDR + (5 * V[DECODE_OP(0x0F00, 8)]);
                    for (int i = 0; i < 5; i++)
                        cout << hex << (int) (I+i) << " ";
                    cout << endl;
                    break;

                case 0x3:
                    regref = V[DECODE_OP(0x0F00, 8)];
                    
                    mem[I] = (u_int8_t) ((regref % 1000) / 100);
                    mem[I+1] = (u_int8_t) ((regref % 100) / 10);
                    mem[I+2] = (u_int8_t) ((regref % 100) % 10);

                    break;

                default:
                    cout << "ERROR: Unable to resolve opcode" << endl;
                    break;
            }
            pc += 2;
            break;
        }
        
        if (delay_timer > 0) delay_timer--;
        if (sound_timer > 0) {
            if (sound_timer == 1) cout << "BEEP!" << endl;
            sound_timer--;
        }
}

u_int8_t chip8hw::getKeyPressed(void) {
    return 0x1;
}

void chip8hw::loadGame(string gameLocation) {
    fstream fptr;
    fptr.open(gameLocation, ios::in);

    if (!fptr)
        cout << "ERROR: No such rom found @ " << gameLocation << "." << endl;
    else {
        u_int8_t buffer;
        u_int16_t counter = 0;

        while (!fptr.eof()) {
            buffer = fptr.get();
            mem[0x200 + counter++] = buffer;
        }

        fptr.close();
    }
}