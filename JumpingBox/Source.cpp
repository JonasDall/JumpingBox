#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <vector>
#include <cstdlib>

//Custom colors
sf::Color Grey(247, 249, 237, 250);
sf::Color FaintBlue(132, 138, 250, 250);
sf::Color FaintRed(250, 112, 103, 250);

//Global constants
const float gravity{ 9.80665 };


//Classes
class Wall
{
	sf::RectangleShape shape;

public:
	Wall(sf::Vector2f size, sf::Vector2f location)
	{
		shape.setSize(size);
		shape.setPosition(location);
		shape.setOrigin(sf::Vector2f(size.x / 2, size.y / 2));
		shape.setFillColor(FaintBlue);
	}

	void display(sf::RenderWindow& window)
	{
		window.draw(shape);
	}

	sf::Vector2f getPosition()
	{
		return shape.getPosition();
	}

	sf::Vector2f getSize()
	{
		return shape.getSize();
	}
};

class Player
{
	sf::RectangleShape rect;
	double movementSpeed{10.f};
	sf::Vector2f velocity{};
	double gravityModifier{ 100 };
	bool isJumping{ false };

public:
	Player(sf::Vector2f size, sf::Vector2f startPosition)
	{
		rect.setSize(size);
		rect.setOrigin(sf::Vector2f(size.x / 2, size.y / 2));
		rect.setPosition(startPosition);
		rect.setFillColor(FaintRed);
	}

	void characterInput(sf::Event& event)
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
			addForce(sf::Vector2f(0.f, movementSpeed * 10 * -1));
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

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			addForce(sf::Vector2f(movementSpeed * -1, 0.f));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			addForce(sf::Vector2f(movementSpeed, 0.f));
		}

		if (isJumping)
		{
			isJumping = false;
			addForce(sf::Vector2f(0.f, -1000.f));
			rect.move(sf::Vector2f(0.f, -5.f));
		}

		sf::Vector2f deltaPosition{ sf::Vector2f((velocity.x * deltaTime), velocity.y *deltaTime) };

		if (velocity.y < 0)
		{
			std::cout << velocity.x << " " << velocity.y << '\n';
		}

		rect.move(deltaPosition);

		//std::cout << rect.getPosition().y << '\n';
	}

	void setVelocity(sf::Vector2f newVelocity)
	{
		velocity = newVelocity;
	}

	void display(sf::RenderWindow& window)
	{
		window.draw(rect);
	}

	sf::Vector2f getVelocity()
	{
		return velocity;
	}

	void setVelocity()
	{
		velocity = sf::Vector2f(0.f, 0.f);
	}

	sf::Vector2f getPosition()
	{
		return rect.getPosition();
	}

	void setPosition(sf::Vector2f newPosition)
	{
		rect.setPosition(newPosition);
	}

	sf::Vector2f getSize()
	{
		return rect.getSize();
	}

	void jump()
	{
		isJumping = true;
	}
};

int main()
{
	//~~INITIAL SETUP~~

	//Setup window
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "Box");
	window.setVerticalSyncEnabled(true);

	//Create walls
	std::vector<Wall*> walls;
	walls.push_back(new Wall(sf::Vector2f(1000.f, 20.f), sf::Vector2f(500.f, 750.f)));

	//Create player
	Player player(sf::Vector2f(50.f, 50.f), sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
	
	//Setup time
	std::chrono::duration<float, std::milli> timeDifference{};
	std::chrono::steady_clock::time_point currentTime{ std::chrono::steady_clock::now() };
	double deltaTime{};

	//~~BEGIN GAME LOOP~~

	//Frame loop
	while (window.isOpen())
	{
		sf::Event event;

		//Input loop

		while (window.pollEvent(event))
		{
			//Close window
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			//Jump
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Space)
				{
					std::cout << "Jump\n";

					if (player.getVelocity().y < 0.1 && player.getVelocity().y > -0.1)
					{
						player.jump();

					}
				}
			}
		}

		//Logic loop
		float length{ abs(player.getPosition().y - walls[0]->getPosition().y) };
		float gap{length - (player.getSize().y / 2) - (walls[0]->getSize().y / 2)};

		//std::cout << length << '\n';
		if (gap < 0)
		{
			player.setVelocity(sf::Vector2f(player.getVelocity().x, 0.f));
			player.setPosition(sf::Vector2f(player.getPosition().x, walls[0]->getPosition().y - (walls[0]->getSize().y / 2) - (player.getSize().y / 2)));
		}



		player.updatePosition(deltaTime);

		// Render loop
		window.clear(Grey);

		//Render walls
		for (unsigned int i{}; i < walls.size(); ++i)
		{
			walls[i]->display(window);
		}

		//Render player
		player.display(window);

		//Display
		window.display();

		//Calculate deltaTime
		timeDifference = std::chrono::steady_clock::now() - currentTime;
		deltaTime = timeDifference.count() / 1000;

		currentTime = std::chrono::steady_clock::now();

		//Print deltaTime
		//std::cout << "Deltatime: " << deltaTime << " FPS: " << 1 / deltaTime << '\n';
		}

	//Free wall memory
	for (unsigned int i{}; i < walls.size(); ++i)
	{
		delete walls[i];
		walls[i] = 0;
	}

	return 0;
	}