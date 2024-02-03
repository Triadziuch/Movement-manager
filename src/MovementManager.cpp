#include "MovementManager.h"
#include <assert.h>

// Singleton initialization
MovementManager* MovementManager::sInstance = nullptr;

// Private update functions
void MovementManager::updateCircleShape(float dt)
{
	for (auto it = this->m_Movements_CS.begin(); it != this->m_Movements_CS.end();) {
		it->second->currentTime += dt;

		if (it->second->isDone()) {
			if (it->second->repeat) {
				if (it->second->repeat_timer < it->second->wait_before_repeating)
					it->second->repeat_timer += dt;
				else {
					it->second->repeat_timer = 0.f;
					it->second->currentTime = 0.f;
					it->first->setPosition(it->second->startingPos);
				}
				++it;
			}
			else {
				delete it->second;
				it = m_Movements_CS.erase(it);
			}
		}
		else {
			float position_x = static_cast<float>(it->second->used_function(static_cast<double>(it->second->currentTime / it->second->movementTime))) * (it->second->endingPos.x - it->second->startingPos.x) + it->second->startingPos.x;
			float position_y = static_cast<float>(it->second->used_function(static_cast<double>(it->second->currentTime / it->second->movementTime))) * (it->second->endingPos.y - it->second->startingPos.y) + it->second->startingPos.y;

			it->first->setPosition(position_x, position_y);
			++it;
		}
	}

	for (auto it = this->m_Scalings_CS.begin(); it != this->m_Scalings_CS.end();) {
		it->second->currentTime += dt;

		if (it->second->isDone()) {
			if (it->second->repeat) {
				if (it->second->repeat_timer < it->second->wait_before_repeating)
					it->second->repeat_timer += dt;
				else {
					it->second->repeat_timer = 0.f;
					it->second->currentTime = 0.f;
					it->first->setScale(it->second->startingScale);
				}
				++it;
			}
			else {
				delete it->second;
				it = m_Scalings_CS.erase(it);
			}
		}
		else {
			float scale_x = static_cast<float>(it->second->used_function(static_cast<double>(it->second->currentTime / it->second->scalingTime))) * (it->second->endingScale.x - it->second->startingScale.x) + it->second->startingScale.x;
			float scale_y = static_cast<float>(it->second->used_function(static_cast<double>(it->second->currentTime / it->second->scalingTime))) * (it->second->endingScale.y - it->second->startingScale.y) + it->second->startingScale.y;

			it->first->setScale(scale_x, scale_y);
			++it;
		}
	}
}

void MovementManager::updateVertexArray(float dt)
{
	for (auto it = this->m_Movements_VA.begin(); it != this->m_Movements_VA.end();) {
		it->second->currentTime += dt;

		// OGARN¥Æ TO
		if (it->first->getVertexCount() != 0) {
			float offset_x = static_cast<float>(it->second->used_function(static_cast<double>(it->second->currentTime / it->second->movementTime))) * (it->second->endingPos.x - it->second->startingPos.x) + it->second->startingPos.x - it->first->operator[](0).position.x;
			float offset_y = static_cast<float>(it->second->used_function(static_cast<double>(it->second->currentTime / it->second->movementTime))) * (it->second->endingPos.y - it->second->startingPos.y) + it->second->startingPos.y - it->first->operator[](0).position.y;

			for (size_t i = 0; i < it->first->getVertexCount(); i++)
				it->first->operator[](i).position += sf::Vector2f(offset_x, offset_y);

			if (it->second->isDone()) {
				if (it->second->repeat) {
					float offset_x = it->second->endingPos.x - it->second->startingPos.x;
					float offset_y = it->second->endingPos.y - it->second->startingPos.y;

					it->second->currentTime = 0.f;
					for (size_t i = 0; i < it->first->getVertexCount(); i++)
						it->first->operator[](i).position -= sf::Vector2f(offset_x, offset_y);
					++it;
				}
				else {
					delete it->second;
					it = m_Movements_VA.erase(it);
				}
			}
			else
				++it;
		}
	}
}

