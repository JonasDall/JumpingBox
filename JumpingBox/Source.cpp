#include <iostream>
#include <SFML/Graphics.hpp>

class Wall
{
	sf::RectangleShape shape;

public:
	Wall(sf::Vector2f size, sf::Vector2f location)
	{
		shape.setSize(size);
		shape.setPosition(location);
		shape.setFillColor(sf::Color(0x00BDC9FF));
	}
};

class Player
{
	sf::RectangleShape rect;

public:
	Player(sf::Vector2f size)
	{
		rect.setSize(size);
		rect.setFillColor(sf::Color::Red);
	}

	void addForce(sf::Vector2f direction)
	{

	}

	void updatePosition()
	{
		
	}
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(500, 500), "Box");

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear();

		window.display();

	}



	return 0;
}