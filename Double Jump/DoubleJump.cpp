//是男人就上一百层
#include <SFML/Graphics.hpp>

using namespace sf;

struct point
{
	int x, y;
}       plat[20];

int main()
{
	RenderWindow window(VideoMode(800, 1066), "Double Jump");
	window.setFramerateLimit(60);

	Texture t1, t2, t3;
	t1.loadFromFile("background.png");
	t2.loadFromFile("platform.png");
	t3.loadFromFile("doodle.png");

	Sprite sBackground(t1), sPlat(t2), sPers(t3);

	sBackground.setScale(2, 2);

	for (int i = 0; i < 10; ++i) //随机平台位置
	{
		plat[i].x = rand() % 800;
		plat[i].y = rand() % 1066;
	}

	int   x = 100, y = 100, h = 200; //人物属性
	float dx = 0,  dy = 0;

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

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			x += 3;
		}
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			x -= 3;
		}

		dy += 0.2; //人物跳跃
		y += dy;
		if (y > 1000)
		{
			dy = -12;
		}

		if (y < h)  //循环平台
		{
			for (int i = 0; i < 10; ++i)
			{
				y = h;
				
				plat[i].y -= dy;
				
				if (plat[i].y > 1066)
				{
					plat[i].y = 0;
					plat[i].x = rand() % 800;
				}
			}
		}

		for (int i = 0; i < 10; ++i) //人物和平台的像素边缘检测
		{
			if ((x + 50 > plat[i].x) && (x + 20 < plat[i].x + 68) && (y + 70 > plat[i].y) && (y + 70 < plat[i].y + 14)
				&& (dy > 0))
			{
				dy = -12;
			}
		}

		sPers.setPosition(x, y);

		window.draw(sBackground);
		window.draw(sPers);

		for (int i = 0; i < 10; ++i)
		{
			sPlat.setPosition(plat[i].x, plat[i].y);
			window.draw(sPlat);
		}
		window.display();
	}
}