void MovementManager::updateSprite(float dt)
{
	for (auto it = this->m_Movements_S.begin(); it != this->m_Movements_S.end();) {
		it->second->currentTime += dt;

		if (it->second->isDone()) {
			if (it->second->repeat) {
				if (it->second->repeat_timer < it->second->wait_before_repeating)
					it->second->repeat_timer += dt;
				else {
					it->second->repeat_timer = 0.f;
					it->second->currentTime = 0.f;
					it->first->setPosition(it->second->startingPos);
				}
				++it;
			}
			else {
				delete it->second;
				it = m_Movements_S.erase(it);
			}
		}
		else {
			float position_x = static_cast<float>(it->second->used_function(static_cast<double>(it->second->currentTime / it->second->movementTime))) * (it->second->endingPos.x - it->second->startingPos.x) + it->second->startingPos.x;
			float position_y = static_cast<float>(it->second->used_function(static_cast<double>(it->second->currentTime / it->second->movementTime))) * (it->second->endingPos.y - it->second->startingPos.y) + it->second->startingPos.y;

			it->first->setPosition(position_x, position_y);
			++it;
		}
	}

	for (auto it = this->m_Scalings_S.begin(); it != this->m_Scalings_S.end();) {
		it->second->currentTime += dt;

		if (it->second->isDone()) {
			if (it->second->repeat) {
				if (it->second->repeat_timer < it->second->wait_before_repeating)
					it->second->repeat_timer += dt;
				else {
					it->second->repeat_timer = 0.f;
					it->second->currentTime = 0.f;
					it->first->setScale(it->second->startingScale);
				}
				++it;
			}
			else {
				delete it->second;
				it = m_Scalings_S.erase(it);
			}
		}
		else {
			float scale_x = static_cast<float>(it->second->used_function(static_cast<double>(it->second->currentTime / it->second->scalingTime))) * (it->second->endingScale.x - it->second->startingScale.x) + it->second->startingScale.x;
			float scale_y = static_cast<float>(it->second->used_function(static_cast<double>(it->second->currentTime / it->second->scalingTime))) * (it->second->endingScale.y - it->second->startingScale.y) + it->second->startingScale.y;

			it->first->setScale(scale_x, scale_y);
			++it;
		}
	}
}

void MovementManager::updateRectangleShape(float dt)
{
	for (auto it = this->m_Movements_RS.begin(); it != this->m_Movements_RS.end();) {
		it->second->currentTime += dt;

		if (it->second->isDone()) {
			if (it->second->repeat) {
				if (it->second->repeat_timer < it->second->wait_before_repeating)
					it->second->repeat_timer += dt;
				else {
					it->second->repeat_timer = 0.f;
					it->second->currentTime = 0.f;
					it->first->setPosition(it->second->startingPos);
				}
				++it;
			}
			else {
				delete it->second;
				it = m_Movements_RS.erase(it);
			}
		}
		else {
			float position_x = static_cast<float>(it->second->used_function(static_cast<double>(it->second->currentTime / it->second->movementTime))) * (it->second->endingPos.x - it->second->startingPos.x) + it->second->startingPos.x;
			float position_y = static_cast<float>(it->second->used_function(static_cast<double>(it->second->currentTime / it->second->movementTime))) * (it->second->endingPos.y - it->second->startingPos.y) + it->second->startingPos.y;

			it->first->setPosition(position_x, position_y);
			++it;
		}
	}

	for (auto it = this->m_Scalings_RS.begin(); it != this->m_Scalings_RS.end();) {
		it->second->currentTime += dt;

		if (it->second->isDone()) {
			if (it->second->repeat) {
				if (it->second->repeat_timer < it->second->wait_before_repeating)
					it->second->repeat_timer += dt;
				else {
					it->second->repeat_timer = 0.f;
					it->second->currentTime = 0.f;
					it->first->setScale(it->second->startingScale);
				}
				++it;
			}
			else {
				delete it->second;
				it = m_Scalings_RS.erase(it);
			}
		}
		else {
			float scale_x = static_cast<float>(it->second->used_function(static_cast<double>(it->second->currentTime / it->second->scalingTime))) * (it->second->endingScale.x - it->second->startingScale.x) + it->second->startingScale.x;
			float scale_y = static_cast<float>(it->second->used_function(static_cast<double>(it->second->currentTime / it->second->scalingTime))) * (it->second->endingScale.y - it->second->startingScale.y) + it->second->startingScale.y;

			it->first->setScale(scale_x, scale_y);
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
	this->updateCircleShape(dt);
	this->updateVertexArray(dt);
	this->updateSprite(dt);
	this->updateRectangleShape(dt);
}

// Public functions

// Movement functions
const bool MovementManager::addMovement(sf::CircleShape* _circleshape, sf::Vector2f endingPos, float movementTime, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& movementMap = sInstance->m_Movements_CS;
	auto movementFound = movementMap.find(_circleshape);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::CircleShape* object already exists!\n");
		return 0;
	}
	else {
		movementInfo* newMovement = new movementInfo(_circleshape->getPosition(), endingPos, movementTime, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating);
		movementMap.insert(std::make_pair(_circleshape, newMovement));
	}
	return 1;
}

const bool MovementManager::addMovement(sf::CircleShape* _circleshape, sf::Vector2f _startingPos, sf::Vector2f _endingPos, float _movementTime, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& movementMap = sInstance->m_Movements_CS;
	auto movementFound = movementMap.find(_circleshape);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::CircleShape* object already exists!\n");
		return 0;
	}
	else {
		movementInfo* newMovement = new movementInfo(_startingPos, _endingPos, _movementTime, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating);
		movementMap.insert(std::make_pair(_circleshape, newMovement));
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
		movementInfo* newMovement = new movementInfo(_vertexarray->operator[](0).position, endingPos, movementTime, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating);
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
		movementInfo* newMovement = new movementInfo(startingPos, endingPos, movementTime, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating);
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

const bool MovementManager::addMovement(sf::RectangleShape* _rectangleshape, sf::Vector2f endingPos, float movementTime, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& movementMap = sInstance->m_Movements_RS;
	auto movementFound = movementMap.find(_rectangleshape);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::RectangleShape* object already exists!\n");
		return 0;
	}
	else {
		movementInfo* newMovement = new movementInfo(_rectangleshape->getPosition(), endingPos, movementTime, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating);
		movementMap.insert(std::make_pair(_rectangleshape, newMovement));
	}
	return 1;
}

const bool MovementManager::addMovement(sf::RectangleShape* _rectangleshape, sf::Vector2f startingPos, sf::Vector2f endingPos, float movementTime, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& movementMap = sInstance->m_Movements_RS;
	auto movementFound = movementMap.find(_rectangleshape);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for sf::RectangleShape* object already exists!\n");
		return 0;
	}
	else {
		movementInfo* newMovement = new movementInfo(startingPos, endingPos, movementTime, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating);
		movementMap.insert(std::make_pair(_rectangleshape, newMovement));
	}
	return 1;
}

