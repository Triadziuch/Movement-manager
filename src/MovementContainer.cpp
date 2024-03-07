#pragma once
#include "MovementManager.h"
#include "MovementContainer.h"
#include "TransformationRoutine.h"

// Singleton initialization
MovementContainer* MovementContainer::sInstance = nullptr;

// Private update functions
void MovementContainer::updateShape(float dt)
{
	for (auto movementRoutine = this->m_Movements_Shape.begin(); movementRoutine != this->m_Movements_Shape.end();) {
		sf::Shape& shape = *movementRoutine->first;
		movementInfo& movement = *movementRoutine->second;

		movement.m_currentTime += dt;

		if (movement.isDone()) {
			if (movement.m_currentTime - movement.m_delayBefore - dt < movement.m_motionDuration)
				shape.setPosition(movement.getEndingPos());

			if (movement.m_repeat) {
				if (movement.m_currentTime - movement.m_motionDuration - movement.m_delayBefore >= movement.m_delayAfter) {
					shape.setPosition(movement.getStartingPos());

					movement.m_currentTime -= movement.m_totalDuration;
				}
			}
			else if (movement.isFinished()) {
				delete movementRoutine->second;
				movementRoutine = m_Movements_Shape.erase(movementRoutine);
				continue;
			}
		}
		else {
			if (movement.m_currentTime > movement.m_delayBefore) 
				shape.setPosition(movement.updatePosition());
			else if (movement.m_currentTime - dt == 0.f)
				shape.setPosition(movement.getStartingPos());
		}

		++movementRoutine;
	}

	for (auto scalingRoutine = this->m_Scalings_Shape.begin(); scalingRoutine != this->m_Scalings_Shape.end();) {
		sf::Shape& shape = *scalingRoutine->first;
		scalingInfo& scaling = *scalingRoutine->second;
		
		scaling.m_currentTime += dt;

		if (scaling.isDone()) {
			if (scaling.m_currentTime - scaling.m_delayBefore - dt < scaling.m_motionDuration)
				shape.setScale(scaling.getEndingScale());

			if (scaling.m_repeat) {
				if (scaling.m_currentTime - scaling.m_motionDuration - scaling.m_delayBefore >= scaling.m_delayAfter) {
					shape.setScale(scaling.getStartingScale());
					
					scaling.m_currentTime -= scaling.m_totalDuration;
				}
			}
			else if (scaling.isFinished()) {
				delete scalingRoutine->second;
				scalingRoutine = m_Scalings_Shape.erase(scalingRoutine);
				continue;
			}
		}
		else {
			if (scaling.m_currentTime > scaling.m_delayBefore) 
				shape.setScale(scaling.updateScale());
			else if (scaling.m_currentTime - dt == 0.f)
				shape.setScale(scaling.getStartingScale());
		}

		++scalingRoutine;
	}

	for (auto rotationRoutine = this->m_Rotations_Shape.begin(); rotationRoutine != this->m_Rotations_Shape.end();) {
		sf::Shape& shape = *rotationRoutine->first;
		rotationInfo& rotation = *rotationRoutine->second;
		
		rotation.m_currentTime += dt;

		if (rotation.isDone()) {
			if (rotation.m_currentTime - rotation.m_delayBefore - dt < rotation.m_motionDuration)
				shape.setRotation(rotation.getEndingRotation());

			if (rotation.m_repeat) {
				if (rotation.m_currentTime - rotation.m_motionDuration - rotation.m_delayBefore >= rotation.m_delayAfter) {
					shape.setRotation(rotation.getStartingRotation());

					rotation.m_currentTime -= rotation.m_totalDuration;
				}
			}
			else if (rotation.isFinished()) {
				delete rotationRoutine->second;
				rotationRoutine = m_Rotations_Shape.erase(rotationRoutine);
				continue;
			}
		}
		else{
			if (rotation.m_currentTime > rotation.m_delayBefore)
				shape.setRotation(rotation.updateRotation());
			else if (rotation.m_currentTime - dt == 0.f)
				shape.setRotation(rotation.getStartingRotation());
		}

		++rotationRoutine;
	}
}

void MovementContainer::updateVertexArray(float dt)
{
	for (auto movementRoutine = this->m_Movements_VA.begin(); movementRoutine != this->m_Movements_VA.end();) {
		VertexArray2& vertexarray = *movementRoutine->first;
		movementInfo& movement = *movementRoutine->second;
		
		movement.m_currentTime += dt;

		if (vertexarray.getVertexCount() != 0) {
			if (movement.isDone()) {
				if (movement.m_currentTime - movement.m_delayBefore - dt < movement.m_motionDuration) 
					vertexarray.setPosition(movement.getEndingPos());

				if (movement.m_repeat) {
					if (movement.m_currentTime - movement.m_motionDuration - movement.m_delayBefore >= movement.m_delayAfter) {
						vertexarray.setPosition(movement.getStartingPos());
						movement.m_currentTime -= movement.m_totalDuration;
					}
				}
				else if (movement.isFinished()) {
					delete movementRoutine->second;
					movementRoutine = m_Movements_VA.erase(movementRoutine);
					continue;
				}
			}
			else {
				if (movement.m_currentTime > movement.m_delayBefore) 
					vertexarray.setPosition(movement.updatePosition());
				else if (movement.m_currentTime - dt == 0.f) 
					vertexarray.setPosition(movement.getStartingPos());
			}
		}

		++movementRoutine;
	}

	for (auto scalingRoutine = this->m_Scalings_VA.begin(); scalingRoutine != this->m_Scalings_VA.end();) {
		VertexArray2& vertexarray = *scalingRoutine->first;
		scalingInfo& scaling = *scalingRoutine->second;

		scaling.m_currentTime += dt;

		if (vertexarray.getVertexCount() != 0) {
			if (scaling.isDone()) {
				if (scaling.m_currentTime - scaling.m_delayBefore - dt < scaling.m_motionDuration) 
					vertexarray.setScale(scaling.getEndingScale());

				if (scaling.m_repeat) {
					if (scaling.m_currentTime - scaling.m_motionDuration - scaling.m_delayBefore >= scaling.m_delayAfter) {
						vertexarray.setScale(scaling.getStartingScale());
						scaling.m_currentTime -= scaling.m_totalDuration;
					}
				}
				else if (scaling.isFinished()) {
					delete scalingRoutine->second;
					scalingRoutine = m_Scalings_VA.erase(scalingRoutine);
					continue;
				}
			}
			else {
				if (scaling.m_currentTime > scaling.m_delayBefore)
					vertexarray.setScale(scaling.updateScale());
				else if (scaling.m_currentTime - dt == 0.f) 
					vertexarray.setScale(scaling.getStartingScale());
			}
		}

		++scalingRoutine;
	}

	for (auto rotationRoutine = this->m_Rotations_VA.begin(); rotationRoutine != this->m_Rotations_VA.end();) {
		VertexArray2& vertexarray = *rotationRoutine->first;
		rotationInfo& rotation = *rotationRoutine->second;

		rotation.m_currentTime += dt;

		if (vertexarray.getVertexCount() != 0) {
			if (rotation.isDone()) {
				if (rotation.m_currentTime - rotation.m_delayBefore - dt < rotation.m_motionDuration)
					vertexarray.setRotation(rotation.getEndingRotation());

				if (rotation.m_repeat) {
					if (rotation.m_currentTime - rotation.m_motionDuration - rotation.m_delayBefore >= rotation.m_delayAfter){
						vertexarray.setRotation(rotation.getStartingRotation());
						rotation.m_currentTime -= rotation.m_totalDuration;
					}
				}
				else if (rotation.isFinished()) {
					delete rotationRoutine->second;
					rotationRoutine = m_Rotations_VA.erase(rotationRoutine);
					continue;
				}
			}
			else{
				if (rotation.m_currentTime > rotation.m_delayBefore) 
					vertexarray.setRotation(rotation.updateRotation());
				else if (rotation.m_currentTime - dt == 0.f) 
					vertexarray.setRotation(rotation.getStartingRotation());
			}
		}

		++rotationRoutine;
	}
}

