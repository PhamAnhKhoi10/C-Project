#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>
#include <conio.h>
#include <Windows.h>

#define MENU_TOP 2 
#define MENU_BOTTOM 22
#define MENU_LEFT 22
#define MENU_RIGHT 82
#define TOP_WALL 2
#define BOTTOM_WALL 22
#define LEFT_WALL 3
#define RIGHT_WALL 63
#define LOSE false
#define WIN 10
#define KEY_NONE -1

typedef struct
{
    int x;
    int y;
}
coordinate;

class ghost
{
public:
    int x;
    int y;
    bool moveUp = false;
    bool moveLeft = false;

    void wait(void)
    {
        Sleep(17);
    }
};

coordinate foodPosition(void);
int inputKey();
void noCursorType();
void gotoXY(int x, int y);
void changeDirection(coordinate* player);
void printPlayer(void);
void point(void);
void printWall(void);
void printGate(coordinate gate);
void teleport(coordinate *player);
void _case_(coordinate player);
void _case_ghost_(coordinate food, ghost ghost_prime);
void printGhost1(ghost *ghost_prime, coordinate food);
void printGhost2(ghost *ghost_prime, coordinate food);
void printGhost3(ghost *ghost_prime, coordinate food, int top, int bottom, int left, int right);
void intruction();
void play();
bool lose(coordinate player);

int pts = 0;
bool play_game = false;
bool congratulation = false;

coordinate gate1;
coordinate gate2;
coordinate gate3;
coordinate gate4;

ghost ghost1;
ghost ghost2;
ghost ghost3;
ghost ghost4;
ghost ghost5;
ghost ghost6;
ghost ghost7;

int main(void)
{ 	
    while (true)
    { 
        SetConsoleOutputCP(65001);

        // Teleport gate
        gate1.x = LEFT_WALL + 4;
        gate1.y = BOTTOM_WALL - 3;

        gate2.x = RIGHT_WALL - 4;
        gate2.y = TOP_WALL + 3;

        gate3.x = LEFT_WALL + 4;
        gate3.y = TOP_WALL + 3;

        gate4.x = RIGHT_WALL - 4;
        gate4.y = BOTTOM_WALL - 3;

        // Player
        coordinate player;
        player.x = LEFT_WALL + 9;
        player.y = TOP_WALL + 9;

        // Ghost
        ghost1.x = LEFT_WALL + 6 + (LEFT_WALL + RIGHT_WALL) / 10;
        ghost1.y = BOTTOM_WALL - 1;

        ghost2.x = LEFT_WALL + 6 + 5 * (LEFT_WALL + RIGHT_WALL) / 10;
        ghost2.y = BOTTOM_WALL - 1;

        ghost3.x = RIGHT_WALL - 1;
        ghost3.y = BOTTOM_WALL - 5;

        ghost4.x = LEFT_WALL + 1;
        ghost4.y = TOP_WALL + 5;

        ghost5.x = LEFT_WALL + 8 + 2 * (LEFT_WALL + RIGHT_WALL) / 10;
        ghost5.y = BOTTOM_WALL - 1;

        ghost6.x = LEFT_WALL + 3;
        ghost6.y = TOP_WALL + 1;

        ghost7.x = RIGHT_WALL - 3;
        ghost7.y = TOP_WALL + 1;


        //Menu
        bool menu = true;
        while(menu)
        {
            system("cls");
            printf("\033[0;33m");
            for (int i = MENU_LEFT; i <= MENU_RIGHT; i++)
            {
                gotoXY(i, MENU_TOP);
                printf("█");
                gotoXY(i, MENU_BOTTOM);
                printf("█");
            }

            for (int i = MENU_TOP; i <= MENU_BOTTOM; i++)
            {
                gotoXY(MENU_LEFT, i);
                printf("█");
                gotoXY(MENU_RIGHT, i);
                printf("█");
            }

            for (int i = MENU_LEFT + 2; i < MENU_RIGHT - 1; i++)
            {
                gotoXY(i, MENU_TOP + 1);
                printf("≈");
            }

            for (int i = MENU_LEFT + 2; i < MENU_RIGHT - 1; i++)
            {
                gotoXY(i, MENU_BOTTOM - 1);
                printf("≈");
            }

            for (int i = MENU_LEFT + 10; i < MENU_RIGHT - 9; i++)
            {
                gotoXY(i, MENU_TOP + 4);
                printf("■");
            }

            for (int i = MENU_LEFT + 12; i < MENU_RIGHT - 11; i++)
            {
                gotoXY(i, MENU_BOTTOM - 4);
                printf("%c", 95);
            }
            
            gotoXY(MENU_LEFT + 27 , MENU_TOP + 5);
            printf("GAME MENU\n");
            gotoXY(MENU_LEFT + 25 , MENU_TOP + 7);
            printf("1. Play\n");
            gotoXY(MENU_LEFT + 25 , MENU_TOP + 9);
            printf("2. Instruction\n");
            gotoXY(MENU_LEFT + 25 , MENU_TOP + 11);
            printf("3. Exit\n");
            gotoXY(MENU_LEFT + 25 , MENU_TOP + 13);
            printf("Your choice: ");

            int choice;
            scanf("%i", &choice);
            switch(choice)
            {
                case 1:
                    play();
                    menu = false;
                    play_game = true;
                    break;
                case 2:
                    intruction();
                    break;
                case 3:
                    exit(0);
                default:
                    system("cls");
                    printf("Invalid choice\n");
                    printf("Press ENTER to continue...");
                    getchar();
                    getchar();
            }
        }
        system("cls");
        printf("\033[0m");

        // Wall
        printWall();

        //Point
        point();

        gotoXY(player.x, player.y);
        printPlayer();

        coordinate food = foodPosition();
        gotoXY(food.x, food.y);
        printf("\033[0;37m");
        printf("❀");

        printGate(gate1);
        printGate(gate2);
        printGate(gate3);
        printGate(gate4);

        
        while (play)
        {
            printGhost3(&ghost1, food, TOP_WALL + 1, BOTTOM_WALL - 1, LEFT_WALL + 3 + (LEFT_WALL + RIGHT_WALL) / 10, RIGHT_WALL - 3 - (LEFT_WALL + RIGHT_WALL) / 10);
            printGhost1(&ghost2, food);
            printGhost2(&ghost3, food);
            printGhost2(&ghost4, food);
            printGhost1(&ghost5, food);
            printGhost1(&ghost6, food);
            printGhost1(&ghost7, food);

            noCursorType();

            // Movement
            changeDirection(&player);
            teleport(&player);

            // Point & Eat food
            if (player.x == food.x && player.y == food.y)
            {
                pts++;
                point();

                food = foodPosition();
                gotoXY(food.x, food.y);
                printf("\033[0;37m");
                printf("❀");
            }

            if (lose(player) == LOSE)
            {
                system("cls");
                printf("GAME OVER\n");
                printf("Press ENTER to continue...");
                getchar();
                pts = 0;
                break;
            }

            // Win
            if (pts == WIN)
            {   
                system("cls");  
                congratulation = true;
                pts = 0;
                play_game = false;
                break;
            }
        }

        while(congratulation)
        {
            printf("CONGRATULATION\nYOU WIN\n\n");
            printf("Press ENTER to continue...");
            getchar();
            congratulation = false;
        }

        system("cls");
    }
}

