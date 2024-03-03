#pragma once
#include <SFML/Graphics.hpp>
#include "VertexArray2.h"
#include "easeFunctions.h"
#include <math.h>

// - - - - - - - - - - - - - - - - - - - - transformationInfo - - - - - - - - - - - - - - - - - - - - \\
 
class transformationInfo {
public:
	bool		  m_repeat{};
	float		  m_currentTime{};
	float		  m_motionDuration{};
	float		  m_delayBefore{};
	float		  m_delayAfter{};
	float		  m_totalDuration{};
	double		  (*m_usedFunctionPtr)(double) {};

	// Constructors / Destructors
	transformationInfo() = default;
	transformationInfo(bool repeat, float motionDuration, float delayBefore, float delayAfter, easeFunctions::Tmovement_function usedFunctionType);
	transformationInfo(bool repeat, float motionDuration, float delayBefore, float delayAfter, double (*usedFunctionPtr)(double));
	transformationInfo(bool repeat, float _current__time, float motionDuration, float delayBefore, float delayAfter, double (*usedFunctionPtr)(double));
	transformationInfo(const transformationInfo& obj);
	virtual ~transformationInfo() = default;

	bool operator==(const transformationInfo& rhs) const;

	// Public functions
	void reset();
	const bool isDone() const;
	const bool isFinished() const;
	void setFunction(easeFunctions::Tmovement_function usedFunctionType);
	void setFunction(double (*usedFunctionPtr)(double));
};


// - - - - - - - - - - - - - - - - - - - - movementInfo - - - - - - - - - - - - - - - - - - - - \\

class movementInfo : public transformationInfo {
private:
	sf::Vector2f m_startingPos{};
	sf::Vector2f m_endingPos{};

public:
	// Constructors / Destructors
	movementInfo(sf::Vector2f startingPos, sf::Vector2f endingPos, float motionDuration, easeFunctions::Tmovement_function usedFunctionType, bool repeat, float delayBefore, float delayAfter);
	movementInfo(sf::Vector2f startingPos, sf::Vector2f endingPos, float motionDuration, double (*usedFunctionPtr)(double), bool repeat, float delayBefore, float delayAfter);
	movementInfo(const movementInfo& obj);

	bool operator==(const movementInfo& rhs) const;

	// Update functions
	const sf::Vector2f updatePosition() const;

	// Accessors
	sf::Vector2f& getStartingPos();
	const sf::Vector2f& getStartingPos() const;

	sf::Vector2f& getEndingPos();
	const sf::Vector2f& getEndingPos() const;
};



// - - - - - - - - - - - - - - - - - - - - scalingInfo - - - - - - - - - - - - - - - - - - - - \\

class scalingInfo : public transformationInfo {
private:
	sf::Vector2f m_startingScale{};
	sf::Vector2f m_endingScale{};

public:
	// Constructors / Destructors
	scalingInfo(sf::Vector2f startingScale, sf::Vector2f endingScale, float motionDuration, easeFunctions::Tmovement_function usedFunctionType, bool repeat, float delayBefore, float delayAfter);
	scalingInfo(sf::Vector2f startingScale, sf::Vector2f endingScale, float motionDuration, double(*usedFunctionPtr)(double), bool repeat, float delayBefore, float delayAfter);
	scalingInfo(const scalingInfo& obj);

	bool operator==(const scalingInfo& rhs) const;

	// Public functions
	void scale(const sf::Vector2f& scale);

	// Update functions
	const sf::Vector2f updateScale() const;

	// Accessors
	sf::Vector2f& getStartingScale();
	const sf::Vector2f& getStartingScale() const;

	sf::Vector2f& getEndingScale();
	const sf::Vector2f& getEndingScale() const;
};


// - - - - - - - - - - - - - - - - - - - - rotationInfo - - - - - - - - - - - - - - - - - - - - \\

class rotationInfo : public transformationInfo {
private:
	float m_startingRotation{};
	float m_endingRotation{};
	bool  m_clockwise = true;

public:
	// Constructors / Destructors
	rotationInfo(float startingRotation, float endingRotation, float motionDuration, easeFunctions::Tmovement_function usedFunctionType, bool repeat, float delayBefore, float delayAfter, bool clockwise);
	rotationInfo(float startingRotation, float endingRotation, float motionDuration, double(*usedFunctionPtr)(double), bool repeat, float delayBefore, float delayAfter, bool clockwise);
	rotationInfo(const rotationInfo& obj);

	bool operator==(const rotationInfo& rhs) const;

	// Update functions
	const float updateRotation() const;

	// Accessors
	float& getStartingRotation();
	const float& getStartingRotation() const;

	float& getEndingRotation();
	const float& getEndingRotation() const;

	bool getClockwise() const;
};