void MovementContainer::updateSprite(float dt)
{
	for (auto movementRoutine = this->m_Movements_S.begin(); movementRoutine != this->m_Movements_S.end();) {
		sf::Sprite& sprite = *movementRoutine->first;
		movementInfo& movement = *movementRoutine->second;

		movement.m_currentTime += dt;

		if (movement.isDone()) {
			if (movement.m_currentTime - movement.m_delayBefore - dt < movement.m_motionDuration)
				sprite.setPosition(movement.getEndingPos());

			if (movement.m_repeat) {
				if (movement.m_currentTime - movement.m_motionDuration - movement.m_delayBefore >= movement.m_delayAfter) {
					sprite.setPosition(movement.getStartingPos());

					movement.m_currentTime -= movement.m_totalDuration;
				}
			}
			else if (movement.isFinished()) {
				delete movementRoutine->second;
				movementRoutine = m_Movements_S.erase(movementRoutine);
				continue;
			}
		}
		else {
			if (movement.m_currentTime > movement.m_delayBefore)
				sprite.setPosition(movement.updatePosition());
			else if (movement.m_currentTime - dt == 0.f)
				sprite.setPosition(movement.getStartingPos());
		}

		++movementRoutine;
	}

	for (auto scalingRoutine = this->m_Scalings_S.begin(); scalingRoutine != this->m_Scalings_S.end();) {
		sf::Sprite& sprite = *scalingRoutine->first;
		scalingInfo& scaling = *scalingRoutine->second;

		scaling.m_currentTime += dt;

		if (scaling.isDone()) {
			if (scaling.m_currentTime - scaling.m_delayBefore - dt < scaling.m_motionDuration)
				sprite.setScale(scaling.getEndingScale());

			if (scaling.m_repeat) {
				if (scaling.m_currentTime - scaling.m_motionDuration - scaling.m_delayBefore >= scaling.m_delayAfter) {
					sprite.setScale(scaling.getStartingScale());

					scaling.m_currentTime -= scaling.m_totalDuration;
				}
			}
			else if (scaling.isFinished()) {
				delete scalingRoutine->second;
				scalingRoutine = m_Scalings_S.erase(scalingRoutine);
				continue;
			}
		}
		else {
			if (scaling.m_currentTime > scaling.m_delayBefore)
				sprite.setScale(scaling.updateScale());
			else if (scaling.m_currentTime - dt == 0.f)
				sprite.setScale(scaling.getStartingScale());
		}

		++scalingRoutine;
	}

	for (auto rotationRoutine = this->m_Rotations_S.begin(); rotationRoutine != this->m_Rotations_S.end();) {
		sf::Sprite& sprite = *rotationRoutine->first;
		rotationInfo& rotation = *rotationRoutine->second;

		rotation.m_currentTime += dt;

		if (rotation.isDone()) {
			if (rotation.m_currentTime - rotation.m_delayBefore - dt < rotation.m_motionDuration)
				sprite.setRotation(rotation.getEndingRotation());

			if (rotation.m_repeat) {
				if (rotation.m_currentTime - rotation.m_motionDuration - rotation.m_delayBefore >= rotation.m_delayAfter) {
					sprite.setRotation(rotation.getStartingRotation());

					rotation.m_currentTime -= rotation.m_totalDuration;
				}
			}
			else if (rotation.isFinished()) {
				delete rotationRoutine->second;
				rotationRoutine = m_Rotations_S.erase(rotationRoutine);
				continue;
			}
		}
		else {
			if (rotation.m_currentTime > rotation.m_delayBefore)
				sprite.setRotation(rotation.updateRotation());
			else if (rotation.m_currentTime - dt == 0.f)
				sprite.setRotation(rotation.getStartingRotation());
		}

		++rotationRoutine;
	}
}

// Default constructor
MovementContainer::MovementContainer()
{
	if (sInstance == nullptr) {
		assert(this->sInstance == nullptr);
		sInstance = this;
	}
}

// Update functions
void MovementContainer::update(float dt)
{
	this->updateShape(dt);
	this->updateVertexArray(dt);
	this->updateSprite(dt);
}

// Public functions
const movementInfo* MovementContainer::addMovement(sf::Shape* _shape, movementInfo* _movementInfo)
{
	auto& movementMap = sInstance->m_Movements_Shape;
	auto movementFound = movementMap.find(_shape);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::Shape* object already exists!\n");
		return nullptr;
	}

	movementMap.insert(std::make_pair(_shape, _movementInfo));
	return _movementInfo;
}

// Movement functions
const movementInfo* MovementContainer::addMovement(sf::Shape* _shape, sf::Vector2f _ending_pos, float _movement_time, easeFunctions::Tmovement_function _used_function, bool _repeat, float _delay_before, float _delay_after)
{
	auto& movementMap = sInstance->m_Movements_Shape;
	auto movementFound = movementMap.find(_shape);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::Shape* object already exists!\n");
		return nullptr;
	}
	
	movementInfo* newMovement = new movementInfo(_shape->getPosition(), _ending_pos, _movement_time, easeFunctions::movement_functions[_used_function], _repeat, _delay_before, _delay_after);
	movementMap.insert(std::make_pair(_shape, newMovement));

	return newMovement;
}

const movementInfo* MovementContainer::addMovement(sf::Shape* _shape, sf::Vector2f _starting_pos, sf::Vector2f _ending_pos, float _movement_time, easeFunctions::Tmovement_function _used_function, bool _repeat, float _delay_before, float _delay_after)
{
	auto& movementMap = sInstance->m_Movements_Shape;
	auto movementFound = movementMap.find(_shape);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::Shape* object already exists!\n");
		return nullptr;
	}
	
	movementInfo* newMovement = new movementInfo(_starting_pos, _ending_pos, _movement_time, easeFunctions::movement_functions[_used_function], _repeat, _delay_before, _delay_after);
	movementMap.insert(std::make_pair(_shape, newMovement));

	return newMovement;
}

