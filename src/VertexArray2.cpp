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

void VertexArray2::setScale(float factorX, float factorY)
{
	m_scale.x = factorX;
	m_scale.y = factorY;
	updateScale();
}
