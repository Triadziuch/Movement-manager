#pragma once
#include "VertexArray2.h"

void VertexArray2::undoTransformations()
{
	const size_t vertex_count = sf::VertexArray::getVertexCount();

	float sine   = - static_cast<float>(sin(static_cast<double>(-m_rotation) * M_RAD)),
		  cosine =   static_cast<float>(cos(static_cast<double>(-m_rotation) * M_RAD));

	for (size_t i = 0; i < vertex_count; ++i) {
		// Undo rotation
		float x = sf::VertexArray::operator[](i).position.x - m_origin.x;
		float y = sf::VertexArray::operator[](i).position.y - m_origin.y;
		sf::VertexArray::operator[](i).position.x = m_origin.x + x * cosine - y * sine;
		sf::VertexArray::operator[](i).position.y = m_origin.y + x * sine + y * cosine;

		// Undo scaling
		sf::VertexArray::operator[](i).position.x = m_origin.x + (sf::VertexArray::operator[](i).position.x - m_origin.x) * (1.f / m_scale.x);
		sf::VertexArray::operator[](i).position.y = m_origin.y + (sf::VertexArray::operator[](i).position.y - m_origin.y) * (1.f / m_scale.x);
	}
}

void VertexArray2::updateCentroid()
{
	sf::Vector2f centroid_temp{};
	const size_t vertex_count = sf::VertexArray::getVertexCount();

	for (size_t i = 0; i < vertex_count; ++i)
		centroid_temp += sf::VertexArray::operator[](i).position;
	m_centroid = centroid_temp / static_cast<float>(vertex_count);
}

void VertexArray2::updateInternalVertices()
{
	updateCentroid();
	undoTransformations();

	const size_t vertex_count = sf::VertexArray::getVertexCount();

	m_originalScaling.resize(vertex_count);
	m_originalRotation.resize(vertex_count);

	float sine =   static_cast<float>(sin(static_cast<double>(m_rotation) * M_RAD)),
		  cosine = static_cast<float>(cos(static_cast<double>(m_rotation) * M_RAD));

	for (size_t i = 0; i < vertex_count; ++i) {
		m_originalScaling[i]  = sf::VertexArray::operator[](i);
		m_originalRotation[i] = sf::VertexArray::operator[](i);

		float x = m_originalScaling[i].position.x - m_origin.x;
		float y = m_originalScaling[i].position.y - m_origin.y;
		m_originalScaling[i].position.x = m_origin.x + x * cosine - y * sine;
		m_originalScaling[i].position.y = m_origin.y + x * sine + y * cosine;

		m_originalRotation[i].position.x = m_origin.x + (m_originalRotation[i].position.x - m_origin.x) * m_scale.x;
		m_originalRotation[i].position.y = m_origin.y + (m_originalRotation[i].position.y - m_origin.y) * m_scale.x;
	}

	m_needUpdate = false;
}

void VertexArray2::updatePosition(const sf::Vector2f& new_pos)
{
	if (m_needUpdate) 
		updateInternalVertices();

	const sf::Vector2f offset = new_pos - m_origin;
	const size_t vertex_count = sf::VertexArray::getVertexCount();

	for (size_t i = 0; i < vertex_count; ++i) {
		sf::VertexArray::operator[](i).position += offset;
		m_originalScaling[i].position  += offset;
		m_originalRotation[i].position += offset;
	}

	m_centroid += offset;
	m_origin = new_pos;
}

void VertexArray2::updateScale()
{
	if (m_needUpdate)
		updateInternalVertices();

	/*for (size_t i = 0; i < _vertexarray->getVertexCount(); i++) {
		_vertexarray->operator[](i).position.x = _scalingInfo->centroid.x + (_scalingInfo->originalVertex.operator[](i).position.x - _scalingInfo->centroid.x) * _scalingInfo->current_scale.x;
		_vertexarray->operator[](i).position.y = _scalingInfo->centroid.y + (_scalingInfo->originalVertex.operator[](i).position.y - _scalingInfo->centroid.y) * _scalingInfo->current_scale.y;
	}*/
}

void VertexArray2::updateRotation()
{
	if (m_needUpdate)
		updateInternalVertices();
}

void VertexArray2::setPosition(float x, float y)
{
	updatePosition(sf::Vector2f(x, y));
}

void VertexArray2::setPosition(const sf::Vector2f& position)
{
	updatePosition(position);
}

sf::Vertex& VertexArray2::operator [](std::size_t index)
{
	m_needUpdate = true;
	return sf::VertexArray::operator[](index);
}

const sf::Vertex& VertexArray2::operator [](std::size_t index) const
{
	return sf::VertexArray::operator[](index);
}

void VertexArray2::setRotation(float angle)
{
	m_rotation = angle;
	m_needUpdate = true;
	updateRotation();
}

void VertexArray2::setScale(float factorX, float factorY)
{
	m_scale.x = factorX;
	m_scale.y = factorY;
	updateScale();
}

void VertexArray2::setScale(const sf::Vector2f& factors)
{
	m_scale = factors;
	updateScale();
}

void VertexArray2::clear()
{
	m_needUpdate = true;
	m_originalScaling.clear();
	m_originalRotation.clear();
	sf::VertexArray::clear();
}

void VertexArray2::resize(std::size_t vertexCount)
{
	m_needUpdate = true;
	m_originalScaling.resize(vertexCount);
	m_originalRotation.resize(vertexCount);
	sf::VertexArray::resize(vertexCount);
}

void VertexArray2::append(const sf::Vertex& vertex)
{
	m_needUpdate = true;
	m_originalScaling.push_back(vertex);
	m_originalRotation.push_back(vertex);
	sf::VertexArray::append(vertex);
}

void VertexArray2::setOrigin(float x, float y)
{
	m_origin = sf::Vector2f(x, y);
}

void VertexArray2::setOrigin(const sf::Vector2f& origin)
{
	m_origin = origin;
}

const sf::Vector2f& VertexArray2::getCentroid() const
{
	return m_centroid;
}