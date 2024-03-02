#pragma once
#include "MovementManager.h"
#include "MovementContainer.h"
#include "TransformationRoutine.h"

// Singleton initialization
MovementContainer* MovementContainer::sInstance = nullptr;

// Private update functions
void MovementContainer::updateShape(float dt)
{
	for (auto movement = this->m_Movements_Shape.begin(); movement != this->m_Movements_Shape.end();) {
		movement->second->m_currentTime += dt;

		if (movement->second->isDone()) {
			if (movement->second->m_currentTime - movement->second->m_delayBefore - dt < movement->second->m_motionDuration)
				movement->first->setPosition(movement->second->getEndingPos());

			if (movement->second->m_repeat) {
				if (movement->second->m_currentTime - movement->second->m_motionDuration - movement->second->m_delayBefore >= movement->second->m_delayAfter) {
					movement->first->setPosition(movement->second->getStartingPos());

					movement->second->m_currentTime -= movement->second->m_totalDuration;
				}
			}
			else if (movement->second->isFinished()) {
				delete movement->second;
				movement = m_Movements_Shape.erase(movement);
				continue;
			}
		}
		else {
			if (movement->second->m_currentTime > movement->second->m_delayBefore) {
				sf::Vector2f new_position(static_cast<float>(movement->second->m_usedFunctionPtr(static_cast<double>((movement->second->m_currentTime - movement->second->m_delayBefore) / movement->second->m_motionDuration))) * (movement->second->getEndingPos().x - movement->second->getStartingPos().x) + movement->second->getStartingPos().x,
										  static_cast<float>(movement->second->m_usedFunctionPtr(static_cast<double>((movement->second->m_currentTime - movement->second->m_delayBefore) / movement->second->m_motionDuration))) * (movement->second->getEndingPos().y - movement->second->getStartingPos().y) + movement->second->getStartingPos().y);

				movement->first->setPosition(new_position);
			}
			else if (movement->second->m_currentTime - dt == 0.f)
				movement->first->setPosition(movement->second->getStartingPos());
		}

		++movement;
	}

	for (auto scaling = this->m_Scalings_Shape.begin(); scaling != this->m_Scalings_Shape.end();) {
		scaling->second->m_currentTime += dt;

		if (scaling->second->isDone()) {
			if (scaling->second->m_currentTime - scaling->second->m_delayBefore - dt < scaling->second->m_motionDuration)
				scaling->first->setScale(scaling->second->getEndingScale());

			if (scaling->second->m_repeat) {
				if (scaling->second->m_currentTime - scaling->second->m_motionDuration - scaling->second->m_delayBefore >= scaling->second->m_delayAfter) {
					scaling->first->setScale(scaling->second->getStartingScale());
					
					scaling->second->m_currentTime -= scaling->second->m_totalDuration;
				}
			}
			else if (scaling->second->isFinished()) {
				delete scaling->second;
				scaling = m_Scalings_Shape.erase(scaling);
				continue;
			}
		}
		else {
			if (scaling->second->m_currentTime > scaling->second->m_delayBefore) {
				sf::Vector2f new_scale(static_cast<float>(scaling->second->m_usedFunctionPtr(static_cast<double>((scaling->second->m_currentTime - scaling->second->m_delayBefore) / scaling->second->m_motionDuration))) * (scaling->second->getEndingScale().x - scaling->second->getStartingScale().x) + scaling->second->getStartingScale().x,
									   static_cast<float>(scaling->second->m_usedFunctionPtr(static_cast<double>((scaling->second->m_currentTime - scaling->second->m_delayBefore) / scaling->second->m_motionDuration))) * (scaling->second->getEndingScale().y - scaling->second->getStartingScale().y) + scaling->second->getStartingScale().y);

				scaling->first->setScale(new_scale);
			}
			else if (scaling->second->m_currentTime - dt == 0.f)
				scaling->first->setScale(scaling->second->getStartingScale());
		}

		++scaling;
	}

	for (auto rotation = this->m_Rotations_Shape.begin(); rotation != this->m_Rotations_Shape.end();) {
		rotation->second->m_currentTime += dt;

		if (rotation->second->isDone()) {
			if (rotation->second->m_currentTime - rotation->second->m_delayBefore - dt < rotation->second->m_motionDuration)
				rotation->first->setRotation(rotation->second->getEndingRotation());

			if (rotation->second->m_repeat) {
				if (rotation->second->m_currentTime - rotation->second->m_motionDuration - rotation->second->m_delayBefore >= rotation->second->m_delayAfter) {
					rotation->first->setRotation(rotation->second->getStartingRotation());

					rotation->second->m_currentTime -= rotation->second->m_totalDuration;
				}
			}
			else if (rotation->second->isFinished()) {
				delete rotation->second;
				rotation = m_Rotations_Shape.erase(rotation);
				continue;
			}
		}
		else{
			if (rotation->second->m_currentTime > rotation->second->m_delayBefore)
				rotation->first->setRotation(rotation->second->updateRotation());
			else if (rotation->second->m_currentTime - dt == 0.f)
				rotation->first->setRotation(rotation->second->getStartingRotation());
		}

		++rotation;
	}
}

