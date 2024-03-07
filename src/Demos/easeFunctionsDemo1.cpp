#include "easeFunctionsDemo1.h"
#include "../SidePanel.h"

void demo1(sf::RenderWindow& window) {
	MovementManager* movementManager = MovementManager::getInstance();

	// Config
	bool running = true;
	const int rows = 3;

	int current_ease_type  = 0;
	const int easeTypeSize = 30;
	
	float delay_before	 = 0.5f;
	float animation_time = 3.f;
	float delay_after	 = 0.5f;

	// Clock variables
	sf::Clock dt_clock;
	float dt;

	

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

	up_arrow.setOrigin(up_arrow.getCentroid());
	down_arrow.setOrigin(down_arrow.getCentroid());

	up_arrow.setPosition(960.f, 50.f);
	down_arrow.setPosition(960.f, 1030.f);

	float arrow_movement_time = 1.f;
	float arrow_wait_time     = 0.5f;

	// Movement
	auto movementRoutine = movementManager->createMovementRoutine("UP_ARROW_MOVEMENT");
	movementRoutine->addMovement(new movementInfo(sf::Vector2f(960.f, 50.f), sf::Vector2f(960.f, 30.f), arrow_movement_time, easeFunctions::IN_OUT_SINE, false, arrow_wait_time, arrow_wait_time));
	movementRoutine->addMovement(new movementInfo(sf::Vector2f(960.f, 30.f), sf::Vector2f(960.f, 50.f), arrow_movement_time, easeFunctions::IN_OUT_SINE, false, arrow_wait_time, arrow_wait_time));
	movementRoutine->adjustStartToCurrentTransform(true);
	movementRoutine->setLooping(true);
	movementManager->linkMovementRoutine(up_arrow, "UP_ARROW_MOVEMENT");
	movementManager->startMovementRoutine(up_arrow, "UP_ARROW_MOVEMENT");

	movementRoutine = movementManager->createMovementRoutine("DOWN_ARROW_MOVEMENT");
	movementRoutine->addMovement(new movementInfo(sf::Vector2f(960.f, 1030.f), sf::Vector2f(960.f, 1050.f), arrow_movement_time, easeFunctions::IN_OUT_SINE, false, arrow_wait_time, arrow_wait_time));
	movementRoutine->addMovement(new movementInfo(sf::Vector2f(960.f, 1050.f), sf::Vector2f(960.f, 1030.f), arrow_movement_time, easeFunctions::IN_OUT_SINE, false, arrow_wait_time, arrow_wait_time));
	movementRoutine->setLooping(true);
	movementManager->linkMovementRoutine(down_arrow, "DOWN_ARROW_MOVEMENT");
	movementManager->startMovementRoutine(down_arrow, "DOWN_ARROW_MOVEMENT");

	for (int i = 0; i < rows; i++) {
		auto movementRoutine = movementManager->createMovementRoutine("SHAPE_MOVEMENT_" + std::to_string(i));
		movementRoutine->addMovement(new movementInfo(start_pos[i], end_pos[i], animation_time, easeFunctions::getTmovement(current_ease_type + i), false, delay_before, delay_after));
		movementRoutine->addMovement(new movementInfo(end_pos[i], start_pos[i], animation_time, easeFunctions::getTmovement(current_ease_type + i), false, delay_before, delay_after));
		movementRoutine->setLooping(true);
		movementManager->linkMovementRoutine(shapes[i], "SHAPE_MOVEMENT_" + std::to_string(i));
		movementManager->startMovementRoutine(shapes[i], "SHAPE_MOVEMENT_" + std::to_string(i));

		auto scalingRoutine = movementManager->createScalingRoutine("SHAPE_SCALING_" + std::to_string(i));
		scalingRoutine->addScaling(new scalingInfo(sf::Vector2f(1.f, 1.f), sf::Vector2f(1.5f, 1.5f), animation_time, easeFunctions::getTmovement(current_ease_type + i), false, delay_before, delay_after));
		scalingRoutine->addScaling(new scalingInfo(sf::Vector2f(1.5f, 1.5f), sf::Vector2f(1.f, 1.f), animation_time, easeFunctions::getTmovement(current_ease_type + i), false, delay_before, delay_after));
		scalingRoutine->setLooping(true);
		movementManager->linkScalingRoutine(shapes[i], "SHAPE_SCALING_" + std::to_string(i));
		movementManager->startScalingRoutine(shapes[i], "SHAPE_SCALING_" + std::to_string(i));

		auto rotationRoutine = movementManager->createRotationRoutine("SHAPE_ROTATION_" + std::to_string(i));
		rotationRoutine->addRotation(new rotationInfo(0.f, 360.f, animation_time, easeFunctions::getTmovement(current_ease_type + i), false, delay_before, delay_after, true));
		rotationRoutine->addRotation(new rotationInfo(360.f, 0.f, animation_time, easeFunctions::getTmovement(current_ease_type + i), false, delay_before, delay_after, true));
		rotationRoutine->setLooping(true);
		movementManager->linkRotationRoutine(shapes[i], "SHAPE_ROTATION_" + std::to_string(i));
		movementManager->startRotationRoutine(shapes[i], "SHAPE_ROTATION_" + std::to_string(i));
	}


	// Side panel
	SidePanel side_panel(window, "Fonts/Helvetica Regular.otf", 48u, 28u, 20u);
	side_panel.setBackgroundColor(sf::Color(50, 50, 50, 245));
	side_panel.setTitle("Controls");
	side_panel.addText("Up      - Next function");
	side_panel.addText("Down - Previous function");
	side_panel.addText(" ");
	side_panel.addText("Left   - Decrease animation speed");
	side_panel.addText("Right - Increase animation speed");
	side_panel.addText(" ");
	side_panel.addText("M - Pause/Unpause movement");
	side_panel.addText("S - Pause/Unpause scaling");
	side_panel.addText("R - Pause/Unpause rotation");
	side_panel.addText(" ");
	side_panel.addText("C - Show controls");

	// GUI Text
	sf::Text controls_text("[C] - Controls", font, 20u);
	controls_text.setPosition(10.f, 10.f);

	sf::Text animation_speed_text("Animation speed: " + std::format("{:.2f}", animation_time) + "s", font, 20u);
	animation_speed_text.setPosition(10.f, 40.f);

	bool movement_paused = false, scaling_paused  = false, rotation_paused = false;
	sf::Text movement_paused_text("Movement: ON", font, 20u);
	sf::Text scaling_paused_text("Scaling: ON", font, 20u);
	sf::Text rotation_paused_text("Rotation: ON", font, 20u);

	movement_paused_text.setPosition(10.f, window.getSize().y - 120.f);
	scaling_paused_text.setPosition(10.f, window.getSize().y - 80.f);
	rotation_paused_text.setPosition(10.f, window.getSize().y - 40.f);

	while (running)
	{
		dt = dt_clock.restart().asSeconds();
		movementManager->update(dt);

		sf::Event event;
		if (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				running = false;

			if (event.type == sf::Event::KeyPressed) {
				// = = = = = Change ease function = = = = =
				if (event.key.code == sf::Keyboard::Up) {
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
				else if (event.key.code == sf::Keyboard::Down) {
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

				// = = = = = Change animation time = = = = =
				if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A) {
					animation_time -= 0.25f;
					if (animation_time < 0.25f)
						animation_time = 0.25f;

					animation_speed_text.setString("Animation speed: " + std::format("{:.2f}", animation_time) + "s");
					for (size_t i = 0; i < rows; i++)
						movementManager->setAnimationTime(shapes[i], animation_time);
				}
				else if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D) {
					animation_time += 0.25f;
					if (animation_time > 25.f)
						animation_time = 25.f;

					animation_speed_text.setString("Animation speed: " + std::format("{:.2f}", animation_time) + "s");
					for (size_t i = 0; i < rows; i++)
						movementManager->setAnimationTime(shapes[i], animation_time);
				}

				// = = = = = Pause/Unpause movement = = = = =
				if (event.key.code == sf::Keyboard::M) {
					if (movement_paused) {
						for (size_t i = 0; i < rows; ++i) {
							movementManager->resetMovementRoutine(shapes[i], "SHAPE_MOVEMENT_" + std::to_string(i));

							if (!scaling_paused)
								movementManager->resetScalingRoutine(shapes[i], "SHAPE_SCALING_" + std::to_string(i));

							if (!rotation_paused)
								movementManager->resetRotationRoutine(shapes[i], "SHAPE_ROTATION_" + std::to_string(i));
						}
						movement_paused_text.setString("Movement: ON");
					}
					else {
						for (size_t i = 0; i < rows; ++i)
							movementManager->pauseMovementRoutine(shapes[i], "SHAPE_MOVEMENT_" + std::to_string(i));
						movement_paused_text.setString("Movement: OFF");
					}

					movement_paused = !movement_paused;
				}

				// = = = = = Pause/Unpause scaling = = = = =
				if (event.key.code == sf::Keyboard::S) {
					if (scaling_paused) {
						for (size_t i = 0; i < rows; ++i) {
							movementManager->resetScalingRoutine(shapes[i], "SHAPE_SCALING_" + std::to_string(i));

							if (!movement_paused)
								movementManager->resetMovementRoutine(shapes[i], "SHAPE_MOVEMENT_" + std::to_string(i));

							if (!rotation_paused)
								movementManager->resetRotationRoutine(shapes[i], "SHAPE_ROTATION_" + std::to_string(i));
						}
						scaling_paused_text.setString("Scaling: ON");
					}
					else {
						for (size_t i = 0; i < rows; ++i)
							movementManager->pauseScalingRoutine(shapes[i], "SHAPE_SCALING_" + std::to_string(i));
						scaling_paused_text.setString("Scaling: OFF");
					}

					scaling_paused = !scaling_paused;
				}

				// = = = = = Pause/Unpause rotation = = = = =
				if (event.key.code == sf::Keyboard::R) {
					if (rotation_paused) {
						for (size_t i = 0; i < rows; ++i) {
							movementManager->resetRotationRoutine(shapes[i], "SHAPE_ROTATION_" + std::to_string(i));

							if (!movement_paused)
								movementManager->resetMovementRoutine(shapes[i], "SHAPE_MOVEMENT_" + std::to_string(i));

							if (!scaling_paused)
								movementManager->resetScalingRoutine(shapes[i], "SHAPE_SCALING_" + std::to_string(i));
						}
						rotation_paused_text.setString("Rotation: ON");
					}
					else {
						for (size_t i = 0; i < rows; ++i)
							movementManager->pauseRotationRoutine(shapes[i], "SHAPE_ROTATION_" + std::to_string(i));
						rotation_paused_text.setString("Rotation: OFF");
					}

					rotation_paused = !rotation_paused;
				}

				// = = = = = Show controls = = = = =
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C) {
					side_panel.toggle();
				}
			}
		}

		window.clear();
		for (int i = 0; i < rows; i++) {
			window.draw(text[i]);
			window.draw(shapes[i]);
			graphs[i].draw(window);
		}

		window.draw(up_arrow, up_arrow.getTransform());
		window.draw(down_arrow, down_arrow.getTransform());

		window.draw(animation_speed_text);
		window.draw(controls_text);
		window.draw(movement_paused_text);
		window.draw(scaling_paused_text);
		window.draw(rotation_paused_text);
		side_panel.draw(window);

		window.display();
	}

	movementManager->deleteMovementRoutine();
	movementManager->deleteScalingRoutine();
	movementManager->deleteRotationRoutine();
}