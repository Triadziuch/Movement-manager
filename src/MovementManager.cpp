#include "MovementManager.h"
#include <assert.h>

// Singleton initialization
MovementManager* MovementManager::sInstance = nullptr;

void MovementManager::updateMovementInfoVA(sf::VertexArray* _vertexarray, movementInfoVA* _movementInfo, sf::Vector2f _offset)
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

void MovementManager::updateScalingInfoVA(sf::VertexArray* _vertexarray, scalingInfoVA* _scalingInfo)
{
	// VA Scaling
	for (size_t i = 0; i < _vertexarray->getVertexCount(); i++) {
		_vertexarray->operator[](i).position.x = _scalingInfo->centroid.x + (_scalingInfo->originalVertex.operator[](i).position.x - _scalingInfo->centroid.x) * _scalingInfo->currentScale.x;
		_vertexarray->operator[](i).position.y = _scalingInfo->centroid.y + (_scalingInfo->originalVertex.operator[](i).position.y - _scalingInfo->centroid.y) * _scalingInfo->currentScale.y;
	}


	// === Rotation ===
	auto& rotationMap = sInstance->m_Rotations_VA;
	auto rotation = rotationMap.find(_vertexarray);

	if (rotation != rotationMap.end())
		for (size_t i = 0; i < rotation->second->originalVertex.getVertexCount(); i++)
			rotation->second->originalVertex.operator[](i).position = _vertexarray->operator[](i).position;
}

void MovementManager::updateRotationInfoVA(sf::VertexArray* _vertexarray, rotationInfoVA* _rotationInfo)
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
void MovementManager::updateShape(float dt)
{
	for (auto movement = this->m_Movements_Shape.begin(); movement != this->m_Movements_Shape.end();) {
		movement->second->current_time += dt;

		if (movement->second->isDone()) {
			if (movement->second->current_time - dt < movement->second->movement_time)
				movement->first->setPosition(movement->second->ending_pos);

			if (movement->second->repeat) {
				if (movement->second->current_time - movement->second->movement_time >= movement->second->wait_before_repeating) {
					movement->first->setPosition(movement->second->starting_pos);

					movement->second->current_time -= (movement->second->movement_time + movement->second->wait_before_repeating);
				}
				++movement;
			}
			else {
				delete movement->second;
				movement = m_Movements_Shape.erase(movement);
			}
		}
		else {
			sf::Vector2f new_position(static_cast<float>(movement->second->used_function(static_cast<double>(movement->second->current_time / movement->second->movement_time))) * (movement->second->ending_pos.x - movement->second->starting_pos.x) + movement->second->starting_pos.x,
									  static_cast<float>(movement->second->used_function(static_cast<double>(movement->second->current_time / movement->second->movement_time))) * (movement->second->ending_pos.y - movement->second->starting_pos.y) + movement->second->starting_pos.y);

			movement->first->setPosition(new_position);
			++movement;
		}
	}

	for (auto scaling = this->m_Scalings_Shape.begin(); scaling != this->m_Scalings_Shape.end();) {
		scaling->second->current_time += dt;

		if (scaling->second->isDone()) {
			if (scaling->second->current_time - dt < scaling->second->scaling_time)
				scaling->first->setScale(scaling->second->ending_scale);

			if (scaling->second->repeat) {
				if (scaling->second->current_time - scaling->second->scaling_time >= scaling->second->wait_before_repeating) {
					scaling->first->setScale(scaling->second->starting_scale);
					
					scaling->second->current_time -= (scaling->second->scaling_time + scaling->second->wait_before_repeating);
				}
				++scaling;
			}
			else {
				delete scaling->second;
				scaling = m_Scalings_Shape.erase(scaling);
			}
		}
		else {
			sf::Vector2f new_scale(static_cast<float>(scaling->second->used_function(static_cast<double>(scaling->second->current_time / scaling->second->scaling_time))) * (scaling->second->ending_scale.x - scaling->second->starting_scale.x) + scaling->second->starting_scale.x,
								   static_cast<float>(scaling->second->used_function(static_cast<double>(scaling->second->current_time / scaling->second->scaling_time))) * (scaling->second->ending_scale.y - scaling->second->starting_scale.y) + scaling->second->starting_scale.y);

			scaling->first->setScale(new_scale);
			++scaling;
		}
	}

	for (auto rotation = this->m_Rotations_Shape.begin(); rotation != this->m_Rotations_Shape.end();) {
		rotation->second->current_time += dt;

		if (rotation->second->isDone()) {
			if (rotation->second->current_time - dt < rotation->second->rotation_time)
				rotation->first->setRotation(rotation->second->ending_rotation);

			if (rotation->second->repeat) {
				if (rotation->second->current_time - rotation->second->rotation_time >= rotation->second->wait_before_repeating) {
					rotation->first->setRotation(rotation->second->starting_rotation);

					rotation->second->current_time -= (rotation->second->rotation_time + rotation->second->wait_before_repeating);
				}
				++rotation;
			}
			else {
				delete rotation->second;
				rotation = m_Rotations_Shape.erase(rotation);
			}
		}
		else {
			float new_rotation = static_cast<float>(rotation->second->used_function(static_cast<double>(rotation->second->current_time / rotation->second->rotation_time))) * (rotation->second->ending_rotation - rotation->second->starting_rotation) + rotation->second->starting_rotation;
			
			if (!rotation->second->clockwise) {
				new_rotation = 360.f - new_rotation;
				new_rotation = fmod(new_rotation, 360.f);
			}

			rotation->first->setRotation(new_rotation);
			++rotation;
		}
	}
}

