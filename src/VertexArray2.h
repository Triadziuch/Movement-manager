#pragma once
#include <SFML/Graphics.hpp>

class VertexArray2 : public sf::VertexArray {
private:
	constexpr static double M_RAD = 3.14159265358979323846 / 180.0;

	sf::Vector2f	m_centroid;
	sf::Vector2f*	m_origin;
	sf::Vector2f	m_scale;
	float			m_rotation;
	bool			m_needCentroidUpdate;
	bool			m_originIsCentroid;

	// Private update functions
	void updateCentroid();
	void updatePosition(const sf::Vector2f& new_pos);
	void updateScale(const sf::Vector2f& new_scale);
	void updateRotation(const float new_rotation);

public:
	// Constructors / Destructors
	VertexArray2();
	VertexArray2(sf::PrimitiveType type, size_t vertexCount = 0);
	VertexArray2(const VertexArray2& other);
	~VertexArray2();

	// Overriden functions
	void clear();
	void resize(std::size_t vertexCount);
	void append(const sf::Vertex& vertex);

	// Overriden operators
	sf::Vertex& operator [](std::size_t index);
	const sf::Vertex& operator [](std::size_t index) const;

	operator sf::VertexArray& ();
	operator const sf::VertexArray& () const;

	// Public functions
	void setPosition(float x, float y);
	void setPosition(const sf::Vector2f& position);
	
	void move(float offsetX, float offsetY);
	void move(const sf::Vector2f& offset);

	void setRotation(float angle);

	void setScale(float factorX, float factorY);
	void setScale(const sf::Vector2f& factors);

	void setOrigin(float x, float y);
	void setOrigin(const sf::Vector2f& origin);

	// Mutators
	void setOriginIsCentroid(bool value);

	// Accessors
	const sf::Vector2f& getCentroid();
	const sf::Vector2f& getPosition();

	const sf::Vector2f& getOrigin();

	const sf::Vector2f& getScale();

	const float& getRotation();

	const bool getOriginIsCentroid();

};