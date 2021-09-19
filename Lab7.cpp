#include<iostream>
#include<Windows.h>
#include<conio.h>
#include <time.h>

using namespace std;


void setcursor(bool visible) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}

void setcolor(int fg, int bg) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (bg * 16) + fg);
}
void gotoxy(int x, int y) {

	COORD c = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);

}

char cursor(int x, int y) {
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	char buf[2];
	COORD c = { x,y };
	DWORD num_read;
	if (!ReadConsoleOutputCharacter(hStd,(LPTSTR)buf,1,c,(LPDWORD)&num_read)) {
		return '\0';
	}
	else {
		return buf[0];
	}
}
void drawShip(int x, int y) {

	gotoxy(x, y);
	setcolor(2, 4);
	cout << "<-0->";
}
void eraseShip(int x, int y) {

	gotoxy(x, y);
	setcolor(0, 0);
	cout << "     ";

}

void eraseStar(int x,int y) {
	gotoxy(x, y);
	cout << " ";
}

void drawBullet(int x, int y) {
	gotoxy(x, y);
	setcolor(2, 0);
	cout << "^";
}

void eraseBullet(int x, int y) {
	gotoxy(x, y);
	setcolor(2, 0);
	cout << " ";
}

void drawStar(int x, int y) {
	gotoxy(x, y);
	setcolor(7, 0);
	cout << "*";
}

struct Star {
	int x;
	int y;
};


int main() {

	setcursor(0);
	srand(time(NULL));
	Star star[20];
	int count_star = 0;
	char ch = ' ';
	int x = 38, y = 20;
	bool isLpress = 0, isRpress = 0;
	int Bullet[5] = { 0,0,0,0,0 };
	int Posx[5];
	int Posy[5];
	int score = 0;

	for (int i = 0; i < 20; i++) {
		star[i].x = rand() % (70+1-10)+10;
		star[i].y = rand() % (5+1-2)+2;
	}
	

	do {
		
		drawShip(x, y);
		if (_kbhit()) {
			ch = _getch();
			if (ch == 'a') {
				isLpress = 1;
				isRpress = 0;
			}
			if (ch == 'd') {
				isRpress = 1;
				isLpress = 0;

			}
			if (ch == 's') {
				isLpress = 0;
				isRpress = 0;
			}
			if (ch == ' ') {
				for (int i = 0; i < 5; i++) {
					if (Bullet[i] == 0) {
						Bullet[i] = 1;
						Posx[i] = x;
						Posy[i] = y - 1;
						Beep(700, 100);
						break;
					}
				}
			}
			fflush(stdin);
		}
		setcolor(7, 0);
		gotoxy(75, 0); cout << "Score: " << score;

		for (int i = 0; i < 20;i++) {
			if(star[i].x != 0 && star[i].y != 0) drawStar(star[i].x,star[i].y);
		}
		for (int i = 0; i < 5; i++) {
			if (Bullet[i] == 1) {
				eraseBullet(Posx[i]+2, Posy[i]);
				drawBullet(Posx[i]+2, --Posy[i]);
			}
			if (Posy[i] == 0) {
				Bullet[i] = 0;
				eraseBullet(Posx[i]+2, Posy[i]);
			}
			
		}

		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 5; j++) {
				if (star[i].x == Posx[j]+2 && star[i].y == Posy[j]) {
					eraseStar(star[i].x, star[i].y);
					star[i].x = rand() % (70 + 1 - 10) + 10;
					star[i].y = rand() % (5 + 1 - 2) + 2;
					score += 10;
					Beep(700, 100);
				}
			}
		}
		
		if (isLpress && x >= 1) {
			eraseShip(x, y);
			drawShip(--x, y);
		}

		if (isRpress && x <= 79) {
			eraseShip(x, y);
			drawShip(++x, y);
		}
		setcolor(7, 0);
		Sleep(100);
	} while (ch != 'x');
	
	return 0;
}