void MovementManager::updateVertexArray(float dt)
{
	for (auto movement = this->m_Movements_VA.begin(); movement != this->m_Movements_VA.end();) {
		movement->second->current_time += dt;

		if (movement->first->getVertexCount() != 0) {
			if (movement->second->isDone()) {
				if (movement->second->current_time - dt < movement->second->movement_time) {
					sf::Vector2f offset(static_cast<float>(movement->second->used_function(1.0)) * (movement->second->ending_pos.x - movement->second->starting_pos.x) + movement->second->starting_pos.x - movement->second->centroid.x,
										static_cast<float>(movement->second->used_function(1.0)) * (movement->second->ending_pos.y - movement->second->starting_pos.y) + movement->second->starting_pos.y - movement->second->centroid.y);

					this->updateMovementInfoVA(movement->first, movement->second, offset);
				}

				if (movement->second->repeat) {
					if (movement->second->current_time - movement->second->movement_time >= movement->second->wait_before_repeating){
						sf::Vector2f offset(movement->second->ending_pos - movement->second->starting_pos);

						this->updateMovementInfoVA(movement->first, movement->second, -offset);

						movement->second->current_time -= (movement->second->movement_time + movement->second->wait_before_repeating);
					}
					++movement;
				}
				else {
					delete movement->second;
					movement = m_Movements_VA.erase(movement);
				}
			}
			else {
				sf::Vector2f offset(static_cast<float>(movement->second->used_function(static_cast<double>(movement->second->current_time / movement->second->movement_time))) * (movement->second->ending_pos.x - movement->second->starting_pos.x) + movement->second->starting_pos.x - movement->second->centroid.x,
									static_cast<float>(movement->second->used_function(static_cast<double>(movement->second->current_time / movement->second->movement_time))) * (movement->second->ending_pos.y - movement->second->starting_pos.y) + movement->second->starting_pos.y - movement->second->centroid.y);

				this->updateMovementInfoVA(movement->first, movement->second, offset);
				++movement;
			}
		}
	}

	for (auto scaling = this->m_Scalings_VA.begin(); scaling != this->m_Scalings_VA.end();) {
		scaling->second->current_time += dt;

		if (scaling->first->getVertexCount() != 0) {
			if (scaling->second->isDone()) {
				if (scaling->second->current_time - dt < scaling->second->scaling_time) {

					scaling->second->currentScale = scaling->second->ending_scale;
					updateScalingInfoVA(scaling->first, scaling->second);
				}

				if (scaling->second->repeat) {
					if (scaling->second->current_time - scaling->second->scaling_time < scaling->second->wait_before_repeating) {
						scaling->second->currentScale = scaling->second->ending_scale;
						updateScalingInfoVA(scaling->first, scaling->second);
					}
						
					else {
						scaling->second->currentScale = scaling->second->starting_scale;
						updateScalingInfoVA(scaling->first, scaling->second);

						scaling->second->current_time -= (scaling->second->scaling_time + scaling->second->wait_before_repeating);
					}
					++scaling;
				}
				else {
					delete scaling->second;
					scaling = m_Scalings_VA.erase(scaling);
				}
			}
			else {
				sf::Vector2f new_scale(scaling->second->used_function(static_cast<double>(scaling->second->current_time / scaling->second->scaling_time)) * (scaling->second->ending_scale.x - scaling->second->starting_scale.x) + scaling->second->starting_scale.x,
								       scaling->second->used_function(static_cast<double>(scaling->second->current_time / scaling->second->scaling_time)) * (scaling->second->ending_scale.y - scaling->second->starting_scale.y) + scaling->second->starting_scale.y);
				scaling->second->currentScale = new_scale;
				updateScalingInfoVA(scaling->first, scaling->second);

				++scaling;
			}
		}
	}

	for (auto rotation = this->m_Rotations_VA.begin(); rotation != this->m_Rotations_VA.end();) {
		rotation->second->current_time += dt;

		if (rotation->first->getVertexCount() != 0) {
			if (rotation->second->isDone()) {
				if (rotation->second->current_time - dt < rotation->second->rotation_time) {
					rotation->second->current_rotation = rotation->second->ending_rotation;
					updateRotationInfoVA(rotation->first, rotation->second);
				}

				if (rotation->second->repeat) {
					if (rotation->second->current_time - rotation->second->rotation_time < rotation->second->wait_before_repeating) {
						rotation->second->current_rotation = rotation->second->ending_rotation;
						updateRotationInfoVA(rotation->first, rotation->second);
					}
						
					else {
						rotation->second->current_rotation = rotation->second->starting_rotation;
						updateRotationInfoVA(rotation->first, rotation->second);

						rotation->second->current_time -= (rotation->second->rotation_time + rotation->second->wait_before_repeating);
					}

					++rotation;
				}
				else {
					delete rotation->second;
					rotation = m_Rotations_VA.erase(rotation);
				}
			}
			else {
				rotation->second->current_rotation = static_cast<float>(rotation->second->used_function(static_cast<double>(rotation->second->current_time / rotation->second->rotation_time))) * (rotation->second->ending_rotation - rotation->second->starting_rotation) + rotation->second->starting_rotation;
				updateRotationInfoVA(rotation->first, rotation->second);

				++rotation;
			}
		}
	}
}

