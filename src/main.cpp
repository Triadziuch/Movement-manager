#include <iostream>
#include "MovementManager.h"
#include "SFML\Graphics.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Ease functions", sf::Style::Fullscreen);
	MovementManager movementManager;

	constexpr size_t easeTypeSize = 30;
	std::pair<std::string, movement_type> easeType[easeTypeSize] = {
		{"IN_SINE", movement_type::IN_SINE},
		{"OUT_SINE", movement_type::OUT_SINE},
		{"IN_OUT_SINE", movement_type::IN_OUT_SINE},
		{"IN_QUAD", movement_type::IN_QUAD},
		{"OUT_QUAD", movement_type::OUT_QUAD},
		{"IN_OUT_QUAD", movement_type::IN_OUT_QUAD},
		{"IN_CUBIC", movement_type::IN_CUBIC},
		{"OUT_CUBIC", movement_type::OUT_CUBIC},
		{"IN_OUT_CUBIC", movement_type::IN_OUT_CUBIC},
		{"IN_QUART", movement_type::IN_QUART},
		{"OUT_QUART", movement_type::OUT_QUART},
		{"IN_OUT_QUART", movement_type::IN_OUT_QUART},
		{"IN_QUINT", movement_type::IN_QUINT},
		{"OUT_QUINT", movement_type::OUT_QUINT},
		{"IN_OUT_QUINT", movement_type::IN_OUT_QUINT},
		{"IN_EXPO", movement_type::IN_EXPO},
		{"OUT_EXPO", movement_type::OUT_EXPO},
		{"IN_OUT_EXPO", movement_type::IN_OUT_EXPO},
		{"IN_CIRC", movement_type::IN_CIRC},
		{"OUT_CIRC", movement_type::OUT_CIRC},
		{"IN_OUT_CIRC", movement_type::IN_OUT_CIRC},
		{"IN_BACK", movement_type::IN_BACK},
		{"OUT_BACK", movement_type::OUT_BACK},
		{"IN_OUT_BACK", movement_type::IN_OUT_BACK},
		{"IN_ELASTIC", movement_type::IN_ELASTIC},
		{"OUT_ELASTIC", movement_type::OUT_ELASTIC},
		{"IN_OUT_ELASTIC", movement_type::IN_OUT_ELASTIC},
		{"IN_BOUNCE", movement_type::IN_BOUNCE},
		{"OUT_BOUNCE", movement_type::OUT_BOUNCE},
		{"IN_OUT_BOUNCE", movement_type::IN_OUT_BOUNCE}
	};

	// Start position initialization
	sf::Vector2f start_pos[3];
	for (int i = 0; i < 3; i++) 
		start_pos[i] = sf::Vector2f(300.f, static_cast<float>(window.getSize().y) / 3.f * static_cast<float>(i) + window.getSize().y / 3.f / 2.f);

	sf::Vector2f end_pos[3];
	for (int i = 0; i < 3; i++)
		end_pos[i] = sf::Vector2f(static_cast<float>(window.getSize().x) - 300.f, static_cast<float>(window.getSize().y) / 3.f * static_cast<float>(i) + window.getSize().y / 3.f / 2.f);

	sf::CircleShape shapes[3];
	for (int i = 0; i < 3; i++) {
		shapes[i].setRadius(20.f);
		shapes[i].setFillColor(sf::Color::Blue);
		shapes[i].setOrigin(shapes[i].getRadius(), shapes[i].getRadius());
		shapes[i].setPosition(start_pos[i]);
	}

	int n = 15;
	float section_height = window.getSize().y / n;
	sf::CircleShape* shapes = new sf::CircleShape[n];



	float animation_time = 2.f;
	movement_type type = movement_type::LINEAR;
	for (int i = 0; i < n; i++) {
		shapes[i].setRadius(20.f);
		shapes[i].setFillColor(sf::Color::Blue);
		shapes[i].setOrigin(shapes[i].getRadius(), shapes[i].getRadius());
		shapes[i].setPosition(300.f, section_height * i + section_height / 2);
	}

	sf::Clock dt_clock;
	float dt;

	float wait_time = 0.f;
	float wait_time_max = 2.f;

	while (window.isOpen())
	{
		dt = dt_clock.restart().asSeconds();

		movementManager.update(dt);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				window.close();
		}

		window.clear();
		window.display();
	}

	return 0;
}