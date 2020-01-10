//消消乐
#include <SFML/Graphics.hpp>

using namespace sf;

int      t_size = 54; //×©¿é´óÐ¡
Vector2i offset(48, 24);

struct piece
{
	int x, y, col, row, kind, match, alpha;

	piece()
	{
		match = 0;
		alpha = 255;
	}
} grid[10][10];

void swap(piece p1, piece p2)
{
	std::swap(p1.col, p2.col);
	std::swap(p1.row, p2.row);

	grid[p1.row][p1.col] = p1;
	grid[p2.row][p2.col] = p2;
}

int main()
{
	srand(time(0));
	RenderWindow window(VideoMode(740, 480), "Match-3!");
	window.setFramerateLimit(60);

	Texture t1, t2;

	t1.loadFromFile("background.png");
	t2.loadFromFile("gems.png");

	Sprite background(t1), gems(t2);

	for (int i = 1; i <= 8; ++i)
	{
		for (int j = 1; j <= 8; ++j)
		{
			grid[i][j].kind = rand() % 7;
			grid[i][j].col = j;
			grid[i][j].row = i;
			grid[i][j].x = j * t_size;
			grid[i][j].y = i * t_size;
		}
	}

	int      x0, y0, x, y;
	int      click = 0;
	Vector2i pos;

	bool isSwap = false;
	bool isMoving = false;

	while (window.isOpen())
	{
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
			{
				window.close();
			}
			if (e.type == Event::MouseButtonPressed)
			{
				if (e.key.code == Mouse::Left && !isMoving && !isSwap)
				{
					++click;
					pos = Mouse::getPosition(window) - offset;
				}
			}
		}

		////   Mouse Click   ////

		if (click == 1)
		{
			x0 = pos.x / t_size + 1;
			y0 = pos.y / t_size + 1;
		}
		if (click == 2)
		{
			x = pos.x / t_size + 1;
			y = pos.y / t_size + 1;

			if (abs(x - x0) + abs(y - y0) == 1)
			{
				swap(grid[y0][x0], grid[y][x]);

				isSwap = true;

				click = 0;
			} else
			{
				click = 1;
			}
		}

		// Match Finding ///
		for (int i = 1; i <= 8; ++i)
		{
			for (int j = 1; j <= 8; ++j)
			{
				if (grid[i][j].kind == grid[i + 1][j].kind)
				{
					if (grid[i][j].kind == grid[i - 1][j].kind)
					{
						for (int n = -1; n <= 1; ++n)
						{
							++grid[i + n][j].match;
						}
					}
				}

				if (grid[i][j].kind == grid[i][j + 1].kind)
				{
					if (grid[i][j].kind == grid[i][j - 1].kind)
					{
						for (int n = -1; n <= 1; ++n)
						{
							++grid[i][j + n].match;
						}
					}
				}
			}
		}

		/////  Moving animation   ////
		isMoving = false;
		for (int i = 1; i <= 8; ++i)
		{
			for (int j = 1; j <= 8; ++j)
			{
				piece& p = grid[i][j];
				int    dx, dy;
				for (int n = 0; n < 4; ++n)
				{
					dx = p.x - p.col * t_size;
					dy = p.y - p.row * t_size;
					if (dx)
					{
						p.x -= dx / abs(dx);
					}
					if (dy)
					{
						p.y -= dy / abs(dy);
					}
				}
				if (dx || dy)
				{
					isMoving = true;
				}
			}
		}

		//Deleting animation
		if (!isMoving)
		{
			for (int i = 1; i <= 8; ++i)
			{
				for (int j = 1; j <= 8; ++j)
				{
					if (grid[i][j].match)
					{
						if (grid[i][j].alpha > 10)
						{
							grid[i][j].alpha -= 10;
							isMoving = true;
						}
					}
				}
			}
		}

		//Get Score
		int score = 0;
		for (int i = 1; i <= 8; ++i)
		{
			for (int j = 1; j <= 8; ++j)
			{
				score += grid[i][j].match;
			}
		}

		/////    Second swap if no match    ////
		if (isSwap && !isMoving)
		{
			swap(grid[y0][x0], grid[y][x]);
			isSwap = false;
		}

		//Update grid
		if (!isMoving)
		{
			for (int i = 8; i > 0; --i)
			{
				for (int j = 1; j <= 8; ++j)
				{
					if (grid[i][j].match)
					{
						for (int n = i; n > 0; --n)
						{
							if (!grid[n][j].match)
							{
								swap(grid[n][j], grid[i][j]);
								break;
							}
						}
					}
				}
			}
			for (int j = 1; j <= 8; ++j)
			{
				for (int i = 8, n = 0; i > 0; --i)
				{
					if (grid[i][j].match)
					{
						grid[i][j].kind = rand() % 7;
						grid[i][j].y = -t_size * n++;
						grid[i][j].match = 0;
						grid[i][j].alpha = 255;
					}
				}
			}
		}

		///////////////    Draw    ///////////////
		window.draw(background);

		for (int i = 1; i <= 8; ++i)
		{
			for (int j = 1; j <= 8; ++j)
			{
				gems.setTextureRect(IntRect(grid[i][j].kind * 49, 0, 49, 49));

				gems.setColor(Color(255, 255, 255, grid[i][j].alpha));

				gems.setPosition(grid[i][j].x, grid[i][j].y);

				gems.move(offset.x - t_size, offset.y - t_size);

				window.draw(gems);
			}
		}
		window.display();
	}

	return 0;
}