void MovementManager::updateSprite(float dt)
{
	for (auto movement = this->m_Movements_S.begin(); movement != this->m_Movements_S.end();) {
		movement->second->current_time += dt;

		if (movement->second->isDone()) {
			if (movement->second->current_time - dt < movement->second->movement_time)
				movement->first->setPosition(movement->second->ending_pos);

			if (movement->second->repeat) {
				if (movement->second->current_time - movement->second->movement_time >= movement->second->wait_before_repeating){
					movement->first->setPosition(movement->second->starting_pos);

					movement->second->current_time -= (movement->second->movement_time + movement->second->wait_before_repeating);
				}

				++movement;
			}
			else {
				delete movement->second;
				movement = m_Movements_S.erase(movement);
			}
		}
		else {
			sf::Vector2f new_position(static_cast<float>(movement->second->used_function(static_cast<double>(movement->second->current_time / movement->second->movement_time))) * (movement->second->ending_pos.x - movement->second->starting_pos.x) + movement->second->starting_pos.x,
									  static_cast<float>(movement->second->used_function(static_cast<double>(movement->second->current_time / movement->second->movement_time))) * (movement->second->ending_pos.y - movement->second->starting_pos.y) + movement->second->starting_pos.y);

			movement->first->setPosition(new_position);
			++movement;
		}
	}

	for (auto scaling = this->m_Scalings_S.begin(); scaling != this->m_Scalings_S.end();) {
		scaling->second->current_time += dt;

		if (scaling->second->isDone()) {
			if (scaling->second->current_time - dt < scaling->second->scaling_time)
				scaling->first->setScale(scaling->second->ending_scale);

			if (scaling->second->repeat) {
				if (scaling->second->current_time - scaling->second->scaling_time >= scaling->second->wait_before_repeating){
					scaling->first->setScale(scaling->second->starting_scale);

					scaling->second->current_time -= (scaling->second->scaling_time + scaling->second->wait_before_repeating);
				}

				++scaling;
			}
			else {
				delete scaling->second;
				scaling = m_Scalings_S.erase(scaling);
			}
		}
		else {
			sf::Vector2f new_scale(static_cast<float>(scaling->second->used_function(static_cast<double>(scaling->second->current_time / scaling->second->scaling_time))) * (scaling->second->ending_scale.x - scaling->second->starting_scale.x) + scaling->second->starting_scale.x,
								   static_cast<float>(scaling->second->used_function(static_cast<double>(scaling->second->current_time / scaling->second->scaling_time))) * (scaling->second->ending_scale.y - scaling->second->starting_scale.y) + scaling->second->starting_scale.y);

			scaling->first->setScale(new_scale);
			++scaling;
		}
	}

	for (auto rotation = this->m_Rotations_S.begin(); rotation != this->m_Rotations_S.end();) {
		rotation->second->current_time += dt;

		if (rotation->second->isDone()) {
			if (rotation->second->current_time - dt < rotation->second->rotation_time)
				rotation->first->setRotation(rotation->second->ending_rotation);

			if (rotation->second->repeat) {
				if (rotation->second->current_time - rotation->second->rotation_time >= rotation->second->wait_before_repeating){
					rotation->first->setRotation(rotation->second->starting_rotation);

					rotation->second->current_time -= (rotation->second->rotation_time + rotation->second->wait_before_repeating);
				}

				++rotation;
			}
			else {
				delete rotation->second;
				rotation = m_Rotations_S.erase(rotation);
			}
		}
		else {
			float new_rotation = static_cast<float>(rotation->second->used_function(static_cast<double>(rotation->second->current_time / rotation->second->rotation_time))) * (rotation->second->ending_rotation - rotation->second->starting_rotation) + rotation->second->starting_rotation;

			if (!rotation->second->clockwise) {
				new_rotation = 360.f - new_rotation;
				new_rotation = fmod(new_rotation, 360.f);
			}

			rotation->first->setRotation(new_rotation);
			++rotation;
		}
	}
}

// Default constructor
MovementManager::MovementManager()
{
	assert(this->sInstance == nullptr);
	sInstance = this;
}

// Update functions
void MovementManager::update(float dt)
{
	this->updateShape(dt);
	this->updateVertexArray(dt);
	this->updateSprite(dt);
}

// Public functions

