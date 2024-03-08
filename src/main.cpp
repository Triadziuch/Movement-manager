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

sf::Vector2f getCentroid(const sf::VertexArray& vertexArray) {
	sf::Vector2f centroid{};
	for (size_t i = 0; i < vertexArray.getVertexCount(); ++i)
		centroid += vertexArray.operator[](i).position;
	centroid /= static_cast<float>(vertexArray.getVertexCount());
	return centroid;
}

inline const sf::Vector2f randomPosition(const sf::Vector2f& min = sf::Vector2f(0.f, 0.f), const sf::Vector2f& max = sf::Vector2f(1920.f, 1080.f)) {
	return sf::Vector2f(static_cast<float>(rand() % static_cast<int>(max.x - min.x) + static_cast<int>(min.x)), static_cast<float>(rand() % static_cast<int>(max.y - min.y) + static_cast<int>(min.y)));
}

inline const sf::Vector2f randomScale(const sf::Vector2f& min = sf::Vector2f(0.5f, 0.5f), const sf::Vector2f& max = sf::Vector2f(1.5f, 1.5f)) {
	return sf::Vector2f(static_cast<float>(rand() % static_cast<int>(max.x * 100 - min.x * 100) + static_cast<int>(min.x * 100)) / 100.f, static_cast<float>(rand() % static_cast<int>(max.y * 100 - min.y * 100) + static_cast<int>(min.y * 100)) / 100.f);
}

inline const float randomRotation(const float min = 0.f, const float max = 360.f) {
	return static_cast<float>(rand() % static_cast<int>(max - min) + static_cast<int>(min));
}

inline const size_t randomEaseType(const size_t min = 0, const size_t max = 30) {
	return 0;
	size_t random = static_cast<size_t>(rand() % static_cast<int>(max - min) + static_cast<int>(min));
	if (random == 18 || random == 19 || random == 20)
		return randomEaseType(min, max);
	return static_cast<size_t>(rand() % static_cast<int>(max - min) + static_cast<int>(min));
}

inline const float randomDelay(const float min = 0.f, const float max = 1.f) {
	return 0.5f;
	return static_cast<float>(rand() % static_cast<int>(max * 100 - min * 100) + static_cast<int>(min * 100)) / 100.f;
}

inline sf::Color randomColor() {
	return sf::Color(rand() % 235 + 20, rand() % 235 + 20, rand() % 235 + 20);
}