void MovementContainer::updateVertexArray(float dt)
{
	for (auto movement = this->m_Movements_VA.begin(); movement != this->m_Movements_VA.end();) {
		movement->second->m_currentTime += dt;

		if (movement->first->getVertexCount() != 0) {
			if (movement->second->isDone()) {
				if (movement->second->m_currentTime - movement->second->m_delayBefore - dt < movement->second->m_motionDuration) 
					movement->first->setPosition(movement->second->getEndingPos());

				if (movement->second->m_repeat) {
					if (movement->second->m_currentTime - movement->second->m_motionDuration - movement->second->m_delayBefore >= movement->second->m_delayAfter) {
						movement->first->setPosition(movement->second->getStartingPos());
						movement->second->m_currentTime -= movement->second->m_totalDuration;
					}
				}
				else if (movement->second->isFinished()) {
					delete movement->second;
					movement = m_Movements_VA.erase(movement);
					continue;
				}
			}
			else {
				if (movement->second->m_currentTime > movement->second->m_delayBefore) {
					sf::Vector2f new_position(static_cast<float>(movement->second->m_usedFunctionPtr(static_cast<double>((movement->second->m_currentTime - movement->second->m_delayBefore) / movement->second->m_motionDuration))) * (movement->second->getEndingPos().x - movement->second->getStartingPos().x) + movement->second->getStartingPos().x,
											  static_cast<float>(movement->second->m_usedFunctionPtr(static_cast<double>((movement->second->m_currentTime - movement->second->m_delayBefore) / movement->second->m_motionDuration))) * (movement->second->getEndingPos().y - movement->second->getStartingPos().y) + movement->second->getStartingPos().y);

					movement->first->setPosition(new_position);
				}
				else if (movement->second->m_currentTime - dt == 0.f) 
					movement->first->setPosition(movement->second->getStartingPos());
			}
		}

		++movement;
	}

	for (auto scaling = this->m_Scalings_VA.begin(); scaling != this->m_Scalings_VA.end();) {
		scaling->second->m_currentTime += dt;

		if (scaling->first->getVertexCount() != 0) {
			if (scaling->second->isDone()) {
				if (scaling->second->m_currentTime - scaling->second->m_delayBefore - dt < scaling->second->m_motionDuration) 
					scaling->first->setScale(scaling->second->getEndingScale());

				if (scaling->second->m_repeat) {
					if (scaling->second->m_currentTime - scaling->second->m_motionDuration - scaling->second->m_delayBefore >= scaling->second->m_delayAfter) {
						scaling->first->setScale(scaling->second->getStartingScale());
						scaling->second->m_currentTime -= scaling->second->m_totalDuration;
					}
				}
				else if (scaling->second->isFinished()) {
					delete scaling->second;
					scaling = m_Scalings_VA.erase(scaling);
					continue;
				}
			}
			else {
				if (scaling->second->m_currentTime > scaling->second->m_delayBefore) {
					sf::Vector2f new_scale(static_cast<float>(scaling->second->m_usedFunctionPtr(static_cast<double>((scaling->second->m_currentTime - scaling->second->m_delayBefore) / scaling->second->m_motionDuration))) * (scaling->second->getEndingScale().x - scaling->second->getStartingScale().x) + scaling->second->getStartingScale().x,
										   static_cast<float>(scaling->second->m_usedFunctionPtr(static_cast<double>((scaling->second->m_currentTime - scaling->second->m_delayBefore) / scaling->second->m_motionDuration))) * (scaling->second->getEndingScale().y - scaling->second->getStartingScale().y) + scaling->second->getStartingScale().y);
					
					scaling->first->setScale(new_scale);
				}
				else if (scaling->second->m_currentTime - dt == 0.f) 
					scaling->first->setScale(scaling->second->getStartingScale());
			}
		}

		++scaling;
	}

	for (auto rotation = this->m_Rotations_VA.begin(); rotation != this->m_Rotations_VA.end();) {
		rotation->second->m_currentTime += dt;

		if (rotation->first->getVertexCount() != 0) {
			if (rotation->second->isDone()) {
				if (rotation->second->m_currentTime - rotation->second->m_delayBefore - dt < rotation->second->m_motionDuration)
					rotation->first->setRotation(rotation->second->getEndingRotation());

				if (rotation->second->m_repeat) {
					if (rotation->second->m_currentTime - rotation->second->m_motionDuration - rotation->second->m_delayBefore >= rotation->second->m_delayAfter){
						rotation->first->setRotation(rotation->second->getStartingRotation());
						rotation->second->m_currentTime -= rotation->second->m_totalDuration;
					}
				}
				else if (rotation->second->isFinished()) {
					delete rotation->second;
					rotation = m_Rotations_VA.erase(rotation);
					continue;
				}
			}
			else{
				if (rotation->second->m_currentTime > rotation->second->m_delayBefore) 
					rotation->first->setRotation(rotation->second->updateRotation());
				else if (rotation->second->m_currentTime - dt == 0.f) 
					rotation->first->setRotation(rotation->second->getStartingRotation());
			}
		}

		++rotation;
	}
}

void MovementContainer::updateSprite(float dt)
{
	for (auto movement = this->m_Movements_S.begin(); movement != this->m_Movements_S.end();) {
		movement->second->m_currentTime += dt;

		if (movement->second->isDone()) {
			if (movement->second->m_currentTime - movement->second->m_delayBefore - dt < movement->second->m_motionDuration)
				movement->first->setPosition(movement->second->getEndingPos());

			if (movement->second->m_repeat) {
				if (movement->second->m_currentTime - movement->second->m_motionDuration - movement->second->m_delayBefore >= movement->second->m_delayAfter) {
					movement->first->setPosition(movement->second->getStartingPos());

					movement->second->m_currentTime -= movement->second->m_totalDuration;
				}
			}
			else if (movement->second->isFinished()) {
				delete movement->second;
				movement = m_Movements_S.erase(movement);
				continue;
			}
		}
		else {
			if (movement->second->m_currentTime > movement->second->m_delayBefore) {
				sf::Vector2f new_position(static_cast<float>(movement->second->m_usedFunctionPtr(static_cast<double>((movement->second->m_currentTime - movement->second->m_delayBefore) / movement->second->m_motionDuration))) * (movement->second->getEndingPos().x - movement->second->getStartingPos().x) + movement->second->getStartingPos().x,
										  static_cast<float>(movement->second->m_usedFunctionPtr(static_cast<double>((movement->second->m_currentTime - movement->second->m_delayBefore) / movement->second->m_motionDuration))) * (movement->second->getEndingPos().y - movement->second->getStartingPos().y) + movement->second->getStartingPos().y);

				movement->first->setPosition(new_position);
			}
			else if (movement->second->m_currentTime - dt == 0.f)
				movement->first->setPosition(movement->second->getStartingPos());
		}
		++movement;
	}

	for (auto scaling = this->m_Scalings_S.begin(); scaling != this->m_Scalings_S.end();) {
		scaling->second->m_currentTime += dt;

		if (scaling->second->isDone()) {
			if (scaling->second->m_currentTime - scaling->second->m_delayBefore - dt < scaling->second->m_motionDuration)
				scaling->first->setScale(scaling->second->getEndingScale());

			if (scaling->second->m_repeat) {
				if (scaling->second->m_currentTime - scaling->second->m_motionDuration - scaling->second->m_delayBefore >= scaling->second->m_delayAfter) {
					scaling->first->setScale(scaling->second->getStartingScale());

					scaling->second->m_currentTime -= scaling->second->m_totalDuration;
				}
			}
			else if (scaling->second->isFinished()) {
				delete scaling->second;
				scaling = m_Scalings_S.erase(scaling);
				continue;
			}
		}
		else {
			if (scaling->second->m_currentTime > scaling->second->m_delayBefore) {
				sf::Vector2f new_scale(static_cast<float>(scaling->second->m_usedFunctionPtr(static_cast<double>((scaling->second->m_currentTime - scaling->second->m_delayBefore) / scaling->second->m_motionDuration))) * (scaling->second->getEndingScale().x - scaling->second->getStartingScale().x) + scaling->second->getStartingScale().x,
									   static_cast<float>(scaling->second->m_usedFunctionPtr(static_cast<double>((scaling->second->m_currentTime - scaling->second->m_delayBefore) / scaling->second->m_motionDuration))) * (scaling->second->getEndingScale().y - scaling->second->getStartingScale().y) + scaling->second->getStartingScale().y);

				scaling->first->setScale(new_scale);
			}
			else if (scaling->second->m_currentTime - dt == 0.f)
				scaling->first->setScale(scaling->second->getStartingScale());
		}

		++scaling;
	}

	for (auto rotation = this->m_Rotations_S.begin(); rotation != this->m_Rotations_S.end();) {
		rotation->second->m_currentTime += dt;

		if (rotation->second->isDone()) {
			if (rotation->second->m_currentTime - rotation->second->m_delayBefore - dt < rotation->second->m_motionDuration)
				rotation->first->setRotation(rotation->second->getEndingRotation());

			if (rotation->second->m_repeat) {
				if (rotation->second->m_currentTime - rotation->second->m_motionDuration - rotation->second->m_delayBefore >= rotation->second->m_delayAfter) {
					rotation->first->setRotation(rotation->second->getStartingRotation());

					rotation->second->m_currentTime -= rotation->second->m_totalDuration;
				}
			}
			else if (rotation->second->isFinished()) {
				delete rotation->second;
				rotation = m_Rotations_S.erase(rotation);
				continue;
			}
		}
		else {
			if (rotation->second->m_currentTime > rotation->second->m_delayBefore)
				rotation->first->setRotation(rotation->second->updateRotation());
			else if (rotation->second->m_currentTime - dt == 0.f)
				rotation->first->setRotation(rotation->second->getStartingRotation());
		}

		++rotation;
	}
}