// Movement functions
const bool MovementManager::addMovement(sf::Shape* _shape, sf::Vector2f ending_pos, float movement_time, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& movementMap = sInstance->m_Movements_Shape;
	auto movementFound = movementMap.find(_shape);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::CircleShape* object already exists!\n");
		return 0;
	}
	else {
		movementInfo* newMovement = new movementInfo(_shape->getPosition(), ending_pos, movement_time, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating);
		movementMap.insert(std::make_pair(_shape, newMovement));
	}
	return 1;
}

const bool MovementManager::addMovement(sf::Shape* _shape, sf::Vector2f _starting_pos, sf::Vector2f _ending_pos, float _movement_time, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& movementMap = sInstance->m_Movements_Shape;
	auto movementFound = movementMap.find(_shape);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::CircleShape* object already exists!\n");
		return 0;
	}
	else {
		movementInfo* newMovement = new movementInfo(_starting_pos, _ending_pos, _movement_time, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating);
		movementMap.insert(std::make_pair(_shape, newMovement));
	}
	return 1;
}

const bool MovementManager::addMovement(sf::VertexArray* _vertexarray, sf::Vector2f ending_pos, float movement_time, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& movementMap = sInstance->m_Movements_VA;
	auto movementFound = movementMap.find(_vertexarray);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::VertexArray* object already exists!\n");
		return 0;
	}
	else if (_vertexarray->getVertexCount() != 0) {
		movementInfoVA* newMovement = new movementInfoVA(_vertexarray->operator[](0).position, ending_pos, movement_time, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating, _vertexarray);
		movementMap.insert(std::make_pair(_vertexarray, newMovement));
	}
	return 1;
}

const bool MovementManager::addMovement(sf::VertexArray* _vertexarray, sf::Vector2f starting_pos, sf::Vector2f ending_pos, float movement_time, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& movementMap = sInstance->m_Movements_VA;
	auto movementFound = movementMap.find(_vertexarray);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::VertexArray* object already exists!\n");
		return 0;
	}
	else if (_vertexarray->getVertexCount() != 0){
		movementInfoVA* newMovement = new movementInfoVA(starting_pos, ending_pos, movement_time, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating, _vertexarray);
		movementMap.insert(std::make_pair(_vertexarray, newMovement));
	}
	return 1;
}

const bool MovementManager::addMovement(sf::Sprite* _sprite, sf::Vector2f ending_pos, float movement_time, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& movementMap = sInstance->m_Movements_S;
	auto movementFound = movementMap.find(_sprite);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::Sprite* object already exists!\n");
		return 0;
	}
	else {
		movementInfo* newMovement = new movementInfo(_sprite->getPosition(), ending_pos, movement_time, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating);
		movementMap.insert(std::make_pair(_sprite, newMovement));
	}
	return 1;
}

const bool MovementManager::addMovement(sf::Sprite* _sprite, sf::Vector2f starting_pos, sf::Vector2f ending_pos, float movement_time, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& movementMap = sInstance->m_Movements_S;
	auto movementFound = movementMap.find(_sprite);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::Sprite* object already exists!\n");
		return 0;
	}
	else {
		movementInfo* newMovement = new movementInfo(starting_pos, ending_pos, movement_time, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating);
		movementMap.insert(std::make_pair(_sprite, newMovement));
	}
	return 1;
}

void MovementManager::undoMovement()
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

void MovementManager::undoMovement(sf::Shape* _shape)
{
	auto& movementMap = sInstance->m_Movements_Shape;
	auto movementFound = movementMap.find(_shape);

	if (movementFound != movementMap.end()) {
		_shape->setPosition(movementFound->second->starting_pos);
		delete movementFound->second;
		movementMap.erase(movementFound);
	}
}

void MovementManager::undoMovement(sf::VertexArray* _vertexarray)
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

void MovementManager::undoMovement(sf::Sprite* _sprite)
{
	auto& movementMap = sInstance->m_Movements_S;
	auto movementFound = movementMap.find(_sprite);

	if (movementFound != movementMap.end()) {
		_sprite->setPosition(movementFound->second->starting_pos);
		delete movementFound->second;
		movementMap.erase(movementFound);
	}
}

void MovementManager::resetMovement()
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

void MovementManager::resetMovement(sf::Shape* _shape)
{
	auto& movementMap = sInstance->m_Movements_Shape;
	auto movementFound = movementMap.find(_shape);

	if (movementFound != movementMap.end()) {
		_shape->setPosition(movementFound->second->starting_pos);
		movementFound->second->current_time = 0.f;
	}
}

void MovementManager::resetMovement(sf::VertexArray* _vertexarray)
{
	auto& movementMap = sInstance->m_Movements_VA;
	auto movementFound = movementMap.find(_vertexarray);

	if (movementFound != movementMap.end()) {
		sf::Vector2f offset(movementFound->second->centroid - movementFound->second->originalCentroid);
		updateMovementInfoVA(_vertexarray, movementFound->second, -offset);

		movementFound->second->current_time = 0.f;
	}	
}

