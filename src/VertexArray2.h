#pragma once
#include <SFML/Graphics.hpp>

class VertexArray2 {
public:
	sf::VertexArray vertexArray;
	sf::Vector2f centroid{};
	sf::Vector2f current_scale{ 1.f, 1.f };
	float current_rotation{ 0.f };

	// T� klas� wpierdalamy do MovementRoutineEngine zamiast sf::VertexArray
	// Trzeba dopisa� setRotation, setScale, setPosition, 
};