void printGate(coordinate gate)
{	printf("\033[0;32m");
    gotoXY(gate.x, gate.y);
    printf("▓");
}

void _case_(coordinate player)
{
    if(gate1.x == player.x && gate1.y == player.y)
    {
        printGate(gate1);
    }

    if(gate2.x == player.x && gate2.y == player.y)
    {
        printGate(gate2);
    }

    if(gate3.x == player.x && gate3.y == player.y)
    {
        printGate(gate3);
    }

    if(gate4.x == player.x && gate4.y == player.y)
    {
        printGate(gate4);
    }
}

void teleport(coordinate *player)
{
    if(player->x == gate1.x && player->y == gate1.y)
    {
        gotoXY(player->x, player->y);
        printf(" ");
        _case_(*player);

        player->x = gate2.x + 1;
        player->y = gate2.y;

        gotoXY(player->x, player->y);
        printPlayer();
    }

    if(player->x == gate2.x && player->y == gate2.y)
    {
        gotoXY(player->x, player->y);
        printf(" ");
        _case_(*player);

        player->x = gate3.x - 1;
        player->y = gate3.y;

        gotoXY(player->x, player->y);
        printPlayer();
    }

    if(player->x == gate3.x && player->y == gate3.y)
    {
        gotoXY(player->x, player->y);
        printf(" ");
        _case_(*player);

        player->x = gate4.x + 1;
        player->y = gate4.y;

        gotoXY(player->x, player->y);
        printPlayer();
    }

    if(player->x == gate4.x && player->y == gate4.y)
    {
        gotoXY(player->x, player->y);
        printf(" ");
        _case_(*player);

        player->x = gate1.x - 1;
        player->y = gate1.y;

        gotoXY(player->x, player->y);
        printPlayer();
    }
}