// Default constructor
MovementContainer::MovementContainer()
{
	assert(this->sInstance == nullptr);
	sInstance = this;
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
const movementInfo* MovementContainer::addMovement(sf::Shape* _shape, sf::Vector2f _ending_pos, float _movement_time, movement_type _used_function, bool _repeat, float _delay_before, float _delay_after)
{
	auto& movementMap = sInstance->m_Movements_Shape;
	auto movementFound = movementMap.find(_shape);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::Shape* object already exists!\n");
		return nullptr;
	}
	
	movementInfo* newMovement = new movementInfo(_shape->getPosition(), _ending_pos, _movement_time, sInstance->movement_functions[_used_function], _repeat, _delay_before, _delay_after);
	movementMap.insert(std::make_pair(_shape, newMovement));

	return newMovement;
}

const movementInfo* MovementContainer::addMovement(sf::Shape* _shape, sf::Vector2f _starting_pos, sf::Vector2f _ending_pos, float _movement_time, movement_type _used_function, bool _repeat, float _delay_before, float _delay_after)
{
	auto& movementMap = sInstance->m_Movements_Shape;
	auto movementFound = movementMap.find(_shape);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::Shape* object already exists!\n");
		return nullptr;
	}
	
	movementInfo* newMovement = new movementInfo(_starting_pos, _ending_pos, _movement_time, sInstance->movement_functions[_used_function], _repeat, _delay_before, _delay_after);
	movementMap.insert(std::make_pair(_shape, newMovement));

	return newMovement;
}

const movementInfo* MovementContainer::addMovement(sf::Shape* _shape, float _offset_x, float _offset_y, float _movement_time, movement_type _used_function, bool _repeat, float _delay_before, float _delay_after)
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

const movementInfo* MovementContainer::addMovement(VertexArray2* _vertexarray, sf::Vector2f _ending_pos, float _movement_time, movement_type _used_function, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& movementMap = sInstance->m_Movements_VA;
	auto movementFound = movementMap.find(_vertexarray);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::VertexArray* object already exists!\n");
		return nullptr;
	}

	movementInfo* newMovement = new movementInfo(_vertexarray->getPosition(), _ending_pos, _movement_time, sInstance->movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after);
	movementMap.insert(std::make_pair(_vertexarray, newMovement));

	return newMovement;
}

const movementInfo* MovementContainer::addMovement(VertexArray2* _vertexarray, sf::Vector2f _starting_pos, sf::Vector2f _ending_pos, float _movement_time, movement_type _used_function, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& movementMap = sInstance->m_Movements_VA;
	auto movementFound = movementMap.find(_vertexarray);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::VertexArray* object already exists!\n");
		return nullptr;
	}

	movementInfo* newMovement = new movementInfo(_starting_pos, _ending_pos, _movement_time, sInstance->movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after);
	movementMap.insert(std::make_pair(_vertexarray, newMovement));

	return newMovement;
}

const movementInfo* MovementContainer::addMovement(VertexArray2* _vertexarray, float _offset_x, float _offset_y, float _movement_time, movement_type _used_function, bool _repeat, float _m_delayBefore, float _delay_after)
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

const movementInfo* MovementContainer::addMovement(sf::Sprite* _sprite, sf::Vector2f _ending_pos, float _movement_time, movement_type _used_function, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& movementMap = sInstance->m_Movements_S;
	auto movementFound = movementMap.find(_sprite);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::Sprite* object already exists!\n");
		return nullptr;
	}
	
	movementInfo* newMovement = new movementInfo(_sprite->getPosition(), _ending_pos, _movement_time, sInstance->movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after);
	movementMap.insert(std::make_pair(_sprite, newMovement));

	return newMovement;
}

const movementInfo* MovementContainer::addMovement(sf::Sprite* _sprite, sf::Vector2f _starting_pos, sf::Vector2f _ending_pos, float _movement_time, movement_type _used_function, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& movementMap = sInstance->m_Movements_S;
	auto movementFound = movementMap.find(_sprite);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::Sprite* object already exists!\n");
		return nullptr;
	}

	movementInfo* newMovement = new movementInfo(_starting_pos, _ending_pos, _movement_time, sInstance->movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after);
	movementMap.insert(std::make_pair(_sprite, newMovement));

	return newMovement;
}

const movementInfo* MovementContainer::addMovement(sf::Sprite* _sprite, float _offset_x, float _offset_y, float _movement_time, movement_type _used_function, bool _repeat, float _m_delayBefore, float _delay_after)
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
const bool MovementContainer::addScaling(sf::Shape* _shape, sf::Vector2f _ending_scale, float _scaling_time, movement_type _used_function, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& scalingMap = sInstance->m_Scalings_Shape;
	auto scalingFound = scalingMap.find(_shape);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::Shape* object already exists!\n");
		return 0;
	}
	else {
		scalingInfo* newScaling = new scalingInfo(_shape->getScale(), _ending_scale, _scaling_time, sInstance->movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after);
		scalingMap.insert(std::make_pair(_shape, newScaling));
	}
	return 1;
}

const bool MovementContainer::addScaling(sf::Shape* _shape, sf::Vector2f _starting_scale, sf::Vector2f _ending_scale, float _scaling_time, movement_type _used_function, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& scalingMap = sInstance->m_Scalings_Shape;
	auto scalingFound = scalingMap.find(_shape);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::Shape* object already exists!\n");
		return 0;
	}
	else {
		scalingInfo* newScaling = new scalingInfo(_starting_scale, _ending_scale, _scaling_time, sInstance->movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after);
		scalingMap.insert(std::make_pair(_shape, newScaling));
	}
	return 1;
}

