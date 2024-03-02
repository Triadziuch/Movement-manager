#pragma once
#include "TransformationRoutine.h"
#include "MovementContainer.h"

// - - - - - - - - - - - - - - - - - - - - TransformationRoutine - - - - - - - - - - - - - - - - - - - - \\

// Constructors / Destructors
TransformationRoutine::TransformationRoutine() :
	m_movementRoutineEngine{ nullptr }, 
	m_routineName{}, 
	m_current{}, 
	m_count{}, 
	m_isActive{}, 
	m_isLooping{}, 
	m_isPaused{}, 
	m_adjustStartToCurrentTransform{}, 
	m_adjustAllToCurrentTransform{} 
	{}

TransformationRoutine::TransformationRoutine(const std::string& name, const MovementRoutineEngine* movementRoutineEnginePtr) :
	m_movementRoutineEngine{ movementRoutineEnginePtr }, 
	m_routineName{ name }, 
	m_current{}, 
	m_count{}, 
	m_isActive{}, 
	m_isLooping{}, 
	m_isPaused{}, 
	m_adjustStartToCurrentTransform{}, 
	m_adjustAllToCurrentTransform{} 
	{}

TransformationRoutine::TransformationRoutine(const TransformationRoutine& obj) :
	m_movementRoutineEngine{ obj.m_movementRoutineEngine }, 
	m_routineName{ obj.m_routineName }, 
	m_current{ obj.m_current }, 
	m_count{ obj.m_count }, 
	m_isActive{ obj.m_isActive }, 
	m_isLooping{ obj.m_isLooping }, 
	m_isPaused{ obj.m_isPaused }, 
	m_adjustStartToCurrentTransform{ obj.m_adjustStartToCurrentTransform }, 
	m_adjustAllToCurrentTransform{ obj.m_adjustAllToCurrentTransform } 
	{}

TransformationRoutine::~TransformationRoutine() {
	m_movementRoutineEngine = nullptr;
}

// Mutators
void TransformationRoutine::setLooping(bool looping) {
	this->m_isLooping = looping;
}

void TransformationRoutine::adjustStartToCurrentTransform(bool adjust) {
	this->m_adjustStartToCurrentTransform = adjust;
}

void TransformationRoutine::adjustAllToCurrentTransform(bool adjust) {
	this->m_adjustAllToCurrentTransform = adjust;
}

void TransformationRoutine::pause() { 
	this->m_isPaused = true;
}

void TransformationRoutine::resume() {
	this->m_isPaused = false;
}


// - - - - - - - - - - - - - - - - - - - - MovementRoutine - - - - - - - - - - - - - - - - - - - - \\

// Private functions
void MovementRoutine::adjustStartToCurrent(const sf::Vector2f& currentPosition)
{
	if (m_routineMovements.size() == 0) return;
	
	const sf::Vector2f offset = currentPosition - m_routineMovements.front()->getStartingPos();
	m_routineMovements.front()->getStartingPos() += offset;
}

void MovementRoutine::adjustAllToCurrent(const sf::Vector2f& currentPosition)
{
	if (m_routineMovements.size() == 0) return;

	const sf::Vector2f offset = currentPosition - m_routineMovements.front()->getStartingPos();
	for (auto& movement : m_routineMovements) {
		movement->getStartingPos() += offset;
		movement->getEndingPos()   += offset;
	}
}

// Constructors / Destructors
MovementRoutine::MovementRoutine() :
	TransformationRoutine{}, 
	m_routineMovements{}
	{}

MovementRoutine::MovementRoutine(const std::string& name, const MovementRoutineEngine* const movementRoutineEnginePtr) :
	TransformationRoutine{ name, movementRoutineEnginePtr } 
	{}

MovementRoutine::MovementRoutine(const std::string& name, const MovementRoutineEngine* const movementRoutineEnginePtr, const movementInfo& movement) :
	TransformationRoutine{ name, movementRoutineEnginePtr } 
{
	m_routineMovements.emplace_back(movement);
	++m_count;
}

MovementRoutine::MovementRoutine(const std::string& name, const MovementRoutineEngine* const movementRoutineEnginePtr, const std::vector<movementInfo*> movements) :
	TransformationRoutine{ name, movementRoutineEnginePtr },
	m_routineMovements{ movements }
	{}

