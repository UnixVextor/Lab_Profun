#include <iostream>
#include <windows.h>
#include <time.h>

#define scount 80
#define screen_x 80
#define screen_y 25

using namespace std;

HANDLE wHnd;
HANDLE rHnd;
HANDLE Hnd;
CHAR_INFO consoleBuffer[screen_x * screen_y];
COORD bufferSize = { screen_x,screen_y };
COORD characterPos = { 0,0 };
SMALL_RECT windowSize = { 0,0,screen_x - 1,screen_y - 1 };
COORD star[scount];
DWORD fdwMode;


void setcursor(bool visible) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}

void gotoxy(int x, int y) {
	COORD c = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
int setConsole(int x, int y) {
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	SetConsoleScreenBufferSize(wHnd, bufferSize);
	return 0;
}

int setMode() {
	rHnd = GetStdHandle(STD_INPUT_HANDLE);
	fdwMode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	SetConsoleMode(rHnd,fdwMode);
	return 0;
}


void fill_data_to_buffer(char ch,int color) {
	for (int y = 0; y < screen_y; ++y) {
		for (int x = 0; x < screen_x; ++x) {
			consoleBuffer[x + screen_x * y].Char.AsciiChar = ch;
			consoleBuffer[x + screen_x * y].Attributes = color;
		}
	}
}

void fill_buffer_to_console() {
	WriteConsoleOutputA(wHnd, consoleBuffer, bufferSize, characterPos, &windowSize);
}

void init_star() {
	for (int i = 0; i < scount; i++) {
		star[i].X = rand() % screen_x;
		star[i].Y = rand() % screen_y;
	}
}

void clear_buffer() {
	for (int y = 0; y < screen_y; ++y) {
		for (int x = 0; x < screen_x; ++x) {
			consoleBuffer[x + screen_x * y].Char.AsciiChar = ' ';
			consoleBuffer[x + screen_x * y].Attributes = 0;
		}
	}
}

void fill_star_to_buffer() {
	for (int i = 0; i < scount; i++) {
		consoleBuffer[star[i].X + screen_x * star[i].Y].Char.AsciiChar = '*';
		consoleBuffer[star[i].X + screen_x * star[i].Y].Attributes = 7;
	}
}

void star_fall() {
	for (int i = 0; i < scount; i++) {
		if (star[i].Y >= screen_y - 1) {
			star[i] = { (rand() % screen_x),1 };
		}
		else {
			star[i] = { star[i].X,star[i].Y + 1 };
		}
	}
}

void draw(char strChar[], int posx,int posy, unsigned short color) {
	int x = posx;
	int y = posy;
	for (size_t i = 0; i < strlen(strChar); i++){
			consoleBuffer[x + screen_x * y].Char.AsciiChar = strChar[i];
			consoleBuffer[x + screen_x * y].Attributes = color;
			x++;
	}
}


int main() {
	srand(time(NULL));
	setConsole(screen_x, screen_y);
	setMode();
	init_star();
	setcursor(0);
	char strChar[6] = "<-0->";
	bool play = true;
	DWORD numEvents = 0;
	DWORD numEventsRead = 0;
	int posx = 0;
	int posy = 0;
	int collision = 10;
	unsigned int randomcolor = 7;
	while (play && collision != 0)
	{

		GetNumberOfConsoleInputEvents(rHnd, &numEvents);
		if (numEvents != 0) {
			INPUT_RECORD* eventBuffer = new INPUT_RECORD[numEvents];
			ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);

			for (DWORD i = 0; i < numEventsRead; ++i) {
				
				if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown == true) {
					if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
						play = false;
					}
					if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'c') {
						randomcolor = rand() % 255;
					}
				}
				else if (eventBuffer[i].EventType == MOUSE_EVENT) {
					 posx = eventBuffer[i].Event.MouseEvent.dwMousePosition.X;
					 posy = eventBuffer[i].Event.MouseEvent.dwMousePosition.Y;
					if (eventBuffer[i].Event.MouseEvent.dwButtonState &
						FROM_LEFT_1ST_BUTTON_PRESSED) {
						randomcolor = rand() % 255;
					}
					else if (eventBuffer[i].Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED) {
						//printf("right click\n");
					}

					else if (eventBuffer[i].Event.MouseEvent.dwEventFlags & MOUSE_MOVED) {
						if (posx <= 0) posx = 0;
						if (posx >= 75) posx = 75;
						if (posy <= 0) posy = 0;
						if (posy >= 25) posy = 25;
					}
				}
			}
			
			delete[] eventBuffer;
		}
		for (int i = 0; i < scount; i++) {
			if (star[i].X >= posx && star[i].X < posx + 5 && star[i].Y == posy) {
				collision--;
				star[i].X = rand() % 80;
				star[i].Y = rand() % 25;
			}
		}
		
		star_fall();
		clear_buffer();
		draw(strChar, posx, posy, randomcolor);
		fill_star_to_buffer();
		fill_buffer_to_console();
		Sleep(200);
	}

	return 0;
}