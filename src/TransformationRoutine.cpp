#pragma once
#include "TransformationRoutine.h"
#include "MovementContainer.h"

// - - - - - - - - - - - - - - - - - - - - MovementRoutine - - - - - - - - - - - - - - - - - - - - \\

void MovementRoutine::adjustStartToCurrent(sf::Vector2f current_position)
{
	if (this->routine_movements.size() == 0) return;
	
	const sf::Vector2f offset = current_position - this->routine_movements[0]->getStartingPos();
	this->routine_movements[0]->adjustStartingPositionByOffset(offset);
}

void MovementRoutine::adjustAllToCurrent(sf::Vector2f current_position)
{
	if (this->routine_movements.size() == 0) return;

	const sf::Vector2f offset = current_position - this->routine_movements[0]->getStartingPos();
	for (auto& movement : this->routine_movements) 
		movement->adjustMovementByOffset(offset);
}

void MovementRoutine::addMovement(movementInfo* movement)
{
	this->routine_movements.emplace_back(movement);
	++this->count;
}

void MovementRoutine::removeMovement(movementInfo* movement) 
{
	auto it = std::find(this->routine_movements.begin(), this->routine_movements.end(), movement);

	if (it != this->routine_movements.end()) {
		this->routine_movements.erase(it);
		--this->count;
	}
}

void MovementRoutine::clear() 
{
	this->routine_movements.clear();
	this->current = 0;
	this->count = 0;
	this->is_active = false;
	this->is_paused = false;
	this->is_looping = false;
}

void MovementRoutine::reset() 
{
	for (auto& movement : this->routine_movements)
		movement->reset();

	this->current = 0;
	this->is_active = false;
	this->is_paused = false;
}

const bool MovementRoutine::start(sf::Shape* shape) 
{
	if (!this->is_active && this->routine_movements.size() != 0) {
		if (this->adjust_all_to_current_transform)		  { this->adjustAllToCurrent(shape->getPosition()); }
		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(shape->getPosition()); }

		this->reset();

		this->current = 0;
		this->is_active = true;
		this->movementRoutineEngine->addMovement(shape, this);
		return true;
	}

	printf("MovementRoutine::start: Routine is empty\n");
	return false;
}

const bool MovementRoutine::start(sf::Sprite* sprite) 
{
	if (this->routine_movements.size() != 0) {
		if (this->adjust_all_to_current_transform)		  { this->adjustAllToCurrent(sprite->getPosition()); }
		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(sprite->getPosition()); }

		this->reset();

		this->current = 0;
		this->is_active = true;
		this->movementRoutineEngine->addMovement(sprite, this);
		return true;
	}

	printf("MovementRoutine::start: Routine is empty\n");
	return false;
}

void MovementRoutine::stop(sf::Shape* shape) 
{
	this->reset();
	this->movementRoutineEngine->stopMovement(shape);
}

void MovementRoutine::stop(sf::Sprite* sprite) {
	this->reset();
	this->movementRoutineEngine->stopMovement(sprite);
}

movementInfo* MovementRoutine::getCurrentMovement() 
{
	if (this->current < this->count)
		return this->routine_movements[this->current];
	else {
		printf("MovementRoutine::getCurrentMovement: Current movement index out of range\n");
		return nullptr;
	}
}

const bool MovementRoutine::goToNextMovement(sf::Shape* shape)
{
	if (this->current < this->count - 1) {
		++this->current;
		return true;
	}
	else {
		if (this->adjust_all_to_current_transform)		  { this->adjustAllToCurrent(shape->getPosition()); }
		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(shape->getPosition()); }
		this->reset();

		if (this->is_looping) {
			this->is_active = true;
			return true;
		}

		return false;
	}
}

const bool MovementRoutine::goToNextMovement(sf::Sprite* sprite)
{
	if (this->current < this->count - 1) {
		++this->current;
		return true;
	}
	else {
		if (this->adjust_all_to_current_transform)		  { this->adjustAllToCurrent(sprite->getPosition()); }
		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(sprite->getPosition()); }
		this->reset();

		if (this->is_looping) {
			this->is_active = true;
			return true;
		}

		return false;
	}
}

// - - - - - - - - - - - - - - - - - - - - MovementRoutineVA - - - - - - - - - - - - - - - - - - - - \\

