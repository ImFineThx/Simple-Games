//太空弹幕射击
#include <SFML/Graphics.hpp>
#include <list>

using namespace sf;

const int W = 1200;
const int H = 800;

float DEGTORAD = 0.017453f;

class Animation
{
public:
	float                Frame, speed;
	Sprite               sprite;
	std::vector<IntRect> frames;
	Animation() {}

	Animation(Texture& t, int x, int y, int w, int h, int count, float Speed)
	{
		Frame = 0;
		speed = Speed;

		for (int i = 0; i < count; ++i)
		{
			frames.push_back(IntRect(x + i * w, y, w, h));

			sprite.setTexture(t);
			sprite.setOrigin(w / 2, h / 2);
			sprite.setTextureRect(frames[0]);
		}
	}

	void update()
	{
		Frame += speed;
		int n = frames.size();
		if (Frame >= n)
		{
			Frame -= n;
		}
		if (n > 0)
		{
			sprite.setTextureRect(frames[int(Frame)]);
		}
	}

	bool isEnd()
	{
		return Frame + speed >= frames.size();
	}
};

class Entity
{
public:
	float       x, y, dx, dy, R, angle;
	bool        life;
	std::string name;
	Animation   anim;

	Entity()
	{
		life = 1;
	}

	void settings(Animation& a, int X, int Y, float Angle = 0, int radius = 1)
	{
		x = X;
		y = Y;
		anim = a;
		angle = Angle;
		R = radius;
	}

	virtual void update() { }

	void draw(RenderWindow& Window)
	{
		anim.sprite.setPosition(x, y);
		anim.sprite.setRotation(angle + 90);
		Window.draw(anim.sprite);

		CircleShape circle(R);
		circle.setFillColor(Color(255, 0, 0, 170));
		circle.setPosition(x, y);
		circle.setOrigin(R, R);
	}

	virtual ~Entity() {};
};

class asteroid : public Entity
{
public:
	asteroid()
	{
		dx = rand() % 8 - 4;
		dy = rand() % 8 - 4;
		name = "asteroid";
	}

	void update() override
	{
		x += dx;
		y += dy;
		if (x > W)
		{
			x = 0;
		}
		if (x < 0)
		{
			x = W;
		}
		if (y > H)
		{
			y = 0;
		}
		if (y < 0)
		{
			y = H;
		}
	}
};

class bullet : public Entity
{
public:
	bullet()
	{
		name = "bullet";
	}

	void update() override
	{
		dx = cos(angle * DEGTORAD) * 6;
		dy = sin(angle * DEGTORAD) * 6;
		x += dx;
		y += dy;
		if (x > W || x < 0 || y > H || y < 0)
		{
			life = 0;
		}
	}
};

class player : public Entity
{
public:
	bool thrust;

	player()
	{
		name = "player";
		thrust = false;
	}

	void update()
	{
		if (thrust)
		{
			dx += cos(angle * DEGTORAD) * 0.2;
			dy += sin(angle * DEGTORAD) * 0.2;
		} else
		{
			dx *= 0.99;
			dy *= 0.99;
		}

		int   maxSpeed = 15;
		float speed = sqrt(dx * dx + dy * dy);
		if (speed > maxSpeed)
		{
			dx *= maxSpeed / speed;
			dy *= maxSpeed / speed;
		}

		x += dx;
		y += dy;

		if (x > W)
			x = 0;
		if (x < 0)
			x = W;
		if (y > H)
			y = 0;
		if (y < 0)
			y = H;
	}
};

bool isCollide(Entity* a, Entity* b)
{
	return (b->x - a->x) * (b->x - a->x) + (b->y - a->y) * (b->y - a->y) < (a->R + b->R) * (a->R + b->R);
}

