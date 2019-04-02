#include <Windows.h>
#include "util.h"

void GotoXY(int x, int y)		// Ŀ�� ��ġ �̵�
{
	COORD Pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

double GetDiffTime(clock_t start_time)
{
    clock_t cur_time;
    cur_time = clock();
    return ((double) (cur_time - start_time) / CLOCKS_PER_SEC);
}

void SetCursorVisible(BOOL tf)	// ȭ�� Ŀ���� On/Off
{
    CONSOLE_CURSOR_INFO ci = { 10, tf };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
}