const movementInfo* MovementContainer::addMovement(sf::Shape* _shape, float _offset_x, float _offset_y, float _movement_time, easeFunctions::Tmovement_function _used_function, bool _repeat, float _delay_before, float _delay_after)
{
	return this->addMovement(_shape, _shape->getPosition() + sf::Vector2f(_offset_x, _offset_y), _movement_time, _used_function, _repeat, _delay_before, _delay_after);
}

const movementInfo* MovementContainer::addMovement(VertexArray2* _vertexarray, movementInfo* _movementInfo)
{
	auto& movementMap = sInstance->m_Movements_VA;
	auto movementFound = movementMap.find(_vertexarray);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::VertexArray* object already exists!\n");
		return nullptr;
	}

	movementMap.insert(std::make_pair(_vertexarray, _movementInfo));
	return _movementInfo;
}

const movementInfo* MovementContainer::addMovement(VertexArray2* _vertexarray, sf::Vector2f _ending_pos, float _movement_time, easeFunctions::Tmovement_function _used_function, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& movementMap = sInstance->m_Movements_VA;
	auto movementFound = movementMap.find(_vertexarray);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::VertexArray* object already exists!\n");
		return nullptr;
	}

	movementInfo* newMovement = new movementInfo(_vertexarray->getPosition(), _ending_pos, _movement_time, easeFunctions::movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after);
	movementMap.insert(std::make_pair(_vertexarray, newMovement));

	return newMovement;
}

const movementInfo* MovementContainer::addMovement(VertexArray2* _vertexarray, sf::Vector2f _starting_pos, sf::Vector2f _ending_pos, float _movement_time, easeFunctions::Tmovement_function _used_function, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& movementMap = sInstance->m_Movements_VA;
	auto movementFound = movementMap.find(_vertexarray);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::VertexArray* object already exists!\n");
		return nullptr;
	}

	movementInfo* newMovement = new movementInfo(_starting_pos, _ending_pos, _movement_time, easeFunctions::movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after);
	movementMap.insert(std::make_pair(_vertexarray, newMovement));

	return newMovement;
}

const movementInfo* MovementContainer::addMovement(VertexArray2* _vertexarray, float _offset_x, float _offset_y, float _movement_time, easeFunctions::Tmovement_function _used_function, bool _repeat, float _m_delayBefore, float _delay_after)
{
	if (_vertexarray->getVertexCount() == 0) {
		printf("ERROR: VertexArray has no vertices!\n");
		return nullptr;
	}
	return this->addMovement(_vertexarray, _vertexarray->getPosition() + sf::Vector2f(_offset_x, _offset_y), _movement_time, _used_function, _repeat, _m_delayBefore, _delay_after);
}

const movementInfo* MovementContainer::addMovement(sf::Sprite* _sprite, movementInfo* _movementInfo)
{
	auto& movementMap = sInstance->m_Movements_S;
	auto movementFound = movementMap.find(_sprite);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::Sprite* object already exists!\n");
		return nullptr;
	}

	movementMap.insert(std::make_pair(_sprite, _movementInfo));
	return _movementInfo;
}

const movementInfo* MovementContainer::addMovement(sf::Sprite* _sprite, sf::Vector2f _ending_pos, float _movement_time, easeFunctions::Tmovement_function _used_function, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& movementMap = sInstance->m_Movements_S;
	auto movementFound = movementMap.find(_sprite);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::Sprite* object already exists!\n");
		return nullptr;
	}
	
	movementInfo* newMovement = new movementInfo(_sprite->getPosition(), _ending_pos, _movement_time, easeFunctions::movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after);
	movementMap.insert(std::make_pair(_sprite, newMovement));

	return newMovement;
}

const movementInfo* MovementContainer::addMovement(sf::Sprite* _sprite, sf::Vector2f _starting_pos, sf::Vector2f _ending_pos, float _movement_time, easeFunctions::Tmovement_function _used_function, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& movementMap = sInstance->m_Movements_S;
	auto movementFound = movementMap.find(_sprite);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::Sprite* object already exists!\n");
		return nullptr;
	}

	movementInfo* newMovement = new movementInfo(_starting_pos, _ending_pos, _movement_time, easeFunctions::movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after);
	movementMap.insert(std::make_pair(_sprite, newMovement));

	return newMovement;
}

const movementInfo* MovementContainer::addMovement(sf::Sprite* _sprite, float _offset_x, float _offset_y, float _movement_time, easeFunctions::Tmovement_function _used_function, bool _repeat, float _m_delayBefore, float _delay_after)
{
	return this->addMovement(_sprite, _sprite->getPosition() + sf::Vector2f(_offset_x, _offset_y), _movement_time, _used_function, _repeat, _m_delayBefore, _delay_after);
}

void MovementContainer::undoMovement()
{
	for (auto movement = this->m_Movements_Shape.begin(); movement != this->m_Movements_Shape.end();) {
		movement->first->setPosition(movement->second->getStartingPos());
		delete movement->second;
		movement = m_Movements_Shape.erase(movement);
	}

	for (auto movement = this->m_Movements_VA.begin(); movement != this->m_Movements_VA.end();) {
		movement->first->setPosition(movement->second->getStartingPos());
		delete movement->second;
		movement = m_Movements_VA.erase(movement);
	}

	for (auto movement = this->m_Movements_S.begin(); movement != this->m_Movements_S.end();) {
		movement->first->setPosition(movement->second->getStartingPos());
		delete movement->second;
		movement = m_Movements_S.erase(movement);
	}
}

void MovementContainer::undoMovement(sf::Shape* _shape)
{
	auto& movementMap = sInstance->m_Movements_Shape;
	auto movementFound = movementMap.find(_shape);

	if (movementFound != movementMap.end()) {
		_shape->setPosition(movementFound->second->getStartingPos());
		delete movementFound->second;
		movementMap.erase(movementFound);
	}
}

void MovementContainer::undoMovement(VertexArray2* _vertexarray)
{
	auto& movementMap = sInstance->m_Movements_VA;
	auto movementFound = movementMap.find(_vertexarray);

	if (movementFound != movementMap.end()) {
		_vertexarray->setPosition(movementFound->second->getStartingPos());
		delete movementFound->second;
		movementMap.erase(movementFound);
	}
}

void MovementContainer::undoMovement(sf::Sprite* _sprite)
{
	auto& movementMap = sInstance->m_Movements_S;
	auto movementFound = movementMap.find(_sprite);

	if (movementFound != movementMap.end()) {
		_sprite->setPosition(movementFound->second->getStartingPos());
		delete movementFound->second;
		movementMap.erase(movementFound);
	}
}

