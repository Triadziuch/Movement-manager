#pragma once
#include "MovementManager.h"

// Singleton initialization
MovementManager* MovementManager::sInstance = nullptr;

MovementManager::MovementManager()
{
	assert(this->sInstance == nullptr);
	sInstance = this;

	movementContainer = new MovementContainer();
	movementRoutineContainer = new MovementRoutineContainer();
}

MovementRoutine* MovementManager::linkMovementRoutine(sf::Shape* _shape, const std::string& _name)
{
	// Check if routine with given name exists
	MovementRoutine* movementRoutineOriginal = movementRoutineContainer->getRoutinePtr(_name);

	if (movementRoutineOriginal == nullptr) {
		printf("MovementManager::linkMovementRoutine: Routine with name %s not found\n", _name.c_str());
		return nullptr;
	}

	// Check if we linked a movement routine to this shape before
	auto movementRoutineContainerFound = m_Routine_Movement_Shape.find(_shape);
	if (movementRoutineContainerFound != m_Routine_Movement_Shape.end()){

		auto* movementRoutineFound = movementRoutineContainerFound->second->exists(_name);
		if (movementRoutineFound != nullptr){
			printf("MovementManager::linkMovementRoutine: Routine with name %s already linked to shape\n", _name.c_str());
			return movementRoutineFound;
		}
	}
	else {
		// If not, create a new movement routine container for this shape and link the movement routine to it
		MovementRoutineContainer* newMovementRoutineContainer = new MovementRoutineContainer(this->movementContainer);
		m_Routine_Movement_Shape.insert(std::make_pair(_shape, newMovementRoutineContainer));

		MovementRoutine* newMovementRoutine = new MovementRoutine(*movementRoutineOriginal);
		return newMovementRoutineContainer->createRoutine(_name, newMovementRoutine);
	}

	// If yes, create a new movement routine and link it to the existing movement routine container
	MovementRoutine* newMovementRoutine = new MovementRoutine(*movementRoutineOriginal);
	return movementRoutineContainerFound->second->createRoutine(_name, newMovementRoutine);
}

MovementRoutine* MovementManager::linkMovementRoutine(sf::Sprite* _sprite, const std::string& _name)
{
	// Check if routine with given name exists
	MovementRoutine* movementRoutineOriginal = movementRoutineContainer->getRoutinePtr(_name);

	if (movementRoutineOriginal == nullptr) {
		printf("MovementManager::linkMovementRoutine: Routine with name %s not found\n", _name.c_str());
		return nullptr;
	}

	// Check if we linked a movement routine to this shape before
	auto movementRoutineContainerFound = m_Routine_Movement_Sprite.find(_sprite);
	if (movementRoutineContainerFound != m_Routine_Movement_Sprite.end()){

		auto* movementRoutineFound = movementRoutineContainerFound->second->exists(_name);
		if (movementRoutineFound != nullptr){
			printf("MovementManager::linkMovementRoutine: Routine with name %s already linked to shape\n", _name.c_str());
			return movementRoutineFound;
		}
	}
	else {
		// If not, create a new movement routine container for this shape and link the movement routine to it
		MovementRoutineContainer* newMovementRoutineContainer = new MovementRoutineContainer(this->movementContainer);
		m_Routine_Movement_Sprite.insert(std::make_pair(_sprite, newMovementRoutineContainer));

		MovementRoutine* newMovementRoutine = new MovementRoutine(*movementRoutineOriginal);
		return newMovementRoutineContainer->createRoutine(_name, newMovementRoutine);
	}

	// If yes, create a new movement routine and link it to the existing movement routine container
	MovementRoutine* newMovementRoutine = new MovementRoutine(*movementRoutineOriginal);
	return movementRoutineContainerFound->second->createRoutine(_name, newMovementRoutine);
}

void MovementManager::startMovementRoutine(sf::Shape* _shape, const std::string& _name)
{
	auto movementRoutineContainerFound = m_Routine_Movement_Shape.find(_shape);
	if (movementRoutineContainerFound != m_Routine_Movement_Shape.end()) {

		auto* movementRoutineFound = movementRoutineContainerFound->second->exists(_name);
		if (movementRoutineFound != nullptr) {
			movementRoutineFound->start();
			m_Routine_Movement_Shape_Active.insert(std::make_pair(_shape, movementRoutineFound));
		}
		else
			printf("MovementManager::startMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::startMovementRoutine: Routine for shape not found\n");
}

void MovementManager::startMovementRoutine(sf::Sprite* _sprite, const std::string& _name)
{
	auto movementRoutineContainerFound = m_Routine_Movement_Sprite.find(_sprite);
	if (movementRoutineContainerFound != m_Routine_Movement_Sprite.end()) {

		auto* movementRoutineFound = movementRoutineContainerFound->second->exists(_name);
		if (movementRoutineFound != nullptr) {
			movementRoutineFound->reset();
			this->movementContainer->addMovement(_sprite, movementRoutineFound->getCurrentMovement());
			m_Routine_Movement_Sprite_Active.insert(std::make_pair(_sprite, movementRoutineFound));
		}
		else
			printf("MovementManager::startMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::startMovementRoutine: Routine for sprite not found\n");
}

void MovementManager::update(float dt)
{
	this->updateShape(dt);

	//this->movementContainer->update(dt);
}

void MovementManager::updateShape(float dt)
{
	for (auto& movementRoutine : m_Routine_Movement_Shape_Active) {
		movementRoutine.second->update(dt);
	}
}
