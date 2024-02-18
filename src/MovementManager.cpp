#pragma once
#include "MovementManager.h"

// Singleton initialization
MovementManager* MovementManager::sInstance = nullptr;

MovementManager::MovementManager()
{
	assert(this->sInstance == nullptr);
	sInstance = this;

	movementContainer = new MovementContainer();
}

const MovementRoutine* MovementManager::createMovementRoutine(sf::Shape* _shape, const std::string& _name)
{
	auto& routineMap = sInstance->m_Routine_Movement_Shape;
	auto routineFound = routineMap.find(_shape);

	if (routineFound != routineMap.end()){
		for (auto& routine : routineFound->second)
			if (routine->routine_name == _name)
				return routine;
	}

	auto routine = new MovementRoutine(_shape, _name);
	routineMap[_shape] = routine;
	sInstance->m_Routine_Movement_Shape_Active[_shape] = routine;
	return routine;
}

void MovementManager::update(float dt)
{
	for (auto& movementRoutine : m_Routine_Movement_Shape_Active){
		movementRoutine.second->update(this->movementContainer, movementRoutine.first);
	}

	this->movementContainer->update(dt);
}
