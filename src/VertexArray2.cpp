#pragma once
#include "VertexArray2.h"

// = = = = = = = = = = Private update functions = = = = = = = = = = 
void VertexArray2::updateCentroid()
{
	m_needCentroidUpdate = false;

	sf::Vector2f centroid_temp{};
	const size_t vertex_count = sf::VertexArray::getVertexCount();

	if (vertex_count == 0) {
		m_centroid = centroid_temp;
		return;
	}

	for (size_t i = 0; i < vertex_count; ++i)
		centroid_temp += sf::VertexArray::operator[](i).position;
	m_centroid = centroid_temp / static_cast<float>(vertex_count);
}

void VertexArray2::updatePosition(const sf::Vector2f& new_pos)
{
	if (m_needCentroidUpdate)
		updateCentroid();

	const sf::Vector2f offset = new_pos - *m_origin;
	const size_t vertex_count = sf::VertexArray::getVertexCount();

	for (size_t i = 0; i < vertex_count; ++i) 
		sf::VertexArray::operator[](i).position += offset;

	m_centroid += offset;
	*m_origin = new_pos;
}

void VertexArray2::updateScale(const sf::Vector2f& new_scale)
{
	printf("Updating scale: %f %f\n", new_scale.x, new_scale.y);
	if (new_scale.x == 0 || new_scale.y == 0)
		return;

	if (m_needCentroidUpdate)
		updateCentroid();

	const size_t vertex_count = sf::VertexArray::getVertexCount();

	for (size_t i = 0; i < vertex_count; ++i) {
		sf::Vertex& vertex = sf::VertexArray::operator[](i);
		vertex.position.x = m_origin->x + (vertex.position.x - m_origin->x) * new_scale.x / m_scale.x;
		vertex.position.y = m_origin->y + (vertex.position.y - m_origin->y) * new_scale.y / m_scale.y;
	}

	m_scale = new_scale;
}

void VertexArray2::updateRotation(float new_rotation)
{
	if (m_needCentroidUpdate)
		updateCentroid();

	new_rotation = std::fmod(new_rotation, 360.f);
	if (new_rotation < 0)
		new_rotation += 360.f;

	const size_t vertex_count = sf::VertexArray::getVertexCount();

	const double angle = static_cast<double>(new_rotation - m_rotation) * M_RAD;
	const float sine   = static_cast<float>(sin(angle)),
		        cosine = static_cast<float>(cos(angle));

	for (size_t i = 0; i < vertex_count; ++i) {
		sf::Vertex& vertex = sf::VertexArray::operator[](i);
		const float x = vertex.position.x - m_origin->x;
		const float y = vertex.position.y - m_origin->y;

		vertex.position.x = m_origin->x + x * cosine - y * sine;
		vertex.position.y = m_origin->y + x * sine + y * cosine;
	}

	m_rotation = new_rotation;
}

// = = = = = = = = = = Constructors / Destructors = = = = = = = = = =

VertexArray2::VertexArray2() : sf::VertexArray{}, m_centroid{}, m_origin{&m_centroid}, m_scale{1, 1},
							   m_rotation{0}, m_needCentroidUpdate{false}, m_originIsCentroid{true} {}

VertexArray2::VertexArray2(sf::PrimitiveType type, size_t vertexCount) : sf::VertexArray{ type, vertexCount },
							   m_centroid{}, m_origin{ &m_centroid }, m_scale{ 1, 1 }, m_rotation{ 0 },
							   m_needCentroidUpdate{ false }, m_originIsCentroid{ true } {}

VertexArray2::VertexArray2(const VertexArray2& other) : sf::VertexArray{ other }, m_centroid{ other.m_centroid },
							   m_origin{ other.m_origin }, m_scale{ other.m_scale }, m_rotation{ other.m_rotation },
							   m_needCentroidUpdate{ other.m_needCentroidUpdate }, m_originIsCentroid{ other.m_originIsCentroid } {}

VertexArray2::~VertexArray2() { 
	if (!m_originIsCentroid) 
		delete m_origin; 
};

// = = = = = = = = = = Overriden functions = = = = = = = = = =
void VertexArray2::clear()
{
	m_needCentroidUpdate = true;
	sf::VertexArray::clear();
}

void VertexArray2::resize(std::size_t vertexCount)
{
	m_needCentroidUpdate = true;
	sf::VertexArray::resize(vertexCount);
}

void VertexArray2::append(const sf::Vertex& vertex)
{
	m_needCentroidUpdate = true;
	sf::VertexArray::append(vertex);
}

// = = = = = = = = = = Overriden operators = = = = = = = = = =
sf::Vertex& VertexArray2::operator [](std::size_t index)
{
	m_needCentroidUpdate = true;
	return sf::VertexArray::operator[](index);
}

const sf::Vertex& VertexArray2::operator [](std::size_t index) const
{
	return sf::VertexArray::operator[](index);
}

VertexArray2::operator sf::VertexArray& () {
	m_needCentroidUpdate = true;
	return *this;
}

VertexArray2::operator const sf::VertexArray& () const {
	return *this;
}

//  = = = = = = = = = = Public functions = = = = = = = = = =

void VertexArray2::setPosition(float x, float y)
{
	updatePosition(sf::Vector2f(x, y));
}

void VertexArray2::setPosition(const sf::Vector2f& position)
{
	updatePosition(position);
}

void VertexArray2::move(float offsetX, float offsetY)
{
	updatePosition(*m_origin + sf::Vector2f(offsetX, offsetY));
}

void VertexArray2::move(const sf::Vector2f& offset)
{
	updatePosition(*m_origin + offset);
}

void VertexArray2::setRotation(float angle)
{
	updateRotation(angle);
}

void VertexArray2::setScale(float factorX, float factorY)
{
	updateScale(sf::Vector2f(factorX, factorY));
}

void VertexArray2::setScale(const sf::Vector2f& factors)
{
	updateScale(factors);
}

void VertexArray2::setOrigin(float x, float y)
{
	if (!m_originIsCentroid)
		*m_origin = sf::Vector2f(x, y);
}

void VertexArray2::setOrigin(const sf::Vector2f& origin)
{
	if (!m_originIsCentroid)
		*m_origin = origin;
}

// = = = = = = = = = = Mutators = = = = = = = = = =
void VertexArray2::setOriginIsCentroid(bool value)
{
	if (value == m_originIsCentroid)
		return;

	if (value && !m_originIsCentroid) {
		delete m_origin;
		m_origin = &m_centroid;
		m_originIsCentroid = true;
	}
	else if (!value && m_originIsCentroid) {
		m_origin = new sf::Vector2f{m_centroid};
		m_originIsCentroid = false;
	}
}

// = = = = = = = = = = Accessors = = = = = = = = = =
const sf::Vector2f& VertexArray2::getCentroid()
{
	if (m_needCentroidUpdate)
		updateCentroid();

	return m_centroid;
}

const sf::Vector2f& VertexArray2::getPosition()
{
	return getOrigin();
}

const sf::Vector2f& VertexArray2::getOrigin()
{
	if (m_originIsCentroid && m_needCentroidUpdate)
		updateCentroid();

	return *m_origin;
}

const sf::Vector2f& VertexArray2::getScale()
{
	return m_scale;
}

const float& VertexArray2::getRotation()
{
	return m_rotation;
}

const bool VertexArray2::getOriginIsCentroid()
{
	return m_originIsCentroid;
}