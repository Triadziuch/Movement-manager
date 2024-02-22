#pragma once
#include "MovementContainer.h"
#include "TransformationRoutine.h"

// Singleton initialization
MovementContainer* MovementContainer::sInstance = nullptr;

void MovementContainer::updateMovementInfoVA(sf::VertexArray* _vertexarray, movementInfoVA* _movementInfo, sf::Vector2f _offset)
{
	// === Movement ===
	_movementInfo->centroid += _offset;

	for (size_t i = 0; i < _vertexarray->getVertexCount(); i++)
		_vertexarray->operator[](i).position += _offset;

	// === Scaling ===
	auto& scalingMap = sInstance->m_Scalings_VA;
	auto scaling = scalingMap.find(_vertexarray);

	if (scaling != scalingMap.end()) {
		scaling->second->centroid = _movementInfo->centroid;

		for (size_t i = 0; i < scaling->second->originalVertex.getVertexCount(); i++)
			scaling->second->originalVertex.operator[](i).position += _offset;
	}

	// === Rotation ===
	auto& rotationMap = sInstance->m_Rotations_VA;
	auto rotation = rotationMap.find(_vertexarray);

	if (rotation != rotationMap.end()) {
		rotation->second->centroid = _movementInfo->centroid;

		for (size_t i = 0; i < rotation->second->originalVertex.getVertexCount(); i++)
			rotation->second->originalVertex.operator[](i).position += _offset;
	}
}

void MovementContainer::updateScalingInfoVA(sf::VertexArray* _vertexarray, scalingInfoVA* _scalingInfo)
{
	// VA Scaling
	for (size_t i = 0; i < _vertexarray->getVertexCount(); i++) {
		_vertexarray->operator[](i).position.x = _scalingInfo->centroid.x + (_scalingInfo->originalVertex.operator[](i).position.x - _scalingInfo->centroid.x) * _scalingInfo->current_scale.x;
		_vertexarray->operator[](i).position.y = _scalingInfo->centroid.y + (_scalingInfo->originalVertex.operator[](i).position.y - _scalingInfo->centroid.y) * _scalingInfo->current_scale.y;
	}


	// === Rotation ===
	auto& rotationMap = sInstance->m_Rotations_VA;
	auto rotation = rotationMap.find(_vertexarray);

	if (rotation != rotationMap.end())
		for (size_t i = 0; i < rotation->second->originalVertex.getVertexCount(); i++)
			rotation->second->originalVertex.operator[](i).position = _vertexarray->operator[](i).position;
}

void MovementContainer::updateRotationInfoVA(sf::VertexArray* _vertexarray, rotationInfoVA* _rotationInfo)
{
	float sine   = static_cast<float>(sin(static_cast<double>(_rotationInfo->current_rotation) * M_RAD)),
		  cosine = static_cast<float>(cos(static_cast<double>(_rotationInfo->current_rotation) * M_RAD));

	for (size_t i = 0; i < _vertexarray->getVertexCount(); ++i) {
		float x = _rotationInfo->originalVertex.operator[](i).position.x - _rotationInfo->centroid.x;
		float y = _rotationInfo->originalVertex.operator[](i).position.y - _rotationInfo->centroid.y;
		_vertexarray->operator[](i).position.x = _rotationInfo->centroid.x + x * cosine - y * sine;
		_vertexarray->operator[](i).position.y = _rotationInfo->centroid.y + x * sine + y * cosine;
	}
}

// Private update functions
void MovementContainer::updateShape(float dt)
{
	for (auto movement = this->m_Movements_Shape.begin(); movement != this->m_Movements_Shape.end();) {
		movement->second->current_time += dt;

		if (movement->second->isDone()) {
			if (movement->second->current_time - movement->second->delay_before - dt < movement->second->motion_duration)
				movement->first->setPosition(movement->second->ending_pos);

			if (movement->second->repeat) {
				if (movement->second->current_time - movement->second->motion_duration - movement->second->delay_before >= movement->second->delay_after) {
					movement->first->setPosition(movement->second->starting_pos);

					movement->second->current_time -= movement->second->total_duration;
				}
			}
			else if (movement->second->isFinished()) {
				delete movement->second;
				movement = m_Movements_Shape.erase(movement);
				continue;
			}
		}
		else {
			if (movement->second->current_time > movement->second->delay_before) {
				sf::Vector2f new_position(static_cast<float>(movement->second->used_function(static_cast<double>((movement->second->current_time - movement->second->delay_before) / movement->second->motion_duration))) * (movement->second->ending_pos.x - movement->second->starting_pos.x) + movement->second->starting_pos.x,
										  static_cast<float>(movement->second->used_function(static_cast<double>((movement->second->current_time - movement->second->delay_before) / movement->second->motion_duration))) * (movement->second->ending_pos.y - movement->second->starting_pos.y) + movement->second->starting_pos.y);

				movement->first->setPosition(new_position);
			}
			else if (movement->second->current_time - dt == 0.f)
				movement->first->setPosition(movement->second->starting_pos);
		}

		++movement;
	}

	for (auto scaling = this->m_Scalings_Shape.begin(); scaling != this->m_Scalings_Shape.end();) {
		scaling->second->current_time += dt;

		if (scaling->second->isDone()) {
			if (scaling->second->current_time - scaling->second->delay_before - dt < scaling->second->motion_duration)
				scaling->first->setScale(scaling->second->ending_scale);

			if (scaling->second->repeat) {
				if (scaling->second->current_time - scaling->second->motion_duration - scaling->second->delay_before >= scaling->second->delay_after) {
					scaling->first->setScale(scaling->second->starting_scale);
					
					scaling->second->current_time -= scaling->second->total_duration;
				}
			}
			else if (scaling->second->isFinished()) {
				delete scaling->second;
				scaling = m_Scalings_Shape.erase(scaling);
				continue;
			}
		}
		else {
			if (scaling->second->current_time > scaling->second->delay_before) {
				sf::Vector2f new_scale(static_cast<float>(scaling->second->used_function(static_cast<double>((scaling->second->current_time - scaling->second->delay_before) / scaling->second->motion_duration))) * (scaling->second->ending_scale.x - scaling->second->starting_scale.x) + scaling->second->starting_scale.x,
									   static_cast<float>(scaling->second->used_function(static_cast<double>((scaling->second->current_time - scaling->second->delay_before) / scaling->second->motion_duration))) * (scaling->second->ending_scale.y - scaling->second->starting_scale.y) + scaling->second->starting_scale.y);

				scaling->first->setScale(new_scale);
			}
			else if (scaling->second->current_time - dt == 0.f)
				scaling->first->setScale(scaling->second->starting_scale);
		}

		++scaling;
	}

	for (auto rotation = this->m_Rotations_Shape.begin(); rotation != this->m_Rotations_Shape.end();) {
		rotation->second->current_time += dt;

		if (rotation->second->isDone()) {
			if (rotation->second->current_time - rotation->second->delay_before - dt < rotation->second->motion_duration)
				rotation->first->setRotation(rotation->second->getEndingRotation());

			if (rotation->second->repeat) {
				if (rotation->second->current_time - rotation->second->motion_duration - rotation->second->delay_before >= rotation->second->delay_after) {
					rotation->first->setRotation(rotation->second->getStartingRotation());

					rotation->second->current_time -= rotation->second->total_duration;
				}
			}
			else if (rotation->second->isFinished()) {
				delete rotation->second;
				rotation = m_Rotations_Shape.erase(rotation);
				continue;
			}
		}
		else{
			if (rotation->second->current_time > rotation->second->delay_before)
				rotation->first->setRotation(rotation->second->updateRotation());
			else if (rotation->second->current_time - dt == 0.f)
				rotation->first->setRotation(rotation->second->getStartingRotation());
		}

		++rotation;
	}
}

