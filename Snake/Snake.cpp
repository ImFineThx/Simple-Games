#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

int W = 30;
int H = 20;

int i_size = 29;

int w = i_size * (W + 1);

int h = i_size * (H + 1);

int num = 4;

int frog_num = 10;

int dir = 0;

struct Snake
{
	int x, y;
}       s[100];

struct Food
{
	int x, y;
}       f;

struct Frog
{
	int x, y;
}       frog[10];

void tick()
{
	for (int i = num; i > 0; --i) //依次移动蛇的方格
	{
		s[i].x = s[i - 1].x;
		s[i].y = s[i - 1].y;
	}

	switch (dir) //方向控制
	{
	case 0: //down
		s[0].y += 1;
		break;
	case 1: //left
		s[0].x -= 1;
		break;
	case 2: //right
		s[0].x += 1;
		break;
	case 3: //up
		s[0].y -= 1;
		break;
	}

	if ((s[0].x == f.x) && (s[0].y == f.y)) //吃到食物并刷新下一个
	{
		++num;
		f.x = rand() % W;
		f.y = rand() % H;

		for (int i = 0; i < num; ++i)
		{
			if (f.x == s[i].x && f.y == s[i].y)
			{
				f.x += 1;
				f.y += 1;
			}
		}
	}

	//如果碰到青蛙则 头部变回尾部的位置 段数变为1
	for (int i = 0; i < frog_num; ++i)
	{
		if ((s[0].x == frog[i].x) && (s[0].y == frog[i].y))
		{
			s[0].x = s[num - 1].x;
			s[0].y = s[num - 1].y;

			num = 1;

			for (int i = 0; i < frog_num; ++i)
			{
				frog[i].x = rand() % W;
				frog[i].y = rand() % H;
			}
		}
	}

	if (s[0].x > W) //边界循环
	{
		s[0].x = 0;
	} else if (s[0].x < 0)
	{
		s[0].x = W;
	} else if (s[0].y > H)
	{
		s[0].y = 0;
	} else if (s[0].y < 0)
	{
		s[0].y = H;
	}

	for (int i = 1; i < num; ++i) //自我碰撞（碰到第几节，就只保留这几节）
	{
		if ((s[0].x == s[i].x) && (s[0].y == s[i].y))
		{
			num = i;
		}
	}
}

int main()
{
	srand(time(0));;

	Clock clock;
	float timer = 0, delay = 0.1;

	f.x = 10;
	f.y = 10;

	for (int i = 0; i < frog_num; ++i)
	{
		frog[i].x = rand() % W;
		frog[i].y = rand() % H;
	}

	RenderWindow window(VideoMode(w, h), "SnakebySelf");

	Texture t1, t2, t3, t4;
	t1.loadFromFile("white.png");
	t2.loadFromFile("green.png");
	t3.loadFromFile("red.png");
	t4.loadFromFile("frog.png");

	Sprite sprite1(t1);
	Sprite sprite2(t2);
	Sprite sprite3(t3);
	Sprite sprite4(t4);



	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
			{
				window.close();
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Left) && dir != 2)
		{
			dir = 1;
		} else if (Keyboard::isKeyPressed(Keyboard::Right) && dir != 1)
		{
			dir = 2;
		} else if (Keyboard::isKeyPressed(Keyboard::Up) && dir != 0)
		{
			dir = 3;
		} else if (Keyboard::isKeyPressed(Keyboard::Down) && dir != 3)
		{
			dir = 0;
		}

		if (timer > delay)
		{
			timer = 0;
			tick();
		}

		//////////////draw//////////////
		window.clear();

		for (int i = 0; i <= W; ++i)
		{
			for (int j = 0; j <= H; ++j)
			{
				sprite1.setPosition(i * i_size, j * i_size);
				window.draw(sprite1);
			}
		}

		for (int i = 0; i < num; ++i)
		{
			sprite2.setPosition(i_size * s[i].x, i_size * s[i].y);
			window.draw(sprite2);
		}

		for (int i = 0; i < frog_num; ++i)
		{
			sprite4.setPosition(i_size * frog[i].x, i_size * frog[i].y);
			window.draw(sprite4);
		}

		sprite3.setPosition(f.x * i_size, f.y * i_size);
		window.draw(sprite3);

		window.display();
	}
}