MovementRoutine::MovementRoutine(const MovementRoutine& obj) : 
	TransformationRoutine{ obj } 
{
	for (const auto& movement : obj.m_routineMovements)
		this->m_routineMovements.emplace_back(*movement);
}

MovementRoutine::~MovementRoutine() { 
	for (auto& movement : m_routineMovements) 
		delete movement; 
	m_routineMovements.clear();
}

// Public functions
void MovementRoutine::addMovement(const movementInfo& movement)
{
	m_routineMovements.emplace_back(movement);
	++m_count;
}

void MovementRoutine::removeMovement(const movementInfo& movement) 
{
	auto it = std::find(m_routineMovements.begin(), m_routineMovements.end(), movement);

	if (it != m_routineMovements.end()) {
		m_routineMovements.erase(it);
		--m_count;
	}
}

void MovementRoutine::clear() 
{
	m_routineMovements.clear();
	m_current	= 0;
	m_count		= 0;
	m_isActive	= false;
	m_isPaused	= false;
	m_isLooping = false;
}

void MovementRoutine::reset() 
{
	for (auto& movement : m_routineMovements)
		movement->reset();

	m_current  = 0;
	m_isActive = false;
	m_isPaused = false;
}

const bool MovementRoutine::start(sf::Shape& shape)
{
	if (m_routineMovements.size() != 0) {
		if (m_adjustAllToCurrentTransform)		  { adjustAllToCurrent(shape.getPosition()); }
		else if (m_adjustStartToCurrentTransform) { adjustStartToCurrent(shape.getPosition()); }

		reset();

		m_current = 0;
		m_isActive = true;
		m_movementRoutineEngine->addMovement(&shape, this);
		return true;
	}

	return false;
}

const bool MovementRoutine::start(sf::Sprite& sprite) 
{
	if (m_routineMovements.size() != 0) {
		if (m_adjustAllToCurrentTransform)		  { adjustAllToCurrent(sprite.getPosition()); }
		else if (m_adjustStartToCurrentTransform) { adjustStartToCurrent(sprite.getPosition()); }

		reset();

		m_current = 0;
		m_isActive = true;
		m_movementRoutineEngine->addMovement(&sprite, this);
		return true;
	}

	return false;
}

const bool MovementRoutine::start(VertexArray2& vertexArray)
{
	if (m_routineMovements.size() != 0) {
		if (m_adjustAllToCurrentTransform)		  { adjustAllToCurrent(vertexArray.getPosition()); }
		else if (m_adjustStartToCurrentTransform) { adjustStartToCurrent(vertexArray.getPosition()); }

		reset();

		m_current = 0;
		m_isActive = true;
		m_movementRoutineEngine->addMovement(&vertexArray, this);
		return true;
	}

	return false;
}

void MovementRoutine::stop(sf::Shape& shape) 
{
	reset();
	m_movementRoutineEngine->stopMovement(shape);
}

void MovementRoutine::stop(sf::Sprite& sprite) {
	reset();
	m_movementRoutineEngine->stopMovement(sprite);
}

void MovementRoutine::stop(VertexArray2& vertexArray)
{
	reset();
	m_movementRoutineEngine->stopMovement(vertexArray);
}

movementInfo* MovementRoutine::getCurrentMovement() 
{
	if (m_current < m_count)
		return m_routineMovements[m_current];
	else 
		return nullptr;
}

const bool MovementRoutine::goToNextMovement(const sf::Shape& shape)
{
	if (m_current < m_count - 1) {
		++m_current;
		return true;
	}
	else {
		if (m_adjustAllToCurrentTransform)		  { adjustAllToCurrent(shape.getPosition()); }
		else if (m_adjustStartToCurrentTransform) { adjustStartToCurrent(shape.getPosition()); }
		reset();

		if (m_isLooping) {
			m_isActive = true;
			return true;
		}

		return false;
	}
}

