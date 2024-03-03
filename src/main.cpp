#include <iostream>
#include "MovementManager.h"
#include "SFML\Graphics.hpp"
#include "Graph.h"
#include <Windows.h>
#include <random>
#include "VertexArray2.h"
#include <chrono>

using namespace std::chrono;

long long int it = 0;
long long int total_duration = 0;
bool synced = false;

sf::Vector2f getCentroid(const sf::VertexArray& vertexArray) {
	sf::Vector2f centroid{};
	for (size_t i = 0; i < vertexArray.getVertexCount(); ++i)
		centroid += vertexArray.operator[](i).position;
	centroid /= static_cast<float>(vertexArray.getVertexCount());
	return centroid;
}

void demo1(sf::RenderWindow& window) {
	MovementManager* movementManager = MovementManager::getInstance();

	// Config
	bool running = true;

	const int	rows = 3;
	const int	easeTypeSize = 30;
	int			current_ease_type = 0;
	float		shape_delay_before = 0.5f;
	float		shape_delay_after = 0.5f;

	float animation_time = 3.f;

	// Start position initialization
	sf::Vector2f start_pos[rows];
	for (int i = 0; i < rows; i++)
		start_pos[i] = sf::Vector2f(500.f, static_cast<float>(window.getSize().y) / static_cast<float>(rows) * static_cast<float>(i) + window.getSize().y / static_cast<float>(rows) / 2.f);

	// End position initialization
	sf::Vector2f end_pos[rows];
	for (int i = 0; i < rows; i++)
		end_pos[i] = sf::Vector2f(static_cast<float>(window.getSize().x) - 400.f, static_cast<float>(window.getSize().y) / static_cast<float>(rows) * static_cast<float>(i) + window.getSize().y / static_cast<float>(rows) / 2.f);

	// Shapes initialiaztion
	sf::RectangleShape shapes[rows];
	for (int i = 0; i < rows; i++) {
		shapes[i].setSize(sf::Vector2f(100.f, 100.f));
		shapes[i].setFillColor(sf::Color::Blue);
		shapes[i].setOrigin(shapes[i].getSize().x / 2.f, shapes[i].getSize().y / 2.f);
		shapes[i].setPosition(start_pos[i]);
	}

	// Graph initialization
	Graph graphs[rows];
	for (int i = 0; i < rows; i++) {
		graphs[i].setPrecision(200);
		graphs[i].setSize(200.f, 200.f);
		graphs[i].setPosition(sf::Vector2f(end_pos[i].x + 150.f, end_pos[i].y + graphs[i].getSize().y / 2.f));
		graphs[i].setFunction(easeFunctions::getFunction(current_ease_type + i));
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
		text[i].setString(easeFunctions::getFunctionName(current_ease_type + i));
		text[i].setStyle(sf::Text::Bold);
		text[i].setPosition(50.f, static_cast<float>(window.getSize().y) / static_cast<float>(rows) * static_cast<float>(i) + window.getSize().y / static_cast<float>(rows) / 2.f - text[i].getGlobalBounds().height / 2.f);
	}

	// Arrows initialization
	VertexArray2 up_arrow(sf::LineStrip, 3u), down_arrow(sf::LineStrip, 3u);
	up_arrow[0].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f - 40.f, 60.f);
	up_arrow[1].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f, 30.f);
	up_arrow[2].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f + 40.f, 60.f);

	down_arrow[0].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f - 40.f, static_cast<float>(window.getSize().y) - 60.f);
	down_arrow[1].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f, static_cast<float>(window.getSize().y) - 30.f);
	down_arrow[2].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f + 40.f, static_cast<float>(window.getSize().y) - 60.f);

	float arrow_movement_time = 0.5f;
	float arrow_scaling_time = 0.5f;
	float arrow_rotation_time = 0.5f;
	float arrow_wait_time = 0.25f;

	// Movement
	auto movementRoutine = movementManager->createMovementRoutine("UP_ARROW_MOVEMENT");
	movementRoutine->addMovement(new movementInfo(sf::Vector2f(960.f, 50.f), sf::Vector2f(960.f, 30.f), arrow_movement_time, easeFunctions::IN_OUT_SINE, false, arrow_wait_time, arrow_wait_time));
	movementRoutine->addMovement(new movementInfo(sf::Vector2f(960.f, 30.f), sf::Vector2f(960.f, 50.f), arrow_movement_time, easeFunctions::IN_OUT_SINE, false, arrow_wait_time, arrow_wait_time));
	movementRoutine->setLooping(true);
	movementManager->linkMovementRoutine(&up_arrow, movementRoutine);
	movementManager->startMovementRoutine(up_arrow, "UP_ARROW_MOVEMENT");

	movementRoutine = movementManager->createMovementRoutine("DOWN_ARROW_MOVEMENT");
	movementRoutine->addMovement(new movementInfo(sf::Vector2f(960.f, 1030.f), sf::Vector2f(960.f, 1050.f), arrow_movement_time, easeFunctions::IN_OUT_SINE, false, arrow_wait_time, arrow_wait_time));
	movementRoutine->addMovement(new movementInfo(sf::Vector2f(960.f, 1050.f), sf::Vector2f(960.f, 1030.f), arrow_movement_time, easeFunctions::IN_OUT_SINE, false, arrow_wait_time, arrow_wait_time));
	movementRoutine->setLooping(true);
	movementManager->linkMovementRoutine(&down_arrow, movementRoutine);
	movementManager->startMovementRoutine(down_arrow, "DOWN_ARROW_MOVEMENT");

	// Scaling
	auto scalingRoutine = movementManager->createScalingRoutine("UP_ARROW_SCALING");
	scalingRoutine->addScaling(new scalingInfo(sf::Vector2f(1.f, 1.f), sf::Vector2f(1.2f, 1.2f), arrow_scaling_time, easeFunctions::IN_OUT_SINE, false, arrow_wait_time, arrow_wait_time));
	scalingRoutine->addScaling(new scalingInfo(sf::Vector2f(1.2f, 1.2f), sf::Vector2f(1.f, 1.f), arrow_scaling_time, easeFunctions::IN_OUT_SINE, false, arrow_wait_time, arrow_wait_time));
	scalingRoutine->setLooping(true);
	movementManager->linkScalingRoutine(&up_arrow, scalingRoutine);
	movementManager->startScalingRoutine(up_arrow, "UP_ARROW_SCALING");

	scalingRoutine = movementManager->createScalingRoutine("DOWN_ARROW_SCALING");
	scalingRoutine->addScaling(new scalingInfo(sf::Vector2f(1.f, 1.f), sf::Vector2f(1.2f, 1.2f), arrow_scaling_time, easeFunctions::IN_OUT_SINE, false, arrow_wait_time, arrow_wait_time));
	scalingRoutine->addScaling(new scalingInfo(sf::Vector2f(1.2f, 1.2f), sf::Vector2f(1.f, 1.f), arrow_scaling_time, easeFunctions::IN_OUT_SINE, false, arrow_wait_time, arrow_wait_time));
	scalingRoutine->setLooping(true);
	movementManager->linkScalingRoutine(&down_arrow, scalingRoutine);
	movementManager->startScalingRoutine(down_arrow, "DOWN_ARROW_SCALING");

	// Rotation
	auto rotationRoutine = movementManager->createRotationRoutine("UP_ARROW_ROTATION");
	rotationRoutine->addRotation(new rotationInfo(-10.f, 10.f, arrow_rotation_time, easeFunctions::IN_OUT_SINE, false, arrow_wait_time, arrow_wait_time, false));
	rotationRoutine->addRotation(new rotationInfo(10.f, -10.f, arrow_rotation_time, easeFunctions::IN_OUT_SINE, false, arrow_wait_time, arrow_wait_time, false));
	rotationRoutine->setLooping(true);
	movementManager->linkRotationRoutine(&up_arrow, rotationRoutine);
	movementManager->startRotationRoutine(up_arrow, "UP_ARROW_ROTATION");

	rotationRoutine = movementManager->createRotationRoutine("DOWN_ARROW_ROTATION");
	rotationRoutine->addRotation(new rotationInfo(10.f, -10.f, arrow_rotation_time, easeFunctions::IN_OUT_SINE, false, arrow_wait_time, arrow_wait_time, false));
	rotationRoutine->addRotation(new rotationInfo(-10.f, 10.f, arrow_rotation_time, easeFunctions::IN_OUT_SINE, false, arrow_wait_time, arrow_wait_time, false));
	rotationRoutine->setLooping(true);
	movementManager->linkRotationRoutine(&down_arrow, rotationRoutine);
	movementManager->startRotationRoutine(down_arrow, "DOWN_ARROW_ROTATION");

	sf::Clock dt_clock;
	float dt;
	float wait_time = 0.5f;

	for (int i = 0; i < rows; i++) {
		auto movementRoutine = movementManager->createMovementRoutine("SHAPE_MOVEMENT_" + std::to_string(i));
		movementRoutine->addMovement(new movementInfo(start_pos[i], end_pos[i], animation_time, easeFunctions::IN_OUT_SINE, false, shape_delay_before, shape_delay_after));
		movementRoutine->addMovement(new movementInfo(end_pos[i], start_pos[i], animation_time, easeFunctions::IN_OUT_SINE, false, shape_delay_before, shape_delay_after));
		movementRoutine->setLooping(true);
		movementManager->linkMovementRoutine(&shapes[i], movementRoutine);
		movementManager->startMovementRoutine(shapes[i], "SHAPE_MOVEMENT_" + std::to_string(i));

		auto scalingRoutine = movementManager->createScalingRoutine("SHAPE_SCALING_" + std::to_string(i));
		scalingRoutine->addScaling(new scalingInfo(sf::Vector2f(1.f, 1.f), sf::Vector2f(1.5f, 1.5f), animation_time, easeFunctions::IN_OUT_SINE, false, shape_delay_before, shape_delay_after));
		scalingRoutine->addScaling(new scalingInfo(sf::Vector2f(1.5f, 1.5f), sf::Vector2f(1.f, 1.f), animation_time, easeFunctions::IN_OUT_SINE, false, shape_delay_before, shape_delay_after));
		scalingRoutine->setLooping(true);
		movementManager->linkScalingRoutine(&shapes[i], scalingRoutine);
		movementManager->startScalingRoutine(shapes[i], "SHAPE_SCALING_" + std::to_string(i));

		auto rotationRoutine = movementManager->createRotationRoutine("SHAPE_ROTATION_" + std::to_string(i));
		rotationRoutine->addRotation(new rotationInfo(0.f, 360.f, animation_time, easeFunctions::IN_OUT_SINE, false, shape_delay_before, shape_delay_after, true));
		rotationRoutine->addRotation(new rotationInfo(360.f, 0.f, animation_time, easeFunctions::IN_OUT_SINE, false, shape_delay_before, shape_delay_after, true));
		rotationRoutine->setLooping(true);
		movementManager->linkRotationRoutine(&shapes[i], rotationRoutine);
		movementManager->startRotationRoutine(shapes[i], "SHAPE_ROTATION_" + std::to_string(i));
	}


	while (running)
	{
		dt = dt_clock.restart().asSeconds();

		auto start = high_resolution_clock::now();
		movementManager->update(dt);
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<nanoseconds>(stop - start);
		total_duration += duration.count();
		it++;

		if (it % 1000 == 0) {
			std::cout << static_cast<float>(total_duration / it) << " ns  =  " << static_cast<float>(total_duration / it) / 100000.f << " ms  =  " << static_cast<float>(total_duration / it) / 100000000.f << " s\n";
			total_duration = 0;
			it = 0;
		}

		sf::Event event;
		if (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				running = false;

			if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W)) {
				current_ease_type += rows;
				if (current_ease_type >= easeTypeSize - 2)
					current_ease_type = 0;

				for (int i = 0; i < rows; i++) {
					text[i].setString(easeFunctions::getFunctionName(current_ease_type + i));
					graphs[i].setFunction(easeFunctions::getFunction(current_ease_type + i));

					movementManager->setFunction(shapes[i], easeFunctions::getTmovement(current_ease_type + i));
					movementManager->resetRoutines(shapes[i]);
				}

			}
			else if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S)) {
				current_ease_type -= rows;
				if (current_ease_type < 0)
					current_ease_type = easeTypeSize - rows;

				for (size_t i = 0; i < rows; i++) {
					text[i].setString(easeFunctions::getFunctionName(current_ease_type + i));
					graphs[i].setFunction(easeFunctions::getFunction(current_ease_type + i));

					movementManager->setFunction(shapes[i], easeFunctions::getTmovement(current_ease_type + i));
					movementManager->resetRoutines(shapes[i]);
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

	movementManager->deleteMovementRoutine();
	movementManager->deleteScalingRoutine();
	movementManager->deleteRotationRoutine();
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
	size_t random = static_cast<size_t>(rand() % static_cast<int>(max - min) + static_cast<int>(min));
	if (random == 18 || random == 19 || random == 20)
		return randomEaseType(min, max);
	return static_cast<size_t>(rand() % static_cast<int>(max - min) + static_cast<int>(min));
}

inline const float randomDelay(const float min = 0.f, const float max = 1.f) {
	return static_cast<float>(rand() % static_cast<int>(max * 100 - min * 100) + static_cast<int>(min * 100)) / 100.f;
}

inline sf::Color randomColor() {
	return sf::Color(rand() % 235 + 20, rand() % 235 + 20, rand() % 235 + 20);
}

void demo2(sf::RenderWindow& window) {
	MovementManager* movementManager = MovementManager::getInstance();

	// Config
	bool running = true;

	const int easeTypeSize = 30;

	constexpr static int max_routines = 100000;
	constexpr static int max_movements_in_routine = 10;
	constexpr static int max_shapes = 100000;

	float animation_time = 10.f;

	// Routine initialization
	printf("Creating routines...\n");
	for (int i = 0; i < max_routines; ++i) {
		auto movementRoutine = movementManager->createMovementRoutine("SM" + std::to_string(i));
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
		shapes[i].setSize(sf::Vector2f(5.f, 5.f));
		shapes[i].setFillColor(sf::Color::Blue);
		shapes[i].setOrigin(shapes[i].getSize().x / 2.f, shapes[i].getSize().y / 2.f);
		shapes[i].setPosition(randomPosition());

		movementManager->linkMovementRoutine(&shapes[i], movementManager->getMovementRoutine("SM" + std::to_string(i % max_routines)));
		movementManager->linkScalingRoutine(&shapes[i], movementManager->getScalingRoutine("SC" + std::to_string(i % max_routines)));
		movementManager->linkRotationRoutine(&shapes[i], movementManager->getRotationRoutine("SR" + std::to_string(i % max_routines)));

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

		auto start = high_resolution_clock::now();
		movementManager->update(dt);
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<nanoseconds>(stop - start);
		total_duration += duration.count();
		it++;

		if (it == 1000 && !synced) {
			synced = true;
			it = 0;
			total_duration = 0;
		}

		if (it % 1000 == 0 && it != 0) {
			std::cout << static_cast<float>(total_duration / it) << " ns  =  " << static_cast<float>(total_duration / it) / 100000.f << " ms  =  " << static_cast<float>(total_duration / it) / 100000000.f << " s\n";
			//total_duration = 0;
			//it = 0;
		}

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

	constexpr static int max_routines = 20000;
	constexpr static int max_movements_in_routine = 10;
	constexpr static int max_shapes = 20000;

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
		shapes[i].setPrimitiveType(sf::Quads);
		shapes[i].resize(4);
		shapes[i][0].position = sf::Vector2f(0.f, 0.f);
		shapes[i][1].position = shapes[i][0].position + sf::Vector2f(5.f, 0.f);
		shapes[i][2].position = shapes[i][0].position + sf::Vector2f(5.f, 5.f);
		shapes[i][3].position = shapes[i][0].position + sf::Vector2f(0.f, 5.f);

		shapes[i][0].color = randomColor();
		shapes[i][1].color = randomColor();
		shapes[i][2].color = randomColor();
		shapes[i][3].color = randomColor();

		shapes[i].setOrigin(shapes[i].getCentroid());
		shapes[i].setPosition(randomPosition());

		movementManager->linkMovementRoutine(&shapes[i], movementManager->getMovementRoutine("SM" + std::to_string(i % max_routines)));
		movementManager->linkScalingRoutine(&shapes[i], movementManager->getScalingRoutine("SC" + std::to_string(i % max_routines)));
		movementManager->linkRotationRoutine(&shapes[i], movementManager->getRotationRoutine("SR" + std::to_string(i % max_routines)));

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

		auto start = high_resolution_clock::now();
		movementManager->update(dt);
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<nanoseconds>(stop - start);
		total_duration += duration.count();
		it++;

		if (it == 1000 && !synced) {
			synced = true;
			it = 0;
			total_duration = 0;
		}

		if (it % 1000 == 0 && it != 0) {
			std::cout << static_cast<float>(total_duration / it) << " ns  =  " << static_cast<float>(total_duration / it) / 100000.f << " ms  =  " << static_cast<float>(total_duration / it) / 100000000.f << " s\n";
			//total_duration = 0;
			//it = 0;
		}

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

	demo3(window);

	MovementContainer MovementContainer;

	// Configuration
	constexpr size_t easeTypeSize = 30;
	const int rows = 3;
	//float animation_time = 3.f;
	int current_ease_type = 0;
	float wait_time_max = 0.5f;

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
		shapes[i].setSize(sf::Vector2f(100.f, 100.f));
		shapes[i].setFillColor(sf::Color::Blue);
		shapes[i].setOrigin(shapes[i].getSize().x / 2.f, shapes[i].getSize().y / 2.f);
		shapes[i].setPosition(start_pos[i]);
	}

	// Graph initialization
	Graph graphs[rows];
	for (int i = 0; i < rows; i++) {
		graphs[i].setPrecision(200);
		graphs[i].setSize(200.f, 200.f);
		graphs[i].setPosition(sf::Vector2f(end_pos[i].x + 150.f, end_pos[i].y + graphs[i].getSize().y / 2.f));
		graphs[i].setFunction(easeFunctions::getFunction(current_ease_type + i));
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
		text[i].setString(easeFunctions::getFunctionName(current_ease_type + i));
		text[i].setStyle(sf::Text::Bold);
		text[i].setPosition(50.f, static_cast<float>(window.getSize().y) / static_cast<float>(rows) * static_cast<float>(i) + window.getSize().y / static_cast<float>(rows) / 2.f - text[i].getGlobalBounds().height / 2.f);
	}

	// Arrows initialization
	sf::VertexArray up_arrow(sf::LineStrip, 3u), down_arrow(sf::LineStrip, 3u);
	up_arrow[0].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f - 40.f, 500.f);
	up_arrow[1].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f, 470.f);
	up_arrow[2].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f + 40.f, 500.f);

	down_arrow[0].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f - 40.f, static_cast<float>(window.getSize().y) - 60.f);
	down_arrow[1].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f, static_cast<float>(window.getSize().y) - 30.f);
	down_arrow[2].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f + 40.f, static_cast<float>(window.getSize().y) - 60.f);

	float arrow_movement_time = 1.f;
	float arrow_scaling_time = 1.f;
	float arrow_rotation_time = 1.f;
	float arrow_delay_before = 1.f, arrow_delay_after = 1.f;

	sf::Clock dt_clock;
	float dt;
	float wait_time = 0.5f;

	/*for (int i = 0; i < rows; i++) {
		MovementContainer.addMovement(&shapes[i], start_pos[i], end_pos[i], animation_time, easeType[current_ease_type + i].second, true, 0.5f, 0.5f);
		MovementContainer.addScaling(&shapes[i], {1.5f, 1.5f}, animation_time, easeType[current_ease_type + i].second, true, 0.5f, 0.5f);
		MovementContainer.addRotation(&shapes[i], 360.f, animation_time, easeType[current_ease_type + i].second, false, true, 0.5f, 0.5f);
	}*/

	// ----- Movement Container testing ----- //
	sf::CircleShape test_shape[4];

	for (int i = 0; i < 4; i++) {
		test_shape[i].setPointCount(3);
		test_shape[i].setRadius(50.f);
		test_shape[i].setFillColor(sf::Color::Blue);
		test_shape[i].setOrigin(test_shape[i].getRadius(), test_shape[i].getRadius());
		test_shape[i].setPosition(800.f, 100.f + 200.f * i);
	}

	VertexArray2 arrow1(sf::LineStrip, 3u);
	VertexArray2 arrow2(sf::LineStrip, 3u);
	VertexArray2 arrow3(sf::LineStrip, 3u);
	VertexArray2 arrow4(sf::LineStrip, 3u);

	arrow1[0].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f - 40.f, 100.f);
	arrow1[1].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f, 70.f);
	arrow1[2].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f + 40.f, 100.f);

	arrow2[0].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f - 40.f, 300.f);
	arrow2[1].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f, 270.f);
	arrow2[2].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f + 40.f, 300.f);

	arrow3[0].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f - 40.f, 500.f);
	arrow3[1].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f, 470.f);
	arrow3[2].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f + 40.f, 500.f);

	arrow4[0].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f - 40.f, 700.f);
	arrow4[1].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f, 670.f);
	arrow4[2].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f + 40.f, 700.f);

	MovementManager* movementManager = MovementManager::getInstance();
	float animation_time = 0.5f;

	bool adjust_start_rotation = false;
	bool adjust_all_rotation = true;
	bool looping_rotation = false;
	auto routineRShape = movementManager->createRotationRoutine("TestowyR1");
	routineRShape->addRotation(new rotationInfo(30.f, 180.f, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f, false));
	routineRShape->addRotation(new rotationInfo(180.f, 60.f, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f, false));
	routineRShape->adjustStartToCurrentTransform(adjust_start_rotation);
	routineRShape->adjustAllToCurrentTransform(adjust_all_rotation);
	routineRShape->setLooping(looping_rotation);
	movementManager->linkRotationRoutine(test_shape[0], "TestowyR1");

	routineRShape = movementManager->createRotationRoutine("TestowyR2");
	routineRShape->addRotation(new rotationInfo(30.f, 180.f, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f, false));
	routineRShape->addRotation(new rotationInfo(180.f, 60.f, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f, true));
	routineRShape->adjustStartToCurrentTransform(adjust_start_rotation);
	routineRShape->adjustAllToCurrentTransform(adjust_all_rotation);
	routineRShape->setLooping(looping_rotation);
	movementManager->linkRotationRoutine(test_shape[1], "TestowyR2");

	routineRShape = movementManager->createRotationRoutine("TestowyR3");
	routineRShape->addRotation(new rotationInfo(30.f, 180.f, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f, true));
	routineRShape->addRotation(new rotationInfo(180.f, 60.f, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f, false));
	routineRShape->adjustStartToCurrentTransform(adjust_start_rotation);
	routineRShape->adjustAllToCurrentTransform(adjust_all_rotation);
	routineRShape->setLooping(looping_rotation);
	movementManager->linkRotationRoutine(test_shape[2], "TestowyR3");

	routineRShape = movementManager->createRotationRoutine("TestowyR4");
	routineRShape->addRotation(new rotationInfo(30.f, 180.f, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f, true));
	routineRShape->addRotation(new rotationInfo(180.f, 60.f, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f, true));
	routineRShape->adjustStartToCurrentTransform(adjust_start_rotation);
	routineRShape->adjustAllToCurrentTransform(adjust_all_rotation);
	routineRShape->setLooping(looping_rotation);
	movementManager->linkRotationRoutine(test_shape[3], "TestowyR4");

	auto routineRVA = movementManager->createRotationRoutine("TestowyRVA1");
	routineRVA->addRotation(new rotationInfo(30.f, 180.f, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f, false));
	routineRVA->addRotation(new rotationInfo(180.f, 60.f, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f, false));
	routineRVA->adjustStartToCurrentTransform(adjust_start_rotation);
	routineRVA->adjustAllToCurrentTransform(adjust_all_rotation);
	routineRVA->setLooping(looping_rotation);
	movementManager->linkRotationRoutine(arrow1, "TestowyRVA1");

	routineRVA = movementManager->createRotationRoutine("TestowyRVA2");
	routineRVA->addRotation(new rotationInfo(30.f, 180.f, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f, false));
	routineRVA->addRotation(new rotationInfo(180.f, 60.f, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f, true));
	routineRVA->adjustStartToCurrentTransform(adjust_start_rotation);
	routineRVA->adjustAllToCurrentTransform(adjust_all_rotation);
	routineRVA->setLooping(looping_rotation);
	movementManager->linkRotationRoutine(arrow2, "TestowyRVA2");

	routineRVA = movementManager->createRotationRoutine("TestowyRVA3");
	routineRVA->addRotation(new rotationInfo(30.f, 180.f, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f, true));
	routineRVA->addRotation(new rotationInfo(180.f, 60.f, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f, false));
	routineRVA->adjustStartToCurrentTransform(adjust_start_rotation);
	routineRVA->adjustAllToCurrentTransform(adjust_all_rotation);
	routineRVA->setLooping(looping_rotation);
	movementManager->linkRotationRoutine(arrow3, "TestowyRVA3");

	routineRVA = movementManager->createRotationRoutine("TestowyRVA4");
	routineRVA->addRotation(new rotationInfo(30.f, 180.f, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f, true));
	routineRVA->addRotation(new rotationInfo(180.f, 60.f, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f, true));
	routineRVA->adjustStartToCurrentTransform(adjust_start_rotation);
	routineRVA->adjustAllToCurrentTransform(adjust_all_rotation);
	routineRVA->setLooping(looping_rotation);
	movementManager->linkRotationRoutine(arrow4, "TestowyRVA4");

	sf::Vector2f starting_offset1(0.f, 0.f);
	sf::Vector2f ending_offset1 = sf::Vector2f(300.f, -100.f) + starting_offset1;
	sf::Vector2f starting_offset2 = sf::Vector2f(0.f, 0.f) + ending_offset1;
	sf::Vector2f ending_offset2 = sf::Vector2f(-300.f, 100.f) + starting_offset2;

	
	bool adjust_start_movement = false;
	bool adjust_all_movement = true;
	bool looping_movement = false;
	auto routineMShape = movementManager->createMovementRoutine("TestowyM1");
	routineMShape->addMovement(new movementInfo(test_shape[0].getPosition() + starting_offset1, test_shape[0].getPosition() + ending_offset1, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineMShape->addMovement(new movementInfo(test_shape[0].getPosition() + starting_offset2, test_shape[0].getPosition() + ending_offset2, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineMShape->adjustStartToCurrentTransform(adjust_start_movement);
	routineMShape->adjustAllToCurrentTransform(adjust_all_movement);
	routineMShape->setLooping(looping_movement);
	movementManager->linkMovementRoutine(test_shape[0], "TestowyM1");

	routineMShape = movementManager->createMovementRoutine("TestowyM2");
	routineMShape->addMovement(new movementInfo(test_shape[1].getPosition() + starting_offset1, test_shape[1].getPosition() + ending_offset1, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineMShape->addMovement(new movementInfo(test_shape[1].getPosition() + starting_offset2, test_shape[1].getPosition() + ending_offset2, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineMShape->adjustStartToCurrentTransform(adjust_start_movement);
	routineMShape->adjustAllToCurrentTransform(adjust_all_movement);
	routineMShape->setLooping(looping_movement);
	movementManager->linkMovementRoutine(test_shape[1], "TestowyM2");

	routineMShape = movementManager->createMovementRoutine("TestowyM3");
	routineMShape->addMovement(new movementInfo(test_shape[2].getPosition() + starting_offset1, test_shape[2].getPosition() + ending_offset1, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineMShape->addMovement(new movementInfo(test_shape[2].getPosition() + starting_offset2, test_shape[2].getPosition() + ending_offset2, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineMShape->adjustStartToCurrentTransform(adjust_start_movement);
	routineMShape->adjustAllToCurrentTransform(adjust_all_movement);
	routineMShape->setLooping(looping_movement);
	movementManager->linkMovementRoutine(test_shape[2], "TestowyM3");

	routineMShape = movementManager->createMovementRoutine("TestowyM4");
	routineMShape->addMovement(new movementInfo(test_shape[3].getPosition() + starting_offset1, test_shape[3].getPosition() + ending_offset1, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineMShape->addMovement(new movementInfo(test_shape[3].getPosition() +  starting_offset2, test_shape[3].getPosition() + ending_offset2, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineMShape->adjustStartToCurrentTransform(adjust_start_movement);
	routineMShape->adjustAllToCurrentTransform(adjust_all_movement);
	routineMShape->setLooping(looping_movement);
	movementManager->linkMovementRoutine(test_shape[3], "TestowyM4");

	auto routineMVA = movementManager->createMovementRoutine("TestowyMVA1");
	routineMVA->addMovement(new movementInfo(getCentroid(arrow1) + starting_offset1, getCentroid(arrow1) + ending_offset1, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineMVA->addMovement(new movementInfo(getCentroid(arrow1) + starting_offset2, getCentroid(arrow1) + ending_offset2, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineMVA->adjustStartToCurrentTransform(adjust_start_movement);
	routineMVA->adjustAllToCurrentTransform(adjust_all_movement);
	routineMVA->setLooping(looping_movement);
	movementManager->linkMovementRoutine(arrow1, "TestowyMVA1");

	routineMVA = movementManager->createMovementRoutine("TestowyMVA2");
	routineMVA->addMovement(new movementInfo(getCentroid(arrow2) + starting_offset1, getCentroid(arrow2) + ending_offset1, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineMVA->addMovement(new movementInfo(getCentroid(arrow2) + starting_offset2, getCentroid(arrow2) + ending_offset2, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineMVA->adjustStartToCurrentTransform(adjust_start_movement);
	routineMVA->adjustAllToCurrentTransform(adjust_all_movement);
	routineMVA->setLooping(looping_movement);
	movementManager->linkMovementRoutine(arrow2, "TestowyMVA2");

	routineMVA = movementManager->createMovementRoutine("TestowyMVA3");
	routineMVA->addMovement(new movementInfo(getCentroid(arrow3) + starting_offset1, getCentroid(arrow3) + ending_offset1, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineMVA->addMovement(new movementInfo(getCentroid(arrow3) + starting_offset2, getCentroid(arrow3) + ending_offset2, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineMVA->adjustStartToCurrentTransform(adjust_start_movement);
	routineMVA->adjustAllToCurrentTransform(adjust_all_movement);
	routineMVA->setLooping(looping_movement);
	movementManager->linkMovementRoutine(arrow3, "TestowyMVA3");

	routineMVA = movementManager->createMovementRoutine("TestowyMVA4");
	routineMVA->addMovement(new movementInfo(getCentroid(arrow4) + starting_offset1, getCentroid(arrow4) + ending_offset1, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineMVA->addMovement(new movementInfo(getCentroid(arrow4) + starting_offset2, getCentroid(arrow4) + ending_offset2, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineMVA->adjustStartToCurrentTransform(adjust_start_movement);
	routineMVA->adjustAllToCurrentTransform(adjust_all_movement);
	routineMVA->setLooping(looping_movement);
	movementManager->linkMovementRoutine(arrow4, "TestowyMVA4");

	bool adjust_start_scaling = false;
	bool adjust_all_scaling = true;
	bool looping_scaling = false;
	sf::Vector2f starting_scale1(1.f, 1.f);
	sf::Vector2f ending_scale1(1.5f, 1.5f);
	sf::Vector2f starting_scale2(1.5f, 1.5f);
	sf::Vector2f ending_scale2(1.25f, 1.25f);
	auto routineSShape = movementManager->createScalingRoutine("TestowyS1");
	routineSShape->addScaling(new scalingInfo(starting_scale1, ending_scale1, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineSShape->addScaling(new scalingInfo(starting_scale2, ending_scale2, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineSShape->adjustStartToCurrentTransform(adjust_start_scaling);
	routineSShape->adjustAllToCurrentTransform(adjust_all_scaling);
	routineSShape->setLooping(looping_scaling);
	movementManager->linkScalingRoutine(test_shape[0], "TestowyS1");

	routineSShape = movementManager->createScalingRoutine("TestowyS2");
	routineSShape->addScaling(new scalingInfo(starting_scale1, ending_scale1, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineSShape->addScaling(new scalingInfo(starting_scale2, ending_scale2, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineSShape->adjustStartToCurrentTransform(adjust_start_scaling);
	routineSShape->adjustAllToCurrentTransform(adjust_all_scaling);
	routineSShape->setLooping(looping_scaling);
	movementManager->linkScalingRoutine(test_shape[1], "TestowyS2");

	routineSShape = movementManager->createScalingRoutine("TestowyS3");
	routineSShape->addScaling(new scalingInfo(starting_scale1, ending_scale1, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineSShape->addScaling(new scalingInfo(starting_scale2, ending_scale2, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineSShape->adjustStartToCurrentTransform(adjust_start_scaling);
	routineSShape->adjustAllToCurrentTransform(adjust_all_scaling);
	routineSShape->setLooping(looping_scaling);
	movementManager->linkScalingRoutine(test_shape[2], "TestowyS3");

	routineSShape = movementManager->createScalingRoutine("TestowyS4");
	routineSShape->addScaling(new scalingInfo(starting_scale1, ending_scale1, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineSShape->addScaling(new scalingInfo(starting_scale2, ending_scale2, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineSShape->adjustStartToCurrentTransform(adjust_start_scaling);
	routineSShape->adjustAllToCurrentTransform(adjust_all_scaling);
	routineSShape->setLooping(looping_scaling);
	movementManager->linkScalingRoutine(test_shape[3], "TestowyS4");

	auto routineSVA = movementManager->createScalingRoutine("TestowySVA1");
	routineSVA->addScaling(new scalingInfo(starting_scale1, ending_scale1, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineSVA->addScaling(new scalingInfo(starting_scale2, ending_scale2, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineSVA->adjustStartToCurrentTransform(adjust_start_scaling);
	routineSVA->adjustAllToCurrentTransform(adjust_all_scaling);
	routineSVA->setLooping(looping_scaling);
	movementManager->linkScalingRoutine(arrow1, "TestowySVA1");

	routineSVA = movementManager->createScalingRoutine("TestowySVA2");
	routineSVA->addScaling(new scalingInfo(starting_scale1, ending_scale1, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineSVA->addScaling(new scalingInfo(starting_scale2, ending_scale2, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineSVA->adjustStartToCurrentTransform(adjust_start_scaling);
	routineSVA->adjustAllToCurrentTransform(adjust_all_scaling);
	routineSVA->setLooping(looping_scaling);
	movementManager->linkScalingRoutine(arrow2, "TestowySVA2");

	routineSVA = movementManager->createScalingRoutine("TestowySVA3");
	routineSVA->addScaling(new scalingInfo(starting_scale1, ending_scale1, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineSVA->addScaling(new scalingInfo(starting_scale2, ending_scale2, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineSVA->adjustStartToCurrentTransform(adjust_start_scaling);
	routineSVA->adjustAllToCurrentTransform(adjust_all_scaling);
	routineSVA->setLooping(looping_scaling);
	movementManager->linkScalingRoutine(arrow3, "TestowySVA3");

	routineSVA = movementManager->createScalingRoutine("TestowySVA4");
	routineSVA->addScaling(new scalingInfo(starting_scale1, ending_scale1, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineSVA->addScaling(new scalingInfo(starting_scale2, ending_scale2, animation_time, easeFunctions::IN_OUT_SINE, false, 0.5f, 0.5f));
	routineSVA->adjustStartToCurrentTransform(adjust_start_scaling);
	routineSVA->adjustAllToCurrentTransform(adjust_all_scaling);
	routineSVA->setLooping(looping_scaling);
	movementManager->linkScalingRoutine(arrow4, "TestowySVA4");



	constexpr int test_shape_size = 10000;
	constexpr int test_routine_size = 10000;
	constexpr int max_movements_in_routine = 10;

	printf("Size before:\n");
	printf("Movement size: %lld\n", movementManager->getSizeMovement());
	printf("Scaling size: %lld\n", movementManager->getSizeScaling());
	printf("Rotation size: %lld\n", movementManager->getSizeRotation());



	//sf::RectangleShape *test_shape = new sf::RectangleShape[test_shape_size];

	/*for (int i = 0; i < test_shape_size; i++) {
		test_shape[i].setSize(sf::Vector2f(5.f, 5.f));
		test_shape[i].setFillColor(sf::Color::Blue);
		test_shape[i].setPosition(100.f, 100.f);
	}

	for (size_t i = 0; i < test_routine_size; ++i) {
		auto* routine = movementManager->createMovementRoutine("T" + std::to_string(i));
		int count = 10;

		sf::Vector2f previous = sf::Vector2f(100.f, 100.f);
		for (size_t j = 0; j < count; ++j) {
			sf::Vector2f end = sf::Vector2f(static_cast<float>(rand() % 1920), static_cast<float>(rand() % 1080));
			routine->addMovement(new movementInfo(previous, end, 2.f, MovementContainer.getFunctionPointer(MovementContainer::IN_OUT_SINE), false, 0.2f, 0.2f));
			previous = end;
		}
			

		routine->adjustAllToCurrentTransform(true);
		routine->setLooping(true);

	}

	for (size_t i = 0; i < test_shape_size; ++i)
		movementManager->linkMovementRoutine(&test_shape[i], "T" + std::to_string(i));

	for (size_t i = 0; i < test_shape_size; ++i)
		movementManager->startMovementRoutine(&test_shape[i], "T" + std::to_string(i));

	printf("Size after:\n");
	printf("Active: %lld\n", movementManager->getSizeShapeActive());
	printf("Shape: %lld\n", movementManager->getSizeShape());
	printf("Container: %lld\n", movementManager->getSizeContainer());*/

	//movementManager->printAllRoutines();

	while (window.isOpen())
	{
		dt = dt_clock.restart().asSeconds();
		
		//MovementContainer.update(dt);
		movementManager->update(dt);

		sf::Event event;
		if (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				window.close();

			// Start all
			if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Space)) {
				movementManager->startMovementRoutine(test_shape[0], "TestowyM1");
				movementManager->startMovementRoutine(test_shape[1], "TestowyM2");
				movementManager->startMovementRoutine(test_shape[2], "TestowyM3");
				movementManager->startMovementRoutine(test_shape[3], "TestowyM4");

				movementManager->startMovementRoutine(arrow1, "TestowyMVA1");
				movementManager->startMovementRoutine(arrow2, "TestowyMVA2");
				movementManager->startMovementRoutine(arrow3, "TestowyMVA3");
				movementManager->startMovementRoutine(arrow4, "TestowyMVA4");

				movementManager->startScalingRoutine(test_shape[0], "TestowyS1");
				movementManager->startScalingRoutine(test_shape[1], "TestowyS2");
				movementManager->startScalingRoutine(test_shape[2], "TestowyS3");
				movementManager->startScalingRoutine(test_shape[3], "TestowyS4");

				movementManager->startScalingRoutine(arrow1, "TestowySVA1");
				movementManager->startScalingRoutine(arrow2, "TestowySVA2");
				movementManager->startScalingRoutine(arrow3, "TestowySVA3");
				movementManager->startScalingRoutine(arrow4, "TestowySVA4");

				movementManager->startRotationRoutine(arrow1, "TestowyRVA1");
				movementManager->startRotationRoutine(arrow2, "TestowyRVA2");
				movementManager->startRotationRoutine(arrow3, "TestowyRVA3");
				movementManager->startRotationRoutine(arrow4, "TestowyRVA4");

				movementManager->startRotationRoutine(test_shape[0], "TestowyR1");
				movementManager->startRotationRoutine(test_shape[1], "TestowyR2");
				movementManager->startRotationRoutine(test_shape[2], "TestowyR3");
				movementManager->startRotationRoutine(test_shape[3], "TestowyR4");
			}

			// Start rotation
			if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::R)) {
				movementManager->startRotationRoutine(arrow1, "TestowyRVA1");
				movementManager->startRotationRoutine(arrow2, "TestowyRVA2");
				movementManager->startRotationRoutine(arrow3, "TestowyRVA3");
				movementManager->startRotationRoutine(arrow4, "TestowyRVA4");

				movementManager->startRotationRoutine(test_shape[0], "TestowyR1");
				movementManager->startRotationRoutine(test_shape[1], "TestowyR2");
				movementManager->startRotationRoutine(test_shape[2], "TestowyR3");
				movementManager->startRotationRoutine(test_shape[3], "TestowyR4");
			}

			// Start movement
			if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::M)) {
				movementManager->startMovementRoutine(test_shape[0], "TestowyM1");
				movementManager->startMovementRoutine(test_shape[1], "TestowyM2");
				movementManager->startMovementRoutine(test_shape[2], "TestowyM3");
				movementManager->startMovementRoutine(test_shape[3], "TestowyM4");

				movementManager->startMovementRoutine(arrow1, "TestowyMVA1");
				movementManager->startMovementRoutine(arrow2, "TestowyMVA2");
				movementManager->startMovementRoutine(arrow3, "TestowyMVA3");
				movementManager->startMovementRoutine(arrow4, "TestowyMVA4");
			}

			// Start scaling
			if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::S)) {
				movementManager->startScalingRoutine(test_shape[0], "TestowyS1");
				movementManager->startScalingRoutine(test_shape[1], "TestowyS2");
				movementManager->startScalingRoutine(test_shape[2], "TestowyS3");
				movementManager->startScalingRoutine(test_shape[3], "TestowyS4");

				movementManager->startScalingRoutine(arrow1, "TestowySVA1");
				movementManager->startScalingRoutine(arrow2, "TestowySVA2");
				movementManager->startScalingRoutine(arrow3, "TestowySVA3");
				movementManager->startScalingRoutine(arrow4, "TestowySVA4");
			}



			if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Up || event.key.code ==sf::Keyboard::W)) {
				current_ease_type += rows;
				if (current_ease_type >= easeTypeSize - 2)
					current_ease_type = 0;

				for (int i = 0; i < rows; i++) {
					text[i].setString(easeFunctions::getFunctionName(current_ease_type + i));
					graphs[i].setFunction(easeFunctions::getFunction(current_ease_type + i));
				}

				/*for (int i = 0; i < rows; i++) {
					MovementContainer.undoMovement(&shapes[i]);
					MovementContainer.undoScaling(&shapes[i]);
					MovementContainer.undoRotation(&shapes[i]);
					MovementContainer.addMovement(&shapes[i], start_pos[i], end_pos[i], animation_time, easeType[current_ease_type + i].second, true, 0.5f, 0.5f);
					MovementContainer.addScaling(&shapes[i], { 1.5f, 1.5f }, animation_time, easeType[current_ease_type + i].second, true, 0.5f, 0.5f);
					MovementContainer.addRotation(&shapes[i], 360.f, animation_time, easeType[current_ease_type + i].second, false, true, 0.5f, 0.5f);
				}*/

				/*MovementContainer.resetMovement(&up_arrow);
				MovementContainer.resetMovement(&down_arrow);
				MovementContainer.stopScaling(&up_arrow);
				MovementContainer.stopScaling(&down_arrow);
				MovementContainer.undoRotation(&up_arrow);
				MovementContainer.undoRotation(&down_arrow);*/

			}
			else if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S)) {
				current_ease_type -= rows;
				if (current_ease_type < 0)
					current_ease_type = easeTypeSize - rows;

				for (int i = 0; i < rows; i++) {
					text[i].setString(easeFunctions::getFunctionName(current_ease_type + i));
					graphs[i].setFunction(easeFunctions::getFunction(current_ease_type + i));
				}

				/*for (int i = 0; i < rows; i++) {
					MovementContainer.undoMovement(&shapes[i]);
					MovementContainer.undoScaling(&shapes[i]);
					MovementContainer.undoRotation(&shapes[i]);
					MovementContainer.addMovement(&shapes[i], start_pos[i], end_pos[i], animation_time, easeType[current_ease_type + i].second, 0.5f, 0.5f);
					MovementContainer.addScaling(&shapes[i], { 1.5f, 1.5f }, animation_time, easeType[current_ease_type + i].second, 0.5f, 0.5f);
					MovementContainer.addRotation(&shapes[i], 360.f, animation_time, easeType[current_ease_type + i].second, false, 0.5f, 0.5f);
				}*/

				/*MovementContainer.resetMovement(&up_arrow);
				MovementContainer.resetMovement(&down_arrow);
				MovementContainer.stopScaling(&up_arrow);
				MovementContainer.stopScaling(&down_arrow);
				MovementContainer.undoRotation(&up_arrow);
				MovementContainer.undoRotation(&down_arrow);*/
			}
		}

		window.clear();
		for (int i = 0; i < rows; i++) {
			//window.draw(text[i]);
			//window.draw(shapes[i]);

			//graphs[i].draw(window);

			//window.draw(up_arrow);
			//window.draw(down_arrow);
		}
		for (int i = 0; i < 4; i++)
			window.draw(test_shape[i]);

		window.draw(arrow1);
		window.draw(arrow2);
		window.draw(arrow3);
		window.draw(arrow4);

		//window.draw(test_shape);
		window.display();
	}

	//delete[] test_shape;

	return 0;
}