void MovementContainer::updateVertexArray(float dt)
{
	for (auto movement = this->m_Movements_VA.begin(); movement != this->m_Movements_VA.end();) {
		movement->second->current_time += dt;

		if (movement->first->getVertexCount() != 0) {
			if (movement->second->isDone()) {
				if (movement->second->current_time - movement->second->delay_before - dt < movement->second->motion_duration) {
					sf::Vector2f offset(static_cast<float>(movement->second->used_function(1.0)) * (movement->second->ending_pos.x - movement->second->starting_pos.x) + movement->second->starting_pos.x - movement->second->centroid.x,
										static_cast<float>(movement->second->used_function(1.0)) * (movement->second->ending_pos.y - movement->second->starting_pos.y) + movement->second->starting_pos.y - movement->second->centroid.y);

					this->updateMovementInfoVA(movement->first, movement->second, offset);
				}

				if (movement->second->repeat) {
					if (movement->second->current_time - movement->second->motion_duration - movement->second->delay_before >= movement->second->delay_after) {
						sf::Vector2f offset(movement->second->ending_pos - movement->second->starting_pos);

						this->updateMovementInfoVA(movement->first, movement->second, -offset);

						movement->second->current_time -= movement->second->total_duration;
					}
				}
				else if (movement->second->isFinished()) {
					delete movement->second;
					movement = m_Movements_VA.erase(movement);
					continue;
				}
			}
			else {
				if (movement->second->current_time > movement->second->delay_before) {
					sf::Vector2f offset(static_cast<float>(movement->second->used_function(static_cast<double>(movement->second->current_time / movement->second->motion_duration))) * (movement->second->ending_pos.x - movement->second->starting_pos.x) + movement->second->starting_pos.x - movement->second->centroid.x,
										static_cast<float>(movement->second->used_function(static_cast<double>(movement->second->current_time / movement->second->motion_duration))) * (movement->second->ending_pos.y - movement->second->starting_pos.y) + movement->second->starting_pos.y - movement->second->centroid.y);

					this->updateMovementInfoVA(movement->first, movement->second, offset);
				}
				else if (movement->second->current_time - dt == 0.f) {
					sf::Vector2f offset(movement->second->starting_pos - movement->second->centroid);
					this->updateMovementInfoVA(movement->first, movement->second, offset);
				}
			}
		}

		++movement;
	}

	for (auto scaling = this->m_Scalings_VA.begin(); scaling != this->m_Scalings_VA.end();) {
		scaling->second->current_time += dt;

		if (scaling->first->getVertexCount() != 0) {
			if (scaling->second->isDone()) {
				if (scaling->second->current_time - scaling->second->delay_before - dt < scaling->second->motion_duration) {

					scaling->second->current_scale = scaling->second->ending_scale;
					updateScalingInfoVA(scaling->first, scaling->second);
				}

				if (scaling->second->repeat) {
					if (scaling->second->current_time - scaling->second->motion_duration - scaling->second->delay_before < scaling->second->delay_after) {
						scaling->second->current_scale = scaling->second->ending_scale;
						updateScalingInfoVA(scaling->first, scaling->second);
					}
					else {
						scaling->second->current_scale = scaling->second->starting_scale;
						updateScalingInfoVA(scaling->first, scaling->second);

						scaling->second->current_time -= scaling->second->total_duration;
					}
				}
				else if (scaling->second->isFinished()) {
					delete scaling->second;
					scaling = m_Scalings_VA.erase(scaling);
					continue;
				}
			}
			else {
				if (scaling->second->current_time > scaling->second->delay_before) {
					sf::Vector2f new_scale(scaling->second->used_function(static_cast<double>((scaling->second->current_time - scaling->second->delay_before) / scaling->second->motion_duration)) * (scaling->second->ending_scale.x - scaling->second->starting_scale.x) + scaling->second->starting_scale.x,
										   scaling->second->used_function(static_cast<double>((scaling->second->current_time - scaling->second->delay_before) / scaling->second->motion_duration)) * (scaling->second->ending_scale.y - scaling->second->starting_scale.y) + scaling->second->starting_scale.y);
					
					scaling->second->current_scale = new_scale;
					updateScalingInfoVA(scaling->first, scaling->second);
				}
				else if (scaling->second->current_time - dt == 0.f) {
					scaling->second->current_scale = scaling->second->starting_scale;
					updateScalingInfoVA(scaling->first, scaling->second);
				}
			}
		}

		++scaling;
	}

	for (auto rotation = this->m_Rotations_VA.begin(); rotation != this->m_Rotations_VA.end();) {
		rotation->second->current_time += dt;

		if (rotation->first->getVertexCount() != 0) {
			if (rotation->second->isDone()) {
				if (rotation->second->current_time - rotation->second->delay_before - dt < rotation->second->motion_duration) {
					rotation->second->current_rotation = rotation->second->getEndingRotation();
					updateRotationInfoVA(rotation->first, rotation->second);
				}

				if (rotation->second->repeat) {
					if (rotation->second->current_time - rotation->second->motion_duration - rotation->second->delay_before < rotation->second->delay_after) {
						rotation->second->current_rotation = rotation->second->getEndingRotation();
						updateRotationInfoVA(rotation->first, rotation->second);
					}
						
					else {
						rotation->second->current_rotation = rotation->second->getStartingRotation();
						updateRotationInfoVA(rotation->first, rotation->second);

						rotation->second->current_time -= rotation->second->total_duration;
					}
				}
				else if (rotation->second->isFinished()) {
					delete rotation->second;
					rotation = m_Rotations_VA.erase(rotation);
					continue;
				}
			}
			else{
				if (rotation->second->current_time > rotation->second->delay_before) {
					rotation->second->current_rotation = rotation->second->updateRotation();
					updateRotationInfoVA(rotation->first, rotation->second);
				}
				else if (rotation->second->current_time - dt == 0.f) {
					rotation->second->current_rotation = rotation->second->getStartingRotation();
					updateRotationInfoVA(rotation->first, rotation->second);
				}
			}
		}

		++rotation;
	}
}

void MovementContainer::updateSprite(float dt)
{
	for (auto movement = this->m_Movements_S.begin(); movement != this->m_Movements_S.end();) {
		movement->second->current_time += dt;

		if (movement->second->isDone()) {
			if (movement->second->current_time - movement->second->delay_before - dt < movement->second->motion_duration)
				movement->first->setPosition(movement->second->ending_pos);

			if (movement->second->repeat) {
				if (movement->second->current_time - movement->second->motion_duration - movement->second->delay_before >= movement->second->delay_after) {
					movement->first->setPosition(movement->second->starting_pos);

					movement->second->current_time -= movement->second->total_duration;
				}
			}
			else if (movement->second->isFinished()) {
				delete movement->second;
				movement = m_Movements_S.erase(movement);
				continue;
			}
		}
		else {
			if (movement->second->current_time > movement->second->delay_before) {
				sf::Vector2f new_position(static_cast<float>(movement->second->used_function(static_cast<double>((movement->second->current_time - movement->second->delay_before) / movement->second->motion_duration))) * (movement->second->ending_pos.x - movement->second->starting_pos.x) + movement->second->starting_pos.x,
										  static_cast<float>(movement->second->used_function(static_cast<double>((movement->second->current_time - movement->second->delay_before) / movement->second->motion_duration))) * (movement->second->ending_pos.y - movement->second->starting_pos.y) + movement->second->starting_pos.y);

				movement->first->setPosition(new_position);
			}
			else if (movement->second->current_time - dt == 0.f)
				movement->first->setPosition(movement->second->starting_pos);
		}
		++movement;
	}

	for (auto scaling = this->m_Scalings_S.begin(); scaling != this->m_Scalings_S.end();) {
		scaling->second->current_time += dt;

		if (scaling->second->isDone()) {
			if (scaling->second->current_time - scaling->second->delay_before - dt < scaling->second->motion_duration)
				scaling->first->setScale(scaling->second->ending_scale);

			if (scaling->second->repeat) {
				if (scaling->second->current_time - scaling->second->motion_duration - scaling->second->delay_before >= scaling->second->delay_after) {
					scaling->first->setScale(scaling->second->starting_scale);

					scaling->second->current_time -= scaling->second->total_duration;
				}
			}
			else if (scaling->second->isFinished()) {
				delete scaling->second;
				scaling = m_Scalings_S.erase(scaling);
				continue;
			}
		}
		else {
			if (scaling->second->current_time > scaling->second->delay_before) {
				sf::Vector2f new_scale(static_cast<float>(scaling->second->used_function(static_cast<double>((scaling->second->current_time - scaling->second->delay_before) / scaling->second->motion_duration))) * (scaling->second->ending_scale.x - scaling->second->starting_scale.x) + scaling->second->starting_scale.x,
									   static_cast<float>(scaling->second->used_function(static_cast<double>((scaling->second->current_time - scaling->second->delay_before) / scaling->second->motion_duration))) * (scaling->second->ending_scale.y - scaling->second->starting_scale.y) + scaling->second->starting_scale.y);

				scaling->first->setScale(new_scale);
			}
			else if (scaling->second->current_time - dt == 0.f)
				scaling->first->setScale(scaling->second->starting_scale);
		}

		++scaling;
	}

	for (auto rotation = this->m_Rotations_S.begin(); rotation != this->m_Rotations_S.end();) {
		rotation->second->current_time += dt;

		if (rotation->second->isDone()) {
			if (rotation->second->current_time - rotation->second->delay_before - dt < rotation->second->motion_duration)
				rotation->first->setRotation(rotation->second->getEndingRotation());

			if (rotation->second->repeat) {
				if (rotation->second->current_time - rotation->second->motion_duration - rotation->second->delay_before >= rotation->second->delay_after) {
					rotation->first->setRotation(rotation->second->getStartingRotation());

					rotation->second->current_time -= rotation->second->total_duration;
				}
			}
			else if (rotation->second->isFinished()) {
				delete rotation->second;
				rotation = m_Rotations_S.erase(rotation);
				continue;
			}
		}
		else {
			if (rotation->second->current_time > rotation->second->delay_before)
				rotation->first->setRotation(rotation->second->updateRotation());
			else if (rotation->second->current_time - dt == 0.f)
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

const movementInfoVA* MovementContainer::addMovement(sf::VertexArray* _vertexarray, movementInfoVA* _movementInfo)
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

const movementInfoVA* MovementContainer::addMovement(sf::VertexArray* _vertexarray, sf::Vector2f _ending_pos, float _movement_time, movement_type _used_function, bool _repeat, float _delay_before, float _delay_after)
{
	auto& movementMap = sInstance->m_Movements_VA;
	auto movementFound = movementMap.find(_vertexarray);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::VertexArray* object already exists!\n");
		return nullptr;
	}

	movementInfoVA* newMovement = new movementInfoVA(_vertexarray->operator[](0).position, _ending_pos, _movement_time, sInstance->movement_functions[_used_function], _repeat, _delay_before, _delay_after, _vertexarray);
	movementMap.insert(std::make_pair(_vertexarray, newMovement));

	return newMovement;
}

const movementInfoVA* MovementContainer::addMovement(sf::VertexArray* _vertexarray, sf::Vector2f _starting_pos, sf::Vector2f _ending_pos, float _movement_time, movement_type _used_function, bool _repeat, float _delay_before, float _delay_after)
{
	auto& movementMap = sInstance->m_Movements_VA;
	auto movementFound = movementMap.find(_vertexarray);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::VertexArray* object already exists!\n");
		return nullptr;
	}

	movementInfoVA* newMovement = new movementInfoVA(_starting_pos, _ending_pos, _movement_time, sInstance->movement_functions[_used_function], _repeat, _delay_before, _delay_after, _vertexarray);
	movementMap.insert(std::make_pair(_vertexarray, newMovement));

	return newMovement;
}

const movementInfoVA* MovementContainer::addMovement(sf::VertexArray* _vertexarray, float _offset_x, float _offset_y, float _movement_time, movement_type _used_function, bool _repeat, float _delay_before, float _delay_after)
{
	if (_vertexarray->getVertexCount() == 0) {
		printf("ERROR: VertexArray has no vertices!\n");
		return nullptr;
	}
	return this->addMovement(_vertexarray, _vertexarray->operator[](0).position + sf::Vector2f(_offset_x, _offset_y), _movement_time, _used_function, _repeat, _delay_before, _delay_after);
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

const movementInfo* MovementContainer::addMovement(sf::Sprite* _sprite, sf::Vector2f _ending_pos, float _movement_time, movement_type _used_function, bool _repeat, float _delay_before, float _delay_after)
{
	auto& movementMap = sInstance->m_Movements_S;
	auto movementFound = movementMap.find(_sprite);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::Sprite* object already exists!\n");
		return nullptr;
	}
	
	movementInfo* newMovement = new movementInfo(_sprite->getPosition(), _ending_pos, _movement_time, sInstance->movement_functions[_used_function], _repeat, _delay_before, _delay_after);
	movementMap.insert(std::make_pair(_sprite, newMovement));

	return newMovement;
}

const movementInfo* MovementContainer::addMovement(sf::Sprite* _sprite, sf::Vector2f _starting_pos, sf::Vector2f _ending_pos, float _movement_time, movement_type _used_function, bool _repeat, float _delay_before, float _delay_after)
{
	auto& movementMap = sInstance->m_Movements_S;
	auto movementFound = movementMap.find(_sprite);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::Sprite* object already exists!\n");
		return nullptr;
	}

	movementInfo* newMovement = new movementInfo(_starting_pos, _ending_pos, _movement_time, sInstance->movement_functions[_used_function], _repeat, _delay_before, _delay_after);
	movementMap.insert(std::make_pair(_sprite, newMovement));

	return newMovement;
}

