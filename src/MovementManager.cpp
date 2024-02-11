#include "MovementManager.h"
#include <assert.h>

// Singleton initialization
MovementManager* MovementManager::sInstance = nullptr;

void MovementManager::updateMovementCentroidOriginalVertex(sf::VertexArray* _vertexarray, movementInfoVA* _movementInfo, sf::Vector2f _offset)
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

void MovementManager::updateScalingCentroidOriginalVertex(sf::VertexArray* _vertexarray, scalingInfoVA* _scalingInfo)
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

void MovementManager::updateRotationCentroidOriginalVertex(sf::VertexArray* _vertexarray, rotationInfoVA* _rotationInfo)
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
	for (auto it = this->m_Movements_Shape.begin(); it != this->m_Movements_Shape.end();) {
		it->second->currentTime += dt;
		if (it->second->isDone()) {
			if (it->second->currentTime - dt < it->second->movementTime)
				it->first->setPosition(it->second->endingPos);

			if (it->second->repeat) {
				if (it->second->currentTime - it->second->movementTime >= it->second->wait_before_repeating) {
					it->first->setPosition(it->second->startingPos);

					it->second->currentTime -= (it->second->movementTime + it->second->wait_before_repeating);
					it->second->repeat_timer = 0.f;
				}
				++it;
			}
			else {
				delete it->second;
				it = m_Movements_Shape.erase(it);
			}
		}
		else {
			sf::Vector2f new_position(static_cast<float>(it->second->used_function(static_cast<double>(it->second->currentTime / it->second->movementTime))) * (it->second->endingPos.x - it->second->startingPos.x) + it->second->startingPos.x,
									  static_cast<float>(it->second->used_function(static_cast<double>(it->second->currentTime / it->second->movementTime))) * (it->second->endingPos.y - it->second->startingPos.y) + it->second->startingPos.y);

			it->first->setPosition(new_position);
			++it;
		}
	}

	for (auto it = this->m_Scalings_Shape.begin(); it != this->m_Scalings_Shape.end();) {
		it->second->currentTime += dt;
		if (it->second->isDone()) {
			if (it->second->currentTime - dt < it->second->scalingTime)
				it->first->setScale(it->second->endingScale);

			if (it->second->repeat) {
				if (it->second->currentTime - it->second->scalingTime >= it->second->wait_before_repeating) {
					it->first->setScale(it->second->startingScale);
					
					it->second->currentTime -= (it->second->scalingTime + it->second->wait_before_repeating);
					it->second->repeat_timer = 0.f;
					
				}
				++it;
			}
			else {
				delete it->second;
				it = m_Scalings_Shape.erase(it);
			}
		}
		else {
			sf::Vector2f new_scale(static_cast<float>(it->second->used_function(static_cast<double>(it->second->currentTime / it->second->scalingTime))) * (it->second->endingScale.x - it->second->startingScale.x) + it->second->startingScale.x,
								   static_cast<float>(it->second->used_function(static_cast<double>(it->second->currentTime / it->second->scalingTime))) * (it->second->endingScale.y - it->second->startingScale.y) + it->second->startingScale.y);

			it->first->setScale(new_scale);
			++it;
		}
	}

	for (auto it = this->m_Rotations_Shape.begin(); it != this->m_Rotations_Shape.end();) {
		it->second->currentTime += dt;
		if (it->second->isDone()) {
			if (it->second->currentTime - dt < it->second->rotationTime)
				it->first->setRotation(it->second->endingRotation);

			if (it->second->repeat) {
				if (it->second->currentTime - it->second->rotationTime >= it->second->wait_before_repeating) {
					it->first->setRotation(it->second->startingRotation);

					it->second->currentTime -= (it->second->rotationTime + it->second->wait_before_repeating);
					it->second->repeat_timer = 0.f;
				}
				++it;
			}
			else {
				delete it->second;
				it = m_Rotations_Shape.erase(it);
			}
		}
		else {
			if (it->second->clockwise) {
				float new_rotation = static_cast<float>(it->second->used_function(static_cast<double>(it->second->currentTime / it->second->rotationTime))) * (it->second->endingRotation - it->second->startingRotation) + it->second->startingRotation;
				it->first->setRotation(new_rotation);
			}
			++it;

			//TODO: Dodaæ opcjê dla rotacji przeciwnie do ruchu wskazówek zegara
		}
	}
}