void MovementManager::resetMovement(sf::Sprite* _sprite)
{
	auto& movementMap = sInstance->m_Movements_S;
	auto movementFound = movementMap.find(_sprite);

	if (movementFound != movementMap.end()) {
		_sprite->setPosition(movementFound->second->starting_pos);
		movementFound->second->current_time = 0.f;
	}
}

void MovementManager::stopMovement()
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

void MovementManager::stopMovement(sf::Shape* _shape)
{
	auto& movementMap = sInstance->m_Movements_Shape;
	auto movementFound = movementMap.find(_shape);

	if (movementFound != movementMap.end()) {
		delete movementFound->second;
		movementMap.erase(movementFound);
	}
}

void MovementManager::stopMovement(sf::VertexArray* _vertexarray)
{
	auto& movementMap = sInstance->m_Movements_VA;
	auto movementFound = movementMap.find(_vertexarray);

	if (movementFound != movementMap.end()) {
		delete movementFound->second;
		movementMap.erase(movementFound);
	}
}

void MovementManager::stopMovement(sf::Sprite* _sprite)
{
	auto& movementMap = sInstance->m_Movements_S;
	auto movementFound = movementMap.find(_sprite);

	if (movementFound != movementMap.end()) {
		delete movementFound->second;
		movementMap.erase(movementFound);
	}
}

// Scaling functions
const bool MovementManager::addScaling(sf::Shape* _shape, sf::Vector2f ending_scale, float scaling_time, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& scalingMap = sInstance->m_Scalings_Shape;
	auto scalingFound = scalingMap.find(_shape);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::CircleShape* object already exists!\n");
		return 0;
	}
	else {
		scalingInfo* newScaling = new scalingInfo(_shape->getScale(), ending_scale, scaling_time, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating);
		scalingMap.insert(std::make_pair(_shape, newScaling));
	}
	return 1;
}

const bool MovementManager::addScaling(sf::Shape* _shape, sf::Vector2f starting_scale, sf::Vector2f ending_scale, float scaling_time, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& scalingMap = sInstance->m_Scalings_Shape;
	auto scalingFound = scalingMap.find(_shape);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::CircleShape* object already exists!\n");
		return 0;
	}
	else {
		scalingInfo* newScaling = new scalingInfo(starting_scale, ending_scale, scaling_time, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating);
		scalingMap.insert(std::make_pair(_shape, newScaling));
	}
	return 1;
}

const bool MovementManager::addScaling(sf::VertexArray* _vertexarray, sf::Vector2f ending_scale, float scaling_time, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& scalingMap = sInstance->m_Scalings_VA;
	auto scalingFound = scalingMap.find(_vertexarray);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::VertexArray* object already exists!\n");
		return 0;
	}
	else if (_vertexarray->getVertexCount() != 0){
		scalingInfoVA* newScaling = new scalingInfoVA({ 1.f, 1.f }, ending_scale, scaling_time, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating, _vertexarray);
		scalingMap.insert(std::make_pair(_vertexarray, newScaling));
	}
	return 1;
}

const bool MovementManager::addScaling(sf::VertexArray* _vertexarray, sf::Vector2f starting_scale, sf::Vector2f ending_scale, float scaling_time, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& scalingMap = sInstance->m_Scalings_VA;
	auto scalingFound = scalingMap.find(_vertexarray);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::VertexArray* object already exists!\n");
		return 0;
	}
	else if (_vertexarray->getVertexCount() != 0) {
		scalingInfoVA* newScaling = new scalingInfoVA(starting_scale, ending_scale, scaling_time, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating, _vertexarray);
		scalingMap.insert(std::make_pair(_vertexarray, newScaling));
	}
	return 1;
}

const bool MovementManager::addScaling(sf::Sprite* _sprite, sf::Vector2f ending_scale, float scaling_time, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& scalingMap = sInstance->m_Scalings_S;
	auto scalingFound = scalingMap.find(_sprite);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::Sprite* object already exists!\n");
		return 0;
	}
	else {
		scalingInfo* newScaling = new scalingInfo(_sprite->getScale(), ending_scale, scaling_time, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating);
		scalingMap.insert(std::make_pair(_sprite, newScaling));
	}
	return 1;
}

const bool MovementManager::addScaling(sf::Sprite* _sprite, sf::Vector2f starting_scale, sf::Vector2f ending_scale, float scaling_time, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& scalingMap = sInstance->m_Scalings_S;
	auto scalingFound = scalingMap.find(_sprite);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::Sprite* object already exists!\n");
		return 0;
	}
	else {
		scalingInfo* newScaling = new scalingInfo(starting_scale, ending_scale, scaling_time, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating);
		scalingMap.insert(std::make_pair(_sprite, newScaling));
	}
	return 1;
}