const movementInfo* MovementContainer::addMovement(sf::Sprite* _sprite, float _offset_x, float _offset_y, float _movement_time, movement_type _used_function, bool _repeat, float _delay_before, float _delay_after)
{
	return this->addMovement(_sprite, _sprite->getPosition() + sf::Vector2f(_offset_x, _offset_y), _movement_time, _used_function, _repeat, _delay_before, _delay_after);
}

void MovementContainer::undoMovement()
{
	for (auto movement = this->m_Movements_Shape.begin(); movement != this->m_Movements_Shape.end();) {
		movement->first->setPosition(movement->second->starting_pos);
		delete movement->second;
		movement = m_Movements_Shape.erase(movement);
	}

	for (auto movement = this->m_Movements_VA.begin(); movement != this->m_Movements_VA.end();) {
		sf::Vector2f offset(movement->second->centroid - movement->second->originalCentroid);
		updateMovementInfoVA(movement->first, movement->second, -offset);

		delete movement->second;
		movement = m_Movements_VA.erase(movement);
	}

	for (auto movement = this->m_Movements_S.begin(); movement != this->m_Movements_S.end();) {
		movement->first->setPosition(movement->second->starting_pos);
		delete movement->second;
		movement = m_Movements_S.erase(movement);
	}
}

void MovementContainer::undoMovement(sf::Shape* _shape)
{
	auto& movementMap = sInstance->m_Movements_Shape;
	auto movementFound = movementMap.find(_shape);

	if (movementFound != movementMap.end()) {
		_shape->setPosition(movementFound->second->starting_pos);
		delete movementFound->second;
		movementMap.erase(movementFound);
	}
}

void MovementContainer::undoMovement(sf::VertexArray* _vertexarray)
{
	auto& movementMap = sInstance->m_Movements_VA;
	auto movementFound = movementMap.find(_vertexarray);

	if (movementFound != movementMap.end()) {
		sf::Vector2f offset(movementFound->second->centroid - movementFound->second->originalCentroid);
		updateMovementInfoVA(_vertexarray, movementFound->second, -offset);

		delete movementFound->second;
		movementMap.erase(movementFound);
	}
}

void MovementContainer::undoMovement(sf::Sprite* _sprite)
{
	auto& movementMap = sInstance->m_Movements_S;
	auto movementFound = movementMap.find(_sprite);

	if (movementFound != movementMap.end()) {
		_sprite->setPosition(movementFound->second->starting_pos);
		delete movementFound->second;
		movementMap.erase(movementFound);
	}
}

void MovementContainer::resetMovement()
{
	for (auto movement = this->m_Movements_Shape.begin(); movement != this->m_Movements_Shape.end();) {
		movement->first->setPosition(movement->second->starting_pos);
		movement->second->current_time = 0.f;
		++movement;
	}

	for (auto movement = this->m_Movements_VA.begin(); movement != this->m_Movements_VA.end();) {
		sf::Vector2f offset(movement->second->centroid - movement->second->originalCentroid);
		updateMovementInfoVA(movement->first, movement->second, -offset);
		
		movement->second->current_time = 0.f;
		++movement;
	}

	for (auto movement = this->m_Movements_S.begin(); movement != this->m_Movements_S.end();) {
		movement->first->setPosition(movement->second->starting_pos);
		movement->second->current_time = 0.f;
		++movement;
	}
}

void MovementContainer::resetMovement(sf::Shape* _shape)
{
	auto& movementMap = sInstance->m_Movements_Shape;
	auto movementFound = movementMap.find(_shape);

	if (movementFound != movementMap.end()) {
		_shape->setPosition(movementFound->second->starting_pos);
		movementFound->second->current_time = 0.f;
	}
}

void MovementContainer::resetMovement(sf::VertexArray* _vertexarray)
{
	auto& movementMap = sInstance->m_Movements_VA;
	auto movementFound = movementMap.find(_vertexarray);

	if (movementFound != movementMap.end()) {
		sf::Vector2f offset(movementFound->second->centroid - movementFound->second->originalCentroid);
		updateMovementInfoVA(_vertexarray, movementFound->second, -offset);

		movementFound->second->current_time = 0.f;
	}	
}