void MovementManager::updateVertexArray(float dt)
{
	for (auto it = this->m_Movements_VA.begin(); it != this->m_Movements_VA.end();) {
		it->second->currentTime += dt;
		if (it->first->getVertexCount() != 0) {
			if (it->second->isDone()) {
				if (it->second->currentTime - dt < it->second->movementTime) {
					sf::Vector2f offset(static_cast<float>(it->second->used_function(1.0)) * (it->second->endingPos.x - it->second->startingPos.x) + it->second->startingPos.x - it->second->centroid.x,
										static_cast<float>(it->second->used_function(1.0)) * (it->second->endingPos.y - it->second->startingPos.y) + it->second->startingPos.y - it->second->centroid.y);

					this->updateMovementCentroidOriginalVertex(it->first, it->second, offset);
				}

				if (it->second->repeat) {
					if (it->second->currentTime - it->second->movementTime >= it->second->wait_before_repeating){
						sf::Vector2f offset(it->second->endingPos - it->second->startingPos);

						this->updateMovementCentroidOriginalVertex(it->first, it->second, -offset);

						it->second->currentTime -= (it->second->movementTime + it->second->wait_before_repeating);
						it->second->repeat_timer = 0.f;
					}
					++it;
				}
				else {
					delete it->second;
					it = m_Movements_VA.erase(it);
				}
			}
			else {
				sf::Vector2f offset(static_cast<float>(it->second->used_function(static_cast<double>(it->second->currentTime / it->second->movementTime))) * (it->second->endingPos.x - it->second->startingPos.x) + it->second->startingPos.x - it->second->centroid.x,
									static_cast<float>(it->second->used_function(static_cast<double>(it->second->currentTime / it->second->movementTime))) * (it->second->endingPos.y - it->second->startingPos.y) + it->second->startingPos.y - it->second->centroid.y);

				this->updateMovementCentroidOriginalVertex(it->first, it->second, offset);
				++it;
			}
		}
	}

	for (auto it = this->m_Scalings_VA.begin(); it != this->m_Scalings_VA.end();) {
		it->second->currentTime += dt;
		if (it->first->getVertexCount() != 0) {
			if (it->second->isDone()) {
				if (it->second->currentTime - dt < it->second->scalingTime) {

					it->second->currentScale = it->second->endingScale;
					updateScalingCentroidOriginalVertex(it->first, it->second);
				}

				if (it->second->repeat) {
					if (it->second->currentTime - it->second->scalingTime < it->second->wait_before_repeating) {
						it->second->currentScale = it->second->endingScale;
						updateScalingCentroidOriginalVertex(it->first, it->second);
					}
						
					else {
						it->second->currentScale = it->second->startingScale;
						updateScalingCentroidOriginalVertex(it->first, it->second);

						it->second->currentTime -= (it->second->scalingTime + it->second->wait_before_repeating);
						it->second->repeat_timer = 0.f;
					}
					++it;
				}
				else {
					delete it->second;
					it = m_Scalings_VA.erase(it);
				}
			}
			else {
				sf::Vector2f new_scale(it->second->used_function(static_cast<double>(it->second->currentTime / it->second->scalingTime)) * (it->second->endingScale.x - it->second->startingScale.x) + it->second->startingScale.x,
								       it->second->used_function(static_cast<double>(it->second->currentTime / it->second->scalingTime)) * (it->second->endingScale.y - it->second->startingScale.y) + it->second->startingScale.y);
				it->second->currentScale = new_scale;
				updateScalingCentroidOriginalVertex(it->first, it->second);

				++it;
			}
		}
	}

	for (auto it = this->m_Rotations_VA.begin(); it != this->m_Rotations_VA.end();) {
		it->second->currentTime += dt;
		if (it->first->getVertexCount() != 0) {
			if (it->second->isDone()) {
				if (it->second->currentTime - dt < it->second->rotationTime) {
					it->second->current_rotation = it->second->endingRotation;
					updateRotationCentroidOriginalVertex(it->first, it->second);
				}

				if (it->second->repeat) {
					if (it->second->currentTime - it->second->rotationTime < it->second->wait_before_repeating) {
						it->second->current_rotation = it->second->endingRotation;
						updateRotationCentroidOriginalVertex(it->first, it->second);
					}
						
					else {
						it->second->current_rotation = it->second->startingRotation;
						updateRotationCentroidOriginalVertex(it->first, it->second);

						it->second->currentTime -= (it->second->rotationTime + it->second->wait_before_repeating);
						it->second->repeat_timer = 0.f;
					}

					++it;
				}
				else {
					delete it->second;
					it = m_Rotations_VA.erase(it);
				}
			}
			else {
				it->second->current_rotation = static_cast<float>(it->second->used_function(static_cast<double>(it->second->currentTime / it->second->rotationTime))) * (it->second->endingRotation - it->second->startingRotation) + it->second->startingRotation;
				updateRotationCentroidOriginalVertex(it->first, it->second);

				++it;
			}
		}
	}
}
//TODO: Naprawiæ sprite i shape pod wzglêdem 
// if (!it->second->repeat || (it->second->repeat_timer == 0.f && it->second->wait_before_repeating != 0.f)) {}
// 
// if (it->second->currentTime - it->second->rotationTime < it->second->wait_before_repeating) {}
// 
// it->second->currentTime -= (it->second->rotationTime + it->second->wait_before_repeating);
// 

