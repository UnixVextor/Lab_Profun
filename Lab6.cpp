#include<iostream>
#include<Windows.h>
#include<conio.h>
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

void drawBullet(int x,int y) {
	gotoxy(x, y);
	setcolor(2, 0);
	cout << "  ^  ";
}

void eraseBullet(int x ,int y) {
	gotoxy(x, y);
	setcolor(2, 0);
	cout << "     ";
}


int main() {
	setcursor(0);
	char ch = ' ';
	int x = 38, y = 20;
	bool isLpress = 0, isRpress = 0;
	int Bullet[5] = {0,0,0,0,0};
	int Posx[5];
	int Posy[5];

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
			if (ch == ' ' ) {
				for (int i = 0; i < 5; i++) {
					if (Bullet[i] == 0) {
						Bullet[i] = 1;
						Posx[i] = x;
						Posy[i] = y - 1;
						break;
					}
				}
			}
			fflush(stdin);
		}

		for (int i = 0; i < 5; i++) {
			if (Bullet[i] == 1) {
				eraseBullet(Posx[i], Posy[i]);
				drawBullet(Posx[i], --Posy[i]);

			}
			if (Posy[i] == 0) {
				Bullet[i] = 0;
				eraseBullet(Posx[i], Posy[i]);
			}
		
		}



		if (isLpress && x>=1) {
			eraseShip(x, y); 
			drawShip(--x, y); 
		}

		if (isRpress && x<=79) {
			eraseShip(x, y);
			drawShip(++x, y);
		}

		
	
		Sleep(100);
	} while (ch != 'x');

	return 0;
}