int main()
{
	RenderWindow window(VideoMode(W, H), "Asteroids!");
	window.setFramerateLimit(60);

	Texture t1, t2, t3, t4, t5, t6, t7;

	t1.loadFromFile("spaceship.png");
	t2.loadFromFile("background.jpg");
	t3.loadFromFile("explosions/type_C.png");
	t4.loadFromFile("rock.png");
	t5.loadFromFile("fire_blue.png");
	t6.loadFromFile("rock_small.png");
	t7.loadFromFile("explosions/type_B.png");

	t1.setSmooth(true);
	t2.setSmooth(true);

	Sprite sBackground(t2);

	Animation sExplosion(t3, 0, 0, 256, 256, 48, 0.5);
	Animation sRock(t4, 0, 0, 64, 64, 16, 0.2);
	Animation sRock_small(t6, 0, 0, 64, 64, 16, 0.2);
	Animation sBullet(t5, 0, 0, 32, 64, 16, 0.8);
	Animation sPlayer(t1, 40, 0, 40, 40, 1, 0);
	Animation sPlayer_go(t1, 40, 40, 40, 40, 1, 0);
	Animation sExplosion_ship(t7, 0, 0, 192, 192, 64, 0.5);

	std::list<Entity*> entities;

	for (int i = 0; i < 15; ++i)
	{
		asteroid* a = new asteroid();
		a->settings(sRock, rand() % W, rand() % H, rand() % 360, 25);
		entities.push_back(a);
	}

	player* p = new player();
	p->settings(sPlayer, 200, 200, 0, 20);
	entities.push_back(p);

	while (window.isOpen())
	{
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
			{
				window.close();
			}
			if (e.type == Event::KeyPressed)
			{
				if (e.key.code == Keyboard::Space)
				{
					bullet* b = new bullet();
					b->settings(sBullet, p->x, p->y, p->angle, 10);
					entities.push_back(b);
				}
			}
		}

		/////////////     Player Input    /////////////

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			p->angle += 3;
		}
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			p->angle -= 3;
		}
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			p->thrust = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			p->thrust = false;
		}
		for (auto a : entities)
		{
			for (auto b : entities)
			{
				if (a->name == "asteroid" && b->name == "bullet")
				{
					if (isCollide(a, b))
					{
						a->life = false;
						b->life = false;
						a->life = false;
						b->life = false;

						Entity* e = new Entity();
						e->settings(sExplosion, a->x, a->y);
						e->name = "explosion";
						entities.push_back(e);

						for (int i = 0; i < 2; i++)
						{
							if (a->R == 15)
								continue;
							Entity* e = new asteroid();
							e->settings(sRock_small, a->x, a->y, rand() % 360, 15);
							entities.push_back(e);
						}
					}
				}
				if (a->name == "player" && b->name == "asteroid")
					if (isCollide(a, b))
					{
						b->life = false;

						Entity* e = new Entity();
						e->settings(sExplosion_ship, a->x, a->y);
						e->name = "explosion";
						entities.push_back(e);

						p->settings(sPlayer, W / 2, H / 2, 0, 20);
						p->dx = 0;
						p->dy = 0;
					}
			}
		}

		////////////////     Ship Movement     ///////////////////
		if (p->thrust)
		{
			p->anim = sPlayer_go;
		} else
		{
			p->anim = sPlayer;
		}
		for (auto e : entities)
		{
			if (e->name == "explosion")
			{
				if (e->anim.isEnd())
					e->life = 0;
			}
		}

		if (rand() % 150 == 0)
		{
			asteroid* a = new asteroid();
			a->settings(sRock, 0, rand() % H, rand() % 360, 25);
			entities.push_back(a);
		}

		for (auto i = entities.begin(); i != entities.end();)
		{
			Entity* e = *i;

			e->update();
			e->anim.update();

			if (e->life == false)
			{
				i = entities.erase(i);
				delete e;
			} else
				i++;
		}

		/////////     Draw     //////////
		window.clear();
		window.draw(sBackground);
		for (auto i : entities)
		{
			i->draw(window);
		}
		window.display();
	}
	return 0;
}
