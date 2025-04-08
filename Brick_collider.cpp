#pragma comment(lib, "winmm.lib")
#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

#include "iGraphics.h"
#define screenWidth 800
#define screenHeight 600
#define yAxisMargin 50 // ei margin er niche gele game over(stick er niche chole jay)
#define brick_initial 50
#define angle_initial 30
#define PI 3.14159265

int r = 255, g = 0, b = 255;
bool musicOn = true;
bool islevel = false;
int life_count = 0;
int selected_menu = 0;
int level_menu = 0;
int game_menu = 0;
int menu = 0;
struct buttonCordinate
{
    int x;
    int y;
} bcordinate[4];

struct levelCordinate
{
    int x;
    int y;
} lcordinate[3];

struct menu_cordinate
{
    int x;
    int y;
} mcordinate[3];

int mposx, mposy;

char button[10][150] = {"images\\start.bmp", "images\\highscores.bmp", "images\\instructions.bmp", "images\\close.bmp"};
char levels[4][150] = {"images\\easy.bmp", "images\\medium.bmp", "images\\hard.bmp"};
char life_over[4][150] = {"images\\life3.bmp", "images\\life2.bmp", "images\\life1.bmp"};
char menu_ops[4][150] = {"images\\sound.bmp", "images\\return.bmp", "images\\exit.bmp"};
char homemenu[100] = {"images\\homemenu.bmp"};
char difficulty[100] = {"images\\difficulty.bmp"};
char hscore[100] = {"images\\leaderboard.bmp"};
char ins[100] = {"images\\ins.bmp"};
char quit[100] = {"images\\quit.bmp"};
char gameover[100] = {"images\\gameover.bmp"};
char gamecomplete[100] = {"images\\levelcomplete.bmp"};
char obs1pic1[100]= {"images\\obs1.bmp"};
char obs1pic2[100]= {"images\\obs2.bmp"};

int gameState = -1; // game er initial obostha
int level = -1;
char background1[100] = {"images\\level1.bmp"};
char background2[100] = {"images\\level2.bmp"};
char background3[100] = {"images\\level3.bmp"};
char spbric1[100] = {"images\\spbrickk1.bmp"};
char spbric2[100] = {"images\\spbrickk2.bmp"};
char normbrk[100] = {"images\\normalbrk.bmp"};

// drawing the bricks
struct brick_making
{
    int x;
    int y;
    int dx = 100;
    int dy = 20;
    bool show = true;

    int red, green, blue; // coloring the bricks
} bricks[brick_initial];

int xBall = (int)screenWidth / 2; // ball ke center e rakhte
int yBall = yAxisMargin;          // xBall and yBall means posiition co-ordinate of ball.
int rBall = 10;                   // radius of ball

int angle = angle_initial; // ball er velocity jate angle er upore depend kore change hoy.
int velocity = 10;
float dx = (float)velocity * cos(angle * PI / 180); // dx,dy means speed co-ordinate of ball.
float dy = (float)velocity * sin(angle * PI / 180);

// creating the stick to hold ball
int xstick = xBall - 50;
int ystick = yBall - 20;
int dxstick = 100;
int dystick = 15;

// obstacle for level 2 and level 3
int xobs = 0, xobs1 = 730;
int yobs = 250, yobs1= 230;
int dxobs = 70;
int dyobs = 15;
int obs_speed1 = 3;
int obs_speed2 = 5; // level 3 er jonno speed besi
int obs_direction = 1, obs_direction1 = -1;

bool isStarted = false;
int point;
int length = 0;
char string[100];
bool point_up = true;

struct player_info
{
    char player[100];
    int score;
} ply_info[100];

player_info players[100];
int total_ply = 0;
char flscore[] = "score_leader.txt";
char username[100];
int username_index = 0;
int scores; // point er value save&pass korte

void change();
void restart();
void making_bricks();
void points_reading();
void points_loading();
void points_sorting();
void new_ldbrd();

void making_bricks()
{
    int sumx = 100, sumy = 400;
    point = 0;
    for (int i = 0; i < brick_initial; i++)
    {
        bricks[i].red = rand() % 255;
        bricks[i].green = rand() % 255;
        bricks[i].blue = rand() % 255;

        bricks[i].x = sumx;
        bricks[i].y = sumy;
        bricks[i].show = true;
        sumx += 100;
        if (sumx >= 600)
        {
            sumx = 100;
            sumy += 20;
        }
    }
}