void MovementContainer::resetMovement()
{
	for (auto movement = this->m_Movements_Shape.begin(); movement != this->m_Movements_Shape.end();) {
		movement->first->setPosition(movement->second->getStartingPos());
		movement->second->m_currentTime = 0.f;
		++movement;
	}

	for (auto movement = this->m_Movements_VA.begin(); movement != this->m_Movements_VA.end();) {
		movement->first->setPosition(movement->second->getStartingPos());
		movement->second->m_currentTime = 0.f;
		++movement;
	}

	for (auto movement = this->m_Movements_S.begin(); movement != this->m_Movements_S.end();) {
		movement->first->setPosition(movement->second->getStartingPos());
		movement->second->m_currentTime = 0.f;
		++movement;
	}
}

void MovementContainer::resetMovement(sf::Shape* _shape)
{
	auto& movementMap = sInstance->m_Movements_Shape;
	auto movementFound = movementMap.find(_shape);

	if (movementFound != movementMap.end()) {
		_shape->setPosition(movementFound->second->getStartingPos());
		movementFound->second->m_currentTime = 0.f;
	}
}

void MovementContainer::resetMovement(VertexArray2* _vertexarray)
{
	auto& movementMap = sInstance->m_Movements_VA;
	auto movementFound = movementMap.find(_vertexarray);

	if (movementFound != movementMap.end()) {
		_vertexarray->setPosition(movementFound->second->getStartingPos());
		movementFound->second->m_currentTime = 0.f;
	}	
}

void MovementContainer::resetMovement(sf::Sprite* _sprite)
{
	auto& movementMap = sInstance->m_Movements_S;
	auto movementFound = movementMap.find(_sprite);

	if (movementFound != movementMap.end()) {
		_sprite->setPosition(movementFound->second->getStartingPos());
		movementFound->second->m_currentTime = 0.f;
	}
}

void MovementContainer::stopMovement()
{
	for (auto movement = this->m_Movements_Shape.begin(); movement != this->m_Movements_Shape.end();) {
		delete movement->second;
		movement = m_Movements_Shape.erase(movement);
	}

	for (auto movement = this->m_Movements_VA.begin(); movement != this->m_Movements_VA.end();) {
		delete movement->second;
		movement = m_Movements_VA.erase(movement);
	}

	for (auto movement = this->m_Movements_S.begin(); movement != this->m_Movements_S.end();) {
		delete movement->second;
		movement = m_Movements_S.erase(movement);
	}
}

void MovementContainer::stopMovement(sf::Shape* _shape)
{
	auto& movementMap = sInstance->m_Movements_Shape;
	auto movementFound = movementMap.find(_shape);

	if (movementFound != movementMap.end()) {
		delete movementFound->second;
		movementMap.erase(movementFound);
	}
}

void MovementContainer::stopMovement(VertexArray2* _vertexarray)
{
	auto& movementMap = sInstance->m_Movements_VA;
	auto movementFound = movementMap.find(_vertexarray);

	if (movementFound != movementMap.end()) {
		delete movementFound->second;
		movementMap.erase(movementFound);
	}
}

void MovementContainer::stopMovement(sf::Sprite* _sprite)
{
	auto& movementMap = sInstance->m_Movements_S;
	auto movementFound = movementMap.find(_sprite);

	if (movementFound != movementMap.end()) {
		delete movementFound->second;
		movementMap.erase(movementFound);
	}
}

void MovementContainer::swapMovement(sf::Shape* _shape, movementInfo* _newMovementInfo)
{
	auto& movementMap = sInstance->m_Movements_Shape;
	auto movementFound = movementMap.find(_shape);

	if (movementFound != movementMap.end()) 
		movementFound->second = _newMovementInfo;
	else {
		printf("MovementContainer::swapMovement: Movement for sf::Shape* object does not exist!\n");
		movementMap.insert(std::make_pair(_shape, _newMovementInfo));
	}
}

// Scaling functions
const bool MovementContainer::addScaling(sf::Shape* _shape, sf::Vector2f _ending_scale, float _scaling_time, easeFunctions::Tmovement_function _used_function, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& scalingMap = sInstance->m_Scalings_Shape;
	auto scalingFound = scalingMap.find(_shape);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::Shape* object already exists!\n");
		return 0;
	}
	else {
		scalingInfo* newScaling = new scalingInfo(_shape->getScale(), _ending_scale, _scaling_time, easeFunctions::movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after);
		scalingMap.insert(std::make_pair(_shape, newScaling));
	}
	return 1;
}

const bool MovementContainer::addScaling(sf::Shape* _shape, sf::Vector2f _starting_scale, sf::Vector2f _ending_scale, float _scaling_time, easeFunctions::Tmovement_function _used_function, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& scalingMap = sInstance->m_Scalings_Shape;
	auto scalingFound = scalingMap.find(_shape);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::Shape* object already exists!\n");
		return 0;
	}
	else {
		scalingInfo* newScaling = new scalingInfo(_starting_scale, _ending_scale, _scaling_time, easeFunctions::movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after);
		scalingMap.insert(std::make_pair(_shape, newScaling));
	}
	return 1;
}

const bool MovementContainer::addScaling(VertexArray2* _vertexarray, sf::Vector2f _ending_scale, float _scaling_time, easeFunctions::Tmovement_function _used_function, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& scalingMap = sInstance->m_Scalings_VA;
	auto scalingFound = scalingMap.find(_vertexarray);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::VertexArray* object already exists!\n");
		return 0;
	}
	else if (_vertexarray->getVertexCount() != 0){
		scalingInfo* newScaling = new scalingInfo({ 1.f, 1.f }, _ending_scale, _scaling_time, easeFunctions::movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after);
		scalingMap.insert(std::make_pair(_vertexarray, newScaling));
	}
	return 1;
}

const bool MovementContainer::addScaling(VertexArray2* _vertexarray, sf::Vector2f _starting_scale, sf::Vector2f _ending_scale, float _scaling_time, easeFunctions::Tmovement_function _used_function, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& scalingMap = sInstance->m_Scalings_VA;
	auto scalingFound = scalingMap.find(_vertexarray);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::VertexArray* object already exists!\n");
		return 0;
	}
	else if (_vertexarray->getVertexCount() != 0) {
		scalingInfo* newScaling = new scalingInfo(_starting_scale, _ending_scale, _scaling_time, easeFunctions::movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after);
		scalingMap.insert(std::make_pair(_vertexarray, newScaling));
	}
	return 1;
}