//TODO: Naprawiæ b³êdn¹ pozycjê VA po resecie animacji (nie wraca do pozycji pocz¹tkowej)

void MovementManager::updateSprite(float dt)
{
	for (auto it = this->m_Movements_S.begin(); it != this->m_Movements_S.end();) {
		it->second->currentTime += dt;
		if (it->second->isDone()) {
			if (it->second->currentTime - dt < it->second->movementTime)
				it->first->setPosition(it->second->endingPos);

			if (it->second->repeat) {
				if (it->second->currentTime - it->second->movementTime >= it->second->wait_before_repeating){
					it->first->setPosition(it->second->startingPos);

					it->second->currentTime -= (it->second->movementTime + it->second->wait_before_repeating);
					it->second->repeat_timer = 0.f;
				}

				++it;
			}
			else {
				delete it->second;
				it = m_Movements_S.erase(it);
			}
		}
		else {
			sf::Vector2f new_position(static_cast<float>(it->second->used_function(static_cast<double>(it->second->currentTime / it->second->movementTime))) * (it->second->endingPos.x - it->second->startingPos.x) + it->second->startingPos.x,
									  static_cast<float>(it->second->used_function(static_cast<double>(it->second->currentTime / it->second->movementTime))) * (it->second->endingPos.y - it->second->startingPos.y) + it->second->startingPos.y);

			it->first->setPosition(new_position);
			++it;
		}
	}

	for (auto it = this->m_Scalings_S.begin(); it != this->m_Scalings_S.end();) {
		it->second->currentTime += dt;
		if (it->second->isDone()) {
			if (it->second->currentTime - dt < it->second->scalingTime)
				it->first->setScale(it->second->endingScale);

			if (it->second->repeat) {
				if (it->second->currentTime - it->second->scalingTime >= it->second->wait_before_repeating){
					it->first->setScale(it->second->startingScale);

					it->second->currentTime -= (it->second->scalingTime + it->second->wait_before_repeating);
					it->second->repeat_timer = 0.f;
				}

				++it;
			}
			else {
				delete it->second;
				it = m_Scalings_S.erase(it);
			}
		}
		else {
			sf::Vector2f new_scale(static_cast<float>(it->second->used_function(static_cast<double>(it->second->currentTime / it->second->scalingTime))) * (it->second->endingScale.x - it->second->startingScale.x) + it->second->startingScale.x,
								   static_cast<float>(it->second->used_function(static_cast<double>(it->second->currentTime / it->second->scalingTime))) * (it->second->endingScale.y - it->second->startingScale.y) + it->second->startingScale.y);

			it->first->setScale(new_scale);
			++it;
		}
	}

	for (auto it = this->m_Rotations_S.begin(); it != this->m_Rotations_S.end();) {
		it->second->currentTime += dt;
		if (it->second->isDone()) {
			if (it->second->currentTime - dt < it->second->rotationTime)
				it->first->setRotation(it->second->endingRotation);

			if (it->second->repeat) {
				if (it->second->currentTime - it->second->rotationTime >= it->second->wait_before_repeating){
					it->first->setRotation(it->second->startingRotation);

					it->second->currentTime -= (it->second->rotationTime + it->second->wait_before_repeating);
					it->second->repeat_timer = 0.f;
				}

				++it;
			}
			else {
				delete it->second;
				it = m_Rotations_S.erase(it);
			}
		}
		else {
			if (it->second->clockwise) {
				float rotation = static_cast<float>(it->second->used_function(static_cast<double>(it->second->currentTime / it->second->rotationTime))) * (it->second->endingRotation - it->second->startingRotation) + it->second->startingRotation;
				it->first->setRotation(rotation);
			}
			//TODO: Dodaæ opcjê dla rotacji przeciwnie do ruchu wskazówek zegara
			++it;
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
const bool MovementManager::addMovement(sf::Shape* _shape, sf::Vector2f endingPos, float movementTime, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& movementMap = sInstance->m_Movements_Shape;
	auto movementFound = movementMap.find(_shape);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::CircleShape* object already exists!\n");
		return 0;
	}
	else {
		movementInfo* newMovement = new movementInfo(_shape->getPosition(), endingPos, movementTime, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating);
		movementMap.insert(std::make_pair(_shape, newMovement));
	}
	return 1;
}

const bool MovementManager::addMovement(sf::Shape* _shape, sf::Vector2f _startingPos, sf::Vector2f _endingPos, float _movementTime, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& movementMap = sInstance->m_Movements_Shape;
	auto movementFound = movementMap.find(_shape);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::CircleShape* object already exists!\n");
		return 0;
	}
	else {
		movementInfo* newMovement = new movementInfo(_startingPos, _endingPos, _movementTime, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating);
		movementMap.insert(std::make_pair(_shape, newMovement));
	}
	return 1;
}

const bool MovementManager::addMovement(sf::VertexArray* _vertexarray, sf::Vector2f endingPos, float movementTime, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& movementMap = sInstance->m_Movements_VA;
	auto movementFound = movementMap.find(_vertexarray);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::VertexArray* object already exists!\n");
		return 0;
	}
	else if (_vertexarray->getVertexCount() != 0) {
		movementInfoVA* newMovement = new movementInfoVA(_vertexarray->operator[](0).position, endingPos, movementTime, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating, _vertexarray);
		movementMap.insert(std::make_pair(_vertexarray, newMovement));
	}
	return 1;
}