void demo4(sf::RenderWindow& window) {
	MovementManager* movementManager = MovementManager::getInstance();

	// Config
	bool running = true;

	const int easeTypeSize = 30;

	constexpr static int max_routines = 10000;
	constexpr static int max_movements_in_routine = 10;
	constexpr static int max_shapes = 10000;

	float animation_time = 3.f;

	long long int time_creating = 0, time_linking = 0, time_starting = 0;

	// Routine initialization
	printf("Creating routines...\n");
	for (int i = 0; i < max_routines; ++i) {
		auto start = high_resolution_clock::now();
		auto movementRoutine = movementManager->createMovementRoutine("SM" + std::to_string(i));
		auto stop = high_resolution_clock::now();
		time_creating += duration_cast<nanoseconds>(stop - start).count();
		sf::Vector2f previous_position = randomPosition();
		for (int j = 0; j < max_movements_in_routine; ++j) {
			sf::Vector2f new_position = randomPosition();
			movementRoutine->addMovement(new movementInfo(previous_position, new_position, animation_time, easeFunctions::getTmovement(randomEaseType()), false, randomDelay(), randomDelay()));
			previous_position = new_position;
		}
			
		movementRoutine->setLooping(true);
		movementRoutine->adjustStartToCurrentTransform(true);

		auto scalingRoutine = movementManager->createScalingRoutine("SC" + std::to_string(i));
		sf::Vector2f previous_scale = randomScale();
		for (int j = 0; j < max_movements_in_routine; ++j) {
			sf::Vector2f new_scale = randomScale();
			scalingRoutine->addScaling(new scalingInfo(previous_scale, new_scale, animation_time, easeFunctions::getTmovement(randomEaseType()), false, randomDelay(), randomDelay()));
			previous_scale = new_scale;
		}

		scalingRoutine->setLooping(true);
		scalingRoutine->adjustStartToCurrentTransform(true);

		auto rotationRoutine = movementManager->createRotationRoutine("SR" + std::to_string(i));
		float previous_rotation = randomRotation();
		for (int j = 0; j < max_movements_in_routine; ++j) {
			float new_rotation = randomRotation();
			rotationRoutine->addRotation(new rotationInfo(previous_rotation, new_rotation, animation_time, easeFunctions::getTmovement(randomEaseType()), false, randomDelay(), randomDelay(), true));
			previous_rotation = new_rotation;
		}

		rotationRoutine->setLooping(true);
		rotationRoutine->adjustStartToCurrentTransform(true);
	}


	// Shapes initialiaztion
	printf("Creating shapes...\n");
	sf::RectangleShape *shapes = new sf::RectangleShape[max_shapes];
	for (int i = 0; i < max_shapes; i++) {
		if (i % 1000 == 0)
			printf("Creating shape %d...\n", i);
		shapes[i].setSize(sf::Vector2f(1.f, 1.f));
		shapes[i].setFillColor(sf::Color::Blue);
		shapes[i].setOrigin(shapes[i].getSize().x / 2.f, shapes[i].getSize().y / 2.f);
		shapes[i].setPosition(randomPosition());

		auto start = high_resolution_clock::now();
		movementManager->linkMovementRoutine(shapes[i], "SM" + std::to_string(i % max_routines));
		//movementManager->linkScalingRoutine(shapes[i], "SC" + std::to_string(i % max_routines));
		//movementManager->linkRotationRoutine(shapes[i], "SR" + std::to_string(i % max_routines));
		auto stop = high_resolution_clock::now();
		time_linking += duration_cast<nanoseconds>(stop - start).count();

		auto start2 = high_resolution_clock::now();
		movementManager->startMovementRoutine(shapes[i], "SM" + std::to_string(i % max_routines));
		//movementManager->startScalingRoutine(shapes[i], "SC" + std::to_string(i % max_routines));
		//movementManager->startRotationRoutine(shapes[i], "SR" + std::to_string(i % max_routines));
		auto stop2 = high_resolution_clock::now();
		time_starting += duration_cast<nanoseconds>(stop2 - start2).count();
	}

	//// GUI Initialization
	//sf::Font font;
	//if (!font.loadFromFile("Fonts/Helvetica Regular.otf"))
	//	std::cout << "ERROR: Font not found!\n";

	//sf::Text text[rows];
	//for (int i = 0; i < rows; i++) {
	//	text[i].setFont(font);
	//	text[i].setCharacterSize(30);
	//	text[i].setFillColor(sf::Color::White);
	//	text[i].setString(easeFunctions::getFunctionName(current_ease_type + i));
	//	text[i].setStyle(sf::Text::Bold);
	//	text[i].setPosition(50.f, static_cast<float>(window.getSize().y) / static_cast<float>(rows) * static_cast<float>(i) + window.getSize().y / static_cast<float>(rows) / 2.f - text[i].getGlobalBounds().height / 2.f);
	//}

	sf::Clock dt_clock;
	float dt;
	float wait_time = 0.5f;

	printf("MovementManager size Demo2:\n");
	long long int sizeMovement = movementManager->getSizeMovement();
	long long int sizeScaling = movementManager->getSizeScaling();
	long long int sizeRotation = movementManager->getSizeRotation();
	printf("Movement size: %lld B\t%f KB\t%f MB\n", sizeMovement, static_cast<float>(sizeMovement) / 1024.f, static_cast<float>(sizeMovement) / 1024.f / 1024.f);
	printf("Scaling size: %lld B\t%f KB\t%f MB\n", sizeScaling, static_cast<float>(sizeScaling) / 1024.f, static_cast<float>(sizeScaling) / 1024.f / 1024.f);
	printf("Rotation size: %lld B\t%f KB\t%f MB\n\n\n", sizeRotation, static_cast<float>(sizeRotation) / 1024.f, static_cast<float>(sizeRotation) / 1024.f / 1024.f);

	printf("Time_creating: %lld ns\t=  %f ms\t=  %f s\n", time_creating, static_cast<float>(time_creating) / 1000000.f, static_cast<float>(time_creating) / 1000000000.f);
	printf("Time linking:  %lld ns\t=  %f ms\t=  %f s\n", time_linking, static_cast<float>(time_linking) / 1000000.f, static_cast<float>(time_linking) / 1000000000.f);
	printf("Time starting: %lld ns\t=  %f ms\t=  %f s\n", time_starting, static_cast<float>(time_starting) / 1000000.f, static_cast<float>(time_starting) / 1000000000.f);
	printf("Time total:    %lld ns\t=  %f ms\t=  %f s\n\n\n", time_creating + time_linking + time_starting, static_cast<float>(time_creating + time_linking + time_starting) / 1000000.f, static_cast<float>(time_creating + time_linking + time_starting) / 1000000000.f);

	while (running)
	{
		dt = dt_clock.restart().asSeconds();

		auto start = high_resolution_clock::now();
		movementManager->update(dt);
		auto stop = high_resolution_clock::now();
		time_movement += duration_cast<nanoseconds>(stop - start).count();
		it++;

		if (it == 100 && !synced) {
			synced = true;
			it = 0;
			time_movement = 0;
		}

		if (it % 500 == 0 && it != 0) 
			std::cout << "Time: " << static_cast<float>(time_movement / it) << " ns  =  " << static_cast<float>(time_movement / it) / 100000.f << " ms  =  " << static_cast<float>(time_movement / it) / 100000000.f << " s\n";

		sf::Event event;
		if (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				running = false;

			if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W)) {

			}
			else if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S)) {

			}
		}

		window.clear();
		for (int i = 0; i < max_shapes; i++) {
			window.draw(shapes[i]);
		}

		window.display();
	}

	movementManager->deleteMovementRoutine();
	movementManager->deleteScalingRoutine();
	movementManager->deleteRotationRoutine();

	delete[] shapes;
}