const bool MovementContainer::addScaling(sf::Sprite* _sprite, sf::Vector2f _ending_scale, float _scaling_time, easeFunctions::Tmovement_function _used_function, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& scalingMap = sInstance->m_Scalings_S;
	auto scalingFound = scalingMap.find(_sprite);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::Sprite* object already exists!\n");
		return 0;
	}
	else {
		scalingInfo* newScaling = new scalingInfo(_sprite->getScale(), _ending_scale, _scaling_time, easeFunctions::movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after);
		scalingMap.insert(std::make_pair(_sprite, newScaling));
	}
	return 1;
}

const bool MovementContainer::addScaling(sf::Sprite* _sprite, sf::Vector2f _starting_scale, sf::Vector2f _ending_scale, float _scaling_time, easeFunctions::Tmovement_function _used_function, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& scalingMap = sInstance->m_Scalings_S;
	auto scalingFound = scalingMap.find(_sprite);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::Sprite* object already exists!\n");
		return 0;
	}
	else {
		scalingInfo* newScaling = new scalingInfo(_starting_scale, _ending_scale, _scaling_time, easeFunctions::movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after);
		scalingMap.insert(std::make_pair(_sprite, newScaling));
	}
	return 1;
}

void MovementContainer::undoScaling()
{
	for (auto scaling = this->m_Scalings_Shape.begin(); scaling != this->m_Scalings_Shape.end();) {
		scaling->first->setScale(scaling->second->getStartingScale());
		delete scaling->second;
		scaling = m_Scalings_Shape.erase(scaling);
	}

	for (auto scaling = this->m_Scalings_VA.begin(); scaling != this->m_Scalings_VA.end();) {
		scaling->first->setScale(scaling->second->getStartingScale());
		delete scaling->second;
		scaling = m_Scalings_VA.erase(scaling);
	}

	for (auto scaling = this->m_Scalings_S.begin(); scaling != this->m_Scalings_S.end();) {
		scaling->first->setScale(scaling->second->getStartingScale());
		delete scaling->second;
		scaling = m_Scalings_S.erase(scaling);
	}
}

void MovementContainer::undoScaling(sf::Shape* _shape)
{
	auto& scalingMap = sInstance->m_Scalings_Shape;
	auto scalingFound = scalingMap.find(_shape);

	if (scalingFound != scalingMap.end()) {
		_shape->setScale(scalingFound->second->getStartingScale());
		delete scalingFound->second;
		scalingMap.erase(scalingFound);
	}
}

void MovementContainer::undoScaling(VertexArray2* _vertexarray)
{
	auto& scalingMap = sInstance->m_Scalings_VA;
	auto scalingFound = scalingMap.find(_vertexarray);

	if (scalingFound != scalingMap.end()) {
		_vertexarray->setScale(scalingFound->second->getStartingScale());
		delete scalingFound->second;
		scalingMap.erase(scalingFound);
	}
}

void MovementContainer::undoScaling(sf::Sprite* _sprite)
{
	auto& scalingMap = sInstance->m_Scalings_S;
	auto scalingFound = scalingMap.find(_sprite);

	if (scalingFound != scalingMap.end()) {
		_sprite->setScale(scalingFound->second->getStartingScale());
		delete scalingFound->second;
		scalingMap.erase(scalingFound);
	}
}

void MovementContainer::resetScaling()
{
	for (auto scaling = this->m_Scalings_Shape.begin(); scaling != this->m_Scalings_Shape.end();) {
		scaling->first->setScale(scaling->second->getStartingScale());
		scaling->second->m_currentTime = 0.f;
		++scaling;
	}

	for (auto scaling = this->m_Scalings_VA.begin(); scaling != this->m_Scalings_VA.end();) {
		scaling->first->setScale(scaling->second->getStartingScale());
		scaling->second->m_currentTime = 0.f;
		++scaling;
	}

	for (auto scaling = this->m_Scalings_S.begin(); scaling != this->m_Scalings_S.end();) {
		scaling->first->setScale(scaling->second->getStartingScale());
		scaling->second->m_currentTime = 0.f;
		++scaling;
	}
}

void MovementContainer::resetScaling(sf::Shape* _shape)
{
	auto& scalingMap = sInstance->m_Scalings_Shape;
	auto scalingFound = scalingMap.find(_shape);

	if (scalingFound != scalingMap.end()) {
		_shape->setScale(scalingFound->second->getStartingScale());
		scalingFound->second->m_currentTime = 0.f;
	}
}

void MovementContainer::resetScaling(VertexArray2* _vertexarray)
{
	auto& scalingMap = sInstance->m_Scalings_VA;
	auto scalingFound = scalingMap.find(_vertexarray);

	if (scalingFound != scalingMap.end()) {
		_vertexarray->setScale(scalingFound->second->getStartingScale());
		scalingFound->second->m_currentTime = 0.f;
	}
}

void MovementContainer::resetScaling(sf::Sprite* _sprite)
{
	auto& scalingMap = sInstance->m_Scalings_S;
	auto scalingFound = scalingMap.find(_sprite);

	if (scalingFound != scalingMap.end()) {
		_sprite->setScale(scalingFound->second->getStartingScale());
		scalingFound->second->m_currentTime = 0.f;
	}
}

void MovementContainer::stopScaling()
{
	for (auto scaling = this->m_Scalings_Shape.begin(); scaling != this->m_Scalings_Shape.end();) {
		delete scaling->second;
		scaling = this->m_Scalings_Shape.erase(scaling);
	}

	for (auto scaling = this->m_Scalings_VA.begin(); scaling != this->m_Scalings_VA.end();) {
		delete scaling->second;
		scaling = m_Scalings_VA.erase(scaling);
	}

	for (auto scaling = this->m_Scalings_S.begin(); scaling != this->m_Scalings_S.end();) {
		delete scaling->second;
		scaling = m_Scalings_S.erase(scaling);
	}
}

void MovementContainer::stopScaling(sf::Shape* _shape)
{
	auto& scalingMap = sInstance->m_Scalings_Shape;
	auto scalingFound = scalingMap.find(_shape);

	if (scalingFound != scalingMap.end()) {
		delete scalingFound->second;
		scalingMap.erase(scalingFound);
	}
}

void MovementContainer::stopScaling(VertexArray2* _vertexarray)
{
	auto& scalingMap = sInstance->m_Scalings_VA;
	auto scalingFound = scalingMap.find(_vertexarray);

	if (scalingFound != scalingMap.end()) {
		delete scalingFound->second;
		scalingMap.erase(scalingFound);
	}
}

void MovementContainer::stopScaling(sf::Sprite* _sprite)
{
	auto& scalingMap = sInstance->m_Scalings_S;
	auto scalingFound = scalingMap.find(_sprite);

	if (scalingFound != scalingMap.end()) {
		delete scalingFound->second;
		scalingMap.erase(scalingFound);
	}
}

