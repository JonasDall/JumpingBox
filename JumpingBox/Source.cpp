#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <vector>

sf::Color Grey(247, 249, 237, 250);
sf::Color FaintBlue(132, 138, 250, 250);
sf::Color FaintRed(250, 112, 103, 250);

const float gravity{ 9.80665 };

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
	sf::Vector2f velocity{};
	double gravityModifier{ 100 };

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
		velocity.x += newForce.x;
		velocity.y += newForce.y;
	}

	void updatePosition(double deltaTime)
	{
		addForce(sf::Vector2f(0.f, (gravity * gravityModifier * deltaTime)));

		sf::Vector2f deltaPosition{ sf::Vector2f((velocity.x * deltaTime), velocity.y *deltaTime) };

		rect.move(deltaPosition);

		//std::cout << rect.getPosition().y << '\n';
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

	Player player(sf::Vector2f(50.f, 50.f), 100.f);

	walls.push_back(new Wall(sf::Vector2f(20.f, 20.f), sf::Vector2f(250.f, 250.f)));

	std::chrono::duration<float, std::milli> timeDifference{};
	std::chrono::steady_clock::time_point currentTime{ std::chrono::steady_clock::now() };
	double deltaTime{};

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
		player.updatePosition(deltaTime);


		// Render loop
		window.clear(Grey);

		for (unsigned int i {}; i < walls.size(); ++i)
		{
			walls[i]->display(window);
		}
		
		player.display(window);

		window.display();

		//Calculate deltaTime
		timeDifference = std::chrono::steady_clock::now() - currentTime;
		deltaTime = timeDifference.count() / 1000;

		currentTime = std::chrono::steady_clock::now();

		std::cout << deltaTime << '\n';
	}

	for (unsigned int i{}; i < walls.size(); ++i)
	{
		delete walls[i];
		walls[i] = 0;
	}

	return 0;
}