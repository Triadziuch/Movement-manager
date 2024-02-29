#pragma once
#include "VertexArray2.h"

void VertexArray2::updateCentroid()
{
	sf::Vector2f centroid_temp{};
	const size_t vertex_count = sf::VertexArray::getVertexCount();

	for (size_t i = 0; i < vertex_count; ++i)
		centroid_temp += sf::VertexArray::operator[](i).position;
	m_centroid = centroid_temp / static_cast<float>(vertex_count);
}

void VertexArray2::updateScale()
{
	for (size_t i = 0; i < _vertexarray->getVertexCount(); i++) {
		_vertexarray->operator[](i).position.x = _scalingInfo->centroid.x + (_scalingInfo->originalVertex.operator[](i).position.x - _scalingInfo->centroid.x) * _scalingInfo->current_scale.x;
		_vertexarray->operator[](i).position.y = _scalingInfo->centroid.y + (_scalingInfo->originalVertex.operator[](i).position.y - _scalingInfo->centroid.y) * _scalingInfo->current_scale.y;
	}
}

void VertexArray2::updateRotation()
{
}

void VertexArray2::setPosition(float x, float y)
{
}

void VertexArray2::setPosition(const sf::Vector2f& position)
{
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