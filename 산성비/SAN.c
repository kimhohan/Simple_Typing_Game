#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "util.h"
#include <windows.h>

#define KEY_ESC 27
#define KEY_ENTER '\r'
#define KEY_BS '\b'

#define MAX_WORD_LENGTH 24
#define MAX_SCREEN_WORD_COUNT 19
#define WORD_COUNT 10
#define BELL 0x07 

int g_word_count = 11;                          // Ȯ���� ���� ������ ����
char *g_words[11] = 
{
	"programming",
	"school",
	"student",
	"chair",
	"desk",
	"average",
	"screen",
	"mouse",
	"window",
	"door",
	" "
};
int g_fail_count = 0;                           // ������ ����
int g_score = 0;                                // ����
char g_input_word[MAX_WORD_LENGTH + 1];         // �Է� �ܾ� ����
int g_input_word_length = 0;                    // �Է� �ܾ� ����
int g_stage_count=1;

struct ScreenWord
{
	int index;
	int x;
	int y;
};
typedef struct ScreenWord ScreenWord;
ScreenWord g_screen_word[MAX_SCREEN_WORD_COUNT];// ȭ�鿡 ��Ÿ�� �ܾ��(g_words�� ���� index ����)
int g_screen_word_count = 0;                    // ȭ�鿡 ��Ÿ�� �ܾ� ����

clock_t g_start_time;                           // ���� �ð�
double g_falling_speed = 1.5;                   // �ܾ� ���� �ð�(�� ����)

void InitScreen(void);
void InitData(void);                            // ���� ���� ���� (Ȯ�� ����)
void Run(void);
void CompareWords(void);
void ProcessInput(int key);
void NormalMode(void);
void DrawWord(int i);                           // ȭ�鿡 ��Ÿ�� i��° �ܾ� ���
void EraseWord(int i);                          // ȭ�鿡 ��Ÿ�� i��° �ܾ� �����
void randomWord(int i);
void UpdateScore(void);
void UpdateFailCount(void);
void inputkey (int key);
void stageboard(void);
void pause(void);
void stage(void);

int main(void)
{
	int key,c;
	SetCursorVisible(0);

	GotoXY(27, 10);	printf("====== �� �� �� �� �� ======");
	GotoXY(24, 12);	printf("������ �����Ϸ��� �ƹ�Ű�� ġ����.");
	system("pause > null");
	srand(time(NULL));
	InitScreen();
	Run();


	return 0;
}

void InitScreen(void)
{
	int i;

	system("cls");

	printf("������������������������������������������������������������������������������\n");
	for (i = 0; i < 19; i++)
		printf("��                                                                          ��\n");
	printf("������������������������������������������������������������������������������\n");

	// Programmed by
	printf("����������������������������\n");
	printf("��  Programmed by ��ȣ��  ��\n");
	printf("����������������������������");

	// �Է�ĭ
	GotoXY(28, 21); printf("����������������������������\n");
	GotoXY(28, 22);	printf("��                        ��\n");
	GotoXY(28, 23);	printf("����������������������������");
	// ����Ƚ��
	GotoXY(58, 21);printf("����������\n");
	GotoXY(58, 22);printf("��  %d   ��\n",g_score);	
	GotoXY(58, 23);printf("����������");
	// ����Ƚ��
	GotoXY(68, 21);printf("����������\n");
	GotoXY(68, 22);printf("��  %d   ��\n",g_fail_count);	
	GotoXY(68, 23);printf("����������");
}

void Run(void)
{
	int i=0;
	int j,key,k;

	while(1)
	{
		randomWord(i);  
		while(1)
		{

			if(_kbhit())
			{
				key=getch();
				inputkey(key);
				if(key=='1')
				{
					system("cls");
					pause();
				}
			}
			stage();
			if(g_fail_count == 6)
			{
				system("cls");
				GotoXY(33, 10);	printf(" G A M E O V E R");
				GotoXY(24, 12);	printf("�ƹ�Ű�� �ι� �����ø� ������ ����˴ϴ�");
				system("pause > null");
				exit(0);
			}
			if(GetDiffTime(g_start_time) > g_falling_speed)
			{
				for(j = 0; j < 19; j++)
				{
					EraseWord(j);
					if( g_screen_word[j].y == 19 )
					{
						i=j-1;
						if( g_screen_word[j].index != 10)
						{
							g_fail_count++;
							UpdateFailCount();
						}
					}
					else
					{
						if(g_screen_word[j].x > 1)
						{
							g_screen_word[j].y++;
							DrawWord(j);
						}

					}
				}
				g_start_time=clock();
				break;
			}
		}
		i++;
	}
}

