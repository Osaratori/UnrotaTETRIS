#include <iostream>
#include <time.h>
#include <string>

#include "raylib.h"
#include "board.h"

enum Title : unsigned char {title = 0, game, game_over, credit_screen};
enum TitleChoice : unsigned char {startgame, credit, leavegame};
enum InGameStatus : char {not_pause, is_pause};

int main(void)
{
    // Initialization
    const int screenWidth = 1000;
    const int screenHeight = 1000;
    srand(time(NULL));

    //Window
    InitWindow(screenWidth, screenHeight, "UnrotaTETRIS");
    Image gameIcon(LoadImage("asset/texture/icon.png"));
    SetWindowIcon(gameIcon);
    
    SetExitKey(0);

    //Load textures
    Texture2D 
    Block(LoadTexture("asset/texture/block.png")), 
    Board(LoadTexture("asset/texture/board.png")), 
    Background(LoadTexture("asset/texture/bg.png")), 
    TitleBG(LoadTexture("asset/texture/title.png")), 
    Box(LoadTexture("asset/texture/box.png")), 
    ThinBox(LoadTexture("asset/texture/thin box.png")), 
    NextBox(LoadTexture("asset/texture/next box.png")), 
    BlocksMap(LoadTexture("asset/texture/blocks map.png"));

    //Board base
    Entity b_state[230];
    bool needNewBlock = true, runOnce = true, needReset = false, shouldLeaveGame = false, isLose = false, setting_level_need_reset = false, canHold;
    bool _7_1 = false, _2_2 = false, _7_3 = false;
    int random = rand()%19 + 1, bg_move = 0, bg_moveCounter = 0,
        left_counter = 0, right_counter = 0, down_counter = 0, move_counter = 0, ARE = 1,                //counters
        lines = 0, Gridcell = 48, changeStateCounter = 0, justPause, score, currentBlock, holdBlock = rand()%19 + 1, //game utils
        current_index_1 = 0, current_index_2 = 0, current_index_3 = 0, current_index_4 = 0,              //index savers
        lineDestroyed = 0, levelDevideLine = 0, level = 0, setting_level = 0, holdDummy = 0,
                egg_1 = 0, egg_2 = 5;
    TitleChoice titleChoice = startgame;
    Title currentTitle = title;
    InGameStatus in_game_status = not_pause;
    std::string scoreText = "SCOREWYSI";

    //60fps Max
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose() && !shouldLeaveGame)
    {
        SetWindowSize(screenWidth, screenHeight);

        BeginDrawing();

        switch (currentTitle)
        {
            //Title screen
            case title:

                if (bg_move == 225)
                {
                    bg_move = 0;
                }

                if (setting_level_need_reset)
                {
                    setting_level = 0, setting_level_need_reset = false, egg_1 = 0, egg_2 = 5;
                }
                
                if (IsKeyPressed(KEY_SEVEN)) //???
                {
                    if (!_7_1)
                    {
                        _7_1 = true;
                    }
                }
                if (IsKeyPressed(KEY_TWO))
                {
                    if (_7_1)
                    {
                        _2_2 = true;
                    }
                }
                if (IsKeyPressed(KEY_SEVEN))
                {
                    if (_2_2)
                    {
                        _7_3 = true;
                        needReset = true;
                        currentTitle = game;
                    }
                }

                DrawTexture(TitleBG, 0, 0, WHITE);

                switch (titleChoice)
                {
                    case startgame:

                        DrawText("> START GAME <", 245, 600, 60, WHITE);
                        DrawText("CREDIT", 383, 700, 60, WHITE);
                        DrawText("LEAVE GAME", 299, 800, 60, WHITE);

                        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
                        {
                            currentTitle = game;
                            needReset = true;
                            needNewBlock = true;
                        }
                        if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))
                        {
                            titleChoice = credit;
                        }
                    break;
                    case credit:

                        DrawText("START GAME", 293, 600, 60, WHITE);
                        DrawText("> CREDIT <", 335, 700, 60, WHITE);
                        DrawText("LEAVE GAME", 299, 800, 60, WHITE);

                        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
                        {
                            currentTitle = credit_screen;
                        }
                        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))
                        {
                            titleChoice = startgame;
                        }
                        if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))
                        {
                            titleChoice = leavegame;
                        }
                    break;
                    case leavegame:

                        DrawText("START GAME", 293, 600, 60, WHITE);
                        DrawText("CREDIT", 383, 700, 60, WHITE);
                        DrawText("> LEAVE GAME <", 251, 800, 60, WHITE);

                        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))
                        {
                            titleChoice = credit;
                        }
                        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
                        {
                            shouldLeaveGame = true;
                        }

                    break;
                }

                if ((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) && setting_level != 0)
                {
                    --setting_level;
                }
                else if ((IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) && setting_level < 18)
                {
                    ++setting_level;
                }
                
                DrawText("STARTING LEVEL", (1000 - MeasureText("STARTING LEVEL", 50)) / 2, 450, 50, YELLOW);
                DrawText(("<- " + std::to_string(setting_level) + " +>").c_str(), (1000 - MeasureText(("<- " + std::to_string(setting_level) + " +>").c_str(), 70)) / 2, 520, 70, YELLOW);

            break;

            //Game screen
            case game:
                if (!runOnce) //Generate new block at start-up
                {
                    random = rand()%19 + 1;
                    runOnce = true;
                }
                if (needReset) //Reset board
                {
                    reset(&b_state[0]);
                    needReset = false;
                    score = 0;
                    lines = 0;
                    level = 0;
                    in_game_status = not_pause;
                    canHold = true;
                    holdDummy = 0;
                    holdBlock = rand()%19 + 1;
                }
                if (_7_1 && _2_2 && _7_3)
                {
                    setting_level = 727;
                    score = 727;
                    egg_1 = 5, egg_2 = 4;
                    _7_1 = false, _2_2 = false, _7_3 = false, setting_level_need_reset = true;
                }

                if (lose(b_state[0])) //Detect GameOver
                {
                    isLose = true;
                }
                    
                if (!isLose && in_game_status == not_pause) //In game
                {
                    if (bg_move != 225) //BG moving
                    {
                        ++bg_moveCounter;
                        if (!(bg_moveCounter % 6))
                        {
                            bg_move += 45;
                        }
                    }
                    //Key pressed and actions START
                    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) //LEFT
                    {
                        if (left_counter == 0 || left_counter == 16 || (left_counter > 16 && !((left_counter - 16) % 6)))
                        {
                            if (moveStatic(&b_state[0], -1, current_index_1, current_index_2, current_index_3, current_index_4))
                            {
                                --current_index_1;
                                --current_index_2;
                                --current_index_3;
                                --current_index_4;
                            }
                        }
                        ++left_counter;
                    } 
                    else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) //RIGHT
                    {
                        if (right_counter == 0 || right_counter == 16 || (right_counter > 16 && !((right_counter - 16) % 6)))
                        {
                            if (moveStatic(&b_state[0], 1, current_index_1, current_index_2, current_index_3, current_index_4))
                            {
                                ++current_index_1;
                                ++current_index_2;
                                ++current_index_3;
                                ++current_index_4;
                            }
                        }
                        ++right_counter;
                    }
                    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) //SOFT DROP
                    {
                        if (down_counter == 0 || !(down_counter % (Gridcell / 2)))
                        {
                            if (moveStatic(&b_state[0], 10, current_index_1, current_index_2, current_index_3, current_index_4))
                            {
                                current_index_1 += 10;
                                current_index_2 += 10;
                                current_index_3 += 10;
                                current_index_4 += 10;
                            }
                        }
                        ++down_counter;
                    }
                    if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && canHold) //Hold and rotate
                    {
                        canHold = false;
                        holdDummy = currentBlock;
                        currentBlock = holdBlock;
                        switch (holdDummy) // rotate clockwise
                        {
                            case 1: holdBlock = 2; br
                            case 2: holdBlock = 1; br
                            case 3: holdBlock = 4; br
                            case 4: holdBlock = 5; br
                            case 5: holdBlock = 6; br
                            case 6: holdBlock = 3; br
                            case 7: holdBlock = 10; br
                            case 8: holdBlock = 7; br
                            case 9: holdBlock = 8; br
                            case 10: holdBlock = 9; br
                            case 11: holdBlock = 11; br
                            case 12: holdBlock = 13; br
                            case 13: holdBlock = 12; br
                            case 14: holdBlock = 15; br
                            case 15: holdBlock = 14; br
                            case 16: holdBlock = 19; br
                            case 17: holdBlock = 18; br
                            case 18: holdBlock = 16; br
                            case 19: holdBlock = 17;
                        }
                        *(&b_state[0] + current_index_1) = air;
                        *(&b_state[0] + current_index_2) = air;
                        *(&b_state[0] + current_index_3) = air;
                        *(&b_state[0] + current_index_4) = air;
                        spawnBlock(b_state[0], currentBlock);
                        switch (currentBlock) //Re-register current_index
                            {
                                case 1: //I - 1
                                    current_index_1 = 15;
                                    current_index_2 = 25;
                                    current_index_3 = 35;
                                    current_index_4 = 45; br
                                case 2: // I - 2
                                    current_index_1 = 13;
                                    current_index_2 = 14;
                                    current_index_3 = 15;
                                    current_index_4 = 16; br
                                case 3: //L - 1
                                    current_index_1 = 5;
                                    current_index_2 = 15;
                                    current_index_3 = 25;
                                    current_index_4 = 4; br
                                case 4: //L - 2
                                    current_index_1 = 5;
                                    current_index_2 = 13;
                                    current_index_3 = 14;
                                    current_index_4 = 15; br
                                case 5: //L - 3
                                    current_index_1 = 4;
                                    current_index_2 = 14;
                                    current_index_3 = 24;
                                    current_index_4 = 25; br
                                case 6: //L - 4
                                    current_index_1 = 4;
                                    current_index_2 = 5;
                                    current_index_3 = 6;
                                    current_index_4 = 14; br
                                case 7: //J - 1
                                    current_index_1 = 4;
                                    current_index_2 = 14;
                                    current_index_3 = 24;
                                    current_index_4 = 5; br
                                case 8: //J - 2
                                    current_index_1 = 4;
                                    current_index_2 = 14;
                                    current_index_3 = 15;
                                    current_index_4 = 16; br
                                case 9: //J - 3
                                    current_index_1 = 5;
                                    current_index_2 = 15;
                                    current_index_3 = 25;
                                    current_index_4 = 24; br
                                case 10: //J - 4
                                    current_index_1 = 4;
                                    current_index_2 = 5;
                                    current_index_3 = 6;
                                    current_index_4 = 16; br
                                case 11: // O
                                    current_index_1 = 4;
                                    current_index_2 = 5;
                                    current_index_3 = 14;
                                    current_index_4 = 15; br
                                case 12: //S - 1
                                    current_index_1 = 14;
                                    current_index_2 = 15;
                                    current_index_3 = 5;
                                    current_index_4 = 6; br
                                case 13: //S - 2
                                current_index_1 = 4;
                                    current_index_2 = 14;
                                    current_index_3 = 15;
                                    current_index_4 = 25; br
                                case 14: //Z - 1
                                    current_index_1 = 3;
                                    current_index_2 = 4;
                                    current_index_3 = 14;
                                    current_index_4 = 15; br
                                case 15: //Z - 2
                                    current_index_1 = 5;
                                    current_index_2 = 15;
                                    current_index_3 = 14;
                                    current_index_4 = 24; br
                                case 16: //T - 1
                                    current_index_1 = 5;
                                    current_index_2 = 14;
                                    current_index_3 = 15;
                                    current_index_4 = 16; br
                                case 17: //T - 2
                                    current_index_1 = 4;
                                    current_index_2 = 5;
                                    current_index_3 = 6;
                                    current_index_4 = 15; br
                                case 18: //T - 3
                                    current_index_1 = 14;
                                    current_index_2 = 5;
                                    current_index_3 = 15;
                                    current_index_4 = 25; br
                                case 19: //T - 4
                                    current_index_1 = 4;
                                    current_index_2 = 14;
                                    current_index_3 = 24;
                                    current_index_4 = 15; br
                            }
                    }
                    // - Reset timer when release KEY:
                    if (IsKeyUp(KEY_LEFT) && IsKeyUp(KEY_A))
                    {
                        left_counter = 0;
                    }
                    if (IsKeyUp(KEY_RIGHT) && IsKeyUp(KEY_D))
                    {
                        right_counter = 0;
                    }
                    if (IsKeyUp(KEY_DOWN) && IsKeyUp(KEY_S))
                    {
                        down_counter = 0;
                    }
                    //Key pressed and actions END

                    if (needNewBlock) //Detect if need to spawn new block
                    {
                        ++ARE;
                        if (!(ARE % 19))
                        {
                            switch (random) //Re-register current_index
                            {
                                case 1: //I - 1
                                    current_index_1 = 15;
                                    current_index_2 = 25;
                                    current_index_3 = 35;
                                    current_index_4 = 45; br
                                case 2: // I - 2
                                    current_index_1 = 13;
                                    current_index_2 = 14;
                                    current_index_3 = 15;
                                    current_index_4 = 16; br
                                case 3: //L - 1
                                    current_index_1 = 5;
                                    current_index_2 = 15;
                                    current_index_3 = 25;
                                    current_index_4 = 4; br
                                case 4: //L - 2
                                    current_index_1 = 5;
                                    current_index_2 = 13;
                                    current_index_3 = 14;
                                    current_index_4 = 15; br
                                case 5: //L - 3
                                    current_index_1 = 4;
                                    current_index_2 = 14;
                                    current_index_3 = 24;
                                    current_index_4 = 25; br
                                case 6: //L - 4
                                    current_index_1 = 4;
                                    current_index_2 = 5;
                                    current_index_3 = 6;
                                    current_index_4 = 14; br
                                case 7: //J - 1
                                    current_index_1 = 4;
                                    current_index_2 = 14;
                                    current_index_3 = 24;
                                    current_index_4 = 5; br
                                case 8: //J - 2
                                    current_index_1 = 4;
                                    current_index_2 = 14;
                                    current_index_3 = 15;
                                    current_index_4 = 16; br
                                case 9: //J - 3
                                    current_index_1 = 5;
                                    current_index_2 = 15;
                                    current_index_3 = 25;
                                    current_index_4 = 24; br
                                case 10: //J - 4
                                    current_index_1 = 4;
                                    current_index_2 = 5;
                                    current_index_3 = 6;
                                    current_index_4 = 16; br
                                case 11: // O
                                    current_index_1 = 4;
                                    current_index_2 = 5;
                                    current_index_3 = 14;
                                    current_index_4 = 15; br
                                case 12: //S - 1
                                    current_index_1 = 14;
                                    current_index_2 = 15;
                                    current_index_3 = 5;
                                    current_index_4 = 6; br
                                case 13: //S - 2
                                current_index_1 = 4;
                                    current_index_2 = 14;
                                    current_index_3 = 15;
                                    current_index_4 = 25; br
                                case 14: //Z - 1
                                    current_index_1 = 3;
                                    current_index_2 = 4;
                                    current_index_3 = 14;
                                    current_index_4 = 15; br
                                case 15: //Z - 2
                                    current_index_1 = 5;
                                    current_index_2 = 15;
                                    current_index_3 = 14;
                                    current_index_4 = 24; br
                                case 16: //T - 1
                                    current_index_1 = 5;
                                    current_index_2 = 14;
                                    current_index_3 = 15;
                                    current_index_4 = 16; br
                                case 17: //T - 2
                                    current_index_1 = 4;
                                    current_index_2 = 5;
                                    current_index_3 = 6;
                                    current_index_4 = 15; br
                                case 18: //T - 3
                                    current_index_1 = 14;
                                    current_index_2 = 5;
                                    current_index_3 = 15;
                                    current_index_4 = 25; br
                                case 19: //T - 4
                                    current_index_1 = 4;
                                    current_index_2 = 14;
                                    current_index_3 = 24;
                                    current_index_4 = 15; br
                            }
                            spawnBlock(b_state[0], random);
                            currentBlock = random;
                            random = rand()%19 + 1;
                            needNewBlock = false;
                            ARE = 1;
                            canHold = true;
                        }
                        
                    }
                    
                    if (IsKeyUp(KEY_DOWN) && IsKeyUp(KEY_S)) //Move block down when unpressed down (per period)
                    {
                        ++move_counter;
                        if (!(move_counter % Gridcell))
                        {
                            if (moveStatic(&b_state[0], 10, current_index_1, current_index_2, current_index_3, current_index_4))
                            {
                                current_index_1 += 10;
                                current_index_2 += 10;
                                current_index_3 += 10;
                                current_index_4 += 10;
                                move_counter = 0;
                            }
                        }
                    }

                    lineDestroyed = deleteLines(&b_state[0]); //Count and delete line
                    switch (lineDestroyed) //Add score
                    {
                        case 1:
                            score += 40;
                        break;
                        case 2:
                            score += 100;
                        break;
                        case 3:
                            score += 300;
                        break;
                        case 4:
                            score += 1200;
                        break;
                    }
                    lines += lineDestroyed; //Add lines

                    level = (lines - (lines % 10)) / 10 + setting_level;
                    
                    for (int index = 0; index < 220; ++index) //blocks check and modify
                    {
                        if (needChangeState(b_state[index]))
                        {
                            ++changeStateCounter;
                            if (!(changeStateCounter % Gridcell))
                            {
                                changeAllState(&(b_state[0]));
                                needNewBlock = true;
                                break;
                            }
                        }
                    }
                
                    switch (level) //Change Gridcell by level
                    {
                        case 0: Gridcell = 48; br
                        case 1: Gridcell = 43; br
                        case 2: Gridcell = 38; br
                        case 3: Gridcell = 33; br
                        case 4: Gridcell = 28; br
                        case 5: Gridcell = 23; br
                        case 6: Gridcell = 18; br
                        case 7: Gridcell = 13; br
                        case 8: Gridcell = 8; br
                        case 9: Gridcell = 6; br
                        case 10 ... 12: Gridcell = 5; br
                        case 13 ... 15: Gridcell = 4; br
                        case 16 ... 18: Gridcell = 3; br
                        case 19 ... 28: Gridcell = 2; br
                        default: Gridcell = 1;
                    }
                }

                //BG and gameboard and FPS texture START
                DrawTextureRec(Background, Rectangle{0, (float)0 + bg_move, 1000, 1000}, Vector2{0, 0}, WHITE); //Background
                DrawTexture(Board, 265, 40, WHITE);                                                             //Gameboard
                DrawTexture(Box, 32, 429, WHITE); //Lines box
                DrawTexture(Box, 768, 429, WHITE); //Score box
                DrawTexture(ThinBox, 32, 579, WHITE); //Level box
                DrawTexture(NextBox, 768, 150, WHITE); //NextBlock box
                DrawTexture(NextBox, 32, 150, WHITE); //HoldBlock box
                DrawText("LINES", 32 + (200 - MeasureText("LINES", 20)) / 2, 449, 20, WHITE);
                DrawText((scoreText.substr(egg_1, egg_2)).c_str(), 768 + (200 - MeasureText((scoreText.substr(egg_1, egg_2)).c_str(), 20)) / 2, 449, 20, WHITE);
                DrawText("NEXT", 768 + (200 - MeasureText("NEXT", 20)) / 2, 165, 20, WHITE);
                DrawText("HOLD", 32 + (200 - MeasureText("HOLD", 20)) / 2, 165, 20, WHITE);
                DrawText(std::to_string(lines).c_str(), 32 + (200 - MeasureText(std::to_string(lines).c_str(), 50)) / 2, 489, 50, WHITE);
                DrawText(std::to_string(score).c_str(), 768 + (200 - MeasureText(std::to_string(score).c_str(), 50)) / 2, 489, 50, WHITE);
                DrawText(("Level - " + std::to_string(level)).c_str(), 32 + (200 - MeasureText(("Level - " + std::to_string(level)).c_str(), 20)) / 2, 596, 20, WHITE);
                DrawTextureRec(BlocksMap, Rectangle{0, (float)180 * (random - 1), 180, 180}, Vector2{778, 205}, WHITE);
                DrawTextureRec(BlocksMap, Rectangle{0, (float)180 * (holdBlock - 1), 180, 180}, Vector2{42, 205}, WHITE);
                //BG and gameboard and FPS texture END

                DrawRectangle(940, 10, 50, 50, WHITE);
                DrawRectangle(950, 15, 12, 25, Color{56, 56, 56, 255});
                DrawRectangle(968, 15, 12, 25, Color{56, 56, 56, 255});
                DrawText("SPACE", 940 + (50 - MeasureText("SPACE", 8)) / 2, 45, 8, Color{56, 56, 56, 255});

                for (int index = 0; index < 220; ++index) //Render blocks
                {
                    if (b_state[index] == air)
                        continue;
                    switch (b_state[index])
                    {
                        case I:
                        case sI:
                            DrawTextureRec(Block, 
                            Rectangle{45, 0, 45, 45}, Vector2{
                            (float)275 + (45 * (index % 10)), // x
                            (float)(-40 + 45 * ((index - index % 10) / 10))},  // y
                            WHITE); 
                            break;
                        case L:
                        case sL:
                            DrawTextureRec(Block, 
                            Rectangle{135, 0, 45, 45}, Vector2{
                            (float)275 + (45 * (index % 10)), // x
                            (float)(-40 + 45 * ((index - index % 10) / 10))},  // y
                            WHITE); 
                            break;
                        case J:
                        case sJ:
                            DrawTextureRec(Block, 
                            Rectangle{0, 0, 45, 45}, Vector2{
                            (float)275 + (45 * (index % 10)), // x
                            (float)(-40 + 45 * ((index - index % 10) / 10))},  // y
                            WHITE); 
                            break;
                        case O:
                        case sO:
                            DrawTextureRec(Block, 
                            Rectangle{270, 0, 45, 45}, Vector2{
                            (float)275 + (45 * (index % 10)), // x
                            (float)(-40 +  45 * ((index - index % 10) / 10))}, // y
                            WHITE); 
                            break;
                        case S:
                        case sS:
                            DrawTextureRec(Block, 
                            Rectangle{90, 0, 45, 45}, Vector2{
                            (float)275 + (45 * (index % 10)), // x
                            (float)(-40 + 45 * ((index - index % 10) / 10))},  // y
                            WHITE); 
                            break;
                        case Z:
                        case sZ:
                            DrawTextureRec(Block, 
                            Rectangle{225, 0, 45, 45}, Vector2{
                            (float)275 + (45 * (index % 10)), // x
                            (float)(-40 + 45 * ((index - index % 10) / 10))},  // y
                            WHITE); 
                            break;
                        case T:
                        case sT:
                            DrawTextureRec(Block, 
                            Rectangle{180, 0, 45, 45}, Vector2{
                            (float)275 + (45 * (index % 10)), // x
                            (float)(-40 + 45 * ((index - index % 10) / 10))},  // y
                            WHITE); 
                            break;
                    }
                }

                if (isLose) //Lose screen
                {
                    DrawText("GAME OVER", (1000 - MeasureText("GAME OVER", 100)) / 2, 400, 100, WHITE);
                    DrawText("Press SPACE to continue", (1000 - MeasureText("Press SPACE to continue", 50)) / 2, 700, 50, WHITE);
                    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
                    {
                        currentTitle = game_over;
                    }
                }

                if (IsKeyPressed(KEY_SPACE) && in_game_status == not_pause) //Pause Game
                {
                    in_game_status = is_pause;
                    justPause = 0;
                }
                else if (IsKeyPressed(KEY_SPACE) && in_game_status == is_pause) //Back to Game
                {
                    in_game_status = not_pause;
                }
                else if (IsKeyPressed(KEY_ESCAPE) && in_game_status == is_pause) //Quit Game when Pause
                {
                    in_game_status = not_pause;
                    currentTitle = game_over;
                }

                if (!isLose && in_game_status == is_pause) //Pause screen
                {
                    //Just move BG
                    if (justPause < 16)
                    {
                        DrawTextureRec(Block, Rectangle{135, 0, 45, 45}, Vector2{950, 955}, WHITE);
                        DrawTextureRec(Block, Rectangle{135, 0, 45, 45}, Vector2{950, 910}, WHITE);
                        DrawTextureRec(Block, Rectangle{135, 0, 45, 45}, Vector2{950, 865}, WHITE);
                        DrawTextureRec(Block, Rectangle{135, 0, 45, 45}, Vector2{905, 865}, WHITE);
                        ++justPause;
                    } 
                    else if (justPause == 16)
                    {
                        bg_move = 90;
                    }
                    
                    //Pause UI
                    DrawTexture(Box, 400, 400, WHITE);
                    DrawText("ZA WARUDO", 400 + (200 - MeasureText("ZA WARUDO", 25)) / 2, 415, 25, WHITE);
                    DrawText("SPACE: Continue", 417, 460, 20, WHITE);
                    DrawText("    ESC: Quit", 413, 480, 20, WHITE);
                }
            break;
            //Game Over screen
            case game_over:
                if (runOnce) //Next game's init
                {
                    isLose = false;
                    random = rand()%19 + 1;
                    runOnce = false;
                }

                DrawTextureRec(Background, Rectangle{0, 135, 1000, 1000}, Vector2{0, 0}, WHITE);
                DrawText("Result", (1000 - MeasureText("Result", 100)) / 2, 150, 100, WHITE);
                DrawText("SPACE to go back to title", (1000 - MeasureText("SPACE to go back to title", 50)) / 2, 750, 50, WHITE);
                DrawTexture(Box, 275, 340, WHITE); //Lines Box
                DrawTexture(Box, 525, 340, WHITE); //Score Box
                DrawTexture(ThinBox, 400, 515, WHITE);
                DrawText("LINES", 275 + (200 - MeasureText("LINES", 20)) / 2, 360, 20, WHITE);
                DrawText("SCORE", 525 + (200 - MeasureText("SCORE", 20)) / 2, 360, 20, WHITE);
                DrawText(("Level - " + std::to_string(level)).c_str(), 418, 532, 20, WHITE);
                DrawText(std::to_string(lines).c_str(), 275 + (200 - MeasureText(std::to_string(lines).c_str(), 50)) / 2, 400, 50, WHITE); //Lines
                DrawText(std::to_string(score).c_str(), 525 + (200 - MeasureText(std::to_string(score).c_str(), 50)) / 2, 400, 50, WHITE); //SCORE

                if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) //Back to title
                {
                    currentTitle = title;
                }

            break;
            case credit_screen:
                DrawTexture(TitleBG, 0, 0, WHITE);
                DrawText("CREDIT", (1000 - MeasureText("CREDIT", 75)) / 2, 420, 75, WHITE);
                DrawText("Game made by Toastori", (1000 - MeasureText("Game made by Toastori", 50)) / 2, 512, 50, WHITE);
                DrawText("Made with RAYLIB", (1000 - MeasureText("Made with RAYLIB", 25)) / 2, 570, 25, WHITE);
                DrawText("SPACE to go back to title", (1000 - MeasureText("SPACE to go back to title", 20)) / 2, 670, 20, DARKGRAY);

                if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
                {
                    currentTitle = title;
                    titleChoice = startgame;
                }
            break;
        }

        EndDrawing();

    }

    // Close window and OpenGL context
    CloseWindow();        

    return 0;
}