const bool MovementRoutine::goToNextMovement(const sf::Sprite& sprite)
{
	if (m_current < m_count - 1) {
		++m_current;
		return true;
	}
	else {
		if (m_adjustAllToCurrentTransform)		  { adjustAllToCurrent(sprite.getPosition()); }
		else if (m_adjustStartToCurrentTransform) { adjustStartToCurrent(sprite.getPosition()); }
		reset();

		if (m_isLooping) {
			m_isActive = true;
			return true;
		}

		return false;
	}
}

const bool MovementRoutine::goToNextMovement(VertexArray2& vertexArray)
{
	if (m_current < m_count - 1) {
		++m_current;
		return true;
	}
	else {
		if (m_adjustAllToCurrentTransform)		  { adjustAllToCurrent(vertexArray.getPosition()); }
		else if (m_adjustStartToCurrentTransform) { adjustStartToCurrent(vertexArray.getPosition()); }
		reset();

		if (m_isLooping) {
			m_isActive = true;
			return true;
		}

		return false;
	}
}

const long long int& MovementRoutine::size() const
{
	long long int size{};
	for (const auto& movement : m_routineMovements) 
		size += sizeof(*movement);
	return size + sizeof(this);
}

// - - - - - - - - - - - - - - - - - - - - ScalingRoutine - - - - - - - - - - - - - - - - - - - - \\

// Private functions
void ScalingRoutine::adjustStartToCurrent(const sf::Vector2f& current_scale)
{
	if (m_routineScalings.size() == 0) return;
	m_routineScalings.front()->starting_scale = current_scale;
}

void ScalingRoutine::adjustAllToCurrent(const sf::Vector2f& current_scale)
{
	if (m_routineScalings.size() == 0) return;

	const sf::Vector2f proportion{ current_scale.x / m_routineScalings.front()->starting_scale.x, current_scale.y / m_routineScalings.front()->starting_scale.y };

	if (proportion == sf::Vector2f{ 1.f, 1.f }) return;

	for (auto& scaling : m_routineScalings) {
		scaling->starting_scale = sf::Vector2f{ scaling->starting_scale.x * proportion.x, scaling->starting_scale.y * proportion.y };
		scaling->ending_scale = sf::Vector2f{ scaling->ending_scale.x * proportion.x, scaling->ending_scale.y * proportion.y };
	}
}

// Constructors / Destructors
ScalingRoutine::ScalingRoutine() :
	TransformationRoutine{}, 
	m_routineScalings{}
	{}

ScalingRoutine::ScalingRoutine(const std::string& name, const MovementRoutineEngine* const movementRoutineEnginePtr) :
	TransformationRoutine{ name, movementRoutineEnginePtr } 
	{}

ScalingRoutine::ScalingRoutine(const std::string& name, const MovementRoutineEngine* const movementRoutineEnginePtr, const scalingInfo& scaling) :
	TransformationRoutine{ name, movementRoutineEnginePtr } 
{ 
	this->m_routineScalings.emplace_back(scaling);
}

ScalingRoutine::ScalingRoutine(const std::string& name, const MovementRoutineEngine* const movementRoutineEnginePtr, const std::vector<scalingInfo*> scalings) :
	TransformationRoutine{ name, movementRoutineEnginePtr },
	m_routineScalings{ scalings }
	{}

ScalingRoutine::ScalingRoutine(const ScalingRoutine & obj) : 
	TransformationRoutine{ obj } 
{
	for (const auto& scaling : obj.m_routineScalings)
		this->m_routineScalings.emplace_back(*scaling);
}

ScalingRoutine::~ScalingRoutine()
{
	for (auto& scaling : m_routineScalings) 
		delete scaling; 
	m_routineScalings.clear();
}

// Public functions
void ScalingRoutine::addScaling(const scalingInfo& scaling)
{
	m_routineScalings.emplace_back(scaling);
	++m_count;
}

void ScalingRoutine::removeScaling(const scalingInfo& scaling)
{
	auto it = std::find(m_routineScalings.begin(), m_routineScalings.end(), scaling);

	if (it != m_routineScalings.end()) {
		m_routineScalings.erase(it); 
		--m_count;
	}
}

void ScalingRoutine::clear()
{
	m_routineScalings.clear();
	m_current = 0;
	m_count = 0;
	m_isActive = false;
	m_isPaused = false;
	m_isLooping = false;
}