void restart()
{
    life_count++;
    if (life_count > 2) // total 3 lives.
    {
        gameState = 4;
        level = -1;
        life_count = 0;
        making_bricks();
    }

    xBall = (int)screenWidth / 2; // ball ke center e rakhte
    yBall = yAxisMargin;

    angle = 45;
    velocity = 15;
    dx = velocity * cos(angle); // ball er velocity jate angle er upore depend kore change hoy.
    dy = velocity * sin(angle);

    // creating the stick to hold ball
    xstick = xBall - 50;
    ystick = yBall - 20;
    isStarted = false;
}

void obstacle_move()
{
    // obstacle ke move korano
    xobs += obs_speed1 * obs_direction;

    if (xobs <= 0)
    {
        xobs = 0;
        obs_direction *= (-1);
    }
    else if (xobs + dxobs >= screenWidth)
    {
        xobs = screenWidth - dxobs;
        obs_direction *= (-1);
    }
}

void obstacle_move2()
{
    xobs += (obs_speed2 * obs_direction);
    xobs1 += (obs_speed2 * obs_direction1);

    if (xobs <= 0)
    {
        xobs = 0;
        obs_direction *= (-1);
    }
    else if (xobs + dxobs >= screenWidth)
    {
        xobs = screenWidth - dxobs;
        obs_direction *= (-1);
    }

    if (xobs1 <= 0)
    {
        xobs1 = 0;
        obs_direction1 *= (-1);
    }
    else if (xobs1 + dxobs >= screenWidth)
    {
        xobs1 = screenWidth - dxobs;
        obs_direction1 *= (-1);
    }
}

void life()
{
    if (life_count == 0)
    {
        iShowBMP2(730, 570, life_over[0], 0);
    }
    if (life_count == 1)
    {
        iShowBMP2(730, 570, life_over[1], 0);
    }
    if (life_count == 2)
    {
        iShowBMP2(730, 570, life_over[2], 0);
    }
}

