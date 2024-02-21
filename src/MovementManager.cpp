#pragma once
#include "MovementManager.h"

// Singleton initialization
MovementManager* MovementManager::sInstance = nullptr;

MovementManager::MovementManager()
{
	assert(this->sInstance == nullptr);
	sInstance = this;

	movementRoutineEngine = new MovementRoutineEngine();
	movementRoutineContainer = new MovementRoutineContainer();
	movementRoutineVAContainer = new MovementRoutineVAContainer();
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
		MovementRoutineContainer* newMovementRoutineContainer = new MovementRoutineContainer(this->movementRoutineEngine);
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
		MovementRoutineContainer* newMovementRoutineContainer = new MovementRoutineContainer(this->movementRoutineEngine);
		m_Routine_Movement_Sprite.insert(std::make_pair(_sprite, newMovementRoutineContainer));

		MovementRoutine* newMovementRoutine = new MovementRoutine(*movementRoutineOriginal);
		return newMovementRoutineContainer->createRoutine(_name, newMovementRoutine);
	}

	// If yes, create a new movement routine and link it to the existing movement routine container
	MovementRoutine* newMovementRoutine = new MovementRoutine(*movementRoutineOriginal);
	return movementRoutineContainerFound->second->createRoutine(_name, newMovementRoutine);
}

MovementRoutineVA* MovementManager::linkMovementRoutine(sf::VertexArray* _vertexarray, const std::string& _name)
{
	// Check if routine with given name exists
	MovementRoutineVA* movementRoutineVAOriginal = movementRoutineVAContainer->getRoutinePtr(_name);

	if (movementRoutineVAOriginal == nullptr) {
		printf("MovementManager::linkMovementRoutine: Routine with name %s not found\n", _name.c_str());
		return nullptr;
	}

	// Check if we linked a movement routine to this vertexarray before
	auto movementRoutineVAContainerFound = m_Routine_Movement_VertexArray.find(_vertexarray);
	if (movementRoutineVAContainerFound != m_Routine_Movement_VertexArray.end()) {

		auto* movementRoutineVAFound = movementRoutineVAContainerFound->second->exists(_name);
		if (movementRoutineVAFound != nullptr) {
			printf("MovementManager::linkMovementRoutine: Routine with name %s already linked to vertexarray\n", _name.c_str());
			return movementRoutineVAFound;
		}
	}
	else {
		// If not, create a new movement routine container for this vertexarray and link the movement routine to it
		MovementRoutineVAContainer* newMovementRoutineVAContainer = new MovementRoutineVAContainer(this->movementRoutineEngine);
		m_Routine_Movement_VertexArray.insert(std::make_pair(_vertexarray, newMovementRoutineVAContainer));

		MovementRoutineVA* newMovementRoutineVA = new MovementRoutineVA(*movementRoutineVAOriginal);
		return newMovementRoutineVAContainer->createRoutine(_name, newMovementRoutineVA);
	}

	// If yes, create a new movement routine and link it to the existing movement routine container
	MovementRoutineVA* newMovementRoutineVA = new MovementRoutineVA(*movementRoutineVAOriginal);
	return movementRoutineVAContainerFound->second->createRoutine(_name, newMovementRoutineVA);

}

