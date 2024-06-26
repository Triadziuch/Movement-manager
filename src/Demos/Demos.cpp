#include "Demos.h"
#include <chrono>

using namespace std::chrono;

// = = = = = = = = = = = = = = = = = = = = = = = = =  Movement Demo 1 = = = = = = = = = = = = = = = = = = = = = = = = = 
void movementDemo1(sf::RenderWindow& window) {
	MovementManager* movementManager = new MovementManager();

	// Config
	bool running = true;
	const int rows = 3;

	const int default_ease_type = 1;
	int current_ease_type  = default_ease_type;
	const int easeTypeSize = 31;
	
	float delay_before	 = 0.5f;
	const float default_animation_time = 3.f;
	float animation_time = default_animation_time;
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
		movementRoutine = movementManager->createMovementRoutine("SHAPE_MOVEMENT_" + std::to_string(i));
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
	side_panel.addText("Up - Next function");
	side_panel.addText("Down - Previous function");
	side_panel.addText(" ");
	side_panel.addText("Left - Decrease animation speed");
	side_panel.addText("Right - Increase animation speed");
	side_panel.addText(" ");
	side_panel.addText("M - Pause/Unpause movement");
	side_panel.addText("S - Pause/Unpause scaling");
	side_panel.addText("R - Pause/Unpause rotation");
	side_panel.addText(" ");
	side_panel.addText("Q - Reset demo");
	side_panel.addText("ESC - Close demo");
	side_panel.addText(" ");
	side_panel.addText("C - Show controls");

	// GUI Text
	sf::Text controls_text("[C] - Controls", font, 20u);
	controls_text.setPosition(10.f, 10.f);

	sf::Text animation_speed_text("Animation speed: " + std::format("{:.2f}", animation_time) + "s", font, 20u);
	animation_speed_text.setPosition(10.f, 40.f);

	const bool default_paused = false;
	bool movement_paused = false, scaling_paused  = false, rotation_paused = false;
	sf::Text movement_paused_text("Movement: ON", font, 20u);
	sf::Text scaling_paused_text("Scaling: ON", font, 20u);
	sf::Text rotation_paused_text("Rotation: ON", font, 20u);

	movement_paused_text.setPosition(10.f, 70.f);
	scaling_paused_text.setPosition(10.f, 100.f);
	rotation_paused_text.setPosition(10.f, 130.f);

	while (running)
	{
		dt = dt_clock.restart().asSeconds();
		movementManager->update(dt);
		side_panel.update(dt);

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
						current_ease_type = 1;

					for (int i = 0; i < rows; i++) {
						text[i].setString(easeFunctions::getFunctionName(current_ease_type + i));
						graphs[i].setFunction(easeFunctions::getFunction(current_ease_type + i));

						movementManager->setFunction(shapes[i], easeFunctions::getTmovement(current_ease_type + i));
						movementManager->resetRoutines(shapes[i]);

						if (movement_paused) {
							movementManager->pauseMovementRoutine(shapes[i], "SHAPE_MOVEMENT_" + std::to_string(i));
							shapes[i].setPosition(sf::Vector2f(window.getSize().x / 2.f, shapes[i].getPosition().y));
						}

						if (scaling_paused) {
							movementManager->pauseScalingRoutine(shapes[i], "SHAPE_SCALING_" + std::to_string(i));
							shapes[i].setScale(1.f, 1.f);
						}

						if (rotation_paused) {
							movementManager->pauseRotationRoutine(shapes[i], "SHAPE_ROTATION_" + std::to_string(i));
							shapes[i].setRotation(0.f);
						}
					}
				}
				else if (event.key.code == sf::Keyboard::Down) {
					current_ease_type -= rows;
					if (current_ease_type < 1)
						current_ease_type = easeTypeSize - rows;

					for (size_t i = 0; i < rows; i++) {
						text[i].setString(easeFunctions::getFunctionName(current_ease_type + i));
						graphs[i].setFunction(easeFunctions::getFunction(current_ease_type + i));

						movementManager->setFunction(shapes[i], easeFunctions::getTmovement(current_ease_type + i));
						movementManager->resetRoutines(shapes[i]);

						if (movement_paused) {
							movementManager->pauseMovementRoutine(shapes[i], "SHAPE_MOVEMENT_" + std::to_string(i));
							shapes[i].setPosition(sf::Vector2f(window.getSize().x / 2.f, shapes[i].getPosition().y));
						}

						if (scaling_paused) {
							movementManager->pauseScalingRoutine(shapes[i], "SHAPE_SCALING_" + std::to_string(i));
							shapes[i].setScale(1.f, 1.f);
						}

						if (rotation_paused) {
							movementManager->pauseRotationRoutine(shapes[i], "SHAPE_ROTATION_" + std::to_string(i));
							shapes[i].setRotation(0.f);
						}
					}
				}

				// = = = = = Change animation time = = = = =
				if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A) {
					animation_time -= 0.25f;
					if (animation_time < 0.25f)
						animation_time = 0.25f;

					animation_speed_text.setString("Animation speed: " + std::format("{:.2f}", animation_time) + "s");
					for (size_t i = 0; i < rows; i++)
						movementManager->setMotionDuration(shapes[i], animation_time, false);
				}
				else if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D) {
					animation_time += 0.25f;
					if (animation_time > 25.f)
						animation_time = 25.f;

					animation_speed_text.setString("Animation speed: " + std::format("{:.2f}", animation_time) + "s");
					for (size_t i = 0; i < rows; i++)
						movementManager->setMotionDuration(shapes[i], animation_time, false);
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
						for (size_t i = 0; i < rows; ++i) {
							movementManager->pauseMovementRoutine(shapes[i], "SHAPE_MOVEMENT_" + std::to_string(i));
							shapes[i].setPosition(sf::Vector2f(window.getSize().x / 2.f, shapes[i].getPosition().y));
						}
							
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
						for (size_t i = 0; i < rows; ++i) {
							movementManager->pauseScalingRoutine(shapes[i], "SHAPE_SCALING_" + std::to_string(i));
							shapes[i].setScale(1.f, 1.f);
						}
							
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
						for (size_t i = 0; i < rows; ++i) {
							movementManager->pauseRotationRoutine(shapes[i], "SHAPE_ROTATION_" + std::to_string(i));
							shapes[i].setRotation(0.f);
						}
							
						rotation_paused_text.setString("Rotation: OFF");
					}

					rotation_paused = !rotation_paused;
				}

				// = = = = = Reset demo = = = = =
				if (event.key.code == sf::Keyboard::Q) {
					current_ease_type = default_ease_type;
					animation_time = default_animation_time;
					movement_paused = default_paused;
					scaling_paused = default_paused;
					rotation_paused = default_paused;
					animation_speed_text.setString("Animation speed: " + std::format("{:.2f}", animation_time) + "s");
					movement_paused_text.setString("Movement: ON");
					scaling_paused_text.setString("Scaling: ON");
					rotation_paused_text.setString("Rotation: ON");

					for (size_t i = 0; i < rows; i++) {
						text[i].setString(easeFunctions::getFunctionName(current_ease_type + i));
						graphs[i].setFunction(easeFunctions::getFunction(current_ease_type + i));

						movementManager->setFunction(shapes[i], easeFunctions::getTmovement(current_ease_type + i));
						movementManager->resetRoutines(shapes[i]);
					}
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

		window.draw(controls_text);
		window.draw(animation_speed_text);
		window.draw(movement_paused_text);
		window.draw(scaling_paused_text);
		window.draw(rotation_paused_text);
		
		side_panel.draw(window);

		window.display();
	}

	delete movementManager;
}

