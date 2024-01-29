#pragma once
#include "easeFunctions.cpp"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>

class Graph {
private:
	// Private variables
	sf::VertexArray linearray{};
	sf::Vertex OX[2]{}, OY[2]{};
	int precision = 100;
	double(*used_function)(double) {};
	sf::Vector2f position{};
	sf::Color OX_color = sf::Color::White, OY_color = sf::Color::White, line_color = sf::Color::White;
	sf::Vector2f size{};
	sf::Vector2f scale{};


	// Private functions
	void drawFunction();
	void drawOX();
	void drawOY();

	

public:

	Graph() {};
	Graph(sf::Vector2f _position, sf::Vector2f _size, int _precision, double(*_used_function)(double));
	Graph(const Graph& _graph);
	~Graph();

	// Public functions
	void setOXColor(sf::Color _color);
	void setOYColor(sf::Color _color);
	void setLineColor(sf::Color _color);

	void setSize(sf::Vector2f _size);
	void setSize(float _width, float _height) { this->setSize(sf::Vector2f(_width, _height)); }
	void setPrecision(int _precision);
	void setPosition(sf::Vector2f _position);
	void setPosition(float _x, float _y) { this->setPosition(sf::Vector2f(_x, _y)); }
	void setFunction(double(*_used_function)(double));

	// Accessors
	sf::Vector2f getSize() { return this->size; }
	sf::Vector2f getPosition() { return this->position; }
	int getPrecision() { return this->precision; }

	void draw(sf::RenderWindow& window);
};