void ScalingRoutine::reset()
{
	for (auto& scaling : m_routineScalings)
		scaling->reset();

	m_current = 0;
	m_isActive = false;
	m_isPaused = false;
}

const bool ScalingRoutine::start(sf::Shape& shape)
{
	if (m_routineScalings.size() != 0) {
		if (m_adjustAllToCurrentTransform)		  { adjustAllToCurrent(shape.getScale()); }
		else if (m_adjustStartToCurrentTransform) { adjustStartToCurrent(shape.getScale()); }
		reset();

		m_current = 0;
		m_isActive = true;
		m_movementRoutineEngine->addScaling(&shape, this);
		return true;
	}

	return false;
}

const bool ScalingRoutine::start(sf::Sprite& sprite)
{
	if (m_routineScalings.size() != 0) {
		if (m_adjustAllToCurrentTransform)		  { adjustAllToCurrent(sprite.getScale()); }
		else if (m_adjustStartToCurrentTransform) { adjustStartToCurrent(sprite.getScale()); }
		reset();

		m_current = 0;
		m_isActive = true;
		m_movementRoutineEngine->addScaling(&sprite, this);
		return true;
	}

	return false;
}

const bool ScalingRoutine::start(VertexArray2& vertexArray)
{
	if (m_routineScalings.size() != 0) {
		if (m_adjustAllToCurrentTransform)		  { adjustAllToCurrent(vertexArray.getScale()); }
		else if (m_adjustStartToCurrentTransform) { adjustStartToCurrent(vertexArray.getScale()); }
		reset();

		m_current = 0;
		m_isActive = true;
		m_movementRoutineEngine->addScaling(&vertexArray, this);
		return true;
	}

	return false;
}

void ScalingRoutine::stop(sf::Shape& shape)
{
	reset();
	m_movementRoutineEngine->stopScaling(&shape);
}

void ScalingRoutine::stop(sf::Sprite& sprite)
{
	reset();
	m_movementRoutineEngine->stopScaling(&sprite);
}

void ScalingRoutine::stop(VertexArray2& vertexArray)
{
	reset();
	m_movementRoutineEngine->stopScaling(&vertexArray);
}

scalingInfo* ScalingRoutine::getCurrentScaling()
{
	if (m_current < m_count)
		return m_routineScalings[m_current];
	else 
		return nullptr;
}

const bool ScalingRoutine::goToNextScaling(const sf::Shape& shape)
{
	if (m_current < m_count - 1) {
		++m_current;
		return true;
	}
	else {
		if (m_adjustAllToCurrentTransform)		  { adjustAllToCurrent(shape.getScale()); }
		else if (m_adjustStartToCurrentTransform) { adjustStartToCurrent(shape.getScale()); }
		reset();

		if (m_isLooping) {
			m_isActive = true;
			return true;
		}

		return false;
	}
}

const bool ScalingRoutine::goToNextScaling(const sf::Sprite& sprite)
{
	if (m_current < m_count - 1) {
		++m_current;
		return true;
	}
	else {
		if (m_adjustAllToCurrentTransform)		  { adjustAllToCurrent(sprite.getScale()); }
		else if (m_adjustStartToCurrentTransform) { adjustStartToCurrent(sprite.getScale()); }
		reset();

		if (m_isLooping) {
			m_isActive = true;
			return true;
		}

		return false;
	}
}

const bool ScalingRoutine::goToNextScaling(VertexArray2& vertexArray)
{
	if (m_current < m_count - 1) {
		++m_current;
		return true;
	}
	else {
		if (m_adjustAllToCurrentTransform)		  { adjustAllToCurrent(vertexArray.getScale()); }
		else if (m_adjustStartToCurrentTransform) { adjustStartToCurrent(vertexArray.getScale()); }
		reset();

		if (m_isLooping) {
			m_isActive = true;
			return true;
		}

		return false;
	}
}

const long long int& ScalingRoutine::size() const
{
	long long int size{};
	for (const auto& scaling : m_routineScalings) size += sizeof(*scaling);
	return size + sizeof(this);
}

// - - - - - - - - - - - - - - - - - - - - RotationRoutine - - - - - - - - - - - - - - - - - - - - \\

