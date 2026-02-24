/*
 * food.c
 *
 * Food handling file
 * This file manages food in the snake game
 *
 * Course: Advanced Programming Lab
 * Date: February 2026
 */

#include "snake_game.h"


// ==============================
// FOOD INITIALIZATION
// ==============================

/*
 * initialize food
 * set size and color
 * food is not active at start
 */
void Food_Initialize(Food* f)
{

    f->size.x = SQUARE_SIZE;
    f->size.y = SQUARE_SIZE;

    f->color = YELLOW;

    f->active = false;

    f->position.x = 0;
    f->position.y = 0;

}



// ==============================
// FOOD SPAWN
// ==============================

/*
 * generate food at random place
 * avoid snake body
 */
void Food_Spawn(Food* f, Snake* s, Vector2 off)
{

    int c = Utils_GetGridColumns();
    int r = Utils_GetGridRows();

    int fx, fy;

    f->active = true;


    // if snake fills grid then no food
    if(s->length >= c*r)
    {
        f->active = false;
        return;
    }


    int ok = 0;


    // find empty place
    while(!ok)
    {

        ok = 1;

        fx = GetRandomValue(0,c-1);
        fy = GetRandomValue(0,r-1);

        f->position.x = off.x + fx*SQUARE_SIZE;
        f->position.y = off.y + fy*SQUARE_SIZE;


        // check collision with snake
        for(int i=0;i<s->length;i++)
        {

            if(f->position.x == s->segments[i].position.x &&
               f->position.y == s->segments[i].position.y)
            {
                ok = 0;
                break;
            }

        }

    }

}



// ==============================
// COLLISION CHECK
// ==============================

/*
 * check snake head hit food
 */
bool Food_CheckCollision(Food* f, Vector2 p)
{

    if(f->active == false)
        return false;


    if(f->position.x == p.x &&
       f->position.y == p.y)
        return true;


    return false;

}



// ==============================
// DRAW FOOD
// ==============================

/*
 * draw food on screen
 */
void Food_Render(Food* f)
{

    if(f->active)
    {

        DrawRectangleV(
            f->position,
            f->size,
            f->color
        );

    }

}