const bool MovementManager::addMovement(sf::VertexArray* _vertexarray, sf::Vector2f startingPos, sf::Vector2f endingPos, float movementTime, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& movementMap = sInstance->m_Movements_VA;
	auto movementFound = movementMap.find(_vertexarray);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::VertexArray* object already exists!\n");
		return 0;
	}
	else if (_vertexarray->getVertexCount() != 0){
		movementInfoVA* newMovement = new movementInfoVA(startingPos, endingPos, movementTime, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating, _vertexarray);
		movementMap.insert(std::make_pair(_vertexarray, newMovement));
	}
	return 1;
}

const bool MovementManager::addMovement(sf::Sprite* _sprite, sf::Vector2f endingPos, float movementTime, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& movementMap = sInstance->m_Movements_S;
	auto movementFound = movementMap.find(_sprite);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::Sprite* object already exists!\n");
		return 0;
	}
	else {
		movementInfo* newMovement = new movementInfo(_sprite->getPosition(), endingPos, movementTime, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating);
		movementMap.insert(std::make_pair(_sprite, newMovement));
	}
	return 1;
}

const bool MovementManager::addMovement(sf::Sprite* _sprite, sf::Vector2f startingPos, sf::Vector2f endingPos, float movementTime, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& movementMap = sInstance->m_Movements_S;
	auto movementFound = movementMap.find(_sprite);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::Sprite* object already exists!\n");
		return 0;
	}
	else {
		movementInfo* newMovement = new movementInfo(startingPos, endingPos, movementTime, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating);
		movementMap.insert(std::make_pair(_sprite, newMovement));
	}
	return 1;
}

void MovementManager::undoMovement()
{
	for (auto it = this->m_Movements_Shape.begin(); it != this->m_Movements_Shape.end();) {
		it->first->setPosition(it->second->startingPos);
		delete it->second;
		it = m_Movements_Shape.erase(it);
	}

	for (auto it = this->m_Movements_VA.begin(); it != this->m_Movements_VA.end();) {
		if (it->first->getVertexCount() != 0) {
			sf::Vector2f offset = it->first->operator[](0).position - it->second->startingPos;

			for (size_t i = 0; i < it->first->getVertexCount(); i++)
				it->first->operator[](i).position -= offset;

			delete it->second;
			it = m_Movements_VA.erase(it);
		}
	}

	for (auto it = this->m_Movements_S.begin(); it != this->m_Movements_S.end();) {
		it->first->setPosition(it->second->startingPos);
		delete it->second;
		it = m_Movements_S.erase(it);
	}
}

void MovementManager::undoMovement(sf::Shape* _shape)
{
	auto& movementMap = sInstance->m_Movements_Shape;
	auto movementFound = movementMap.find(_shape);

	if (movementFound != movementMap.end()) {
		_shape->setPosition(movementFound->second->startingPos);
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

		updateMovementCentroidOriginalVertex(_vertexarray, movementFound->second, -offset);

		delete movementFound->second;
		movementMap.erase(movementFound);
	}
}

void MovementManager::undoMovement(sf::Sprite* _sprite)
{
	auto& movementMap = sInstance->m_Movements_S;
	auto movementFound = movementMap.find(_sprite);

	if (movementFound != movementMap.end()) {
		_sprite->setPosition(movementFound->second->startingPos);
		delete movementFound->second;
		movementMap.erase(movementFound);
	}
}

void MovementManager::resetMovement()
{
	for (auto it = this->m_Movements_Shape.begin(); it != this->m_Movements_Shape.end();) {
		it->first->setPosition(it->second->startingPos);
		it->second->currentTime = 0.f;
		it->second->repeat_timer = 0.f;
		++it;
	}

	for (auto it = this->m_Movements_VA.begin(); it != this->m_Movements_VA.end();) {
		if (it->first->getVertexCount() != 0) {
			sf::Vector2f offset = it->first->operator[](0).position - it->second->startingPos;

			for (size_t i = 0; i < it->first->getVertexCount(); i++)
				it->first->operator[](i).position -= offset;
		}
		it->second->currentTime = 0.f;
		it->second->repeat_timer = 0.f;
		++it;
	}

	for (auto it = this->m_Movements_S.begin(); it != this->m_Movements_S.end();) {
		it->first->setPosition(it->second->startingPos);
		it->second->currentTime = 0.f;
		it->second->repeat_timer = 0.f;
		++it;
	}
}

