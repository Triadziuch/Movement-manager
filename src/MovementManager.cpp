#pragma once
#include "MovementManager.h"

// Singleton initialization
MovementManager* MovementManager::sInstance = nullptr;

// Constructors / Destructors
MovementManager::MovementManager()
{
	movementRoutineEngine = MovementRoutineEngine::getInstance();
	movementRoutineEngine->setMovementManager(this);

	m_movementRoutineContainer = new MovementRoutineContainer();
	m_scalingRoutineContainer = new ScalingRoutineContainer();
	m_rotationRoutineContainer = new RotationRoutineContainer();
}

MovementManager::~MovementManager()
{
	this->deleteMovementRoutine();
	this->deleteScalingRoutine();
	this->deleteRotationRoutine();

	delete m_movementRoutineContainer;
}

MovementManager* MovementManager::getInstance()
{
	if (!sInstance) 
		sInstance = new MovementManager();
	return sInstance;
}

// Update functions
void MovementManager::update(const float dt)
{
	this->movementRoutineEngine->update(dt);
}

void MovementManager::setFunction(sf::Transformable& transformable, easeFunctions::Tmovement_function usedFunctionType)
{
	auto movementRoutineFound = m_routineMovementActive.find(&transformable);
	if (movementRoutineFound != m_routineMovementActive.end())
		movementRoutineFound->second->setFunction(usedFunctionType);

	auto scalingRoutineFound = m_routineScaling_Active.find(&transformable);
	if (scalingRoutineFound != m_routineScaling_Active.end())
		scalingRoutineFound->second->setFunction(usedFunctionType);

	auto rotationRoutineFound = m_routineRotation_Active.find(&transformable);
	if (rotationRoutineFound != m_routineRotation_Active.end())
		rotationRoutineFound->second->setFunction(usedFunctionType);
}

void MovementManager::resetRoutines(sf::Transformable& transformable)
{
	auto movementRoutineFound = m_routineMovementActive.find(&transformable);
	if (movementRoutineFound != m_routineMovementActive.end())
		movementRoutineFound->second->reset(transformable);

	auto scalingRoutineFound = m_routineScaling_Active.find(&transformable);
	if (scalingRoutineFound != m_routineScaling_Active.end())
		scalingRoutineFound->second->reset(transformable);

	auto rotationRoutineFound = m_routineRotation_Active.find(&transformable);
	if (rotationRoutineFound != m_routineRotation_Active.end())
		rotationRoutineFound->second->reset(transformable);
}

// - - - - - - - - - - - - - - - - - - - - Movement  - - - - - - - - - - - - - - - - - - - - \\

MovementRoutine* MovementManager::createMovementRoutine(const std::string& _name)
{
	return this->m_movementRoutineContainer->createRoutine(_name);
}

MovementRoutine* MovementManager::getMovementRoutine(const std::string& _name)
{
	return this->m_movementRoutineContainer->getRoutinePtr(_name);
}

MovementRoutine* MovementManager::getMovementRoutine(sf::Transformable& transformable, const std::string& _name)
{
	auto m_movementRoutineContainerFound = m_routineMovement.find(&transformable);
	if (m_movementRoutineContainerFound != m_routineMovement.end()) {
		return m_movementRoutineContainerFound->second->exists(_name);
	}
	else {
		printf("MovementManager::getMovementRoutine: Routine for shape not found\n");
		return nullptr;
	}
}

MovementRoutine* MovementManager::linkMovementRoutine(sf::Transformable& transformable, const std::string& _name)
{
	// Check if routine with given name exists
	MovementRoutine* movementRoutineOriginal = m_movementRoutineContainer->getRoutinePtr(_name);

	if (movementRoutineOriginal == nullptr) {
		printf("MovementManager::linkMovementRoutine: Routine with name %s not found\n", _name.c_str());
		return nullptr;
	}

	// Check if we linked a movement routine to this shape before
	auto m_movementRoutineContainerFound = m_routineMovement.find(&transformable);
	if (m_movementRoutineContainerFound != m_routineMovement.end()){

		auto* movementRoutineFound = m_movementRoutineContainerFound->second->exists(_name);
		if (movementRoutineFound != nullptr){
			printf("MovementManager::linkMovementRoutine: Routine with name %s already linked to shape\n", _name.c_str());
			return movementRoutineFound;
		}
	}
	else {
		// If not, create a new movement routine container for this shape and link the movement routine to it
		MovementRoutineContainer* newMovementRoutineContainer = new MovementRoutineContainer(this->movementRoutineEngine);
		m_routineMovement.insert(std::make_pair(&transformable, newMovementRoutineContainer));

		MovementRoutine* newMovementRoutine = new MovementRoutine(*movementRoutineOriginal);
		return newMovementRoutineContainer->createRoutine(_name, newMovementRoutine);
	}

	// If yes, create a new movement routine and link it to the existing movement routine container
	MovementRoutine* newMovementRoutine = new MovementRoutine(*movementRoutineOriginal);
	return m_movementRoutineContainerFound->second->createRoutine(_name, newMovementRoutine);
}

