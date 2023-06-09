#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

void quit(int score) {
    erase();
    curs_set(1);
    endwin();
    printf("Game over! Score: %d\n", score);
    exit(0);
}

int main(int argc, char** argv) {
    srand(time(NULL));

    int direction = 0;

    /* 
     * 0 = left
     * 1 = down
     * 2 = up
     * 3 = right
     */

    initscr();
    signal(SIGINT, quit);
    nodelay(stdscr, TRUE);
    start_color();
    keypad(stdscr, TRUE);

    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);

    noecho();
    curs_set(0);

    int x, y;
    getmaxyx(stdscr, y, x);

    char* horizontalWall = malloc(x + 1);

    for (int i = 0; i < x; i++)
        horizontalWall[i] = '~';

    horizontalWall[x] = '\0';
    printw("%s", horizontalWall);

    for (int i = 0; i < y - 2; i++) {
        move(i + 1, 0);
        printw("|");
        move(i + 1, x - 1);
        printw("|");
    }

    move(y - 1, 0);
    printw("%s", horizontalWall);

    int snakePos[2] = {y / 2, x / 2};
    // MEGA HAX
    int score = 2;
    int snakeBody[2048][2];

    int food = (argc > 1) ? atoi(argv[1]) : 20;

    for (int i = 0; i < food; i++) {
        int curx, cury;
        do {
            curx = rand() % (x - 2) + 1;
            cury = rand() % (y - 2) + 1;
            move(cury, curx);
        } while (mvinch(cury, curx) != ' ');

        move(cury, curx);
        printw("@");
    }

    while(1) {
        int c = getch();

        if (c == 'q')
            break;
        if ((c == 'h' || c == KEY_LEFT) && direction != 3)
           direction = 0;
        if ((c == 'j' || c == KEY_DOWN) && direction != 2)
            direction = 1;
        if ((c == 'k' || c == KEY_UP) && direction != 1)
            direction = 2;
        if ((c == 'l' || c == KEY_RIGHT) && direction != 0)
            direction = 3;

        if (direction == 0)
            snakePos[1]--;
        if (direction == 1)
            snakePos[0]++;
        if (direction == 2)
            snakePos[0]--;
        if (direction == 3)
            snakePos[1]++;

        for (int i = 0; i < score; i++) {
            if (snakePos[0] == snakeBody[i][0] && snakePos[1] == snakeBody[i][1])
                quit(score - 2); 
        }

        if(mvinch(snakePos[0], snakePos[1]) == '@') {
            move(rand() % (y - 2) + 1, rand() % (x - 2) + 1);
            printw("@");
            score++;
        }

        if (snakePos[0] == 0 || snakePos[0] == y - 1 || snakePos[1] == 0 || snakePos[1] == x - 1)
            break;

        for (int i = 0; i < score; i++) {
            move(snakeBody[i][0], snakeBody[i][1]);
            printw(" ");
        }

        for (int i = score; i > 0; i--) {
            snakeBody[i][0] = snakeBody[i - 1][0];
            snakeBody[i][1] = snakeBody[i - 1][1];
        }

        attron(COLOR_PAIR(1));

        for (int i = 0; i < score; i++) {
            move(snakeBody[i][0], snakeBody[i][1]);
            printw("o");
        }

        attroff(COLOR_PAIR(1));

        snakeBody[0][0] = snakePos[0];
        snakeBody[0][1] = snakePos[1];

        move(snakePos[0], snakePos[1]);
        attron(COLOR_PAIR(1));   
        printw("#");
        attroff(COLOR_PAIR(1));

        refresh();
        
        usleep(70000 - (score * 1000));
    }

    quit(score - 2);
}

