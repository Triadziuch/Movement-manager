#include <iostream>
#include "MovementManager.h"
#include "SFML\Graphics.hpp"
#include "Graph.h"
#include <Windows.h>
#include <random>

int iterator = 0;

int main()
{
	srand(time(NULL));
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Ease functions", sf::Style::Fullscreen, settings);
	window.setVerticalSyncEnabled(true);

	

	MovementContainer MovementContainer;

	// Configuration
	constexpr size_t easeTypeSize = 30;
	const int rows = 3;
	//float animation_time = 3.f;
	int current_ease_type = 0;
	float wait_time_max = 0.5f;

	std::pair<std::string, MovementContainer::movement_type> easeType[easeTypeSize] = {
		{"IN_SINE", MovementContainer::IN_SINE},
		{"OUT_SINE", MovementContainer::OUT_SINE},
		{"IN_OUT_SINE", MovementContainer::IN_OUT_SINE},
		{"IN_QUAD", MovementContainer::IN_QUAD},
		{"OUT_QUAD", MovementContainer::OUT_QUAD},
		{"IN_OUT_QUAD", MovementContainer::IN_OUT_QUAD},
		{"IN_CUBIC", MovementContainer::IN_CUBIC},
		{"OUT_CUBIC", MovementContainer::OUT_CUBIC},
		{"IN_OUT_CUBIC", MovementContainer::IN_OUT_CUBIC},
		{"IN_QUART", MovementContainer::IN_QUART},
		{"OUT_QUART", MovementContainer::OUT_QUART},
		{"IN_OUT_QUART", MovementContainer::IN_OUT_QUART},
		{"IN_QUINT", MovementContainer::IN_QUINT},
		{"OUT_QUINT", MovementContainer::OUT_QUINT},
		{"IN_OUT_QUINT", MovementContainer::IN_OUT_QUINT},
		{"IN_EXPO", MovementContainer::IN_EXPO},
		{"OUT_EXPO", MovementContainer::OUT_EXPO},
		{"IN_OUT_EXPO", MovementContainer::IN_OUT_EXPO},
		{"IN_CIRC", MovementContainer::IN_CIRC},
		{"OUT_CIRC", MovementContainer::OUT_CIRC},
		{"IN_OUT_CIRC", MovementContainer::IN_OUT_CIRC},
		{"IN_BACK", MovementContainer::IN_BACK},
		{"OUT_BACK", MovementContainer::OUT_BACK},
		{"IN_OUT_BACK", MovementContainer::IN_OUT_BACK},
		{"IN_ELASTIC", MovementContainer::IN_ELASTIC},
		{"OUT_ELASTIC", MovementContainer::OUT_ELASTIC},
		{"IN_OUT_ELASTIC", MovementContainer::IN_OUT_ELASTIC},
		{"IN_BOUNCE", MovementContainer::IN_BOUNCE},
		{"OUT_BOUNCE", MovementContainer::OUT_BOUNCE},
		{"IN_OUT_BOUNCE", MovementContainer::IN_OUT_BOUNCE}
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
		graphs[i].setFunction(MovementContainer.getFunctionPointer(easeType[current_ease_type + i].second));
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
	/*up_arrow[0].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f - 40.f, 60.f);
	up_arrow[1].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f, 30.f);
	up_arrow[2].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f + 40.f, 60.f);*/

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

	// TODO: Dodaæ lepsze zarz¹dzanie pocz¹tkiem i koñcem animacji
	//MovementContainer.addMovement(&up_arrow, sf::Vector2f(960.f, 50.f), sf::Vector2f(960.f, 30.f), arrow_movement_time, MovementContainer::IN_OUT_SINE, true, arrow_delay_before, arrow_delay_after);
	//MovementContainer.addMovement(&down_arrow, sf::Vector2f(960.f, 1030.f), sf::Vector2f(960.f, 1050.f), arrow_movement_time, MovementContainer::IN_OUT_SINE, true, arrow_delay_before, arrow_delay_after);
	/*MovementContainer.addScaling(&up_arrow, { 1.1f, 1.1f }, arrow_scaling_time, MovementContainer::IN_OUT_SINE, true, arrow_delay_before, arrow_delay_after);
	MovementContainer.addScaling(&down_arrow, { 1.5f, 1.5f }, arrow_scaling_time, MovementContainer::IN_OUT_SINE, true, arrow_delay_before, arrow_delay_after);
	MovementContainer.addRotation(&up_arrow, -10.f, 10.f, arrow_rotation_time, MovementContainer::IN_OUT_SINE, true, true, arrow_delay_before, arrow_delay_after);
	MovementContainer.addRotation(&down_arrow, -10.f, 10.f, arrow_rotation_time, MovementContainer::IN_OUT_SINE, false, true, arrow_delay_before, arrow_delay_after);*/

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

	sf::VertexArray arrow1(sf::LineStrip, 3u);
	sf::VertexArray arrow2(sf::LineStrip, 3u);
	sf::VertexArray arrow3(sf::LineStrip, 3u);
	sf::VertexArray arrow4(sf::LineStrip, 3u);

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

	MovementManager movementManager;
	float animation_time = 0.5f;
	auto routineRShape = movementManager.createRotationRoutine("TestowyR1");
	routineRShape->addRotation(new rotationInfo(30.f, 180.f, animation_time, MovementContainer.getFunctionPointer(MovementContainer::IN_OUT_SINE), false, 0.5f, 0.5f, false));
	routineRShape->addRotation(new rotationInfo(180.f, 60.f, animation_time, MovementContainer.getFunctionPointer(MovementContainer::IN_OUT_SINE), false, 0.5f, 0.5f, false));
	routineRShape->adjustStartToCurrentTransform(true);
	movementManager.linkRotationRoutine(&test_shape[0], "TestowyR1");

	routineRShape = movementManager.createRotationRoutine("TestowyR2");
	routineRShape->addRotation(new rotationInfo(30.f, 180.f, animation_time, MovementContainer.getFunctionPointer(MovementContainer::IN_OUT_SINE), false, 0.5f, 0.5f, false));
	routineRShape->addRotation(new rotationInfo(180.f, 60.f, animation_time, MovementContainer.getFunctionPointer(MovementContainer::IN_OUT_SINE), false, 0.5f, 0.5f, true));
	routineRShape->adjustStartToCurrentTransform(true);
	movementManager.linkRotationRoutine(&test_shape[1], "TestowyR2");

	routineRShape = movementManager.createRotationRoutine("TestowyR3");
	routineRShape->addRotation(new rotationInfo(30.f, 180.f, animation_time, MovementContainer.getFunctionPointer(MovementContainer::IN_OUT_SINE), false, 0.5f, 0.5f, true));
	routineRShape->addRotation(new rotationInfo(180.f, 60.f, animation_time, MovementContainer.getFunctionPointer(MovementContainer::IN_OUT_SINE), false, 0.5f, 0.5f, false));
	routineRShape->adjustStartToCurrentTransform(true);
	movementManager.linkRotationRoutine(&test_shape[2], "TestowyR3");

	routineRShape = movementManager.createRotationRoutine("TestowyR4");
	routineRShape->addRotation(new rotationInfo(30.f, 180.f, animation_time, MovementContainer.getFunctionPointer(MovementContainer::IN_OUT_SINE), false, 0.5f, 0.5f, true));
	routineRShape->addRotation(new rotationInfo(180.f, 60.f, animation_time, MovementContainer.getFunctionPointer(MovementContainer::IN_OUT_SINE), false, 0.5f, 0.5f, true));
	routineRShape->adjustStartToCurrentTransform(true);
	movementManager.linkRotationRoutine(&test_shape[3], "TestowyR4");

	auto routineRVA = movementManager.createRotationRoutineVA("TestowyRVA1");
	routineRVA->addRotation(new rotationInfoVA(30.f, 180.f, animation_time, MovementContainer.getFunctionPointer(MovementContainer::IN_OUT_SINE), &arrow1, false, 0.5f, 0.5f, false));
	routineRVA->addRotation(new rotationInfoVA(180.f, 60.f, animation_time, MovementContainer.getFunctionPointer(MovementContainer::IN_OUT_SINE), &arrow1, false, 0.5f, 0.5f, false));
	routineRVA->adjustStartToCurrentTransform(true);
	movementManager.linkRotationRoutine(&arrow1, "TestowyRVA1");

	routineRVA = movementManager.createRotationRoutineVA("TestowyRVA2");
	routineRVA->addRotation(new rotationInfoVA(30.f, 180.f, animation_time, MovementContainer.getFunctionPointer(MovementContainer::IN_OUT_SINE), &arrow2, false, 0.5f, 0.5f, false));
	routineRVA->addRotation(new rotationInfoVA(180.f, 60.f, animation_time, MovementContainer.getFunctionPointer(MovementContainer::IN_OUT_SINE), &arrow2, false, 0.5f, 0.5f, true));
	routineRVA->adjustStartToCurrentTransform(true);
	movementManager.linkRotationRoutine(&arrow2, "TestowyRVA2");

	routineRVA = movementManager.createRotationRoutineVA("TestowyRVA3");
	routineRVA->addRotation(new rotationInfoVA(30.f, 180.f, animation_time, MovementContainer.getFunctionPointer(MovementContainer::IN_OUT_SINE), &arrow3, false, 0.5f, 0.5f, true));
	routineRVA->addRotation(new rotationInfoVA(180.f, 60.f, animation_time, MovementContainer.getFunctionPointer(MovementContainer::IN_OUT_SINE), &arrow3, false, 0.5f, 0.5f, false));
	routineRVA->adjustStartToCurrentTransform(true);
	movementManager.linkRotationRoutine(&arrow3, "TestowyRVA3");

	routineRVA = movementManager.createRotationRoutineVA("TestowyRVA4");
	routineRVA->addRotation(new rotationInfoVA(30.f, 180.f, animation_time, MovementContainer.getFunctionPointer(MovementContainer::IN_OUT_SINE), &arrow4, false, 0.5f, 0.5f, true));
	routineRVA->addRotation(new rotationInfoVA(180.f, 60.f, animation_time, MovementContainer.getFunctionPointer(MovementContainer::IN_OUT_SINE), &arrow4, false, 0.5f, 0.5f, true));
	routineRVA->adjustStartToCurrentTransform(true);
	movementManager.linkRotationRoutine(&arrow4, "TestowyRVA4");

	constexpr int test_shape_size = 10000;
	constexpr int test_routine_size = 10000;
	constexpr int max_movements_in_routine = 10;

	printf("Size before:\n");
	printf("Active: %lld\n", movementManager.getSizeShapeActiveMovement());
	printf("Shape: %lld\n", movementManager.getSizeShapeMovement());
	printf("Container: %lld\n", movementManager.getSizeContainerMovement());



	//sf::RectangleShape *test_shape = new sf::RectangleShape[test_shape_size];

	/*for (int i = 0; i < test_shape_size; i++) {
		test_shape[i].setSize(sf::Vector2f(5.f, 5.f));
		test_shape[i].setFillColor(sf::Color::Blue);
		test_shape[i].setPosition(100.f, 100.f);
	}

	for (size_t i = 0; i < test_routine_size; ++i) {
		auto* routine = movementManager.createMovementRoutine("T" + std::to_string(i));
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
		movementManager.linkMovementRoutine(&test_shape[i], "T" + std::to_string(i));

	for (size_t i = 0; i < test_shape_size; ++i)
		movementManager.startMovementRoutine(&test_shape[i], "T" + std::to_string(i));

	printf("Size after:\n");
	printf("Active: %lld\n", movementManager.getSizeShapeActive());
	printf("Shape: %lld\n", movementManager.getSizeShape());
	printf("Container: %lld\n", movementManager.getSizeContainer());*/

	//movementManager.printAllRoutines();

	while (window.isOpen())
	{
		dt = dt_clock.restart().asSeconds();
		
		//MovementContainer.update(dt);
		movementManager.update(dt);

		sf::Event event;
		if (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				window.close();

			// Start
			if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Space)) {
				movementManager.startRotationRoutine(&arrow1, "TestowyRVA1");
				movementManager.startRotationRoutine(&arrow2, "TestowyRVA2");
				movementManager.startRotationRoutine(&arrow3, "TestowyRVA3");
				movementManager.startRotationRoutine(&arrow4, "TestowyRVA4");

				movementManager.startRotationRoutine(&test_shape[0], "TestowyR1");
				movementManager.startRotationRoutine(&test_shape[1], "TestowyR2");
				movementManager.startRotationRoutine(&test_shape[2], "TestowyR3");
				movementManager.startRotationRoutine(&test_shape[3], "TestowyR4");
			}



			if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Up || event.key.code ==sf::Keyboard::W)) {
				current_ease_type += rows;
				if (current_ease_type >= easeTypeSize - 2)
					current_ease_type = 0;

				for (int i = 0; i < rows; i++) {
					text[i].setString(easeType[current_ease_type + i].first);
					graphs[i].setFunction(MovementContainer.getFunctionPointer(easeType[current_ease_type + i].second));
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
					text[i].setString(easeType[current_ease_type + i].first);
					graphs[i].setFunction(MovementContainer.getFunctionPointer(easeType[current_ease_type + i].second));
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