void MovementManager::undoScaling()
{
	for (auto scaling = this->m_Scalings_Shape.begin(); scaling != this->m_Scalings_Shape.end();) {
		scaling->first->setScale(scaling->second->starting_scale);
		delete scaling->second;
		scaling = m_Scalings_Shape.erase(scaling);
	}

	for (auto scaling = this->m_Scalings_VA.begin(); scaling != this->m_Scalings_VA.end();) {
		scaling->second->currentScale = scaling->second->starting_scale;
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

void MovementManager::undoScaling(sf::Shape* _shape)
{
	auto& scalingMap = sInstance->m_Scalings_Shape;
	auto scalingFound = scalingMap.find(_shape);

	if (scalingFound != scalingMap.end()) {
		_shape->setScale(scalingFound->second->starting_scale);
		delete scalingFound->second;
		scalingMap.erase(scalingFound);
	}
}

void MovementManager::undoScaling(sf::VertexArray* _vertexarray)
{
	auto& scalingMap = sInstance->m_Scalings_VA;
	auto scalingFound = scalingMap.find(_vertexarray);

	if (scalingFound != scalingMap.end()) {
		scalingFound->second->currentScale = scalingFound->second->starting_scale;
		updateScalingInfoVA(_vertexarray, scalingFound->second);

		auto& rotationMap = sInstance->m_Rotations_VA;
		auto rotationFound = rotationMap.find(_vertexarray);

		if (rotationFound != rotationMap.end())
			updateRotationInfoVA(_vertexarray, rotationFound->second);

		delete scalingFound->second;
		scalingMap.erase(scalingFound);
	}
}

void MovementManager::undoScaling(sf::Sprite* _sprite)
{
	auto& scalingMap = sInstance->m_Scalings_S;
	auto scalingFound = scalingMap.find(_sprite);

	if (scalingFound != scalingMap.end()) {
		_sprite->setScale(scalingFound->second->starting_scale);
		delete scalingFound->second;
		scalingMap.erase(scalingFound);
	}
}

void MovementManager::resetScaling()
{
	for (auto scaling = this->m_Scalings_Shape.begin(); scaling != this->m_Scalings_Shape.end();) {
		scaling->first->setScale(scaling->second->starting_scale);
		scaling->second->current_time = 0.f;
		++scaling;
	}

	for (auto scaling = this->m_Scalings_VA.begin(); scaling != this->m_Scalings_VA.end();) {
		scaling->second->currentScale = scaling->second->starting_scale;
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

void MovementManager::resetScaling(sf::Shape* _shape)
{
	auto& scalingMap = sInstance->m_Scalings_Shape;
	auto scalingFound = scalingMap.find(_shape);

	if (scalingFound != scalingMap.end()) {
		_shape->setScale(scalingFound->second->starting_scale);
		scalingFound->second->current_time = 0.f;
	}
}

void MovementManager::resetScaling(sf::VertexArray* _vertexarray)
{
	auto& scalingMap = sInstance->m_Scalings_VA;
	auto scalingFound = scalingMap.find(_vertexarray);

	if (scalingFound != scalingMap.end()) {
		scalingFound->second->currentScale = scalingFound->second->starting_scale;
		updateScalingInfoVA(_vertexarray, scalingFound->second);

		auto& rotationMap = sInstance->m_Rotations_VA;
		auto rotationFound = rotationMap.find(_vertexarray);

		if (rotationFound != rotationMap.end())
			updateRotationInfoVA(_vertexarray, rotationFound->second);

		scalingFound->second->current_time = 0.f;
	}
}

void MovementManager::resetScaling(sf::Sprite* _sprite)
{
	auto& scalingMap = sInstance->m_Scalings_S;
	auto scalingFound = scalingMap.find(_sprite);

	if (scalingFound != scalingMap.end()) {
		_sprite->setScale(scalingFound->second->starting_scale);
		scalingFound->second->current_time = 0.f;
	}
}

void MovementManager::stopScaling()
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

void MovementManager::stopScaling(sf::Shape* _shape)
{
	auto& scalingMap = sInstance->m_Scalings_Shape;
	auto scalingFound = scalingMap.find(_shape);

	if (scalingFound != scalingMap.end()) {
		delete scalingFound->second;
		scalingMap.erase(scalingFound);
	}
}

void MovementManager::stopScaling(sf::VertexArray* _vertexarray)
{
	auto& scalingMap = sInstance->m_Scalings_VA;
	auto scalingFound = scalingMap.find(_vertexarray);

	if (scalingFound != scalingMap.end()) {
		delete scalingFound->second;
		scalingMap.erase(scalingFound);
	}
}

void MovementManager::stopScaling(sf::Sprite* _sprite)
{
	auto& scalingMap = sInstance->m_Scalings_S;
	auto scalingFound = scalingMap.find(_sprite);

	if (scalingFound != scalingMap.end()) {
		delete scalingFound->second;
		scalingMap.erase(scalingFound);
	}
}

// Rotation functions
const bool MovementManager::addRotation(sf::Shape* _shape, float ending_rotation, float rotation_time, movement_type _used_function, bool _clockwise, bool _repeat, float _wait_before_repeating)
{
	auto& rotationMap = sInstance->m_Rotations_Shape;
	auto rotationFound = rotationMap.find(_shape);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::CircleShape* object already exists!\n");
		return 0;
	}
	else {
		rotationInfo* newRotation = new rotationInfo(_shape->getRotation(), ending_rotation, rotation_time, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating, _clockwise);
		rotationMap.insert(std::make_pair(_shape, newRotation));
	}
	return 1;
}

const bool MovementManager::addRotation(sf::Shape* _shape, float starting_rotation, float ending_rotation, float rotation_time, movement_type _used_function, bool _clockwise, bool _repeat, float _wait_before_repeating)
{
	auto& rotationMap = sInstance->m_Rotations_Shape;
	auto rotationFound = rotationMap.find(_shape);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::CircleShape* object already exists!\n");
		return 0;
	}
	else {
		rotationInfo* newRotation = new rotationInfo(starting_rotation, ending_rotation, rotation_time, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating, _clockwise);
		rotationMap.insert(std::make_pair(_shape, newRotation));
	}
	return 1;
}

const bool MovementManager::addRotation(sf::VertexArray* _vertexarray, float ending_rotation, float rotation_time, movement_type _used_function, bool _clockwise, bool _repeat, float _wait_before_repeating)
{
	auto& rotationMap = sInstance->m_Rotations_VA;
	auto rotationFound = rotationMap.find(_vertexarray);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::VertexArray* object already exists!\n");
		return 0;
	}
	else if (_vertexarray->getVertexCount() != 0) {
		rotationInfoVA* newRotation = new rotationInfoVA(0.f, ending_rotation, rotation_time, sInstance->movement_functions[_used_function], _vertexarray, _repeat, _wait_before_repeating, _clockwise);
		rotationMap.insert(std::make_pair(_vertexarray, newRotation));
	}
	return 1;
}

const bool MovementManager::addRotation(sf::VertexArray* _vertexarray, float starting_rotation, float ending_rotation, float rotation_time, movement_type _used_function, bool _clockwise, bool _repeat, float _wait_before_repeating)
{
	auto& rotationMap = sInstance->m_Rotations_VA;
	auto rotationFound = rotationMap.find(_vertexarray);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::VertexArray* object already exists!\n");
		return 0;
	}
	else if (_vertexarray->getVertexCount() != 0) {
		rotationInfoVA* newRotation = new rotationInfoVA(starting_rotation, ending_rotation, rotation_time, sInstance->movement_functions[_used_function], _vertexarray, _repeat, _wait_before_repeating, _clockwise);
		rotationMap.insert(std::make_pair(_vertexarray, newRotation));
	}
	return 1;
}

