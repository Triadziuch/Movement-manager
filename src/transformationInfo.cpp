#pragma once
#include "transformationInfo.h"

// - - - - - - - - - - - - - - - - - - - - transformationInfo - - - - - - - - - - - - - - - - - - - - \\

// Constructors / Destructors
transformationInfo::transformationInfo(bool repeat, float motionDuration, float delayBefore, float delayAfter, easeFunctions::Tmovement_function usedFunctionType) :
	m_repeat{ repeat }, 
	m_motionDuration{ motionDuration }, 
	m_delayBefore{ delayBefore }, 
	m_delayAfter{ delayAfter }, 
	m_usedFunctionPtr{ easeFunctions::movement_functions[usedFunctionType] },
	m_totalDuration{ delayBefore + motionDuration + delayAfter }
	{}

transformationInfo::transformationInfo(bool repeat, float motionDuration, float delayBefore, float delayAfter, double (*usedFunctionPtr)(double)) :
	m_repeat{ repeat }, 
	m_motionDuration{ motionDuration }, 
	m_delayBefore{ delayBefore }, 
	m_delayAfter{ delayAfter }, 
	m_usedFunctionPtr{ usedFunctionPtr }, 
	m_totalDuration{ delayBefore + motionDuration + delayAfter }
	{}

transformationInfo::transformationInfo(bool repeat, float _current__time, float motionDuration, float delayBefore, float delayAfter, double(*usedFunctionPtr)(double)) :
	m_repeat{ repeat }, 
	m_currentTime{ _current__time }, 
	m_motionDuration{ motionDuration }, 
	m_delayBefore{ delayBefore }, 
	m_delayAfter{ delayAfter }, 
	m_usedFunctionPtr{ usedFunctionPtr }, 
	m_totalDuration{ delayBefore + motionDuration + delayAfter } 
	{}

transformationInfo::transformationInfo(const transformationInfo& obj) :
	m_repeat{ obj.m_repeat }, 
	m_currentTime{ obj.m_currentTime }, 
	m_motionDuration{ obj.m_motionDuration }, 
	m_delayBefore{ obj.m_delayBefore }, 
	m_delayAfter{ obj.m_delayAfter }, 
	m_usedFunctionPtr{ obj.m_usedFunctionPtr }, 
	m_totalDuration{ obj.m_totalDuration } 
	{}

// Operators
bool transformationInfo::operator==(const transformationInfo& rhs) const
{
	return m_repeat == rhs.m_repeat &&
		m_currentTime == rhs.m_currentTime &&
		m_motionDuration == rhs.m_motionDuration &&
		m_delayBefore == rhs.m_delayBefore &&
		m_delayAfter == rhs.m_delayAfter &&
		m_usedFunctionPtr == rhs.m_usedFunctionPtr;
}

// Public functions
void transformationInfo::reset()
{
	m_currentTime = 0.f;
}

inline const bool transformationInfo::isDone() const
{
	return (m_currentTime - m_delayBefore >= m_motionDuration);
}

const bool transformationInfo::isFinished() const
{
	return (m_currentTime >= m_totalDuration);
}

// Mutators
void transformationInfo::setFunction(easeFunctions::Tmovement_function usedFunctionType)
{
	m_usedFunctionPtr = easeFunctions::movement_functions[usedFunctionType];
}

void transformationInfo::setFunction(double(*usedFunctionPtr)(double))
{
	m_usedFunctionPtr = usedFunctionPtr;
}

void transformationInfo::setAnimationTime(const float time) {

	if (time <= 0.f || time == m_motionDuration) return;

	if (m_currentTime < m_delayBefore) 
		m_motionDuration = time;
	else {
		const float timeFactor = (m_currentTime - m_delayBefore) / m_motionDuration;
		m_motionDuration = time;
		m_currentTime = m_delayBefore + timeFactor * m_motionDuration;
	}

	m_totalDuration = m_delayBefore + m_motionDuration + m_delayAfter;

	if (m_currentTime > m_totalDuration) m_currentTime = m_totalDuration;
}

// - - - - - - - - - - - - - - - - - - - - movementInfo - - - - - - - - - - - - - - - - - - - - \\

movementInfo::movementInfo(sf::Vector2f startingPos, sf::Vector2f endingPos, float motionDuration, easeFunctions::Tmovement_function usedFunctionType, bool repeat, float delayBefore, float delayAfter) :
	transformationInfo{ repeat, motionDuration, delayBefore, delayAfter, usedFunctionType }, 
	m_startingPos{ startingPos }, 
	m_endingPos{ endingPos }
	{}

