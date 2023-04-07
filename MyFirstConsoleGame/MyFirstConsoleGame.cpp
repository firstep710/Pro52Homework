#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <random>
#include <time.h>
#include <math.h>
#include <iostream>

#include "InputSystem.h" // 기능 별로 모듈화를 한다는 개념에 대해 생각해 봅시다!
#include "RenderSystem.h"

//#define NORMAL_BULLET 1
#define WIDTH 118
#define HEIGHT 29
#define PLAYER 1
#define ENEMY 2
#define BULLET 3


int bulletDirection[8][2] = { {0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1} };
int map[HEIGHT][WIDTH] = {};

using namespace std;

namespace global
{
    namespace time
    {
        ULONGLONG previousTime;
        ULONGLONG currentTime;
        ULONGLONG deltaTime;

        int updateCount;
        int fixedUpdateCount;

        void InitTime()
        {
            currentTime = previousTime = GetTickCount64();
        }

        void UpdateTime()
        {
            previousTime = currentTime;
            currentTime = GetTickCount64();

            deltaTime = currentTime - previousTime;
        }

        ULONGLONG GetDeltaTime()
        {
            return deltaTime;
        }
    };

    
 

    SMALL_RECT playerMovableRect = { 5, 5, 30, 30 }; // @SEE StartGame()

    const int playerMoveSpeed = 200;

    // 노가다로-0- 적을 만들어 봅시다.
    const int ENEMY_CNT = 10;
    int eCnt;
    struct Enemy
    {
        COORD localPos;
        int maxHp;
        int hp;
        bool isLiving = true;
        char  character;
    };
    struct Player
    {
        COORD   curPlayerPos;  // 현재 플레이어 위치
        double playerHp;

    };
    struct Bullet
    {
        COORD position;
        bool isHit;
        char bullet;
        int direction;
    };
    struct BulletStat
    {
        int cooldown;
        bool open = false;
    };
    BulletStat b_stat[2];
    vector<Bullet> bullet;
    
    Player player;
    Enemy consoleEnemy[ENEMY_CNT];
    //Bullet bullet[2];
};

void PlayerHit()
{
    if (map[global::player.curPlayerPos.Y - 1][global::player.curPlayerPos.X] == 2)
        global::player.playerHp -= 0.1f;
    if (map[global::player.curPlayerPos.Y - 1][global::player.curPlayerPos.X + 1] == 2)
        global::player.playerHp -= 0.1f;
    if (map[global::player.curPlayerPos.Y - 1][global::player.curPlayerPos.X - 1] == 2)
        global::player.playerHp -= 0.1f;
    if (map[global::player.curPlayerPos.Y][global::player.curPlayerPos.X + 1] == 2)
        global::player.playerHp -= 0.1f;
    if (map[global::player.curPlayerPos.Y][global::player.curPlayerPos.X -1] == 2)
        global::player.playerHp -= 0.1f;
    if (map[global::player.curPlayerPos.Y + 1][global::player.curPlayerPos.X + 1] == 2)
        global::player.playerHp -= 0.1f;
    if (map[global::player.curPlayerPos.Y + 1][global::player.curPlayerPos.X - 1] == 2)
        global::player.playerHp -= 0.1f;
    if (map[global::player.curPlayerPos.Y + 1][global::player.curPlayerPos.X] == 2)
        global::player.playerHp -= 0.1f;
}
void Clamp(short& n, short min, short max) // 레퍼런스 타입에 대해 배워 봅시다.
{
    if (n < min) n = min;
    if (n > max) n = max;
}
bool IsMaxEnemy()
{
    return global::eCnt >= global::ENEMY_CNT;
}

void DrawPlayer()
{
    render::ScreenDraw(global::player.curPlayerPos.X, global::player.curPlayerPos.Y, '@');
}

