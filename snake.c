#include <assert.h>
#include <locale.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

typedef enum {UP, DOWN, LEFT, RIGHT} Direction;
typedef struct {int x, y;} Pos;
typedef struct SnakePart {Pos p; struct SnakePart *next, *prev;} SnakePart;

int main() {
	setlocale(LC_ALL, "");

	initscr();
	cbreak();
	noecho();
	nonl();
	intrflush(stdscr, FALSE);
	timeout(50);
	curs_set(0);

	srand(time(NULL));

	int i,j,size=0;
	Direction dir = UP;
	Pos food = {rand()%COLS,rand()%LINES};
	mvaddch(food.y, food.x, 'A');
	SnakePart *head;
	head = (SnakePart*) malloc(sizeof(SnakePart));
	SnakePart *tail = head;
	head->p = (Pos) {COLS/2,LINES/2};

	bool dead = false;

	while (!dead) {
		SnakePart *tmp = head;
		switch (getch()) {
			case 'w':
				if (dir == DOWN) {break;}
				dir = UP;
				break;
			case 's':
				if (dir == UP) {break;}
				dir = DOWN;
				break;
			case 'a':
				if (dir == RIGHT) {break;}
				dir = LEFT;
				break;
			case 'd':
				if (dir == LEFT) {break;}
				dir = RIGHT;
				break;
			case 'q':
				dead = true;
				break;
			default:
				break;
		}
		tmp = head;
		head = (SnakePart*) malloc(sizeof(SnakePart));
		head->next = tmp;
		head->p.x = head->next->p.x + ((int[]){0,0,-1,1})[dir];
		head->p.y = head->next->p.y + ((int[]){-1,1,0,0})[dir];

		if (head->p.x > COLS || head->p.y > LINES
		 || head->p.x < 0    || head->p.y < 0
		 || (mvinch(head->p.y, head->p.x) & A_CHARTEXT) == 'X') {dead = true;}

		mvaddch(tail->p.y, tail->p.x, ' ');
		mvaddch(head->p.y, head->p.x, 'X');

		head->next->prev = head;

		if (head->p.x != food.x || head->p.y != food.y) {
			tail = tail->prev;
			free(tail->next);
			tail->next = NULL;
		} else {
			size++;
			food = (Pos){rand()%COLS,rand()%LINES};
			mvaddch(food.y, food.x, 'A');
		}
	}

	endwin();

	printf("Your size was %d\n", size);
}