// Rotation functions
const bool MovementContainer::addRotation(sf::Shape* _shape, float _ending_rotation, float _rotation_time, easeFunctions::Tmovement_function _used_function, bool _clockwise, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& rotationMap = sInstance->m_Rotations_Shape;
	auto rotationFound = rotationMap.find(_shape);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::Shape* object already exists!\n");
		return 0;
	}
	else {
		rotationInfo* newRotation = new rotationInfo(_shape->getRotation(), _ending_rotation, _rotation_time, easeFunctions::movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after, _clockwise);
		rotationMap.insert(std::make_pair(_shape, newRotation));
	}
	return 1;
}

const bool MovementContainer::addRotation(sf::Shape* _shape, float _starting_rotation, float _ending_rotation, float _rotation_time, easeFunctions::Tmovement_function _used_function, bool _clockwise, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& rotationMap = sInstance->m_Rotations_Shape;
	auto rotationFound = rotationMap.find(_shape);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::Shape* object already exists!\n");
		return 0;
	}
	else {
		rotationInfo* newRotation = new rotationInfo(_starting_rotation, _ending_rotation, _rotation_time, easeFunctions::movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after, _clockwise);
		rotationMap.insert(std::make_pair(_shape, newRotation));
	}
	return 1;
}

const bool MovementContainer::addRotation(VertexArray2* _vertexarray, float _ending_rotation, float _rotation_time, easeFunctions::Tmovement_function _used_function, bool _clockwise, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& rotationMap = sInstance->m_Rotations_VA;
	auto rotationFound = rotationMap.find(_vertexarray);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::VertexArray* object already exists!\n");
		return 0;
	}
	else if (_vertexarray->getVertexCount() != 0) {
		rotationInfo* newRotation = new rotationInfo(0.f, _ending_rotation, _rotation_time, easeFunctions::movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after, _clockwise);
		rotationMap.insert(std::make_pair(_vertexarray, newRotation));
	}
	return 1;
}

const bool MovementContainer::addRotation(VertexArray2* _vertexarray, float _starting_rotation, float _ending_rotation, float _rotation_time, easeFunctions::Tmovement_function _used_function, bool _clockwise, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& rotationMap = sInstance->m_Rotations_VA;
	auto rotationFound = rotationMap.find(_vertexarray);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::VertexArray* object already exists!\n");
		return 0;
	}
	else if (_vertexarray->getVertexCount() != 0) {
		rotationInfo* newRotation = new rotationInfo(_starting_rotation, _ending_rotation, _rotation_time, easeFunctions::movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after, _clockwise);
		rotationMap.insert(std::make_pair(_vertexarray, newRotation));
	}
	return 1;
}

const bool MovementContainer::addRotation(sf::Sprite* _sprite, float _ending_rotation, float _rotation_time, easeFunctions::Tmovement_function _used_function, bool _clockwise, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& rotationMap = sInstance->m_Rotations_S;
	auto rotationFound = rotationMap.find(_sprite);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::Sprite* object already exists!\n");
		return 0;
	}
	else {
		rotationInfo* newRotation = new rotationInfo(_sprite->getRotation(), _ending_rotation, _rotation_time, easeFunctions::movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after, _clockwise);
		rotationMap.insert(std::make_pair(_sprite, newRotation));
	}
}

const bool MovementContainer::addRotation(sf::Sprite* _sprite, float _starting_rotation, float _ending_rotation, float _rotation_time, easeFunctions::Tmovement_function _used_function, bool _clockwise, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& rotationMap = sInstance->m_Rotations_S;
	auto rotationFound = rotationMap.find(_sprite);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::Sprite* object already exists!\n");
		return 0;
	}
	else {
		rotationInfo* newRotation = new rotationInfo(_starting_rotation, _ending_rotation, _rotation_time, easeFunctions::movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after, _clockwise);
		rotationMap.insert(std::make_pair(_sprite, newRotation));
	}
	return 1;
}

void MovementContainer::undoRotation()
{
	for (auto rotation = this->m_Rotations_Shape.begin(); rotation != this->m_Rotations_Shape.end();) {
		rotation->first->setRotation(rotation->second->getStartingRotation());
		delete rotation->second;
		rotation = m_Rotations_Shape.erase(rotation);
	}

	for (auto rotation = this->m_Rotations_VA.begin(); rotation != this->m_Rotations_VA.end();) {
		rotation->first->setRotation(rotation->second->getStartingRotation());
		delete rotation->second;
		rotation = m_Rotations_VA.erase(rotation);
	}

	for (auto rotation = this->m_Rotations_S.begin(); rotation != this->m_Rotations_S.end();) {
		rotation->first->setRotation(rotation->second->getStartingRotation());
		delete rotation->second;
		rotation = m_Rotations_S.erase(rotation);
	}
}

void MovementContainer::undoRotation(sf::Shape* _shape)
{
	auto& rotationMap = sInstance->m_Rotations_Shape;
	auto rotationFound = rotationMap.find(_shape);

	if (rotationFound != rotationMap.end()) {
		_shape->setRotation(rotationFound->second->getStartingRotation());

		delete rotationFound->second;
		rotationMap.erase(rotationFound);
	}
}

void MovementContainer::undoRotation(VertexArray2* _vertexarray)
{
	auto& rotationMap = sInstance->m_Rotations_VA;
	auto rotationFound = rotationMap.find(_vertexarray);

	if (rotationFound != rotationMap.end()) {
		_vertexarray->setRotation(rotationFound->second->getStartingRotation());

		delete rotationFound->second;
		rotationMap.erase(rotationFound);
	}
}

void MovementContainer::undoRotation(sf::Sprite* _sprite)
{
	auto& rotationMap = sInstance->m_Rotations_S;
	auto rotationFound = rotationMap.find(_sprite);

	if (rotationFound != rotationMap.end()) {
		_sprite->setRotation(rotationFound->second->getStartingRotation());
		delete rotationFound->second;
		rotationMap.erase(rotationFound);
	}
}

void MovementContainer::resetRotation()
{
	for (auto rotation = this->m_Rotations_Shape.begin(); rotation != this->m_Rotations_Shape.end();) {
		rotation->first->setRotation(rotation->second->getStartingRotation());
		rotation->second->m_currentTime = 0.f;
		++rotation;
	}

	for (auto rotation = this->m_Rotations_VA.begin(); rotation != this->m_Rotations_VA.end();) {
		rotation->first->setRotation(rotation->second->getStartingRotation());
		rotation->second->m_currentTime = 0.f;
		++rotation;
	}

	for (auto rotation = this->m_Rotations_S.begin(); rotation != this->m_Rotations_S.end();) {
		rotation->first->setRotation(rotation->second->getStartingRotation());
		rotation->second->m_currentTime = 0.f;
		++rotation;
	}
}

void MovementContainer::resetRotation(sf::Shape* _shape)
{
	auto& rotationMap = sInstance->m_Rotations_Shape;
	auto rotationFound = rotationMap.find(_shape);

	if (rotationFound != rotationMap.end()) {
		_shape->setRotation(rotationFound->second->getStartingRotation());
		rotationFound->second->m_currentTime = 0.f;
	}
}