const bool MovementContainer::addScaling(VertexArray2* _vertexarray, sf::Vector2f _ending_scale, float _scaling_time, movement_type _used_function, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& scalingMap = sInstance->m_Scalings_VA;
	auto scalingFound = scalingMap.find(_vertexarray);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::VertexArray* object already exists!\n");
		return 0;
	}
	else if (_vertexarray->getVertexCount() != 0){
		scalingInfo* newScaling = new scalingInfo({ 1.f, 1.f }, _ending_scale, _scaling_time, sInstance->movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after);
		scalingMap.insert(std::make_pair(_vertexarray, newScaling));
	}
	return 1;
}

const bool MovementContainer::addScaling(VertexArray2* _vertexarray, sf::Vector2f _starting_scale, sf::Vector2f _ending_scale, float _scaling_time, movement_type _used_function, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& scalingMap = sInstance->m_Scalings_VA;
	auto scalingFound = scalingMap.find(_vertexarray);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::VertexArray* object already exists!\n");
		return 0;
	}
	else if (_vertexarray->getVertexCount() != 0) {
		scalingInfo* newScaling = new scalingInfo(_starting_scale, _ending_scale, _scaling_time, sInstance->movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after);
		scalingMap.insert(std::make_pair(_vertexarray, newScaling));
	}
	return 1;
}

const bool MovementContainer::addScaling(sf::Sprite* _sprite, sf::Vector2f _ending_scale, float _scaling_time, movement_type _used_function, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& scalingMap = sInstance->m_Scalings_S;
	auto scalingFound = scalingMap.find(_sprite);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::Sprite* object already exists!\n");
		return 0;
	}
	else {
		scalingInfo* newScaling = new scalingInfo(_sprite->getScale(), _ending_scale, _scaling_time, sInstance->movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after);
		scalingMap.insert(std::make_pair(_sprite, newScaling));
	}
	return 1;
}

const bool MovementContainer::addScaling(sf::Sprite* _sprite, sf::Vector2f _starting_scale, sf::Vector2f _ending_scale, float _scaling_time, movement_type _used_function, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& scalingMap = sInstance->m_Scalings_S;
	auto scalingFound = scalingMap.find(_sprite);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::Sprite* object already exists!\n");
		return 0;
	}
	else {
		scalingInfo* newScaling = new scalingInfo(_starting_scale, _ending_scale, _scaling_time, sInstance->movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after);
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
const bool MovementContainer::addRotation(sf::Shape* _shape, float _ending_rotation, float _rotation_time, movement_type _used_function, bool _clockwise, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& rotationMap = sInstance->m_Rotations_Shape;
	auto rotationFound = rotationMap.find(_shape);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::Shape* object already exists!\n");
		return 0;
	}
	else {
		rotationInfo* newRotation = new rotationInfo(_shape->getRotation(), _ending_rotation, _rotation_time, sInstance->movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after, _clockwise);
		rotationMap.insert(std::make_pair(_shape, newRotation));
	}
	return 1;
}

const bool MovementContainer::addRotation(sf::Shape* _shape, float _starting_rotation, float _ending_rotation, float _rotation_time, movement_type _used_function, bool _clockwise, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& rotationMap = sInstance->m_Rotations_Shape;
	auto rotationFound = rotationMap.find(_shape);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::Shape* object already exists!\n");
		return 0;
	}
	else {
		rotationInfo* newRotation = new rotationInfo(_starting_rotation, _ending_rotation, _rotation_time, sInstance->movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after, _clockwise);
		rotationMap.insert(std::make_pair(_shape, newRotation));
	}
	return 1;
}

const bool MovementContainer::addRotation(VertexArray2* _vertexarray, float _ending_rotation, float _rotation_time, movement_type _used_function, bool _clockwise, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& rotationMap = sInstance->m_Rotations_VA;
	auto rotationFound = rotationMap.find(_vertexarray);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::VertexArray* object already exists!\n");
		return 0;
	}
	else if (_vertexarray->getVertexCount() != 0) {
		rotationInfo* newRotation = new rotationInfo(0.f, _ending_rotation, _rotation_time, sInstance->movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after, _clockwise);
		rotationMap.insert(std::make_pair(_vertexarray, newRotation));
	}
	return 1;
}

const bool MovementContainer::addRotation(VertexArray2* _vertexarray, float _starting_rotation, float _ending_rotation, float _rotation_time, movement_type _used_function, bool _clockwise, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& rotationMap = sInstance->m_Rotations_VA;
	auto rotationFound = rotationMap.find(_vertexarray);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::VertexArray* object already exists!\n");
		return 0;
	}
	else if (_vertexarray->getVertexCount() != 0) {
		rotationInfo* newRotation = new rotationInfo(_starting_rotation, _ending_rotation, _rotation_time, sInstance->movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after, _clockwise);
		rotationMap.insert(std::make_pair(_vertexarray, newRotation));
	}
	return 1;
}

const bool MovementContainer::addRotation(sf::Sprite* _sprite, float _ending_rotation, float _rotation_time, movement_type _used_function, bool _clockwise, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& rotationMap = sInstance->m_Rotations_S;
	auto rotationFound = rotationMap.find(_sprite);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::Sprite* object already exists!\n");
		return 0;
	}
	else {
		rotationInfo* newRotation = new rotationInfo(_sprite->getRotation(), _ending_rotation, _rotation_time, sInstance->movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after, _clockwise);
		rotationMap.insert(std::make_pair(_sprite, newRotation));
	}
}

