#include "easeFunctionsDemo1.h"
#include "../SidePanel.h"

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
	movementManager->linkMovementRoutine(up_arrow, "UP_ARROW_MOVEMENT");
	movementManager->startMovementRoutine(up_arrow, "UP_ARROW_MOVEMENT");

	movementRoutine = movementManager->createMovementRoutine("DOWN_ARROW_MOVEMENT");
	movementRoutine->addMovement(new movementInfo(sf::Vector2f(960.f, 1030.f), sf::Vector2f(960.f, 1050.f), arrow_movement_time, easeFunctions::IN_OUT_SINE, false, arrow_wait_time, arrow_wait_time));
	movementRoutine->addMovement(new movementInfo(sf::Vector2f(960.f, 1050.f), sf::Vector2f(960.f, 1030.f), arrow_movement_time, easeFunctions::IN_OUT_SINE, false, arrow_wait_time, arrow_wait_time));
	movementRoutine->setLooping(true);
	movementManager->linkMovementRoutine(down_arrow, "DOWN_ARROW_MOVEMENT");
	movementManager->startMovementRoutine(down_arrow, "DOWN_ARROW_MOVEMENT");

	// Scaling
	auto scalingRoutine = movementManager->createScalingRoutine("UP_ARROW_SCALING");
	scalingRoutine->addScaling(new scalingInfo(sf::Vector2f(1.f, 1.f), sf::Vector2f(1.2f, 1.2f), arrow_scaling_time, easeFunctions::IN_OUT_SINE, false, arrow_wait_time, arrow_wait_time));
	scalingRoutine->addScaling(new scalingInfo(sf::Vector2f(1.2f, 1.2f), sf::Vector2f(1.f, 1.f), arrow_scaling_time, easeFunctions::IN_OUT_SINE, false, arrow_wait_time, arrow_wait_time));
	scalingRoutine->setLooping(true);
	movementManager->linkScalingRoutine(up_arrow, "UP_ARROW_SCALING");
	movementManager->startScalingRoutine(up_arrow, "UP_ARROW_SCALING");

	scalingRoutine = movementManager->createScalingRoutine("DOWN_ARROW_SCALING");
	scalingRoutine->addScaling(new scalingInfo(sf::Vector2f(1.f, 1.f), sf::Vector2f(1.2f, 1.2f), arrow_scaling_time, easeFunctions::IN_OUT_SINE, false, arrow_wait_time, arrow_wait_time));
	scalingRoutine->addScaling(new scalingInfo(sf::Vector2f(1.2f, 1.2f), sf::Vector2f(1.f, 1.f), arrow_scaling_time, easeFunctions::IN_OUT_SINE, false, arrow_wait_time, arrow_wait_time));
	scalingRoutine->setLooping(true);
	movementManager->linkScalingRoutine(down_arrow, "DOWN_ARROW_SCALING");
	movementManager->startScalingRoutine(down_arrow, "DOWN_ARROW_SCALING");

	// Rotation
	auto rotationRoutine = movementManager->createRotationRoutine("UP_ARROW_ROTATION");
	rotationRoutine->addRotation(new rotationInfo(-10.f, 10.f, arrow_rotation_time, easeFunctions::IN_OUT_SINE, false, arrow_wait_time, arrow_wait_time, false));
	rotationRoutine->addRotation(new rotationInfo(10.f, -10.f, arrow_rotation_time, easeFunctions::IN_OUT_SINE, false, arrow_wait_time, arrow_wait_time, false));
	rotationRoutine->setLooping(true);
	movementManager->linkRotationRoutine(up_arrow, "UP_ARROW_ROTATION");
	movementManager->startRotationRoutine(up_arrow, "UP_ARROW_ROTATION");

	rotationRoutine = movementManager->createRotationRoutine("DOWN_ARROW_ROTATION");
	rotationRoutine->addRotation(new rotationInfo(10.f, -10.f, arrow_rotation_time, easeFunctions::IN_OUT_SINE, false, arrow_wait_time, arrow_wait_time, false));
	rotationRoutine->addRotation(new rotationInfo(-10.f, 10.f, arrow_rotation_time, easeFunctions::IN_OUT_SINE, false, arrow_wait_time, arrow_wait_time, false));
	rotationRoutine->setLooping(true);
	movementManager->linkRotationRoutine(down_arrow, "DOWN_ARROW_ROTATION");
	movementManager->startRotationRoutine(down_arrow, "DOWN_ARROW_ROTATION");

	sf::Clock dt_clock;
	float dt;
	float wait_time = 0.5f;

	for (int i = 0; i < rows; i++) {
		auto movementRoutine = movementManager->createMovementRoutine("SHAPE_MOVEMENT_" + std::to_string(i));
		movementRoutine->addMovement(new movementInfo(start_pos[i], end_pos[i], animation_time, easeFunctions::IN_OUT_SINE, false, shape_delay_before, shape_delay_after));
		movementRoutine->addMovement(new movementInfo(end_pos[i], start_pos[i], animation_time, easeFunctions::IN_OUT_SINE, false, shape_delay_before, shape_delay_after));
		movementRoutine->setLooping(true);
		movementManager->linkMovementRoutine(shapes[i], "SHAPE_MOVEMENT_" + std::to_string(i));
		movementManager->startMovementRoutine(shapes[i], "SHAPE_MOVEMENT_" + std::to_string(i));

		auto scalingRoutine = movementManager->createScalingRoutine("SHAPE_SCALING_" + std::to_string(i));
		scalingRoutine->addScaling(new scalingInfo(sf::Vector2f(1.f, 1.f), sf::Vector2f(1.5f, 1.5f), animation_time, easeFunctions::IN_OUT_SINE, false, shape_delay_before, shape_delay_after));
		scalingRoutine->addScaling(new scalingInfo(sf::Vector2f(1.5f, 1.5f), sf::Vector2f(1.f, 1.f), animation_time, easeFunctions::IN_OUT_SINE, false, shape_delay_before, shape_delay_after));
		scalingRoutine->setLooping(true);
		movementManager->linkScalingRoutine(shapes[i], "SHAPE_SCALING_" + std::to_string(i));
		movementManager->startScalingRoutine(shapes[i], "SHAPE_SCALING_" + std::to_string(i));

		auto rotationRoutine = movementManager->createRotationRoutine("SHAPE_ROTATION_" + std::to_string(i));
		rotationRoutine->addRotation(new rotationInfo(0.f, 360.f, animation_time, easeFunctions::IN_OUT_SINE, false, shape_delay_before, shape_delay_after, true));
		rotationRoutine->addRotation(new rotationInfo(360.f, 0.f, animation_time, easeFunctions::IN_OUT_SINE, false, shape_delay_before, shape_delay_after, true));
		rotationRoutine->setLooping(true);
		movementManager->linkRotationRoutine(shapes[i], "SHAPE_ROTATION_" + std::to_string(i));
		movementManager->startRotationRoutine(shapes[i], "SHAPE_ROTATION_" + std::to_string(i));
	}

	SidePanel side_panel(window, "Fonts/Helvetica Regular.otf", 48u, 32u, 12.f);
	side_panel.setBackgroundColor(sf::Color(50, 50, 50));
	side_panel.setPadding(40.f);
	side_panel.setTitle("Ease Functions");
	side_panel.addText("Up - Next function");
	side_panel.addText("Down - Previous function");
	side_panel.addText("X - Show panel");



	while (running)
	{
		dt = dt_clock.restart().asSeconds();
		movementManager->update(dt);

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

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X) {
				side_panel.show();
			}

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z) {
				side_panel.hide();
			}

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::LBracket) {
				side_panel.setTitleFontSize(side_panel.getTitleFontSize() + 1);
			}

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::RBracket) {
				side_panel.setTitleFontSize(side_panel.getTitleFontSize() - 1);
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

		side_panel.draw(window);

		window.display();
	}

	movementManager->deleteMovementRoutine();
	movementManager->deleteScalingRoutine();
	movementManager->deleteRotationRoutine();
}