void MovementManager::resetMovement(sf::Shape* _shape)
{
	auto& movementMap = sInstance->m_Movements_Shape;
	auto movementFound = movementMap.find(_shape);

	if (movementFound != movementMap.end()) {
		_shape->setPosition(movementFound->second->startingPos);
		movementFound->second->currentTime = 0.f;
		movementFound->second->repeat_timer = 0.f;
	}
}

void MovementManager::resetMovement(sf::VertexArray* _vertexarray)
{
	auto& movementMap = sInstance->m_Movements_VA;
	auto movementFound = movementMap.find(_vertexarray);

	if (movementFound != movementMap.end()) 
		movementFound->second->currentTime = 0.f;
}

void MovementManager::resetMovement(sf::Sprite* _sprite)
{
	auto& movementMap = sInstance->m_Movements_S;
	auto movementFound = movementMap.find(_sprite);

	if (movementFound != movementMap.end()) {
		_sprite->setPosition(movementFound->second->startingPos);
		movementFound->second->currentTime = 0.f;
		movementFound->second->repeat_timer = 0.f;
	}
}

void MovementManager::stopMovement()
{
	for (auto it = this->m_Movements_Shape.begin(); it != this->m_Movements_Shape.end();) {
		delete it->second;
		it = m_Movements_Shape.erase(it);
	}

	for (auto it = this->m_Movements_VA.begin(); it != this->m_Movements_VA.end();) {
		delete it->second;
		it = m_Movements_VA.erase(it);
	}

	for (auto it = this->m_Movements_S.begin(); it != this->m_Movements_S.end();) {
		delete it->second;
		it = m_Movements_S.erase(it);
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
const bool MovementManager::addScaling(sf::Shape* _shape, sf::Vector2f endingScale, float scalingTime, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& scalingMap = sInstance->m_Scalings_Shape;
	auto scalingFound = scalingMap.find(_shape);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::CircleShape* object already exists!\n");
		return 0;
	}
	else {
		scalingInfo* newScaling = new scalingInfo(_shape->getScale(), endingScale, scalingTime, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating);
		scalingMap.insert(std::make_pair(_shape, newScaling));
	}
	return 1;
}

const bool MovementManager::addScaling(sf::Shape* _shape, sf::Vector2f startingScale, sf::Vector2f endingScale, float scalingTime, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& scalingMap = sInstance->m_Scalings_Shape;
	auto scalingFound = scalingMap.find(_shape);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::CircleShape* object already exists!\n");
		return 0;
	}
	else {
		scalingInfo* newScaling = new scalingInfo(startingScale, endingScale, scalingTime, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating);
		scalingMap.insert(std::make_pair(_shape, newScaling));
	}
	return 1;
}

const bool MovementManager::addScaling(sf::VertexArray* _vertexarray, sf::Vector2f endingScale, float scalingTime, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& scalingMap = sInstance->m_Scalings_VA;
	auto scalingFound = scalingMap.find(_vertexarray);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::VertexArray* object already exists!\n");
		return 0;
	}
	else if (_vertexarray->getVertexCount() != 0){
		scalingInfoVA* newScaling = new scalingInfoVA({ 1.f, 1.f }, endingScale, scalingTime, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating, _vertexarray);
		scalingMap.insert(std::make_pair(_vertexarray, newScaling));
	}
	return 1;
}

const bool MovementManager::addScaling(sf::VertexArray* _vertexarray, sf::Vector2f startingScale, sf::Vector2f endingScale, float scalingTime, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& scalingMap = sInstance->m_Scalings_VA;
	auto scalingFound = scalingMap.find(_vertexarray);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::VertexArray* object already exists!\n");
		return 0;
	}
	else if (_vertexarray->getVertexCount() != 0) {
		scalingInfoVA* newScaling = new scalingInfoVA(startingScale, endingScale, scalingTime, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating, _vertexarray);
		scalingMap.insert(std::make_pair(_vertexarray, newScaling));
	}
	return 1;
}

const bool MovementManager::addScaling(sf::Sprite* _sprite, sf::Vector2f endingScale, float scalingTime, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& scalingMap = sInstance->m_Scalings_S;
	auto scalingFound = scalingMap.find(_sprite);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::Sprite* object already exists!\n");
		return 0;
	}
	else {
		scalingInfo* newScaling = new scalingInfo(_sprite->getScale(), endingScale, scalingTime, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating);
		scalingMap.insert(std::make_pair(_sprite, newScaling));
	}
	return 1;
}