void DrawEnemy()
{
    int x = 0; int y = 0;

    // 노가다로 그리는 적
    for (int i = 0; i < global::ENEMY_CNT; i++)
    {
        x =  global::consoleEnemy[i].localPos.X;
        y =  global::consoleEnemy[i].localPos.Y;

        render::ScreenDraw(x, y, global::consoleEnemy[i].character);
    }
}
void DrawBullet()
{
    for (int i = 0; i < global::bullet.size(); i++)
    {
        render::ScreenDraw(global::bullet[i].position.X, global::bullet[i].position.Y, global::bullet[i].bullet);
    }
    
}
void UpdatePlayerPosition()
{
    if (global::input::IsEscapeCmdOn())
    {
        global::input::Set(global::input::ESCAPE_KEY_INDEX, false);

        //TODO: 게임 화면 전환이나 종료 처리

        return; // 다른 키 입력을 신경 쓸 필요가 없어요.
    }

    if (global::input::IsLeftCmdOn())
    {
        global::input::Set(global::input::USER_CMD_LEFT, false);

        global::player.curPlayerPos.X--;
        Clamp(global::player.curPlayerPos.X, global::playerMovableRect.Left, global::playerMovableRect.Right);
    }

    if (global::input::IsRightCmdOn())
    {
        global::input::Set(global::input::USER_CMD_RIGHT, false);

        global::player.curPlayerPos.X++;
        Clamp(global::player.curPlayerPos.X, global::playerMovableRect.Left, global::playerMovableRect.Right);
    }
    if (global::input::IsUpCmdOn())
    {
        global::input::Set(global::input::USER_CMD_UP, false);

        global::player.curPlayerPos.Y--;
        Clamp(global::player.curPlayerPos.Y, global::playerMovableRect.Top, global::playerMovableRect.Bottom);
    }
    if (global::input::IsDownCmdOn())
    {
        global::input::Set(global::input::USER_CMD_DOWN, false);

        global::player.curPlayerPos.Y++;
        Clamp(global::player.curPlayerPos.Y, global::playerMovableRect.Top, global::playerMovableRect.Bottom);
    }

}
void BulletMove();
COORD CheckShortest();

void Fire()
{
    static ULONGLONG elapsedTime;

    COORD pos = CheckShortest();
    bool a = false;
    elapsedTime += global::time::GetDeltaTime();
    double gradient = (double)(global::player.curPlayerPos.Y - pos.Y) / (global::player.curPlayerPos.X - pos.X);
    
        if (elapsedTime >= global::b_stat[0].cooldown * 100)
        {
            a = true;
            if (a)
            {
                 
            }
            elapsedTime = 0;


        }
        if (global::input::IsSpaceCmdOn())
        {
            a = false;
            global::input::Set(global::input::USER_CMD_SPACE, false);
            char buffer[64] = "";
            sprintf_s(buffer, " %d ", a);
            global::Bullet tan;
            tan.bullet = '*';
            //tan.cooldown = 5;
            tan.position = global::player.curPlayerPos;
            tan.isHit = false;


            if (round(gradient) < 5 && round(gradient) >= 1) {
                if (global::player.curPlayerPos.X > pos.X)
                {
                    tan.direction = 7;
                }
                else
                {
                    tan.direction = 3;
                }
            }
            else if (round(gradient) > -5 && round(gradient) <= -1)
            {
                if (global::player.curPlayerPos.X > pos.X)
                {
                    tan.direction = 5;
                }
                else
                {
                    tan.direction = 1;
                }
            }
            else
            {
                if ((round(gradient) >= 5 || round(gradient) <= -5) && global::player.curPlayerPos.Y < pos.Y)
                    tan.direction = 4;
                else if ((round(gradient) >= 5 || round(gradient) <= -5) && global::player.curPlayerPos.Y > pos.Y)
                    tan.direction = 0;
                else if (round(gradient) < 1 && round(gradient) > -1 && global::player.curPlayerPos.X < pos.X)
                    tan.direction = 2;
                else if (round(gradient) < 1 && round(gradient) > -1 && global::player.curPlayerPos.X > pos.X)
                    tan.direction = 6;
            }

            global::bullet.push_back(tan);
            render::ScreenDraw(90, 0, buffer);
            //global::bullet.direction = global::player.curPlayerPos;

        }
    

}