MovementRoutine* MovementManager::linkMovementRoutine(sf::Transformable* transformable, MovementRoutine* routine)
{
	MovementRoutine* movementRoutineFound = m_movementRoutineContainer->exists(routine);

	if (movementRoutineFound == nullptr) {
		printf("MovementManager::linkMovementRoutine: Routine not found\n");
		return nullptr;
	}
	else {
		// Check if we linked a movement routine to this shape before
		auto m_movementRoutineContainerFound = m_routineMovement.find(transformable);
		if (m_movementRoutineContainerFound != m_routineMovement.end()) {

			auto* movementRoutineFound = m_movementRoutineContainerFound->second->exists(routine);
			if (movementRoutineFound != nullptr) {
				printf("MovementManager::linkMovementRoutine: Routine already linked to shape\n");
				return movementRoutineFound;
			}
		}
		else {
			// If not, create a new movement routine container for this shape and link the movement routine to it
			MovementRoutineContainer* newMovementRoutineContainer = new MovementRoutineContainer(this->movementRoutineEngine);
			m_routineMovement.insert(std::make_pair(transformable, newMovementRoutineContainer));

			return newMovementRoutineContainer->createRoutine(movementRoutineFound->getName(), new MovementRoutine(*routine));
		}

		// If yes, create a new movement routine and link it to the existing movement routine container
		return m_movementRoutineContainerFound->second->createRoutine(movementRoutineFound->getName(), new MovementRoutine(*routine));
	}
}