void change()
{
    if (gameState == 11 && islevel == true)
    {
        if (level == 0)
        {
            if (isStarted == true)
            {
                xBall += dx;
                yBall += dy;

                if (xBall >= screenWidth || xBall <= 0)
                    dx *= (-1); // screen er baire ball chole gele ulta dike phire ashe
                if (yBall >= screenHeight || yBall <= 0)
                    dy *= (-1);

                // colliding with bricks
                for (int i = 0; i < brick_initial - 20; i++)
                {
                    if (bricks[i].show)
                    {
                        if (xBall >= bricks[i].x && xBall <= bricks[i].x + bricks[i].dx)
                        {
                            if (yBall >= bricks[i].y && yBall <= bricks[i].y + bricks[i].dy)
                            {
                                dy *= (-1);
                                bricks[i].show = false; // oi brick ta vanish
                                point++;
                            }
                        }
                        else if (yBall >= bricks[i].y && yBall <= bricks[i].y + bricks[i].dy)
                        {
                            if (xBall >= bricks[i].x && xBall <= bricks[i].x + bricks[i].dx)
                            {
                                dx *= (-1);
                                bricks[i].show = false; // oi brick ta vanish
                                point++;
                            }
                        }
                    }
                }
                scores = point;
                // capturing the ball in stick
                if (xBall >= xstick && xBall <= xstick + dxstick && yBall >= ystick && yBall <= ystick + dystick)
                {
                    angle = (xstick + dxstick - xBall) + 40;    // ball er angle change hobe jodi stick er majhe ball pore
                    dx = (int)velocity * cos(angle * PI / 180); // ball er velocity jate angle er upore depend kore change hoy.
                    dy = (int)velocity * sin(angle * PI / 180);
                }
                else
                {
                    // ball board er niche porle game restart korte hobe
                    if (yBall < ystick)
                        restart();
                }
                if (point == 30 && level == 0)
                {
                    gameState = 5;
                    making_bricks();
                    restart();
                    life_count = 0;
                    isStarted=false;
                }
            }
        }
    }
    if (level == 1)
    {
        if (isStarted == true)
        {
            xBall += dx;
            yBall += dy;

            if (xBall >= screenWidth || xBall <= 0)
                dx *= (-1); // screen er baire ball chole gele ulta dike phire ashe
            if (yBall >= screenHeight || yBall <= 0)
                dy *= (-1);

            obstacle_move();
            //  obstacle r ball collision
            if (xBall >= xobs && xBall <= xobs + dxobs && yBall >= yobs && yBall <= yobs + dyobs)
            {
                if (xBall >= xobs && xBall <= xobs + dxobs)
                {
                    if (yBall + rBall >= yobs && yBall - rBall <= yobs + dyobs)
                        restart();
                }
                if (yBall >= yobs && yBall <= yobs + dyobs)
                {
                    if (xBall + rBall >= xobs && xBall - rBall <= xobs + dxobs)
                        restart();
                }
            }

            // colliding with bricks

            for (int i = 0; i < brick_initial - 10; i++)
            {
                if (bricks[i].show)
                {
                    if (xBall >= bricks[i].x && xBall <= bricks[i].x + bricks[i].dx)
                    {
                        if (yBall >= bricks[i].y && yBall <= bricks[i].y + bricks[i].dy)
                        {
                            dy *= (-1);
                            bricks[i].show = false; // oi brick ta vanish
                            point++;
                        }
                    }
                    else if (yBall >= bricks[i].y && yBall <= bricks[i].y + bricks[i].dy)
                    {
                        if (xBall >= bricks[i].x && xBall <= bricks[i].x + bricks[i].dx)
                        {
                            dx *= (-1);
                            bricks[i].show = false; // oi brick ta vanish
                            point++;
                        }
                    }
                }
            }
            scores = point;
            // capturing the ball in stick
            if (xBall >= xstick && xBall <= xstick + dxstick && yBall >= ystick && yBall <= ystick + dystick)
            {
                angle = (xstick + dxstick - xBall) + 40;    // ball er angle change hobe jodi stick er majhe ball pore
                dx = (int)velocity * cos(angle * PI / 180); // ball er velocity jate angle er upore depend kore change hoy.
                dy = (int)velocity * sin(angle * PI / 180);
            }
            else
            {
                // ball board er niche porle game restart korte hobe
                if (yBall < ystick)
                    restart();
            }

            if (point == 40 && level == 1)
            {
                gameState = 5;
                making_bricks();
                restart();
                life_count = 0;
                isStarted=false;
            }
        }
    }
    if (level == 2)
    {
        if (isStarted == true)
        {
            xBall += dx;
            yBall += dy;

            if (xBall >= screenWidth || xBall <= 0)
                dx *= (-1); // screen er baire ball chole gele ulta dike phire ashe
            if (yBall >= screenHeight || yBall <= 0)
                dy *= (-1);

            obstacle_move2();
            //  obstacle r ball collision
            if (xBall >= xobs && xBall <= xobs + dxobs && yBall >= yobs && yBall <= yobs + dyobs)
            {
                if (xBall >= xobs && xBall <= xobs + dxobs)
                {
                    if (yBall + rBall >= yobs && yBall - rBall <= yobs + dyobs)
                        dx *= (-1);
                }
                if (yBall >= yobs && yBall <= yobs + dyobs)
                {
                    if (xBall + rBall >= xobs && xBall - rBall <= xobs + dxobs)
                        dy *= (-1);
                }
            }

            if (xBall >= xobs1 && xBall <= xobs1 + dxobs && yBall >= yobs1 && yBall <= yobs1 + dyobs)
            {
                if (xBall >= xobs1 && xBall <= xobs1 + dxobs)
                {
                    if (yBall >= yobs1 && yBall <= yobs1 + dyobs)
                        restart();
                }
                if (yBall >= yobs1 && yBall <= yobs1 + dyobs)
                {
                    if (xBall >= xobs1 && xBall <= xobs1 + dxobs)
                        restart();
                }
            }

            // colliding with bricks
            for (int i = 0; i < brick_initial; i++)
            {
                if (bricks[i].show)
                {
                    if (xBall >= bricks[i].x && xBall <= bricks[i].x + bricks[i].dx)
                    {
                        if (yBall >= bricks[i].y && yBall <= bricks[i].y + bricks[i].dy)
                        {
                            dy *= (-1);
                            bricks[i].show = false; // oi brick ta vanish
                            point++;
                        }
                    }
                    else if (yBall >= bricks[i].y && yBall <= bricks[i].y + bricks[i].dy)
                    {
                        if (xBall >= bricks[i].x && xBall <= bricks[i].x + bricks[i].dx)
                        {
                            dx *= (-1);
                            bricks[i].show = false; // oi brick ta vanish
                            point++;
                        }
                    }
                }
            }
            scores = point;
            // capturing the ball in stick
            if (xBall >= xstick && xBall <= xstick + dxstick && yBall >= ystick && yBall <= ystick + dystick)
            {
                angle = (xstick + dxstick - xBall) + 40;    // ball er angle change hobe jodi stick er majhe ball pore
                dx = (int)velocity * cos(angle * PI / 180); // ball er velocity jate angle er upore depend kore change hoy.
                dy = (int)velocity * sin(angle * PI / 180);
            }
            else
            {
                // ball board er niche porle game restart korte hobe
                if (yBall < ystick)
                    restart();
            }

            if (point == 50 && level == 2)
            {
                gameState = 5;
                making_bricks();
                restart();
                life_count = 0;
                isStarted=false;
            }
        }
    }
}

