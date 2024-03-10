#pragma once
#include <iostream>
#include "MovementManager.h"
#include "SFML\Graphics.hpp"
#include "Graph.h"
#include <Windows.h>
#include <random>
#include "VertexArray2.h"
#include <chrono>
#include "Demos/easeFunctionsDemo1.h"

using namespace std::chrono;

long long int it = 0;
long long int time_movement{};
bool synced = false;



//void demo3(sf::RenderWindow& window) {
//	MovementManager* movementManager = MovementManager::getInstance();
//
//	// Config
//	bool running = true;
//
//	const int easeTypeSize = 30;
//
//	constexpr static int max_routines = 10000;
//	constexpr static int max_movements_in_routine = 10;
//	constexpr static int max_shapes = 10000;
//
//	float animation_time = 3.f;
//
//	// Routine initialization
//	printf("Creating routines...\n");
//	for (int i = 0; i < max_routines; ++i) {
//		auto movementRoutine = movementManager->createMovementRoutine("SM" + std::to_string(i));
//		sf::Vector2f previous_position = randomPosition();
//		for (int j = 0; j < max_movements_in_routine; ++j) {
//			sf::Vector2f new_position = randomPosition();
//			movementRoutine->addMovement(new movementInfo(previous_position, new_position, randomDelay(0.5f, 5.f), easeFunctions::getTmovement(randomEaseType()), false, randomDelay(), randomDelay()));
//			previous_position = new_position;
//		}
//
//		movementRoutine->setLooping(true);
//		movementRoutine->adjustStartToCurrentTransform(true);
//
//		auto scalingRoutine = movementManager->createScalingRoutine("SC" + std::to_string(i));
//		sf::Vector2f previous_scale = randomScale();
//		for (int j = 0; j < max_movements_in_routine; ++j) {
//			sf::Vector2f new_scale = randomScale();
//			scalingRoutine->addScaling(new scalingInfo(previous_scale, new_scale, randomDelay(0.5f, 5.f), easeFunctions::getTmovement(randomEaseType()), false, randomDelay(), randomDelay()));
//			previous_scale = new_scale;
//		}
//
//		scalingRoutine->setLooping(true);
//		scalingRoutine->adjustStartToCurrentTransform(true);
//
//		auto rotationRoutine = movementManager->createRotationRoutine("SR" + std::to_string(i));
//		float previous_rotation = randomRotation();
//		for (int j = 0; j < max_movements_in_routine; ++j) {
//			float new_rotation = randomRotation();
//			rotationRoutine->addRotation(new rotationInfo(previous_rotation, new_rotation, randomDelay(0.5f, 5.f), easeFunctions::getTmovement(randomEaseType()), false, randomDelay(), randomDelay(), true));
//			previous_rotation = new_rotation;
//		}
//
//		rotationRoutine->setLooping(true);
//		rotationRoutine->adjustStartToCurrentTransform(true);
//	}
//
//
//	// Shapes initialiaztion
//	printf("Creating shapes...\n");
//	VertexArray2* shapes = new VertexArray2[max_shapes];
//	for (int i = 0; i < max_shapes; i++) {
//		if (i % 1000 == 0)
//			printf("Creating shape %d...\n", i);
//		shapes[i].setPrimitiveType(sf::Points);
//		shapes[i].resize(4);
//		shapes[i][0].position = sf::Vector2f(0.f, 0.f);
//		shapes[i][1].position = shapes[i][0].position + sf::Vector2f(5.f, 0.f);
//		shapes[i][2].position = shapes[i][0].position + sf::Vector2f(5.f, 5.f);
//		//shapes[i][3].position = shapes[i][0].position + sf::Vector2f(0.f, 5.f);
//
//		/*shapes[i][0].color = randomColor();
//		shapes[i][1].color = randomColor();
//		shapes[i][2].color = randomColor();
//		shapes[i][3].color = randomColor();*/
//
//		shapes[i].setOrigin(shapes[i].getCentroid());
//		shapes[i].setPosition(randomPosition());
//
//		movementManager->linkMovementRoutine(shapes[i], "SM" + std::to_string(i % max_routines));
//		movementManager->linkScalingRoutine(shapes[i], "SC" + std::to_string(i % max_routines));
//		movementManager->linkRotationRoutine(shapes[i], "SR" + std::to_string(i % max_routines));
//
//		movementManager->startMovementRoutine(shapes[i], "SM" + std::to_string(i % max_routines));
//		movementManager->startScalingRoutine(shapes[i], "SC" + std::to_string(i % max_routines));
//		movementManager->startRotationRoutine(shapes[i], "SR" + std::to_string(i % max_routines));
//	}
//
//	//// GUI Initialization
//	//sf::Font font;
//	//if (!font.loadFromFile("Fonts/Helvetica Regular.otf"))
//	//	std::cout << "ERROR: Font not found!\n";
//
//	//sf::Text text[rows];
//	//for (int i = 0; i < rows; i++) {
//	//	text[i].setFont(font);
//	//	text[i].setCharacterSize(30);
//	//	text[i].setFillColor(sf::Color::White);
//	//	text[i].setString(easeFunctions::getFunctionName(current_ease_type + i));
//	//	text[i].setStyle(sf::Text::Bold);
//	//	text[i].setPosition(50.f, static_cast<float>(window.getSize().y) / static_cast<float>(rows) * static_cast<float>(i) + window.getSize().y / static_cast<float>(rows) / 2.f - text[i].getGlobalBounds().height / 2.f);
//	//}
//
//	sf::Clock dt_clock;
//	float dt;
//	float wait_time = 0.5f;
//
//	printf("MovementManager size Demo2:\n");
//	long long int sizeMovement = movementManager->getSizeMovement();
//	long long int sizeScaling = movementManager->getSizeScaling();
//	long long int sizeRotation = movementManager->getSizeRotation();
//	printf("Movement size: %lld B\t%f KB\t%f MB\n", sizeMovement, static_cast<float>(sizeMovement) / 1024.f, static_cast<float>(sizeMovement) / 1024.f / 1024.f);
//	printf("Scaling size: %lld B\t%f KB\t%f MB\n", sizeScaling, static_cast<float>(sizeScaling) / 1024.f, static_cast<float>(sizeScaling) / 1024.f / 1024.f);
//	printf("Rotation size: %lld B\t%f KB\t%f MB\n", sizeRotation, static_cast<float>(sizeRotation) / 1024.f, static_cast<float>(sizeRotation) / 1024.f / 1024.f);
//
//	while (running)
//	{
//		dt = dt_clock.restart().asSeconds();
//
//		movementManager->update(dt);
//		it++;
//
//		sf::Event event;
//		if (window.pollEvent(event))
//		{
//			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
//				running = false;
//
//			if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W)) {
//
//			}
//			else if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S)) {
//
//			}
//		}
//
//		window.clear();
//		for (int i = 0; i < max_shapes; i++) {
//			window.draw(shapes[i], shapes[i].getTransform());
//		}
//
//		window.display();
//	}
//
//	movementManager->deleteMovementRoutine();
//	movementManager->deleteScalingRoutine();
//	movementManager->deleteRotationRoutine();
//
//	delete[] shapes;
//}

int main()
{
	srand(time(NULL));
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Ease functions", sf::Style::Fullscreen, settings);
	window.setVerticalSyncEnabled(true);

	//movementDemo1(window);
	//plotDemo(window);
	//movementDemo2(window);
	movementDemo3(window);


	return 0;
}