const bool MovementManager::addRotation(sf::Sprite* _sprite, float ending_rotation, float rotation_time, movement_type _used_function, bool _clockwise, bool _repeat, float _wait_before_repeating)
{
	auto& rotationMap = sInstance->m_Rotations_S;
	auto rotationFound = rotationMap.find(_sprite);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::Sprite* object already exists!\n");
		return 0;
	}
	else {
		rotationInfo* newRotation = new rotationInfo(_sprite->getRotation(), ending_rotation, rotation_time, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating, _clockwise);
		rotationMap.insert(std::make_pair(_sprite, newRotation));
	}
}

const bool MovementManager::addRotation(sf::Sprite* _sprite, float starting_rotation, float ending_rotation, float rotation_time, movement_type _used_function, bool _clockwise, bool _repeat, float _wait_before_repeating)
{
	auto& rotationMap = sInstance->m_Rotations_S;
	auto rotationFound = rotationMap.find(_sprite);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::Sprite* object already exists!\n");
		return 0;
	}
	else {
		rotationInfo* newRotation = new rotationInfo(starting_rotation, ending_rotation, rotation_time, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating, _clockwise);
		rotationMap.insert(std::make_pair(_sprite, newRotation));
	}
	return 1;
}

void MovementManager::undoRotation()
{
	for (auto rotation = this->m_Rotations_Shape.begin(); rotation != this->m_Rotations_Shape.end();) {
		rotation->first->setRotation(rotation->second->starting_rotation);
		delete rotation->second;
		rotation = m_Rotations_Shape.erase(rotation);
	}

	for (auto rotation = this->m_Rotations_VA.begin(); rotation != this->m_Rotations_VA.end();) {
		rotation->second->current_rotation = rotation->second->starting_rotation;
		updateRotationInfoVA(rotation->first, rotation->second);

		delete rotation->second;
		rotation = m_Rotations_VA.erase(rotation);
	}

	for (auto rotation = this->m_Rotations_S.begin(); rotation != this->m_Rotations_S.end();) {
		rotation->first->setRotation(rotation->second->starting_rotation);
		delete rotation->second;
		rotation = m_Rotations_S.erase(rotation);
	}
}

void MovementManager::undoRotation(sf::Shape* _shape)
{
	auto& rotationMap = sInstance->m_Rotations_Shape;
	auto rotationFound = rotationMap.find(_shape);

	if (rotationFound != rotationMap.end()) {
		_shape->setRotation(rotationFound->second->starting_rotation);

		delete rotationFound->second;
		rotationMap.erase(rotationFound);
	}
}

