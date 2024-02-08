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
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);
	MovementManager movementManager;

	// Configuration
	constexpr size_t easeTypeSize = 30;
	const int rows = 3;
	float animation_time = 3.f;
	int current_ease_type = 0;
	float wait_time_max = 0.5f;

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
	sf::Vector2f start_pos[rows];
	for (int i = 0; i < rows; i++) 
		start_pos[i] = sf::Vector2f(500.f, static_cast<float>(window.getSize().y) / static_cast<float>(rows) * static_cast<float>(i) + window.getSize().y / static_cast<float>(rows) / 2.f);

	// End position initialization
	sf::Vector2f end_pos[rows];
	for (int i = 0; i < rows; i++)
		end_pos[i] = sf::Vector2f(static_cast<float>(window.getSize().x) - 400.f, static_cast<float>(window.getSize().y) / static_cast<float>(rows) * static_cast<float>(i) + window.getSize().y / static_cast<float>(rows) / 2.f);

	// Shapes initialiaztion
	sf::RectangleShape shapes[5];
	for (int i = 0; i < rows; i++) {
		//shapes[i].setRadius(50.f);
		shapes[i].setSize(sf::Vector2f(100.f, 100.f));
		shapes[i].setFillColor(sf::Color::Blue);
		shapes[i].setOrigin(shapes[i].getSize().x / 2.f, shapes[i].getSize().y / 2.f);
		//shapes[i].setOrigin(shapes[i].getRadius(), shapes[i].getRadius());
		shapes[i].setPosition(start_pos[i]);
	}

	// Graph initialization
	Graph graphs[rows];
	for (int i = 0; i < rows; i++) {
		graphs[i].setPrecision(200);
		graphs[i].setSize(200.f, 200.f);
		graphs[i].setPosition(sf::Vector2f(end_pos[i].x + 150.f, end_pos[i].y + graphs[i].getSize().y / 2.f));
		graphs[i].setFunction(movementManager.getFunctionPointer(easeType[current_ease_type + i].second));
	}

	// GUI Initialization
	sf::Font font;
	if (!font.loadFromFile("Fonts/Helvetica Regular.otf"))
		std::cout << "ERROR: Font not found!\n";

	sf::Text text[rows];
	for (int i = 0; i < rows; i++) {
		text[i].setFont(font);
		text[i].setCharacterSize(30);
		text[i].setFillColor(sf::Color::White);
		text[i].setString(easeType[current_ease_type + i].first);
		text[i].setStyle(sf::Text::Bold);
		text[i].setPosition(50.f, static_cast<float>(window.getSize().y) / static_cast<float>(rows) * static_cast<float>(i) + window.getSize().y / static_cast<float>(rows) / 2.f - text[i].getGlobalBounds().height / 2.f);
	}

	// Arrows initialization
	sf::VertexArray up_arrow(sf::LineStrip, 3u), down_arrow(sf::LineStrip, 3u);
	up_arrow[0].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f - 40.f, 60.f);
	up_arrow[1].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f, 30.f);
	up_arrow[2].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f + 40.f, 60.f);

	down_arrow[0].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f - 40.f, static_cast<float>(window.getSize().y) - 60.f);
	down_arrow[1].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f, static_cast<float>(window.getSize().y) - 30.f);
	down_arrow[2].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f + 40.f, static_cast<float>(window.getSize().y) - 60.f);

	float arrow_movement_time = 0.5f;
	float arrow_scaling_time = 0.25f;
	float arrow_rotation_time = 0.25f;
	float arrow_wait_time = 0.f;

	movementManager.addMovement(&up_arrow, up_arrow[0].position, up_arrow[0].position + sf::Vector2f(0.f, -20.f), arrow_movement_time,movement_type::IN_OUT_SINE, true, arrow_wait_time);
	movementManager.addMovement(&down_arrow, down_arrow[0].position, down_arrow[0].position + sf::Vector2f(0.f, 20.f), arrow_movement_time, movement_type::IN_OUT_SINE, true, arrow_wait_time);
	movementManager.addScaling(&up_arrow, { 1.5f, 1.5f }, arrow_scaling_time, movement_type::IN_OUT_SINE, true, arrow_wait_time);
	movementManager.addScaling(&down_arrow, { 1.5f, 1.5f }, arrow_scaling_time, movement_type::IN_OUT_SINE, true, arrow_wait_time);
	movementManager.addRotation(&up_arrow, 90.f, arrow_rotation_time, movement_type::IN_OUT_SINE, true, true, arrow_wait_time);
	movementManager.addRotation(&down_arrow, 90.f, arrow_rotation_time, movement_type::IN_OUT_SINE, true, true, arrow_wait_time);

	sf::Clock dt_clock;
	float dt;
	float wait_time = 0.5f;

	for (int i = 0; i < rows; i++) {
		movementManager.addMovement(&shapes[i], start_pos[i], end_pos[i], animation_time, easeType[current_ease_type + i].second, true, 1.f);
		movementManager.addScaling(&shapes[i], {1.5f, 1.5f}, animation_time, easeType[current_ease_type + i].second, true, 1.f);
		movementManager.addRotation(&shapes[i], 360.f, animation_time, easeType[current_ease_type + i].second, true, true, 1.f);
	}
		

	while (window.isOpen())
	{
		dt = dt_clock.restart().asSeconds();
		
		movementManager.update(dt);

		sf::Event event;
		if (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				window.close();

			if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Up || event.key.code ==sf::Keyboard::W)) {
				current_ease_type += rows;
				if (current_ease_type >= easeTypeSize - 2)
					current_ease_type = 0;

				for (int i = 0; i < rows; i++) {
					text[i].setString(easeType[current_ease_type + i].first);
					graphs[i].setFunction(movementManager.getFunctionPointer(easeType[current_ease_type + i].second));
				}

				for (int i = 0; i < rows; i++) {
					movementManager.undoMovement(&shapes[i]);
					movementManager.undoScaling(&shapes[i]);
					movementManager.undoRotation(&shapes[i]);
					movementManager.addMovement(&shapes[i], start_pos[i], end_pos[i], animation_time, easeType[current_ease_type + i].second, true, 1.f);
					movementManager.addScaling(&shapes[i], { 1.5f, 1.5f }, animation_time, easeType[current_ease_type + i].second, true, 1.f);
					movementManager.addRotation(&shapes[i], 360.f, animation_time, easeType[current_ease_type + i].second, true, true, 1.f);
				}

				movementManager.resetScaling(&up_arrow);
				movementManager.resetScaling(&down_arrow);
			}
			else if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S)) {
				current_ease_type -= rows;
				if (current_ease_type < 0)
					current_ease_type = easeTypeSize - rows;

				for (int i = 0; i < rows; i++) {
					text[i].setString(easeType[current_ease_type + i].first);
					graphs[i].setFunction(movementManager.getFunctionPointer(easeType[current_ease_type + i].second));
				}

				for (int i = 0; i < rows; i++) {
					movementManager.undoMovement(&shapes[i]);
					movementManager.undoScaling(&shapes[i]);
					movementManager.undoRotation(&shapes[i]);
					movementManager.addMovement(&shapes[i], start_pos[i], end_pos[i], animation_time, easeType[current_ease_type + i].second, true, 1.f);
					movementManager.addScaling(&shapes[i], { 1.5f, 1.5f }, animation_time, easeType[current_ease_type + i].second, true, 1.f);
					movementManager.addRotation(&shapes[i], 360.f, animation_time, easeType[current_ease_type + i].second, true, true, 1.f);
				}
			}
		}

		window.clear();
		for (int i = 0; i < rows; i++) {
			window.draw(text[i]);
			window.draw(shapes[i]);

			graphs[i].draw(window);

			window.draw(up_arrow);
			window.draw(down_arrow);
		}
		window.display();
	}

	return 0;
}