void CheckHit()
{

    vector<int> c;
    for (int i = 0; i < global::bullet.size(); i++)
    {
        if (global::bullet[i].position.X == 1 || global::bullet[i].position.Y == 1 || global::bullet[i].position.X == 118 || global::bullet[i].position.Y == 28)
        {
            global::bullet[i].isHit = true;
            c.push_back(i);

        }/*
        else if (map[global::bullet[i].position.Y][global::bullet[i].position.X] == ENEMY)
        {
            global::bullet[i].isHit = true;
            c.push_back(i);

        }
        else  if (map[global::bullet[i].position.Y - 1][global::bullet[i].position.X] == ENEMY)
        {
            global::bullet[i].isHit = true;
            c.push_back(i);
        }
        else  if (map[global::bullet[i].position.Y - 1][global::bullet[i].position.X - 1] == ENEMY)
        {
            global::bullet[i].isHit = true;
            c.push_back(i);
        }
        else  if (map[global::bullet[i].position.Y - 1][global::bullet[i].position.X + 1] == ENEMY)
        {
            global::bullet[i].isHit = true;
            c.push_back(i);
        }
        else  if (map[global::bullet[i].position.Y][global::bullet[i].position.X - 1] == ENEMY)
        {
            global::bullet[i].isHit = true;
            c.push_back(i);
        }
        else  if (map[global::bullet[i].position.Y + 1][global::bullet[i].position.X - 1] == ENEMY)
        {
            global::bullet[i].isHit = true;
            c.push_back(i);
        }
        else  if (map[global::bullet[i].position.Y + 1][global::bullet[i].position.X + 1] == ENEMY)
        {
            global::bullet[i].isHit = true;
            c.push_back(i);
        }
        else  if (map[global::bullet[i].position.Y][global::bullet[i].position.X + 1] == ENEMY)
        {
            global::bullet[i].isHit = true;
            c.push_back(i);
        }*/
    }
    for (int i = 0; i < c.size(); i++)
    {
        map[global::bullet[i].position.Y][global::bullet[i].position.X] = 0;
        global::bullet.erase(global::bullet.begin()+c[i]);
    }
    
}

void BulletMove()
{
    for (int i = 0; i < global::bullet.size(); i++)
    {
        if (global::bullet[i].isHit == false)
        {
            map[global::bullet[i].position.Y][global::bullet[i].position.X] = 0;
            global::bullet[i].position.X += bulletDirection[global::bullet[i].direction][0];
            global::bullet[i].position.Y += bulletDirection[global::bullet[i].direction][1];
            CheckHit();
            
        }
    }
    for (int i = 0; i < global::bullet.size(); i++) {
        if (global::bullet[i].isHit == false)
        {
            map[global::bullet[i].position.Y][global::bullet[i].position.X] = BULLET;
        }

    }
        
        
    
}
void UpdatePlayer()
{
    // 키 입력과 화면 출력과 게임 로직 갱신을 분리해서 생각해 봅시다.
    static ULONGLONG elapsedTime;

    elapsedTime += global::time::GetDeltaTime();

    while (elapsedTime >= global::playerMoveSpeed)
    {
        map[global::player.curPlayerPos.Y][global::player.curPlayerPos.X] = 0;
        UpdatePlayerPosition();
        Fire();
        PlayerHit();
        
        map[global::player.curPlayerPos.Y][global::player.curPlayerPos.X] = PLAYER;
        elapsedTime -= global::playerMoveSpeed;
    }
}

