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
	sf::Color OX_color = sf::Color::White, OY_color = sf::Color::White;
	bool axis_visible = true;
	sf::Text OX_label{}, OY_label{};
	std::string OX_label_string{"t"}, OY_label_string{"x"};
	unsigned label_size = 30u;
	sf::Font font{};

	// Function variables
	sf::VertexArray function_vertexes{};
	sf::Color		function_color	= sf::Color::White;
	int				precision		= 100;
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
	Graph(sf::Vector2f _position, sf::Vector2f _size, int _precision, double(*_used_function)(double));
	Graph(const Graph& _graph);
	~Graph();

	// Public functions
	void setOXColor(sf::Color _color);
	void setOYColor(sf::Color _color);
	void setLineColor(sf::Color _color);

	void setSize(sf::Vector2f _size);
	void setSize(float _width, float _height) { setSize(sf::Vector2f(_width, _height)); }

	void setPrecision(int _precision);

	void setPosition(sf::Vector2f _position);
	void setPosition(float _x, float _y) { setPosition(sf::Vector2f(_x, _y)); }

	void setFunction(double(*_used_function)(double));

	void setAxisVisible(bool _visible) { axis_visible = _visible; }
	void toggleAxisVisible() { axis_visible = !axis_visible; }

	void setOXLabelText(std::string _label);
	void setOYLabelText(std::string _label);


	// Accessors
	const sf::Vector2f getSize()	 { return size; }
	const int getPrecision()		 { return precision; }
	const sf::Vector2f getPosition() { return position; }
	double (*getFunction())(double)	 { return used_function; }
	const bool getAxisVisible()	     { return axis_visible; }

	// Render functions
	void draw(sf::RenderWindow& window);
};