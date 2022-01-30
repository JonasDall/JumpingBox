#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <vector>

sf::Color Grey(0x00CBDCCF);
sf::Color FaintBlue(0x00BDC9FF);
sf::Color FaintRed(0x00DE8C8A);

class Wall
{
	sf::RectangleShape shape;

public:
	Wall(sf::Vector2f size, sf::Vector2f location)
	{
		shape.setSize(size);
		shape.setPosition(location);
		shape.setFillColor(FaintBlue);
	}

	void display(sf::RenderWindow& window)
	{
		window.draw(shape);
	}
};

class Player
{
	sf::RectangleShape rect;
	double movementSpeed{};
	sf::Vector2f force{};

public:
	Player(sf::Vector2f size, double speed) : movementSpeed{speed}
	{
		rect.setSize(size);
		rect.setFillColor(FaintRed);
	}

	void addInput(sf::Event& event)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::D :
			addForce(sf::Vector2f(movementSpeed, 0.f));
			break;

		case sf::Keyboard::A :
			addForce(sf::Vector2f(movementSpeed * -1, 0.f));
			break;

		case sf::Keyboard::Space :
			addForce(sf::Vector2f(0.f, movementSpeed));
			break;
		}
	}

	void addForce(sf::Vector2f newForce)
	{
		force.x += newForce.x;
		force.y += newForce.y;
	}

	void updatePosition()
	{
		
	}

	void display(sf::RenderWindow& window)
	{
		window.draw(rect);
	}
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(500, 500), "Box");
	std::vector<Wall*> walls;

	Player player(sf::Vector2f(20.f, 20.f), 100.f);

	walls.push_back(new Wall(sf::Vector2f(20.f, 20.f), sf::Vector2f(250.f, 250.f)));

	while (window.isOpen())
	{
		sf::Event event;

		//Input loop

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (event.type == sf::Event::KeyPressed)
			{

			}
		}

		//Logic loop



		// Render loop
		window.clear(Grey);

		for (unsigned int i {}; i < walls.size(); ++i)
		{
			walls[i]->display(window);
		}
		
		player.display(window);

		window.display();
	}

	for (unsigned int i{}; i < walls.size(); ++i)
	{
		delete walls[i];
		walls[i] = 0;
	}

	return 0;
}