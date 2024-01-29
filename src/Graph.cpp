#include "Graph.h"

//sf::VertexArray linearray(sf::LineStrip, precision + 1);
//for (int i = 0; i <= precision; i++) {
//	linearray[i].position = sf::Vector2f(zero_point.x + i * scale, zero_point.y - precision * scale * inOutElastic(static_cast<double>(i) / static_cast<double>(precision)));
//	linearray[i].color = sf::Color::Blue;
//}
//
//sf::Vertex OX[] = {
//	sf::Vertex(sf::Vector2f(linearray[0].position.x, zero_point.y)),
//	sf::Vertex(sf::Vector2f(linearray[precision].position.x, zero_point.y))
//};
//
//sf::Vertex OY[] = {
//	sf::Vertex(sf::Vector2f(zero_point.x, linearray[0].position.y)),
//	sf::Vertex(sf::Vector2f(zero_point.x, linearray[precision].position.y))
//};

void Graph::drawFunction()
{
	if (this->linearray.getVertexCount() != 0)
		this->linearray.clear();

	this->linearray.setPrimitiveType(sf::LineStrip);
	this->linearray.resize(static_cast<size_t>(this->precision + 1));

	for (size_t i = 0; i <= precision; i++) {
		this->linearray[i].position = sf::Vector2f(this->position.x + static_cast<float>(i) * this->scale.x, this->position.y - this->precision * this->scale.y * this->used_function(static_cast<double>(i) / static_cast<double>(this->precision)));
		this->linearray[i].color = this->line_color;
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
}

Graph::Graph(const Graph& _graph)
{
}

Graph::~Graph()
{
}

void Graph::draw(sf::RenderWindow& window)
{
	window.draw(this->OX, 2, sf::Lines);
	window.draw(this->OY, 2, sf::Lines);
	window.draw(this->linearray);
}

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
	if (this->line_color != _color) {
		this->line_color = _color;

		for (size_t i = 0; i < this->linearray.getVertexCount(); i++)
			this->linearray[i].color = this->line_color;
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
		
		for (size_t i = 0; i < this->linearray.getVertexCount(); i++)
			this->linearray[i].position += offset;
		
		for (int i = 0; i < 2; i++) {
			this->OX[i].position += offset;
			this->OY[i].position += offset;
		}
	}
}

void Graph::setFunction(double(*_used_function)(double))
{
	if (this->used_function != _used_function && _used_function != nullptr) {
		this->used_function = _used_function;
		this->drawFunction();
	}
}