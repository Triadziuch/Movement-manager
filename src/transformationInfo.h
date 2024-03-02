#pragma once
#include <SFML/Graphics.hpp>
#include "VertexArray2.h"
#include <math.h>

// Movement info class
class transformationInfo {
protected:
	bool		 m_repeat{};
	float		 m_currentTime{};
	float		 m_motionDuration{};
	float		 m_delayBefore{};
	float		 m_delayAfter{};
	float		 m_totalDuration{};
	double		 (*m_usedFunctionPtr)(double) {};

public:
	// Constructors / Destructors
	transformationInfo() = default;
	transformationInfo(bool repeat, float motionDuration, float delayBefore, float delayAfter, double (*usedFunctionPtr)(double));
	transformationInfo(bool repeat, float _current__time, float motionDuration, float delayBefore, float delayAfter, double (*usedFunctionPtr)(double));
	transformationInfo(const transformationInfo& obj);
	virtual ~transformationInfo() = default;

	// Public functions
	void reset();
	const bool isDone() const;
	const bool isFinished() const;
};

class movementInfo : public transformationInfo {
private:
	sf::Vector2f m_startingPos{};
	sf::Vector2f m_endingPos{};

public:
	// Constructors / Destructors
	movementInfo(sf::Vector2f startingPos, sf::Vector2f endingPos, float motionDuration, double (*usedFunctionPtr)(double), bool repeat, float delayBefore, float delayAfter);
	movementInfo(const movementInfo& obj);

	// Accessors
	sf::Vector2f& getStartingPos();
	const sf::Vector2f& getStartingPos() const;

	sf::Vector2f& getEndingPos();
	const sf::Vector2f& getEndingPos() const;
};

class scalingInfo : public transformationInfo {
private:
	sf::Vector2f m_startingScale{};
	sf::Vector2f m_endingScale{};

public:
	// Constructors / Destructors
	scalingInfo(sf::Vector2f startingScale, sf::Vector2f endingScale, float motionDuration, double(*usedFunctionPtr)(double), bool repeat, float delayBefore, float delayAfter);
	scalingInfo(const scalingInfo& obj);

	// Accessors
	sf::Vector2f& getStartingScale();
	const sf::Vector2f& getStartingScale() const;

	sf::Vector2f& getEndingScale();
	const sf::Vector2f& getEndingScale() const;
};

class rotationInfo : public transformationInfo {
private:
	float m_startingRotation{};
	float m_endingRotation{};
	float m_rotationOffset{};
	bool  m_clockwise = true;

public:
	// Constructors / Destructors
	rotationInfo(float startingRotation, float endingRotation, float motionDuration, double(*usedFunctionPtr)(double), bool repeat, float delayBefore, float delayAfter, bool clockwise);
	rotationInfo(const rotationInfo& obj);

	// Update functions
	const float updateRotation() const;

	// Accessors
	float& getStartingRotation();
	const float& getStartingRotation() const;

	float& getEndingRotation();
	const float& getEndingRotation() const;

	bool getClockwise() const;
};