// Private functions
void RotationRoutine::adjustStartToCurrent(float current_rotation)
{
	if (m_routineRotations.size() == 0) return;

	const rotationInfo& rotation = *m_routineRotations.front();

	if (!was_last_clockwise) {
		if (rotation.clockwise) {
			if (current_rotation > rotation.ending_rotation)
				current_rotation -= 360.f;
		}
		else {
			current_rotation -= 360.f;
			current_rotation *= -1.f;

			if (current_rotation > rotation.ending_rotation)
				current_rotation -= 360.f;
		}
	}
	else {
		if (rotation.clockwise) {
			if (current_rotation > rotation.ending_rotation)
				current_rotation -= 360.f;
		}
		else if (rotation.clockwise == false) {
			current_rotation -= 360.f;
			current_rotation *= -1.f;

			if (current_rotation > rotation.ending_rotation)
				current_rotation -= 360.f;
		}
	}

	if (rotation.clockwise)
		m_routineRotations.front()->starting_rotation = current_rotation;
	else
		m_routineRotations.front()->starting_rotation = 360.f - current_rotation;
}

void RotationRoutine::adjustAllToCurrent(const float current_rotation)
{
	if (m_routineRotations.size() == 0) return;

	float rotation_offset = current_rotation - m_routineRotations.front()->starting_rotation;

	if (!m_routineRotations.front()->clockwise)
		rotation_offset = 360.f + rotation_offset;

	if (rotation_offset == 0.f || rotation_offset == 360.f) return;

	for (auto& rotation : m_routineRotations) {
		float& new_starting_rotation = rotation->starting_rotation;
		float& new_ending_rotation = rotation->ending_rotation;

		new_starting_rotation += rotation_offset;
		new_ending_rotation += rotation_offset;
	}
}

// Constructors / Destructors
RotationRoutine::RotationRoutine() :
	TransformationRoutine{}, 
	m_routineRotations{}
	{}

RotationRoutine::RotationRoutine(const std::string& name, const MovementRoutineEngine* const movementRoutineEnginePtr) :
	TransformationRoutine{ name, movementRoutineEnginePtr } 
	{}

RotationRoutine::RotationRoutine(const std::string& name, const MovementRoutineEngine* const movementRoutineEnginePtr, const rotationInfo& rotation) :
	TransformationRoutine{ name, movementRoutineEnginePtr } 
{
	m_routineRotations.emplace_back(rotation);
	++m_count;
}

RotationRoutine::RotationRoutine(const std::string& name, const MovementRoutineEngine* const movementRoutineEnginePtr, const std::vector<rotationInfo*> rotations) :
	TransformationRoutine{ name, movementRoutineEnginePtr },
	m_routineRotations{ rotations }
	{}

RotationRoutine::RotationRoutine(const RotationRoutine& obj) :
	TransformationRoutine{ obj },
	was_last_clockwise{ obj.was_last_clockwise }
{
	for (const auto& rotation : obj.m_routineRotations)
		this->m_routineRotations.emplace_back(*rotation);
}

RotationRoutine::~RotationRoutine()
{
	for (auto& rotation : m_routineRotations) 
		delete rotation; 
	m_routineRotations.clear();
}

// Public functions
void RotationRoutine::addRotation(const rotationInfo& rotation)
{
	was_last_clockwise = rotation.clockwise;
	m_routineRotations.emplace_back(rotation);
	++m_count;
}

void RotationRoutine::removeRotation(const rotationInfo& rotation)
{
	auto it = std::find(m_routineRotations.begin(), m_routineRotations.end(), rotation);
	if (it != m_routineRotations.end()) { 
		m_routineRotations.erase(it); 
		--m_count;
	}
}

void RotationRoutine::clear()
{
	m_routineRotations.clear();
	m_current = 0;
	m_count = 0;
	m_isActive = false;
	m_isPaused = false;
	m_isLooping = false;
}

void RotationRoutine::reset()
{
	for (auto& rotation : m_routineRotations)
		rotation->reset();

	m_current = 0;
	m_isActive = false;
	m_isPaused = false;
}

