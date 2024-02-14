#pragma once
#include "MovementManager.h"

// Singleton initialization
MovementManager* MovementManager::sInstance = nullptr;

MovementManager::MovementManager()
{
	assert(this->sInstance == nullptr);
	sInstance = this;
}

const MovementRoutine* MovementManager::createMovementRoutine(const sf::Shape* _shape, const std::string& _name)
{
MovementRoutine* newRoutine = new MovementRoutine(_shape, _name);
	movementRoutines.push_back(newRoutine);
	return newRoutine;
}
