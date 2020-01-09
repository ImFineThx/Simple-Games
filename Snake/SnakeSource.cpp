#include <graphics.h>
#include <mmsystem.h>
#include <conio.h>
#include <ctime>
#include <string>
#pragma  comment(lib,"winmm.lib") //预编译指令，表示显式链接winmm.lib这个库，就不用进行项目设置了

/**********************		struct&enum&Data		**********************/
struct coor //保存坐标
{
	int x;
	int y;
};

typedef enum Direction //方向
{
	up = 72,
	//虚拟按键的值
	down = 80,
	left = 75,
	right = 77
} Dir;

struct snake //蛇的信息
{
	struct coor snake_pos[100]; //100节蛇的坐标信息
	int         snake_segments; //蛇的节数
	Dir         direction;      //蛇的方向
}               Snake;

struct food
{
	struct coor foodPos; //食物坐标
	int         flag;    //有没有吃掉
}               Food;

int  SnakeSize = 40;
bool bDead = false;
/**********************		struct&enum		**********************/

/**********************		funtion		**********************/
void GameInit(); //游戏初始化

void SnakeMove(); //蛇的移动

void DrawSnake(); //画出蛇

void ChangeDir(); //改变方向

void FoodPos(); //食物坐标

void DrawFood(); //画出食物

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
			settextstyle(50, 0, _T("微软雅黑"));
			outtextxy(1000 / 2, 680 / 2, _T("游戏结束"));
			mciSendString(L"stop bgm", 0, 0, 0);
		}
		ChangeDir();
	}
	_getch();
	closegraph();
}

//游戏初始化
void GameInit()
{
	initgraph(1280, 720); //创建窗口

	setfillcolor(LIGHTBLUE);                            //设置填充颜色
	HRGN rgn_right = CreateRectRgn(1000, 0, 1280, 720); //创建矩形区
	setcliprgn(rgn_right);                              //裁剪矩形区

	DeleteObject(rgn_right); //删除句柄

	solidrectangle(1000, 0, 1280, 720); //实心的矩形

	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 25;
	_tcscpy_s(f.lfFaceName,_T("微软雅黑"));
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	setcolor(WHITE); //设置颜色
	//settextstyle(25, 0, L"微软雅黑"); //字体格式

	setbkmode(TRANSPARENT); //文字透明背景

	outtextxy(1100, 50, L"套娃蛇"); //显示文字

	RECT rect = {1050, 100, 1250, 700};

	drawtext(_T("[游戏说明]\n禁止套娃！\n\n[控制说明]\n方向键 控制方向\nF5       不管用\nESC     不管用"), &rect,DT_WORDBREAK);

	setbkcolor(DARKGRAY);
	HRGN rgn_left = CreateRectRgn(0, 0, 1000, 720);
	setcliprgn(rgn_left);
	DeleteObject(rgn_left);
	solidrectangle(0, 0, 1000, 720);

	clearcliprgn();
	//播放音乐
	mciSendString(L"open BGM.mp3 alias bgm", 0, 0, 0);
	mciSendString(L"play bgm repeat", 0, 0, 0);

	//初始化蛇
	setlinecolor(BLACK);
	setfillcolor(GREEN);
	fillrectangle(0, 0, SnakeSize, SnakeSize);
	Snake.snake_pos[0].x = 0;
	Snake.snake_pos[0].y = 0;
	Snake.direction = right;
	Snake.snake_segments = 5;
	Food.flag = 0; //被吃了
}

//蛇的移动
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
	Food.flag = 1; //没有被吃
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