COORD ChasePlayer(global::Enemy& enemy);
bool CheckMove(COORD position);
bool EnemyHit(COORD position)
{
    for (int i = 0; i < global::bullet.size(); i++)
    {
        if (sqrt(pow((double)position.X - global::bullet[i].position.X, 2) + pow((double)position.Y - global::bullet[i].position.Y, 2))<=sqrt(2))
        {
            map[global::bullet[i].position.Y][global::bullet[i].position.X] = 0;
            global::bullet.erase(global::bullet.begin() + i);
            return true;
        }
    }
    return false;
    /*if(map[position.Y][position.X] == BULLET)
    {
        return true;
    }*/
    /*else  if (map[position.Y - 1][position.X] == BULLET)
    {
        return true;
    }
    else  if (map[position.Y - 1][position.X-1] == BULLET)
    {
        return true;
    }
    else  if (map[position.Y - 1][position.X+1] == BULLET)
    {
        return true;
    }
    else  if (map[position.Y][position.X-1] == BULLET)
    {
        return true;
    }
    else  if (map[position.Y+1][position.X - 1] == BULLET)
    {
        return true;
    }
    else  if (map[position.Y+1][position.X + 1] == BULLET)
    {
        return true;
    }
    else  if (map[position.Y][position.X + 1] == BULLET)
    {
        return true;
    }
    else return false;*/
}
void UpdateEnemy()
{
    
    for (int i = 0; i < 10; i++)
    {
        map[global::consoleEnemy[i].localPos.Y][global::consoleEnemy[i].localPos.X] = 0;
        global::consoleEnemy[i].localPos = ChasePlayer(global::consoleEnemy[i]);
          
            if (!global::consoleEnemy[i].isLiving)
            {
                map[global::consoleEnemy[i].localPos.Y][global::consoleEnemy[i].localPos.X] = 0;
                global::consoleEnemy[i].character++;
                global::consoleEnemy[i].maxHp++;
                global::consoleEnemy[i].hp = global::consoleEnemy[i].maxHp;
                global::consoleEnemy[i].localPos.X = rand() % (global::playerMovableRect.Right - global::playerMovableRect.Left) + global::playerMovableRect.Left;
                global::consoleEnemy[i].localPos.Y = rand() % (global::playerMovableRect.Bottom - global::playerMovableRect.Top) + global::playerMovableRect.Top;
                global::consoleEnemy[i].isLiving = true;
            }
   
        map[global::consoleEnemy[i].localPos.Y][global::consoleEnemy[i].localPos.X] = ENEMY;
    }
    DrawEnemy();
}

bool CheckMove( COORD position)
{
    if (map[position.Y][position.X]==0) {
        return true;
    }
    else
        return false;
}

COORD CheckShortest()
{
    int result = 0;
    int shortest = 50;
    for (int i = 0; i < global::ENEMY_CNT; i++)
    {
        int distance = sqrt(pow(global::consoleEnemy[i].localPos.X - global::player.curPlayerPos.X, 2) + pow(global::consoleEnemy[i].localPos.Y - global::player.curPlayerPos.Y, 2));
        if (distance < shortest)
        {
            shortest = distance;
            result = i;
        }
    }
    return global::consoleEnemy[result].localPos;
}

