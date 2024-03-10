#pragma once
#include <iostream>
#include "MovementManager.h"
#include "SFML\Graphics.hpp"
#include <Windows.h>
#include <random>
#include "Demos/Demos.h"

int main()
{
	srand(time(NULL));
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Ease functions", sf::Style::Fullscreen, settings);
	window.setVerticalSyncEnabled(true);
	bool running = true;

	// GUI
	sf::Font font;
	if (!font.loadFromFile("Fonts/Helvetica Regular.otf"))
		throw std::runtime_error("ERROR: Font not found!\n");

	std::string demo_string[4] = { "Ease functions demo 1", "Ease functions demo 2", "Ease functions demo 3", "Plot demo" };
	sf::Text demo_text[4];
	sf::Color selected_color(150, 150, 150);

	int selected = 0;
	for (size_t i = 0; i < 4; ++i) {
		demo_text[i].setFont(font);
		demo_text[i].setCharacterSize(36u);
		demo_text[i].setFillColor(sf::Color::White);
		demo_text[i].setString(demo_string[i]);
		demo_text[i].setStyle(sf::Text::Bold);
		demo_text[i].setOrigin(demo_text[i].getGlobalBounds().width / 2.f, demo_text[i].getGlobalBounds().height / 2.f);
		demo_text[i].setPosition(static_cast<float>(window.getSize().x) / 2.f, static_cast<float>(window.getSize().y) / 4.f + i * 150.f);
	}
	demo_text[0].setFillColor(selected_color);

	while (running) {
		sf::Event event;
		if (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				running = false;


			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W) {
					demo_text[selected].setFillColor(sf::Color::White);
					if (--selected < 0)
						selected = 3;
					demo_text[selected].setFillColor(selected_color);
				}
				else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) {
					demo_text[selected].setFillColor(sf::Color::White);
					if (++selected > 3)
						selected = 0;
					demo_text[selected].setFillColor(selected_color);
				}
				else if (event.key.code == sf::Keyboard::Return) {
					switch (selected) {
					case 0:
						movementDemo1(window);
						break;
					case 1:
						movementDemo2(window);
						break;
					case 2:
						movementDemo3(window);
						break;
					case 3:
						plotDemo(window);
						break;
					}
				}
			}
		}

		window.clear();

		for (size_t i = 0; i < 4; ++i) {
			window.draw(demo_text[i]);
		}

		window.display();
	}
	//movementDemo1(window);
	//plotDemo(window);
	//movementDemo2(window);
	//movementDemo3(window);


	return 0;
}