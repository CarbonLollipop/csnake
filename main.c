// You can configure the amount of food by passing an argument to the program
// Configure everything else here

// The color of the snake
#define SNAKE_COLOR COLOR_GREEN
#define SNAKE_BACKGROUND_COLOR COLOR_BLACK

// For example, this value set to 2 would make the snake two times faster
#define GAME_SPEED_MULTIPLIER 1

// How much score you get for eating food
#define FOOD_SCORE 1

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

void quit() {
    endwin(); 
    curs_set(1);
    exit(0);
}

void type(char string[]) {
	int len = strlen(string);

	for(int i = 0; i < len; i++) {
		int isPunct = string[i] == '!' || string[i] == '.' || string[i] == ':';
		
		printw("%c", string[i]);
		refresh();
		usleep(50000 + (isPunct * 100000));
	}
}

void drawFood(int x, int y) {
    int foodx, foody;
    
    do {
        foodx = rand() % (x - 2) + 1;
        foody = rand() % (y - 2) + 1;
        move(foody, foodx);
    } while (mvinch(foody, foodx) != ' ');

    printw("@");
}

int main(int argc, char** argv) {
    srand(time(NULL));

    initscr();
    signal(SIGINT, quit);
    nodelay(stdscr, TRUE);
    start_color();
    keypad(stdscr, TRUE);

    init_pair(1, SNAKE_COLOR, SNAKE_BACKGROUND_COLOR);
    init_pair(2, COLOR_RED, SNAKE_BACKGROUND_COLOR);

    noecho();
    curs_set(0);

    int x, y;
    getmaxyx(stdscr, y, x);

    int snakePos[2] = {y / 2, x / 2};
    // MEGA HAX
    int score = 2;
    int snakeBody[2048][2];

    int food = (argc > 1) ? atoi(argv[1]) : 20;

    for (int i = 0; i < food; i++) {
        drawFood(x, y);
    }
 
    int direction = rand() % 4;

    /* 
     * 0 = left
     * 1 = down
     * 2 = up
     * 3 = right
     */

    int lost = 0;

    while(1) {
        int c = getch();

        if (c == 'q')
            quit();
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
                lost = 1;
        }

        if(mvinch(snakePos[0], snakePos[1]) == '@') {
            score += FOOD_SCORE;
            drawFood(x, y);
        }

        if (snakePos[0] == 0 || snakePos[0] == y - 1 || snakePos[1] == 0 || snakePos[1] == x - 1)
            lost = 1;

        for (int i = 0; i < score; i++) {
            if(mvinch(snakeBody[i][0], snakeBody[i][1]) != '|') 
            printw(" ");
        }

        for (int i = score; i > 0; i--) {
            snakeBody[i][0] = snakeBody[i - 1][0];
            snakeBody[i][1] = snakeBody[i - 1][1];
        }

        lost ? attron(COLOR_PAIR(2)) : attron(COLOR_PAIR(1));

        for (int i = 0; i < score; i++) {
            move(snakeBody[i][0], snakeBody[i][1]);
            printw("o");
        }

        snakeBody[0][0] = snakePos[0];
        snakeBody[0][1] = snakePos[1];

        move(snakePos[0], snakePos[1]);

        printw("#");
       
        lost ? attroff(COLOR_PAIR(2)) : attroff(COLOR_PAIR(1));

        refresh();
      
        if(lost)
            break;

        usleep(((50000 - (score * 1000) > 20000 ? 50000 - (score * 1000) : 20000) * 2) / GAME_SPEED_MULTIPLIER);
    }
    
    move(y / 2 - 1, (x / 2) - 10);
    type("Game over! Score: ");

    // oh dear lord the hax
    printw("%d", score);
    refresh();
    
    usleep(1000000);
    
    move((y / 2) + 1, (x / 2) - 10 - 1);
    
    refresh();
    
    type("Hit any key to exit...");
    nodelay(stdscr, FALSE);

    curs_set(1);
    refresh();
    getch();
    
    quit(score - 2);
}

