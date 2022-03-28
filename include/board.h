#pragma once

#include <iostream>
#include "raylib.h"

#define sout std::cout
#define endl std::endl;
#define br break;

enum Entity : unsigned short {air, I, L, J, O, S, Z, T, sI = 11, sL, sJ, sO, sS, sZ, sT, floor};
                           //  0   1  2  3  4  5  6  7  11       12  13  14  15  16  17   18

Entity buffer1, buffer2, buffer3, buffer4;
bool isDeleted, haveAir;
int linesDeleted;

void reset(Entity* entity) // (loop) initialize the board to all air
{
    for (int index = 0; index < 220; ++index)
        *(entity + index) = air;
    for (int index = 220; index < 230; ++index)
        *(entity + index) = floor;
}

bool needChangeState(Entity& entity) // (switch) change a static block to solid when it meet a solid
{
    if (entity != 0 && *(&entity+10) > 10 && entity < 10 && entity)
        return true;
    return false;
}

void changeAllState(Entity* entity) // (loop and switch) change all static block to solid
{
    for (int index = 0; index < 220; ++index)
    {
        switch (*(entity + index))
        {
            case I: *(entity + index) = sI;
            br
            case L: *(entity + index) = sL;
            br
            case J: *(entity + index) = sJ;
            br
            case O: *(entity + index) = sO;
            br
            case S: *(entity + index) = sS;
            br
            case Z: *(entity + index) = sZ;
            br
            case T: *(entity + index) = sT;
            br
            default: br
        }
    }
}

void print(const Entity& entity) //just for debug
{
    switch (entity)
    {
        case air: sout << "air" << endl br
        case I: sout << "I" << endl br
        case L: sout << "L" << endl br
        case J: sout << "J" << endl br
        case O: sout << "O" << endl br
        case S: sout << "S" << endl br
        case Z: sout << "Z" << endl br
        case T: sout << "T" << endl br
        case sI: sout << "sI" << endl br
        case sL: sout << "sL" << endl br
        case sJ: sout << "sJ" << endl br
        case sO: sout << "sO" << endl br
        case sS: sout << "sS" << endl br
        case sZ: sout << "sZ" << endl br
        case sT: sout << "sT" << endl br
        case floor: sout << "floor" << endl
    }
}

void moving(Entity* entity, int move, Entity& final_target)
{
    switch (*entity)
    {
        case I: 
        *(entity + move) = final_target;
        br
        case L: 
        *(entity + move) = final_target;
        br
        case J: 
        *(entity + move) = final_target;
        br
        case O: 
        *(entity + move) = final_target;
        br
        case S: 
        *(entity + move) = final_target;
        br
        case Z: 
        *(entity + move) = final_target;
        br
        case T: 
        *(entity + move) = final_target;
        br
    }
}

bool moveStatic(Entity* entity, char move, 
                int current_index_1, int current_index_2, 
                int current_index_3, int current_index_4) // (loop and switch) move a static block
{
    bool canMove = true;
    if (move == 1)
    {
        if (!(current_index_1 % 10 == 0 || current_index_2 % 10 == 0 || current_index_3 % 10 == 0 || current_index_4 % 10 == 0))
        {
            if (!((current_index_1 % 10) % 9))
            canMove = false;
            else if (!((current_index_2 % 10) % 9))
            canMove = false;
            else if (!((current_index_3 % 10) % 9))
            canMove = false;
            else if (!((current_index_4 % 10) % 9))
            canMove = false;
        }
        if (*(entity + current_index_1 + 1) > 10 || *(entity + current_index_2 + 1) > 10 || 
            *(entity + current_index_3 + 1) > 10 || *(entity + current_index_4 + 1) > 10)
        {
            canMove = false;
        }
    }
    else if (move == -1)
    {
        if (!(current_index_1 % 10) || *(entity + current_index_1 - 1) > 10)
            canMove = false;
        else if (!(current_index_2 % 10) || *(entity + current_index_2 - 1) > 10)
            canMove = false;
        else if (!(current_index_3 % 10) || *(entity + current_index_3 - 1) > 10)
            canMove = false;
        else if (!(current_index_4 % 10) || *(entity + current_index_4 - 1) > 10)
            canMove = false;
    }
    else if (move == 10)
    {
        if (*(entity + current_index_1 + 10) > 10)
            canMove = false;
        else if (*(entity + current_index_2 + 10) > 10)
            canMove = false;
        else if (*(entity + current_index_3 + 10) > 10)
            canMove = false;
        else if (*(entity + current_index_4 + 10) > 10)
            canMove = false;
    }
    
    if (canMove)
    {
        buffer1 = *(entity + current_index_1),
        buffer2 = *(entity + current_index_2),
        buffer3 = *(entity + current_index_3),
        buffer4 = *(entity + current_index_4);

        moving(entity + current_index_1, move, buffer1);
        moving(entity + current_index_2, move, buffer2);
        moving(entity + current_index_3, move, buffer3);
        moving(entity + current_index_4, move, buffer4);

        if (current_index_1 != current_index_1 + move && current_index_1 != current_index_2 + move && current_index_1 != current_index_3 + move && current_index_1 != current_index_4 + move)
            *(entity + current_index_1) = air;
        if (current_index_2 != current_index_1 + move && current_index_2 != current_index_2 + move && current_index_2 != current_index_3 + move && current_index_2 != current_index_4 + move)
            *(entity + current_index_2) = air;
        if (current_index_3 != current_index_1 + move && current_index_3 != current_index_2 + move && current_index_3 != current_index_3 + move && current_index_3 != current_index_4 + move)
            *(entity + current_index_3) = air;
        if (current_index_4 != current_index_1 + move && current_index_4 != current_index_2 + move && current_index_4 != current_index_3 + move && current_index_4 != current_index_4 + move)
            *(entity + current_index_4) = air;
    }
    return canMove;
}

