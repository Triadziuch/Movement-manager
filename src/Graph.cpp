#pragma once
#include "Graph.h"

// Private functions
void Graph::drawFunction()
{
	if (m_used_function == nullptr)
		return;

	if (m_function_vertexes.getVertexCount() != 0)
		m_function_vertexes.clear();

	m_function_vertexes.setPrimitiveType(sf::LineStrip);
	m_function_vertexes.resize(static_cast<size_t>(m_precision + 1));
	
	for (size_t i = 0; i <= static_cast<size_t>(m_precision); i++) {
		m_function_vertexes[i].position = sf::Vector2f(
					m_position.x + static_cast<float>(i) * m_scale.x,
			m_position.y - m_precision * m_scale.y * static_cast<float>(m_used_function(static_cast<double>(i) / static_cast<double>(m_precision))));
		
		m_function_vertexes[i].color = m_functioncolor;
	}
}

void Graph::drawOX()
{
	m_OX[0].position = m_position;
	m_OX[1].position = sf::Vector2f(m_position.x + m_size.x, m_position.y);

	for (int i = 0; i < 2; i++)
		m_OX[i].color = m_OXcolor;
}

void Graph::drawOY()
{
	m_OY[0].position = m_position;
	m_OY[1].position = sf::Vector2f(m_position.x, m_position.y - m_size.y);

	for (int i = 0; i < 2; i++)
		m_OY[i].color = m_OYcolor;
}

// Constructors & Destructors
Graph::Graph()
{
	if (!m_font.loadFromFile("Fonts/Helvetica Regular.otf"))
		printf("ERROR: Couldn't load font file: Fonts/Helvetica Regular.otf\n");

	m_OX_label.setFont(m_font);
	m_OX_label.setCharacterSize(m_labelsize);
	m_OX_label.setString(m_OX_label_string);
	m_OX_label.setFillColor(sf::Color::White);

	m_OY_label.setFont(m_font);
	m_OY_label.setCharacterSize(m_labelsize);
	m_OY_label.setString(m_OY_label_string);
	m_OX_label.setFillColor(sf::Color::White);
}

Graph::Graph(sf::Vector2f pos, sf::Vector2f size, int precision, double(*used_function)(double))
{
	m_position = pos;
	m_used_function = used_function;
	m_size = size;
	m_precision = precision < 0 ? 0 : precision;
	m_scale = { size.x / static_cast<float>(precision), size.y / static_cast<float>(precision) };

	if (used_function != nullptr) {
		drawFunction();
		drawOX();
		drawOY();
	}

	if (!m_font.loadFromFile("Fonts/Arial.ttf"))
		printf("ERROR: Couldn't load font file: Fonst/Arial.ttf\n");

	m_OX_label.setFont(m_font);
	m_OX_label.setCharacterSize(m_labelsize);
	m_OX_label.setString(m_OX_label_string);
	m_OX_label.setFillColor(sf::Color::White);
	m_OX_label.setPosition(m_position.x + size.x + 10.f, m_position.y - m_OX_label.getGlobalBounds().height);

	m_OY_label.setFont(m_font);
	m_OY_label.setCharacterSize(m_labelsize);
	m_OY_label.setString(m_OY_label_string);
	m_OX_label.setFillColor(sf::Color::White);
	m_OY_label.setPosition(m_position.x - m_OY_label.getGlobalBounds().width / 2.f, m_position.y - size.y - m_OY_label.getGlobalBounds().height - 30.f);
}

Graph::Graph(const Graph& _graph)
{
	// Axis variables
	for (size_t i = 0; i < 2; ++i) {
		m_OX[i] = _graph.m_OX[i];
		m_OY[i] = _graph.m_OY[i];
	}
	m_OXcolor	 = _graph.m_OXcolor;
	m_OYcolor	 = _graph.m_OYcolor;
	m_axis_visible = _graph.m_axis_visible;

	m_font = _graph.m_font;
	m_labelsize = _graph.m_labelsize;
	m_OX_label_string = _graph.m_OX_label_string;
	m_OY_label_string = _graph.m_OY_label_string;
	m_OX_label = _graph.m_OX_label;
	m_OY_label = _graph.m_OY_label;

	// Function variables
	m_function_vertexes	 = _graph.m_function_vertexes;
	m_functioncolor	 = _graph.m_functioncolor;
	m_precision			 = _graph.m_precision;
	m_used_function		 = _graph.m_used_function;

	// General variables
	m_position	= _graph.m_position;
	m_size		= _graph.m_size;
	m_scale		= _graph.m_scale;
}