void MovementManager::unlinkMovementRoutine(sf::Transformable* transformable, const std::string& _name)
{
	// Stop the routine if it's active
	this->stopMovementRoutine(transformable, _name);

	// Delete the routine from m_routineMovement
	auto m_movementRoutineContainerFound = m_routineMovement.find(transformable);
	if (m_movementRoutineContainerFound != m_routineMovement.end()) {

		auto* movementRoutineFound = m_movementRoutineContainerFound->second->exists(_name);
		if (movementRoutineFound != nullptr) 
			m_movementRoutineContainerFound->second->deleteRoutine(_name);
		else
			printf("MovementManager::unlinkMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::unlinkMovementRoutine: Routine for shape not found\n");
}

void MovementManager::startMovementRoutine(sf::Transformable& transformable, const std::string& _name)
{
	auto m_movementRoutineContainerFound = m_routineMovement.find(&transformable);
	if (m_movementRoutineContainerFound != m_routineMovement.end()) {

		auto* movementRoutineFound = m_movementRoutineContainerFound->second->exists(_name);
		if (movementRoutineFound != nullptr) {
			if (movementRoutineFound->start(transformable))
				m_routineMovementActive.insert(std::make_pair(&transformable, movementRoutineFound));
		}
		else
			printf("MovementManager::startMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::startMovementRoutine: Routine for shape not found\n");
}

void MovementManager::pauseMovementRoutine(sf::Transformable& transformable, const std::string& _name)
{
	auto movementRoutineFound = m_routineMovementActive.find(&transformable);
	if (movementRoutineFound != m_routineMovementActive.end()) {
		if (movementRoutineFound->second->getName() == _name) 
			movementRoutineFound->second->pause();
		else
			printf("MovementManager::pauseMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::pauseMovementRoutine: Routine for shape not found\n");
}

void MovementManager::resumeMovementRoutine(sf::Transformable& transformable, const std::string& _name)
{
	auto movementRoutineFound = m_routineMovementActive.find(&transformable);
	if (movementRoutineFound != m_routineMovementActive.end()) {
		if (movementRoutineFound->second->getName() == _name) 
			movementRoutineFound->second->resume();
		else
			printf("MovementManager::resumeMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::resumeMovementRoutine: Routine for shape not found\n");
}

void MovementManager::resetMovementRoutine(sf::Transformable& transformable, const std::string& _name)
{
	auto movementRoutineFound = m_routineMovementActive.find(&transformable);
	if (movementRoutineFound != m_routineMovementActive.end()) {
		if (movementRoutineFound->second->getName() == _name) 
			movementRoutineFound->second->reset(transformable);
		else
			printf("MovementManager::resetMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::resetMovementRoutine: Routine for shape not found\n");
}

void MovementManager::stopMovementRoutine(sf::Transformable* transformable, const std::string& _name)
{
	auto movementRoutineFound = m_routineMovementActive.find(transformable);
	if (movementRoutineFound != m_routineMovementActive.end()) {
		if (movementRoutineFound->second->getName() == _name) {
			movementRoutineFound->second->stop(transformable);
			m_routineMovementActive.erase(movementRoutineFound);
		}
		else
			printf("MovementManager::stopMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::stopMovementRoutine: Routine for shape not found\n");
}

void MovementManager::deleteMovementRoutine()
{
	for (auto routine_active = m_routineMovementActive.begin(); routine_active != m_routineMovementActive.end();) {
		routine_active->second->stop(routine_active->first);
		routine_active = m_routineMovementActive.erase(routine_active);
	}

	for (auto routineContainer = m_routineMovement.begin(); routineContainer != m_routineMovement.end();) {
		routineContainer->second->clear();
		delete routineContainer->second;
		routineContainer = m_routineMovement.erase(routineContainer);
	}

	m_movementRoutineContainer->clear();
}

void MovementManager::deleteMovementRoutine(const std::string& _name)
{
	for (auto routine_active = m_routineMovementActive.begin(); routine_active != m_routineMovementActive.end();) {
		if (routine_active->second->getName() == _name) {
			routine_active->second->stop(routine_active->first);
			routine_active = m_routineMovementActive.erase(routine_active);
		}
		else
			++routine_active;
	}

	for (auto routineContainer = m_routineMovement.begin(); routineContainer != m_routineMovement.end();) {
		routineContainer->second->deleteRoutine(_name);
		if (routineContainer->second->getRoutineCount() == 0) {
			delete routineContainer->second;
			routineContainer = m_routineMovement.erase(routineContainer);
		}
		else
			++routineContainer;
	}

	m_movementRoutineContainer->deleteRoutine(_name);
}

const long long int MovementManager::getSizeMovement() const
{
	long long int size{};
	size += m_movementRoutineContainer->size() + sizeof(m_movementRoutineContainer);

	for (const auto& routineShape : m_routineMovement)
		size += routineShape.second->size() + sizeof(routineShape.second);

	for (const auto& routineActive : m_routineMovementActive)
		size += sizeof(routineActive.second);

	return size;
}

// - - - - - - - - - - - - - - - - - - - - Scaling  - - - - - - - - - - - - - - - - - - - - \\

ScalingRoutine* MovementManager::createScalingRoutine(const std::string& _name)
{
	return this->m_scalingRoutineContainer->createRoutine(_name);
}

ScalingRoutine* MovementManager::getScalingRoutine(const std::string& _name)
{
	return this->m_scalingRoutineContainer->getRoutinePtr(_name);
}

ScalingRoutine* MovementManager::getScalingRoutine(sf::Transformable& transformable, const std::string& _name)
{
	auto scalingRoutineContainerFound = m_routineScaling.find(&transformable);
	if (scalingRoutineContainerFound != m_routineScaling.end()) {
		return scalingRoutineContainerFound->second->exists(_name);
	}
	else {
		printf("MovementManager::getScalingRoutine: Routine for shape not found\n");
		return nullptr;
	}
}

ScalingRoutine* MovementManager::linkScalingRoutine(sf::Transformable& transformable, const std::string& _name)
{
	// Check if routine with given name exists
	ScalingRoutine* scalingRoutineOriginal = m_scalingRoutineContainer->getRoutinePtr(_name);

	if (scalingRoutineOriginal == nullptr) {
		printf("MovementManager::linkScalingRoutine: Routine with name %s not found\n", _name.c_str());
		return nullptr;
	}

	// Check if we linked a scaling routine to this shape before
	auto m_scalingRoutineContainerFound = m_routineScaling.find(&transformable);
	if (m_scalingRoutineContainerFound != m_routineScaling.end()) {

		auto* scalingRoutineFound = m_scalingRoutineContainerFound->second->exists(_name);
		if (scalingRoutineFound != nullptr) {
			printf("MovementManager::linkScalingRoutine: Routine with name %s already linked to shape\n", _name.c_str());
			return scalingRoutineFound;
		}
	}
	else {
		// If not, create a new scaling routine container for this shape and link the scaling routine to it
		ScalingRoutineContainer* newScalingRoutineContainer = new ScalingRoutineContainer(this->movementRoutineEngine);
		m_routineScaling.insert(std::make_pair(&transformable, newScalingRoutineContainer));

		ScalingRoutine* newScalingRoutine = new ScalingRoutine(*scalingRoutineOriginal);
		return newScalingRoutineContainer->createRoutine(_name, newScalingRoutine);
	}

	// If yes, create a new scaling routine and link it to the existing scaling routine container
	ScalingRoutine* newScalingRoutine = new ScalingRoutine(*scalingRoutineOriginal);
	return m_scalingRoutineContainerFound->second->createRoutine(_name, newScalingRoutine);
}

ScalingRoutine* MovementManager::linkScalingRoutine(sf::Transformable* transformable, ScalingRoutine* routine)
{
	ScalingRoutine* scalingRoutineFound = m_scalingRoutineContainer->exists(routine);

	if (scalingRoutineFound == nullptr) {
		printf("MovementManager::linkScalingRoutine: Routine not found\n");
		return nullptr;
	}
	else {
		// Check if we linked a scaling routine to this shape before
		auto m_scalingRoutineContainerFound = m_routineScaling.find(transformable);
		if (m_scalingRoutineContainerFound != m_routineScaling.end()) {

			auto* scalingRoutineFound = m_scalingRoutineContainerFound->second->exists(routine);
			if (scalingRoutineFound != nullptr) {
				printf("MovementManager::linkScalingRoutine: Routine already linked to shape\n");
				return scalingRoutineFound;
			}
		}
		else {
			// If not, create a new scaling routine container for this shape and link the scaling routine to it
			ScalingRoutineContainer* newScalingRoutineContainer = new ScalingRoutineContainer(this->movementRoutineEngine);
			m_routineScaling.insert(std::make_pair(transformable, newScalingRoutineContainer));

			return newScalingRoutineContainer->createRoutine(scalingRoutineFound->getName(), new ScalingRoutine(*routine));
		}

		// If yes, create a new scaling routine and link it to the existing scaling routine container
		return m_scalingRoutineContainerFound->second->createRoutine(scalingRoutineFound->getName(), new ScalingRoutine(*routine));
	}
}

void MovementManager::unlinkScalingRoutine(sf::Transformable* transformable, const std::string& _name)
{
	// Stop the routine if it's active
	this->stopScalingRoutine(transformable, _name);

	// Delete the routine from m_routineScaling
	auto m_scalingRoutineContainerFound = m_routineScaling.find(transformable);
	if (m_scalingRoutineContainerFound != m_routineScaling.end()) {

		auto* scalingRoutineFound = m_scalingRoutineContainerFound->second->exists(_name);
		if (scalingRoutineFound != nullptr) {
			m_scalingRoutineContainerFound->second->deleteRoutine(_name);
		}
		else
			printf("MovementManager::unlinkScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::unlinkScalingRoutine: Routine for shape not found\n");
}

void MovementManager::startScalingRoutine(sf::Transformable& transformable, const std::string& _name)
{
	auto m_scalingRoutineContainerFound = m_routineScaling.find(&transformable);
	if (m_scalingRoutineContainerFound != m_routineScaling.end()) {

		auto* scalingRoutineFound = m_scalingRoutineContainerFound->second->exists(_name);
		if (scalingRoutineFound != nullptr) {

			if (scalingRoutineFound->start(transformable))
				m_routineScaling_Active.insert(std::make_pair(&transformable, scalingRoutineFound));
		}
		else
			printf("MovementManager::startScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::startScalingRoutine: Routine for shape not found\n");
}

void MovementManager::pauseScalingRoutine(sf::Transformable& transformable, const std::string& _name)
{
	auto scalingRoutineFound = m_routineScaling_Active.find(&transformable);
	if (scalingRoutineFound != m_routineScaling_Active.end()) {
		if (scalingRoutineFound->second->getName() == _name) 
			scalingRoutineFound->second->pause();
		else
			printf("MovementManager::pauseScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::pauseScalingRoutine: Routine for shape not found\n");
}

void MovementManager::resumeScalingRoutine(sf::Transformable& transformable, const std::string& _name)
{
	auto scalingRoutineFound = m_routineScaling_Active.find(&transformable);
	if (scalingRoutineFound != m_routineScaling_Active.end()) {
		if (scalingRoutineFound->second->getName() == _name) 
			scalingRoutineFound->second->resume();
		else
			printf("MovementManager::resumeScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::resumeScalingRoutine: Routine for shape not found\n");
}

void MovementManager::resetScalingRoutine(sf::Transformable& transformable, const std::string& _name)
{
	auto scalingRoutineFound = m_routineScaling_Active.find(&transformable);
	if (scalingRoutineFound != m_routineScaling_Active.end()) {
		if (scalingRoutineFound->second->getName() == _name) 
			scalingRoutineFound->second->reset(transformable);
		else
			printf("MovementManager::resetScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::resetScalingRoutine: Routine for shape not found\n");
}

void MovementManager::stopScalingRoutine(sf::Transformable* transformable, const std::string& _name)
{
	auto scalingRoutineFound = m_routineScaling_Active.find(transformable);
	if (scalingRoutineFound != m_routineScaling_Active.end()) {
		if (scalingRoutineFound->second->getName() == _name) {
			scalingRoutineFound->second->stop(transformable);
			m_routineScaling_Active.erase(scalingRoutineFound);
		}
		else
			printf("MovementManager::stopScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::stopScalingRoutine: Routine for shape not found\n");
}

void MovementManager::deleteScalingRoutine()
{
	for (auto routine_active = m_routineScaling_Active.begin(); routine_active != m_routineScaling_Active.end();) {
		routine_active->second->stop(routine_active->first);
		routine_active = m_routineScaling_Active.erase(routine_active);
	}

	for (auto routineContainer = m_routineScaling.begin(); routineContainer != m_routineScaling.end();) {
		routineContainer->second->clear();
		delete routineContainer->second;
		routineContainer = m_routineScaling.erase(routineContainer);
	}

	m_scalingRoutineContainer->clear();
}

void MovementManager::deleteScalingRoutine(const std::string& _name)
{
	for (auto routine_active = m_routineScaling_Active.begin(); routine_active != m_routineScaling_Active.end();) {
		if (routine_active->second->getName() == _name) {
			routine_active->second->stop(routine_active->first);
			routine_active = m_routineScaling_Active.erase(routine_active);
		}
		else
			++routine_active;
	}

	for (auto routineContainer = m_routineScaling.begin(); routineContainer != m_routineScaling.end();) {
		routineContainer->second->deleteRoutine(_name);
		if (routineContainer->second->getRoutineCount() == 0) {
			delete routineContainer->second;
			routineContainer = m_routineScaling.erase(routineContainer);
		}
		else
			++routineContainer;
	}

	m_scalingRoutineContainer->deleteRoutine(_name);
}

const long long int MovementManager::getSizeScaling() const
{
	long long int size{};
	size += m_scalingRoutineContainer->size() + sizeof(m_scalingRoutineContainer);

	for (const auto& routineShape : m_routineScaling)
		size += routineShape.second->size() + sizeof(routineShape.second);

	for (const auto& routineActive : m_routineScaling_Active)
		size += sizeof(routineActive.second);

	return size;
}

// - - - - - - - - - - - - - - - - - - - - Rotation  - - - - - - - - - - - - - - - - - - - - \\

RotationRoutine* MovementManager::createRotationRoutine(const std::string& _name)
{
	return this->m_rotationRoutineContainer->createRoutine(_name);
}

RotationRoutine* MovementManager::getRotationRoutine(const std::string& _name)
{
	return this->m_rotationRoutineContainer->getRoutinePtr(_name);
}

RotationRoutine* MovementManager::getRotationRoutine(sf::Transformable& transformable, const std::string& _name)
{
	auto rotationRoutineContainerFound = m_routineRotation.find(&transformable);
	if (rotationRoutineContainerFound != m_routineRotation.end()) {
		return rotationRoutineContainerFound->second->exists(_name);
	}
	else {
		printf("MovementManager::getRotationRoutine: Routine for shape not found\n");
		return nullptr;
	}
}

RotationRoutine* MovementManager::linkRotationRoutine(sf::Transformable& transformable, const std::string& _name)
{
	// Check if routine with given name exists
	RotationRoutine* rotationRoutineOriginal = m_rotationRoutineContainer->getRoutinePtr(_name);

	if (rotationRoutineOriginal == nullptr) {
		printf("MovementManager::linkRotationRoutine: Routine with name %s not found\n", _name.c_str());
		return nullptr;
	}

	// Check if we linked a rotation routine to this shape before
	auto m_rotationRoutineContainerFound = m_routineRotation.find(&transformable);
	if (m_rotationRoutineContainerFound != m_routineRotation.end()) {

		auto* rotationRoutineFound = m_rotationRoutineContainerFound->second->exists(_name);
		if (rotationRoutineFound != nullptr) {
			printf("MovementManager::linkRotationRoutine: Routine with name %s already linked to shape\n", _name.c_str());
			return rotationRoutineFound;
		}
	}
	else {
		// If not, create a new rotation routine container for this shape and link the rotation routine to it
		RotationRoutineContainer* newRotationRoutineContainer = new RotationRoutineContainer(this->movementRoutineEngine);
		m_routineRotation.insert(std::make_pair(&transformable, newRotationRoutineContainer));

		RotationRoutine* newRotationRoutine = new RotationRoutine(*rotationRoutineOriginal);
		return newRotationRoutineContainer->createRoutine(_name, newRotationRoutine);
	}

	// If yes, create a new rotation routine and link it to the existing rotation routine container
	RotationRoutine* newRotationRoutine = new RotationRoutine(*rotationRoutineOriginal);
	return m_rotationRoutineContainerFound->second->createRoutine(_name, newRotationRoutine);
}

RotationRoutine* MovementManager::linkRotationRoutine(sf::Transformable* transformable, RotationRoutine* routine)
{
	RotationRoutine* rotationRoutineFound = m_rotationRoutineContainer->exists(routine);

	if (rotationRoutineFound == nullptr) {
		printf("MovementManager::linkRotationRoutine: Routine not found\n");
		return nullptr;
	}
	else {
		// Check if we linked a rotation routine to this shape before
		auto m_rotationRoutineContainerFound = m_routineRotation.find(transformable);
		if (m_rotationRoutineContainerFound != m_routineRotation.end()) {

			auto* rotationRoutineFound = m_rotationRoutineContainerFound->second->exists(routine);
			if (rotationRoutineFound != nullptr) {
				printf("MovementManager::linkRotationRoutine: Routine already linked to shape\n");
				return rotationRoutineFound;
			}
		}
		else {
			// If not, create a new rotation routine container for this shape and link the rotation routine to it
			RotationRoutineContainer* newRotationRoutineContainer = new RotationRoutineContainer(this->movementRoutineEngine);
			m_routineRotation.insert(std::make_pair(transformable, newRotationRoutineContainer));

			return newRotationRoutineContainer->createRoutine(rotationRoutineFound->getName(), new RotationRoutine(*routine));
		}

		// If yes, create a new rotation routine and link it to the existing rotation routine container
		return m_rotationRoutineContainerFound->second->createRoutine(rotationRoutineFound->getName(), new RotationRoutine(*routine));
	}
}

void MovementManager::unlinkRotationRoutine(sf::Transformable* transformable, const std::string& _name)
{
	// Stop the routine if it's active
	this->stopRotationRoutine(transformable, _name);

	// Delete the routine from m_routineRotation
	auto m_rotationRoutineContainerFound = m_routineRotation.find(transformable);
	if (m_rotationRoutineContainerFound != m_routineRotation.end()) {

		auto* rotationRoutineFound = m_rotationRoutineContainerFound->second->exists(_name);
		if (rotationRoutineFound != nullptr) {
			m_rotationRoutineContainerFound->second->deleteRoutine(_name);
		}
		else
			printf("MovementManager::unlinkRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::unlinkRotationRoutine: Routine for shape not found\n");
}

void MovementManager::startRotationRoutine(sf::Transformable& transformable, const std::string& _name)
{
	auto m_rotationRoutineContainerFound = m_routineRotation.find(&transformable);
	if (m_rotationRoutineContainerFound != m_routineRotation.end()) {

		auto* rotationRoutineFound = m_rotationRoutineContainerFound->second->exists(_name);
		if (rotationRoutineFound != nullptr) {

			if (rotationRoutineFound->start(transformable))
				m_routineRotation_Active.insert(std::make_pair(&transformable, rotationRoutineFound));
		}
		else
			printf("MovementManager::startRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::startRotationRoutine: Routine for shape not found\n");
}

void MovementManager::pauseRotationRoutine(sf::Transformable& transformable, const std::string& _name)
{
	auto rotationRoutineFound = m_routineRotation_Active.find(&transformable);
	if (rotationRoutineFound != m_routineRotation_Active.end()) {
		if (rotationRoutineFound->second->getName() == _name) 
			rotationRoutineFound->second->pause();
		else
			printf("MovementManager::pauseRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::pauseRotationRoutine: Routine for shape not found\n");
}

void MovementManager::resumeRotationRoutine(sf::Transformable& transformable, const std::string& _name)
{
	auto rotationRoutineFound = m_routineRotation_Active.find(&transformable);
	if (rotationRoutineFound != m_routineRotation_Active.end()) {
		if (rotationRoutineFound->second->getName() == _name) 
			rotationRoutineFound->second->resume();
		else
			printf("MovementManager::resumeRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::resumeRotationRoutine: Routine for shape not found\n");
}

void MovementManager::resetRotationRoutine(sf::Transformable& transformable, const std::string& _name)
{
	auto rotationRoutineFound = m_routineRotation_Active.find(&transformable);
	if (rotationRoutineFound != m_routineRotation_Active.end()) {
		if (rotationRoutineFound->second->getName() == _name) 
			rotationRoutineFound->second->reset(transformable);
		else
			printf("MovementManager::resetRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::resetRotationRoutine: Routine for shape not found\n");
}

void MovementManager::stopRotationRoutine(sf::Transformable* transformable, const std::string& _name)
{
	auto rotationRoutineFound = m_routineRotation_Active.find(transformable);
	if (rotationRoutineFound != m_routineRotation_Active.end()) {
		if (rotationRoutineFound->second->getName() == _name) {
			rotationRoutineFound->second->stop(transformable);
			m_routineRotation_Active.erase(rotationRoutineFound);
		}
		else
			printf("MovementManager::stopRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::stopRotationRoutine: Routine for shape not found\n");
}

void MovementManager::deleteRotationRoutine()
{
	for (auto routine_active = m_routineRotation_Active.begin(); routine_active != m_routineRotation_Active.end();) {
		routine_active->second->stop(routine_active->first);
		routine_active = m_routineRotation_Active.erase(routine_active);
	}

	for (auto routineContainer = m_routineRotation.begin(); routineContainer != m_routineRotation.end();) {
		routineContainer->second->clear();
		delete routineContainer->second;
		routineContainer = m_routineRotation.erase(routineContainer);
	}

	m_rotationRoutineContainer->clear();
}

void MovementManager::deleteRotationRoutine(const std::string& _name)
{
for (auto routine_active = m_routineRotation_Active.begin(); routine_active != m_routineRotation_Active.end();) {
		if (routine_active->second->getName() == _name) {
			routine_active->second->stop(routine_active->first);
			routine_active = m_routineRotation_Active.erase(routine_active);
		}
		else
			++routine_active;
	}

	for (auto routineContainer = m_routineRotation.begin(); routineContainer != m_routineRotation.end();) {
		routineContainer->second->deleteRoutine(_name);
		if (routineContainer->second->getRoutineCount() == 0) {
			delete routineContainer->second;
			routineContainer = m_routineRotation.erase(routineContainer);
		}
		else
			++routineContainer;
	}

	m_rotationRoutineContainer->deleteRoutine(_name);
}

const long long int MovementManager::getSizeRotation() const
{
	long long int size{};
	size += m_rotationRoutineContainer->size() + sizeof(m_rotationRoutineContainer);

	for (const auto& routineShape : m_routineRotation)
		size += routineShape.second->size() + sizeof(routineShape.second);

	for (const auto& routineActive : m_routineRotation_Active)
		size += sizeof(routineActive.second);

	return size;
}