void points_reading(const char *plynme, int score) // score file e save ba append er function
{
    FILE *fdr = fopen(flscore, "a");
    // score = *count;
    if (fdr)
    {
        fprintf(fdr, "%s  %d\n", plynme, score);
        fclose(fdr);
    }
}

void points_loading() // score file e read ba load er function
{
    FILE *fdl = fopen(flscore, "r");
    total_ply = 0;
    if (fdl)
    {
        while (fscanf(fdl, "%s %d", ply_info[total_ply].player, &ply_info[total_ply].score) != EOF)
        {
            total_ply++;
        }
        fclose(fdl);
    }
}

void points_sorting()
{
    for (int i = 0; i < total_ply - 1; i++)
    {
        int max_index = i;
        for (int j = i + 1; j < total_ply; j++)
        {
            if (ply_info[max_index].score < ply_info[j].score)
            {
                max_index = j;
            }
        }
        player_info temp = ply_info[i];
        ply_info[i] = ply_info[max_index];
        ply_info[max_index] = temp;
    }
}

void new_ldbrd()
{
    points_loading();
    points_sorting();

    char show_point[100];
    for (int i = 0; i < 5 && i < total_ply; i++)
    {
        sprintf(show_point, "%d. %s ---> %d ", i + 1, ply_info[i].player, ply_info[i].score);
        iSetColor(255, 255, 255);
        iText(300, 350 - i * 50, show_point, GLUT_BITMAP_TIMES_ROMAN_24);
    }
}