void MovementRoutineVA::adjustVertexarrayToStartingPosition(sf::VertexArray* vertexarray)
{
	movementInfoVA* current_movement = this->getCurrentMovement();
	if (current_movement == nullptr) return;

	sf::Vector2f centroid{};
	for (size_t i = 0; i < vertexarray->getVertexCount(); ++i) 
		centroid += vertexarray->operator[](i).position;
	centroid /= static_cast<float>(vertexarray->getVertexCount());

	sf::Vector2f offset = current_movement->starting_pos - centroid;
	for (size_t i = 0; i < vertexarray->getVertexCount(); ++i) 
		vertexarray->operator[](i).position += offset;
}

void MovementRoutineVA::adjustStartToCurrent(sf::VertexArray* vertexArray)
{
	if (this->routine_movements.size() == 0) return;

	sf::Vector2f centroid{};
	for (size_t i = 0; i < vertexArray->getVertexCount(); ++i) 
		centroid += vertexArray->operator[](i).position;
	centroid /= static_cast<float>(vertexArray->getVertexCount());

	this->routine_movements[0]->starting_pos = centroid;
	this->routine_movements[0]->moveCentroidsToStartingPosition();
}

void MovementRoutineVA::adjustAllToCurrent(sf::VertexArray* vertexArray)
{
	if (this->routine_movements.size() == 0) return;

	sf::Vector2f centroid{};
	for (size_t i = 0; i < vertexArray->getVertexCount(); ++i)
		centroid += vertexArray->operator[](i).position;
	centroid /= static_cast<float>(vertexArray->getVertexCount());

	const sf::Vector2f offset = this->routine_movements[0]->starting_pos - centroid;
	for (auto& movement : this->routine_movements) {
		movement->starting_pos -= offset;
		movement->ending_pos -= offset;
	}
}

void MovementRoutineVA::addMovement(movementInfoVA * movement)
{
	this->routine_movements.emplace_back(movement);
	++this->count;
}

void MovementRoutineVA::removeMovement(movementInfoVA* movement)
{
	auto it = std::find(this->routine_movements.begin(), this->routine_movements.end(), movement);
	if (it != this->routine_movements.end()) { this->routine_movements.erase(it); --this->count; }
}

void MovementRoutineVA::clear()
{
	this->routine_movements.clear();
	this->current = 0;
	this->count = 0;
	this->is_active = false;
	this->is_paused = false;
	this->is_looping = false;
}

void MovementRoutineVA::reset()
{
	for (auto& movement : this->routine_movements) {
		movement->reset();
		movement->moveCentroidsToStartingPosition();
	}

	this->current = 0;
	this->is_active = false;
	this->is_paused = false;
}

const bool MovementRoutineVA::start(sf::VertexArray* vertexArray)
{
	if (this->routine_movements.size() != 0) {
		if (this->adjust_all_to_current_transform) { this->adjustAllToCurrent(vertexArray); }
		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(vertexArray); }

		this->reset();
		
		this->adjustVertexarrayToStartingPosition(vertexArray);

		this->current = 0;
		this->is_active = true;
		this->movementRoutineEngine->addMovement(vertexArray, this);
		return true;
	}

	printf("MovementRoutineVA::start: Routine is empty\n");
	return false;
}

void MovementRoutineVA::stop(sf::VertexArray* vertexArray)
{
	this->reset();
	this->movementRoutineEngine->stopMovement(vertexArray);
}

movementInfoVA* MovementRoutineVA::getCurrentMovement()
{
	if (this->current < this->count)
		return this->routine_movements[this->current];
	else {
		printf("MovementRoutineVA::getCurrentMovement: Current movement index out of range\n");
		return nullptr;
	}
}

const bool MovementRoutineVA::goToNextMovement(sf::VertexArray* vertexArray)
{
	if (this->current < this->count - 1) {
		++this->current;
		this->adjustVertexarrayToStartingPosition(vertexArray);
		return true;
	}
	else {
		if (this->adjust_all_to_current_transform)		  { this->adjustAllToCurrent(vertexArray); }
		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(vertexArray); }
		this->reset();

		if (this->is_looping) {
			this->adjustVertexarrayToStartingPosition(vertexArray);
			this->is_active = true;
			return true;
		}

		return false;
	}
}

// - - - - - - - - - - - - - - - - - - - - ScalingRoutine - - - - - - - - - - - - - - - - - - - - \\

void ScalingRoutine::addScaling(scalingInfo* scaling)
{
	this->routine_scalings.emplace_back(scaling);
	++this->count;
}

