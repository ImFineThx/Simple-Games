#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

bool isCollide(Sprite s1, Sprite s2)
{
	return s1.getGlobalBounds().intersects(s2.getGlobalBounds());
}

int main()
{
	srand(time(0));

	RenderWindow window(VideoMode(520, 450), "Arkanoid!");

	window.setFramerateLimit(60); //限制帧数
	window.setSize(Vector2u(1040,800));
	
	//载入贴图
	Texture t1, t2, t3, t4;
	t1.loadFromFile("block01.png");
	t2.loadFromFile("background.jpg");
	t3.loadFromFile("ball.png");
	t4.loadFromFile("paddle.png");

	Sprite sBackground(t2);
	Sprite sBall(t3);
	Sprite sPaddle(t4);

	sPaddle.setPosition(300, 440);
	sBall.setPosition(300, 300);

	Sprite block[1000];

	int n = 0;
	for (int i = 1; i <= 10; ++i)
	{
		for (int j = 1; j <= 10; ++j)
		{
			block[n].setTexture(t1);
			block[n].setPosition(i * 43, j * 20);
			++n;
		}
	}

	float dx = 4, dy = 3;

	while (window.isOpen())
	{
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
			{
				window.close();
			}
		}

		sBall.move(dx, 0); //x,y要分别判断并移动 不然就只会斜着移动

		for (int i = 0; i < n; ++i)
		{
			if (isCollide(sBall, block[i]))
			{
				block[i].setPosition(-100, 0);
				dx = -dx;
			}
		}

		sBall.move(0, dy);
		for (int i = 0; i < n; ++i)
		{
			if (isCollide(sBall, block[i]))
			{
				block[i].setPosition(-100, 0);
				dy = -dy;
			}
		}

		Vector2f b = sBall.getPosition();
		if (b.x < 0 || b.x > 520)
		{
			dx = -dx;
		}
		if (b.y < 0 )
		{
			dy = -dy;
		}
		

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			sPaddle.move(7, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			sPaddle.move(-7, 0);
		}

		if (isCollide(sPaddle, sBall))
		{
			dy = -(rand() % 5 + 2);
		}

		window.clear();
		window.draw(sBackground);
		window.draw(sBall);
		window.draw(sPaddle);

		for (int i = 0; i < n; ++i)
		{
			window.draw(block[i]);
		}
		window.display();
	}
}
