#pragma once
#include "Graph.h"

// Private functions
void Graph::drawFunction()
{
	if (used_function == nullptr)
		return;

	if (function_vertexes.getVertexCount() != 0)
		function_vertexes.clear();

	function_vertexes.setPrimitiveType(sf::LineStrip);
	function_vertexes.resize(static_cast<size_t>(precision + 1));
	
	for (size_t i = 0; i <= static_cast<size_t>(precision); i++) {
		function_vertexes[i].position = sf::Vector2f(
					position.x + static_cast<float>(i) * scale.x,
					position.y - precision * scale.y * static_cast<float>(used_function(static_cast<double>(i) / static_cast<double>(precision))));
		
		function_vertexes[i].color = function_color;
	}
}

void Graph::drawOX()
{
	OX[0].position = position;
	OX[1].position = sf::Vector2f(position.x + size.x, position.y);

	for (int i = 0; i < 2; i++)
		OX[i].color = OX_color;
}

void Graph::drawOY()
{
	OY[0].position = position;
	OY[1].position = sf::Vector2f(position.x, position.y - size.y);

	for (int i = 0; i < 2; i++)
		OY[i].color = OY_color;
}

// Constructors & Destructors
Graph::Graph()
{
	if (!font.loadFromFile("Fonts/Helvetica Regular.otf"))
		printf("ERROR: Couldn't load font file: Fonts/Helvetica Regular.otf\n");

	OX_label.setFont(font);
	OX_label.setCharacterSize(label_size);
	OX_label.setString(OX_label_string);
	OX_label.setFillColor(sf::Color::White);

	OY_label.setFont(font);
	OY_label.setCharacterSize(label_size);
	OY_label.setString(OY_label_string);
	OX_label.setFillColor(sf::Color::White);
}

Graph::Graph(sf::Vector2f _position, sf::Vector2f _size, int _precision, double(*_used_function)(double))
{
	position = _position;
	used_function = _used_function;
	size = _size;
	precision = _precision < 0 ? 0 : _precision;
	scale = { size.x / static_cast<float>(precision), size.y / static_cast<float>(precision) };

	if (_used_function != nullptr) {
		drawFunction();
		drawOX();
		drawOY();
	}

	if (!font.loadFromFile("Fonts/Arial.ttf"))
		printf("ERROR: Couldn't load font file: Fonst/Arial.ttf\n");

	OX_label.setFont(font);
	OX_label.setCharacterSize(label_size);
	OX_label.setString(OX_label_string);
	OX_label.setFillColor(sf::Color::White);
	OX_label.setPosition(position.x + size.x + 10.f, position.y - OX_label.getGlobalBounds().height);

	OY_label.setFont(font);
	OY_label.setCharacterSize(label_size);
	OY_label.setString(OY_label_string);
	OX_label.setFillColor(sf::Color::White);
	OY_label.setPosition(position.x - OY_label.getGlobalBounds().width / 2.f, position.y - size.y - OY_label.getGlobalBounds().height - 30.f);
}

Graph::Graph(const Graph& _graph)
{
	// Axis variables
	for (size_t i = 0; i < 2; ++i) {
		OX[i] = _graph.OX[i];
		OY[i] = _graph.OY[i];
	}
	OX_color		 = _graph.OX_color;
	OY_color		 = _graph.OY_color;
	axis_visible	 = _graph.axis_visible;

	font = _graph.font;
	label_size = _graph.label_size;
	OX_label_string = _graph.OX_label_string;
	OY_label_string = _graph.OY_label_string;
	OX_label = _graph.OX_label;
	OY_label = _graph.OY_label;

	// Function variables
	function_vertexes	 = _graph.function_vertexes;
	function_color	 = _graph.function_color;
	precision			 = _graph.precision;
	used_function		 = _graph.used_function;

	// General variables
	position	= _graph.position;
	size		= _graph.size;
	scale		= _graph.scale;
}

Graph::~Graph()
{
}

// Public functions
void Graph::setOXColor(sf::Color _color)
{
	if (OX_color != _color) {
		OX_color = _color;

		for (int i = 0; i < 2; i++)
			OX[i].color = OX_color;
	}
}

void Graph::setOYColor(sf::Color _color)
{
	if (OY_color != _color) {
		OY_color = _color;

		for (int i = 0; i < 2; i++)
			OY[i].color = OY_color;
	}
}

void Graph::setLineColor(sf::Color _color)
{
	if (function_color != _color) {
		function_color = _color;

		for (size_t i = 0; i < function_vertexes.getVertexCount(); i++)
			function_vertexes[i].color = function_color;
	}
}

void Graph::setSize(sf::Vector2f _size)
{
	if (size != _size) {
		size = _size;
		scale = { size.x / static_cast<float>(precision), size.y / static_cast<float>(precision) };

		drawFunction();
		drawOX();
		drawOY();

		OX_label.setPosition(position.x + size.x + 10.f, position.y - OX_label.getGlobalBounds().height);
		OY_label.setPosition(position.x - OY_label.getGlobalBounds().width / 2.f, position.y - size.y - OY_label.getGlobalBounds().height - 30.f);
	}
}

void Graph::setPrecision(int _precision)
{
	if (precision != _precision) {
		precision = _precision < 0 ? 0 : _precision;
		scale = { size.x / static_cast<float>(precision), size.y / static_cast<float>(precision) };

		drawFunction();
		drawOX();
		drawOY();
	}
}

void Graph::setPosition(sf::Vector2f _position)
{
	if (position != _position) {

		sf::Vector2f offset = _position - position;
		position = _position;
		
		for (size_t i = 0; i < function_vertexes.getVertexCount(); i++)
			function_vertexes[i].position += offset;
		
		for (int i = 0; i < 2; i++) {
			OX[i].position += offset;
			OY[i].position += offset;
		}

		OX_label.setPosition(OX_label.getPosition() + offset);
		OY_label.setPosition(OY_label.getPosition() + offset);
	}
}

void Graph::setFunction(double(*_used_function)(double))
{
	if (used_function != _used_function && _used_function != nullptr) {
		used_function = _used_function;
		drawFunction();
	}
}

void Graph::setOXLabelText(std::string _label)
{
	if (OX_label_string != _label) {
		OX_label_string = _label;
		OX_label.setString(OX_label_string);
		OX_label.setPosition(position.x + size.x + 10.f, position.y - OX_label.getGlobalBounds().height);
	}
}

void Graph::setOYLabelText(std::string _label)
{
	if (OY_label_string != _label) {
		OY_label_string = _label;
		OY_label.setString(OY_label_string);
		OY_label.setPosition(position.x - OY_label.getGlobalBounds().width / 2.f, position.y - size.y - OY_label.getGlobalBounds().height - 30.f);
	}
}

// Render functions
void Graph::draw(sf::RenderWindow& window)
{
	if (axis_visible) {
		window.draw(OX, 2, sf::Lines);
		window.draw(OY, 2, sf::Lines);
		window.draw(OX_label);
		window.draw(OY_label);
	}
	window.draw(function_vertexes);
}