void iDraw()
{
    iClear();

    if (gameState == -1)
    {
        // homemenu background
        iShowBMP(0, 0, homemenu);

        for (int i = 0; i < 4; i++)
        {
            if (i == selected_menu)
            {
                int rm, gm, bm;
                rm = rand() % 255;
                gm = rand() % 255;
                bm = rand() % 255;

                iSetColor(rm, gm, bm);
                iRectangle(bcordinate[i].x - 3, bcordinate[i].y - 3, 255, 70);
            }
            iShowBMP2(bcordinate[i].x, bcordinate[i].y, button[i], 0);
        }

        iShowBMP2(mcordinate[0].x, mcordinate[0].y, menu_ops[0], 0);
        iSetColor(255, 255, 255);
        iText(630, 5, "Press 'ENTER' to select", GLUT_BITMAP_TIMES_ROMAN_10);
    }
    else if (gameState == 0)
    {
        iShowBMP(0, 0, difficulty);
        for (int i = 0; i < 3; i++)
        {
            if (i == level_menu)
            {
                int rm, gm, bm;
                rm = rand() % 255;
                gm = rand() % 255;
                bm = rand() % 255;

                iSetColor(rm, gm, bm);
                iRectangle(lcordinate[i].x - 5, lcordinate[i].y - 5, 250, 90);
            }
            iShowBMP2(lcordinate[i].x, lcordinate[i].y, levels[i], 0);
        }
        iSetColor(0, 0, 0);
        iFilledRectangle(0, 0, 800, 25);
        for (int i = 0; i < 3; i++)
        {
            iShowBMP2(mcordinate[i].x, mcordinate[i].y, menu_ops[i], 0);
        }
        iSetColor(255, 255, 255);
        iText(350, 5, "Press 'X' to select city", GLUT_BITMAP_TIMES_ROMAN_10);
    }
    else if (gameState == 11 && islevel == true)
    {
        if (level == 0)
        {
            iShowBMP(0, 0, background1);

            iSetColor(r, g, b);
            iFilledCircle(xBall, yBall, rBall, 1000); // 1000 for sharping the ball
            iSetColor(50, 0, 50);
            iFilledRectangle(xstick, ystick, dxstick, dystick);

            life(); //life icon show kora

            for (int i = 0; i < brick_initial - 20; i++)
            {
                if (bricks[i].show)
                {
                    iShowBMP2(bricks[i].x + 20, bricks[i].y + 5, normbrk, 0);
                }
            }

            iSetColor(0, 0, 0);
            iFilledRectangle(0, 0, 800, 25);

            iSetColor(200, 120, 80);
            iText(700, 10, "POINTS :", GLUT_BITMAP_9_BY_15); // score show korte
            char points[20];
            sprintf(points, "%d", point);
            iText(780, 10, points, GLUT_BITMAP_9_BY_15);

            for (int i = 0; i < 3; i++)
            {
                iShowBMP2(mcordinate[i].x, mcordinate[i].y, menu_ops[i], 0);
            }
            iSetColor(255, 255, 255);
            iText(300, 10, "press 'SPACE' to start and 'TAB' to goto CityGate", GLUT_BITMAP_TIMES_ROMAN_10);
        }

        if (level == 1)
        {
            iShowBMP(0, 0, background2);

            iSetColor(r, g, b);
            iFilledCircle(xBall, yBall, rBall, 1000); // 1000 for sharping the ball

            iSetColor(100, 0, 50);
            iFilledRectangle(xstick, ystick, dxstick, dystick);

            iShowBMP2(xobs,yobs,obs1pic1,0);

            life();

            for (int i = 0; i < brick_initial - 10; i++)
            {
                if (bricks[i].show)
                {
                    if (i % 2 == 0)
                    {
                        iShowBMP2(bricks[i].x + 20, bricks[i].y + 5, spbric1, 0);
                    }
                    else
                    {
                        iShowBMP2(bricks[i].x + 20, bricks[i].y + 5, normbrk, 0);
                    }
                }
            }

            iSetColor(0, 0, 0);
            iFilledRectangle(0, 0, 800, 25);

            iSetColor(200, 120, 80);
            iText(700, 10, "POINTS :", GLUT_BITMAP_9_BY_15); // score show korte
            char points[20];
            sprintf(points, "%d", point);
            iText(780, 10, points, GLUT_BITMAP_9_BY_15);

            for (int i = 0; i < 3; i++)
            {
                iShowBMP2(mcordinate[i].x, mcordinate[i].y, menu_ops[i], 0);
            }
            iSetColor(255, 255, 255);
            iText(300, 10, "press 'SPACE' to start and 'TAB' to goto CityGate", GLUT_BITMAP_TIMES_ROMAN_10);
        }

        if (level == 2)
        {
            iShowBMP(0, 0, background3);

            iSetColor(r, g, b);
            iFilledCircle(xBall, yBall, rBall, 1000); // 1000 for sharping the ball
            iSetColor(50, 0, 50);
            iFilledRectangle(xstick, ystick, dxstick, dystick);

            iShowBMP2(xobs,yobs,obs1pic1,0);
            iShowBMP2(xobs1,yobs1,obs1pic2,0);

            life();

            for (int i = 0; i < brick_initial; i++)
            {
                if (bricks[i].show)
                {
                    if (i % 3 == 0)
                    {
                        iShowBMP2(bricks[i].x + 20, bricks[i].y + 5, spbric1, 0);
                    }
                    else if( i%3 ==1)
                    {
                        iShowBMP2(bricks[i].x + 20, bricks[i].y + 5, normbrk, 0);
                    }
                    else if( i%3 ==2)
                    {
                        iShowBMP2(bricks[i].x + 20, bricks[i].y + 5, spbric2, 0);
                    }

                }
            }

            iSetColor(0, 0, 0);
            iFilledRectangle(0, 0, 800, 25);

            iSetColor(200, 120, 80);
            iText(700, 10, "POINTS :", GLUT_BITMAP_9_BY_15); // score show korte
            char points[20];
            sprintf(points, "%d", point);
            iText(780, 10, points, GLUT_BITMAP_9_BY_15);

            for (int i = 0; i < 3; i++)
            {
                iShowBMP2(mcordinate[i].x, mcordinate[i].y, menu_ops[i], 0);
            }
            iSetColor(255, 255, 255);
            iText(300, 10, "press 'SPACE' to start and 'TAB' to goto CityGate", GLUT_BITMAP_TIMES_ROMAN_10);
        }
    }
    else if (gameState == 1)
    {
        iShowBMP(0, 0, hscore);
        new_ldbrd();

        iSetColor(0, 0, 0);
        iFilledRectangle(0, 0, 800, 25);
        for (int i = 0; i < 3; i++)
        {
            iShowBMP2(mcordinate[i].x, mcordinate[i].y, menu_ops[i], 0);
        }
        iSetColor(255, 255, 255);
        iText(670, 5, "Press 'TAB' to goto CityGate", GLUT_BITMAP_TIMES_ROMAN_10);
    }
    else if (gameState == 2)
    {
        iShowBMP(0, 0, ins);
        iSetColor(0, 0, 0);
        iFilledRectangle(0, 0, 800, 25);
        for (int i = 0; i < 3; i++)
        {
            iShowBMP2(mcordinate[i].x, mcordinate[i].y, menu_ops[i], 0);
        }
        iSetColor(255, 255, 255);
        iText(670, 5, "Press 'TAB' to goto CityGate", GLUT_BITMAP_TIMES_ROMAN_10);
    }
    else if (gameState == 3)
    {
        iShowBMP(0, 0, quit);

        iSetColor(0, 0, 0);
        iText(300, 300, "## You want to end the game?", GLUT_BITMAP_TIMES_ROMAN_24);
        iSetColor(0, 0, 0);
        iText(300, 250, "* Press '=' if Yes", GLUT_BITMAP_TIMES_ROMAN_24);
        iSetColor(0, 0, 0);
        iText(300, 200, "* Press ESC if No", GLUT_BITMAP_TIMES_ROMAN_24);

        iFilledRectangle(0, 0, 800, 25);
        for (int i = 0; i < 3; i++)
        {
            iShowBMP2(mcordinate[i].x, mcordinate[i].y, menu_ops[i], 0);
        }
        iSetColor(255, 255, 255);
        iText(670, 5, "Press 'TAB' to goto CityGate", GLUT_BITMAP_TIMES_ROMAN_10);
    }
    else if (gameState == 4)
    {
        iShowBMP(0, 0, gameover);
        iSetColor(255, 255, 255);
        iText(300, 200, "Enter your name:", GLUT_BITMAP_TIMES_ROMAN_24);

        char nameinput[100];
        sprintf(nameinput, "%s ", username);
        iText(470, 200, nameinput, GLUT_BITMAP_TIMES_ROMAN_24);

        iSetColor(0, 0, 0);
        iFilledRectangle(0, 0, 800, 25);

        for (int i = 0; i < 3; i++)
        {
            iShowBMP2(mcordinate[i].x, mcordinate[i].y, menu_ops[i], 0);
        }
        iSetColor(255, 255, 255);
        iText(300, 10, "press '*' to save score and name.", GLUT_BITMAP_TIMES_ROMAN_10);
    }
    else if (gameState == 5)
    {
        iShowBMP(0, 0, gamecomplete);
        iSetColor(255, 255, 255);
        iText(300, 200, "Enter your name:", GLUT_BITMAP_TIMES_ROMAN_24);

        char nameinput[100];
        sprintf(nameinput, "%s ", username);
        iText(470, 200, nameinput, GLUT_BITMAP_TIMES_ROMAN_24);

        iSetColor(0, 0, 0);
        iFilledRectangle(0, 0, 800, 25);

        for (int i = 0; i < 3; i++)
        {
            iShowBMP2(mcordinate[i].x, mcordinate[i].y, menu_ops[i], 0);
        }
        iSetColor(255, 255, 255);
        iText(300, 10, "press ' * ' to save score and name", GLUT_BITMAP_TIMES_ROMAN_10);
    }
}
void iMouseMove(int mx, int my)
{
    xstick = mx - dxstick / 2; // moving stick with mouse
    if (!isStarted)
        xBall = xstick + dxstick / 2; // game suru na hole ball stcik er upor thakbe

    if (xstick < 0)
        xstick = 0;
    if (xstick > screenWidth - dxstick)
        xstick = screenWidth - dxstick;
}
void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        for (int i = 0; i < 3; i++)
        {
            if (mx >= mcordinate[i].x && mx <= mcordinate[i].x + 20 && my >= mcordinate[i].y && my <= mcordinate[i].y + 25)
            {
                if (i == 0)
                {
                    if (musicOn)
                    {
                        musicOn = false;
                        PlaySound(0, 0, 0);
                    }
                    else
                    {
                        musicOn = true;
                        PlaySound("music\\bg_music.wav", NULL, SND_LOOP | SND_ASYNC);
                    }
                }
                else if (i == 1)
                {
                    gameState = -1;
                }
                else
                {
                    exit(0);
                }
            }
        }
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
    }
}
void iPassiveMouseMove(int mx, int my)
{
}
void iKeyboard(unsigned char key)
{
    if (gameState != 4 || gameState != 5)
    {
        if (key == ' ') // space chepe game suru
        {
            if (!isStarted && gameState == 11)
            {
                isStarted = true; // isStarted false jokhon tokhn game chalu korte hobe
            }
        }
        // restarting when all life is gone
        if (key == '\t')
        {
            life_count = 0;
            gameState = 0;
            making_bricks();
        }
        if (key == '=')
        {
            exit(0);
        }
        if (key == 27)
        {
            gameState = -1;
        }
        if (key == '\r') // press enter select mennu
        {
            if (gameState == -1)
            {
                gameState = selected_menu;
            }
        }
        if (key == 'x' || key == 'X')
        {
            if (gameState == 0)
            {
                level = level_menu;
                islevel = true;
                gameState = 11;
            }
        }
    }
    if (gameState == 4 || gameState == 5)
    {
        if ((key >= 'a' && key <= 'z') || (key >= 'A' && key <= 'Z') || (key >= '0' && key <= '9'))
        {

            if (username_index < sizeof(username) - 1)
            {
                username[username_index++] = key;
            }
        }
        else if (key == '\b')
        {
            if (username_index > 0)
                username_index--;
            username[username_index] = '\0';
        }
        else if (key == '\r')
        {
            username[username_index + 1] = '\0';
            points_reading(username, scores);
        }
    }
}

