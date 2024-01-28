#include <iostream>
#include "MovementManager.h"
#include "SFML\Graphics.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Ease functions");
	MovementManager movementManager;

	sf::Vector2f starting_pos{ 100.f, 300.f };
	sf::Vector2f ending_pos{ 700.f, 300.f };
	float movementTime = 4.f;

	sf::CircleShape shape(20.f);
	shape.setFillColor(sf::Color::Blue);
	shape.setOrigin(shape.getRadius(), shape.getRadius());
	shape.setPosition(starting_pos);

	sf::Clock dt_clock;
	float dt;

	MovementManager::addMovement(starting_pos, ending_pos, movementTime, &shape, movement_type::IN_EXP);

	while (window.isOpen())
	{
		dt = dt_clock.restart().asSeconds();

		movementManager.update(dt);

		if (movementManager.getMovementCount() == 0) {
			shape.setPosition(starting_pos);
			MovementManager::addMovement(starting_pos, ending_pos, movementTime, &shape, movement_type::IN_EXP);
		}

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}