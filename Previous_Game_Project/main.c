#include "raylib.h"

#define MAX 400
#define SZ 31

int W = 800;
int H = 450;

int x[MAX];
int y[MAX];
int snkLen = 1;
int scr = 0;
int dir = 1;
int frm = 0;
int fx, fy;
bool food = false;
bool go = false;
bool mvOK = false;


void init();
void mv();
void chk();
void drw();
void loop();

int main()
{
    InitWindow(W,H,"snake");

    init();

    SetTargetFPS(30);

    while(!WindowShouldClose())
    {
        loop();
    }

    CloseWindow();
}


void init()
{

    snkLen=1;

    scr=0;

    dir=1;

    frm=0;

    go=false;

    for(int i=0;i<MAX;i++)
    {
        x[i]=W/2;
        y[i]=H/2;
    }

    food=false;
}


void mv()
{

    if(IsKeyPressed(KEY_RIGHT) && dir!=2 && mvOK)
    {
        dir=1;
        mvOK=false;
    }

    if(IsKeyPressed(KEY_LEFT) && dir!=1 && mvOK)
    {
        dir=2;
        mvOK=false;
    }

    if(IsKeyPressed(KEY_UP) && dir!=4 && mvOK)
    {
        dir=3;
        mvOK=false;
    }

    if(IsKeyPressed(KEY_DOWN) && dir!=3 && mvOK)
    {
        dir=4;
        mvOK=false;
    }


    if(frm%5==0)
    {

        for(int i=snkLen;i>0;i--)
        {
            x[i]=x[i-1];
            y[i]=y[i-1];
        }

        if(dir==1) x[0]+=SZ;
        if(dir==2) x[0]-=SZ;
        if(dir==3) y[0]-=SZ;
        if(dir==4) y[0]+=SZ;

        mvOK=true;
    }

    frm++;
}


void chk()
{

    if(x[0]<0) x[0]=W-SZ;
    if(x[0]>W-SZ) x[0]=0;

    if(y[0]<0) y[0]=H-SZ;
    if(y[0]>H-SZ) y[0]=0;


    for(int i=1;i<snkLen;i++)
    {
        if(x[0]==x[i] && y[0]==y[i])
            go=true;
    }


    if(!food)
    {
        fx=GetRandomValue(0,W/SZ-1)*SZ;
        fy=GetRandomValue(0,H/SZ-1)*SZ;

        food=true;
    }


    if(x[0]==fx && y[0]==fy)
    {
        snkLen++;

        scr++;

        food=false;
    }

}


void drw()
{

    BeginDrawing();

    ClearBackground(BLACK);


    if(!go)
    {

        for(int i=0;i<snkLen;i++)
        DrawRectangle(x[i],y[i],SZ,SZ,GREEN);

        DrawRectangle(fx,fy,SZ,SZ,YELLOW);

        DrawText(TextFormat("Score: %d",scr),
        10,10,20,WHITE);
    }
    else
    {
        DrawText("GAME OVER",
        W/2-100,
        H/2,
        40,
        RED);
    }
    EndDrawing();
}

void loop()
{
    if(!go)
    {
        mv();
        chk();
    }
    else
    {
        if(IsKeyPressed(KEY_ENTER))
            init();
    }
    
    drw();

}