const bool MovementContainer::addRotation(sf::Sprite* _sprite, float _starting_rotation, float _ending_rotation, float _rotation_time, movement_type _used_function, bool _clockwise, bool _repeat, float _m_delayBefore, float _delay_after)
{
	auto& rotationMap = sInstance->m_Rotations_S;
	auto rotationFound = rotationMap.find(_sprite);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::Sprite* object already exists!\n");
		return 0;
	}
	else {
		rotationInfo* newRotation = new rotationInfo(_starting_rotation, _ending_rotation, _rotation_time, sInstance->movement_functions[_used_function], _repeat, _m_delayBefore, _delay_after, _clockwise);
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

void MovementRoutineEngine::updateShape(float dt)
{
	for (auto movementRoutine = this->m_Movement_Routines_Shape.begin(); movementRoutine != this->m_Movement_Routines_Shape.end();) {
		if (!movementRoutine->second->m_isPaused) {
			sf::Shape& shape = *movementRoutine->first;
			movementInfo& movement = *movementRoutine->second->getCurrentMovement();

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
					if (!movementRoutine->second->goToNextMovement(shape)) {
						movementRoutine->second = nullptr;
						movementRoutine = m_Movement_Routines_Shape.erase(movementRoutine);
						continue;
					}
				}
			}
			else {
				if (movement.m_currentTime > movement.m_delayBefore) {
					sf::Vector2f new_position(static_cast<float>(movement.m_usedFunctionPtr(static_cast<double>((movement.m_currentTime - movement.m_delayBefore) / movement.m_motionDuration))) * (movement.getEndingPos().x - movement.getStartingPos().x) + movement.getStartingPos().x,
											  static_cast<float>(movement.m_usedFunctionPtr(static_cast<double>((movement.m_currentTime - movement.m_delayBefore) / movement.m_motionDuration))) * (movement.getEndingPos().y - movement.getStartingPos().y) + movement.getStartingPos().y);

					shape.setPosition(new_position);
				}
				else if (movement.m_currentTime - dt == 0.f)
					shape.setPosition(movement.getStartingPos());
			}
		}

		++movementRoutine;
	}

	for (auto scalingRoutine = this->m_Scaling_Routines_Shape.begin(); scalingRoutine != this->m_Scaling_Routines_Shape.end();) {
		if (!scalingRoutine->second->m_isPaused) {
			sf::Shape& shape = *scalingRoutine->first;
			scalingInfo& scaling = *scalingRoutine->second->getCurrentScaling();

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
					if (!scalingRoutine->second->goToNextScaling(shape)) {
						scalingRoutine->second = nullptr;
						scalingRoutine = m_Scaling_Routines_Shape.erase(scalingRoutine);
						continue;
					}
				}
			}
			else {
				if (scaling.m_currentTime > scaling.m_delayBefore) {
					sf::Vector2f new_scale(static_cast<float>(scaling.m_usedFunctionPtr(static_cast<double>((scaling.m_currentTime - scaling.m_delayBefore) / scaling.m_motionDuration))) * (scaling.getEndingScale().x - scaling.getStartingScale().x) + scaling.getStartingScale().x,
										   static_cast<float>(scaling.m_usedFunctionPtr(static_cast<double>((scaling.m_currentTime - scaling.m_delayBefore) / scaling.m_motionDuration))) * (scaling.getEndingScale().y - scaling.getStartingScale().y) + scaling.getStartingScale().y);

					shape.setScale(new_scale);
				}
				else if (scaling.m_currentTime - dt == 0.f)
					shape.setScale(scaling.getStartingScale());
			}
		}

		++scalingRoutine;
	}

	for (auto rotationRoutine = this->m_Rotation_Routines_Shape.begin(); rotationRoutine != this->m_Rotation_Routines_Shape.end();) {
		if (!rotationRoutine->second->m_isPaused) {
			sf::Shape& shape = *rotationRoutine->first;
			rotationInfo& rotation = *rotationRoutine->second->getCurrentRotation();

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
					if (!rotationRoutine->second->goToNextRotation(shape)) {
						rotationRoutine->second = nullptr;
						rotationRoutine = m_Rotation_Routines_Shape.erase(rotationRoutine);
						continue;
					}
				}
			}
			else {
				if (rotation.m_currentTime > rotation.m_delayBefore)
					shape.setRotation(rotation.updateRotation());
				else if (rotation.m_currentTime - dt == 0.f)
					shape.setRotation(rotation.getStartingRotation());

			}
		}

		++rotationRoutine;
	}
}

void MovementRoutineEngine::updateVertexArray(float dt)
{
	for (auto movementRoutine = this->m_Movement_Routines_VA.begin(); movementRoutine != this->m_Movement_Routines_VA.end();) {
		if (!movementRoutine->second->m_isPaused) {
			VertexArray2& vertexarray = *movementRoutine->first;
			movementInfo& movement = *movementRoutine->second->getCurrentMovement();

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
						if (!movementRoutine->second->goToNextMovement(vertexarray)) {
							movementRoutine->second = nullptr;
							movementRoutine = m_Movement_Routines_VA.erase(movementRoutine);
							continue;
						}
					}					
				}
				else {
					if (movement.m_currentTime > movement.m_delayBefore) {
						sf::Vector2f new_position(static_cast<float>(movement.m_usedFunctionPtr(static_cast<double>((movement.m_currentTime - movement.m_delayBefore) / movement.m_motionDuration))) * (movement.getEndingPos().x - movement.getStartingPos().x) + movement.getStartingPos().x,
												  static_cast<float>(movement.m_usedFunctionPtr(static_cast<double>((movement.m_currentTime - movement.m_delayBefore) / movement.m_motionDuration))) * (movement.getEndingPos().y - movement.getStartingPos().y) + movement.getStartingPos().y);

						vertexarray.setPosition(new_position);
					}
					else if (movement.m_currentTime - dt == 0.f)
						vertexarray.setPosition(movement.getStartingPos());
				}
			}
		}

		++movementRoutine;
	}

	for (auto scalingRoutine = this->m_Scaling_Routines_VA.begin(); scalingRoutine != this->m_Scaling_Routines_VA.end();) {
		if (!scalingRoutine->second->m_isPaused) {
			VertexArray2& vertexarray = *scalingRoutine->first;
			scalingInfo&  scaling = *scalingRoutine->second->getCurrentScaling();

			scaling.m_currentTime += dt;

			if (vertexarray.getVertexCount() != 0) {
				if (scaling.isDone()) {
					if (scaling.m_currentTime - scaling.m_delayBefore - dt < scaling.m_motionDuration) 
						vertexarray.setScale(scaling.getEndingScale());
					

					if (scaling.m_repeat) {
						if (scaling.m_currentTime - scaling.m_motionDuration - scaling.m_delayBefore >= scaling.m_delayAfter){
							vertexarray.setScale(scaling.getStartingScale());

							scaling.m_currentTime -= scaling.m_totalDuration;
						}
					}
					else if (scaling.isFinished()) {
						if (!scalingRoutine->second->goToNextScaling(vertexarray)) {
							scalingRoutine->second = nullptr;
							scalingRoutine = m_Scaling_Routines_VA.erase(scalingRoutine);
							continue;
						}
					}
				}
				else {
					if (scaling.m_currentTime > scaling.m_delayBefore) {
						sf::Vector2f new_scale(static_cast<float>(scaling.m_usedFunctionPtr(static_cast<double>((scaling.m_currentTime - scaling.m_delayBefore) / scaling.m_motionDuration))) * (scaling.getEndingScale().x - scaling.getStartingScale().x) + scaling.getStartingScale().x,
											   static_cast<float>(scaling.m_usedFunctionPtr(static_cast<double>((scaling.m_currentTime - scaling.m_delayBefore) / scaling.m_motionDuration))) * (scaling.getEndingScale().y - scaling.getStartingScale().y) + scaling.getStartingScale().y);

						vertexarray.setScale(new_scale);
					}
					else if (scaling.m_currentTime - dt == 0.f) 
						vertexarray.setScale(scaling.getStartingScale());
				}
			}
		}

		++scalingRoutine;
	}

	for (auto rotationRoutine = this->m_Rotation_Routines_VA.begin(); rotationRoutine != this->m_Rotation_Routines_VA.end();) {
		if (!rotationRoutine->second->m_isPaused) {
			VertexArray2& vertexarray = *rotationRoutine->first;
			rotationInfo& rotation = *rotationRoutine->second->getCurrentRotation();

			rotation.m_currentTime += dt;

			if (vertexarray.getVertexCount() != 0) {
				if (rotation.isDone()) {
					if (rotation.m_currentTime - rotation.m_delayBefore - dt < rotation.m_motionDuration) 
						vertexarray.setRotation(rotation.getEndingRotation());

					if (rotation.m_repeat) {
						if (rotation.m_currentTime - rotation.m_motionDuration - rotation.m_delayBefore >= rotation.m_delayAfter) {
							vertexarray.setRotation(rotation.getStartingRotation());

							rotation.m_currentTime -= rotation.m_totalDuration;
						}
					}
					else if (rotation.isFinished()) {
						if (!rotationRoutine->second->goToNextRotation(vertexarray)) {
							rotationRoutine->second = nullptr;
							rotationRoutine = m_Rotation_Routines_VA.erase(rotationRoutine);
							continue;
						}
					}
				}
				else {
					if (rotation.m_currentTime > rotation.m_delayBefore) 
						vertexarray.setRotation(rotation.updateRotation());
					else if (rotation.m_currentTime - dt == 0.f) 
						vertexarray.setRotation(rotation.getStartingRotation());
				}
			}
		}

		++rotationRoutine;
	}
}