const bool MovementManager::addScaling(sf::Sprite* _sprite, sf::Vector2f startingScale, sf::Vector2f endingScale, float scalingTime, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& scalingMap = sInstance->m_Scalings_S;
	auto scalingFound = scalingMap.find(_sprite);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::Sprite* object already exists!\n");
		return 0;
	}
	else {
		scalingInfo* newScaling = new scalingInfo(startingScale, endingScale, scalingTime, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating);
		scalingMap.insert(std::make_pair(_sprite, newScaling));
	}
	return 1;
}

void MovementManager::undoScaling()
{
	for (auto it = this->m_Scalings_Shape.begin(); it != this->m_Scalings_Shape.end();) {
		it->first->setScale(it->second->startingScale);
		delete it->second;
		it = m_Scalings_Shape.erase(it);
	}

	for (auto it = this->m_Scalings_VA.begin(); it != this->m_Scalings_VA.end();) {
		it->first->operator= (it->second->originalVertex);
		delete it->second;
		it = m_Scalings_VA.erase(it);
	}

	for (auto it = this->m_Scalings_S.begin(); it != this->m_Scalings_S.end();) {
		it->first->setScale(it->second->startingScale);
		delete it->second;
		it = m_Scalings_S.erase(it);
	}
}

void MovementManager::undoScaling(sf::Shape* _shape)
{
	auto& scalingMap = sInstance->m_Scalings_Shape;
	auto scalingFound = scalingMap.find(_shape);

	if (scalingFound != scalingMap.end()) {
		_shape->setScale(scalingFound->second->startingScale);
		delete scalingFound->second;
		scalingMap.erase(scalingFound);
	}
}

void MovementManager::undoScaling(sf::VertexArray* _vertexarray)
{
	auto& scalingMap = sInstance->m_Scalings_VA;
	auto scalingFound = scalingMap.find(_vertexarray);

	if (scalingFound != scalingMap.end()) {
		_vertexarray->operator= (scalingFound->second->originalVertex);
		delete scalingFound->second;
		scalingMap.erase(scalingFound);
	}
}

void MovementManager::undoScaling(sf::Sprite* _sprite)
{
	auto& scalingMap = sInstance->m_Scalings_S;
	auto scalingFound = scalingMap.find(_sprite);

	if (scalingFound != scalingMap.end()) {
		_sprite->setScale(scalingFound->second->startingScale);
		delete scalingFound->second;
		scalingMap.erase(scalingFound);
	}
}

void MovementManager::resetScaling()
{
	for (auto it = this->m_Scalings_Shape.begin(); it != this->m_Scalings_Shape.end();) {
		it->first->setScale(it->second->startingScale);
		it->second->currentTime = 0.f;
		it->second->repeat_timer = 0.f;
		++it;
	}

	for (auto it = this->m_Scalings_VA.begin(); it != this->m_Scalings_VA.end();) {
		it->first->operator= (it->second->originalVertex);
		it->second->currentTime = 0.f;
		it->second->repeat_timer = 0.f;
		++it;
	}

	for (auto it = this->m_Scalings_S.begin(); it != this->m_Scalings_S.end();) {
		it->first->setScale(it->second->startingScale);
		it->second->currentTime = 0.f;
		it->second->repeat_timer = 0.f;
		++it;
	}
}

void MovementManager::resetScaling(sf::Shape* _shape)
{
	auto& scalingMap = sInstance->m_Scalings_Shape;
	auto scalingFound = scalingMap.find(_shape);

	if (scalingFound != scalingMap.end()) {
		_shape->setScale(scalingFound->second->startingScale);
		scalingFound->second->currentTime = 0.f;
		scalingFound->second->repeat_timer = 0.f;
	}
}

void MovementManager::resetScaling(sf::VertexArray* _vertexarray)
{
	auto& scalingMap = sInstance->m_Scalings_VA;
	auto scalingFound = scalingMap.find(_vertexarray);

	if (scalingFound != scalingMap.end()) {
		_vertexarray->operator= (scalingFound->second->originalVertex);
		scalingFound->second->currentTime = 0.f;
		scalingFound->second->repeat_timer = 0.f;
	}
}

void MovementManager::resetScaling(sf::Sprite* _sprite)
{
	auto& scalingMap = sInstance->m_Scalings_S;
	auto scalingFound = scalingMap.find(_sprite);

	if (scalingFound != scalingMap.end()) {
		_sprite->setScale(scalingFound->second->startingScale);
		scalingFound->second->currentTime = 0.f;
		scalingFound->second->repeat_timer = 0.f;
	}
}

