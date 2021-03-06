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

//Global functions
bool checkIntersection()
{
	return false;
}

//Classes
class Wall
{
	sf::RectangleShape shape;
	sf::RectangleShape origin;

public:
	Wall(sf::Vector2f size, sf::Vector2f location)
	{
		shape.setSize(size);
		shape.setPosition(location);
		shape.setOrigin(sf::Vector2f(size.x / 2, size.y / 2));
		shape.setFillColor(FaintBlue);
		origin.setPosition(shape.getOrigin());
	}

	void display(sf::RenderWindow& window)
	{
		window.draw(shape);
		window.draw(origin);
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
	double movementSpeed{100.f};
	sf::Vector2f velocity{};
	double gravityModifier{ 100 };
	bool isJumping{ false };
	float bounciness{0.5};
	bool isOnGround{ 0 };
	float jumpStrength{ -500 };
	float airResistance{ 0.5 };

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
		setVelocity(sf::Vector2f(getVelocity().x * airResistance, getVelocity().y));

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
			addForce(sf::Vector2f(0.f, jumpStrength));
			rect.move(sf::Vector2f(0.f, -20.f));
		}

		sf::Vector2f deltaPosition{ sf::Vector2f((velocity.x * deltaTime), velocity.y * deltaTime) };

		rect.move(deltaPosition);

		isOnGround = false;
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

	float getBounciness()
	{
		return bounciness;
	}

	void setOnGround(bool state)
	{
		isOnGround = state;
	}

	bool getOnGround()
	{
		return isOnGround;
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

		//Logic loop

		//Check Y

		{
			for (unsigned int i{}; i < walls.size(); ++i)
			{
				if (((player.getPosition().x < (walls[i]->getPosition().x + walls[i]->getSize().x)) 
					&&
					((player.getPosition().x + player.getSize().x) > walls[i]->getPosition().x)) 
					&&
					((player.getPosition().y < (walls[i]->getPosition().y + walls[i]->getSize().y)) 
					&&
					((player.getPosition().y + player.getSize().y) > walls[i]->getPosition().y)))
					{
						std::cout << player.getVelocity().y << '\n';
						//player.setVelocity(sf::Vector2f(player.getVelocity().x, player.getVelocity().y * -1 * player.getBounciness()));
						if (player.getVelocity().y > 0)
						{
							player.setVelocity(sf::Vector2f(player.getVelocity().x, 0.f));
						}
						player.setPosition(sf::Vector2f(player.getPosition().x, walls[i]->getPosition().y - ((walls[i]->getSize().y / 2) + (player.getSize().y / 2))));
						player.setOnGround(true);
					}
			}
		}

		//Check X

		/*
		float length{ abs(player.getPosition().y - walls[0]->getPosition().y) };
		float gap{length - (player.getSize().y / 2) - (walls[0]->getSize().y / 2)};

		if (gap < 0)
		{
			player.setVelocity(sf::Vector2f(player.getVelocity().x, 0.f));
			player.setPosition(sf::Vector2f(player.getPosition().x, walls[0]->getPosition().y - (walls[0]->getSize().y / 2) - (player.getSize().y / 2)));
		}
		*/

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
					if (player.getOnGround())
					{
						player.jump();
						std::cout << "Jump\n";
					}
				}

				if (event.key.code == sf::Keyboard::Escape)
				{
					window.close();
				}
			}
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