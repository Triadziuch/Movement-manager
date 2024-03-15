#pragma once
#include "easeFunctions.h"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>

class Graph {
private:
	// Private variables

	// Axis variables
	sf::Vertex m_OX[2]{}, m_OY[2]{};
	sf::Color m_colorOX{ sf::Color::White }, m_colorOY{ sf::Color::White };
	bool m_isAxisVisible{ true };
	sf::Text m_labelOX{}, m_labelOY{};
	std::string m_labelOX_str{"t"}, m_labelOY_str{"x"};
	unsigned m_labelSize{ 30u };
	sf::Font m_font{};

	// Function variables
	sf::VertexArray m_functionVertexes{};
	sf::Color		m_functionColor{ sf::Color::White };
	int				m_precision{ 100 };
	double(*m_usedFunctionPtr)(double) {};

	// General variables
	sf::Vector2f m_position{};
	sf::Vector2f m_size{};
	sf::Vector2f m_scale{};

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

	void setAxisVisible(bool visible) { m_isAxisVisible = visible; }
	void toggleAxisVisible() { m_isAxisVisible = !m_isAxisVisible; }

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