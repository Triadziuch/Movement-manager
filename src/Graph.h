#pragma once
#include "easeFunctions.h"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>

class Graph {
private:
	// Private variables

	// Axis variables
	sf::Vertex OX[2]{}, OY[2]{};
	sf::Color OXcolor{ sf::Color::White }, OYcolor{ sf::Color::White };
	bool axis_visible{ true };
	sf::Text OX_label{}, OY_label{};
	std::string OX_label_string{"t"}, OY_label_string{"x"};
	unsigned labelsize{ 30u };
	sf::Font font{};

	// Function variables
	sf::VertexArray function_vertexes{};
	sf::Color		functioncolor{ sf::Color::White };
	int				precision{ 100 };
	double(*used_function)(double) {};

	// General variables
	sf::Vector2f position{};
	sf::Vector2f size{};
	sf::Vector2f scale{};

	// Private functions
	void drawFunction();
	void drawOX();
	void drawOY();

public:
	// Constructors & Destructors
	Graph();
	Graph(sf::Vector2f pos, sf::Vector2f size, int precision, double(*used_function)(double));
	Graph(const Graph& _graph);
	~Graph();

	// Public functions
	void setOXColor(sf::Color color);
	void setOYColor(sf::Color color);
	void setLineColor(sf::Color color);

	void setSize(sf::Vector2f size);
	void setSize(float width, float height) { setSize(sf::Vector2f(width, height)); }

	void setPrecision(int precision);

	void setPosition(sf::Vector2f pos);
	void setPosition(float x, float y) { setPosition(sf::Vector2f(x, y)); }

	void setFunction(double(*used_function)(double));

	void setAxisVisible(bool visible) { axis_visible = visible; }
	void toggleAxisVisible() { axis_visible = !axis_visible; }

	void setOXLabelText(std::string label);
	void setOYLabelText(std::string label);


	// Accessors
	const sf::Vector2f getSize() const;
	const int getPrecision() const;
	const sf::Vector2f getPosition() const;
	double (*getFunction())(double);
	const bool getAxisVisible() const;

	// Render functions
	void draw(sf::RenderWindow& window);
};