void MovementContainer::resetMovement(sf::Sprite* _sprite)
{
	auto& movementMap = sInstance->m_Movements_S;
	auto movementFound = movementMap.find(_sprite);

	if (movementFound != movementMap.end()) {
		_sprite->setPosition(movementFound->second->starting_pos);
		movementFound->second->current_time = 0.f;
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

void MovementContainer::stopMovement(sf::VertexArray* _vertexarray)
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
const bool MovementContainer::addScaling(sf::Shape* _shape, sf::Vector2f _ending_scale, float _scaling_time, movement_type _used_function, bool _repeat, float _delay_before, float _delay_after)
{
	auto& scalingMap = sInstance->m_Scalings_Shape;
	auto scalingFound = scalingMap.find(_shape);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::Shape* object already exists!\n");
		return 0;
	}
	else {
		scalingInfo* newScaling = new scalingInfo(_shape->getScale(), _ending_scale, _scaling_time, sInstance->movement_functions[_used_function], _repeat, _delay_before, _delay_after);
		scalingMap.insert(std::make_pair(_shape, newScaling));
	}
	return 1;
}

const bool MovementContainer::addScaling(sf::Shape* _shape, sf::Vector2f _starting_scale, sf::Vector2f _ending_scale, float _scaling_time, movement_type _used_function, bool _repeat, float _delay_before, float _delay_after)
{
	auto& scalingMap = sInstance->m_Scalings_Shape;
	auto scalingFound = scalingMap.find(_shape);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::Shape* object already exists!\n");
		return 0;
	}
	else {
		scalingInfo* newScaling = new scalingInfo(_starting_scale, _ending_scale, _scaling_time, sInstance->movement_functions[_used_function], _repeat, _delay_before, _delay_after);
		scalingMap.insert(std::make_pair(_shape, newScaling));
	}
	return 1;
}

const bool MovementContainer::addScaling(sf::VertexArray* _vertexarray, sf::Vector2f _ending_scale, float _scaling_time, movement_type _used_function, bool _repeat, float _delay_before, float _delay_after)
{
	auto& scalingMap = sInstance->m_Scalings_VA;
	auto scalingFound = scalingMap.find(_vertexarray);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::VertexArray* object already exists!\n");
		return 0;
	}
	else if (_vertexarray->getVertexCount() != 0){
		scalingInfoVA* newScaling = new scalingInfoVA({ 1.f, 1.f }, _ending_scale, _scaling_time, sInstance->movement_functions[_used_function], _repeat, _delay_before, _delay_after, _vertexarray);
		scalingMap.insert(std::make_pair(_vertexarray, newScaling));
	}
	return 1;
}

const bool MovementContainer::addScaling(sf::VertexArray* _vertexarray, sf::Vector2f _starting_scale, sf::Vector2f _ending_scale, float _scaling_time, movement_type _used_function, bool _repeat, float _delay_before, float _delay_after)
{
	auto& scalingMap = sInstance->m_Scalings_VA;
	auto scalingFound = scalingMap.find(_vertexarray);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::VertexArray* object already exists!\n");
		return 0;
	}
	else if (_vertexarray->getVertexCount() != 0) {
		scalingInfoVA* newScaling = new scalingInfoVA(_starting_scale, _ending_scale, _scaling_time, sInstance->movement_functions[_used_function], _repeat, _delay_before, _delay_after, _vertexarray);
		scalingMap.insert(std::make_pair(_vertexarray, newScaling));
	}
	return 1;
}

const bool MovementContainer::addScaling(sf::Sprite* _sprite, sf::Vector2f _ending_scale, float _scaling_time, movement_type _used_function, bool _repeat, float _delay_before, float _delay_after)
{
	auto& scalingMap = sInstance->m_Scalings_S;
	auto scalingFound = scalingMap.find(_sprite);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::Sprite* object already exists!\n");
		return 0;
	}
	else {
		scalingInfo* newScaling = new scalingInfo(_sprite->getScale(), _ending_scale, _scaling_time, sInstance->movement_functions[_used_function], _repeat, _delay_before, _delay_after);
		scalingMap.insert(std::make_pair(_sprite, newScaling));
	}
	return 1;
}

const bool MovementContainer::addScaling(sf::Sprite* _sprite, sf::Vector2f _starting_scale, sf::Vector2f _ending_scale, float _scaling_time, movement_type _used_function, bool _repeat, float _delay_before, float _delay_after)
{
	auto& scalingMap = sInstance->m_Scalings_S;
	auto scalingFound = scalingMap.find(_sprite);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::Sprite* object already exists!\n");
		return 0;
	}
	else {
		scalingInfo* newScaling = new scalingInfo(_starting_scale, _ending_scale, _scaling_time, sInstance->movement_functions[_used_function], _repeat, _delay_before, _delay_after);
		scalingMap.insert(std::make_pair(_sprite, newScaling));
	}
	return 1;
}

void MovementContainer::undoScaling()
{
	for (auto scaling = this->m_Scalings_Shape.begin(); scaling != this->m_Scalings_Shape.end();) {
		scaling->first->setScale(scaling->second->starting_scale);
		delete scaling->second;
		scaling = m_Scalings_Shape.erase(scaling);
	}

	for (auto scaling = this->m_Scalings_VA.begin(); scaling != this->m_Scalings_VA.end();) {
		scaling->second->current_scale = scaling->second->starting_scale;
		updateScalingInfoVA(scaling->first, scaling->second);

		auto& rotationMap = sInstance->m_Rotations_VA;
		auto rotationFound = rotationMap.find(scaling->first);

		if (rotationFound != rotationMap.end())
			updateRotationInfoVA(scaling->first, rotationFound->second);

		delete scaling->second;
		scaling = m_Scalings_VA.erase(scaling);
	}

	for (auto scaling = this->m_Scalings_S.begin(); scaling != this->m_Scalings_S.end();) {
		scaling->first->setScale(scaling->second->starting_scale);
		delete scaling->second;
		scaling = m_Scalings_S.erase(scaling);
	}
}

void MovementContainer::undoScaling(sf::Shape* _shape)
{
	auto& scalingMap = sInstance->m_Scalings_Shape;
	auto scalingFound = scalingMap.find(_shape);

	if (scalingFound != scalingMap.end()) {
		_shape->setScale(scalingFound->second->starting_scale);
		delete scalingFound->second;
		scalingMap.erase(scalingFound);
	}
}

void MovementContainer::undoScaling(sf::VertexArray* _vertexarray)
{
	auto& scalingMap = sInstance->m_Scalings_VA;
	auto scalingFound = scalingMap.find(_vertexarray);

	if (scalingFound != scalingMap.end()) {
		scalingFound->second->current_scale = scalingFound->second->starting_scale;
		updateScalingInfoVA(_vertexarray, scalingFound->second);

		auto& rotationMap = sInstance->m_Rotations_VA;
		auto rotationFound = rotationMap.find(_vertexarray);

		if (rotationFound != rotationMap.end())
			updateRotationInfoVA(_vertexarray, rotationFound->second);

		delete scalingFound->second;
		scalingMap.erase(scalingFound);
	}
}

void MovementContainer::undoScaling(sf::Sprite* _sprite)
{
	auto& scalingMap = sInstance->m_Scalings_S;
	auto scalingFound = scalingMap.find(_sprite);

	if (scalingFound != scalingMap.end()) {
		_sprite->setScale(scalingFound->second->starting_scale);
		delete scalingFound->second;
		scalingMap.erase(scalingFound);
	}
}

void MovementContainer::resetScaling()
{
	for (auto scaling = this->m_Scalings_Shape.begin(); scaling != this->m_Scalings_Shape.end();) {
		scaling->first->setScale(scaling->second->starting_scale);
		scaling->second->current_time = 0.f;
		++scaling;
	}

	for (auto scaling = this->m_Scalings_VA.begin(); scaling != this->m_Scalings_VA.end();) {
		scaling->second->current_scale = scaling->second->starting_scale;
		updateScalingInfoVA(scaling->first, scaling->second);

		auto& rotationMap = sInstance->m_Rotations_VA;
		auto rotationFound = rotationMap.find(scaling->first);

		if (rotationFound != rotationMap.end())
			updateRotationInfoVA(scaling->first, rotationFound->second);

		scaling->second->current_time = 0.f;
		++scaling;
	}

	for (auto scaling = this->m_Scalings_S.begin(); scaling != this->m_Scalings_S.end();) {
		scaling->first->setScale(scaling->second->starting_scale);
		scaling->second->current_time = 0.f;
		++scaling;
	}
}

void MovementContainer::resetScaling(sf::Shape* _shape)
{
	auto& scalingMap = sInstance->m_Scalings_Shape;
	auto scalingFound = scalingMap.find(_shape);

	if (scalingFound != scalingMap.end()) {
		_shape->setScale(scalingFound->second->starting_scale);
		scalingFound->second->current_time = 0.f;
	}
}

void MovementContainer::resetScaling(sf::VertexArray* _vertexarray)
{
	auto& scalingMap = sInstance->m_Scalings_VA;
	auto scalingFound = scalingMap.find(_vertexarray);

	if (scalingFound != scalingMap.end()) {
		scalingFound->second->current_scale = scalingFound->second->starting_scale;
		updateScalingInfoVA(_vertexarray, scalingFound->second);

		auto& rotationMap = sInstance->m_Rotations_VA;
		auto rotationFound = rotationMap.find(_vertexarray);

		if (rotationFound != rotationMap.end())
			updateRotationInfoVA(_vertexarray, rotationFound->second);

		scalingFound->second->current_time = 0.f;
	}
}

