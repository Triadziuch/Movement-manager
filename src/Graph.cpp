#pragma once
#include "Graph.h"

// Private functions
void Graph::drawFunction()
{
	if (m_usedFunctionPtr == nullptr)
		return;

	if (m_functionVertexes.getVertexCount() != 0)
		m_functionVertexes.clear();

	m_functionVertexes.setPrimitiveType(sf::LineStrip);
	m_functionVertexes.resize(static_cast<size_t>(m_precision + 1));
	
	for (size_t i = 0; i <= static_cast<size_t>(m_precision); i++) {
		m_functionVertexes[i].position = sf::Vector2f(
					m_position.x + static_cast<float>(i) * m_scale.x,
			m_position.y - m_precision * m_scale.y * static_cast<float>(m_usedFunctionPtr(static_cast<double>(i) / static_cast<double>(m_precision))));
		
		m_functionVertexes[i].color = m_functionColor;
	}
}

void Graph::drawOX()
{
	m_OX[0].position = m_position;
	m_OX[1].position = sf::Vector2f(m_position.x + m_size.x, m_position.y);

	for (int i = 0; i < 2; i++)
		m_OX[i].color = m_colorOX;
}

void Graph::drawOY()
{
	m_OY[0].position = m_position;
	m_OY[1].position = sf::Vector2f(m_position.x, m_position.y - m_size.y);

	for (int i = 0; i < 2; i++)
		m_OY[i].color = m_colorOY;
}

// Constructors & Destructors
Graph::Graph()
{
	if (!m_font.loadFromFile("Fonts/Helvetica Regular.otf"))
		printf("ERROR: Couldn't load font file: Fonts/Helvetica Regular.otf\n");

	m_labelOX.setFont(m_font);
	m_labelOX.setCharacterSize(m_labelSize);
	m_labelOX.setString(m_labelOX_str);
	m_labelOX.setFillColor(sf::Color::White);

	m_labelOY.setFont(m_font);
	m_labelOY.setCharacterSize(m_labelSize);
	m_labelOY.setString(m_labelOY_str);
	m_labelOX.setFillColor(sf::Color::White);
}

Graph::Graph(sf::Vector2f pos, sf::Vector2f size, int precision, double(*used_function)(double))
{
	m_position = pos;
	m_usedFunctionPtr = used_function;
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

	m_labelOX.setFont(m_font);
	m_labelOX.setCharacterSize(m_labelSize);
	m_labelOX.setString(m_labelOX_str);
	m_labelOX.setFillColor(sf::Color::White);
	m_labelOX.setPosition(m_position.x + size.x + 10.f, m_position.y - m_labelOX.getGlobalBounds().height);

	m_labelOY.setFont(m_font);
	m_labelOY.setCharacterSize(m_labelSize);
	m_labelOY.setString(m_labelOY_str);
	m_labelOX.setFillColor(sf::Color::White);
	m_labelOY.setPosition(m_position.x - m_labelOY.getGlobalBounds().width / 2.f, m_position.y - size.y - m_labelOY.getGlobalBounds().height - 30.f);
}

Graph::Graph(const Graph& _graph)
{
	// Axis variables
	for (size_t i = 0; i < 2; ++i) {
		m_OX[i] = _graph.m_OX[i];
		m_OY[i] = _graph.m_OY[i];
	}
	m_colorOX	 = _graph.m_colorOX;
	m_colorOY	 = _graph.m_colorOY;
	m_isAxisVisible = _graph.m_isAxisVisible;

	m_font = _graph.m_font;
	m_labelSize = _graph.m_labelSize;
	m_labelOX_str = _graph.m_labelOX_str;
	m_labelOY_str = _graph.m_labelOY_str;
	m_labelOX = _graph.m_labelOX;
	m_labelOY = _graph.m_labelOY;

	// Function variables
	m_functionVertexes	 = _graph.m_functionVertexes;
	m_functionColor	 = _graph.m_functionColor;
	m_precision			 = _graph.m_precision;
	m_usedFunctionPtr		 = _graph.m_usedFunctionPtr;

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
	m_colorOX = color;
	for (auto& vertex: m_OX)
		vertex.color = m_colorOX;
}

void Graph::setOYColor(sf::Color color)
{
	m_colorOY = color;
	for (auto& vertex : m_OY)
		vertex.color = m_colorOY;
}

void Graph::setLineColor(sf::Color color)
{
	m_functionColor = color;
	for (size_t i = 0; i < m_functionVertexes.getVertexCount(); i++)
		m_functionVertexes[i].color = m_functionColor;
}

void Graph::setSize(sf::Vector2f size)
{
	m_size = size;
	m_scale = { size.x / static_cast<float>(m_precision), size.y / static_cast<float>(m_precision) };

	drawFunction();
	drawOX();
	drawOY();

	m_labelOX.setPosition(m_position.x + size.x + 10.f, m_position.y - m_labelOX.getGlobalBounds().height);
	m_labelOY.setPosition(m_position.x - m_labelOY.getGlobalBounds().width / 2.f, m_position.y - size.y - m_labelOY.getGlobalBounds().height - 30.f);
}

void Graph::setPrecision(int precision)
{
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

	for (size_t i = 0; i < m_functionVertexes.getVertexCount(); i++)
		m_functionVertexes[i].position += offset;

	for (size_t i = 0; i < 2; i++) {
		m_OX[i].position += offset;
		m_OY[i].position += offset;
	}

	m_labelOX.setPosition(m_labelOX.getPosition() + offset);
	m_labelOY.setPosition(m_labelOY.getPosition() + offset);
}

void Graph::setFunction(double(*used_function)(double))
{
	if (m_usedFunctionPtr != used_function && used_function != nullptr) {
		m_usedFunctionPtr = used_function;
		drawFunction();
	}
}

void Graph::setOXLabelText(std::string label)
{
	if (m_labelOX_str != label) {
		m_labelOX_str = label;
		m_labelOX.setString(m_labelOX_str);
		m_labelOX.setPosition(m_position.x + m_size.x + 10.f, m_position.y - m_labelOX.getGlobalBounds().height);
	}
}

void Graph::setOYLabelText(std::string label)
{
	if (m_labelOY_str != label) {
		m_labelOY_str = label;
		m_labelOY.setString(m_labelOY_str);
		m_labelOY.setPosition(m_position.x - m_labelOY.getGlobalBounds().width / 2.f, m_position.y - m_size.y - m_labelOY.getGlobalBounds().height - 30.f);
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
	return m_usedFunctionPtr;
}

const bool Graph::getAxisVisible() const
{
	return m_isAxisVisible;
}


// Render functions
void Graph::draw(sf::RenderWindow& window)
{
	if (m_isAxisVisible) {
		window.draw(m_OX, 2, sf::Lines);
		window.draw(m_OY, 2, sf::Lines);
		window.draw(m_labelOX);
		window.draw(m_labelOY);
	}
	window.draw(m_functionVertexes);
}