void _case_ghost_(coordinate food, ghost ghost_prime)
{
    if(food.x == ghost_prime.x && food.y == ghost_prime.y)
    {
        gotoXY(food.x, food.y);
        printf("\033[0;37m");
        printf("❀");
    }

}

void printGhost1(ghost *ghost_prime, coordinate food)
{
    if (ghost_prime->y == TOP_WALL + 1) {
        ghost_prime->moveUp = false; 
    }
    else if (ghost_prime->y == BOTTOM_WALL - 1) {
        ghost_prime->moveUp = true;
    }

    gotoXY(ghost_prime->x, ghost_prime->y);
    printf("\033[1;31m☻\033[0m");
    gotoXY(ghost_prime->x, ghost_prime->y);
    printf(" ");
    _case_ghost_(food, *ghost_prime);

    if (ghost_prime->moveUp) {
        ghost_prime->y--;
    }
    else {
        ghost_prime->y++;
    }   

    gotoXY(ghost_prime->x, ghost_prime->y);
    printf("\033[1;31m☻\033[0m");
    ghost_prime->wait();
}

void printGhost2(ghost *ghost_prime, coordinate food)
{
    if (ghost_prime->x == LEFT_WALL + 1) {
        ghost_prime->moveLeft = false;
    }
    else if (ghost_prime->x == RIGHT_WALL - 1) {
        ghost_prime->moveLeft = true; 
    }

    gotoXY(ghost_prime->x, ghost_prime->y);
    printf("\033[1;31m☻\033[0m");
    gotoXY(ghost_prime->x, ghost_prime->y);
    printf(" ");
    _case_ghost_(food, *ghost_prime);

    if (ghost_prime->moveLeft) 
    {
        ghost_prime->x--;
    }
    else 
    {
        ghost_prime->x++;
    }   
    gotoXY(ghost_prime->x, ghost_prime->y);
    printf("\033[1;31m☻\033[0m");
}

void printGhost3(ghost *ghost_prime, coordinate food, int top, int bottom, int left, int right)
{
    static int direction = 0; 

    gotoXY(ghost_prime->x, ghost_prime->y);
    printf(" ");
    _case_ghost_(food, *ghost_prime);

    if (ghost_prime->x == right && direction == 0) 
    {
        direction = 3;
    }
    else if (ghost_prime->y == bottom && direction == 1) 
    {
        direction = 0;
    }
    else if (ghost_prime->x == left && direction == 2) 
    {
        direction = 1;
    }
    else if (ghost_prime->y == top && direction == 3) 
    {
        direction = 2;
    }

    switch (direction) {
        case 0: // moving right
            ghost_prime->x++;
            break;
        case 1: // moving down
            ghost_prime->y++;
            break;
        case 2: // moving left
            ghost_prime->x--;
            break;
        case 3: // moving up
            ghost_prime->y--;
            break;
    }
    gotoXY(ghost_prime->x, ghost_prime->y);
    printf("\033[1;31m☻\033[0m");
}


void printWall(void)
{
   printf("\033[0;34m");

    for(int i = LEFT_WALL + 1; i <= RIGHT_WALL - 1; i++)
    {
        gotoXY(i, TOP_WALL);
        printf("═");
        gotoXY(i, BOTTOM_WALL);
        printf("═");
    }

    for(int j = TOP_WALL + 1; j <= BOTTOM_WALL - 1; j++)
    {
        gotoXY(LEFT_WALL, j);
        printf("║\n");
        gotoXY(RIGHT_WALL, j);
        printf("║\n");
    }

    gotoXY(LEFT_WALL, TOP_WALL);
    printf("╔");
    gotoXY(LEFT_WALL, BOTTOM_WALL);
    printf("╚");
    gotoXY(RIGHT_WALL, BOTTOM_WALL);
    printf("╝");
    gotoXY(RIGHT_WALL, TOP_WALL);
    printf("╗");

    int collumn = (LEFT_WALL + RIGHT_WALL) / 10;
    int row = (TOP_WALL + BOTTOM_WALL) / 10;

    for (int i = TOP_WALL + row; i <= BOTTOM_WALL - row; i += row)
    {
        for (int j = LEFT_WALL + collumn; j <= RIGHT_WALL - collumn; j += collumn)
        {
            gotoXY(j, i);
            printf("✛");
        }
    }
}

