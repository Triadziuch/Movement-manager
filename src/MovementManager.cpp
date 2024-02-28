#pragma once
#include "MovementManager.h"

// Singleton initialization
MovementManager* MovementManager::sInstance = nullptr;

MovementManager::MovementManager()
{
	assert(this->sInstance == nullptr);
	sInstance = this;

	movementRoutineEngine = new MovementRoutineEngine(this);

	movementRoutineContainer = new MovementRoutineContainer();
	movementRoutineVAContainer = new MovementRoutineVAContainer();

	scalingRoutineContainer = new ScalingRoutineContainer();
	scalingRoutineVAContainer = new ScalingRoutineVAContainer();

	rotationRoutineContainer = new RotationRoutineContainer();
	rotationRoutineVAContainer = new RotationRoutineVAContainer();
}

MovementManager::~MovementManager()
{
	this->deleteMovementRoutine();
	this->deleteScalingRoutine();

	delete movementRoutineContainer;
	delete movementRoutineVAContainer;

	delete scalingRoutineContainer;
	delete scalingRoutineVAContainer;

	delete rotationRoutineContainer;
	delete rotationRoutineVAContainer;

	delete movementRoutineEngine;
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

ScalingRoutine* MovementManager::linkScalingRoutine(sf::Shape* _shape, const std::string& _name)
{
	// Check if routine with given name exists
	ScalingRoutine* scalingRoutineOriginal = scalingRoutineContainer->getRoutinePtr(_name);

	if (scalingRoutineOriginal == nullptr) {
		printf("MovementManager::linkScalingRoutine: Routine with name %s not found\n", _name.c_str());
		return nullptr;
	}

	// Check if we linked a scaling routine to this shape before
	auto scalingRoutineContainerFound = m_Routine_Scaling_Shape.find(_shape);
	if (scalingRoutineContainerFound != m_Routine_Scaling_Shape.end()) {

		auto* scalingRoutineFound = scalingRoutineContainerFound->second->exists(_name);
		if (scalingRoutineFound != nullptr) {
			printf("MovementManager::linkScalingRoutine: Routine with name %s already linked to shape\n", _name.c_str());
			return scalingRoutineFound;
		}
	}
	else {
		// If not, create a new scaling routine container for this shape and link the scaling routine to it
		ScalingRoutineContainer* newScalingRoutineContainer = new ScalingRoutineContainer(this->movementRoutineEngine);
		m_Routine_Scaling_Shape.insert(std::make_pair(_shape, newScalingRoutineContainer));

		ScalingRoutine* newScalingRoutine = new ScalingRoutine(*scalingRoutineOriginal);
		return newScalingRoutineContainer->createRoutine(_name, newScalingRoutine);
	}

	// If yes, create a new scaling routine and link it to the existing scaling routine container
	ScalingRoutine* newScalingRoutine = new ScalingRoutine(*scalingRoutineOriginal);
	return scalingRoutineContainerFound->second->createRoutine(_name, newScalingRoutine);
}

ScalingRoutine* MovementManager::linkScalingRoutine(sf::Sprite* _sprite, const std::string& _name)
{
	// Check if routine with given name exists
	ScalingRoutine* scalingRoutineOriginal = scalingRoutineContainer->getRoutinePtr(_name);

	if (scalingRoutineOriginal == nullptr) {
		printf("MovementManager::linkScalingRoutine: Routine with name %s not found\n", _name.c_str());
		return nullptr;
	}

	// Check if we linked a scaling routine to this shape before
	auto scalingRoutineContainerFound = m_Routine_Scaling_Sprite.find(_sprite);
	if (scalingRoutineContainerFound != m_Routine_Scaling_Sprite.end()) {

		auto* scalingRoutineFound = scalingRoutineContainerFound->second->exists(_name);
		if (scalingRoutineFound != nullptr) {
			printf("MovementManager::linkScalingRoutine: Routine with name %s already linked to shape\n", _name.c_str());
			return scalingRoutineFound;
		}
	}
	else {
		// If not, create a new scaling routine container for this shape and link the scaling routine to it
		ScalingRoutineContainer* newScalingRoutineContainer = new ScalingRoutineContainer(this->movementRoutineEngine);
		m_Routine_Scaling_Sprite.insert(std::make_pair(_sprite, newScalingRoutineContainer));

		ScalingRoutine* newScalingRoutine = new ScalingRoutine(*scalingRoutineOriginal);
		return newScalingRoutineContainer->createRoutine(_name, newScalingRoutine);
	}

	// If yes, create a new scaling routine and link it to the existing scaling routine container
	ScalingRoutine* newScalingRoutine = new ScalingRoutine(*scalingRoutineOriginal);
	return scalingRoutineContainerFound->second->createRoutine(_name, newScalingRoutine);
}

ScalingRoutineVA* MovementManager::linkScalingRoutine(sf::VertexArray* _vertexarray, const std::string& _name)
{
	// Check if routine with given name exists
	ScalingRoutineVA* scalingRoutineVAOriginal = scalingRoutineVAContainer->getRoutinePtr(_name);

	if (scalingRoutineVAOriginal == nullptr) {
		printf("MovementManager::linkScalingRoutine: Routine with name %s not found\n", _name.c_str());
		return nullptr;
	}

	// Check if we linked a scaling routine to this vertexarray before
	auto scalingRoutineVAContainerFound = m_Routine_Scaling_VertexArray.find(_vertexarray);
	if (scalingRoutineVAContainerFound != m_Routine_Scaling_VertexArray.end()) {

		auto* scalingRoutineVAFound = scalingRoutineVAContainerFound->second->exists(_name);
		if (scalingRoutineVAFound != nullptr) {
			printf("MovementManager::linkScalingRoutine: Routine with name %s already linked to vertexarray\n", _name.c_str());
			return scalingRoutineVAFound;
		}
	}
	else {
		// If not, create a new scaling routine container for this vertexarray and link the scaling routine to it
		ScalingRoutineVAContainer* newScalingRoutineVAContainer = new ScalingRoutineVAContainer(this->movementRoutineEngine);
		m_Routine_Scaling_VertexArray.insert(std::make_pair(_vertexarray, newScalingRoutineVAContainer));

		ScalingRoutineVA* newScalingRoutineVA = new ScalingRoutineVA(*scalingRoutineVAOriginal);
		return newScalingRoutineVAContainer->createRoutine(_name, newScalingRoutineVA);
	}

	// If yes, create a new scaling routine and link it to the existing scaling routine container
	ScalingRoutineVA* newScalingRoutineVA = new ScalingRoutineVA(*scalingRoutineVAOriginal);
	return scalingRoutineVAContainerFound->second->createRoutine(_name, newScalingRoutineVA);
}

void MovementManager::unlinkScalingRoutine(sf::Shape* _shape, const std::string& _name)
{
	// Stop the routine if it's active
	this->stopScalingRoutine(_shape, _name);

	// Delete the routine from m_Routine_Scaling_Shape
	auto scalingRoutineContainerFound = m_Routine_Scaling_Shape.find(_shape);
	if (scalingRoutineContainerFound != m_Routine_Scaling_Shape.end()) {

		auto* scalingRoutineFound = scalingRoutineContainerFound->second->exists(_name);
		if (scalingRoutineFound != nullptr) {
			scalingRoutineContainerFound->second->deleteRoutine(_name);
		}
		else
			printf("MovementManager::unlinkScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::unlinkScalingRoutine: Routine for shape not found\n");
}

void MovementManager::unlinkScalingRoutine(sf::Sprite* _sprite, const std::string& _name)
{
	// Stop the routine if it's active
	this->stopScalingRoutine(_sprite, _name);

	// Delete the routine from m_Routine_Scaling_Sprite
	auto scalingRoutineContainerFound = m_Routine_Scaling_Sprite.find(_sprite);
	if (scalingRoutineContainerFound != m_Routine_Scaling_Sprite.end()) {

		auto* scalingRoutineFound = scalingRoutineContainerFound->second->exists(_name);
		if (scalingRoutineFound != nullptr) {
			scalingRoutineContainerFound->second->deleteRoutine(_name);
		}
		else
			printf("MovementManager::unlinkScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::unlinkScalingRoutine: Routine for sprite not found\n");
}

void MovementManager::unlinkScalingRoutine(sf::VertexArray* _vertexarray, const std::string& _name)
{
	// Stop the routine if it's active
	this->stopScalingRoutine(_vertexarray, _name);

	// Delete the routine from m_Routine_Scaling_VertexArray
	auto scalingRoutineContainerFound = m_Routine_Scaling_VertexArray.find(_vertexarray);
	if (scalingRoutineContainerFound != m_Routine_Scaling_VertexArray.end()) {

		auto* scalingRoutineFound = scalingRoutineContainerFound->second->exists(_name);
		if (scalingRoutineFound != nullptr) {
			scalingRoutineContainerFound->second->deleteRoutine(_name);
		}
		else
			printf("MovementManager::unlinkScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::unlinkScalingRoutine: Routine for vertexarray not found\n");
}

void MovementManager::startScalingRoutine(sf::Shape* _shape, const std::string& _name)
{
	auto scalingRoutineContainerFound = m_Routine_Scaling_Shape.find(_shape);
	if (scalingRoutineContainerFound != m_Routine_Scaling_Shape.end()) {

		auto* scalingRoutineFound = scalingRoutineContainerFound->second->exists(_name);
		if (scalingRoutineFound != nullptr) {

			if (scalingRoutineFound->start(_shape))
				m_Routine_Scaling_Shape_Active.insert(std::make_pair(_shape, scalingRoutineFound));
		}
		else
			printf("MovementManager::startScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::startScalingRoutine: Routine for shape not found\n");
}

void MovementManager::startScalingRoutine(sf::Sprite* _sprite, const std::string& _name)
{
	auto scalingRoutineContainerFound = m_Routine_Scaling_Sprite.find(_sprite);
	if (scalingRoutineContainerFound != m_Routine_Scaling_Sprite.end()) {

		auto* scalingRoutineFound = scalingRoutineContainerFound->second->exists(_name);
		if (scalingRoutineFound != nullptr) {

			if (scalingRoutineFound->start(_sprite))
				m_Routine_Scaling_Sprite_Active.insert(std::make_pair(_sprite, scalingRoutineFound));
		}
		else
			printf("MovementManager::startScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::startScalingRoutine: Routine for sprite not found\n");
}

void MovementManager::startScalingRoutine(sf::VertexArray* _vertexarray, const std::string& _name)
{
	auto scalingRoutineContainerFound = m_Routine_Scaling_VertexArray.find(_vertexarray);
	if (scalingRoutineContainerFound != m_Routine_Scaling_VertexArray.end()) {

		auto* scalingRoutineFound = scalingRoutineContainerFound->second->exists(_name);
		if (scalingRoutineFound != nullptr) {

			if (scalingRoutineFound->start(_vertexarray)) {
				m_Routine_Scaling_VertexArray_Active.insert(std::make_pair(_vertexarray, scalingRoutineFound));

				/*auto rotationRoutineFound = m_Routine_Scaling_VertexArray_Active.find(_vertexarray);
				if (rotationRoutineFound != m_Routine_Scaling_VertexArray_Active.end())
					scalingRoutineFound->updateRotationInfoVA(*_vertexarray);*/
			}
		}
		else
			printf("MovementManager::startScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::startScalingRoutine: Routine for vertexarray not found\n");
}

void MovementManager::pauseScalingRoutine(sf::Shape* _shape, const std::string& _name)
{
	auto scalingRoutineFound = m_Routine_Scaling_Shape_Active.find(_shape);
	if (scalingRoutineFound != m_Routine_Scaling_Shape_Active.end()) {
		if (scalingRoutineFound->second->routine_name == _name) 
			scalingRoutineFound->second->pause();
		else
			printf("MovementManager::pauseScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::pauseScalingRoutine: Routine for shape not found\n");
}

void MovementManager::pauseScalingRoutine(sf::Sprite* _sprite, const std::string& _name)
{
	auto scalingRoutineFound = m_Routine_Scaling_Sprite_Active.find(_sprite);
	if (scalingRoutineFound != m_Routine_Scaling_Sprite_Active.end()) {
		if (scalingRoutineFound->second->routine_name == _name) 
			scalingRoutineFound->second->pause();
		else
			printf("MovementManager::pauseScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::pauseScalingRoutine: Routine for sprite not found\n");
}

void MovementManager::pauseScalingRoutine(sf::VertexArray* _vertexarray, const std::string& _name)
{
	auto scalingRoutineFound = m_Routine_Scaling_VertexArray_Active.find(_vertexarray);
	if (scalingRoutineFound != m_Routine_Scaling_VertexArray_Active.end()) {
		if (scalingRoutineFound->second->routine_name == _name) 
			scalingRoutineFound->second->pause();
		else
			printf("MovementManager::pauseScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::pauseScalingRoutine: Routine for vertexarray not found\n");
}

void MovementManager::resumeScalingRoutine(sf::Shape* _shape, const std::string& _name)
{
	auto scalingRoutineFound = m_Routine_Scaling_Shape_Active.find(_shape);
	if (scalingRoutineFound != m_Routine_Scaling_Shape_Active.end()) {
		if (scalingRoutineFound->second->routine_name == _name) 
			scalingRoutineFound->second->resume();
		else
			printf("MovementManager::resumeScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::resumeScalingRoutine: Routine for shape not found\n");
}

void MovementManager::resumeScalingRoutine(sf::Sprite* _sprite, const std::string& _name)
{
	auto scalingRoutineFound = m_Routine_Scaling_Sprite_Active.find(_sprite);
	if (scalingRoutineFound != m_Routine_Scaling_Sprite_Active.end()) {
		if (scalingRoutineFound->second->routine_name == _name) 
			scalingRoutineFound->second->resume();
		else
			printf("MovementManager::resumeScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::resumeScalingRoutine: Routine for sprite not found\n");
}

void MovementManager::resumeScalingRoutine(sf::VertexArray* _vertexarray, const std::string& _name)
{
	auto scalingRoutineFound = m_Routine_Scaling_VertexArray_Active.find(_vertexarray);
	if (scalingRoutineFound != m_Routine_Scaling_VertexArray_Active.end()) {
		if (scalingRoutineFound->second->routine_name == _name) 
			scalingRoutineFound->second->resume();
		else
			printf("MovementManager::resumeScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::resumeScalingRoutine: Routine for vertexarray not found\n");
}

void MovementManager::stopScalingRoutine(sf::Shape* _shape, const std::string& _name)
{
	auto scalingRoutineFound = m_Routine_Scaling_Shape_Active.find(_shape);
	if (scalingRoutineFound != m_Routine_Scaling_Shape_Active.end()) {
		if (scalingRoutineFound->second->routine_name == _name) {
			scalingRoutineFound->second->stop(_shape);
			m_Routine_Scaling_Shape_Active.erase(scalingRoutineFound);
		}
		else
			printf("MovementManager::stopScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::stopScalingRoutine: Routine for shape not found\n");
}

void MovementManager::stopScalingRoutine(sf::Sprite* _sprite, const std::string& _name)
{
	auto scalingRoutineFound = m_Routine_Scaling_Sprite_Active.find(_sprite);
	if (scalingRoutineFound != m_Routine_Scaling_Sprite_Active.end()) {
		if (scalingRoutineFound->second->routine_name == _name) {
			scalingRoutineFound->second->stop(_sprite);
			m_Routine_Scaling_Sprite_Active.erase(scalingRoutineFound);
		}
		else
			printf("MovementManager::stopScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::stopScalingRoutine: Routine for sprite not found\n");
}

void MovementManager::stopScalingRoutine(sf::VertexArray* _vertexarray, const std::string& _name)
{
	auto scalingRoutineFound = m_Routine_Scaling_VertexArray_Active.find(_vertexarray);
	if (scalingRoutineFound != m_Routine_Scaling_VertexArray_Active.end()) {
		if (scalingRoutineFound->second->routine_name == _name) {
			scalingRoutineFound->second->stop(_vertexarray);
			m_Routine_Scaling_VertexArray_Active.erase(scalingRoutineFound);
		}
		else
			printf("MovementManager::stopScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::stopScalingRoutine: Routine for vertexarray not found\n");
}

void MovementManager::deleteScalingRoutine()
{
	for (auto routine_active = m_Routine_Scaling_Shape_Active.begin(); routine_active != m_Routine_Scaling_Shape_Active.end();) {
		routine_active->second->stop(routine_active->first);
		routine_active = m_Routine_Scaling_Shape_Active.erase(routine_active);
	}

	for (auto routine_active = m_Routine_Scaling_Sprite_Active.begin(); routine_active != m_Routine_Scaling_Sprite_Active.end();) {
		routine_active->second->stop(routine_active->first);
		routine_active = m_Routine_Scaling_Sprite_Active.erase(routine_active);
	}

	for (auto routine_active = m_Routine_Scaling_VertexArray_Active.begin(); routine_active != m_Routine_Scaling_VertexArray_Active.end();) {
		routine_active->second->stop(routine_active->first);
		routine_active = m_Routine_Scaling_VertexArray_Active.erase(routine_active);
	}

	for (auto routineContainer = m_Routine_Scaling_Shape.begin(); routineContainer != m_Routine_Scaling_Shape.end();) {
		routineContainer->second->clear();
		delete routineContainer->second;
		routineContainer = m_Routine_Scaling_Shape.erase(routineContainer);
	}

	for (auto routineContainer = m_Routine_Scaling_Sprite.begin(); routineContainer != m_Routine_Scaling_Sprite.end();) {
		routineContainer->second->clear();
		delete routineContainer->second;
		routineContainer = m_Routine_Scaling_Sprite.erase(routineContainer);
	}

	for (auto routineContainer = m_Routine_Scaling_VertexArray.begin(); routineContainer != m_Routine_Scaling_VertexArray.end();) {
		routineContainer->second->clear();
		delete routineContainer->second;
		routineContainer = m_Routine_Scaling_VertexArray.erase(routineContainer);
	}

	scalingRoutineContainer->clear();
	scalingRoutineVAContainer->clear();
}

void MovementManager::deleteScalingRoutine(const std::string& _name)
{
	for (auto routine_active = m_Routine_Scaling_Shape_Active.begin(); routine_active != m_Routine_Scaling_Shape_Active.end();) {
		if (routine_active->second->routine_name == _name) {
			routine_active->second->stop(routine_active->first);
			routine_active = m_Routine_Scaling_Shape_Active.erase(routine_active);
		}
		else
			++routine_active;
	}

	for (auto routine_active = m_Routine_Scaling_Sprite_Active.begin(); routine_active != m_Routine_Scaling_Sprite_Active.end();) {
		if (routine_active->second->routine_name == _name) {
			routine_active->second->stop(routine_active->first);
			routine_active = m_Routine_Scaling_Sprite_Active.erase(routine_active);
		}
		else
			++routine_active;
	}

	for (auto routine_active = m_Routine_Scaling_VertexArray_Active.begin(); routine_active != m_Routine_Scaling_VertexArray_Active.end();) {
		if (routine_active->second->routine_name == _name) {
			routine_active->second->stop(routine_active->first);
			routine_active = m_Routine_Scaling_VertexArray_Active.erase(routine_active);
		}
		else
			++routine_active;
	}

	for (auto routineContainer = m_Routine_Scaling_Shape.begin(); routineContainer != m_Routine_Scaling_Shape.end();) {
		routineContainer->second->deleteRoutine(_name);
		if (routineContainer->second->routineCount() == 0) {
			delete routineContainer->second;
			routineContainer = m_Routine_Scaling_Shape.erase(routineContainer);
		}
		else
			++routineContainer;
	}

	for (auto routineContainer = m_Routine_Scaling_Sprite.begin(); routineContainer != m_Routine_Scaling_Sprite.end();) {
		routineContainer->second->deleteRoutine(_name);
		if (routineContainer->second->routineCount() == 0) {
			delete routineContainer->second;
			routineContainer = m_Routine_Scaling_Sprite.erase(routineContainer);
		}
		else
			++routineContainer;
	}

	for (auto routineContainer = m_Routine_Scaling_VertexArray.begin(); routineContainer != m_Routine_Scaling_VertexArray.end();) {
		routineContainer->second->deleteRoutine(_name);
		if (routineContainer->second->routineCount() == 0) {
			delete routineContainer->second;
			routineContainer = m_Routine_Scaling_VertexArray.erase(routineContainer);
		}
		else
			++routineContainer;
	}

	scalingRoutineContainer->deleteRoutine(_name);
	scalingRoutineVAContainer->deleteRoutine(_name);
}

RotationRoutine* MovementManager::linkRotationRoutine(sf::Shape* _shape, const std::string& _name)
{
	// Check if routine with given name exists
	RotationRoutine* rotationRoutineOriginal = rotationRoutineContainer->getRoutinePtr(_name);

	if (rotationRoutineOriginal == nullptr) {
		printf("MovementManager::linkRotationRoutine: Routine with name %s not found\n", _name.c_str());
		return nullptr;
	}

	// Check if we linked a rotation routine to this shape before
	auto rotationRoutineContainerFound = m_Routine_Rotation_Shape.find(_shape);
	if (rotationRoutineContainerFound != m_Routine_Rotation_Shape.end()) {

		auto* rotationRoutineFound = rotationRoutineContainerFound->second->exists(_name);
		if (rotationRoutineFound != nullptr) {
			printf("MovementManager::linkRotationRoutine: Routine with name %s already linked to shape\n", _name.c_str());
			return rotationRoutineFound;
		}
	}
	else {
		// If not, create a new rotation routine container for this shape and link the rotation routine to it
		RotationRoutineContainer* newRotationRoutineContainer = new RotationRoutineContainer(this->movementRoutineEngine);
		m_Routine_Rotation_Shape.insert(std::make_pair(_shape, newRotationRoutineContainer));

		RotationRoutine* newRotationRoutine = new RotationRoutine(*rotationRoutineOriginal);
		return newRotationRoutineContainer->createRoutine(_name, newRotationRoutine);
	}

	// If yes, create a new rotation routine and link it to the existing rotation routine container
	RotationRoutine* newRotationRoutine = new RotationRoutine(*rotationRoutineOriginal);
	return rotationRoutineContainerFound->second->createRoutine(_name, newRotationRoutine);
}

RotationRoutine* MovementManager::linkRotationRoutine(sf::Sprite* _sprite, const std::string& _name)
{
	// Check if routine with given name exists
	RotationRoutine* rotationRoutineOriginal = rotationRoutineContainer->getRoutinePtr(_name);

	if (rotationRoutineOriginal == nullptr) {
		printf("MovementManager::linkRotationRoutine: Routine with name %s not found\n", _name.c_str());
		return nullptr;
	}

	// Check if we linked a rotation routine to this shape before
	auto rotationRoutineContainerFound = m_Routine_Rotation_Sprite.find(_sprite);
	if (rotationRoutineContainerFound != m_Routine_Rotation_Sprite.end()) {

		auto* rotationRoutineFound = rotationRoutineContainerFound->second->exists(_name);
		if (rotationRoutineFound != nullptr) {
			printf("MovementManager::linkRotationRoutine: Routine with name %s already linked to shape\n", _name.c_str());
			return rotationRoutineFound;
		}
	}
	else {
		// If not, create a new rotation routine container for this shape and link the rotation routine to it
		RotationRoutineContainer* newRotationRoutineContainer = new RotationRoutineContainer(this->movementRoutineEngine);
		m_Routine_Rotation_Sprite.insert(std::make_pair(_sprite, newRotationRoutineContainer));

		RotationRoutine* newRotationRoutine = new RotationRoutine(*rotationRoutineOriginal);
		return newRotationRoutineContainer->createRoutine(_name, newRotationRoutine);
	}

	// If yes, create a new rotation routine and link it to the existing rotation routine container
	RotationRoutine* newRotationRoutine = new RotationRoutine(*rotationRoutineOriginal);
	return rotationRoutineContainerFound->second->createRoutine(_name, newRotationRoutine);
}

RotationRoutineVA* MovementManager::linkRotationRoutine(sf::VertexArray* _vertexarray, const std::string& _name)
{
	// Check if routine with given name exists
	RotationRoutineVA* rotationRoutineVAOriginal = rotationRoutineVAContainer->getRoutinePtr(_name);

	if (rotationRoutineVAOriginal == nullptr) {
		printf("MovementManager::linkRotationRoutine: Routine with name %s not found\n", _name.c_str());
		return nullptr;
	}

	// Check if we linked a rotation routine to this vertexarray before
	auto rotationRoutineVAContainerFound = m_Routine_Rotation_VertexArray.find(_vertexarray);
	if (rotationRoutineVAContainerFound != m_Routine_Rotation_VertexArray.end()) {

		auto* rotationRoutineVAFound = rotationRoutineVAContainerFound->second->exists(_name);
		if (rotationRoutineVAFound != nullptr) {
			printf("MovementManager::linkRotationRoutine: Routine with name %s already linked to vertexarray\n", _name.c_str());
			return rotationRoutineVAFound;
		}
	}
	else {
		// If not, create a new rotation routine container for this vertexarray and link the rotation routine to it
		RotationRoutineVAContainer* newRotationRoutineVAContainer = new RotationRoutineVAContainer(this->movementRoutineEngine);
		m_Routine_Rotation_VertexArray.insert(std::make_pair(_vertexarray, newRotationRoutineVAContainer));

		RotationRoutineVA* newRotationRoutineVA = new RotationRoutineVA(*rotationRoutineVAOriginal);
		return newRotationRoutineVAContainer->createRoutine(_name, newRotationRoutineVA);
	}

	// If yes, create a new rotation routine and link it to the existing rotation routine container
	RotationRoutineVA* newRotationRoutineVA = new RotationRoutineVA(*rotationRoutineVAOriginal);
	return rotationRoutineVAContainerFound->second->createRoutine(_name, newRotationRoutineVA);
}

void MovementManager::unlinkRotationRoutine(sf::Shape* _shape, const std::string& _name)
{
	// Stop the routine if it's active
	this->stopRotationRoutine(_shape, _name);

	// Delete the routine from m_Routine_Rotation_Shape
	auto rotationRoutineContainerFound = m_Routine_Rotation_Shape.find(_shape);
	if (rotationRoutineContainerFound != m_Routine_Rotation_Shape.end()) {

		auto* rotationRoutineFound = rotationRoutineContainerFound->second->exists(_name);
		if (rotationRoutineFound != nullptr) {
			rotationRoutineContainerFound->second->deleteRoutine(_name);
		}
		else
			printf("MovementManager::unlinkRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::unlinkRotationRoutine: Routine for shape not found\n");
}

void MovementManager::unlinkRotationRoutine(sf::Sprite* _sprite, const std::string& _name)
{
	// Stop the routine if it's active
	this->stopRotationRoutine(_sprite, _name);

	// Delete the routine from m_Routine_Rotation_Sprite
	auto rotationRoutineContainerFound = m_Routine_Rotation_Sprite.find(_sprite);
	if (rotationRoutineContainerFound != m_Routine_Rotation_Sprite.end()) {

		auto* rotationRoutineFound = rotationRoutineContainerFound->second->exists(_name);
		if (rotationRoutineFound != nullptr) {
			rotationRoutineContainerFound->second->deleteRoutine(_name);
		}
		else
			printf("MovementManager::unlinkRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::unlinkRotationRoutine: Routine for sprite not found\n");
}

void MovementManager::unlinkRotationRoutine(sf::VertexArray* _vertexarray, const std::string& _name)
{
	// Stop the routine if it's active
	this->stopRotationRoutine(_vertexarray, _name);

	// Delete the routine from m_Routine_Rotation_VertexArray
	auto rotationRoutineContainerFound = m_Routine_Rotation_VertexArray.find(_vertexarray);
	if (rotationRoutineContainerFound != m_Routine_Rotation_VertexArray.end()) {

		auto* rotationRoutineFound = rotationRoutineContainerFound->second->exists(_name);
		if (rotationRoutineFound != nullptr) {
			rotationRoutineContainerFound->second->deleteRoutine(_name);
		}
		else
			printf("MovementManager::unlinkRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::unlinkRotationRoutine: Routine for vertexarray not found\n");
}

void MovementManager::startRotationRoutine(sf::Shape* _shape, const std::string& _name)
{
	auto rotationRoutineContainerFound = m_Routine_Rotation_Shape.find(_shape);
	if (rotationRoutineContainerFound != m_Routine_Rotation_Shape.end()) {

		auto* rotationRoutineFound = rotationRoutineContainerFound->second->exists(_name);
		if (rotationRoutineFound != nullptr) {

			if (rotationRoutineFound->start(_shape))
				m_Routine_Rotation_Shape_Active.insert(std::make_pair(_shape, rotationRoutineFound));
		}
		else
			printf("MovementManager::startRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::startRotationRoutine: Routine for shape not found\n");
}

void MovementManager::startRotationRoutine(sf::Sprite* _sprite, const std::string& _name)
{
	auto rotationRoutineContainerFound = m_Routine_Rotation_Sprite.find(_sprite);
	if (rotationRoutineContainerFound != m_Routine_Rotation_Sprite.end()) {

		auto* rotationRoutineFound = rotationRoutineContainerFound->second->exists(_name);
		if (rotationRoutineFound != nullptr) {

			if (rotationRoutineFound->start(_sprite))
				m_Routine_Rotation_Sprite_Active.insert(std::make_pair(_sprite, rotationRoutineFound));
		}
		else
			printf("MovementManager::startRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::startRotationRoutine: Routine for sprite not found\n");
}

void MovementManager::startRotationRoutine(sf::VertexArray* _vertexarray, const std::string& _name)
{
	auto rotationRoutineContainerFound = m_Routine_Rotation_VertexArray.find(_vertexarray);
	if (rotationRoutineContainerFound != m_Routine_Rotation_VertexArray.end()) {

		auto* rotationRoutineFound = rotationRoutineContainerFound->second->exists(_name);
		if (rotationRoutineFound != nullptr) {

			if (rotationRoutineFound->start(_vertexarray)) {
				m_Routine_Rotation_VertexArray_Active.insert(std::make_pair(_vertexarray, rotationRoutineFound));

				/*auto scalingRoutineFound = m_Routine_Scaling_VertexArray_Active.find(_vertexarray);
				if (scalingRoutineFound != m_Routine_Scaling_VertexArray_Active.end()) 
					rotationRoutineFound->updateScalingInfoVA(*_vertexarray);*/
			}
				
		}
		else
			printf("MovementManager::startRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::startRotationRoutine: Routine for vertexarray not found\n");
}

void MovementManager::pauseRotationRoutine(sf::Shape* _shape, const std::string& _name)
{
	auto rotationRoutineFound = m_Routine_Rotation_Shape_Active.find(_shape);
	if (rotationRoutineFound != m_Routine_Rotation_Shape_Active.end()) {
		if (rotationRoutineFound->second->routine_name == _name) 
			rotationRoutineFound->second->pause();
		else
			printf("MovementManager::pauseRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::pauseRotationRoutine: Routine for shape not found\n");
}

void MovementManager::pauseRotationRoutine(sf::Sprite* _sprite, const std::string& _name)
{
	auto rotationRoutineFound = m_Routine_Rotation_Sprite_Active.find(_sprite);
	if (rotationRoutineFound != m_Routine_Rotation_Sprite_Active.end()) {
		if (rotationRoutineFound->second->routine_name == _name) 
			rotationRoutineFound->second->pause();
		else
			printf("MovementManager::pauseRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::pauseRotationRoutine: Routine for sprite not found\n");
}

void MovementManager::pauseRotationRoutine(sf::VertexArray* _vertexarray, const std::string& _name)
{
	auto rotationRoutineFound = m_Routine_Rotation_VertexArray_Active.find(_vertexarray);
	if (rotationRoutineFound != m_Routine_Rotation_VertexArray_Active.end()) {
		if (rotationRoutineFound->second->routine_name == _name) 
			rotationRoutineFound->second->pause();
		else
			printf("MovementManager::pauseRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::pauseRotationRoutine: Routine for vertexarray not found\n");
}

void MovementManager::resumeRotationRoutine(sf::Shape* _shape, const std::string& _name)
{
	auto rotationRoutineFound = m_Routine_Rotation_Shape_Active.find(_shape);
	if (rotationRoutineFound != m_Routine_Rotation_Shape_Active.end()) {
		if (rotationRoutineFound->second->routine_name == _name) 
			rotationRoutineFound->second->resume();
		else
			printf("MovementManager::resumeRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::resumeRotationRoutine: Routine for shape not found\n");
}

void MovementManager::resumeRotationRoutine(sf::Sprite* _sprite, const std::string& _name)
{
	auto rotationRoutineFound = m_Routine_Rotation_Sprite_Active.find(_sprite);
	if (rotationRoutineFound != m_Routine_Rotation_Sprite_Active.end()) {
		if (rotationRoutineFound->second->routine_name == _name) 
			rotationRoutineFound->second->resume();
		else
			printf("MovementManager::resumeRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::resumeRotationRoutine: Routine for sprite not found\n");
}

void MovementManager::resumeRotationRoutine(sf::VertexArray* _vertexarray, const std::string& _name)
{
	auto rotationRoutineFound = m_Routine_Rotation_VertexArray_Active.find(_vertexarray);
	if (rotationRoutineFound != m_Routine_Rotation_VertexArray_Active.end()) {
		if (rotationRoutineFound->second->routine_name == _name) 
			rotationRoutineFound->second->resume();
		else
			printf("MovementManager::resumeRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::resumeRotationRoutine: Routine for vertexarray not found\n");
}

void MovementManager::stopRotationRoutine(sf::Shape* _shape, const std::string& _name)
{
	auto rotationRoutineFound = m_Routine_Rotation_Shape_Active.find(_shape);
	if (rotationRoutineFound != m_Routine_Rotation_Shape_Active.end()) {
		if (rotationRoutineFound->second->routine_name == _name) {
			rotationRoutineFound->second->stop(_shape);
			m_Routine_Rotation_Shape_Active.erase(rotationRoutineFound);
		}
		else
			printf("MovementManager::stopRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::stopRotationRoutine: Routine for shape not found\n");
}

void MovementManager::stopRotationRoutine(sf::Sprite* _sprite, const std::string& _name)
{
	auto rotationRoutineFound = m_Routine_Rotation_Sprite_Active.find(_sprite);
	if (rotationRoutineFound != m_Routine_Rotation_Sprite_Active.end()) {
		if (rotationRoutineFound->second->routine_name == _name) {
			rotationRoutineFound->second->stop(_sprite);
			m_Routine_Rotation_Sprite_Active.erase(rotationRoutineFound);
		}
		else
			printf("MovementManager::stopRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::stopRotationRoutine: Routine for sprite not found\n");
}

void MovementManager::stopRotationRoutine(sf::VertexArray* _vertexarray, const std::string& _name)
{
	auto rotationRoutineFound = m_Routine_Rotation_VertexArray_Active.find(_vertexarray);
	if (rotationRoutineFound != m_Routine_Rotation_VertexArray_Active.end()) {
		if (rotationRoutineFound->second->routine_name == _name) {
			rotationRoutineFound->second->stop(_vertexarray);
			m_Routine_Rotation_VertexArray_Active.erase(rotationRoutineFound);
		}
		else
			printf("MovementManager::stopRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::stopRotationRoutine: Routine for vertexarray not found\n");
}

void MovementManager::deleteRotationRoutine()
{
	for (auto routine_active = m_Routine_Rotation_Shape_Active.begin(); routine_active != m_Routine_Rotation_Shape_Active.end();) {
		routine_active->second->stop(routine_active->first);
		routine_active = m_Routine_Rotation_Shape_Active.erase(routine_active);
	}

	for (auto routine_active = m_Routine_Rotation_Sprite_Active.begin(); routine_active != m_Routine_Rotation_Sprite_Active.end();) {
		routine_active->second->stop(routine_active->first);
		routine_active = m_Routine_Rotation_Sprite_Active.erase(routine_active);
	}

	for (auto routine_active = m_Routine_Rotation_VertexArray_Active.begin(); routine_active != m_Routine_Rotation_VertexArray_Active.end();) {
		routine_active->second->stop(routine_active->first);
		routine_active = m_Routine_Rotation_VertexArray_Active.erase(routine_active);
	}

	for (auto routineContainer = m_Routine_Rotation_Shape.begin(); routineContainer != m_Routine_Rotation_Shape.end();) {
		routineContainer->second->clear();
		delete routineContainer->second;
		routineContainer = m_Routine_Rotation_Shape.erase(routineContainer);
	}

	for (auto routineContainer = m_Routine_Rotation_Sprite.begin(); routineContainer != m_Routine_Rotation_Sprite.end();) {
		routineContainer->second->clear();
		delete routineContainer->second;
		routineContainer = m_Routine_Rotation_Sprite.erase(routineContainer);
	}

	for (auto routineContainer = m_Routine_Rotation_VertexArray.begin(); routineContainer != m_Routine_Rotation_VertexArray.end();) {
		routineContainer->second->clear();
		delete routineContainer->second;
		routineContainer = m_Routine_Rotation_VertexArray.erase(routineContainer);
	}

	rotationRoutineContainer->clear();
	rotationRoutineVAContainer->clear();
}

void MovementManager::deleteRotationRoutine(const std::string& _name)
{
for (auto routine_active = m_Routine_Rotation_Shape_Active.begin(); routine_active != m_Routine_Rotation_Shape_Active.end();) {
		if (routine_active->second->routine_name == _name) {
			routine_active->second->stop(routine_active->first);
			routine_active = m_Routine_Rotation_Shape_Active.erase(routine_active);
		}
		else
			++routine_active;
	}

	for (auto routine_active = m_Routine_Rotation_Sprite_Active.begin(); routine_active != m_Routine_Rotation_Sprite_Active.end();) {
		if (routine_active->second->routine_name == _name) {
			routine_active->second->stop(routine_active->first);
			routine_active = m_Routine_Rotation_Sprite_Active.erase(routine_active);
		}
		else
			++routine_active;
	}

	for (auto routine_active = m_Routine_Rotation_VertexArray_Active.begin(); routine_active != m_Routine_Rotation_VertexArray_Active.end();) {
		if (routine_active->second->routine_name == _name) {
			routine_active->second->stop(routine_active->first);
			routine_active = m_Routine_Rotation_VertexArray_Active.erase(routine_active);
		}
		else
			++routine_active;
	}

	for (auto routineContainer = m_Routine_Rotation_Shape.begin(); routineContainer != m_Routine_Rotation_Shape.end();) {
		routineContainer->second->deleteRoutine(_name);
		if (routineContainer->second->routineCount() == 0) {
			delete routineContainer->second;
			routineContainer = m_Routine_Rotation_Shape.erase(routineContainer);
		}
		else
			++routineContainer;
	}

	for (auto routineContainer = m_Routine_Rotation_Sprite.begin(); routineContainer != m_Routine_Rotation_Sprite.end();) {
		routineContainer->second->deleteRoutine(_name);
		if (routineContainer->second->routineCount() == 0) {
			delete routineContainer->second;
			routineContainer = m_Routine_Rotation_Sprite.erase(routineContainer);
		}
		else
			++routineContainer;
	}

	for (auto routineContainer = m_Routine_Rotation_VertexArray.begin(); routineContainer != m_Routine_Rotation_VertexArray.end();) {
		routineContainer->second->deleteRoutine(_name);
		if (routineContainer->second->routineCount() == 0) {
			delete routineContainer->second;
			routineContainer = m_Routine_Rotation_VertexArray.erase(routineContainer);
		}
		else
			++routineContainer;
	}
}