void MovementContainer::resetRotation(VertexArray2* _vertexarray)
{
	auto& rotationMap = sInstance->m_Rotations_VA;
	auto rotationFound = rotationMap.find(_vertexarray);

	if (rotationFound != rotationMap.end()) {
		_vertexarray->setRotation(rotationFound->second->getStartingRotation());
		rotationFound->second->m_currentTime = 0.f;
	}
}

void MovementContainer::resetRotation(sf::Sprite* _sprite)
{
	auto& rotationMap = sInstance->m_Rotations_S;
	auto rotationFound = rotationMap.find(_sprite);

	if (rotationFound != rotationMap.end()) {
		_sprite->setRotation(rotationFound->second->getStartingRotation());
		rotationFound->second->m_currentTime = 0.f;
	}
}

void MovementContainer::stopRotation()
{
	for (auto rotation = this->m_Rotations_Shape.begin(); rotation != this->m_Rotations_Shape.end();) {
		delete rotation->second;
		rotation = m_Rotations_Shape.erase(rotation);
	}

	for (auto rotation = this->m_Rotations_VA.begin(); rotation != this->m_Rotations_VA.end();) {
		delete rotation->second;
		rotation = m_Rotations_VA.erase(rotation);
	}

	for (auto rotation = this->m_Rotations_S.begin(); rotation != this->m_Rotations_S.end();) {
		delete rotation->second;
		rotation = m_Rotations_S.erase(rotation);
	}
}

void MovementContainer::stopRotation(sf::Shape* _shape)
{
	auto& rotationMap = sInstance->m_Rotations_Shape;
	auto rotationFound = rotationMap.find(_shape);

	if (rotationFound != rotationMap.end()) {
		delete rotationFound->second;
		rotationMap.erase(rotationFound);
	}
}

void MovementContainer::stopRotation(VertexArray2* _vertexarray)
{
	auto& rotationMap = sInstance->m_Rotations_VA;
	auto rotationFound = rotationMap.find(_vertexarray);

	if (rotationFound != rotationMap.end()) {
		delete rotationFound->second;
		rotationMap.erase(rotationFound);
	}
}

void MovementContainer::stopRotation(sf::Sprite* _sprite)
{
	auto& rotationMap = sInstance->m_Rotations_S;
	auto rotationFound = rotationMap.find(_sprite);

	if (rotationFound != rotationMap.end()) {
		delete rotationFound->second;
		rotationMap.erase(rotationFound);
	}
}

const bool MovementContainer::isMoving(sf::Shape* _shape)
{
	if (this->m_Movements_Shape.find(_shape) != this->m_Movements_Shape.end())
		return true;
	return false;
}

const bool MovementContainer::isMoving(VertexArray2* _vertexarray)
{
	if (this->m_Movements_VA.find(_vertexarray) != this->m_Movements_VA.end())
		return true;
	return false;
}

const bool MovementContainer::isMoving(sf::Sprite* _sprite)
{
	if (this->m_Movements_S.find(_sprite) != this->m_Movements_S.end())
		return true;
	return false;
}

const bool MovementContainer::isScaling(sf::Shape* _shape)
{
	if (this->m_Scalings_Shape.find(_shape) != this->m_Scalings_Shape.end())
		return true;
	return false;
}

const bool MovementContainer::isScaling(VertexArray2* _vertexarray)
{
	if (this->m_Scalings_VA.find(_vertexarray) != this->m_Scalings_VA.end())
		return true;
	return false;
}

const bool MovementContainer::isScaling(sf::Sprite* _sprite)
{
	if (this->m_Scalings_S.find(_sprite) != this->m_Scalings_S.end())
		return true;
	return false;
}

const bool MovementContainer::isRotating(sf::Shape* _shape)
{
	if (this->m_Rotations_Shape.find(_shape) != this->m_Rotations_Shape.end())
		return true;
	return false;
}

const bool MovementContainer::isRotating(VertexArray2* _vertexarray)
{
	if (this->m_Rotations_VA.find(_vertexarray) != this->m_Rotations_VA.end())
		return true;
	return false;
}

const bool MovementContainer::isRotating(sf::Sprite* _sprite)
{
	if (this->m_Rotations_S.find(_sprite) != this->m_Rotations_S.end())
		return true;
	return false;
}

// ---------------------------------------------------------------------------------------------------------------------------

// Singleton initialization
MovementRoutineEngine* MovementRoutineEngine::sInstance = nullptr;

MovementRoutineEngine* MovementRoutineEngine::getInstance()
{
	if (!sInstance)
		sInstance = new MovementRoutineEngine();
	return sInstance;
}

void MovementRoutineEngine::setMovementManager(MovementManager* _movementManager)
{
	this->movementManager = _movementManager;
}

void MovementRoutineEngine::update(float dt)
{
	for (auto routines = this->m_Routines.begin(); routines != this->m_Routines.end();) {
		auto& movementRoutine = std::get<0>(*routines->second);
		if (movementRoutine != nullptr) {
			if (!movementRoutine->update(*routines->first, dt))
				movementRoutine = nullptr;
		}
			

		auto& scalingRoutine = std::get<1>(*routines->second);
		if (scalingRoutine != nullptr)
			if (!scalingRoutine->update(*routines->first, dt))
				scalingRoutine = nullptr;

		auto& rotationRoutine = std::get<2>(*routines->second);
		if (rotationRoutine != nullptr) 
			if (!rotationRoutine->update(*routines->first, dt))
				rotationRoutine = nullptr;
			

		if (!movementRoutine && !scalingRoutine && !rotationRoutine) {
			delete routines->second;
			routines = m_Routines.erase(routines);
		}
		else
			++routines;
	}
}

const MovementRoutine* MovementRoutineEngine::addMovement(sf::Transformable* transformable, MovementRoutine* _movementRoutine)
{
	auto& routinesMap = sInstance->m_Routines;
	auto routinesFound = routinesMap.find(transformable);

	if (routinesFound != routinesMap.end() && std::get<0>(*routinesFound->second) != nullptr) {
		printf("MovementRoutineEngine::addMovement: MovementRoutine already exists for this shape\n");
		return nullptr;
	}
	else if (routinesFound == routinesMap.end()) {
		auto routinesTuplePtr = new std::tuple<MovementRoutine*, ScalingRoutine*, RotationRoutine*>(_movementRoutine, nullptr, nullptr);
		routinesMap.insert(std::make_pair(transformable, routinesTuplePtr));
	}
	else {
		auto& routinesTuplePtr = routinesFound->second;
		std::get<0>(*routinesTuplePtr) = _movementRoutine;
	}

	return _movementRoutine;
}

void MovementRoutineEngine::undoMovement()
{
}