coordinate foodPosition(void)
{
    coordinate food;

    srand(time(NULL));
    food.x = LEFT_WALL + 1 + rand() % (RIGHT_WALL - LEFT_WALL - 1);
    food.y = TOP_WALL + 1 + rand() % (BOTTOM_WALL - TOP_WALL - 1);

    int collumn = (LEFT_WALL + RIGHT_WALL) / 10;
    int row = (TOP_WALL + BOTTOM_WALL) / 10;
    
    for (int i = TOP_WALL + row; i <= BOTTOM_WALL - row; i += row)
    {
        for (int j = LEFT_WALL + collumn; j <= RIGHT_WALL - collumn; j += collumn)
        {
            while ((food.x == j && food.y == i) || (food.x == gate1.x && food.y == gate1.y) || (food.x == gate2.x && food.y == gate2.y) || (food.x == gate3.x && food.y == gate3.y) || (food.x == gate4.x && food.y == gate4.y))
            {
                food.x = LEFT_WALL + 1 + rand() % (RIGHT_WALL - LEFT_WALL - 1);
                food.y = TOP_WALL + 1 + rand() % (BOTTOM_WALL - TOP_WALL - 1);
            }
        }
    }

    return food;
}

void point(void)
{
    gotoXY(RIGHT_WALL + 5, TOP_WALL + 2);
    printf("\033[1;33m"); 
    printf("POINTS: %i", pts);
}

void printPlayer(void)
{
	noCursorType();
    printf("\033[0;33m");
    printf("♛");
}

bool lose(coordinate player)
{
    if (player.x == LEFT_WALL || player.x == RIGHT_WALL || player.y == TOP_WALL || player.y == BOTTOM_WALL)
    {
        return false;
    }

    int collumn = (LEFT_WALL + RIGHT_WALL) / 10;
    int row = (TOP_WALL + BOTTOM_WALL) / 10;

    for (int i = TOP_WALL + row; i <= BOTTOM_WALL - row; i += row)
    {
        for (int j = LEFT_WALL + collumn; j <= RIGHT_WALL - collumn; j += collumn)
        {
            if (player.x == j && player.y == i)
            {
                return false;
            }
        }
    }

    if (player.x == ghost1.x && player.y == ghost1.y)
    {
        return false;
    }
    if (player.x == ghost2.x && player.y == ghost2.y)
    {
        return false;
    }
    if (player.x == ghost3.x && player.y == ghost3.y)
    {
        return false;
    }
    if (player.x == ghost4.x && player.y == ghost4.y)
    {
        return false;
    }
    if (player.x == ghost5.x && player.y == ghost5.y)
    {
        return false;
    }
    if (player.x == ghost6.x && player.y == ghost6.y)
    {
        return false;
    }
    if (player.x == ghost7.x && player.y == ghost7.y)
    {
        return false;
    }
    
    return true;
}

void changeDirection(coordinate* player)
{
   int key = tolower(inputKey());
    if (key == 'a')
    {
        gotoXY(player->x, player->y);
        printf(" ");

        player->x--;

        gotoXY(player->x, player->y);
        printPlayer();

    }
    else if (key == 'd')
    {
        gotoXY(player->x, player->y);
        printf(" ");

        player->x++;

        gotoXY(player->x, player->y);
        printPlayer();

    }
    else if (key == 'w')
    {
        gotoXY(player->x, player->y);
        printf(" ");

        player->y--;
        gotoXY(player->x, player->y);
        printPlayer();

    }
    else if (key == 's')
    {
        gotoXY(player->x, player->y);
        printf(" ");

        player->y++;

        gotoXY(player->x, player->y);
        printPlayer();

    }

}

int inputKey()
{
    if (_kbhit())
    {
        int key = _getch();

        if (key == 224)
        {
            key = _getch();
            return key + 1000;
        }

        return key;
    }
    else
    {
        return KEY_NONE;
    }

    return KEY_NONE;
}


void intruction()
{
    system("cls");
    printf("Instruction\n\n");
    printf("Press \"a\" to move left\n");
    printf("Press \"d\" to move right\n");
    printf("Press \"w\" to move up\n");
    printf("Press \"s\" to move down\n\n");
    printf("There are four teleport gates in the game, use them to switch your position\n");
    printf("You have to collect the food. When you reach 10 points, you will win\n");
    printf("If you touch the ghost or the wall and the barrier, you will lose\n\n");
    printf("Good luck!\n\n");
    printf("Press ENTER to continue...");
    getchar();
    getchar();
    system("cls");
}

void play()
{
    system("cls");
    printf("Play\n");
    printf("Press ENTER to continue...");
    getchar();
    getchar();
    system("cls");
}


void gotoXY(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void noCursorType()
{
    CONSOLE_CURSOR_INFO info;
    info.bVisible = FALSE;
    info.dwSize = 20;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}