const bool RotationRoutine::start(sf::Shape& shape)
{
	if (m_routineRotations.size() != 0) {
		if (m_adjustAllToCurrentTransform)		  { adjustAllToCurrent(shape.getRotation()); }
		else if (m_adjustStartToCurrentTransform) { adjustStartToCurrent(shape.getRotation()); }
		reset();

		m_current = 0;
		m_isActive = true;
		m_movementRoutineEngine->addRotation(&shape, this);
		return true;
	}

	return false;
}

const bool RotationRoutine::start(sf::Sprite& sprite)
{
	if (m_routineRotations.size() != 0) {
		if (m_adjustAllToCurrentTransform)		  { adjustAllToCurrent(sprite.getRotation()); }
		else if (m_adjustStartToCurrentTransform) { adjustStartToCurrent(sprite.getRotation()); }
		reset();

		m_current = 0;
		m_isActive = true;
		m_movementRoutineEngine->addRotation(&sprite, this);
		return true;
	}

	return false;
}

const bool RotationRoutine::start(VertexArray2& vertexArray)
{
	if (m_routineRotations.size() != 0) {
		if (m_adjustAllToCurrentTransform)		  { adjustAllToCurrent(vertexArray.getRotation()); }
		else if (m_adjustStartToCurrentTransform) { adjustStartToCurrent(vertexArray.getRotation()); }
		reset();

		m_current = 0;
		m_isActive = true;
		m_movementRoutineEngine->addRotation(&vertexArray, this);
		return true;
	}

	return false;
}

void RotationRoutine::stop(sf::Shape& shape)
{
	reset();
	m_movementRoutineEngine->stopRotation(&shape);
}

void RotationRoutine::stop(sf::Sprite& sprite)
{
	reset();
	m_movementRoutineEngine->stopRotation(&sprite);
}

void RotationRoutine::stop(VertexArray2& vertexArray)
{
	reset();
	m_movementRoutineEngine->stopRotation(&vertexArray);
}

rotationInfo* RotationRoutine::getCurrentRotation()
{
	if (m_current < m_count)
		return m_routineRotations[m_current];
	else 
		return nullptr;
}

const bool RotationRoutine::goToNextRotation(const sf::Shape& shape)
{
	if (m_current < m_count - 1) {
		was_last_clockwise = m_routineRotations[m_current]->clockwise;
		++m_current;
		return true;
	}
	else {
		was_last_clockwise = m_routineRotations[m_current]->clockwise;
		if (m_adjustAllToCurrentTransform)		  { adjustAllToCurrent(shape.getRotation()); }
		else if (m_adjustStartToCurrentTransform) { adjustStartToCurrent(shape.getRotation()); }
		reset();

		if (m_isLooping) {
			m_isActive = true;
			return true;
		}

		return false;
	}
}

const bool RotationRoutine::goToNextRotation(const sf::Sprite& sprite)
{
	if (m_current < m_count - 1) {
		was_last_clockwise = m_routineRotations[m_current]->clockwise;
		++m_current;
		return true;
	}
	else {
		was_last_clockwise = m_routineRotations[m_current]->clockwise;
		if (m_adjustAllToCurrentTransform)		  { adjustAllToCurrent(sprite.getRotation()); }
		else if (m_adjustStartToCurrentTransform) { adjustStartToCurrent(sprite.getRotation()); }
		reset();

		if (m_isLooping) {
			m_isActive = true;
			return true;
		}

		return false;
	}
}

const bool RotationRoutine::goToNextRotation(VertexArray2& vertexArray)
{
	if (m_current < m_count - 1) {
		was_last_clockwise = m_routineRotations[m_current]->clockwise;
		++m_current;
		return true;
	}
	else {
		was_last_clockwise = m_routineRotations[m_current]->clockwise;
		if (m_adjustAllToCurrentTransform)		  { adjustAllToCurrent(vertexArray.getRotation()); }
		else if (m_adjustStartToCurrentTransform) { adjustStartToCurrent(vertexArray.getRotation()); }
		reset();

		if (m_isLooping) {
			m_isActive = true;
			return true;
		}

		return false;
	}
}

const long long int& RotationRoutine::size() const
{
	long long int size{};
	for (const auto& rotation : m_routineRotations) size += sizeof(*rotation);
	return size + sizeof(this);
}