void MovementRoutineEngine::updateSprite(float dt)
{
	for (auto movementRoutine = this->m_Movement_Routines_S.begin(); movementRoutine != this->m_Movement_Routines_S.end();) {
		if (!movementRoutine->second->m_isPaused) {
			sf::Sprite& sprite = *movementRoutine->first;
			movementInfo& movement = *movementRoutine->second->getCurrentMovement();

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
					if (!movementRoutine->second->goToNextMovement(sprite)) {
						movementRoutine->second = nullptr;
						movementRoutine = m_Movement_Routines_S.erase(movementRoutine);
						continue;
					}
				}
			}
			else {
				if (movement.m_currentTime > movement.m_delayBefore) {
					sf::Vector2f new_position(static_cast<float>(movement.m_usedFunctionPtr(static_cast<double>((movement.m_currentTime - movement.m_delayBefore) / movement.m_motionDuration))) * (movement.getEndingPos().x - movement.getStartingPos().x) + movement.getStartingPos().x,
											  static_cast<float>(movement.m_usedFunctionPtr(static_cast<double>((movement.m_currentTime - movement.m_delayBefore) / movement.m_motionDuration))) * (movement.getEndingPos().y - movement.getStartingPos().y) + movement.getStartingPos().y);

					sprite.setPosition(new_position);
				}
				else if (movement.m_currentTime - dt == 0.f)
					sprite.setPosition(movement.getStartingPos());
			}
		}

		++movementRoutine;
	}

	for (auto scalingRoutine = this->m_Scaling_Routines_S.begin(); scalingRoutine != this->m_Scaling_Routines_S.end();) {
		if (!scalingRoutine->second->m_isPaused) {
			sf::Sprite& sprite = *scalingRoutine->first;
			scalingInfo& scaling = *scalingRoutine->second->getCurrentScaling();

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
					if (!scalingRoutine->second->goToNextScaling(sprite)) {
						scalingRoutine->second = nullptr;
						scalingRoutine = m_Scaling_Routines_S.erase(scalingRoutine);
						continue;
					}
				}
			}
			else {
				if (scaling.m_currentTime > scaling.m_delayBefore) {
					sf::Vector2f new_scale(static_cast<float>(scaling.m_usedFunctionPtr(static_cast<double>((scaling.m_currentTime - scaling.m_delayBefore) / scaling.m_motionDuration))) * (scaling.getEndingScale().x - scaling.getStartingScale().x) + scaling.getStartingScale().x,
										   static_cast<float>(scaling.m_usedFunctionPtr(static_cast<double>((scaling.m_currentTime - scaling.m_delayBefore) / scaling.m_motionDuration))) * (scaling.getEndingScale().y - scaling.getStartingScale().y) + scaling.getStartingScale().y);

					sprite.setScale(new_scale);
				}
				else if (scaling.m_currentTime - dt == 0.f)
					sprite.setScale(scaling.getStartingScale());
			}
		}

		++scalingRoutine;
	}

	for (auto rotationRoutine = this->m_Rotation_Routines_S.begin(); rotationRoutine != this->m_Rotation_Routines_S.end();) {
		if (!rotationRoutine->second->m_isPaused) {
			sf::Sprite& sprite = *rotationRoutine->first;
			rotationInfo& rotation = *rotationRoutine->second->getCurrentRotation();

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
					if (!rotationRoutine->second->goToNextRotation(sprite)) {
						rotationRoutine->second = nullptr;
						rotationRoutine = m_Rotation_Routines_S.erase(rotationRoutine);
						continue;
					}
				}
			}
			else {
				if (rotation.m_currentTime > rotation.m_delayBefore)
					sprite.setRotation(rotation.updateRotation());
				else if (rotation.m_currentTime - dt == 0.f)
					sprite.setRotation(rotation.getStartingRotation());
			}
		}
		
		++rotationRoutine;
	}
}

MovementRoutineEngine::MovementRoutineEngine(MovementManager* _MovementManager)
{
	assert(this->sInstance == nullptr);
	sInstance = this;
	this->movementManager = _MovementManager;
}

void MovementRoutineEngine::update(float dt)
{
	this->updateShape(dt);
	this->updateVertexArray(dt);
	this->updateSprite(dt);
}

const MovementRoutine* MovementRoutineEngine::addMovement(sf::Shape* _shape, MovementRoutine* _movementRoutine)
{
	auto& movementRoutineMap = sInstance->m_Movement_Routines_Shape;
	auto movementFound = movementRoutineMap.find(_shape);

	if (movementFound != movementRoutineMap.end()) {
		printf("MovementRoutineEngine::addMovement: MovementRoutine already exists for this shape\n");
		return nullptr;
	}
	
	movementRoutineMap.insert(std::make_pair(_shape, _movementRoutine));
	return _movementRoutine;
}

