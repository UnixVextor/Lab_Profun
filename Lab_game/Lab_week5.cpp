#include<iostream>
#include<Windows.h>
#include<conio.h>
using namespace std;

void gotoxy(int x, int y) {

	COORD c = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);

}
void drawShip(int x, int y) {
	gotoxy(x, y);
	cout << "<-0->";
}
void eraseShip(int x, int y) {
	system("cls");
	drawShip(x, y);
}
int main() {
	
	char ch = ' ';
	int x = 38, y = 20;
	
	do {
		drawShip(x, y);
		if (_kbhit()) {
			ch = _getch();
			if (ch == 'a') drawShip(x--, y);
			if (ch == 'd') drawShip(x++, y);
			if (ch == 'w') drawShip(x,y--);
			if (ch == 's') drawShip(x,y++);
			fflush(stdin);
		}
		if (x <= 0) x = 1;
		else if (x > 80) x = 80;
		if (y <= 0) y = 0;
		else if (y > 23) y = 23;
		
		Sleep(100);
		eraseShip(x, y);
	} while (ch != 'x');
	
	return 0;
}