Graph::~Graph()
{
}

// Public functions
void Graph::setOXColor(sf::Color color)
{
	m_OXcolor = color;
	for (auto& vertex: m_OX)
		vertex.color = m_OXcolor;
}

void Graph::setOYColor(sf::Color color)
{
	m_OYcolor = color;
	for (auto& vertex : m_OY)
		vertex.color = m_OYcolor;
}

void Graph::setLineColor(sf::Color color)
{
	if (m_functioncolor != color) return;

	m_functioncolor = color;
	for (size_t i = 0; i < m_function_vertexes.getVertexCount(); i++)
		m_function_vertexes[i].color = m_functioncolor;
}

void Graph::setSize(sf::Vector2f size)
{
	if (m_size == size) return;

	m_size = size;
	m_scale = { size.x / static_cast<float>(m_precision), size.y / static_cast<float>(m_precision) };

	drawFunction();
	drawOX();
	drawOY();

	m_OX_label.setPosition(m_position.x + size.x + 10.f, m_position.y - m_OX_label.getGlobalBounds().height);
	m_OY_label.setPosition(m_position.x - m_OY_label.getGlobalBounds().width / 2.f, m_position.y - size.y - m_OY_label.getGlobalBounds().height - 30.f);
}

void Graph::setPrecision(int precision)
{
	if (m_precision == precision) return;

	m_precision = precision < 0 ? 0 : precision;
	m_scale = { m_size.x / static_cast<float>(precision), m_size.y / static_cast<float>(precision) };

	drawFunction();
	drawOX();
	drawOY();
}

void Graph::setPosition(sf::Vector2f pos)
{
	if (m_position == pos) return;

	const sf::Vector2f offset = pos - m_position;
	m_position = pos;

	for (size_t i = 0; i < m_function_vertexes.getVertexCount(); i++)
		m_function_vertexes[i].position += offset;

	for (size_t i = 0; i < 2; i++) {
		m_OX[i].position += offset;
		m_OY[i].position += offset;
	}

	m_OX_label.setPosition(m_OX_label.getPosition() + offset);
	m_OY_label.setPosition(m_OY_label.getPosition() + offset);
}

void Graph::setFunction(double(*used_function)(double))
{
	if (m_used_function != used_function && used_function != nullptr) {
		m_used_function = used_function;
		drawFunction();
	}
}

void Graph::setOXLabelText(std::string label)
{
	if (m_OX_label_string != label) {
		m_OX_label_string = label;
		m_OX_label.setString(m_OX_label_string);
		m_OX_label.setPosition(m_position.x + m_size.x + 10.f, m_position.y - m_OX_label.getGlobalBounds().height);
	}
}

void Graph::setOYLabelText(std::string label)
{
	if (m_OY_label_string != label) {
		m_OY_label_string = label;
		m_OY_label.setString(m_OY_label_string);
		m_OY_label.setPosition(m_position.x - m_OY_label.getGlobalBounds().width / 2.f, m_position.y - m_size.y - m_OY_label.getGlobalBounds().height - 30.f);
	}
}

// Accessors
const sf::Vector2f Graph::getSize() const
{
	return m_size;
}

const int Graph::getPrecision() const
{
	return m_precision;
}

const sf::Vector2f Graph::getPosition() const
{
	return m_position;
}

double (*Graph::getFunction())(double) {
	return m_used_function;
}

const bool Graph::getAxisVisible() const
{
	return m_axis_visible;
}


// Render functions
void Graph::draw(sf::RenderWindow& window)
{
	if (m_axis_visible) {
		window.draw(m_OX, 2, sf::Lines);
		window.draw(m_OY, 2, sf::Lines);
		window.draw(m_OX_label);
		window.draw(m_OY_label);
	}
	window.draw(m_function_vertexes);
}