void MovementManager::undoMovement()
{
	for (auto it = this->m_Movements_CS.begin(); it != this->m_Movements_CS.end();) {
		it->first->setPosition(it->second->startingPos);
		delete it->second;
		it = m_Movements_CS.erase(it);
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

	for (auto it = this->m_Movements_RS.begin(); it != this->m_Movements_RS.end();) {
		it->first->setPosition(it->second->startingPos);
		delete it->second;
		it = m_Movements_RS.erase(it);
	}
}

void MovementManager::undoMovement(sf::CircleShape* _circleshape)
{
	auto& movementMap = sInstance->m_Movements_CS;
	auto movementFound = movementMap.find(_circleshape);

	if (movementFound != movementMap.end()) {
		_circleshape->setPosition(movementFound->second->startingPos);
		delete movementFound->second;
		movementMap.erase(movementFound);
	}
}

void MovementManager::undoMovement(sf::VertexArray* _vertexarray)
{
	auto& movementMap = sInstance->m_Movements_VA;
	auto movementFound = movementMap.find(_vertexarray);

	if (movementFound != movementMap.end()) {
		if (_vertexarray->getVertexCount() != 0) {
			sf::Vector2f offset = movementFound->first->operator[](0).position - movementFound->second->startingPos;

			for (size_t i = 0; i < _vertexarray->getVertexCount(); i++)
				_vertexarray->operator[](i).position -= offset;

			delete movementFound->second;
			movementMap.erase(movementFound);
		}
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

void MovementManager::undoMovement(sf::RectangleShape* _rectangleshape)
{
	auto& movementMap = sInstance->m_Movements_RS;
	auto movementFound = movementMap.find(_rectangleshape);

	if (movementFound != movementMap.end()) {
		_rectangleshape->setPosition(movementFound->second->startingPos);
		delete movementFound->second;
		movementMap.erase(movementFound);
	}
}

void MovementManager::resetMovement()
{
	for (auto it = this->m_Movements_CS.begin(); it != this->m_Movements_CS.end();) {
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

	for (auto it = this->m_Movements_RS.begin(); it != this->m_Movements_RS.end();) {
		it->first->setPosition(it->second->startingPos);
		it->second->currentTime = 0.f;
		it->second->repeat_timer = 0.f;
		++it;
	}
}

void MovementManager::resetMovement(sf::CircleShape* _circleshape)
{
	auto& movementMap = sInstance->m_Movements_CS;
	auto movementFound = movementMap.find(_circleshape);

	if (movementFound != movementMap.end()) {
		_circleshape->setPosition(movementFound->second->startingPos);
		movementFound->second->currentTime = 0.f;
		movementFound->second->repeat_timer = 0.f;
	}
}

void MovementManager::resetMovement(sf::VertexArray* _vertexarray)
{
	auto& movementMap = sInstance->m_Movements_VA;
	auto movementFound = movementMap.find(_vertexarray);

	if (movementFound != movementMap.end()) {
		if (_vertexarray->getVertexCount() != 0) {
			sf::Vector2f offset = movementFound->first->operator[](0).position - movementFound->second->startingPos;

			for (size_t i = 0; i < _vertexarray->getVertexCount(); i++)
				_vertexarray->operator[](i).position -= offset;
		}
		movementFound->second->currentTime = 0.f;
		movementFound->second->repeat_timer = 0.f;
	}
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

void MovementManager::resetMovement(sf::RectangleShape* _rectangleshape)
{
	auto& movementMap = sInstance->m_Movements_RS;
	auto movementFound = movementMap.find(_rectangleshape);

	if (movementFound != movementMap.end()) {
		_rectangleshape->setPosition(movementFound->second->startingPos);
		movementFound->second->currentTime = 0.f;
		movementFound->second->repeat_timer = 0.f;
	}
}

void MovementManager::stopMovement()
{
	for (auto it = this->m_Movements_CS.begin(); it != this->m_Movements_CS.end();) {
		delete it->second;
		it = m_Movements_CS.erase(it);
	}

	for (auto it = this->m_Movements_VA.begin(); it != this->m_Movements_VA.end();) {
		delete it->second;
		it = m_Movements_VA.erase(it);
	}

	for (auto it = this->m_Movements_S.begin(); it != this->m_Movements_S.end();) {
		delete it->second;
		it = m_Movements_S.erase(it);
	}

	for (auto it = this->m_Movements_RS.begin(); it != this->m_Movements_RS.end();) {
		delete it->second;
		it = m_Movements_RS.erase(it);
	}
}

void MovementManager::stopMovement(sf::CircleShape* _circleshape)
{
	auto& movementMap = sInstance->m_Movements_CS;
	auto movementFound = movementMap.find(_circleshape);

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

void MovementManager::stopMovement(sf::RectangleShape* _rectangleshape)
{
	auto& movementMap = sInstance->m_Movements_RS;
	auto movementFound = movementMap.find(_rectangleshape);

	if (movementFound != movementMap.end()) {
		delete movementFound->second;
		movementMap.erase(movementFound);
	}
}

// Scaling functions
const bool MovementManager::addScaling(sf::CircleShape* _circleshape, sf::Vector2f endingScale, float scalingTime, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& scalingMap = sInstance->m_Scalings_CS;
	auto scalingFound = scalingMap.find(_circleshape);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::CircleShape* object already exists!\n");
		return 0;
	}
	else {
		scalingInfo* newScaling = new scalingInfo(_circleshape->getScale(), endingScale, scalingTime, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating);
		scalingMap.insert(std::make_pair(_circleshape, newScaling));
	}
	return 1;
}

const bool MovementManager::addScaling(sf::CircleShape* _circleshape, sf::Vector2f startingScale, sf::Vector2f endingScale, float scalingTime, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& scalingMap = sInstance->m_Scalings_CS;
	auto scalingFound = scalingMap.find(_circleshape);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::CircleShape* object already exists!\n");
		return 0;
	}
	else {
		scalingInfo* newScaling = new scalingInfo(startingScale, endingScale, scalingTime, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating);
		scalingMap.insert(std::make_pair(_circleshape, newScaling));
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

const bool MovementManager::addScaling(sf::RectangleShape* _rectangleshape, sf::Vector2f endingScale, float scalingTime, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& scalingMap = sInstance->m_Scalings_RS;
	auto scalingFound = scalingMap.find(_rectangleshape);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::RectangleShape* object already exists!\n");
		return 0;
	}
	else {
		scalingInfo* newScaling = new scalingInfo(_rectangleshape->getScale(), endingScale, scalingTime, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating);
		scalingMap.insert(std::make_pair(_rectangleshape, newScaling));
	}
	return 1;
}

const bool MovementManager::addScaling(sf::RectangleShape* _rectangleshape, sf::Vector2f startingScale, sf::Vector2f endingScale, float scalingTime, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& scalingMap = sInstance->m_Scalings_RS;
	auto scalingFound = scalingMap.find(_rectangleshape);

	if (scalingFound != scalingMap.end()) {
		printf("ERROR: Scaling for sf::RectangleShape* object already exists!\n");
		return 0;
	}
	else {
		scalingInfo* newScaling = new scalingInfo(startingScale, endingScale, scalingTime, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating);
		scalingMap.insert(std::make_pair(_rectangleshape, newScaling));
	}
	return 1;
}

void MovementManager::undoScaling()
{
	for (auto it = this->m_Scalings_CS.begin(); it != this->m_Scalings_CS.end();) {
		it->first->setScale(it->second->startingScale);
		delete it->second;
		it = m_Scalings_CS.erase(it);
	}

	for (auto it = this->m_Scalings_S.begin(); it != this->m_Scalings_S.end();) {
		it->first->setScale(it->second->startingScale);
		delete it->second;
		it = m_Scalings_S.erase(it);
	}

	for (auto it = this->m_Scalings_RS.begin(); it != this->m_Scalings_RS.end();) {
		it->first->setScale(it->second->startingScale);
		delete it->second;
		it = m_Scalings_RS.erase(it);
	}
}

void MovementManager::undoScaling(sf::CircleShape* _circleshape)
{
	auto& scalingMap = sInstance->m_Scalings_CS;
	auto scalingFound = scalingMap.find(_circleshape);

	if (scalingFound != scalingMap.end()) {
		_circleshape->setScale(scalingFound->second->startingScale);
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

void MovementManager::undoScaling(sf::RectangleShape* _rectangleshape)
{
	auto& scalingMap = sInstance->m_Scalings_RS;
	auto scalingFound = scalingMap.find(_rectangleshape);

	if (scalingFound != scalingMap.end()) {
		_rectangleshape->setScale(scalingFound->second->startingScale);
		delete scalingFound->second;
		scalingMap.erase(scalingFound);
	}
}

void MovementManager::resetScaling()
{
	for (auto it = this->m_Scalings_CS.begin(); it != this->m_Scalings_CS.end();) {
		it->first->setScale(it->second->startingScale);
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

	for (auto it = this->m_Scalings_RS.begin(); it != this->m_Scalings_RS.end();) {
		it->first->setScale(it->second->startingScale);
		it->second->currentTime = 0.f;
		it->second->repeat_timer = 0.f;
		++it;
	}
}

void MovementManager::resetScaling(sf::CircleShape* _circleshape)
{
	auto& scalingMap = sInstance->m_Scalings_CS;
	auto scalingFound = scalingMap.find(_circleshape);

	if (scalingFound != scalingMap.end()) {
		_circleshape->setScale(scalingFound->second->startingScale);
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

void MovementManager::resetScaling(sf::RectangleShape* _rectangleshape)
{
	auto& scalingMap = sInstance->m_Scalings_RS;
	auto scalingFound = scalingMap.find(_rectangleshape);

	if (scalingFound != scalingMap.end()) {
		_rectangleshape->setScale(scalingFound->second->startingScale);
		scalingFound->second->currentTime = 0.f;
		scalingFound->second->repeat_timer = 0.f;
	}
}

void MovementManager::stopScaling()
{
	for (auto it = this->m_Scalings_CS.begin(); it != this->m_Scalings_CS.end();) {
		delete it->second;
		it = m_Scalings_CS.erase(it);
	}

	for (auto it = this->m_Scalings_S.begin(); it != this->m_Scalings_S.end();) {
		delete it->second;
		it = m_Scalings_S.erase(it);
	}

	for (auto it = this->m_Scalings_RS.begin(); it != this->m_Scalings_RS.end();) {
		delete it->second;
		it = m_Scalings_RS.erase(it);
	}
}

void MovementManager::stopScaling(sf::CircleShape* _circleshape)
{
	auto& scalingMap = sInstance->m_Scalings_CS;
	auto scalingFound = scalingMap.find(_circleshape);

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

void MovementManager::stopScaling(sf::RectangleShape* _rectangleshape)
{
	auto& scalingMap = sInstance->m_Scalings_RS;
	auto scalingFound = scalingMap.find(_rectangleshape);

	if (scalingFound != scalingMap.end()) {
		delete scalingFound->second;
		scalingMap.erase(scalingFound);
	}
}