void MovementManager::stopScaling()
{
	for (auto it = this->m_Scalings_Shape.begin(); it != this->m_Scalings_Shape.end();) {
		delete it->second;
		it = this->m_Scalings_Shape.erase(it);
	}

	for (auto it = this->m_Scalings_VA.begin(); it != this->m_Scalings_VA.end();) {
		delete it->second;
		it = m_Scalings_VA.erase(it);
	}

	for (auto it = this->m_Scalings_S.begin(); it != this->m_Scalings_S.end();) {
		delete it->second;
		it = m_Scalings_S.erase(it);
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
const bool MovementManager::addRotation(sf::Shape* _shape, float endingRotation, float rotationTime, movement_type _used_function, bool _clockwise, bool _repeat, float _wait_before_repeating)
{
	auto& rotationMap = sInstance->m_Rotations_Shape;
	auto rotationFound = rotationMap.find(_shape);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::CircleShape* object already exists!\n");
		return 0;
	}
	else {
		rotationInfo* newRotation = new rotationInfo(_shape->getRotation(), endingRotation, rotationTime, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating, _clockwise);
		rotationMap.insert(std::make_pair(_shape, newRotation));
	}
	return 1;
}

const bool MovementManager::addRotation(sf::Shape* _shape, float startingRotation, float endingRotation, float rotationTime, movement_type _used_function, bool _clockwise, bool _repeat, float _wait_before_repeating)
{
	auto& rotationMap = sInstance->m_Rotations_Shape;
	auto rotationFound = rotationMap.find(_shape);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::CircleShape* object already exists!\n");
		return 0;
	}
	else {
		rotationInfo* newRotation = new rotationInfo(startingRotation, endingRotation, rotationTime, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating, _clockwise);
		rotationMap.insert(std::make_pair(_shape, newRotation));
	}
	return 1;
}

const bool MovementManager::addRotation(sf::VertexArray* _vertexarray, float endingRotation, float rotationTime, movement_type _used_function, bool _clockwise, bool _repeat, float _wait_before_repeating)
{
	auto& rotationMap = sInstance->m_Rotations_VA;
	auto rotationFound = rotationMap.find(_vertexarray);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::VertexArray* object already exists!\n");
		return 0;
	}
	else if (_vertexarray->getVertexCount() != 0) {
		rotationInfoVA* newRotation = new rotationInfoVA(0.f, endingRotation, rotationTime, sInstance->movement_functions[_used_function], _vertexarray, _repeat, _wait_before_repeating, _clockwise);
		rotationMap.insert(std::make_pair(_vertexarray, newRotation));
	}
	return 1;
}

const bool MovementManager::addRotation(sf::VertexArray* _vertexarray, float startingRotation, float endingRotation, float rotationTime, movement_type _used_function, bool _clockwise, bool _repeat, float _wait_before_repeating)
{
	auto& rotationMap = sInstance->m_Rotations_VA;
	auto rotationFound = rotationMap.find(_vertexarray);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::VertexArray* object already exists!\n");
		return 0;
	}
	else if (_vertexarray->getVertexCount() != 0) {
		rotationInfoVA* newRotation = new rotationInfoVA(startingRotation, endingRotation, rotationTime, sInstance->movement_functions[_used_function], _vertexarray, _repeat, _wait_before_repeating, _clockwise);
		rotationMap.insert(std::make_pair(_vertexarray, newRotation));
	}
	return 1;
}

const bool MovementManager::addRotation(sf::Sprite* _sprite, float endingRotation, float rotationTime, movement_type _used_function, bool _clockwise, bool _repeat, float _wait_before_repeating)
{
	auto& rotationMap = sInstance->m_Rotations_S;
	auto rotationFound = rotationMap.find(_sprite);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::Sprite* object already exists!\n");
		return 0;
	}
	else {
		rotationInfo* newRotation = new rotationInfo(_sprite->getRotation(), endingRotation, rotationTime, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating, _clockwise);
		rotationMap.insert(std::make_pair(_sprite, newRotation));
	}
}

const bool MovementManager::addRotation(sf::Sprite* _sprite, float startingRotation, float endingRotation, float rotationTime, movement_type _used_function, bool _clockwise, bool _repeat, float _wait_before_repeating)
{
	auto& rotationMap = sInstance->m_Rotations_S;
	auto rotationFound = rotationMap.find(_sprite);

	if (rotationFound != rotationMap.end()) {
		printf("ERROR: Rotation for sf::Sprite* object already exists!\n");
		return 0;
	}
	else {
		rotationInfo* newRotation = new rotationInfo(startingRotation, endingRotation, rotationTime, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating, _clockwise);
		rotationMap.insert(std::make_pair(_sprite, newRotation));
	}
	return 1;
}

