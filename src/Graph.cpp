#pragma once
#include "Graph.h"

// Private functions
void Graph::drawFunction()
{
	if (this->used_function == nullptr)
		return;

	if (this->function_vertexes.getVertexCount() != 0)
		this->function_vertexes.clear();

	this->function_vertexes.setPrimitiveType(sf::LineStrip);
	this->function_vertexes.resize(static_cast<size_t>(this->precision + 1));
	
	for (size_t i = 0; i <= static_cast<size_t>(precision); i++) {
		this->function_vertexes[i].position = sf::Vector2f(
					this->position.x + static_cast<float>(i) * this->scale.x,
					this->position.y - this->precision * this->scale.y * static_cast<float>(this->used_function(static_cast<double>(i) / static_cast<double>(this->precision))));
		
		this->function_vertexes[i].color = this->function_color;
	}
}

void Graph::drawOX()
{
	this->OX[0].position = this->position;
	this->OX[1].position = sf::Vector2f(this->position.x + this->size.x, this->position.y);

	for (int i = 0; i < 2; i++)
		this->OX[i].color = this->OX_color;
}

void Graph::drawOY()
{
	this->OY[0].position = this->position;
	this->OY[1].position = sf::Vector2f(this->position.x, this->position.y - this->size.y);

	for (int i = 0; i < 2; i++)
		this->OY[i].color = this->OY_color;
}

// Constructors & Destructors
Graph::Graph()
{
	if (!this->font.loadFromFile("Fonts/Arial.ttf"))
		printf("ERROR: Couldn't load font file: Fonts/Arial.ttf\n");

	this->OX_label.setFont(this->font);
	this->OX_label.setCharacterSize(this->label_size);
	this->OX_label.setString(this->OX_label_string);
	this->OX_label.setFillColor(sf::Color::White);

	this->OY_label.setFont(this->font);
	this->OY_label.setCharacterSize(this->label_size);
	this->OY_label.setString(this->OY_label_string);
	this->OX_label.setFillColor(sf::Color::White);
}

Graph::Graph(sf::Vector2f _position, sf::Vector2f _size, int _precision, double(*_used_function)(double))
{
	this->position = _position;
	this->used_function = _used_function;
	this->size = _size;
	this->precision = _precision < 0 ? 0 : _precision;
	this->scale = { this->size.x / static_cast<float>(this->precision), this->size.y / static_cast<float>(this->precision) };

	if (_used_function != nullptr) {
		this->drawFunction();
		this->drawOX();
		this->drawOY();
	}

	if (!this->font.loadFromFile("Fonts/Arial.ttf"))
		printf("ERROR: Couldn't load font file: Fonst/Arial.ttf\n");

	this->OX_label.setFont(this->font);
	this->OX_label.setCharacterSize(this->label_size);
	this->OX_label.setString(this->OX_label_string);
	this->OX_label.setFillColor(sf::Color::White);
	this->OX_label.setPosition(this->position.x + this->size.x + 10.f, this->position.y - this->OX_label.getGlobalBounds().height);

	this->OY_label.setFont(this->font);
	this->OY_label.setCharacterSize(this->label_size);
	this->OY_label.setString(this->OY_label_string);
	this->OX_label.setFillColor(sf::Color::White);
	this->OY_label.setPosition(this->position.x - this->OY_label.getGlobalBounds().width / 2.f, this->position.y - this->size.y - this->OY_label.getGlobalBounds().height - 30.f);
}

Graph::Graph(const Graph& _graph)
{
	// Axis variables
	for (size_t i = 0; i < 2; ++i) {
		this->OX[i] = _graph.OX[i];
		this->OY[i] = _graph.OY[i];
	}
	this->OX_color		 = _graph.OX_color;
	this->OY_color		 = _graph.OY_color;
	this->axis_visible	 = _graph.axis_visible;

	this->font = _graph.font;
	this->label_size = _graph.label_size;
	this->OX_label_string = _graph.OX_label_string;
	this->OY_label_string = _graph.OY_label_string;
	this->OX_label = _graph.OX_label;
	this->OY_label = _graph.OY_label;

	// Function variables
	this->function_vertexes	 = _graph.function_vertexes;
	this->function_color	 = _graph.function_color;
	this->precision			 = _graph.precision;
	this->used_function		 = _graph.used_function;

	// General variables
	this->position	= _graph.position;
	this->size		= _graph.size;
	this->scale		= _graph.scale;
}