const MovementRoutine* MovementRoutineEngine::addMovement(VertexArray2* _vertexarray, MovementRoutine* _movementRoutine)
{
	auto& movementRoutineMap = sInstance->m_Movement_Routines_VA;
	auto movementFound = movementRoutineMap.find(_vertexarray);

	if (movementFound != movementRoutineMap.end()) {
		printf("MovementRoutineEngine::addMovement: MovementRoutine already exists for this vertexarray\n");
		return nullptr;
	}

	movementRoutineMap.insert(std::make_pair(_vertexarray, _movementRoutine));
	return _movementRoutine;
}

const MovementRoutine* MovementRoutineEngine::addMovement(sf::Sprite* _sprite, MovementRoutine* _movementRoutine)
{
	auto& movementRoutineMap = sInstance->m_Movement_Routines_S;
	auto movementFound = movementRoutineMap.find(_sprite);

	if (movementFound != movementRoutineMap.end()) {
		printf("MovementRoutineEngine::addMovement: MovementRoutine already exists for this sprite\n");
		return nullptr;
	}

	movementRoutineMap.insert(std::make_pair(_sprite, _movementRoutine));
	return _movementRoutine;
}

void MovementRoutineEngine::undoMovement()
{
}

void MovementRoutineEngine::undoMovement(sf::Shape* _shape)
{
}

void MovementRoutineEngine::undoMovement(VertexArray2* _vertexarray)
{
}

void MovementRoutineEngine::undoMovement(sf::Sprite* _sprite)
{
}

void MovementRoutineEngine::resetMovement()
{
}

void MovementRoutineEngine::resetMovement(sf::Shape* _shape)
{
}

void MovementRoutineEngine::resetMovement(VertexArray2* _vertexarray)
{
}

void MovementRoutineEngine::resetMovement(sf::Sprite* _sprite)
{
}

void MovementRoutineEngine::stopMovement()
{
	for (auto routine = this->m_Movement_Routines_Shape.begin(); routine != this->m_Movement_Routines_Shape.end();) {
		routine->second = nullptr;
		routine = this->m_Movement_Routines_Shape.erase(routine);
	}

	for (auto routine = this->m_Movement_Routines_VA.begin(); routine != this->m_Movement_Routines_VA.end();) {
		routine->second = nullptr;
		routine = this->m_Movement_Routines_VA.erase(routine);
	}

	for (auto routine = this->m_Movement_Routines_S.begin(); routine != this->m_Movement_Routines_S.end();) {
		routine->second = nullptr;
		routine = this->m_Movement_Routines_S.erase(routine);
	}
}

void MovementRoutineEngine::stopMovement(sf::Shape* _shape)
{
	auto& movementRoutineMap = sInstance->m_Movement_Routines_Shape;
	auto movementRoutineFound = movementRoutineMap.find(_shape);

	if (movementRoutineFound != movementRoutineMap.end()) {
		movementRoutineFound->second = nullptr;
		movementRoutineMap.erase(movementRoutineFound);
	}
}

void MovementRoutineEngine::stopMovement(VertexArray2* _vertexarray)
{
	auto& movementRoutineMap = sInstance->m_Movement_Routines_VA;
	auto movementRoutineFound = movementRoutineMap.find(_vertexarray);

	if (movementRoutineFound != movementRoutineMap.end()) {
		movementRoutineFound->second = nullptr;
		movementRoutineMap.erase(movementRoutineFound);
	}
}

void MovementRoutineEngine::stopMovement(sf::Sprite* _sprite)
{
	auto& movementRoutineMap = sInstance->m_Movement_Routines_S;
	auto movementRoutineFound = movementRoutineMap.find(_sprite);

	if (movementRoutineFound != movementRoutineMap.end()) {
		movementRoutineFound->second = nullptr;
		movementRoutineMap.erase(movementRoutineFound);
	}
}

const ScalingRoutine* MovementRoutineEngine::addScaling(sf::Shape* _shape, ScalingRoutine* _scalingRoutine)
{
	auto& scalingRoutineMap = sInstance->m_Scaling_Routines_Shape;
	auto scalingFound = scalingRoutineMap.find(_shape);

	if (scalingFound != scalingRoutineMap.end()) {
		printf("MovementRoutineEngine::addScaling: ScalingRoutine already exists for this shape\n");
		return nullptr;
	}

	scalingRoutineMap.insert(std::make_pair(_shape, _scalingRoutine));
	return _scalingRoutine;
}

const ScalingRoutine* MovementRoutineEngine::addScaling(VertexArray2* _vertexarray, ScalingRoutine* _scalingRoutine)
{
	auto& scalingRoutineMap = sInstance->m_Scaling_Routines_VA;
	auto scalingFound = scalingRoutineMap.find(_vertexarray);

	if (scalingFound != scalingRoutineMap.end()) {
		printf("MovementRoutineEngine::addScaling: ScalingRoutine already exists for this vertexarray\n");
		return nullptr;
	}

	scalingRoutineMap.insert(std::make_pair(_vertexarray, _scalingRoutine));
	return _scalingRoutine;
}

const ScalingRoutine* MovementRoutineEngine::addScaling(sf::Sprite* _sprite, ScalingRoutine* _scalingRoutine)
{
	auto& scalingRoutineMap = sInstance->m_Scaling_Routines_S;
	auto scalingFound = scalingRoutineMap.find(_sprite);

	if (scalingFound != scalingRoutineMap.end()) {
		printf("MovementRoutineEngine::addScaling: ScalingRoutine already exists for this sprite\n");
		return nullptr;
	}

	scalingRoutineMap.insert(std::make_pair(_sprite, _scalingRoutine));
	return _scalingRoutine;
}

void MovementRoutineEngine::undoScaling()
{
}

void MovementRoutineEngine::undoScaling(sf::Shape* _shape)
{
}

void MovementRoutineEngine::undoScaling(VertexArray2* _vertexarray)
{
}

void MovementRoutineEngine::undoScaling(sf::Sprite* _sprite)
{
}

void MovementRoutineEngine::resetScaling()
{
}

void MovementRoutineEngine::resetScaling(sf::Shape* _shape)
{
}

void MovementRoutineEngine::resetScaling(VertexArray2* _vertexarray)
{
}

void MovementRoutineEngine::resetScaling(sf::Sprite* _sprite)
{
}

void MovementRoutineEngine::stopScaling()
{
	for (auto routine = this->m_Scaling_Routines_Shape.begin(); routine != this->m_Scaling_Routines_Shape.end();) {
		routine->second = nullptr;
		routine = this->m_Scaling_Routines_Shape.erase(routine);
	}

	for (auto routine = this->m_Scaling_Routines_VA.begin(); routine != this->m_Scaling_Routines_VA.end();) {
		routine->second = nullptr;
		routine = this->m_Scaling_Routines_VA.erase(routine);
	}

	for (auto routine = this->m_Scaling_Routines_S.begin(); routine != this->m_Scaling_Routines_S.end();) {
		routine->second = nullptr;
		routine = this->m_Scaling_Routines_S.erase(routine);
	}
}