void MovementManager::undoRotation()
{
	for (auto it = this->m_Rotations_Shape.begin(); it != this->m_Rotations_Shape.end();) {
		it->first->setRotation(it->second->startingRotation);
		delete it->second;
		it = m_Rotations_Shape.erase(it);
	}

	for (auto it = this->m_Rotations_VA.begin(); it != this->m_Rotations_VA.end();) {
		it->first->operator= (it->second->originalVertex);
		delete it->second;
		it = m_Rotations_VA.erase(it);
	}

	for (auto it = this->m_Rotations_S.begin(); it != this->m_Rotations_S.end();) {
		it->first->setRotation(it->second->startingRotation);
		delete it->second;
		it = m_Rotations_S.erase(it);
	}
}

void MovementManager::undoRotation(sf::Shape* _shape)
{
	auto& rotationMap = sInstance->m_Rotations_Shape;
	auto rotationFound = rotationMap.find(_shape);

	if (rotationFound != rotationMap.end()) {
		_shape->setRotation(rotationFound->second->startingRotation);
		delete rotationFound->second;
		rotationMap.erase(rotationFound);
	}
}

void MovementManager::undoRotation(sf::VertexArray* _vertexarray)
{
	auto& rotationMap = sInstance->m_Rotations_VA;
	auto rotationFound = rotationMap.find(_vertexarray);

	if (rotationFound != rotationMap.end()) {
		_vertexarray->operator= (rotationFound->second->originalVertex);
		delete rotationFound->second;
		rotationMap.erase(rotationFound);
	}
}

void MovementManager::undoRotation(sf::Sprite* _sprite)
{
	auto& rotationMap = sInstance->m_Rotations_S;
	auto rotationFound = rotationMap.find(_sprite);

	if (rotationFound != rotationMap.end()) {
		_sprite->setRotation(rotationFound->second->startingRotation);
		delete rotationFound->second;
		rotationMap.erase(rotationFound);
	}
}

void MovementManager::resetRotation()
{
	for (auto it = this->m_Rotations_Shape.begin(); it != this->m_Rotations_Shape.end();) {
		it->first->setRotation(it->second->startingRotation);
		it->second->currentTime = 0.f;
		it->second->repeat_timer = 0.f;
		++it;
	}

	for (auto it = this->m_Rotations_VA.begin(); it != this->m_Rotations_VA.end();) {
		it->first->operator= (it->second->originalVertex);
		it->second->current_rotation = it->second->startingRotation;
		it->second->currentTime = 0.f;
		it->second->repeat_timer = 0.f;
		++it;
	}

	for (auto it = this->m_Rotations_S.begin(); it != this->m_Rotations_S.end();) {
		it->first->setRotation(it->second->startingRotation);
		it->second->currentTime = 0.f;
		it->second->repeat_timer = 0.f;
		++it;
	}
}

void MovementManager::resetRotation(sf::Shape* _shape)
{
	auto& rotationMap = sInstance->m_Rotations_Shape;
	auto rotationFound = rotationMap.find(_shape);

	if (rotationFound != rotationMap.end()) {
		_shape->setRotation(rotationFound->second->startingRotation);
		rotationFound->second->currentTime = 0.f;
		rotationFound->second->repeat_timer = 0.f;
	}
}

void MovementManager::resetRotation(sf::VertexArray* _vertexarray)
{
	auto& rotationMap = sInstance->m_Rotations_VA;
	auto rotationFound = rotationMap.find(_vertexarray);

	if (rotationFound != rotationMap.end()) {
		_vertexarray->operator= (rotationFound->second->originalVertex);
		rotationFound->second->current_rotation = rotationFound->second->startingRotation;
		rotationFound->second->currentTime = 0.f;
		rotationFound->second->repeat_timer = 0.f;
	}
}

void MovementManager::resetRotation(sf::Sprite* _sprite)
{
	auto& rotationMap = sInstance->m_Rotations_S;
	auto rotationFound = rotationMap.find(_sprite);

	if (rotationFound != rotationMap.end()) {
		_sprite->setRotation(rotationFound->second->startingRotation);
		rotationFound->second->currentTime = 0.f;
		rotationFound->second->repeat_timer = 0.f;
	}
}

void MovementManager::stopRotation()
{
	for (auto it = this->m_Rotations_Shape.begin(); it != this->m_Rotations_Shape.end();) {
		delete it->second;
		it = m_Rotations_Shape.erase(it);
	}

	for (auto it = this->m_Rotations_VA.begin(); it != this->m_Rotations_VA.end();) {
		delete it->second;
		it = m_Rotations_VA.erase(it);
	}

	for (auto it = this->m_Rotations_S.begin(); it != this->m_Rotations_S.end();) {
		delete it->second;
		it = m_Rotations_S.erase(it);
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