// = = = = = = = = = = = = = = = = = = = = = = = = =    Plot Demo 1   = = = = = = = = = = = = = = = = = = = = = = = = = 
void plotDemo(sf::RenderWindow& window)
{
	MovementManager* movementManager = new MovementManager();

	// Config
	bool running = true;

	const int default_ease_type = 0;
	int current_ease_type = default_ease_type;
	const int easeTypeSize = 30;

	// Graph variables
	const int default_precision = 200;
	int precision = default_precision;
	const int precision_step = 2;
	sf::Vector2f graph_size(1200.f, 600.f);

	// Clock variables
	sf::Clock dt_clock;
	float dt;

	// Graph initialization
	Graph graph;
	graph.setPrecision(precision);
	graph.setSize(graph_size);
	graph.setPosition(500.f, window.getSize().y - 230.f);
	graph.setFunction(easeFunctions::getFunction(current_ease_type));

	// GUI Initialization
	sf::Font font;
	if (!font.loadFromFile("Fonts/Helvetica Regular.otf"))
		std::cout << "ERROR: Font not found!\n";


	// Side panel
	SidePanel side_panel(window, "Fonts/Helvetica Regular.otf", 48u, 28u, 20u);
	side_panel.setBackgroundColor(sf::Color(50, 50, 50, 245));
	side_panel.setTitle("Controls");
	side_panel.addText("Up - Next function");
	side_panel.addText("Down - Previous function");
	side_panel.addText(" ");
	side_panel.addText("Left - Decrease graph precision");
	side_panel.addText("Right - Increase graph precision");
	side_panel.addText(" ");
	side_panel.addText("V - Turn on/off axis");
	side_panel.addText(" ");
	side_panel.addText("Q - Reset demo");
	side_panel.addText("ESC - Close demo");
	side_panel.addText(" ");
	side_panel.addText("C - Show controls");

	// GUI Text
	sf::Text controls_text("[C] - Controls", font, 20u);
	controls_text.setPosition(10.f, 10.f);

	sf::Text current_function_text("Current function: " + easeFunctions::getFunctionName(current_ease_type), font, 20u);
	current_function_text.setPosition(10.f, 40.f);

	sf::Text precision_text("Precision: " + std::to_string(precision), font, 20u);
	precision_text.setPosition(10.f, 70.f);


	while (running)
	{
		dt = dt_clock.restart().asSeconds();
		movementManager->update(dt);
		side_panel.update(dt);

		sf::Event event;
		if (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				running = false;

			if (event.type == sf::Event::KeyPressed) {
				// = = = = = Change ease function = = = = =
				if (event.key.code == sf::Keyboard::Up) {
					current_ease_type++;
					if (current_ease_type >= easeTypeSize)
						current_ease_type = 0;

					graph.setFunction(easeFunctions::getFunction(current_ease_type));
					current_function_text.setString("Current function: " + easeFunctions::getFunctionName(current_ease_type));
				}
				else if (event.key.code == sf::Keyboard::Down) {
					current_ease_type--;
					if (current_ease_type < 0)
						current_ease_type = 29;

					graph.setFunction(easeFunctions::getFunction(current_ease_type));
					current_function_text.setString("Current function: " + easeFunctions::getFunctionName(current_ease_type));
				}

				// = = = = = Change precision = = = = =
				if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A) {
					precision -= precision_step;
					if (precision < 0)
						precision = 0;
					graph.setPrecision(precision);
					precision_text.setString("Precision: " + std::to_string(precision));
				}
				else if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D) {
					precision += precision_step;
					graph.setPrecision(precision);
					precision_text.setString("Precision: " + std::to_string(precision));
				}

				// = = = = = Turn on/off axis = = = = =
				if (event.key.code == sf::Keyboard::V) {
					graph.toggleAxisVisible();
				}

				// = = = = = Reset demo = = = = =
				if (event.key.code == sf::Keyboard::Q) {
					current_ease_type = default_ease_type;
					precision = default_precision;
					graph.setPrecision(precision);
					graph.setFunction(easeFunctions::getFunction(current_ease_type));
					graph.setAxisVisible(true);
					current_function_text.setString("Current function: " + easeFunctions::getFunctionName(current_ease_type));
					precision_text.setString("Precision: " + std::to_string(precision));
				}

				// = = = = = Show controls = = = = =
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C) {
					side_panel.toggle();
				}
			}
		}

		window.clear();
		graph.draw(window);
		window.draw(controls_text);
		window.draw(current_function_text);
		window.draw(precision_text);
		side_panel.draw(window);

		window.display();
	}
}