void ScalingRoutine::removeScaling(scalingInfo* scaling)
{
	auto it = std::find(this->routine_scalings.begin(), this->routine_scalings.end(), scaling);
	if (it != this->routine_scalings.end()) { this->routine_scalings.erase(it); --this->count; }
}

void ScalingRoutine::clear()
{
	this->routine_scalings.clear();
	this->current = 0;
	this->count = 0;
	this->is_active = false;
	this->is_paused = false;
	this->is_looping = false;
}

void ScalingRoutine::reset()
{
	for (auto& scaling : this->routine_scalings)
		scaling->reset();

	this->current = 0;
	this->is_active = false;
	this->is_paused = false;
}

const bool ScalingRoutine::start(sf::Shape* shape)
{
	if (this->routine_scalings.size() != 0) {
		this->reset();

		this->current = 0;
		this->is_active = true;
		this->movementRoutineEngine->addScaling(shape, this);
		return true;
	}

	printf("ScalingRoutine::start: Routine is empty\n");
	return false;
}

const bool ScalingRoutine::start(sf::Sprite* sprite)
{
	if (this->routine_scalings.size() != 0) {
		this->reset();

		this->current = 0;
		this->is_active = true;
		this->movementRoutineEngine->addScaling(sprite, this);
		return true;
	}

	printf("ScalingRoutine::start: Routine is empty\n");
	return false;
}

void ScalingRoutine::stop(sf::Shape* shape)
{
	this->reset();
	this->movementRoutineEngine->stopScaling(shape);
}

void ScalingRoutine::stop(sf::Sprite* sprite)
{
	this->reset();
	this->movementRoutineEngine->stopScaling(sprite);
}

scalingInfo* ScalingRoutine::getCurrentScaling()
{
	if (this->current < this->count)
		return this->routine_scalings[this->current];
	else {
		printf("ScalingRoutine::getCurrentScaling: Current scaling index out of range\n");
		return nullptr;
	}
}

const bool ScalingRoutine::goToNextScaling()
{
	if (this->current < this->count - 1) {
		++this->current;
		return true;
	}
	else {
		this->reset();
		if (this->is_looping) {
			this->is_active = true;
			return true;
		}

		return false;
	}
}

// - - - - - - - - - - - - - - - - - - - - ScalingRoutineVA - - - - - - - - - - - - - - - - - - - - \\

void ScalingRoutineVA::addScaling(scalingInfoVA* scaling)
{
	this->routine_scalings.emplace_back(scaling);
	++this->count;
}

void ScalingRoutineVA::removeScaling(scalingInfoVA* scaling)
{
	auto it = std::find(this->routine_scalings.begin(), this->routine_scalings.end(), scaling);
	if (it != this->routine_scalings.end()) { this->routine_scalings.erase(it); --this->count; }
}

void ScalingRoutineVA::clear()
{
	this->routine_scalings.clear();
	this->current = 0;
	this->count = 0;
	this->is_active = false;
	this->is_paused = false;
	this->is_looping = false;
}

void ScalingRoutineVA::reset()
{
	for (auto& scaling : this->routine_scalings)
		scaling->reset();

	this->current = 0;
	this->is_active = false;
	this->is_paused = false;
}

const bool ScalingRoutineVA::start(sf::VertexArray* vertexArray)
{
	if (this->routine_scalings.size() != 0) {
		this->reset();

		this->current = 0;
		this->is_active = true;
		this->movementRoutineEngine->addScaling(vertexArray, this);
		return true;
	}

	printf("ScalingRoutineVA::start: Routine is empty\n");
	return false;
}

void ScalingRoutineVA::stop(sf::VertexArray* vertexArray)
{
	this->reset();
	this->movementRoutineEngine->stopScaling(vertexArray);
}

scalingInfoVA* ScalingRoutineVA::getCurrentScaling()
{
	if (this->current < this->count)
		return this->routine_scalings[this->current];
	else {
		printf("ScalingRoutineVA::getCurrentScaling: Current scaling index out of range\n");
		return nullptr;
	}
}

const bool ScalingRoutineVA::goToNextScaling()
{
	if (this->current < this->count - 1) {
		++this->current;
		return true;
	}
	else {
		this->reset();
		if (this->is_looping) {
			this->is_active = true;
			return true;
		}

		return false;
	}
}

// - - - - - - - - - - - - - - - - - - - - RotationRoutine - - - - - - - - - - - - - - - - - - - - \\

