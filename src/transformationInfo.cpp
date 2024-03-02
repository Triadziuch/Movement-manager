#pragma once
#include "transformationInfo.h"



// Constructors / Destructors
transformationInfo::transformationInfo() {};

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

// Public functions
void transformationInfo::reset()
{
	m_currentTime = 0.f;
}

const bool transformationInfo::isDone() const
{
	return (m_currentTime - m_delayBefore >= m_motionDuration);
}

const bool transformationInfo::isFinished() const
{
	return (m_currentTime >= m_totalDuration);
}

// - - - - - - - - - - - - - - - - - - - - movementInfo - - - - - - - - - - - - - - - - - - - - \\

// Constructors / Destructors
movementInfo::movementInfo(sf::Vector2f startingPos, sf::Vector2f endingPos, float motionDuration, double(*usedFunctionPtr)(double), bool repeat, float delayBefore, float delayAfter) :
	transformationInfo{ repeat, motionDuration, delayBefore, delayAfter, usedFunctionPtr }, 
	m_startingPos(startingPos), 
	m_endingPos(endingPos) 
	{}

movementInfo::movementInfo(const movementInfo & obj) :
	transformationInfo{ obj.m_repeat, obj.m_currentTime, obj.m_motionDuration, obj.m_delayBefore, obj.m_delayAfter, obj.m_usedFunctionPtr },
	m_startingPos(obj.m_startingPos),
	m_endingPos(obj.m_endingPos)
	{}

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

	m_rotationOffset = m_endingRotation - m_startingRotation;
}

rotationInfo::rotationInfo(const rotationInfo& obj) :
	transformationInfo{ obj.m_repeat, obj.m_currentTime, obj.m_motionDuration, obj.m_delayBefore, obj.m_delayAfter, obj.m_usedFunctionPtr },
	m_startingRotation(obj.m_startingRotation),
	m_endingRotation(obj.m_endingRotation),
	m_rotationOffset(obj.m_rotationOffset),
	m_clockwise(obj.m_clockwise)
	{}

// Update functions
const float rotationInfo::updateRotation() const
{
	float new_rotation = static_cast<float>(m_usedFunctionPtr(static_cast<double>((m_currentTime - m_delayBefore) / m_motionDuration))) * m_rotationOffset + m_startingRotation;
	return new_rotation;
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