#pragma once
#include "VertexArray2.h"

void VertexArray2::updateScale()
{
	for (size_t i = 0; i < _vertexarray->getVertexCount(); i++) {
		_vertexarray->operator[](i).position.x = _scalingInfo->centroid.x + (_scalingInfo->originalVertex.operator[](i).position.x - _scalingInfo->centroid.x) * _scalingInfo->current_scale.x;
		_vertexarray->operator[](i).position.y = _scalingInfo->centroid.y + (_scalingInfo->originalVertex.operator[](i).position.y - _scalingInfo->centroid.y) * _scalingInfo->current_scale.y;
	}
}

void VertexArray2::setPosition(float x, float y)
{
}

void VertexArray2::setPosition(const sf::Vector2f& position)
{
}

sf::Vertex& VertexArray2::operator [](std::size_t index)
{
	m_need_update = true;
	return sf::VertexArray::operator[](index);
}

void VertexArray2::setScale(float factorX, float factorY)
{
	m_scale.x = factorX;
	m_scale.y = factorY;
	updateScale();
}

void VertexArray2::clear()
{
	sf::VertexArray::clear();
	m_original_scaling.clear();
	m_original_rotation.clear();
}

void VertexArray2::resize(std::size_t vertexCount)
{
	sf::VertexArray::resize(vertexCount);
	m_original_scaling.resize(vertexCount);
	m_original_rotation.resize(vertexCount);
}

void VertexArray2::append(const sf::Vertex& vertex)
{
	sf::VertexArray::append(vertex);
	m_original_scaling.push_back(vertex);
	m_original_rotation.push_back(vertex);
}

void VertexArray2::setOrigin(float x, float y)
{
}