void MovementContainer::resetScaling(sf::Sprite* _sprite)
{
	auto& scalingMap = sInstance->m_Scalings_S;
	auto scalingFound = scalingMap.find(_sprite);

	if (scalingFound != scalingMap.end()) {
		_sprite->setScale(scalingFound->second->starting_scale);
		scalingFound->second->current_time = 0.f;
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

void MovementContainer::stopScaling(sf::VertexArray* _vertexarray)
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
const bool MovementContainer::addRotation(sf::Shape* _shape, float _ending_rotation, float _rotation_time, movement_type _used_function, bool _clockwise, bool _repeat, float _delay_before, float _delay_after)
{
	auto& rotationMap = sInstance->m_Rotations_Shape;
	auto rotationFound = rotationMap.find(_shape);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::Shape* object already exists!\n");
		return 0;
	}
	else {
		rotationInfo* newRotation = new rotationInfo(_shape->getRotation(), _ending_rotation, _rotation_time, sInstance->movement_functions[_used_function], _repeat, _delay_before, _delay_after, _clockwise);
		rotationMap.insert(std::make_pair(_shape, newRotation));
	}
	return 1;
}

const bool MovementContainer::addRotation(sf::Shape* _shape, float _starting_rotation, float _ending_rotation, float _rotation_time, movement_type _used_function, bool _clockwise, bool _repeat, float _delay_before, float _delay_after)
{
	auto& rotationMap = sInstance->m_Rotations_Shape;
	auto rotationFound = rotationMap.find(_shape);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::Shape* object already exists!\n");
		return 0;
	}
	else {
		rotationInfo* newRotation = new rotationInfo(_starting_rotation, _ending_rotation, _rotation_time, sInstance->movement_functions[_used_function], _repeat, _delay_before, _delay_after, _clockwise);
		rotationMap.insert(std::make_pair(_shape, newRotation));
	}
	return 1;
}

const bool MovementContainer::addRotation(sf::VertexArray* _vertexarray, float _ending_rotation, float _rotation_time, movement_type _used_function, bool _clockwise, bool _repeat, float _delay_before, float _delay_after)
{
	auto& rotationMap = sInstance->m_Rotations_VA;
	auto rotationFound = rotationMap.find(_vertexarray);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::VertexArray* object already exists!\n");
		return 0;
	}
	else if (_vertexarray->getVertexCount() != 0) {
		rotationInfoVA* newRotation = new rotationInfoVA(0.f, _ending_rotation, _rotation_time, sInstance->movement_functions[_used_function], _vertexarray, _repeat, _delay_before, _delay_after, _clockwise);
		rotationMap.insert(std::make_pair(_vertexarray, newRotation));
	}
	return 1;
}

const bool MovementContainer::addRotation(sf::VertexArray* _vertexarray, float _starting_rotation, float _ending_rotation, float _rotation_time, movement_type _used_function, bool _clockwise, bool _repeat, float _delay_before, float _delay_after)
{
	auto& rotationMap = sInstance->m_Rotations_VA;
	auto rotationFound = rotationMap.find(_vertexarray);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::VertexArray* object already exists!\n");
		return 0;
	}
	else if (_vertexarray->getVertexCount() != 0) {
		rotationInfoVA* newRotation = new rotationInfoVA(_starting_rotation, _ending_rotation, _rotation_time, sInstance->movement_functions[_used_function], _vertexarray, _repeat, _delay_before, _delay_after, _clockwise);
		rotationMap.insert(std::make_pair(_vertexarray, newRotation));
	}
	return 1;
}

const bool MovementContainer::addRotation(sf::Sprite* _sprite, float _ending_rotation, float _rotation_time, movement_type _used_function, bool _clockwise, bool _repeat, float _delay_before, float _delay_after)
{
	auto& rotationMap = sInstance->m_Rotations_S;
	auto rotationFound = rotationMap.find(_sprite);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::Sprite* object already exists!\n");
		return 0;
	}
	else {
		rotationInfo* newRotation = new rotationInfo(_sprite->getRotation(), _ending_rotation, _rotation_time, sInstance->movement_functions[_used_function], _repeat, _delay_before, _delay_after, _clockwise);
		rotationMap.insert(std::make_pair(_sprite, newRotation));
	}
}

