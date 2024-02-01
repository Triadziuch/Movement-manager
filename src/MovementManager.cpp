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

		double position_x = it->second->used_function(static_cast<double>(it->second->currentTime / it->second->movementTime)) * (it->second->endingPos.x - it->second->startingPos.x) + it->second->startingPos.x;
		double position_y = it->second->used_function(static_cast<double>(it->second->currentTime / it->second->movementTime)) * (it->second->endingPos.y - it->second->startingPos.y) + it->second->startingPos.y;
		
		it->first->setPosition(static_cast<float>(position_x), static_cast<float>(position_y));

		if (it->second->isDone()) {
			if (it->second->repeat) {
				it->second->currentTime = 0.f;
				it->first->setPosition(it->second->startingPos);
				++it;
			}
			else {
				delete it->second;
				it = m_Movements_CS.erase(it);
			}
		}
		else 
			++it;
	}

	// VertexArray movement
	for (auto it = this->m_Movements_VA.begin(); it != this->m_Movements_VA.end();) {
		it->second->currentTime += dt;

		// OGARN¥Æ TO
		if (it->first->getVertexCount() != 0) {
			double offset_x = it->second->used_function(static_cast<double>(it->second->currentTime / it->second->movementTime)) * (it->second->endingPos.x - it->second->startingPos.x) + it->second->startingPos.x - it->first->operator[](0).position.x;
			double offset_y = it->second->used_function(static_cast<double>(it->second->currentTime / it->second->movementTime)) * (it->second->endingPos.y - it->second->startingPos.y) + it->second->startingPos.y - it->first->operator[](0).position.y;

			for (size_t i = 0; i < it->first->getVertexCount(); i++)
				it->first->operator[](i).position += sf::Vector2f(static_cast<float>(offset_x), static_cast<float>(offset_y));

			if (it->second->isDone()) {
				if (it->second->repeat) {
					double offset_x = it->second->endingPos.x - it->second->startingPos.x;
					double offset_y = it->second->endingPos.y - it->second->startingPos.y;

					it->second->currentTime = 0.f;
					for (size_t i = 0; i < it->first->getVertexCount(); i++)
						it->first->operator[](i).position -= sf::Vector2f(static_cast<float>(offset_x), static_cast<float>(offset_y));
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
void MovementManager::addMovement(sf::Vector2f _startingPos, sf::Vector2f _endingPos, float _movementTime, sf::CircleShape* _circleshape, movement_type _used_function, bool _repeat)
{
	auto& movementMap = sInstance->m_Movements_CS;
	auto movementFound = movementMap.find(_circleshape);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for this object already exists!\n");
	}
	else {
		movementInfo* newMovement = new movementInfo(_startingPos, _endingPos, _movementTime, sInstance->movement_functions[_used_function], _repeat);
		movementMap.insert(std::make_pair(_circleshape, newMovement));
	}
}

void MovementManager::addMovement(sf::Vector2f startingPos, sf::Vector2f endingPos, float movementTime, sf::VertexArray* _vertexarray, movement_type _used_function, bool _repeat)
{
	auto& movementMap = sInstance->m_Movements_VA;
	auto movementFound = movementMap.find(_vertexarray);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement for this object already exists!\n");
	}
	else {
		movementInfo* newMovement = new movementInfo(startingPos, endingPos, movementTime, sInstance->movement_functions[_used_function], _repeat);
		movementMap.insert(std::make_pair(_vertexarray, newMovement));
	}
}

void MovementManager::resetMovement()
{
	for (auto it = this->m_Movements_CS.begin(); it != this->m_Movements_CS.end();) {
		it->first->setPosition(it->second->startingPos);
		delete it->second;
		it = m_Movements_CS.erase(it);
	}
}
