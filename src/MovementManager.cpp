#include "MovementManager.h"
#include <assert.h>

MovementManager* MovementManager::sInstance = nullptr;

MovementManager::MovementManager()
{
	assert(this->sInstance == nullptr);
	sInstance = this;
}

void MovementManager::update(float dt)
{
	for (auto it = this->m_Movements.begin(); it != this->m_Movements.end();) {
		it->second->currentTime += dt;

		double position_x = it->second->used_function(static_cast<double>(it->second->currentTime / it->second->movementTime)) * (it->second->endingPos.x - it->second->startingPos.x) + it->second->startingPos.x;
		double position_y = it->second->used_function(static_cast<double>(it->second->currentTime / it->second->movementTime)) * (it->second->endingPos.y - it->second->startingPos.y) + it->second->startingPos.y;
		
		it->first->setPosition(static_cast<float>(position_x), static_cast<float>(position_y));

		if (it->second->isDone()) {
			delete it->second;
			it = m_Movements.erase(it);
		}
		else 
			++it;
	}
}

void MovementManager::addMovement(sf::Vector2f _startingPos, sf::Vector2f _endingPos, float _movementTime, sf::CircleShape* _sprite, movement_type _used_function)
{
	auto& movementMap = sInstance->m_Movements;
	auto movementFound = movementMap.find(_sprite);

	if (movementFound != movementMap.end()) {
		printf("ERROR: Movement already exists!\n");
	}
	else {
		movementInfo* newMovement = new movementInfo(_startingPos, _endingPos, _movementTime, sInstance->movement_functions[_used_function]);
		movementMap.insert(std::make_pair(_sprite, newMovement));
	}
}

void MovementManager::resetMovement()
{
	for (auto it = this->m_Movements.begin(); it != this->m_Movements.end();) {
		it->first->setPosition(it->second->startingPos);
		delete it->second;
		it = m_Movements.erase(it);
	}
}