// Constructors / Destructors
movementInfo::movementInfo(sf::Vector2f startingPos, sf::Vector2f endingPos, float motionDuration, double(*usedFunctionPtr)(double), bool repeat, float delayBefore, float delayAfter) :
	transformationInfo{ repeat, motionDuration, delayBefore, delayAfter, usedFunctionPtr }, 
	m_startingPos{ startingPos }, 
	m_endingPos{ endingPos }
	{}

movementInfo::movementInfo(const movementInfo & obj) :
	transformationInfo{ obj.m_repeat, obj.m_currentTime, obj.m_motionDuration, obj.m_delayBefore, obj.m_delayAfter, obj.m_usedFunctionPtr },
	m_startingPos(obj.m_startingPos),
	m_endingPos(obj.m_endingPos)
	{}

// Operators
bool movementInfo::operator==(const movementInfo& rhs) const {
	return	m_startingPos == rhs.m_startingPos &&
		m_endingPos == rhs.m_endingPos &&
		static_cast<const transformationInfo&>(*this) == static_cast<const transformationInfo&>(rhs);
}

const bool movementInfo::update(sf::Transformable& transformable, const float dt)
{
	m_currentTime += dt;

	if (isDone()) {
		if (m_currentTime - m_delayBefore - dt < m_motionDuration)
			transformable.setPosition(m_endingPos);
		else if (isFinished())
			return false;
	}
	else {
		if (m_currentTime > m_delayBefore) 
			transformable.setPosition(updatePosition());
		else if (m_currentTime == dt)
			transformable.setPosition(m_startingPos);
	}

	return true;
}

// Update functions
const sf::Vector2f movementInfo::updatePosition() const
{
	const float ease_function_value = static_cast<float>(m_usedFunctionPtr(static_cast<double>((m_currentTime - m_delayBefore) / m_motionDuration)));
	return sf::Vector2f{ ease_function_value * (m_endingPos.x - m_startingPos.x) + m_startingPos.x,
						 ease_function_value * (m_endingPos.y - m_startingPos.y) + m_startingPos.y };
}

// Accessors
sf::Vector2f& movementInfo::getStartingPos()
{
	return m_startingPos;
}

const sf::Vector2f& movementInfo::getStartingPos() const
{
	return m_startingPos;
}

sf::Vector2f& movementInfo::getEndingPos()
{
	return m_endingPos;
}

const sf::Vector2f& movementInfo::getEndingPos() const
{
	return m_endingPos;
}

// - - - - - - - - - - - - - - - - - - - - scalingInfo - - - - - - - - - - - - - - - - - - - - \\

scalingInfo::scalingInfo(sf::Vector2f startingScale, sf::Vector2f endingScale, float motionDuration, easeFunctions::Tmovement_function usedFunctionType, bool repeat, float delayBefore, float delayAfter) :
	transformationInfo{ repeat, motionDuration, delayBefore, delayAfter, usedFunctionType }, 
	m_startingScale(startingScale), 
	m_endingScale(endingScale)
	{}

// Constructors / Destructors
scalingInfo::scalingInfo(sf::Vector2f startingScale, sf::Vector2f endingScale, float motionDuration, double(*usedFunctionPtr)(double), bool repeat, float delayBefore, float delayAfter) :
	transformationInfo{ repeat, motionDuration, delayBefore, delayAfter, usedFunctionPtr },
	m_startingScale(startingScale),
	m_endingScale(endingScale)
	{}

scalingInfo::scalingInfo(const scalingInfo & obj) :
	transformationInfo{ obj.m_repeat, obj.m_currentTime, obj.m_motionDuration, obj.m_delayBefore, obj.m_delayAfter, obj.m_usedFunctionPtr },
	m_startingScale(obj.m_startingScale),
	m_endingScale(obj.m_endingScale)
	{}

// Operators
bool scalingInfo::operator==(const scalingInfo& rhs) const
{
	return	m_startingScale == rhs.m_startingScale &&
		m_endingScale == rhs.m_endingScale &&
		static_cast<const transformationInfo&>(*this) == static_cast<const transformationInfo&>(rhs);
}

// Public functions
void scalingInfo::scale(const sf::Vector2f& scale)
{
	m_startingScale.x *= scale.x;
	m_startingScale.y *= scale.y;
	m_endingScale.x *= scale.x;
	m_endingScale.y *= scale.y;
}