void MovementManager::unlinkMovementRoutine(sf::Shape* _shape, const std::string& _name)
{
	// Stop the routine if it's active
	this->stopMovementRoutine(_shape, _name);

	// Delete the routine from m_Routine_Movement_Shape
	auto movementRoutineContainerFound = m_Routine_Movement_Shape.find(_shape);
	if (movementRoutineContainerFound != m_Routine_Movement_Shape.end()) {

		auto* movementRoutineFound = movementRoutineContainerFound->second->exists(_name);
		if (movementRoutineFound != nullptr) {
			movementRoutineContainerFound->second->deleteRoutine(_name);
		}
		else
			printf("MovementManager::unlinkMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::unlinkMovementRoutine: Routine for shape not found\n");
}

void MovementManager::unlinkMovementRoutine(sf::Sprite* _sprite, const std::string& _name)
{
	// Stop the routine if it's active
	this->stopMovementRoutine(_sprite, _name);

	// Delete the routine from m_Routine_Movement_Sprite
	auto movementRoutineContainerFound = m_Routine_Movement_Sprite.find(_sprite);
	if (movementRoutineContainerFound != m_Routine_Movement_Sprite.end()) {

		auto* movementRoutineFound = movementRoutineContainerFound->second->exists(_name);
		if (movementRoutineFound != nullptr) {
			movementRoutineContainerFound->second->deleteRoutine(_name);
		}
		else
			printf("MovementManager::unlinkMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::unlinkMovementRoutine: Routine for sprite not found\n");
}

void MovementManager::unlinkMovementRoutine(sf::VertexArray* _vertexarray, const std::string& _name)
{
	// Stop the routine if it's active
	this->stopMovementRoutine(_vertexarray, _name);

	// Delete the routine from m_Routine_Movement_VertexArray
	auto movementRoutineContainerFound = m_Routine_Movement_VertexArray.find(_vertexarray);
	if (movementRoutineContainerFound != m_Routine_Movement_VertexArray.end()) {

		auto* movementRoutineFound = movementRoutineContainerFound->second->exists(_name);
		if (movementRoutineFound != nullptr) {
			movementRoutineContainerFound->second->deleteRoutine(_name);
		}
		else
			printf("MovementManager::unlinkMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::unlinkMovementRoutine: Routine for vertexarray not found\n");

}

void MovementManager::startMovementRoutine(sf::Shape* _shape, const std::string& _name)
{
	auto movementRoutineContainerFound = m_Routine_Movement_Shape.find(_shape);
	if (movementRoutineContainerFound != m_Routine_Movement_Shape.end()) {

		auto* movementRoutineFound = movementRoutineContainerFound->second->exists(_name);
		if (movementRoutineFound != nullptr) {

			if (movementRoutineFound->start(_shape))
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

			if (movementRoutineFound->start(_sprite))
				m_Routine_Movement_Sprite_Active.insert(std::make_pair(_sprite, movementRoutineFound));
		}
		else
			printf("MovementManager::startMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::startMovementRoutine: Routine for sprite not found\n");
}

void MovementManager::startMovementRoutine(sf::VertexArray* _vertexarray, const std::string& _name)
{
	auto movementRoutineContainerFound = m_Routine_Movement_VertexArray.find(_vertexarray);
	if (movementRoutineContainerFound != m_Routine_Movement_VertexArray.end()) {

		auto* movementRoutineFound = movementRoutineContainerFound->second->exists(_name);
		if (movementRoutineFound != nullptr) {

			if (movementRoutineFound->start(_vertexarray))
				m_Routine_Movement_VertexArray_Active.insert(std::make_pair(_vertexarray, movementRoutineFound));
		}
		else
			printf("MovementManager::startMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::startMovementRoutine: Routine for vertexarray not found\n");
}

void MovementManager::pauseMovementRoutine(sf::Shape* _shape, const std::string& _name)
{
	auto movementRoutineFound = m_Routine_Movement_Shape_Active.find(_shape);
	if (movementRoutineFound != m_Routine_Movement_Shape_Active.end()) {
		if (movementRoutineFound->second->routine_name == _name) 
			movementRoutineFound->second->pause();
		else
			printf("MovementManager::pauseMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::pauseMovementRoutine: Routine for shape not found\n");
}

void MovementManager::pauseMovementRoutine(sf::Sprite* _sprite, const std::string& _name)
{
	auto movementRoutineFound = m_Routine_Movement_Sprite_Active.find(_sprite);
	if (movementRoutineFound != m_Routine_Movement_Sprite_Active.end()) {
		if (movementRoutineFound->second->routine_name == _name) 
			movementRoutineFound->second->pause();
		else
			printf("MovementManager::pauseMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::pauseMovementRoutine: Routine for sprite not found\n");
}

void MovementManager::pauseMovementRoutine(sf::VertexArray* _vertexarray, const std::string& _name)
{
	auto movementRoutineFound = m_Routine_Movement_VertexArray_Active.find(_vertexarray);
	if (movementRoutineFound != m_Routine_Movement_VertexArray_Active.end()) {
		if (movementRoutineFound->second->routine_name == _name) 
			movementRoutineFound->second->pause();
		else
			printf("MovementManager::pauseMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::pauseMovementRoutine: Routine for vertexarray not found\n");
}

void MovementManager::resumeMovementRoutine(sf::Shape* _shape, const std::string& _name)
{
	auto movementRoutineFound = m_Routine_Movement_Shape_Active.find(_shape);
	if (movementRoutineFound != m_Routine_Movement_Shape_Active.end()) {
		if (movementRoutineFound->second->routine_name == _name) 
			movementRoutineFound->second->resume();
		else
			printf("MovementManager::resumeMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::resumeMovementRoutine: Routine for shape not found\n");
}


void MovementManager::resumeMovementRoutine(sf::Sprite* _sprite, const std::string& _name)
{
	auto movementRoutineFound = m_Routine_Movement_Sprite_Active.find(_sprite);
	if (movementRoutineFound != m_Routine_Movement_Sprite_Active.end()) {
		if (movementRoutineFound->second->routine_name == _name) 
			movementRoutineFound->second->resume();
		else
			printf("MovementManager::resumeMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::resumeMovementRoutine: Routine for sprite not found\n");
}

void MovementManager::resumeMovementRoutine(sf::VertexArray* _vertexarray, const std::string& _name)
{
	auto movementRoutineFound = m_Routine_Movement_VertexArray_Active.find(_vertexarray);
	if (movementRoutineFound != m_Routine_Movement_VertexArray_Active.end()) {
		if (movementRoutineFound->second->routine_name == _name) 
			movementRoutineFound->second->resume();
		else
			printf("MovementManager::resumeMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::resumeMovementRoutine: Routine for vertexarray not found\n");
}

void MovementManager::stopMovementRoutine(sf::Shape* _shape, const std::string& _name)
{
	auto movementRoutineFound = m_Routine_Movement_Shape_Active.find(_shape);
	if (movementRoutineFound != m_Routine_Movement_Shape_Active.end()) {
		if (movementRoutineFound->second->routine_name == _name) {
			movementRoutineFound->second->stop(_shape);
			m_Routine_Movement_Shape_Active.erase(movementRoutineFound);
		}
		else
			printf("MovementManager::stopMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::stopMovementRoutine: Routine for shape not found\n");
}

void MovementManager::stopMovementRoutine(sf::Sprite* _sprite, const std::string& _name)
{
	auto movementRoutineFound = m_Routine_Movement_Sprite_Active.find(_sprite);
	if (movementRoutineFound != m_Routine_Movement_Sprite_Active.end()) {
		if (movementRoutineFound->second->routine_name == _name) {
			movementRoutineFound->second->stop(_sprite);
			m_Routine_Movement_Sprite_Active.erase(movementRoutineFound);
		}
		else
			printf("MovementManager::stopMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::stopMovementRoutine: Routine for sprite not found\n");
}

void MovementManager::stopMovementRoutine(sf::VertexArray* _vertexarray, const std::string& _name)
{
	auto movementRoutineFound = m_Routine_Movement_VertexArray_Active.find(_vertexarray);
	if (movementRoutineFound != m_Routine_Movement_VertexArray_Active.end()) {
		if (movementRoutineFound->second->routine_name == _name) {
			movementRoutineFound->second->stop(_vertexarray);
			m_Routine_Movement_VertexArray_Active.erase(movementRoutineFound);
		}
		else
			printf("MovementManager::stopMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::stopMovementRoutine: Routine for vertexarray not found\n");
}

void MovementManager::deleteMovementRoutine()
{
	for (auto routine_active = m_Routine_Movement_Shape_Active.begin(); routine_active != m_Routine_Movement_Shape_Active.end();) {
		routine_active->second->stop(routine_active->first);
		routine_active = m_Routine_Movement_Shape_Active.erase(routine_active);
	}

	for (auto routine_active = m_Routine_Movement_Sprite_Active.begin(); routine_active != m_Routine_Movement_Sprite_Active.end();) {
		routine_active->second->stop(routine_active->first);
		routine_active = m_Routine_Movement_Sprite_Active.erase(routine_active);
	}

	for (auto routine_active = m_Routine_Movement_VertexArray_Active.begin(); routine_active != m_Routine_Movement_VertexArray_Active.end();) {
		routine_active->second->stop(routine_active->first);
		routine_active = m_Routine_Movement_VertexArray_Active.erase(routine_active);
	}

	for (auto routineContainer = m_Routine_Movement_Shape.begin(); routineContainer != m_Routine_Movement_Shape.end();) {
		routineContainer->second->clear();
		delete routineContainer->second;
		routineContainer = m_Routine_Movement_Shape.erase(routineContainer);
	}

	for (auto routineContainer = m_Routine_Movement_Sprite.begin(); routineContainer != m_Routine_Movement_Sprite.end();) {
		routineContainer->second->clear();
		delete routineContainer->second;
		routineContainer = m_Routine_Movement_Sprite.erase(routineContainer);
	}

	for (auto routineContainer = m_Routine_Movement_VertexArray.begin(); routineContainer != m_Routine_Movement_VertexArray.end();) {
		routineContainer->second->clear();
		delete routineContainer->second;
		routineContainer = m_Routine_Movement_VertexArray.erase(routineContainer);
	}

	movementRoutineContainer->clear();
	movementRoutineVAContainer->clear();
}

void MovementManager::deleteMovementRoutine(const std::string& _name)
{
	for (auto routine_active = m_Routine_Movement_Shape_Active.begin(); routine_active != m_Routine_Movement_Shape_Active.end();) {
		if (routine_active->second->routine_name == _name) {
			routine_active->second->stop(routine_active->first);
			routine_active = m_Routine_Movement_Shape_Active.erase(routine_active);
		}
		else
			++routine_active;
	}

	for (auto routine_active = m_Routine_Movement_Sprite_Active.begin(); routine_active != m_Routine_Movement_Sprite_Active.end();) {
		if (routine_active->second->routine_name == _name) {
			routine_active->second->stop(routine_active->first);
			routine_active = m_Routine_Movement_Sprite_Active.erase(routine_active);
		}
		else
			++routine_active;
	}

	for (auto routine_active = m_Routine_Movement_VertexArray_Active.begin(); routine_active != m_Routine_Movement_VertexArray_Active.end();) {
		if (routine_active->second->routine_name == _name) {
			routine_active->second->stop(routine_active->first);
			routine_active = m_Routine_Movement_VertexArray_Active.erase(routine_active);
		}
		else
			++routine_active;
	}

	for (auto routineContainer = m_Routine_Movement_Shape.begin(); routineContainer != m_Routine_Movement_Shape.end();) {
		routineContainer->second->deleteRoutine(_name);
		if (routineContainer->second->routineCount() == 0) {
			delete routineContainer->second;
			routineContainer = m_Routine_Movement_Shape.erase(routineContainer);
		}
		else
			++routineContainer;
	}

	for (auto routineContainer = m_Routine_Movement_Sprite.begin(); routineContainer != m_Routine_Movement_Sprite.end();) {
		routineContainer->second->deleteRoutine(_name);
		if (routineContainer->second->routineCount() == 0) {
			delete routineContainer->second;
			routineContainer = m_Routine_Movement_Sprite.erase(routineContainer);
		}
		else
			++routineContainer;
	}

	for (auto routineContainer = m_Routine_Movement_VertexArray.begin(); routineContainer != m_Routine_Movement_VertexArray.end();) {
		routineContainer->second->deleteRoutine(_name);
		if (routineContainer->second->routineCount() == 0) {
			delete routineContainer->second;
			routineContainer = m_Routine_Movement_VertexArray.erase(routineContainer);
		}
		else
			++routineContainer;
	}

	movementRoutineContainer->deleteRoutine(_name);
	movementRoutineVAContainer->deleteRoutine(_name);
}

void MovementManager::update(float dt)
{
	this->movementRoutineEngine->update(dt);
}