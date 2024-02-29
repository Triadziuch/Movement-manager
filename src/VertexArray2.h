#pragma once
#include <SFML/Graphics.hpp>

class VertexArray2 : public sf::VertexArray {
private:
	sf::Vector2f	m_centroid;

	sf::Vector2f	m_origin;
	sf::Vector2f	m_scale;
	float			m_rotation;
	bool			m_needUpdate;

	std::vector<sf::Vertex> m_originalScaling;
	std::vector<sf::Vertex> m_originalRotation;

	void updateCentroid();
	void updateScale();
	void updateRotation();

public:

	VertexArray2() : sf::VertexArray{}, m_centroid{}, m_origin{}, m_scale{ 1, 1 }, m_rotation{ 0 }, m_needUpdate{ false } {}
	VertexArray2(sf::PrimitiveType type, size_t vertexCount = 0) : sf::VertexArray{ type, vertexCount }, m_centroid{}, m_origin{}, m_scale{ 1, 1 }, m_rotation{ 0 }, m_needUpdate{ false }, m_originalScaling{ vertexCount }, m_originalRotation{ vertexCount } {}
	VertexArray2(const VertexArray2& other) : sf::VertexArray{ other }, m_centroid{ other.m_centroid }, m_origin{ other.m_origin }, m_scale{ other.m_scale }, m_rotation{ other.m_rotation }, m_needUpdate{ other.m_needUpdate }, m_originalScaling{ other.m_originalScaling }, m_originalRotation{ other.m_originalRotation } {}
	~VertexArray2() {};

	void clear();
	void resize(std::size_t vertexCount);
	void append(const sf::Vertex& vertex);

	sf::Vertex& operator [](std::size_t index);
	const sf::Vertex& operator [](std::size_t index) const;

	operator sf::VertexArray& () { return *this; }
	operator const sf::VertexArray& () const { return *this; }

	void setPosition(float x, float y);
	void setPosition(const sf::Vector2f& position);

	void setRotation(float angle);

	void setScale(float factorX, float factorY);
	void setScale(const sf::Vector2f& factors);

	void setOrigin(float x, float y);
	void setOrigin(const sf::Vector2f& origin);
	

	// T¹ klasê wpierdalamy do MovementRoutineEngine zamiast sf::VertexArray
	// Trzeba dopisaæ setRotation, setScale, setPosition, 
};