const bool scalingInfo::update(sf::Transformable& transformable, const float dt)
{
	m_currentTime += dt;

	if (isDone()) {
		if (m_currentTime - m_delayBefore - dt < m_motionDuration)
			transformable.setScale(m_endingScale);
		else if (isFinished())
			return false;
	}
	else {
		if (m_currentTime > m_delayBefore)
			transformable.setScale(updateScale());
		else if (m_currentTime == dt)
			transformable.setScale(m_startingScale);
	}

	return true;
}

const sf::Vector2f scalingInfo::updateScale() const
{
	const float ease_function_value = static_cast<float>(m_usedFunctionPtr(static_cast<double>((m_currentTime - m_delayBefore) / m_motionDuration)));
	return sf::Vector2f{ ease_function_value * (m_endingScale.x - m_startingScale.x) + m_startingScale.x,
						 ease_function_value * (m_endingScale.y - m_startingScale.y) + m_startingScale.y };
}

// Accessors
sf::Vector2f& scalingInfo::getStartingScale()
{
	return m_startingScale;
}

const sf::Vector2f& scalingInfo::getStartingScale() const
{
	return m_startingScale;
}

sf::Vector2f& scalingInfo::getEndingScale()
{
	return m_endingScale;
}

const sf::Vector2f& scalingInfo::getEndingScale() const
{
	return m_endingScale;
}


// - - - - - - - - - - - - - - - - - - - - rotationInfo - - - - - - - - - - - - - - - - - - - - \\

rotationInfo::rotationInfo(float startingRotation, float endingRotation, float motionDuration, easeFunctions::Tmovement_function usedFunctionType, bool repeat, float delayBefore, float delayAfter, bool clockwise) :
	transformationInfo{ repeat, motionDuration, delayBefore, delayAfter, usedFunctionType },
	m_clockwise(clockwise)
{
	if (m_clockwise) {
		m_startingRotation = startingRotation;
		m_endingRotation = endingRotation;
	}
	else {
		m_startingRotation = 360.f - startingRotation;
		m_endingRotation = 360.f - endingRotation;
	}
}

// Constructors / Destructors
rotationInfo::rotationInfo(float startingRotation, float endingRotation, float motionDuration, double(*usedFunctionPtr)(double), bool repeat, float delayBefore, float delayAfter, bool clockwise) :
	transformationInfo{ repeat, motionDuration, delayBefore, delayAfter, usedFunctionPtr },
	m_clockwise(clockwise) 
{
	if (m_clockwise) {
		m_startingRotation = startingRotation;
		m_endingRotation = endingRotation;
	}
	else {
		m_startingRotation = 360.f - startingRotation;
		m_endingRotation = 360.f - endingRotation;
	}
}

rotationInfo::rotationInfo(const rotationInfo& obj) :
	transformationInfo{ obj.m_repeat, obj.m_currentTime, obj.m_motionDuration, obj.m_delayBefore, obj.m_delayAfter, obj.m_usedFunctionPtr },
	m_startingRotation(obj.m_startingRotation),
	m_endingRotation(obj.m_endingRotation),
	m_clockwise(obj.m_clockwise)
	{}

// Operators
bool rotationInfo::operator==(const rotationInfo& rhs) const
{
	return	m_startingRotation == rhs.m_startingRotation &&
		m_endingRotation == rhs.m_endingRotation &&
		m_clockwise == rhs.m_clockwise &&
		static_cast<const transformationInfo&>(*this) == static_cast<const transformationInfo&>(rhs);
}

const bool rotationInfo::update(sf::Transformable& transformable, const float dt)
{
	m_currentTime += dt;

	if (isDone()) {
		if (m_currentTime - m_delayBefore - dt < m_motionDuration)
			transformable.setRotation(m_endingRotation);
		else if (isFinished())
			return false;
	}
	else {
		if (m_currentTime > m_delayBefore)
			transformable.setRotation(updateRotation());
		else if (m_currentTime == dt)
			transformable.setRotation(m_startingRotation);
	}

	return true;
}

// Update functions
const float rotationInfo::updateRotation() const
{
	return static_cast<const float>(m_usedFunctionPtr(static_cast<double>((m_currentTime - m_delayBefore) / m_motionDuration))) * (m_endingRotation - m_startingRotation) + m_startingRotation;
}

// Accessors
float& rotationInfo::getStartingRotation()
{
	return m_startingRotation;
}

const float& rotationInfo::getStartingRotation() const
{
	return m_startingRotation;
}

float& rotationInfo::getEndingRotation()
{
	return m_endingRotation;
}

const float& rotationInfo::getEndingRotation() const
{
	return m_endingRotation;
}

bool rotationInfo::getClockwise() const
{
	return m_clockwise;
}