void MovementRoutineEngine::undoMovement(sf::Transformable* transformable)
{
}

void MovementRoutineEngine::resetMovement()
{
	for (auto routines = this->m_Routines.begin(); routines != this->m_Routines.end();) {
		auto& routinesTuple = routines->second;
		auto& movement = std::get<0>(*routinesTuple);

		if (movement) {
			movement->reset();
			routines->first->setPosition(movement->getCurrentMovement()->getStartingPos());
		}
	}
}

void MovementRoutineEngine::stopMovement()
{
	for (auto routines = this->m_Routines.begin(); routines != this->m_Routines.end();) {
		auto& routinesTuple = routines->second;
		std::get<0>(*routinesTuple) = nullptr;

		if (std::get<1>(*routinesTuple) == nullptr && std::get<2>(*routinesTuple) == nullptr) {
			delete routines->second;
			routines = this->m_Routines.erase(routines);
		}
		else
			++routines;
	}
}

void MovementRoutineEngine::stopMovement(sf::Transformable* transformable)
{
	auto& routinesMap = sInstance->m_Routines;
	auto routinesFound = routinesMap.find(transformable);

	if (routinesFound != routinesMap.end()) {
		auto& routinesTuple = routinesFound->second;
		std::get<0>(*routinesTuple) = nullptr;

		if (std::get<1>(*routinesTuple) == nullptr && std::get<2>(*routinesTuple) == nullptr) {
			delete routinesFound->second;
			routinesMap.erase(routinesFound);
		}
	}
}

const ScalingRoutine* MovementRoutineEngine::addScaling(sf::Transformable* transformable, ScalingRoutine* _scalingRoutine)
{
	auto& routinesMap = sInstance->m_Routines;
	auto routinesFound = routinesMap.find(transformable);

	if (routinesFound != routinesMap.end() && std::get<1>(*routinesFound->second) != nullptr) {
		printf("MovementRoutineEngine::addScaling: ScalingRoutine already exists for this shape\n");
		return nullptr;
	}
	else if (routinesFound == routinesMap.end()) {
		auto routinesTuplePtr = new std::tuple<MovementRoutine*, ScalingRoutine*, RotationRoutine*>(nullptr, _scalingRoutine, nullptr);
		routinesMap.insert(std::make_pair(transformable, routinesTuplePtr));
	}
	else {
		auto& routinesTuplePtr = routinesFound->second;
		std::get<1>(*routinesTuplePtr) = _scalingRoutine;
	}

	return _scalingRoutine;
}

void MovementRoutineEngine::undoScaling()
{
}

void MovementRoutineEngine::undoScaling(sf::Transformable* transformable)
{
}

void MovementRoutineEngine::resetScaling()
{
	for (auto routines = this->m_Routines.begin(); routines != this->m_Routines.end();) {
		auto& routinesTuple = routines->second;
		auto& scaling = std::get<1>(*routinesTuple);

		if (scaling) {
			scaling->reset();
			routines->first->setScale(scaling->getCurrentScaling()->getStartingScale());
		}
	}
}

void MovementRoutineEngine::stopScaling()
{
	for (auto routines = this->m_Routines.begin(); routines != this->m_Routines.end();) {
		auto& routinesTuple = routines->second;
		std::get<1>(*routinesTuple) = nullptr;

		if (std::get<0>(*routinesTuple) == nullptr && std::get<2>(*routinesTuple) == nullptr) {
			delete routines->second;
			routines = this->m_Routines.erase(routines);
		}
		else
			++routines;
	}
}

void MovementRoutineEngine::stopScaling(sf::Transformable* transformable)
{
	auto& routinesMap = sInstance->m_Routines;
	auto routinesFound = routinesMap.find(transformable);

	if (routinesFound != routinesMap.end()) {
		auto& routinesTuple = routinesFound->second;
		std::get<1>(*routinesTuple) = nullptr;

		if (std::get<0>(*routinesTuple) == nullptr && std::get<2>(*routinesTuple) == nullptr) {
			delete routinesFound->second;
			routinesMap.erase(routinesFound);
		}
	}
}

const RotationRoutine* MovementRoutineEngine::addRotation(sf::Transformable* transformable, RotationRoutine* _rotationRoutine)
{
	auto& routinesMap = sInstance->m_Routines;
	auto routinesFound = routinesMap.find(transformable);

	if (routinesFound != routinesMap.end() && std::get<2>(*routinesFound->second) != nullptr) {
		printf("MovementRoutineEngine::addRotation: RotationRoutine already exists for this shape\n");
		return nullptr;
	}
	else if (routinesFound == routinesMap.end()) {
		auto routinesTuplePtr = new std::tuple<MovementRoutine*, ScalingRoutine*, RotationRoutine*>(nullptr, nullptr, _rotationRoutine);
		routinesMap.insert(std::make_pair(transformable, routinesTuplePtr));
	}
	else {
		auto& routinesTuplePtr = routinesFound->second;
		std::get<2>(*routinesTuplePtr) = _rotationRoutine;
	}

	return _rotationRoutine;
}

void MovementRoutineEngine::undoRotation()
{
}

void MovementRoutineEngine::undoRotation(sf::Transformable* transformable)
{
}

void MovementRoutineEngine::resetRotation()
{
	for (auto routines = this->m_Routines.begin(); routines != this->m_Routines.end();) {
		auto& routinesTuple = routines->second;
		auto& rotation = std::get<2>(*routinesTuple);

		if (rotation) {
			rotation->reset();
			routines->first->setRotation(rotation->getCurrentRotation()->getStartingRotation());
		}
	}
}

void MovementRoutineEngine::stopRotation()
{
	for (auto routines = this->m_Routines.begin(); routines != this->m_Routines.end();) {
		auto& routinesTuple = routines->second;
		std::get<2>(*routinesTuple) = nullptr;

		if (std::get<0>(*routinesTuple) == nullptr && std::get<1>(*routinesTuple) == nullptr) {
			delete routines->second;
			routines = this->m_Routines.erase(routines);
		}
	}
}

void MovementRoutineEngine::stopRotation(sf::Transformable* transformable)
{
	auto& routinesMap = sInstance->m_Routines;
	auto routinesFound = routinesMap.find(transformable);

	if (routinesFound != routinesMap.end()) {
		auto& routinesTuple = routinesFound->second;
		std::get<2>(*routinesTuple) = nullptr;

		if (std::get<0>(*routinesTuple) == nullptr && std::get<1>(*routinesTuple) == nullptr) {
			delete routinesFound->second;
			routinesMap.erase(routinesFound);
		}
	}
}
const bool MovementRoutineEngine::isMoving(sf::Transformable* transformable)
{
	return false;
}

const bool MovementRoutineEngine::isScaling(sf::Transformable* transformable)
{
	return false;
}

const bool MovementRoutineEngine::isRotating(sf::Transformable* transformable)
{
	return false;
}