const bool MovementContainer::addRotation(sf::Sprite* _sprite, float _starting_rotation, float _ending_rotation, float _rotation_time, movement_type _used_function, bool _clockwise, bool _repeat, float _delay_before, float _delay_after)
{
	auto& rotationMap = sInstance->m_Rotations_S;
	auto rotationFound = rotationMap.find(_sprite);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::Sprite* object already exists!\n");
		return 0;
	}
	else {
		rotationInfo* newRotation = new rotationInfo(_starting_rotation, _ending_rotation, _rotation_time, sInstance->movement_functions[_used_function], _repeat, _delay_before, _delay_after, _clockwise);
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
		rotation->second->current_rotation = rotation->second->getStartingRotation();
		updateRotationInfoVA(rotation->first, rotation->second);

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

void MovementContainer::undoRotation(sf::VertexArray* _vertexarray)
{
	auto& rotationMap = sInstance->m_Rotations_VA;
	auto rotationFound = rotationMap.find(_vertexarray);

	if (rotationFound != rotationMap.end()) {
		rotationFound->second->current_rotation = rotationFound->second->getStartingRotation();
		updateRotationInfoVA(_vertexarray, rotationFound->second);

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
		rotation->second->current_time = 0.f;
		++rotation;
	}

	for (auto rotation = this->m_Rotations_VA.begin(); rotation != this->m_Rotations_VA.end();) {
		rotation->second->current_rotation = rotation->second->getStartingRotation();
		updateRotationInfoVA(rotation->first, rotation->second);

		rotation->second->current_time = 0.f;
		++rotation;
	}

	for (auto rotation = this->m_Rotations_S.begin(); rotation != this->m_Rotations_S.end();) {
		rotation->first->setRotation(rotation->second->getStartingRotation());
		rotation->second->current_time = 0.f;
		++rotation;
	}
}

void MovementContainer::resetRotation(sf::Shape* _shape)
{
	auto& rotationMap = sInstance->m_Rotations_Shape;
	auto rotationFound = rotationMap.find(_shape);

	if (rotationFound != rotationMap.end()) {
		_shape->setRotation(rotationFound->second->getStartingRotation());
		rotationFound->second->current_time = 0.f;
	}
}

void MovementContainer::resetRotation(sf::VertexArray* _vertexarray)
{
	auto& rotationMap = sInstance->m_Rotations_VA;
	auto rotationFound = rotationMap.find(_vertexarray);

	if (rotationFound != rotationMap.end()) {
		rotationFound->second->current_rotation = rotationFound->second->getStartingRotation();
		updateRotationInfoVA(_vertexarray, rotationFound->second);

		rotationFound->second->current_time = 0.f;
	}
}

void MovementContainer::resetRotation(sf::Sprite* _sprite)
{
	auto& rotationMap = sInstance->m_Rotations_S;
	auto rotationFound = rotationMap.find(_sprite);

	if (rotationFound != rotationMap.end()) {
		_sprite->setRotation(rotationFound->second->getStartingRotation());
		rotationFound->second->current_time = 0.f;
	}
}

void MovementContainer::stopRotation()
{
	for (auto rotation = this->m_Rotations_Shape.begin(); rotation != this->m_Rotations_Shape.end();) {
		delete rotation->second;
		rotation = m_Rotations_Shape.erase(rotation);
	}

	for (auto rotation = this->m_Rotations_VA.begin(); rotation != this->m_Rotations_VA.end();) {
		auto& movementMap = sInstance->m_Movements_VA;
		auto movementFound = movementMap.find(rotation->first);

		if (movementFound != movementMap.end()) {
			float sine   = static_cast<float>(sin(static_cast<double>(rotation->second->current_rotation) * M_RAD)),
				  cosine = static_cast<float>(cos(static_cast<double>(rotation->second->current_rotation) * M_RAD));

			for (size_t i = 0; i < rotation->first->getVertexCount(); ++i) {
				float x = movementFound->second->originalVertex.operator[](i).position.x - movementFound->second->centroid.x;
				float y = movementFound->second->originalVertex.operator[](i).position.y - movementFound->second->centroid.y;
				movementFound->second->originalVertex.operator[](i).position.x = movementFound->second->centroid.x + x * cosine - y * sine;
				movementFound->second->originalVertex.operator[](i).position.y = movementFound->second->centroid.y + x * sine + y * cosine;
			}
		}

		auto& scalingMap = sInstance->m_Scalings_VA;
		auto scalingFound = scalingMap.find(rotation->first);

		if (scalingFound != scalingMap.end()) {
			float sine   = static_cast<float>(sin(static_cast<double>(rotation->second->current_rotation) * M_RAD)),
				  cosine = static_cast<float>(cos(static_cast<double>(rotation->second->current_rotation) * M_RAD));

			for (size_t i = 0; i < rotation->first->getVertexCount(); ++i) {
				float x = scalingFound->second->originalVertex.operator[](i).position.x - scalingFound->second->centroid.x;
				float y = scalingFound->second->originalVertex.operator[](i).position.y - scalingFound->second->centroid.y;
				scalingFound->second->originalVertex.operator[](i).position.x = scalingFound->second->centroid.x + x * cosine - y * sine;
				scalingFound->second->originalVertex.operator[](i).position.y = scalingFound->second->centroid.y + x * sine + y * cosine;
			}
		}

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

void MovementContainer::stopRotation(sf::VertexArray* _vertexarray)
{
	auto& rotationMap = sInstance->m_Rotations_VA;
	auto rotationFound = rotationMap.find(_vertexarray);

	if (rotationFound != rotationMap.end()) {
		auto& movementMap = sInstance->m_Movements_VA;
		auto movementFound = movementMap.find(_vertexarray);

		if (movementFound != movementMap.end()) {
			float sine   = static_cast<float>(sin(static_cast<double>(rotationFound->second->current_rotation) * M_RAD)),
				  cosine = static_cast<float>(cos(static_cast<double>(rotationFound->second->current_rotation) * M_RAD));

			for (size_t i = 0; i < _vertexarray->getVertexCount(); ++i) {
				float x = movementFound->second->originalVertex.operator[](i).position.x - movementFound->second->centroid.x;
				float y = movementFound->second->originalVertex.operator[](i).position.y - movementFound->second->centroid.y;
				movementFound->second->originalVertex.operator[](i).position.x = movementFound->second->centroid.x + x * cosine - y * sine;
				movementFound->second->originalVertex.operator[](i).position.y = movementFound->second->centroid.y + x * sine + y * cosine;
			}
		}

		auto& scalingMap = sInstance->m_Scalings_VA;
		auto scalingFound = scalingMap.find(_vertexarray);

		if (scalingFound != scalingMap.end()) {
			float sine   = static_cast<float>(sin(static_cast<double>(rotationFound->second->current_rotation) * M_RAD)),
				  cosine = static_cast<float>(cos(static_cast<double>(rotationFound->second->current_rotation) * M_RAD));

			for (size_t i = 0; i < _vertexarray->getVertexCount(); ++i) {
				float x = scalingFound->second->originalVertex.operator[](i).position.x - scalingFound->second->centroid.x;
				float y = scalingFound->second->originalVertex.operator[](i).position.y - scalingFound->second->centroid.y;
				scalingFound->second->originalVertex.operator[](i).position.x = scalingFound->second->centroid.x + x * cosine - y * sine;
				scalingFound->second->originalVertex.operator[](i).position.y = scalingFound->second->centroid.y + x * sine + y * cosine;
			}
		}

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

const bool MovementContainer::isMoving(sf::VertexArray* _vertexarray)
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

const bool MovementContainer::isScaling(sf::VertexArray* _vertexarray)
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

const bool MovementContainer::isRotating(sf::VertexArray* _vertexarray)
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

void MovementRoutineEngine::updateMovementInfoVA(sf::VertexArray* _vertexarray, movementInfoVA* _movementInfo, sf::Vector2f _offset)
{
	if (_offset == sf::Vector2f{}) return;

	// === Movement ===
	_movementInfo->centroid += _offset;

	for (size_t i = 0; i < _vertexarray->getVertexCount(); i++)
		_vertexarray->operator[](i).position += _offset;

	// === Scaling ===
	auto& scalingRoutineMap = sInstance->m_Scaling_Routines_VA;
	auto scalingRoutine = scalingRoutineMap.find(_vertexarray);

	if (scalingRoutine != scalingRoutineMap.end()) 
		scalingRoutine->second->updateMovementInfoVA(_offset);
	

	// === Rotation ===
	auto& rotationRoutineMap = sInstance->m_Rotation_Routines_VA; // To pewnie trzeba zmieniæ tak jak scaling ¿eby zmienia³o pozycjê wszystkich ruchów w rutynie
	auto rotationRoutine = rotationRoutineMap.find(_vertexarray);

	if (rotationRoutine != rotationRoutineMap.end()) {
		rotationInfoVA* rotation = rotationRoutine->second->getCurrentRotation();
		rotation->centroid = _movementInfo->centroid;

		for (size_t i = 0; i < rotation->originalVertex.getVertexCount(); i++)
			rotation->originalVertex.operator[](i).position += _offset;
	}
}

void MovementRoutineEngine::updateScalingInfoVA(sf::VertexArray* _vertexarray, scalingInfoVA* _scalingInfo)
{
	// VA Scaling
	for (size_t i = 0; i < _vertexarray->getVertexCount(); i++) {
		_vertexarray->operator[](i).position.x = _scalingInfo->centroid.x + (_scalingInfo->originalVertex.operator[](i).position.x - _scalingInfo->centroid.x) * _scalingInfo->current_scale.x;
		_vertexarray->operator[](i).position.y = _scalingInfo->centroid.y + (_scalingInfo->originalVertex.operator[](i).position.y - _scalingInfo->centroid.y) * _scalingInfo->current_scale.y;
	}


	// === Rotation ===
	auto& rotationRoutineMap = sInstance->m_Rotation_Routines_VA;
	auto rotationRoutine = rotationRoutineMap.find(_vertexarray);

	if (rotationRoutine != rotationRoutineMap.end()) {
		rotationInfoVA* rotation = rotationRoutine->second->getCurrentRotation();

		for (size_t i = 0; i < rotation->originalVertex.getVertexCount(); i++)
			rotation->originalVertex.operator[](i).position = _vertexarray->operator[](i).position;
	}
}

void MovementRoutineEngine::updateRotationInfoVA(sf::VertexArray* _vertexarray, rotationInfoVA* _rotationInfo)
{
	float sine = static_cast<float>(sin(static_cast<double>(_rotationInfo->current_rotation) * M_RAD)),
		  cosine = static_cast<float>(cos(static_cast<double>(_rotationInfo->current_rotation) * M_RAD));

	for (size_t i = 0; i < _vertexarray->getVertexCount(); ++i) {
		float x = _rotationInfo->originalVertex.operator[](i).position.x - _rotationInfo->centroid.x;
		float y = _rotationInfo->originalVertex.operator[](i).position.y - _rotationInfo->centroid.y;
		_vertexarray->operator[](i).position.x = _rotationInfo->centroid.x + x * cosine - y * sine;
		_vertexarray->operator[](i).position.y = _rotationInfo->centroid.y + x * sine + y * cosine;
	}
}

void MovementRoutineEngine::updateShape(float dt)
{
	for (auto movementRoutine = this->m_Movement_Routines_Shape.begin(); movementRoutine != this->m_Movement_Routines_Shape.end();) {
		if (!movementRoutine->second->is_paused) {
			sf::Shape* shape = movementRoutine->first;
			movementInfo* movement = movementRoutine->second->getCurrentMovement();

			movement->current_time += dt;

			if (movement->isDone()) {
				if (movement->current_time - movement->delay_before - dt < movement->motion_duration)
					shape->setPosition(movement->ending_pos);

				if (movement->repeat) {
					if (movement->current_time - movement->motion_duration - movement->delay_before >= movement->delay_after) {
						shape->setPosition(movement->starting_pos);

						movement->current_time -= movement->total_duration;
					}
				}
				else if (movement->isFinished()) {
					if (!movementRoutine->second->goToNextMovement(shape)) {
						movementRoutine->second = nullptr;
						movementRoutine = m_Movement_Routines_Shape.erase(movementRoutine);
						continue;
					}
				}
			}
			else {
				if (movement->current_time > movement->delay_before) {
					sf::Vector2f new_position(static_cast<float>(movement->used_function(static_cast<double>((movement->current_time - movement->delay_before) / movement->motion_duration))) * (movement->ending_pos.x - movement->starting_pos.x) + movement->starting_pos.x,
											  static_cast<float>(movement->used_function(static_cast<double>((movement->current_time - movement->delay_before) / movement->motion_duration))) * (movement->ending_pos.y - movement->starting_pos.y) + movement->starting_pos.y);

					shape->setPosition(new_position);
				}
				else if (movement->current_time - dt == 0.f)
					shape->setPosition(movement->starting_pos);
			}
		}

		++movementRoutine;
	}

	for (auto scalingRoutine = this->m_Scaling_Routines_Shape.begin(); scalingRoutine != this->m_Scaling_Routines_Shape.end();) {
		if (!scalingRoutine->second->is_paused) {
			sf::Shape* shape = scalingRoutine->first;
			scalingInfo* scaling = scalingRoutine->second->getCurrentScaling();

			scaling->current_time += dt;

			if (scaling->isDone()) {
				if (scaling->current_time - scaling->delay_before - dt < scaling->motion_duration)
					shape->setScale(scaling->ending_scale);

				if (scaling->repeat) {
					if (scaling->current_time - scaling->motion_duration - scaling->delay_before >= scaling->delay_after) {
						shape->setScale(scaling->starting_scale);

						scaling->current_time -= scaling->total_duration;
					}
				}
				else if (scaling->isFinished()) {
					if (!scalingRoutine->second->goToNextScaling(shape)) {
						scalingRoutine->second = nullptr;
						scalingRoutine = m_Scaling_Routines_Shape.erase(scalingRoutine);
						continue;
					}
				}
			}
			else {
				if (scaling->current_time > scaling->delay_before) {
					sf::Vector2f new_scale(static_cast<float>(scaling->used_function(static_cast<double>((scaling->current_time - scaling->delay_before) / scaling->motion_duration))) * (scaling->ending_scale.x - scaling->starting_scale.x) + scaling->starting_scale.x,
										   static_cast<float>(scaling->used_function(static_cast<double>((scaling->current_time - scaling->delay_before) / scaling->motion_duration))) * (scaling->ending_scale.y - scaling->starting_scale.y) + scaling->starting_scale.y);

					shape->setScale(new_scale);
				}
				else if (scaling->current_time - dt == 0.f)
					shape->setScale(scaling->starting_scale);
			}
		}

		++scalingRoutine;
	}

	for (auto rotationRoutine = this->m_Rotation_Routines_Shape.begin(); rotationRoutine != this->m_Rotation_Routines_Shape.end();) {
		if (!rotationRoutine->second->is_paused) {
			sf::Shape* shape = rotationRoutine->first;
			rotationInfo* rotation = rotationRoutine->second->getCurrentRotation();

			rotation->current_time += dt;

			if (rotation->isDone()) {
				if (rotation->current_time - rotation->delay_before - dt < rotation->motion_duration)
					shape->setRotation(rotation->getEndingRotation());

				if (rotation->repeat) {
					if (rotation->current_time - rotation->motion_duration - rotation->delay_before >= rotation->delay_after) {
						shape->setRotation(rotation->getStartingRotation());

						rotation->current_time -= rotation->total_duration;
					}
				}
				else if (rotation->isFinished()) {
					if (!rotationRoutine->second->goToNextRotation()) {
						rotationRoutine->second = nullptr;
						rotationRoutine = m_Rotation_Routines_Shape.erase(rotationRoutine);
						continue;
					}
				}
			}
			else {
				if (rotation->current_time > rotation->delay_before)
					shape->setRotation(rotation->updateRotation());
				else if (rotation->current_time - dt == 0.f)
					shape->setRotation(rotation->getStartingRotation());

			}
		}

		++rotationRoutine;
	}
}

void MovementRoutineEngine::updateVertexArray(float dt)
{
	for (auto movementRoutine = this->m_Movement_Routines_VA.begin(); movementRoutine != this->m_Movement_Routines_VA.end();) {
		if (!movementRoutine->second->is_paused) {
			sf::VertexArray* vertexarray = movementRoutine->first;
			movementInfoVA* movement = movementRoutine->second->getCurrentMovement();

			movement->current_time += dt;

			if (vertexarray->getVertexCount() != 0) {
				if (movement->isDone()) {
					if (movement->current_time - movement->delay_before - dt < movement->motion_duration) {
						sf::Vector2f offset(static_cast<float>(movement->used_function(1.0)) * (movement->ending_pos.x - movement->starting_pos.x) + movement->starting_pos.x - movement->centroid.x,
											static_cast<float>(movement->used_function(1.0)) * (movement->ending_pos.y - movement->starting_pos.y) + movement->starting_pos.y - movement->centroid.y);

						this->updateMovementInfoVA(vertexarray, movement, offset);
					}

					if (movement->repeat) {
						if (movement->current_time - movement->motion_duration - movement->delay_before >= movement->delay_after) {
							sf::Vector2f offset(movement->ending_pos - movement->starting_pos);

							this->updateMovementInfoVA(vertexarray, movement, -offset);

							movement->current_time -= movement->total_duration;
						}
					}
					else if (movement->isFinished()) {
						if (!movementRoutine->second->goToNextMovement(vertexarray)) {
							movementRoutine->second = nullptr;
							movementRoutine = m_Movement_Routines_VA.erase(movementRoutine);
							continue;
						}
					}					
				}
				else {
					if (movement->current_time > movement->delay_before) {
						sf::Vector2f offset(static_cast<float>(movement->used_function(static_cast<double>((movement->current_time - movement->delay_before) / movement->motion_duration))) * (movement->ending_pos.x - movement->starting_pos.x) + movement->starting_pos.x - movement->centroid.x,
											static_cast<float>(movement->used_function(static_cast<double>((movement->current_time - movement->delay_before) / movement->motion_duration))) * (movement->ending_pos.y - movement->starting_pos.y) + movement->starting_pos.y - movement->centroid.y);

						this->updateMovementInfoVA(vertexarray, movement, offset);
					}
					else if (movement->current_time - dt == 0.f) {

						sf::Vector2f offset(movement->starting_pos - movement->centroid);
						this->updateMovementInfoVA(vertexarray, movement, offset);

						auto& scalingRoutineMap = sInstance->m_Scaling_Routines_VA;
						auto scalingRoutine = scalingRoutineMap.find(vertexarray);

						if (scalingRoutine != scalingRoutineMap.end()) {
							const sf::Vector2f scalingOriginalVertextoCurrentMovementOffset = movement->centroid - scalingRoutine->second->getCurrentScaling()->centroid;
							scalingRoutine->second->updateMovementInfoVA(scalingOriginalVertextoCurrentMovementOffset);
						}
					}
				}
			}
		}

		++movementRoutine;
	}

	for (auto scalingRoutine = this->m_Scaling_Routines_VA.begin(); scalingRoutine != this->m_Scaling_Routines_VA.end();) {
		if (!scalingRoutine->second->is_paused) {
			sf::VertexArray* vertexarray = scalingRoutine->first;
			scalingInfoVA* scaling = scalingRoutine->second->getCurrentScaling();

			scaling->current_time += dt;

			if (vertexarray->getVertexCount() != 0) {
				if (scaling->isDone()) {
					if (scaling->current_time - scaling->delay_before - dt < scaling->motion_duration) {

						scaling->current_scale = scaling->ending_scale;
						updateScalingInfoVA(vertexarray, scaling);
					}

					if (scaling->repeat) {
						if (scaling->current_time - scaling->motion_duration - scaling->delay_before < scaling->delay_after) {
							scaling->current_scale = scaling->ending_scale;
							updateScalingInfoVA(vertexarray, scaling);
						}
						else {
							scaling->current_scale = scaling->starting_scale;
							updateScalingInfoVA(vertexarray, scaling);

							scaling->current_time -= scaling->total_duration;
						}
					}
					else if (scaling->isFinished()) {
						if (!scalingRoutine->second->goToNextScaling(vertexarray)) {
							scalingRoutine->second = nullptr;
							scalingRoutine = m_Scaling_Routines_VA.erase(scalingRoutine);
							continue;
						}
					}
				}
				else {
					if (scaling->current_time > scaling->delay_before) {
						sf::Vector2f new_scale(scaling->used_function(static_cast<double>((scaling->current_time - scaling->delay_before) / scaling->motion_duration)) * (scaling->ending_scale.x - scaling->starting_scale.x) + scaling->starting_scale.x,
											   scaling->used_function(static_cast<double>((scaling->current_time - scaling->delay_before) / scaling->motion_duration)) * (scaling->ending_scale.y - scaling->starting_scale.y) + scaling->starting_scale.y);

						scaling->current_scale = new_scale;
						updateScalingInfoVA(vertexarray, scaling);
					}
					else if (scaling->current_time - dt == 0.f) {
 						scaling->current_scale = scaling->starting_scale;
						updateScalingInfoVA(vertexarray, scaling);

						auto& movementRoutineMap = sInstance->m_Movement_Routines_VA;
						auto movementRoutine = movementRoutineMap.find(vertexarray);

						if (movementRoutine != movementRoutineMap.end()) {
							//movementRoutine->second->adjustVertexarrayToStartingPosition(vertexarray);
								//movement.second->adjustVertexarrayToStartingPosition(vertexarray);
							
						}
					}
					
				}
			}
		}

		++scalingRoutine;
	}

	for (auto rotationRoutine = this->m_Rotation_Routines_VA.begin(); rotationRoutine != this->m_Rotation_Routines_VA.end();) {
		if (!rotationRoutine->second->is_paused) {
			sf::VertexArray* vertexarray = rotationRoutine->first;
			rotationInfoVA* rotation = rotationRoutine->second->getCurrentRotation();

			rotation->current_time += dt;

			if (vertexarray->getVertexCount() != 0) {
				if (rotation->isDone()) {
					if (rotation->current_time - rotation->delay_before - dt < rotation->motion_duration) {
						rotation->current_rotation = rotation->getEndingRotation();
						updateRotationInfoVA(vertexarray, rotation);
					}

					if (rotation->repeat) {
						if (rotation->current_time - rotation->motion_duration - rotation->delay_before < rotation->delay_after) {
							rotation->current_rotation = rotation->getEndingRotation();
							updateRotationInfoVA(vertexarray, rotation);
						}

						else {
							rotation->current_rotation = rotation->getStartingRotation();
							updateRotationInfoVA(vertexarray, rotation);

							rotation->current_time -= rotation->total_duration;
						}
					}
					else if (rotation->isFinished()) {
						if (!rotationRoutine->second->goToNextRotation()) {
							rotationRoutine->second = nullptr;
							rotationRoutine = m_Rotation_Routines_VA.erase(rotationRoutine);
							continue;
						}
					}
				}
				else {
					if (rotation->current_time > rotation->delay_before) {
						rotation->current_rotation = rotation->updateRotation();
						updateRotationInfoVA(vertexarray, rotation);
					}
					else if (rotation->current_time - dt == 0.f) {
						rotation->current_rotation = rotation->getStartingRotation();
						updateRotationInfoVA(vertexarray, rotation);
					}
				}
			}
		}

		++rotationRoutine;
	}
}

void MovementRoutineEngine::updateSprite(float dt)
{
	for (auto movementRoutine = this->m_Movement_Routines_S.begin(); movementRoutine != this->m_Movement_Routines_S.end();) {
		if (!movementRoutine->second->is_paused) {
			sf::Sprite* sprite = movementRoutine->first;
			movementInfo* movement = movementRoutine->second->getCurrentMovement();

			movement->current_time += dt;

			if (movement->isDone()) {
				if (movement->current_time - movement->delay_before - dt < movement->motion_duration)
					sprite->setPosition(movement->ending_pos);

				if (movement->repeat) {
					if (movement->current_time - movement->motion_duration - movement->delay_before >= movement->delay_after) {
						sprite->setPosition(movement->starting_pos);

						movement->current_time -= movement->total_duration;
					}
				}
				else if (movement->isFinished()) {
					if (!movementRoutine->second->goToNextMovement(sprite)) {
						movementRoutine->second = nullptr;
						movementRoutine = m_Movement_Routines_S.erase(movementRoutine);
						continue;
					}
				}
			}
			else {
				if (movement->current_time > movement->delay_before) {
					sf::Vector2f new_position(static_cast<float>(movement->used_function(static_cast<double>((movement->current_time - movement->delay_before) / movement->motion_duration))) * (movement->ending_pos.x - movement->starting_pos.x) + movement->starting_pos.x,
											  static_cast<float>(movement->used_function(static_cast<double>((movement->current_time - movement->delay_before) / movement->motion_duration))) * (movement->ending_pos.y - movement->starting_pos.y) + movement->starting_pos.y);

					sprite->setPosition(new_position);
				}
				else if (movement->current_time - dt == 0.f)
					sprite->setPosition(movement->starting_pos);
			}
		}

		++movementRoutine;
	}

	for (auto scalingRoutine = this->m_Scaling_Routines_S.begin(); scalingRoutine != this->m_Scaling_Routines_S.end();) {
		if (!scalingRoutine->second->is_paused) {
			sf::Sprite* sprite = scalingRoutine->first;
			scalingInfo* scaling = scalingRoutine->second->getCurrentScaling();

			scaling->current_time += dt;

			if (scaling->isDone()) {
				if (scaling->current_time - scaling->delay_before - dt < scaling->motion_duration)
					sprite->setScale(scaling->ending_scale);

				if (scaling->repeat) {
					if (scaling->current_time - scaling->motion_duration - scaling->delay_before >= scaling->delay_after) {
						sprite->setScale(scaling->starting_scale);

						scaling->current_time -= scaling->total_duration;
					}
				}
				else if (scaling->isFinished()) {
					if (!scalingRoutine->second->goToNextScaling(sprite)) {
						scalingRoutine->second = nullptr;
						scalingRoutine = m_Scaling_Routines_S.erase(scalingRoutine);
						continue;
					}
				}
			}
			else {
				if (scaling->current_time > scaling->delay_before) {
					sf::Vector2f new_scale(static_cast<float>(scaling->used_function(static_cast<double>((scaling->current_time - scaling->delay_before) / scaling->motion_duration))) * (scaling->ending_scale.x - scaling->starting_scale.x) + scaling->starting_scale.x,
										   static_cast<float>(scaling->used_function(static_cast<double>((scaling->current_time - scaling->delay_before) / scaling->motion_duration))) * (scaling->ending_scale.y - scaling->starting_scale.y) + scaling->starting_scale.y);

					sprite->setScale(new_scale);
				}
				else if (scaling->current_time - dt == 0.f)
					sprite->setScale(scaling->starting_scale);
			}
		}

		++scalingRoutine;
	}

	for (auto rotationRoutine = this->m_Rotation_Routines_S.begin(); rotationRoutine != this->m_Rotation_Routines_S.end();) {
		if (!rotationRoutine->second->is_paused) {
			sf::Sprite* sprite = rotationRoutine->first;
			rotationInfo* rotation = rotationRoutine->second->getCurrentRotation();

			rotation->current_time += dt;

			if (rotation->isDone()) {
				if (rotation->current_time - rotation->delay_before - dt < rotation->motion_duration)
					sprite->setRotation(rotation->getEndingRotation());

				if (rotation->repeat) {
					if (rotation->current_time - rotation->motion_duration - rotation->delay_before >= rotation->delay_after) {
						sprite->setRotation(rotation->getStartingRotation());

						rotation->current_time -= rotation->total_duration;
					}
				}
				else if (rotation->isFinished()) {
					if (!rotationRoutine->second->goToNextRotation()) {
						rotationRoutine->second = nullptr;
						rotationRoutine = m_Rotation_Routines_S.erase(rotationRoutine);
						continue;
					}
				}
			}
			else {
				if (rotation->current_time > rotation->delay_before)
					sprite->setRotation(rotation->updateRotation());
				else if (rotation->current_time - dt == 0.f)
					sprite->setRotation(rotation->getStartingRotation());
			}
		}
		
		++rotationRoutine;
	}
}

MovementRoutineEngine::MovementRoutineEngine()
{
	assert(this->sInstance == nullptr);
	sInstance = this;
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

const MovementRoutineVA* MovementRoutineEngine::addMovement(sf::VertexArray* _vertexarray, MovementRoutineVA* _movementRoutine)
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

void MovementRoutineEngine::undoMovement(sf::VertexArray* _vertexarray)
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

void MovementRoutineEngine::resetMovement(sf::VertexArray* _vertexarray)
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

void MovementRoutineEngine::stopMovement(sf::VertexArray* _vertexarray)
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

const ScalingRoutineVA* MovementRoutineEngine::addScaling(sf::VertexArray* _vertexarray, ScalingRoutineVA* _scalingRoutine)
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

void MovementRoutineEngine::undoScaling(sf::VertexArray* _vertexarray)
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

void MovementRoutineEngine::resetScaling(sf::VertexArray* _vertexarray)
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

void MovementRoutineEngine::stopScaling(sf::VertexArray* _vertexarray)
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
	return nullptr;
}

const RotationRoutineVA* MovementRoutineEngine::addRotation(sf::VertexArray* _vertexarray, RotationRoutineVA* _rotationRoutine)
{
	return nullptr;
}

const RotationRoutine* MovementRoutineEngine::addRotation(sf::Sprite* _sprite, RotationRoutine* _rotationRoutine)
{
	return nullptr;
}

void MovementRoutineEngine::undoRotation()
{
}

void MovementRoutineEngine::undoRotation(sf::Shape* _shape)
{
}

void MovementRoutineEngine::undoRotation(sf::VertexArray* _vertexarray)
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

void MovementRoutineEngine::resetRotation(sf::VertexArray* _vertexarray)
{
}

void MovementRoutineEngine::resetRotation(sf::Sprite* _sprite)
{
}

void MovementRoutineEngine::stopRotation()
{
}

void MovementRoutineEngine::stopRotation(sf::Shape* _shape)
{
}

void MovementRoutineEngine::stopRotation(sf::VertexArray* _vertexarray)
{
}

void MovementRoutineEngine::stopRotation(sf::Sprite* _sprite)
{
}

const bool MovementRoutineEngine::isMoving(sf::Shape* _shape)
{
	return false;
}

const bool MovementRoutineEngine::isMoving(sf::VertexArray* _vertexarray)
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

const bool MovementRoutineEngine::isScaling(sf::VertexArray* _vertexarray)
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

const bool MovementRoutineEngine::isRotating(sf::VertexArray* _vertexarray)
{
	return false;
}

const bool MovementRoutineEngine::isRotating(sf::Sprite* _sprite)
{
	return false;
}
