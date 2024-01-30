#include <iostream>
#include "MovementManager.h"
#include "SFML\Graphics.hpp"
#include "Graph.h"
#include <Windows.h>
int iterator = 0;
int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Ease functions", sf::Style::Fullscreen, settings);

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

	float animation_time = 30.f;

	// Start position initialization
	sf::Vector2f start_pos[3];
	for (int i = 0; i < 3; i++) 
		start_pos[i] = sf::Vector2f(400.f, static_cast<float>(window.getSize().y) / 3.f * static_cast<float>(i) + window.getSize().y / 3.f / 2.f);

	// End position initialization
	sf::Vector2f end_pos[3];
	for (int i = 0; i < 3; i++)
		end_pos[i] = sf::Vector2f(static_cast<float>(window.getSize().x) - 400.f, static_cast<float>(window.getSize().y) / 3.f * static_cast<float>(i) + window.getSize().y / 3.f / 2.f);

	// Shapes initialiaztion
	sf::CircleShape shapes[3];
	for (int i = 0; i < 3; i++) {
		shapes[i].setRadius(50.f);
		shapes[i].setFillColor(sf::Color::Blue);
		shapes[i].setOrigin(shapes[i].getRadius(), shapes[i].getRadius());
		shapes[i].setPosition(start_pos[i]);
	}

	int current_ease_type = 0;

	// Graph initialization
	Graph graphs[3];
	for (int i = 0; i < 3; i++) {
		graphs[i].setPrecision(75);
		graphs[i].setSize(200.f, 200.f);
		graphs[i].setPosition(sf::Vector2f(end_pos[i].x + 150.f, end_pos[i].y + graphs[i].getSize().y / 2.f));
		graphs[i].setFunction(movementManager.getFunctionPointer(easeType[current_ease_type + i].second));
	}

	// GUI Initialization
	sf::Font font;
	if (!font.loadFromFile("Fonts/Helvetica Regular.otf"))
		std::cout << "ERROR: Font not found!\n";
	sf::Text text[3];

	for (int i = 0; i < 3; i++) {
		text[i].setFont(font);
		text[i].setCharacterSize(30);
		text[i].setFillColor(sf::Color::White);
		text[i].setString(easeType[current_ease_type + i].first);
		text[i].setPosition(50.f, static_cast<float>(window.getSize().y) / 3.f * static_cast<float>(i) + window.getSize().y / 3.f / 2.f - text[i].getGlobalBounds().height / 2.f);
	}

	sf::Clock dt_clock;
	float dt;
	float wait_time = 0.f;
	float wait_time_max = 0.5f;
	Sleep(1000);
	while (window.isOpen())
	{
		dt = dt_clock.restart().asSeconds();

		if (movementManager.getMovementCount() == 0) {
			wait_time += dt;
			if (wait_time >= wait_time_max) {
				wait_time = 0.f;
				for (int i = 0; i < 3; i++)
					shapes[i].setPosition(start_pos[i]);

				movementManager.addMovement(start_pos[0], end_pos[0], animation_time, &shapes[0], easeType[current_ease_type].second);
				movementManager.addMovement(start_pos[1], end_pos[1], animation_time, &shapes[1], easeType[current_ease_type + 1].second);
				movementManager.addMovement(start_pos[2], end_pos[2], animation_time, &shapes[2], easeType[current_ease_type + 2].second);
			}
		}
		
		movementManager.update(dt);

		sf::Event event;
		if (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				window.close();

			if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Up || event.key.code ==sf::Keyboard::W)) {
				current_ease_type += 3;
				if (current_ease_type >= easeTypeSize - 2)
					current_ease_type = 0;

				for (int i = 0; i < 3; i++) {
					text[i].setString(easeType[current_ease_type + i].first);
					graphs[i].setFunction(movementManager.getFunctionPointer(easeType[current_ease_type + i].second));
				}
					


				movementManager.resetMovement();
			}
			else if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S)) {
				current_ease_type -= 3;
				if (current_ease_type < 0)
					current_ease_type = easeTypeSize - 3;

				for (int i = 0; i < 3; i++) {
					text[i].setString(easeType[current_ease_type + i].first);
					graphs[i].setFunction(movementManager.getFunctionPointer(easeType[current_ease_type + i].second));
				}

				movementManager.resetMovement();
			}
		}

		window.clear();
		for (int i = 0; i < 3; i++) {
			window.draw(text[i]);
			window.draw(shapes[i]);
			graphs[i].draw(window);
		}
		window.display();
	}

	return 0;
}