Graph::~Graph()
{
}

// Public functions
void Graph::setOXColor(sf::Color _color)
{
	if (this->OX_color != _color) {
		this->OX_color = _color;

		for (int i = 0; i < 2; i++)
			this->OX[i].color = this->OX_color;
	}
}

void Graph::setOYColor(sf::Color _color)
{
	if (this->OY_color != _color) {
		this->OY_color = _color;

		for (int i = 0; i < 2; i++)
			this->OY[i].color = this->OY_color;
	}
}

void Graph::setLineColor(sf::Color _color)
{
	if (this->function_color != _color) {
		this->function_color = _color;

		for (size_t i = 0; i < this->function_vertexes.getVertexCount(); i++)
			this->function_vertexes[i].color = this->function_color;
	}
}

void Graph::setSize(sf::Vector2f _size)
{
	if (this->size != _size) {
		this->size = _size;
		this->scale = { this->size.x / static_cast<float>(this->precision), this->size.y / static_cast<float>(this->precision) };

		this->drawFunction();
		this->drawOX();
		this->drawOY();

		this->OX_label.setPosition(this->position.x + this->size.x + 10.f, this->position.y - this->OX_label.getGlobalBounds().height);
		this->OY_label.setPosition(this->position.x - this->OY_label.getGlobalBounds().width / 2.f, this->position.y - this->size.y - this->OY_label.getGlobalBounds().height - 30.f);
	}
}

void Graph::setPrecision(int _precision)
{
	if (this->precision != _precision) {
		this->precision = _precision < 0 ? 0 : _precision;
		this->scale = { this->size.x / static_cast<float>(this->precision), this->size.y / static_cast<float>(this->precision) };

		this->drawFunction();
		this->drawOX();
		this->drawOY();
	}
}

void Graph::setPosition(sf::Vector2f _position)
{
	if (this->position != _position) {

		sf::Vector2f offset = _position - this->position;
		this->position = _position;
		
		for (size_t i = 0; i < this->function_vertexes.getVertexCount(); i++)
			this->function_vertexes[i].position += offset;
		
		for (int i = 0; i < 2; i++) {
			this->OX[i].position += offset;
			this->OY[i].position += offset;
		}

		this->OX_label.setPosition(this->OX_label.getPosition() + offset);
		this->OY_label.setPosition(this->OY_label.getPosition() + offset);
	}
}

void Graph::setFunction(double(*_used_function)(double))
{
	if (this->used_function != _used_function && _used_function != nullptr) {
		this->used_function = _used_function;
		this->drawFunction();
	}
}

void Graph::setOXLabelText(std::string _label)
{
	if (this->OX_label_string != _label) {
		this->OX_label_string = _label;
		this->OX_label.setString(this->OX_label_string);
		this->OX_label.setPosition(this->position.x + this->size.x + 10.f, this->position.y - this->OX_label.getGlobalBounds().height);
	}
}

void Graph::setOYLabelText(std::string _label)
{
	if (this->OY_label_string != _label) {
		this->OY_label_string = _label;
		this->OY_label.setString(this->OY_label_string);
		this->OY_label.setPosition(this->position.x - this->OY_label.getGlobalBounds().width / 2.f, this->position.y - this->size.y - this->OY_label.getGlobalBounds().height - 30.f);
	}
}

// Render functions
void Graph::draw(sf::RenderWindow& window)
{
	if (this->axis_visible) {
		window.draw(this->OX, 2, sf::Lines);
		window.draw(this->OY, 2, sf::Lines);
		window.draw(this->OX_label);
		window.draw(this->OY_label);
	}
	window.draw(this->function_vertexes);
}