#include "MovementContainer.h"

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

				++movement;
			}
			else {
				delete movement->second;
				movement = m_Movements_Shape.erase(movement);
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

			++movement;
		}
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

				++scaling;
			}
			else {
				delete scaling->second;
				scaling = m_Scalings_Shape.erase(scaling);
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

			++scaling;
		}
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

				++rotation;
			}
			else {
				delete rotation->second;
				rotation = m_Rotations_Shape.erase(rotation);
			}
		}
		else{
			if (rotation->second->current_time > rotation->second->delay_before)
				rotation->first->setRotation(rotation->second->updateRotation());
			else if (rotation->second->current_time - dt == 0.f)
				rotation->first->setRotation(rotation->second->getStartingRotation());

			++rotation;
		}
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
					++movement;
				}
				else {
					delete movement->second;
					movement = m_Movements_VA.erase(movement);
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

				++movement;
			}
		}
		else
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

					++scaling;
				}
				else {
					delete scaling->second;
					scaling = m_Scalings_VA.erase(scaling);
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

				++scaling;
			}
		}
		else
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

					++rotation;
				}
				else {
					delete rotation->second;
					rotation = m_Rotations_VA.erase(rotation);
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

				++rotation;
			}
		}
		else
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
				if (movement->second->current_time - movement->second->motion_duration - movement->second->delay_before >= movement->second->delay_after){
					movement->first->setPosition(movement->second->starting_pos);

					movement->second->current_time -= movement->second->total_duration;
				}

				++movement;
			}
			else {
				delete movement->second;
				movement = m_Movements_S.erase(movement);
			}
		}
		else {
			if (movement->second->current_time > movement->second->delay_before) {
				sf::Vector2f new_position(static_cast<float>(movement->second->used_function(static_cast<double>(movement->second->current_time / movement->second->motion_duration))) * (movement->second->ending_pos.x - movement->second->starting_pos.x) + movement->second->starting_pos.x,
					static_cast<float>(movement->second->used_function(static_cast<double>(movement->second->current_time / movement->second->motion_duration))) * (movement->second->ending_pos.y - movement->second->starting_pos.y) + movement->second->starting_pos.y);

				movement->first->setPosition(new_position);
			}
			else if (movement->second->current_time - dt == 0.f)
				movement->first->setPosition(movement->second->starting_pos);

			++movement;
		}
	}

	for (auto scaling = this->m_Scalings_S.begin(); scaling != this->m_Scalings_S.end();) {
		scaling->second->current_time += dt;

		if (scaling->second->isDone()) {
			if (scaling->second->current_time - scaling->second->delay_before - dt < scaling->second->motion_duration)
				scaling->first->setScale(scaling->second->ending_scale);

			if (scaling->second->repeat) {
				if (scaling->second->current_time - scaling->second->motion_duration - scaling->second->delay_before >= scaling->second->delay_after){
					scaling->first->setScale(scaling->second->starting_scale);

					scaling->second->current_time -= scaling->second->total_duration;
				}

				++scaling;
			}
			else {
				delete scaling->second;
				scaling = m_Scalings_S.erase(scaling);
			}
		}
		else {
			if (scaling->second->current_time > scaling->second->delay_before) {
				sf::Vector2f new_scale(static_cast<float>(scaling->second->used_function(static_cast<double>(scaling->second->current_time / scaling->second->motion_duration))) * (scaling->second->ending_scale.x - scaling->second->starting_scale.x) + scaling->second->starting_scale.x,
					static_cast<float>(scaling->second->used_function(static_cast<double>(scaling->second->current_time / scaling->second->motion_duration))) * (scaling->second->ending_scale.y - scaling->second->starting_scale.y) + scaling->second->starting_scale.y);

				scaling->first->setScale(new_scale);
			}
			else if (scaling->second->current_time - dt == 0.f)
				scaling->first->setScale(scaling->second->starting_scale);

			++scaling;
		}
	}

	for (auto rotation = this->m_Rotations_S.begin(); rotation != this->m_Rotations_S.end();) {
		rotation->second->current_time += dt;

		if (rotation->second->isDone()) {
			if (rotation->second->current_time - rotation->second->delay_before - dt < rotation->second->motion_duration)
				rotation->first->setRotation(rotation->second->getEndingRotation());

			if (rotation->second->repeat) {
				if (rotation->second->current_time - rotation->second->motion_duration - rotation->second->delay_before >= rotation->second->delay_after){
					rotation->first->setRotation(rotation->second->getStartingRotation());

					rotation->second->current_time -= rotation->second->total_duration;
				}

				++rotation;
			}
			else {
				delete rotation->second;
				rotation = m_Rotations_S.erase(rotation);
			}
		}
		else {
			if (rotation->second->current_time > rotation->second->delay_before)
				rotation->first->setRotation(rotation->second->updateRotation());
			else if (rotation->second->current_time - dt == 0.f)
				rotation->first->setRotation(rotation->second->getStartingRotation());

			++rotation;
		}
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