void CompareWords(void)//ȭ�鿡 ��µ� �ܾ�� �߿� �Է� �ܾ�� ��ġ�ϴ� �ܾ �ִ��� �˻��ϰ� ��ġ�ϴ� �ܾ �ִٸ� ���� �� ���� ����
{
	int i,j,a=0,b=0,k;

	for (i = 0; i<19; i++)
	{
		if (strcmp(g_input_word, g_words[g_screen_word[i].index])==0)
		{
			if (a < g_screen_word[i].y)
			{
				b=i;
				a= g_screen_word[i].y;
			}
		}
	}
	if(a>0)
	{
		putch(BELL);
		EraseWord(b);
		g_screen_word[b].index=10;
		g_score++;
		UpdateScore();
	}
}
void randomWord(int i)
{
	g_screen_word[i].x=rand()%63+2;
	g_screen_word[i].y= 0;
	g_screen_word[i].index=rand()%10;
}
void ProcessInput(int key)//Ű �Է��� ���� ��� ���� �Էµ� ���ڿ��� ���� ��
{ 
	if(g_input_word_length<24)
	{
		GotoXY(30+g_input_word_length,22);
		printf("%c",key);
		g_input_word[g_input_word_length] = key;
		g_input_word_length++;
	}
}
void DrawWord(int i)
{
	GotoXY(g_screen_word[i].x, g_screen_word[i].y);
	printf("%s", g_words[g_screen_word[i].index]);
}
void EraseWord(int i)
{
	int c;
	if (g_screen_word[i].y > 0)
	{      
		GotoXY(g_screen_word[i].x, g_screen_word[i].y);
		for (c = 0; c < strlen(g_words[g_screen_word[i].index]); c++)
			printf(" ");
	}
}
void UpdateScore(void)
{
	GotoXY(62,22); 
	printf("%d",g_score);
}
void UpdateFailCount(void)
{
	GotoXY(72,22);
	printf("%d",g_fail_count);
}
void inputkey(int key)
{
	int i;

	switch (key)
	{
	case KEY_ENTER:
		CompareWords();
		for (i = 0; i <g_input_word_length ; i++)
		{
			GotoXY(30 + i, 22);
			printf(" ");
		}
		for (i = 0; i<g_input_word_length; i++)
			g_input_word[i] = 0;

		g_input_word_length=0;
		break;
	case KEY_BS:
		if (g_input_word_length > 0)
		{
			g_input_word_length--;
			GotoXY(30 + g_input_word_length, 22);
			printf(" ");
			g_input_word[g_input_word_length] = 0;
		}
		break;
	case KEY_ESC:
		exit(0);
		break;
	default:
		ProcessInput(key);
		break;
	}
}
void stageboard(void)
{
	g_falling_speed -= 0.2;
	g_score = 0;
	g_fail_count = 0;
	system("cls");
	GotoXY(27, 10);	printf("====== STAGE %d clear ======",g_stage_count);
	GotoXY(20, 12);	printf("���� STAGE�� �����Ϸ��� �ƹ�Ű�� ġ����.");
	system("pause > null");
	InitScreen();
}

void pause(void)
{
	GotoXY(31, 10);	printf("��� ���߼˽��ϴ�.");
	GotoXY(22, 12);	printf("������ ����Ͻ÷��� �ƹ�Ű�� ġ����.");
	getch();  // �ƹ� Ű�� 1�� �Է� �ޱ�
	system("cls");
	InitScreen();
}
void stage(void)
{
	if(g_score == 6 && g_stage_count == 1)
	{
		system( "color 2E" );
		stageboard();
		g_stage_count++;
	}
	if(g_score == 6 && g_stage_count == 2)
	{
		system( "color 3E" );
		stageboard();
		g_stage_count++;
	}
	if(g_score == 5 && g_stage_count == 3)
	{
		system( "color 4E" );
		stageboard();
		g_stage_count++;
	}
	if(g_score == 5 && g_stage_count == 4)
	{
		system( "color 5E" );
		stageboard();
		g_stage_count++;
	}
	if(g_score == 4 && g_stage_count == 5)
	{
		system( "color 8E" );
		stageboard();
		g_stage_count++;
	}
	if(g_score == 4 && g_stage_count == 6)
	{
		system("cls");
		GotoXY(31, 9);	printf("====== �¸� ======");
		GotoXY(28, 11);	printf("���ӿ��� �¸��ϼ̽��ϴ�.");
		GotoXY(21, 13);	printf("�����Ͻ÷��� �ƹ�Ű�� �ι� �����ּ���.");
		system("pause > null");
		exit(0);
	}
}
