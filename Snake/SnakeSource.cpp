#include <graphics.h>
#include <mmsystem.h>
#include <conio.h>
#include <ctime>
#include <string>
#pragma  comment(lib,"winmm.lib") //Ԥ����ָ���ʾ��ʽ����winmm.lib����⣬�Ͳ��ý�����Ŀ������

/**********************		struct&enum&Data		**********************/
struct coor //��������
{
	int x;
	int y;
};

typedef enum Direction //����
{
	up = 72,
	//���ⰴ����ֵ
	down = 80,
	left = 75,
	right = 77
} Dir;

struct snake //�ߵ���Ϣ
{
	struct coor snake_pos[100]; //100���ߵ�������Ϣ
	int         snake_segments; //�ߵĽ���
	Dir         direction;      //�ߵķ���
}               Snake;

struct food
{
	struct coor foodPos; //ʳ������
	int         flag;    //��û�гԵ�
}               Food;

int  SnakeSize = 40;
bool bDead = false;
/**********************		struct&enum		**********************/

/**********************		funtion		**********************/
void GameInit(); //��Ϸ��ʼ��

void SnakeMove(); //�ߵ��ƶ�

void DrawSnake(); //������

void ChangeDir(); //�ı䷽��

void FoodPos(); //ʳ������

void DrawFood(); //����ʳ��

int EatFood();

void Dead();
/**********************		funtion		**********************/

int main()
{
	GameInit();
	int flag = 0;
	while (!bDead)
	{
		while (!_kbhit() && !bDead)
		{
			if (flag == 0)
			{
				FoodPos();
			}
			DrawFood();
			SnakeMove();
			Dead();
			flag = EatFood();

			Sleep(150);
			clearcliprgn();
		}
		if (bDead)
		{
			clearcliprgn();
			settextcolor(BLACK);
			settextstyle(50, 0, _T("΢���ź�"));
			outtextxy(1000 / 2, 680 / 2, _T("��Ϸ����"));
			mciSendString(L"stop bgm", 0, 0, 0);
		}
		ChangeDir();
	}
	_getch();
	closegraph();
}

//��Ϸ��ʼ��
void GameInit()
{
	initgraph(1280, 720); //��������

	setfillcolor(LIGHTBLUE);                            //���������ɫ
	HRGN rgn_right = CreateRectRgn(1000, 0, 1280, 720); //����������
	setcliprgn(rgn_right);                              //�ü�������

	DeleteObject(rgn_right); //ɾ�����

	solidrectangle(1000, 0, 1280, 720); //ʵ�ĵľ���

	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 25;
	_tcscpy_s(f.lfFaceName,_T("΢���ź�"));
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	setcolor(WHITE); //������ɫ
	//settextstyle(25, 0, L"΢���ź�"); //�����ʽ

	setbkmode(TRANSPARENT); //����͸������

	outtextxy(1100, 50, L"������"); //��ʾ����

	RECT rect = {1050, 100, 1250, 700};

	drawtext(_T("[��Ϸ˵��]\n��ֹ���ޣ�\n\n[����˵��]\n����� ���Ʒ���\nF5       ������\nESC     ������"), &rect,DT_WORDBREAK);

	setbkcolor(DARKGRAY);
	HRGN rgn_left = CreateRectRgn(0, 0, 1000, 720);
	setcliprgn(rgn_left);
	DeleteObject(rgn_left);
	solidrectangle(0, 0, 1000, 720);

	clearcliprgn();
	//��������
	mciSendString(L"open BGM.mp3 alias bgm", 0, 0, 0);
	mciSendString(L"play bgm repeat", 0, 0, 0);

	//��ʼ����
	setlinecolor(BLACK);
	setfillcolor(GREEN);
	fillrectangle(0, 0, SnakeSize, SnakeSize);
	Snake.snake_pos[0].x = 0;
	Snake.snake_pos[0].y = 0;
	Snake.direction = right;
	Snake.snake_segments = 5;
	Food.flag = 0; //������
}

//�ߵ��ƶ�
void SnakeMove()
{
	for (int i = Snake.snake_segments - 2; i >= 0; --i)
	{
		Snake.snake_pos[i + 1].x = Snake.snake_pos[i].x;
		Snake.snake_pos[i + 1].y = Snake.snake_pos[i].y;
	}
	switch (Snake.direction)
	{
	case up:
		Snake.snake_pos[0].y -= SnakeSize;
		break;
	case down:
		Snake.snake_pos[0].y += SnakeSize;
		break;
	case left:
		Snake.snake_pos[0].x -= SnakeSize;
		break;
	case right:
		Snake.snake_pos[0].x += SnakeSize;
		break;
	}
	DrawSnake();
}

void DrawSnake()
{
	for (int i = Snake.snake_segments - 1; i >= 0; --i)
	{
		fillrectangle(Snake.snake_pos[i].x,
		              Snake.snake_pos[i].y,
		              Snake.snake_pos[i].x + SnakeSize,
		              Snake.snake_pos[i].y + SnakeSize);
	}
}

void ChangeDir()
{
	int input;

	input = _getch();
	switch (input)
	{
	case up:
		if (Snake.direction != down)
		{
			Snake.direction = up;
		}
		break;
	case down:
		if (Snake.direction != up)
		{
			Snake.direction = down;
		}
		break;
	case left:
		if (Snake.direction != right)
		{
			Snake.direction = left;
		}
		break;
	case right:
		if (Snake.direction != left)
		{
			Snake.direction = right;
		}
		break;
	default:
		break;
	}
}

void FoodPos()
{
	srand(unsigned int(time(NULL)));
	Food.foodPos.x = rand() % (1000 / SnakeSize) * SnakeSize;
	Food.foodPos.y = rand() % (720 / SnakeSize) * SnakeSize;
	Food.flag = 1; //û�б���
}

void DrawFood()
{
	fillroundrect(Food.foodPos.x,
	              Food.foodPos.y,
	              Food.foodPos.x + SnakeSize,
	              Food.foodPos.y + SnakeSize,
	              SnakeSize,
	              SnakeSize);
}

int EatFood()
{
	if (Snake.snake_pos[0].x == Food.foodPos.x && Snake.snake_pos[0].y == Food.foodPos.y)
	{
		++Snake.snake_segments;

		Food.flag = 0;
		return Food.flag;
	} else
	{
		return 1;
	}
}

void Dead()
{
	for (int i = 1; i < Snake.snake_segments; ++i)
	{
		if ((Snake.snake_pos[0].x == Snake.snake_pos[i].x) && (Snake.snake_pos[0].y == Snake.snake_pos[i].y))
		{
			bDead = true;
		}
	}
	if (Snake.snake_pos[0].x < 0 || Snake.snake_pos[0].x > 1000 || Snake.snake_pos[0].y < 0 || Snake.snake_pos[0].y >
		720)
	{
		bDead = true;
	}
}