COORD ChasePlayer(global::Enemy& enemy)
{
    COORD e_position = enemy.localPos;
    //몬스터와 플레이어 사이의 기울기에 따라 몬스터가 플레이어를 쫒아오는 방향 정함
    double gradient = (double)  (global::player.curPlayerPos.Y - e_position.Y) / (global::player.curPlayerPos.X - e_position.X);
    

    if (ceil(gradient)<5 && ceil(gradient) >= 1)
    {
        if (global::player.curPlayerPos.X > e_position.X)
        {
            e_position.X++;
            e_position.Y++;
            
           
        }
        else
        {
            e_position.X--;
            e_position.Y--;
        }
    }
    else if (floor(gradient)>-5 && floor(gradient) <= -1)
    {
        if (global::player.curPlayerPos.X > e_position.X)
        {
            e_position.X++;
            e_position.Y--;
           
        }
        else
        {
            e_position.X--;
            e_position.Y++;
        }
    }
    else {
        if ((ceil(gradient) >= 5 || floor(gradient) <= -5) && global::player.curPlayerPos.Y < e_position.Y)
            e_position.Y--;
        else if ((ceil(gradient) >= 5 || floor(gradient) <= -5) && global::player.curPlayerPos.Y > e_position.Y)
            e_position.Y++;
        else if (global::player.curPlayerPos.Y == e_position.Y && global::player.curPlayerPos.X < e_position.X)
            e_position.X--;
        else if (global::player.curPlayerPos.Y == e_position.Y && global::player.curPlayerPos.X > e_position.X)
            e_position.X++;
    }
    if (EnemyHit(e_position))
    {
        enemy.hp--;
        if (enemy.hp == 0)
        {
            enemy.isLiving = false;
        }
        else {
            return e_position;
        }

    }


    if (CheckMove(e_position))   
    {
        return e_position;
    }
    
    else
        return enemy.localPos;
}
void StartGame()
{
    render::InitScreen();

    global::playerMovableRect = render::GetPlayerMovableRect();

    render::DrawBorder(); // 벽을 그려 놓자!

    // 플레이어 시작 위치 설정
    global::player.curPlayerPos.X = global::playerMovableRect.Left + (global::playerMovableRect.Left + global::playerMovableRect.Right) / 2;
    global::player.curPlayerPos.Y = global::playerMovableRect.Bottom - 2;
    global::player.playerHp = 100.0f;

    global::b_stat[0].open = true;
    global::b_stat[0].cooldown = 5;

    srand((unsigned int)time(NULL));

    // 노가다로 만드는 적
    for (int i = 0; i < global::ENEMY_CNT; i++)
    {
        global::consoleEnemy[i].character = 'A';
        global::consoleEnemy[i].localPos.X = rand() % (global::playerMovableRect.Right - global::playerMovableRect.Left)+global::playerMovableRect.Left;
        global::consoleEnemy[i].localPos.Y = rand() % (global::playerMovableRect.Bottom-global::playerMovableRect.Top)+global::playerMovableRect.Top; // Y 는 고정.
        global::consoleEnemy[i].maxHp = 1;
        global::consoleEnemy[i].hp = global::consoleEnemy[i].maxHp;
    }

    

    DrawPlayer();

    DrawEnemy();
}

void EndGame()
{
    render::ScreenRelease();
}

void ProcessInput()
{
    global::input::UpdateInput();
}

void PrintCountsPerSecond();
void PrintPlayerPostion();


void Render()
{
    render::ScreenClear();

    //PrintCountsPerSecond();

    PrintPlayerPostion();

    

    
    if(!global::bullet.size()==0)
        DrawBullet();
    DrawEnemy();

    DrawPlayer();

    render::DrawBorder();

    render::ScreenFlipping();


}

bool IsGameRun()
{
    if (global::player.playerHp >= 0.0f)
        return true;
    else
        return false;
}

void FixeUpdate()
{
    static ULONGLONG elapsedTime;
    static ULONGLONG elapsedTime2;
    

    elapsedTime += global::time::GetDeltaTime();
    elapsedTime2 += global::time::GetDeltaTime();

    while (elapsedTime2 >= 150)
    {
        if (!global::bullet.size() == 0)
            BulletMove();
        elapsedTime2 -= 150;
    }
    while (elapsedTime >= 300) //1초
    {
        global::time::fixedUpdateCount += 1;

        UpdateEnemy();
        
        elapsedTime -= 300;
    }
    
}


void Update()
{
    global::time::updateCount += 1;
    
    UpdatePlayer();
   
}

void PrintPlayerPostion()
{
    char buffer[64] = "";
    sprintf_s(buffer, "Player Position , hp ((%d, %d),%f)", global::player.curPlayerPos.X, global::player.curPlayerPos.Y,global::player.playerHp);

    render::ScreenDraw(0, 0, buffer);
}

void PrintCountsPerSecond()
{
    static ULONGLONG elapsedTime;

    elapsedTime += global::time::GetDeltaTime();
    if (elapsedTime >= 1000)
    {
        char buffer[64] = "";

        sprintf_s(buffer, "updateCount : %d fixedUpdateCount : %d  delta %d\n", global::time::updateCount, global::time::fixedUpdateCount, global::time::GetDeltaTime());

        render::ScreenDraw(0, 0, buffer);

        elapsedTime = 0;
        global::time::updateCount = 0;
        global::time::fixedUpdateCount = 0;
    }
}

int main()
{
    global::time::InitTime();

    StartGame();

    while (IsGameRun())
    {
        global::time::UpdateTime();

        ProcessInput();
        FixeUpdate();

        Update();
        Render();
    }

    EndGame();

    return 0;
}
