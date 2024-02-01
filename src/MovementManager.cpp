#include "MovementManager.h"
#include <assert.h>

// Singleton initialization
MovementManager* MovementManager::sInstance = nullptr;

// Default constructor
MovementManager::MovementManager()
{
	assert(this->sInstance == nullptr);
	sInstance = this;
}

// Update functions
void MovementManager::update(float dt)
{
	// CircleShape movement
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

	// VertexArray movement
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

// Public functions
void MovementManager::addMovement(sf::Vector2f _startingPos, sf::Vector2f _endingPos, float _movementTime, sf::CircleShape* _circleshape, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& movementMap = sInstance->m_Movements_CS;
	auto movementFound = movementMap.find(_circleshape);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for this object already exists!\n");
	}
	else {
		movementInfo* newMovement = new movementInfo(_startingPos, _endingPos, _movementTime, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating);
		movementMap.insert(std::make_pair(_circleshape, newMovement));
	}
}

void MovementManager::addMovement(sf::Vector2f startingPos, sf::Vector2f endingPos, float movementTime, sf::VertexArray* _vertexarray, movement_type _used_function, bool _repeat, float _wait_before_repeating)
{
	auto& movementMap = sInstance->m_Movements_VA;
	auto movementFound = movementMap.find(_vertexarray);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for this object already exists!\n");
	}
	else {
		movementInfo* newMovement = new movementInfo(startingPos, endingPos, movementTime, sInstance->movement_functions[_used_function], _repeat, _wait_before_repeating);
		movementMap.insert(std::make_pair(_vertexarray, newMovement));
	}
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