void MovementRoutineEngine::stopScaling(sf::Shape* _shape)
{
	auto& scalingRoutineMap = sInstance->m_Scaling_Routines_Shape;
	auto scalingRoutineFound = scalingRoutineMap.find(_shape);

	if (scalingRoutineFound != scalingRoutineMap.end()) {
		scalingRoutineFound->second = nullptr;
		scalingRoutineMap.erase(scalingRoutineFound);
	}
}

void MovementRoutineEngine::stopScaling(VertexArray2* _vertexarray)
{
	auto& scalingRoutineMap = sInstance->m_Scaling_Routines_VA;
	auto scalingRoutineFound = scalingRoutineMap.find(_vertexarray);

	if (scalingRoutineFound != scalingRoutineMap.end()) {
		scalingRoutineFound->second = nullptr;
		scalingRoutineMap.erase(scalingRoutineFound);
	}
}

void MovementRoutineEngine::stopScaling(sf::Sprite* _sprite)
{
	auto& scalingRoutineMap = sInstance->m_Scaling_Routines_S;
	auto scalingRoutineFound = scalingRoutineMap.find(_sprite);

	if (scalingRoutineFound != scalingRoutineMap.end()) {
		scalingRoutineFound->second = nullptr;
		scalingRoutineMap.erase(scalingRoutineFound);
	}
}

const RotationRoutine* MovementRoutineEngine::addRotation(sf::Shape* _shape, RotationRoutine* _rotationRoutine)
{
	auto& rotationRoutineMap = sInstance->m_Rotation_Routines_Shape;
	auto rotationFound = rotationRoutineMap.find(_shape);

	if (rotationFound != rotationRoutineMap.end()) {
		printf("RotationRoutineEngine::addRotation: RotationRoutine already exists for this shape\n");
		return nullptr;
	}

	rotationRoutineMap.insert(std::make_pair(_shape, _rotationRoutine));
	return _rotationRoutine;
}

const RotationRoutine* MovementRoutineEngine::addRotation(VertexArray2* _vertexarray, RotationRoutine* _rotationRoutine)
{
	auto& rotationRoutineMap = sInstance->m_Rotation_Routines_VA;
	auto rotationFound = rotationRoutineMap.find(_vertexarray);

	if (rotationFound != rotationRoutineMap.end()) {
		printf("RotationRoutineEngine::addRotation: RotationRoutine already exists for this shape\n");
		return nullptr;
	}

	rotationRoutineMap.insert(std::make_pair(_vertexarray, _rotationRoutine));
	return _rotationRoutine;
}

const RotationRoutine* MovementRoutineEngine::addRotation(sf::Sprite* _sprite, RotationRoutine* _rotationRoutine)
{
	auto& rotationRoutineMap = sInstance->m_Rotation_Routines_S;
	auto rotationFound = rotationRoutineMap.find(_sprite);

	if (rotationFound != rotationRoutineMap.end()) {
		printf("RotationRoutineEngine::addRotation: RotationRoutine already exists for this shape\n");
		return nullptr;
	}

	rotationRoutineMap.insert(std::make_pair(_sprite, _rotationRoutine));
	return _rotationRoutine;
}

void MovementRoutineEngine::undoRotation()
{
}

void MovementRoutineEngine::undoRotation(sf::Shape* _shape)
{
}

void MovementRoutineEngine::undoRotation(VertexArray2* _vertexarray)
{
}

void MovementRoutineEngine::undoRotation(sf::Sprite* _sprite)
{
}

void MovementRoutineEngine::resetRotation()
{
}

void MovementRoutineEngine::resetRotation(sf::Shape* _shape)
{
}

void MovementRoutineEngine::resetRotation(VertexArray2* _vertexarray)
{
}

void MovementRoutineEngine::resetRotation(sf::Sprite* _sprite)
{
}

void MovementRoutineEngine::stopRotation()
{
	for (auto routine = this->m_Rotation_Routines_Shape.begin(); routine != this->m_Rotation_Routines_Shape.end();) {
		routine->second = nullptr;
		routine = this->m_Rotation_Routines_Shape.erase(routine);
	}

	for (auto routine = this->m_Rotation_Routines_VA.begin(); routine != this->m_Rotation_Routines_VA.end();) {
		routine->second = nullptr;
		routine = this->m_Rotation_Routines_VA.erase(routine);
	}

	for (auto routine = this->m_Rotation_Routines_S.begin(); routine != this->m_Rotation_Routines_S.end();) {
		routine->second = nullptr;
		routine = this->m_Rotation_Routines_S.erase(routine);
	}
}

void MovementRoutineEngine::stopRotation(sf::Shape* _shape)
{
	auto& rotationRoutineMap = sInstance->m_Rotation_Routines_Shape;
	auto rotationRoutineFound = rotationRoutineMap.find(_shape);

	if (rotationRoutineFound != rotationRoutineMap.end()) {
		rotationRoutineFound->second = nullptr;
		rotationRoutineMap.erase(rotationRoutineFound);
	}
}

void MovementRoutineEngine::stopRotation(VertexArray2* _vertexarray)
{
	auto& rotationRoutineMap = sInstance->m_Rotation_Routines_VA;
	auto rotationRoutineFound = rotationRoutineMap.find(_vertexarray);

	if (rotationRoutineFound != rotationRoutineMap.end()) {
		rotationRoutineFound->second = nullptr;
		rotationRoutineMap.erase(rotationRoutineFound);
	}
}

void MovementRoutineEngine::stopRotation(sf::Sprite* _sprite)
{
	auto& rotationRoutineMap = sInstance->m_Rotation_Routines_S;
	auto rotationRoutineFound = rotationRoutineMap.find(_sprite);

	if (rotationRoutineFound != rotationRoutineMap.end()) {
		rotationRoutineFound->second = nullptr;
		rotationRoutineMap.erase(rotationRoutineFound);
	}
}

const bool MovementRoutineEngine::isMoving(sf::Shape* _shape)
{
	return false;
}

const bool MovementRoutineEngine::isMoving(VertexArray2* _vertexarray)
{
	return false;
}

const bool MovementRoutineEngine::isMoving(sf::Sprite* _sprite)
{
	return false;
}

const bool MovementRoutineEngine::isScaling(sf::Shape* _shape)
{
	return false;
}

const bool MovementRoutineEngine::isScaling(VertexArray2* _vertexarray)
{
	return false;
}

const bool MovementRoutineEngine::isScaling(sf::Sprite* _sprite)
{
	return false;
}

const bool MovementRoutineEngine::isRotating(sf::Shape* _shape)
{
	return false;
}

const bool MovementRoutineEngine::isRotating(VertexArray2* _vertexarray)
{
	return false;
}

const bool MovementRoutineEngine::isRotating(sf::Sprite* _sprite)
{
	return false;
}