void spawnBlock(Entity& entity, int value) // (switch) spawn block
{
    switch (value)
    {
        case 1: //I - 1
            *(&entity + 15) = I;
            *(&entity + 25) = I;
            *(&entity + 35) = I;
            *(&entity + 45) = I; br
        case 2: // I - 2
            *(&entity + 13) = I;
            *(&entity + 14) = I;
            *(&entity + 15) = I;
            *(&entity + 16) = I; br
        case 3: //L - 1
            *(&entity + 5) = L;
            *(&entity + 15) = L;
            *(&entity + 25) = L;
            *(&entity + 4) = L; br
        case 4: //L - 2
            *(&entity + 5) = L;
            *(&entity + 13) = L;
            *(&entity + 14) = L;
            *(&entity + 15) = L; br
        case 5: //L - 3
            *(&entity + 4) = L;
            *(&entity + 14) = L;
            *(&entity + 24) = L;
            *(&entity + 25) = L; br
        case 6: //L - 4
            *(&entity + 4) = L;
            *(&entity + 5) = L;
            *(&entity + 6) = L;
            *(&entity + 14) = L; br
        case 7: //J - 1
            *(&entity + 4) = J;
            *(&entity + 14) = J;
            *(&entity + 24) = J;
            *(&entity + 5) = J; br
        case 8: //J - 2
            *(&entity + 4) = J;
            *(&entity + 14) = J;
            *(&entity + 15) = J;
            *(&entity + 16) = J; br
        case 9: //J - 3
            *(&entity + 5) = J;
            *(&entity + 15) = J;
            *(&entity + 25) = J;
            *(&entity + 24) = J; br
        case 10: //J - 4
            *(&entity + 4) = J;
            *(&entity + 5) = J;
            *(&entity + 6) = J;
            *(&entity + 16) = J; br
        case 11: // O
            *(&entity + 4) = O;
            *(&entity + 5) = O;
            *(&entity + 14) = O;
            *(&entity + 15) = O; br
        case 12: //S - 1
            *(&entity + 14) = S;
            *(&entity + 15) = S;
            *(&entity + 5) = S;
            *(&entity + 6) = S; br
        case 13: //S - 2
            *(&entity + 4) = S;
            *(&entity + 14) = S;
            *(&entity + 15) = S;
            *(&entity + 25) = S; br
        case 14: //Z - 1
            *(&entity + 3) = Z;
            *(&entity + 4) = Z;
            *(&entity + 14) = Z;
            *(&entity + 15) = Z; br
        case 15: //Z - 2
            *(&entity + 5) = Z;
            *(&entity + 15) = Z;
            *(&entity + 14) = Z;
            *(&entity + 24) = Z; br
        case 16: //T - 1
            *(&entity + 5) = T;
            *(&entity + 14) = T;
            *(&entity + 15) = T;
            *(&entity + 16) = T; br
        case 17: //T - 2
            *(&entity + 4) = T;
            *(&entity + 5) = T;
            *(&entity + 6) = T;
            *(&entity + 15) = T; br
        case 18: //T - 3
            *(&entity + 14) = T;
            *(&entity + 5) = T;
            *(&entity + 15) = T;
            *(&entity + 25) = T; br
        case 19: //T - 4
            *(&entity + 4) = T;
            *(&entity + 14) = T;
            *(&entity + 24) = T;
            *(&entity + 15) = T; br
    }       
}

bool lose(Entity& entity) //Detect if lose
{
    for (int index = 0; index < 20; ++index)
    {
        if (*(&entity + index) > 10)
        {
            return true;            
        }
    }
    return false;
}

int deleteLines(Entity* entity) //deleted fulfilled line
{
    linesDeleted = 0;
    for (int lines = 2; lines < 22; ++lines)
    {
        isDeleted = false, haveAir = false;
        for (int index = 0; index < 10; ++index)
        {
            if (*(entity + lines * 10 + index) < 10)
            {
                haveAir = true;
                break;
            }
        }
        if (!haveAir)
        {
            ++linesDeleted;
            for (int line = lines; line > 0; --line)
            {
                for (int index = 0; index < 10; ++index)
                {
                    *(entity + index + line * 10) = *(entity + index + (line - 1) * 10);
                }
            }
        }
    }
    return linesDeleted;
}