void MovementManager::undoRotation(sf::VertexArray* _vertexarray)
{
	auto& rotationMap = sInstance->m_Rotations_VA;
	auto rotationFound = rotationMap.find(_vertexarray);

	if (rotationFound != rotationMap.end()) {
		rotationFound->second->current_rotation = rotationFound->second->starting_rotation;
		updateRotationInfoVA(_vertexarray, rotationFound->second);

		delete rotationFound->second;
		rotationMap.erase(rotationFound);
	}
}

void MovementManager::undoRotation(sf::Sprite* _sprite)
{
	auto& rotationMap = sInstance->m_Rotations_S;
	auto rotationFound = rotationMap.find(_sprite);

	if (rotationFound != rotationMap.end()) {
		_sprite->setRotation(rotationFound->second->starting_rotation);
		delete rotationFound->second;
		rotationMap.erase(rotationFound);
	}
}

void MovementManager::resetRotation()
{
	for (auto rotation = this->m_Rotations_Shape.begin(); rotation != this->m_Rotations_Shape.end();) {
		rotation->first->setRotation(rotation->second->starting_rotation);
		rotation->second->current_time = 0.f;
		++rotation;
	}

	for (auto rotation = this->m_Rotations_VA.begin(); rotation != this->m_Rotations_VA.end();) {
		rotation->second->current_rotation = rotation->second->starting_rotation;
		updateRotationInfoVA(rotation->first, rotation->second);

		rotation->second->current_time = 0.f;
		++rotation;
	}

	for (auto rotation = this->m_Rotations_S.begin(); rotation != this->m_Rotations_S.end();) {
		rotation->first->setRotation(rotation->second->starting_rotation);
		rotation->second->current_time = 0.f;
		++rotation;
	}
}

void MovementManager::resetRotation(sf::Shape* _shape)
{
	auto& rotationMap = sInstance->m_Rotations_Shape;
	auto rotationFound = rotationMap.find(_shape);

	if (rotationFound != rotationMap.end()) {
		_shape->setRotation(rotationFound->second->starting_rotation);
		rotationFound->second->current_time = 0.f;
	}
}

void MovementManager::resetRotation(sf::VertexArray* _vertexarray)
{
	auto& rotationMap = sInstance->m_Rotations_VA;
	auto rotationFound = rotationMap.find(_vertexarray);

	if (rotationFound != rotationMap.end()) {
		rotationFound->second->current_rotation = rotationFound->second->starting_rotation;
		updateRotationInfoVA(_vertexarray, rotationFound->second);

		rotationFound->second->current_time = 0.f;
	}
}

void MovementManager::resetRotation(sf::Sprite* _sprite)
{
	auto& rotationMap = sInstance->m_Rotations_S;
	auto rotationFound = rotationMap.find(_sprite);

	if (rotationFound != rotationMap.end()) {
		_sprite->setRotation(rotationFound->second->starting_rotation);
		rotationFound->second->current_time = 0.f;
	}
}

void MovementManager::stopRotation()
{
	for (auto rotation = this->m_Rotations_Shape.begin(); rotation != this->m_Rotations_Shape.end();) {
		delete rotation->second;
		rotation = m_Rotations_Shape.erase(rotation);
	}

	for (auto rotation = this->m_Rotations_VA.begin(); rotation != this->m_Rotations_VA.end();) {
		auto& movementMap = sInstance->m_Movements_VA;
		auto movementFound = movementMap.find(rotation->first);

		if (movementFound != movementMap.end()) {
			float sine = static_cast<float>(sin(static_cast<double>(rotation->second->current_rotation) * M_RAD)),
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
			float sine = static_cast<float>(sin(static_cast<double>(rotation->second->current_rotation) * M_RAD)),
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

void MovementManager::stopRotation(sf::Shape* _shape)
{
	auto& rotationMap = sInstance->m_Rotations_Shape;
	auto rotationFound = rotationMap.find(_shape);

	if (rotationFound != rotationMap.end()) {
		delete rotationFound->second;
		rotationMap.erase(rotationFound);
	}
}

void MovementManager::stopRotation(sf::VertexArray* _vertexarray)
{
	auto& rotationMap = sInstance->m_Rotations_VA;
	auto rotationFound = rotationMap.find(_vertexarray);

	if (rotationFound != rotationMap.end()) {
		auto& movementMap = sInstance->m_Movements_VA;
		auto movementFound = movementMap.find(_vertexarray);

		if (movementFound != movementMap.end()) {
			float sine = static_cast<float>(sin(static_cast<double>(rotationFound->second->current_rotation) * M_RAD)),
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
			float sine = static_cast<float>(sin(static_cast<double>(rotationFound->second->current_rotation) * M_RAD)),
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

void MovementManager::stopRotation(sf::Sprite* _sprite)
{
	auto& rotationMap = sInstance->m_Rotations_S;
	auto rotationFound = rotationMap.find(_sprite);

	if (rotationFound != rotationMap.end()) {
		delete rotationFound->second;
		rotationMap.erase(rotationFound);
	}
}