void iSpecialKeyboard(unsigned char key)
{

    if (key == GLUT_KEY_UP)
    {
        if (gameState == -1)
        {
            selected_menu--;
            if (selected_menu < 0)
                selected_menu = 3;
        }
        else if (gameState == 0) //&& level == -1)
        {
            level_menu--;
            if (level_menu < 0)
            {
                level_menu = 2;
            }
        }
    }
    else if (key == GLUT_KEY_DOWN)
    {
        if (gameState == -1)
        {
            selected_menu++;
            if (selected_menu > 3)
                selected_menu = 0;
        }
        else if (gameState == 0) // && level == -1)
        {
            level_menu++;
            if (level_menu > 2)
            {
                level_menu = 0;
            }
        }
    }

    else if (key == GLUT_KEY_LEFT)
    {
        if (xstick >= 0)
        {
            xstick -= 10; // left key chaple stick ta sorbe
            if (!isStarted)
            {
                xBall -= 10; // game suur na hole ball stick er uporei thakbe
            }
        }
    }
    else if (key == GLUT_KEY_RIGHT)
    {
        if (xstick <= screenWidth - dxstick)
        {
            xstick += 10; // right key chaple stick ta sorbe
            if (!isStarted)
            {
                xBall += 10; // game suur na hole ball stick er uporei thakbe
            }
        }
    }
    else if (key == GLUT_KEY_INSERT)
    {
        if (musicOn)
        {
            musicOn = false;
            PlaySound(0, 0, 0);
        }
        else if (musicOn == false)
        {
            musicOn = true;
            PlaySound("music\\bg_music.wav", NULL, SND_LOOP | SND_ASYNC);
        }
    }
}

int main()
{

    if (musicOn)
        PlaySound("music\\bg_music.wav", NULL, SND_LOOP | SND_ASYNC);

    int sum = 20;
    for (int i = 3; i >= 0; i--)
    {
        bcordinate[i].x = 300;
        bcordinate[i].y = sum;
        sum += 100;
    }

    int sum1 = 25, sumx = 50;
    for (int i = 2; i >= 0; i--)
    {
        lcordinate[i].x = sumx;
        lcordinate[i].y = sum1;
        sum1 += 150;
    }

    int sum3 = 0, sumx2 = 0;
    for (int i = 0; i <= 2; i++)
    {
        mcordinate[i].x = sumx2;
        mcordinate[i].y = sum3;
        sumx2 += 50;
    }

    making_bricks();

    iSetTimer(25, change); // 50 milisecond por por change function call hoy.

    iInitialize(screenWidth, screenHeight, "Brick_collider");
    return 0;
}