void demo3(sf::RenderWindow& window) {
	MovementManager* movementManager = MovementManager::getInstance();

	// Config
	bool running = true;

	const int easeTypeSize = 30;

	constexpr static int max_routines = 10000;
	constexpr static int max_movements_in_routine = 10;
	constexpr static int max_shapes = 10000;

	float animation_time = 3.f;

	// Routine initialization
	printf("Creating routines...\n");
	for (int i = 0; i < max_routines; ++i) {
		auto movementRoutine = movementManager->createMovementRoutine("SM" + std::to_string(i));
		sf::Vector2f previous_position = randomPosition();
		for (int j = 0; j < max_movements_in_routine; ++j) {
			sf::Vector2f new_position = randomPosition();
			movementRoutine->addMovement(new movementInfo(previous_position, new_position, randomDelay(0.5f, 5.f), easeFunctions::getTmovement(randomEaseType()), false, randomDelay(), randomDelay()));
			previous_position = new_position;
		}

		movementRoutine->setLooping(true);
		movementRoutine->adjustStartToCurrentTransform(true);

		auto scalingRoutine = movementManager->createScalingRoutine("SC" + std::to_string(i));
		sf::Vector2f previous_scale = randomScale();
		for (int j = 0; j < max_movements_in_routine; ++j) {
			sf::Vector2f new_scale = randomScale();
			scalingRoutine->addScaling(new scalingInfo(previous_scale, new_scale, randomDelay(0.5f, 5.f), easeFunctions::getTmovement(randomEaseType()), false, randomDelay(), randomDelay()));
			previous_scale = new_scale;
		}

		scalingRoutine->setLooping(true);
		scalingRoutine->adjustStartToCurrentTransform(true);

		auto rotationRoutine = movementManager->createRotationRoutine("SR" + std::to_string(i));
		float previous_rotation = randomRotation();
		for (int j = 0; j < max_movements_in_routine; ++j) {
			float new_rotation = randomRotation();
			rotationRoutine->addRotation(new rotationInfo(previous_rotation, new_rotation, randomDelay(0.5f, 5.f), easeFunctions::getTmovement(randomEaseType()), false, randomDelay(), randomDelay(), true));
			previous_rotation = new_rotation;
		}

		rotationRoutine->setLooping(true);
		rotationRoutine->adjustStartToCurrentTransform(true);
	}


	// Shapes initialiaztion
	printf("Creating shapes...\n");
	VertexArray2* shapes = new VertexArray2[max_shapes];
	for (int i = 0; i < max_shapes; i++) {
		if (i % 1000 == 0)
			printf("Creating shape %d...\n", i);
		shapes[i].setPrimitiveType(sf::Points);
		shapes[i].resize(4);
		shapes[i][0].position = sf::Vector2f(0.f, 0.f);
		shapes[i][1].position = shapes[i][0].position + sf::Vector2f(5.f, 0.f);
		shapes[i][2].position = shapes[i][0].position + sf::Vector2f(5.f, 5.f);
		//shapes[i][3].position = shapes[i][0].position + sf::Vector2f(0.f, 5.f);

		/*shapes[i][0].color = randomColor();
		shapes[i][1].color = randomColor();
		shapes[i][2].color = randomColor();
		shapes[i][3].color = randomColor();*/

		shapes[i].setOrigin(shapes[i].getCentroid());
		shapes[i].setPosition(randomPosition());

		movementManager->linkMovementRoutine(shapes[i], "SM" + std::to_string(i % max_routines));
		movementManager->linkScalingRoutine(shapes[i], "SC" + std::to_string(i % max_routines));
		movementManager->linkRotationRoutine(shapes[i], "SR" + std::to_string(i % max_routines));

		movementManager->startMovementRoutine(shapes[i], "SM" + std::to_string(i % max_routines));
		movementManager->startScalingRoutine(shapes[i], "SC" + std::to_string(i % max_routines));
		movementManager->startRotationRoutine(shapes[i], "SR" + std::to_string(i % max_routines));
	}

	//// GUI Initialization
	//sf::Font font;
	//if (!font.loadFromFile("Fonts/Helvetica Regular.otf"))
	//	std::cout << "ERROR: Font not found!\n";

	//sf::Text text[rows];
	//for (int i = 0; i < rows; i++) {
	//	text[i].setFont(font);
	//	text[i].setCharacterSize(30);
	//	text[i].setFillColor(sf::Color::White);
	//	text[i].setString(easeFunctions::getFunctionName(current_ease_type + i));
	//	text[i].setStyle(sf::Text::Bold);
	//	text[i].setPosition(50.f, static_cast<float>(window.getSize().y) / static_cast<float>(rows) * static_cast<float>(i) + window.getSize().y / static_cast<float>(rows) / 2.f - text[i].getGlobalBounds().height / 2.f);
	//}

	sf::Clock dt_clock;
	float dt;
	float wait_time = 0.5f;

	printf("MovementManager size Demo2:\n");
	long long int sizeMovement = movementManager->getSizeMovement();
	long long int sizeScaling = movementManager->getSizeScaling();
	long long int sizeRotation = movementManager->getSizeRotation();
	printf("Movement size: %lld B\t%f KB\t%f MB\n", sizeMovement, static_cast<float>(sizeMovement) / 1024.f, static_cast<float>(sizeMovement) / 1024.f / 1024.f);
	printf("Scaling size: %lld B\t%f KB\t%f MB\n", sizeScaling, static_cast<float>(sizeScaling) / 1024.f, static_cast<float>(sizeScaling) / 1024.f / 1024.f);
	printf("Rotation size: %lld B\t%f KB\t%f MB\n", sizeRotation, static_cast<float>(sizeRotation) / 1024.f, static_cast<float>(sizeRotation) / 1024.f / 1024.f);

	while (running)
	{
		dt = dt_clock.restart().asSeconds();

		movementManager->update(dt);
		it++;

		sf::Event event;
		if (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				running = false;

			if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W)) {

			}
			else if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S)) {

			}
		}

		window.clear();
		for (int i = 0; i < max_shapes; i++) {
			window.draw(shapes[i], shapes[i].getTransform());
		}

		window.display();
	}

	movementManager->deleteMovementRoutine();
	movementManager->deleteScalingRoutine();
	movementManager->deleteRotationRoutine();

	delete[] shapes;
}

int main()
{
	srand(time(NULL));
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Ease functions", sf::Style::Fullscreen, settings);
	window.setVerticalSyncEnabled(true);

	//movementDemo1(window);
	//plotDemo(window);
	movementDemo2(window);



	return 0;
}