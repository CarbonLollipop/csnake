#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

void quit(int score) {
    curs_set(1);
    endwin();
    printf("Game over! Score: %d\n", score);
    exit(0);
}

int main() {

    int direction = 0;

    /* 0 = left
     * 1 = down
     * 2 = up
     * 3 = right
     */

    srand(time(NULL));

    initscr();
    signal(SIGINT, quit);
    nodelay(stdscr, TRUE);

    noecho();
    curs_set(0);

    int x, y;
    getmaxyx(stdscr, y, x);

    char* horizontalWall = malloc(x + 1);

    for (int i = 0; i < x; i++)
        horizontalWall[i] = '-';

    horizontalWall[x] = '\0';
    printw("%s", horizontalWall);

    for (int i = 0; i < y - 2; i++) {
        move(i + 1, 0);
        printw("[");
        move(i + 1, x - 1);
        printw("]");
    }

    move(y - 1, 0);
    printw("%s", horizontalWall);

    int snakePos[2] = {y / 2, x / 2};
    int foodPos[2] = {rand() % (y - 2) + 1, rand() % (x - 2) + 1};
    int score = 0;
   
    while(1) {
        int c = getch();
 
        move(snakePos[0], snakePos[1]);
        printw(" ");

        if (c == 'q')
            break;
        if (c == 'h')
           direction = 0;
        if (c == 'j')
            direction = 1;
        if (c == 'k')
            direction = 2;
        if (c == 'l')
            direction = 3;
   
        if (direction == 0)
            snakePos[1]--;
        if (direction == 1)
            snakePos[0]++;
        if (direction == 2)
            snakePos[0]--;
        if (direction == 3)
            snakePos[1]++;

        if (snakePos[0] == 0 || snakePos[0] == y - 1 || snakePos[1] == 0 || snakePos[1] == x - 1)
            break;

        if (snakePos[0] == foodPos[0] && snakePos[1] == foodPos[1]) {
            foodPos[0] = rand() % (y - 2) + 1;
            foodPos[1] = rand() % (x - 2) + 1;
            score++;
        }


        move(snakePos[0], snakePos[1]);
        printw("#");
     
        move(foodPos[0], foodPos[1]);
        printw("@");

        refresh();
        usleep(100000);
    }

    quit(score);
}

