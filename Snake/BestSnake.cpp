#include <SFML/Graphics.hpp>
#include <time.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <graphics.h>
#include <mmsystem.h>
#pragma  comment(lib,"winmm.lib")

using namespace std;
using namespace sf;

int N = 30, M = 20;  //����ͼ�����
int image_size = 29; //�����С

int w = image_size * (N + 1); //���ڿ��
int h = image_size * (M + 1);

int dir = 0, num = 4; //�����ߵĶ���

struct Fruct //ʳ��
{
	int x, y;
}       f;

struct Snake
{
	int x, y;
}       s[100];

void Tick()
{
	for (int i = num; i > 0; --i) //�����ƶ�
	{
		s[i].x = s[i - 1].x;
		s[i].y = s[i - 1].y;
	}

	switch (dir) //�������
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

	if ((s[0].x == f.x) && (s[0].y == f.y)) //�Ե�ʳ���ж�
	{
		++num;
		f.x = rand() % N; //���ˢ����һ��ʳ���λ��
		f.y = rand() % M;
		for (int i = 0; i < num; ++i)
		{
			if ((f.x == s[i].x) && (f.y == s[i].y))
			{
				f.x += 1;
				f.y += 1;
			}
		}
	}

	if (s[0].x > N) //�߽�ѭ��
	{
		s[0].x = 0;
	}
	if (s[0].x < 0)
	{
		s[0].x = N;
	}
	if (s[0].y > M)
	{
		s[0].y = 0;
	}
	if (s[0].y < 0)
	{
		s[0].y = M;
	}

	for (int i = 1; i < num; ++i) //������ײ�������ڼ��ڣ���ֻ�����⼸�ڣ�
	{
		if (s[0].x == s[i].x && s[0].y == s[i].y)
		{
			num = i;
		}
	}
}

int main()
{
	srand(time(0));

	//��������
	RenderWindow window(VideoMode(w, h), "Snake Game!");

	//������ͼ
	Texture t1, t2, t3;
	t1.loadFromFile("white.png");
	t2.loadFromFile("red.png");
	t3.loadFromFile("green.png");

	Sprite sprite1(t1);
	Sprite sprite2(t2);
	Sprite sprite3(t3);

	//play BGM
	mciSendString(L"open BGM.mp3 alias bgm", 0, 0, 0);
	mciSendString(L"play bgm repeat", 0, 0, 0);

	Clock clock;
	float timer = 0, delay = 0.1;

	f.x = 10;
	f.y = 10;

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
			Tick();
		}

		//////////////draw//////////////
		window.clear();

		for (int i = 0; i <= N; ++i) //������������
		{
			for (int j = 0; j <= M; ++j)
			{
				sprite1.setPosition(i * image_size, j * image_size);
				window.draw(sprite1);
			}
		}

		for (int i = 0; i < num; ++i) //�����ߵ���ͼ
		{
			sprite2.setPosition(s[i].x * image_size, s[i].y * image_size);

			window.draw(sprite2);
		}

		//����ʳ����ͼ
		sprite3.setPosition(f.x * image_size, f.y * image_size);

		window.draw(sprite3);

		window.display();
	}
}