// = = = = = = = = = = = = = = = = = = = = = = = = =  Movement Demo 2 = = = = = = = = = = = = = = = = = = = = = = = = = 
void movementDemo2(sf::RenderWindow& window)
{
	// Configuration
	MovementManager* movementManager = new MovementManager();
	bool running = true;
	float delay_before = 0.3f, animation_time = 0.5f, delay_after = 0.3f;

	// GUI Initialization
	sf::Font font;
	if (!font.loadFromFile("Fonts/Helvetica Regular.otf"))
		std::cout << "ERROR: Font not found!\n";

	sf::Clock dt_clock;
	float dt;

	sf::CircleShape shape[4];
	for (size_t i = 0; i < 4; i++) {
		shape[i].setPointCount(3);
		shape[i].setRadius(50.f);
		shape[i].setFillColor(sf::Color::Blue);
		shape[i].setOrigin(shape[i].getRadius(), shape[i].getRadius());
		shape[i].setPosition(400.f, 240.f + 200.f * static_cast<float>(i));
	}

	std::vector<VertexArray2> arrows;
	for (size_t i = 0; i < 4; i++) {
		VertexArray2 arrow(sf::LineStrip, 3u);
		arrow[0].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f - 40.f, 240.f + 200.f * static_cast<float>(i));
		arrow[1].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f, 210.f + 200.f * static_cast<float>(i));
		arrow[2].position = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f + 40.f, 240.f + 200.f * static_cast<float>(i));

		arrow.setOrigin(arrow.getCentroid());
		arrow.setPosition(600.f, 240.f + 200.f * static_cast<float>(i));
		arrows.push_back(arrow);
	}

	bool adjust_start_movement = false,
		 adjust_all_movement = false,
		 looping_movement = false;

	sf::Vector2f starting_offset1(0.f, 0.f);
	sf::Vector2f ending_offset1 = sf::Vector2f(300.f, -100.f) + starting_offset1;
	sf::Vector2f starting_offset2 = sf::Vector2f(0.f, 0.f) + ending_offset1;
	sf::Vector2f ending_offset2 = sf::Vector2f(0., 100.f) + starting_offset2;
	sf::Vector2f starting_offset3 = sf::Vector2f(0.f, 0.f) + ending_offset2;
	sf::Vector2f ending_offset3 = sf::Vector2f(-100.f, 0.f) + starting_offset3;

	for (size_t i = 0; i < 4; ++i) {
		auto routineMShape = movementManager->createMovementRoutine("TestowyM" + std::to_string(i + 1));
		routineMShape->addMovement(new movementInfo(shape[i].getPosition() + starting_offset1, shape[i].getPosition() + ending_offset1, animation_time, easeFunctions::IN_OUT_SINE, false, delay_before, 0.5f));
		routineMShape->addMovement(new movementInfo(shape[i].getPosition() + starting_offset2, shape[i].getPosition() + ending_offset2, animation_time, easeFunctions::IN_OUT_SINE, false, delay_before, 0.5f));
		routineMShape->addMovement(new movementInfo(shape[i].getPosition() + starting_offset3, shape[i].getPosition() + ending_offset3, animation_time, easeFunctions::IN_OUT_SINE, false, delay_before, 0.5f));
		routineMShape->adjustStartToCurrentTransform(adjust_start_movement);
		routineMShape->adjustAllToCurrentTransform(adjust_all_movement);
		routineMShape->setLooping(looping_movement);
		movementManager->linkMovementRoutine(shape[i], "TestowyM" + std::to_string(i + 1));

		auto routineMVA = movementManager->createMovementRoutine("TestowyMVA" + std::to_string(i + 1));
		routineMVA->addMovement(new movementInfo(arrows[i].getPosition() + starting_offset1, arrows[i].getPosition() + ending_offset1, animation_time, easeFunctions::IN_OUT_SINE, false, delay_before, 0.5f));
		routineMVA->addMovement(new movementInfo(arrows[i].getPosition() + starting_offset2, arrows[i].getPosition() + ending_offset2, animation_time, easeFunctions::IN_OUT_SINE, false, delay_before, 0.5f));
		routineMVA->addMovement(new movementInfo(arrows[i].getPosition() + starting_offset3, arrows[i].getPosition() + ending_offset3, animation_time, easeFunctions::IN_OUT_SINE, false, delay_before, 0.5f));
		routineMVA->adjustStartToCurrentTransform(adjust_start_movement);
		routineMVA->adjustAllToCurrentTransform(adjust_all_movement);
		routineMVA->setLooping(looping_movement);
		movementManager->linkMovementRoutine(arrows[i], "TestowyMVA" + std::to_string(i + 1));
	}

	bool adjust_start_rotation = false;
	bool adjust_all_rotation = false;
	bool looping_rotation = false;

	for (size_t i = 0; i < 4; ++i) {
		auto routineRShape = movementManager->createRotationRoutine("TestowyR" + std::to_string(i + 1));
		routineRShape->addRotation(new rotationInfo(30.f, 180.f, animation_time, easeFunctions::IN_OUT_SINE, false, delay_before, delay_after, false));
		routineRShape->addRotation(new rotationInfo(180.f, 60.f, animation_time, easeFunctions::IN_OUT_SINE, false, delay_before, delay_after, false));
		routineRShape->addRotation(new rotationInfo(60.f, -30.f, animation_time, easeFunctions::IN_OUT_SINE, false, delay_before, delay_after, false));
		routineRShape->adjustStartToCurrentTransform(adjust_start_rotation);
		routineRShape->adjustAllToCurrentTransform(adjust_all_rotation);
		routineRShape->setLooping(looping_rotation);
		movementManager->linkRotationRoutine(shape[i], "TestowyR" + std::to_string(i + 1));

		auto routineRVA = movementManager->createRotationRoutine("TestowyRVA" + std::to_string(i + 1));
		routineRVA->addRotation(new rotationInfo(30.f, 180.f, animation_time, easeFunctions::IN_OUT_SINE, false, delay_before, delay_after, false));
		routineRVA->addRotation(new rotationInfo(180.f, 60.f, animation_time, easeFunctions::IN_OUT_SINE, false, delay_before, delay_after, false));
		routineRVA->addRotation(new rotationInfo(60.f, -30.f, animation_time, easeFunctions::IN_OUT_SINE, false, delay_before, delay_after, false));
		routineRVA->adjustStartToCurrentTransform(adjust_start_rotation);
		routineRVA->adjustAllToCurrentTransform(adjust_all_rotation);
		routineRVA->setLooping(looping_rotation);
		movementManager->linkRotationRoutine(arrows[i], "TestowyRVA" + std::to_string(i + 1));
	}

	bool adjust_start_scaling = false;
	bool adjust_all_scaling = false;
	bool looping_scaling = false;
	sf::Vector2f starting_scale1(1.f, 1.f);
	sf::Vector2f ending_scale1(1.5f, 1.5f);
	sf::Vector2f starting_scale2(1.5f, 1.5f);
	sf::Vector2f ending_scale2(1.25f, 1.25f);
	sf::Vector2f starting_scale3(1.25f, 1.25f);
	sf::Vector2f ending_scale3(1.1f, 1.1f);

	for (size_t i = 0; i < 4; ++i) {
		auto routineSShape = movementManager->createScalingRoutine("TestowyS" + std::to_string(i + 1));
		routineSShape->addScaling(new scalingInfo(starting_scale1, ending_scale1, animation_time, easeFunctions::IN_OUT_SINE, false, delay_before, delay_after));
		routineSShape->addScaling(new scalingInfo(starting_scale2, ending_scale2, animation_time, easeFunctions::IN_OUT_SINE, false, delay_before, delay_after));
		routineSShape->addScaling(new scalingInfo(starting_scale3, ending_scale3, animation_time, easeFunctions::IN_OUT_SINE, false, delay_before, delay_after));
		routineSShape->adjustStartToCurrentTransform(adjust_start_scaling);
		routineSShape->adjustAllToCurrentTransform(adjust_all_scaling);
		routineSShape->setLooping(looping_scaling);
		movementManager->linkScalingRoutine(shape[i], "TestowyS" + std::to_string(i + 1));

		auto routineSVA = movementManager->createScalingRoutine("TestowySVA" + std::to_string(i + 1));
		routineSVA->addScaling(new scalingInfo(starting_scale1, ending_scale1, animation_time, easeFunctions::IN_OUT_SINE, false, delay_before, delay_after));
		routineSVA->addScaling(new scalingInfo(starting_scale2, ending_scale2, animation_time, easeFunctions::IN_OUT_SINE, false, delay_before, delay_after));
		routineSVA->addScaling(new scalingInfo(starting_scale3, ending_scale3, animation_time, easeFunctions::IN_OUT_SINE, false, delay_before, delay_after));
		routineSVA->adjustStartToCurrentTransform(adjust_start_scaling);
		routineSVA->adjustAllToCurrentTransform(adjust_all_scaling);
		routineSVA->setLooping(looping_scaling);
		movementManager->linkScalingRoutine(arrows[i], "TestowySVA" + std::to_string(i + 1));
	}

	// Side panel initialization
	SidePanel side_panel(window, "Fonts/Helvetica Regular.otf", 48u, 28u, 20u);
	side_panel.setBackgroundColor(sf::Color(50, 50, 50, 245));
	side_panel.setTitle("Controls");
	side_panel.addText("Space - Start all");
	side_panel.addText("M - Start movement");
	side_panel.addText("S - Start scaling");
	side_panel.addText("R - Start rotation");
	side_panel.addText(" ");
	side_panel.addText("F - Adjust first to current transform");
	side_panel.addText("A - Adjust all to current transform");
	side_panel.addText("L - Toggle looping");
	side_panel.addText(" ");
	side_panel.addText("Q - Reset demo");
	side_panel.addText("ESC - Close demo");
	side_panel.addText(" ");
	side_panel.addText("C - Show controls");

	// GUI Initialization
	sf::Text controls_text("[C] - Controls", font, 20u);
	controls_text.setPosition(10.f, 10.f);

	sf::Text adjust_all_text("Adjust all to current transform: OFF", font, 20u);
	adjust_all_text.setPosition(10.f, 40.f);

	sf::Text adjust_start_text("Adjust first to current transform: OFF", font, 20u);
	adjust_start_text.setPosition(10.f, 70.f);

	sf::Text looping_text("Looping: OFF", font, 20u);
	looping_text.setPosition(10.f, 100.f);

	while (running)
	{
		dt = dt_clock.restart().asSeconds();
		movementManager->update(dt);
		side_panel.update(dt);


		// Event handling
		sf::Event event;
		if (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				running = false;

			if (event.type == sf::Event::KeyPressed) {

				// Start all routines
				if (event.key.code == sf::Keyboard::Space) 
					for (size_t i = 0; i < 4; i++) {
						movementManager->startMovementRoutine(shape[i], "TestowyM" + std::to_string(i + 1));
						movementManager->startScalingRoutine(shape[i], "TestowyS" + std::to_string(i + 1));
						movementManager->startRotationRoutine(shape[i], "TestowyR" + std::to_string(i + 1));

						movementManager->startMovementRoutine(arrows[i], "TestowyMVA" + std::to_string(i + 1));
						movementManager->startScalingRoutine(arrows[i], "TestowySVA" + std::to_string(i + 1));
						movementManager->startRotationRoutine(arrows[i], "TestowyRVA" + std::to_string(i + 1));
					}

				// Start rotation routines
				if (event.key.code == sf::Keyboard::R) 
					for (size_t i = 0; i < 4; i++) {
						movementManager->startRotationRoutine(shape[i], "TestowyR" + std::to_string(i + 1));
						movementManager->startRotationRoutine(arrows[i], "TestowyRVA" + std::to_string(i + 1));
					}

				// Start movement routines
				if (event.key.code == sf::Keyboard::M) 
					for (size_t i = 0; i < 4; i++) {
						movementManager->startMovementRoutine(shape[i], "TestowyM" + std::to_string(i + 1));
						movementManager->startMovementRoutine(arrows[i], "TestowyMVA" + std::to_string(i + 1));
					}
				
				// Start scaling routines
				if (event.key.code == sf::Keyboard::S) 
					for (size_t i = 0; i < 4; i++) {
						movementManager->startScalingRoutine(shape[i], "TestowyS" + std::to_string(i + 1));
						movementManager->startScalingRoutine(arrows[i], "TestowySVA" + std::to_string(i + 1));
					}
				
				// Reset demo
				if (event.key.code == sf::Keyboard::Q) {
					for (size_t i = 0; i < 4; i++) {
						shape[i].setPosition(400.f, 240.f + 200.f * i);
						shape[i].setRotation(0.f);
						shape[i].setScale(1.f, 1.f);

						arrows[i].setPosition(600.f, 240.f + 200.f * i);
						arrows[i].setRotation(0.f);
						arrows[i].setScale(1.f, 1.f);
					}

					looping_movement = false;
					looping_rotation = false;
					looping_scaling = false;

					adjust_start_movement = false;
					adjust_all_movement = false;
					adjust_start_rotation = false;

					adjust_start_text.setString("Adjust first to current transform: OFF");
					adjust_all_text.setString("Adjust all to current transform: OFF");
					looping_text.setString("Looping: OFF");
				}

				// Adjust first to current transform
				if (event.key.code == sf::Keyboard::F) {
					adjust_start_movement = !adjust_start_movement;
					adjust_start_rotation = !adjust_start_rotation;
					adjust_start_scaling  = !adjust_start_scaling;

					if (adjust_start_movement)
						adjust_start_text.setString("Adjust first to current transform: ON");
					else
						adjust_start_text.setString("Adjust first to current transform: OFF");
				}

				// Adjust all to current transform
				if (event.key.code == sf::Keyboard::A) {
					adjust_all_movement = !adjust_all_movement;
					adjust_all_rotation = !adjust_all_rotation;
					adjust_all_scaling = !adjust_all_scaling;

					if (adjust_all_movement)
						adjust_all_text.setString("Adjust all to current transform: ON");
					else
						adjust_all_text.setString("Adjust all to current transform: OFF");
				}

				if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::F || event.key.code == sf::Keyboard::Q) 
					for (size_t i = 0; i < 4; ++i) {
						movementManager->unlinkMovementRoutine(&shape[i], "TestowyM" + std::to_string(i + 1));
						auto movementRoutine = movementManager->linkMovementRoutine(shape[i], "TestowyM" + std::to_string(i + 1));
						movementRoutine->adjustStartToCurrentTransform(adjust_start_movement);
						movementRoutine->adjustAllToCurrentTransform(adjust_all_movement);
						movementRoutine->setLooping(looping_movement);

						movementManager->unlinkScalingRoutine(&shape[i], "TestowyS" + std::to_string(i + 1));
						auto scalingRoutine = movementManager->linkScalingRoutine(shape[i], "TestowyS" + std::to_string(i + 1));
						scalingRoutine->adjustStartToCurrentTransform(adjust_start_scaling);
						scalingRoutine->adjustAllToCurrentTransform(adjust_all_scaling);
						scalingRoutine->setLooping(looping_scaling);

						movementManager->unlinkRotationRoutine(&shape[i], "TestowyR" + std::to_string(i + 1));
						auto rotationRoutine = movementManager->linkRotationRoutine(shape[i], "TestowyR" + std::to_string(i + 1));
						rotationRoutine->adjustStartToCurrentTransform(adjust_start_rotation);
						rotationRoutine->adjustAllToCurrentTransform(adjust_all_rotation);
						rotationRoutine->setLooping(looping_rotation);

						movementManager->unlinkMovementRoutine(&arrows[i], "TestowyMVA" + std::to_string(i + 1));
						movementRoutine = movementManager->linkMovementRoutine(arrows[i], "TestowyMVA" + std::to_string(i + 1));
						movementRoutine->adjustStartToCurrentTransform(adjust_start_movement);
						movementRoutine->adjustAllToCurrentTransform(adjust_all_movement);
						movementRoutine->setLooping(looping_movement);

						movementManager->unlinkScalingRoutine(&arrows[i], "TestowySVA" + std::to_string(i + 1));
						scalingRoutine = movementManager->linkScalingRoutine(arrows[i], "TestowySVA" + std::to_string(i + 1));
						scalingRoutine->adjustStartToCurrentTransform(adjust_start_scaling);
						scalingRoutine->adjustAllToCurrentTransform(adjust_all_scaling);
						scalingRoutine->setLooping(looping_scaling);

						movementManager->unlinkRotationRoutine(&arrows[i], "TestowyRVA" + std::to_string(i + 1));
						rotationRoutine = movementManager->linkRotationRoutine(arrows[i], "TestowyRVA" + std::to_string(i + 1));
						rotationRoutine->adjustStartToCurrentTransform(adjust_start_rotation);
						rotationRoutine->adjustAllToCurrentTransform(adjust_all_rotation);
						rotationRoutine->setLooping(looping_rotation);
					}

				// Looping movement
				if (event.key.code == sf::Keyboard::L) {
					looping_movement = !looping_movement;
					looping_rotation = !looping_rotation;
					looping_scaling = !looping_scaling;

					if (looping_movement)
						looping_text.setString("Looping: ON");
					else
						looping_text.setString("Looping: OFF");

					for (size_t i = 0; i < 4; ++i) {
						movementManager->getMovementRoutine(shape[i], "TestowyM" + std::to_string(i + 1))->setLooping(looping_movement);
						movementManager->getScalingRoutine(shape[i], "TestowyS" + std::to_string(i + 1))->setLooping(looping_scaling);
						movementManager->getRotationRoutine(shape[i], "TestowyR" + std::to_string(i + 1))->setLooping(looping_rotation);

						movementManager->getMovementRoutine(arrows[i], "TestowyMVA" + std::to_string(i + 1))->setLooping(looping_movement);
						movementManager->getScalingRoutine(arrows[i], "TestowySVA" + std::to_string(i + 1))->setLooping(looping_scaling);
						movementManager->getRotationRoutine(arrows[i], "TestowyRVA" + std::to_string(i + 1))->setLooping(looping_rotation);
					}
				}

				// Show controls
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C) 
					side_panel.toggle();
				
			}
		}
		window.clear();
		for (size_t i = 0; i < 4; i++) {
			window.draw(shape[i]);
			window.draw(arrows[i], arrows[i].getTransform());
		}

		window.draw(controls_text);
		window.draw(adjust_start_text);
		window.draw(adjust_all_text);
		window.draw(looping_text);
		side_panel.draw(window);

		window.display();
	}

	delete movementManager;
}