void RotationRoutine::addRotation(rotationInfo* rotation)
{
	this->routine_rotations.emplace_back(rotation);
	++this->count;
}

void RotationRoutine::removeRotation(rotationInfo* rotation)
{
	auto it = std::find(this->routine_rotations.begin(), this->routine_rotations.end(), rotation);
	if (it != this->routine_rotations.end()) { this->routine_rotations.erase(it); --this->count; }
}

void RotationRoutine::clear()
{
	this->routine_rotations.clear();
	this->current = 0;
	this->count = 0;
	this->is_active = false;
	this->is_paused = false;
	this->is_looping = false;
}

void RotationRoutine::reset()
{
	for (auto& rotation : this->routine_rotations)
		rotation->reset();

	this->current = 0;
	this->is_active = false;
	this->is_paused = false;
}

const bool RotationRoutine::start(sf::Shape* shape)
{
	if (this->routine_rotations.size() != 0) {
		this->reset();

		this->current = 0;
		this->is_active = true;
		this->movementRoutineEngine->addRotation(shape, this);
		return true;
	}

	printf("RotationRoutine::start: Routine is empty\n");
	return false;
}

const bool RotationRoutine::start(sf::Sprite* sprite)
{
	if (this->routine_rotations.size() != 0) {
		this->reset();

		this->current = 0;
		this->is_active = true;
		this->movementRoutineEngine->addRotation(sprite, this);
		return true;
	}

	printf("RotationRoutine::start: Routine is empty\n");
	return false;
}

void RotationRoutine::stop(sf::Shape* shape)
{
	this->reset();
	this->movementRoutineEngine->stopRotation(shape);
}

void RotationRoutine::stop(sf::Sprite* sprite)
{
	this->reset();
	this->movementRoutineEngine->stopRotation(sprite);
}

rotationInfo* RotationRoutine::getCurrentRotation()
{
	if (this->current < this->count)
		return this->routine_rotations[this->current];
	else {
		printf("RotationRoutine::getCurrentRotation: Current rotation index out of range\n");
		return nullptr;
	}
}

const bool RotationRoutine::goToNextRotation()
{
	if (this->current < this->count - 1) {
		++this->current;
		return true;
	}
	else {
		this->reset();
		if (this->is_looping) {
			this->is_active = true;
			return true;
		}

		return false;
	}
}

// - - - - - - - - - - - - - - - - - - - - RotationVARoutine - - - - - - - - - - - - - - - - - - - - \\

void RotationRoutineVA::addRotation(rotationInfoVA* rotation)
{
	this->routine_rotations.emplace_back(rotation);
	++this->count;
}

void RotationRoutineVA::removeRotation(rotationInfoVA* rotation)
{
	auto it = std::find(this->routine_rotations.begin(), this->routine_rotations.end(), rotation);
	if (it != this->routine_rotations.end()) { this->routine_rotations.erase(it); --this->count; }
}

void RotationRoutineVA::clear()
{
	this->routine_rotations.clear();
	this->current = 0;
	this->count = 0;
	this->is_active = false;
	this->is_paused = false;
	this->is_looping = false;
}

void RotationRoutineVA::reset()
{
	for (auto& rotation : this->routine_rotations)
		rotation->reset();

	this->current = 0;
	this->is_active = false;
	this->is_paused = false;
}

const bool RotationRoutineVA::start(sf::VertexArray* vertexArray)
{
	if (this->routine_rotations.size() != 0) {
		this->reset();

		this->current = 0;
		this->is_active = true;
		this->movementRoutineEngine->addRotation(vertexArray, this);
		return true;
	}

	printf("RotationRoutineVA::start: Routine is empty\n");
	return false;
}

void RotationRoutineVA::stop(sf::VertexArray* vertexArray)
{
	this->reset();
	this->movementRoutineEngine->stopRotation(vertexArray);
}

rotationInfoVA* RotationRoutineVA::getCurrentRotation()
{
	if (this->current < this->count)
		return this->routine_rotations[this->current];
	else {
		printf("RotationRoutineVA::getCurrentRotation: Current rotation index out of range\n");
		return nullptr;
	}
}

const bool RotationRoutineVA::goToNextRotation()
{
	if (this->current < this->count - 1) {
		++this->current;
		return true;
	}
	else {
		this->reset();
		if (this->is_looping) {
			this->is_active = true;
			return true;
		}

		return false;
	}
}