// = = = = = = = = = = = = = = = = = = = = = = = = =  Movement Demo 3 = = = = = = = = = = = = = = = = = = = = = = = = = 
inline const sf::Vector2f randomPosition(const sf::Vector2f& min = sf::Vector2f(0.f, 0.f), const sf::Vector2f& max = sf::Vector2f(1920.f, 1080.f)) {
	return sf::Vector2f(static_cast<float>(rand() % static_cast<int>(max.x - min.x) + static_cast<int>(min.x)), static_cast<float>(rand() % static_cast<int>(max.y - min.y) + static_cast<int>(min.y)));
}

inline const size_t randomEaseType(const size_t min = 0, const size_t max = 30) {
	size_t random = static_cast<size_t>(rand() % static_cast<int>(max - min) + static_cast<int>(min));
	if (random == 18 || random == 19 || random == 20)
		return randomEaseType(min, max);
	return static_cast<size_t>(rand() % static_cast<int>(max - min) + static_cast<int>(min));
}

inline const float randomDelay(const float min = 0.1f, const float max = 1.f) {
	return static_cast<float>(rand() % static_cast<int>(max * 100 - min * 100) + static_cast<int>(min * 100)) / 100.f;
}

void movementDemo3(sf::RenderWindow& window) {
	MovementManager* movementManager = new MovementManager();

	long long int time_movement{};
	float average_time_ms = 0.f;
	bool synced = false, gui = true, running = true, synchronization = false;
	int it = 0;

	int current_ease_type = 1;
	const int easeTypeSize = 31;
	bool random_ease_type = true;

	const int routines = 200000;
	int shapes_count = 50000;
	const int default_shapes_count = shapes_count;
	int movements_in_routine = 10;
	sf::Vector2f shape_size(2.f, 2.f);

	// Routine initialization
	for (int i = 0; i < routines; ++i) {
		auto movementRoutine = movementManager->createMovementRoutine("SM" + std::to_string(i));
		sf::Vector2f previous_position = randomPosition();
		for (int j = 0; j < movements_in_routine; ++j) {
			sf::Vector2f new_position = randomPosition();
			movementRoutine->addMovement(new movementInfo(previous_position, new_position, randomDelay(5.f, 10.f), easeFunctions::getTmovement(randomEaseType()), false, randomDelay(), randomDelay()));
			previous_position = new_position;
		}
		movementRoutine->setLooping(true);
		movementRoutine->adjustStartToCurrentTransform(true);
	}

	// Shapes initialiaztion
	std::vector<sf::RectangleShape*> shapes;
	for (int i = 0; i < shapes_count; i++) {
		sf::RectangleShape* shape = new sf::RectangleShape;
		shape->setSize(shape_size);
		shape->setFillColor(sf::Color::Blue);
		shape->setOrigin(shape->getSize().x / 2.f, shape->getSize().y / 2.f);
		shape->setPosition(randomPosition());
		shapes.emplace_back(shape);

		movementManager->linkMovementRoutine(*shape, "SM" + std::to_string(i % routines));
		movementManager->startMovementRoutine(*shape, "SM" + std::to_string(i % routines));
	}

	sf::Clock dt_clock;
	float dt;

	// GUI Initialization
	sf::Font font;
	if (!font.loadFromFile("Fonts/Helvetica Regular.otf"))
		throw std::runtime_error("Cannot load font");

	SidePanel side_panel(window, "Fonts/Helvetica Regular.otf", 48u, 28u, 20u);
	side_panel.setTitle("Controls");
	side_panel.addText(" ");
	side_panel.addText("Up - +1000 shapes");
	side_panel.addText("Down - -1000 shapes");
	side_panel.addText("Left / Right - Change function");
	side_panel.addText("");
	side_panel.addText("[ - Decrease shape size");
	side_panel.addText("] - Increase shape size");
	side_panel.addText("");
	side_panel.addText("S - Synchronization on/off");
	side_panel.addText("");
	side_panel.addText("Q - Reset demo");
	side_panel.addText("ESC - Close demo");
	side_panel.addText(" ");
	side_panel.addText("H - Hide GUI");
	side_panel.addText("C - Show controls");

	sf::Text controls_text("[C] - Controls", font, 20u);
	controls_text.setPosition(10.f, 10.f);
	sf::Text shapes_count_text("Shapes count: " + std::to_string(shapes_count), font, 20u);
	shapes_count_text.setPosition(10.f, 40.f);
	sf::Text average_time_text("Update time: " + std::to_string(average_time_ms) + " ms", font, 20u);
	average_time_text.setPosition(10.f, 70.f);
	sf::Text current_function_text("Current function: Random", font, 20u);
	current_function_text.setPosition(10.f, 100.f);
	sf::Text shape_size_text("Shape size: " + std::format("{:.2f}", shape_size.x), font, 20u);
	shape_size_text.setPosition(10.f, 130.f);
	sf::Text synchronization_text("Synchronization: OFF", font, 20u);
	synchronization_text.setPosition(10.f, 160.f);

	while (running)
	{
		dt = dt_clock.restart().asSeconds();

		auto start = high_resolution_clock::now();
		movementManager->update(dt);
		auto stop = high_resolution_clock::now();
		time_movement += duration_cast<nanoseconds>(stop - start).count();
		side_panel.update(dt);
		it++;

		// Calculate upadte time
		if (it == 100 && !synced) {
			synced = true;
			it = 0;
			time_movement = 0;
		}

		if (it % 10 == 0 && it != 0) {
			average_time_ms = static_cast<float>(time_movement / it) / 1000000.f;
			average_time_text.setString("Update time: " + std::to_string(average_time_ms) + " ms");

			if (it % 50 == 0) {
				it = 0;
				time_movement = 0;
			}
		}

		// Event handling
		sf::Event event;
		if (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				running = false;

			if (event.type == sf::Event::KeyPressed) {

				// Increase / Decrease shape amount
				if (event.key.code == sf::Keyboard::Up) {
					if (shapes_count + 1000 <= routines) {
						for (size_t i = static_cast<size_t>(shapes_count); i < static_cast<size_t>(shapes_count) + 1000; ++i) {
							sf::RectangleShape* shape = new sf::RectangleShape;
							shape->setSize(shape_size);
							shape->setFillColor(sf::Color::Blue);
							shape->setOrigin(shape->getSize().x / 2.f, shape->getSize().y / 2.f);
							shape->setPosition(randomPosition());
							shapes.emplace_back(shape);

							movementManager->linkMovementRoutine(*shape, "SM" + std::to_string(i % routines));
							if (!random_ease_type)
								movementManager->getMovementRoutine(*shape, "SM" + std::to_string(i % routines))->setFunction(easeFunctions::getTmovement(current_ease_type));

							movementManager->startMovementRoutine(*shape, "SM" + std::to_string(i % routines));
						}
						shapes_count += 1000;

						if (synchronization) {
							movementManager->setDelayBefore(0.5f, true);
							movementManager->setMotionDuration(5.f, true);
							movementManager->setDelayAfter(0.5f, true);
						}
						else {
							std::string name{};
							for (size_t i = static_cast<size_t>(shapes_count - 1000); i < static_cast<size_t>(shapes_count); ++i) {
								name = "SM" + std::to_string(i % routines);
								movementManager->setDelayBefore(name, randomDelay());
								movementManager->setMotionDuration(name, randomDelay(5.f, 10.f));
								movementManager->setDelayAfter(name, randomDelay());
							}
						}

						shapes_count_text.setString("Shapes count: " + std::to_string(shapes_count));
					}
				}
				else if (event.key.code == sf::Keyboard::Down) {
					if (shapes_count - 1000 >= 0) {
						for (size_t i = static_cast<size_t>(shapes_count) - 1000; i < static_cast<size_t>(shapes_count); ++i)
							movementManager->unlinkMovementRoutine(shapes[i], "SM" + std::to_string(i % routines));

						shapes_count -= 1000;
						for (size_t i = shapes_count; i < shapes.size(); ++i)
							delete shapes[i];
						shapes.resize(shapes_count);

						shapes_count_text.setString("Shapes count: " + std::to_string(shapes_count));
					}
				}

				// Toggle movement synchronization
				if (event.key.code == sf::Keyboard::S) {
					synchronization = !synchronization;

					if (synchronization) {
						synchronization_text.setString("Synchronization: ON");
						movementManager->setDelayBefore(0.5f, true);
						movementManager->setMotionDuration(5.f, true);
						movementManager->setDelayAfter(0.5f, true);
					}
					else {
						synchronization_text.setString("Synchronization: OFF");
						for (size_t i = 0; i < static_cast<size_t>(shapes_count); ++i) {
							std::string name{ "SM" + std::to_string(i % routines) };
							movementManager->setDelayBefore(name, randomDelay());
							movementManager->setMotionDuration(name, randomDelay(5.f, 10.f));
							movementManager->setDelayAfter(name, randomDelay());
						}
					}
				}

				// Change current function
				if (event.key.code == sf::Keyboard::Left) {
					random_ease_type = false;
					current_ease_type--;
					if (current_ease_type < 1)
						current_ease_type = easeTypeSize - 1;

					movementManager->setFunction(easeFunctions::getTmovement(current_ease_type));
					movementManager->resetRoutines();
					current_function_text.setString("Current function: " + easeFunctions::getFunctionName(current_ease_type));
				}
				else if (event.key.code == sf::Keyboard::Right) {
					random_ease_type = false;
					current_ease_type++;
					if (current_ease_type >= easeTypeSize)
						current_ease_type = 1;

					movementManager->setFunction(easeFunctions::getTmovement(current_ease_type));
					movementManager->resetRoutines();
					current_function_text.setString("Current function: " + easeFunctions::getFunctionName(current_ease_type));
				}

				// Reset demo
				if (event.key.code == sf::Keyboard::Q) {
					current_ease_type = 1;
					shape_size = sf::Vector2f(2.f, 2.f);
					int shape_count_diff = default_shapes_count - shapes_count;


					if (shape_count_diff > 0) {
						for (size_t i = static_cast<size_t>(shapes_count); i < static_cast<size_t>(default_shapes_count); ++i) {
							sf::RectangleShape* shape = new sf::RectangleShape;
							shape->setSize(shape_size);
							shape->setFillColor(sf::Color::Blue);
							shape->setOrigin(shape->getSize().x / 2.f, shape->getSize().y / 2.f);
							shape->setPosition(randomPosition());
							shapes.emplace_back(shape);
						}
					}
					else if (shape_count_diff < 0) {
						for (size_t i = static_cast<size_t>(default_shapes_count); i < static_cast<size_t>(shapes_count); ++i) {
							movementManager->unlinkMovementRoutine(shapes[i], "SM" + std::to_string(i % routines));
							delete shapes[i];
						}
						shapes.resize(shapes_count);
					}

					shapes_count = default_shapes_count;
					shapes_count_text.setString("Shapes count: " + std::to_string(shapes_count));
					shape_size_text.setString("Shape size: " + std::format("{:.2f}", shape_size.x));

					random_ease_type = true;
					synchronization = false;

					movementManager->setFunction(easeFunctions::getTmovement(randomEaseType()));
					for (size_t i = 0; i < static_cast<size_t>(shapes_count); ++i) {
						shapes[i]->setSize(shape_size);
						shapes[i]->setOrigin(shapes[i]->getSize().x / 2.f, shapes[i]->getSize().y / 2.f);
						
						movementManager->setDelayBefore(*shapes[i], randomDelay(), true);
						movementManager->setMotionDuration(*shapes[i], randomDelay(5.f, 10.f), true);
						movementManager->setDelayAfter(*shapes[i], randomDelay(), true);
					}
					movementManager->resetRoutines();
					current_function_text.setString("Current function: Random");
				}

				// Decrease / increase shape size
				if (event.key.code == sf::Keyboard::LBracket) {
					shape_size.x -= 0.2f;
					shape_size.y -= 0.2f;

					if (shape_size.x < 1.f) {
						shape_size.x = 1.f;
						shape_size.y = 1.f;
					}

					for (size_t i = 0; i < static_cast<size_t>(shapes_count); ++i) {
						shapes[i]->setSize(shape_size);
						shapes[i]->setOrigin(shapes[i]->getSize().x / 2.f, shapes[i]->getSize().y / 2.f);
					}

					shape_size_text.setString("Shape size: " + std::format("{:.2f}", shape_size.x));
				}
				else if (event.key.code == sf::Keyboard::RBracket) {
					shape_size.x += 0.2f;
					shape_size.y += 0.2f;

					if (shape_size.x > 20.f) {
						shape_size.x = 20.f;
						shape_size.y = 20.f;
					}

					for (size_t i = 0; i < static_cast<size_t>(shapes_count); ++i) {
						shapes[i]->setSize(shape_size);
						shapes[i]->setOrigin(shapes[i]->getSize().x / 2.f, shapes[i]->getSize().y / 2.f);
					}

					shape_size_text.setString("Shape size: " + std::format("{:.2f}", shape_size.x));
				}

				// Toggle GUI
				if (event.key.code == sf::Keyboard::G)
					gui = !gui;

				// Show controls
				if (event.key.code == sf::Keyboard::C)
					side_panel.toggle();
			}
		}

		window.clear();
		for (const auto& shape : shapes)
			window.draw(*shape);

		if (gui) {
			window.draw(controls_text);
			window.draw(shapes_count_text);
			window.draw(average_time_text);
			window.draw(current_function_text);
			window.draw(shape_size_text);
			window.draw(synchronization_text);
		}

		side_panel.draw(window);

		window.display();
	}

	delete movementManager;

	for (size_t i = 0; i < shapes.size(); ++i)
		delete shapes[i];
	shapes.clear();
}