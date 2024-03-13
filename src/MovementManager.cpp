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

inline void MovementManager::printDebug(const std::string& message) const
{
	if (debug)
		printf("MovementManager: %s\n", message.c_str());
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

void MovementManager::setAnimationTime(sf::Transformable& transformable, const float _time)
{
	if (_time < 0.f)
		return;

	auto movementRoutineFound = m_routineMovementActive.find(&transformable);
	if (movementRoutineFound != m_routineMovementActive.end())
		movementRoutineFound->second->setAnimationTime(_time);

	auto scalingRoutineFound = m_routineScaling_Active.find(&transformable);
	if (scalingRoutineFound != m_routineScaling_Active.end())
		scalingRoutineFound->second->setAnimationTime(_time);

	auto rotationRoutineFound = m_routineRotation_Active.find(&transformable);
	if (rotationRoutineFound != m_routineRotation_Active.end())
		rotationRoutineFound->second->setAnimationTime(_time);
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
		printDebug("getMovementRoutine: Routine for shape not found");
		return nullptr;
	}
}

MovementRoutine* MovementManager::linkMovementRoutine(sf::Transformable& transformable, const std::string& _name)
{
	// Check if routine with given name exists
	MovementRoutine* movementRoutineOriginal = m_movementRoutineContainer->getRoutinePtr(_name);

	if (movementRoutineOriginal == nullptr) {
		printDebug("linkMovementRoutine: Routine with name " + _name + " not found");
		return nullptr;
	}

	// Check if we linked a movement routine to this shape before
	auto m_movementRoutineContainerFound = m_routineMovement.find(&transformable);
	if (m_movementRoutineContainerFound != m_routineMovement.end()) {

		auto* movementRoutineFound = m_movementRoutineContainerFound->second->exists(_name);
		if (movementRoutineFound != nullptr) {
			printDebug("linkMovementRoutine: Routine with name " + _name + " already linked to shape");
			return movementRoutineFound;
		}
	}
	else {
		// If not, create a new movement routine container for this shape and link the movement routine to it
		MovementRoutineContainer* newMovementRoutineContainer = new MovementRoutineContainer(this->movementRoutineEngine);
		auto map_iterator = m_routineMovement.insert(std::make_pair(&transformable, newMovementRoutineContainer)).first;

		auto routineMappedFound = m_routineMovementMapped.find(_name);
		if (routineMappedFound != m_routineMovementMapped.end())
			routineMappedFound->second.emplace_back(map_iterator);
		else
			m_routineMovementMapped.insert(std::make_pair(_name, std::vector<std::map<sf::Transformable*, MovementRoutineContainer*>::iterator>{ map_iterator }));

		MovementRoutine* newMovementRoutine = new MovementRoutine(*movementRoutineOriginal);
		return newMovementRoutineContainer->createRoutine(_name, newMovementRoutine);
	}

	// If yes, create a new movement routine and link it to the existing movement routine container
	MovementRoutine* newMovementRoutine = new MovementRoutine(*movementRoutineOriginal);

	auto routineMappedFound = m_routineMovementMapped.find(_name);
	if (routineMappedFound != m_routineMovementMapped.end())
		routineMappedFound->second.emplace_back(m_movementRoutineContainerFound);
	else
		m_routineMovementMapped.insert(std::make_pair(_name, std::vector<std::map<sf::Transformable*, MovementRoutineContainer*>::iterator>{ m_movementRoutineContainerFound }));
	return m_movementRoutineContainerFound->second->createRoutine(_name, newMovementRoutine);
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
			printDebug("unlinkMovementRoutine: Routine with name " + _name + " not found");
		
		// Sprawdziæ czy mo¿na usprawniæ
		auto routineMappedFound = m_routineMovementMapped.find(_name);
		if (routineMappedFound != m_routineMovementMapped.end()) 
			for (auto map_iterator = routineMappedFound->second.begin(); map_iterator != routineMappedFound->second.end(); ++map_iterator) 
				if (*map_iterator == m_movementRoutineContainerFound) {
					routineMappedFound->second.erase(map_iterator);
					break;
				}

	}
	else
		printDebug("unlinkMovementRoutine: Routine for shape not found");
}

void MovementManager::startMovementRoutine(sf::Transformable& transformable, const std::string& _name)
{
	auto m_movementRoutineContainerFound = m_routineMovement.find(&transformable);
	if (m_movementRoutineContainerFound != m_routineMovement.end()) {
		
		auto* movementRoutineFound = m_movementRoutineContainerFound->second->exists(_name);
		if (movementRoutineFound != nullptr) {
			if (movementRoutineFound->start(transformable)) {
				auto map_iterator = m_routineMovementActive.insert(std::make_pair(&transformable, movementRoutineFound)).first;

				auto routineActiveMappedFound = m_routineMovementActiveMapped.find(_name);
				if (routineActiveMappedFound != m_routineMovementActiveMapped.end())
					routineActiveMappedFound->second.emplace_back(map_iterator);
				else
					m_routineMovementActiveMapped.insert(std::make_pair(_name, std::vector<std::map<sf::Transformable*, MovementRoutine*>::iterator>{ map_iterator }));
			}
		}
		else
			printDebug("startMovementRoutine: Routine with name " + _name + " not found");
	}
	else
		printDebug("startMovementRoutine: Routine for shape not found");
}

void MovementManager::pauseMovementRoutine(sf::Transformable& transformable, const std::string& _name)
{
	auto movementRoutineFound = m_routineMovementActive.find(&transformable);
	if (movementRoutineFound != m_routineMovementActive.end()) {
		if (movementRoutineFound->second->getName() == _name)
			movementRoutineFound->second->pause();
		else
			printDebug("pauseMovementRoutine: Routine with name " + _name + " not found");
	}
	else
		printDebug("pauseMovementRoutine: Routine for shape not found");
}

void MovementManager::resumeMovementRoutine(sf::Transformable& transformable, const std::string& _name)
{
	auto movementRoutineFound = m_routineMovementActive.find(&transformable);
	if (movementRoutineFound != m_routineMovementActive.end()) {
		if (movementRoutineFound->second->getName() == _name)
			movementRoutineFound->second->resume();
		else
			printDebug("resumeMovementRoutine: Routine with name " + _name + " not found");
	}
	else
		printDebug("resumeMovementRoutine: Routine for shape not found");
}

void MovementManager::resetMovementRoutine(sf::Transformable& transformable, const std::string& _name)
{
	auto movementRoutineFound = m_routineMovementActive.find(&transformable);
	if (movementRoutineFound != m_routineMovementActive.end()) {
		if (movementRoutineFound->second->getName() == _name)
			movementRoutineFound->second->reset(transformable);
		else
			printDebug("resetMovementRoutine: Routine with name " + _name + " not found");
	}
	else
		printDebug("resetMovementRoutine: Routine for shape not found");
}

void MovementManager::stopMovementRoutine(sf::Transformable* transformable, const std::string& _name)
{
	auto movementRoutineFound = m_routineMovementActive.find(transformable);
	if (movementRoutineFound != m_routineMovementActive.end()) {
		if (movementRoutineFound->second->getName() == _name) {
			movementRoutineFound->second->stop(transformable);
			m_routineMovementActiveMapped.erase(_name);
			m_routineMovementActive.erase(movementRoutineFound);
		}
		else
			printDebug("stopMovementRoutine: Routine with name " + _name + " not found");
	}
	else
		printDebug("stopMovementRoutine: Routine for shape not found");
}

void MovementManager::deleteMovementRoutine()
{
	for (auto routine_active = m_routineMovementActive.begin(); routine_active != m_routineMovementActive.end();) {
		routine_active->second->stop(routine_active->first);
		m_routineMovementActiveMapped.erase(routine_active->second->getName());
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
	auto routineActiveFound = m_routineMovementActiveMapped.find(_name);
	if (routineActiveFound != m_routineMovementActiveMapped.end()) {

		/*for (auto& map_iterator : routineActiveFound->second) {
			auto routineContainerFound = m_routineMovement.find(map_iterator->first);
			if (routineContainerFound != m_routineMovement.end()) {
				routineContainerFound->second->deleteRoutine(_name);

				if (routineContainerFound->second->getRoutineCount() == 0) {
					delete routineContainerFound->second;
					m_routineMovement.erase(routineContainerFound);
				}
			}

			m_routineMovementActive.erase(map_iterator);
		}*/
		for (const auto& active_routine : routineActiveFound->second) {
			active_routine->second->stop(active_routine->first);
			m_routineMovementActive.erase(active_routine);
		}
			
		m_routineMovementActiveMapped.erase(routineActiveFound);
	}

	auto routineFound = m_routineMovementMapped.find(_name);
	if (routineFound != m_routineMovementMapped.end()) {
		for (auto routine : routineFound->second) {
			routine->second->deleteRoutine(_name);

			if (routine->second->getRoutineCount() == 0) {
				delete routine->second;
				m_routineMovement.erase(routine);
			}
		}

		m_routineMovementMapped.erase(routineFound);
	}

	//for (auto routineContainer = m_routineMovement.begin(); routineContainer != m_routineMovement.end();) {  // Poprawiæ wydajnoœæ tego
	//	routineContainer->second->deleteRoutine(_name);
	//	if (routineContainer->second->getRoutineCount() == 0) {
	//		delete routineContainer->second;
	//		routineContainer = m_routineMovement.erase(routineContainer);
	//	}
	//	else
	//		++routineContainer;
	//}

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
	else
		printDebug("getScalingRoutine: Routine for shape not found");

	return nullptr;
}

ScalingRoutine* MovementManager::linkScalingRoutine(sf::Transformable& transformable, const std::string& _name)
{
	// Check if routine with given name exists
	ScalingRoutine* scalingRoutineOriginal = m_scalingRoutineContainer->getRoutinePtr(_name);

	if (scalingRoutineOriginal == nullptr) {
		printDebug("linkScalingRoutine: Routine with name " + _name + " not found");
		return nullptr;
	}

	// Check if we linked a scaling routine to this shape before
	auto m_scalingRoutineContainerFound = m_routineScaling.find(&transformable);
	if (m_scalingRoutineContainerFound != m_routineScaling.end()) {

		auto* scalingRoutineFound = m_scalingRoutineContainerFound->second->exists(_name);
		if (scalingRoutineFound != nullptr) {
			printDebug("linkScalingRoutine: Routine with name " + _name + " already linked to shape");
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
			printDebug("unlinkScalingRoutine: Routine with name " + _name + " not found");
	}
	else
		printDebug("unlinkScalingRoutine: Routine for shape not found");
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
			printDebug("startScalingRoutine: Routine with name " + _name + " not found");
	}
	else
		printDebug("startScalingRoutine: Routine for shape not found");
}

void MovementManager::pauseScalingRoutine(sf::Transformable& transformable, const std::string& _name)
{
	auto scalingRoutineFound = m_routineScaling_Active.find(&transformable);
	if (scalingRoutineFound != m_routineScaling_Active.end()) {
		if (scalingRoutineFound->second->getName() == _name)
			scalingRoutineFound->second->pause();
		else
			printDebug("pauseScalingRoutine: Routine with name " + _name + " not found");
	}
	else
		printDebug("pauseScalingRoutine: Routine for shape not found");
}

void MovementManager::resumeScalingRoutine(sf::Transformable& transformable, const std::string& _name)
{
	auto scalingRoutineFound = m_routineScaling_Active.find(&transformable);
	if (scalingRoutineFound != m_routineScaling_Active.end()) {
		if (scalingRoutineFound->second->getName() == _name)
			scalingRoutineFound->second->resume();
		else
			printDebug("resumeScalingRoutine: Routine with name " + _name + " not found");
	}
	else
		printDebug("resumeScalingRoutine: Routine for shape not found");
}

void MovementManager::resetScalingRoutine(sf::Transformable& transformable, const std::string& _name)
{
	auto scalingRoutineFound = m_routineScaling_Active.find(&transformable);
	if (scalingRoutineFound != m_routineScaling_Active.end()) {
		if (scalingRoutineFound->second->getName() == _name)
			scalingRoutineFound->second->reset(transformable);
		else
			printDebug("resetScalingRoutine: Routine with name " + _name + " not found");
	}
	else
		printDebug("resetScalingRoutine: Routine for shape not found");
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
			printDebug("stopScalingRoutine: Routine with name " + _name + " not found");
	}
	else
		printDebug("stopScalingRoutine: Routine for shape not found");
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

	if (rotationRoutineContainerFound != m_routineRotation.end())
		return rotationRoutineContainerFound->second->exists(_name);
	else
		printDebug("getRotationRoutine: Routine for shape not found");

	return nullptr;

}

RotationRoutine* MovementManager::linkRotationRoutine(sf::Transformable& transformable, const std::string& _name)
{
	// Check if routine with given name exists
	RotationRoutine* rotationRoutineOriginal = m_rotationRoutineContainer->getRoutinePtr(_name);

	if (rotationRoutineOriginal == nullptr) {
		printDebug("linkRotationRoutine: Routine with name " + _name + " not found");
		return nullptr;
	}

	// Check if we linked a rotation routine to this shape before
	auto m_rotationRoutineContainerFound = m_routineRotation.find(&transformable);
	if (m_rotationRoutineContainerFound != m_routineRotation.end()) {

		auto* rotationRoutineFound = m_rotationRoutineContainerFound->second->exists(_name);
		if (rotationRoutineFound != nullptr) {
			printDebug("linkRotationRoutine: Routine with name " + _name + " already linked to shape");
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
			printDebug("unlinkRotationRoutine: Routine with name " + _name + " not found");
	}
	else
		printDebug("unlinkRotationRoutine: Routine for shape not found");
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
			printDebug("startRotationRoutine: Routine with name " + _name + " not found");
	}
	else
		printDebug("startRotationRoutine: Routine for shape not found");
}

void MovementManager::pauseRotationRoutine(sf::Transformable& transformable, const std::string& _name)
{
	auto rotationRoutineFound = m_routineRotation_Active.find(&transformable);
	if (rotationRoutineFound != m_routineRotation_Active.end()) {
		if (rotationRoutineFound->second->getName() == _name)
			rotationRoutineFound->second->pause();
		else
			printDebug("pauseRotationRoutine: Routine with name " + _name + " not found");
	}
	else
		printDebug("pauseRotationRoutine: Routine for shape not found");
}

void MovementManager::resumeRotationRoutine(sf::Transformable& transformable, const std::string& _name)
{
	auto rotationRoutineFound = m_routineRotation_Active.find(&transformable);
	if (rotationRoutineFound != m_routineRotation_Active.end()) {
		if (rotationRoutineFound->second->getName() == _name)
			rotationRoutineFound->second->resume();
		else
			printDebug("resumeRotationRoutine: Routine with name " + _name + " not found");
	}
	else
		printDebug("resumeRotationRoutine: Routine for shape not found");
}

void MovementManager::resetRotationRoutine(sf::Transformable& transformable, const std::string& _name)
{
	auto rotationRoutineFound = m_routineRotation_Active.find(&transformable);
	if (rotationRoutineFound != m_routineRotation_Active.end()) {
		if (rotationRoutineFound->second->getName() == _name)
			rotationRoutineFound->second->reset(transformable);
		else
			printDebug("resetRotationRoutine: Routine with name " + _name + " not found");
	}
	else
		printDebug("resetRotationRoutine: Routine for shape not found");
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
			printDebug("stopRotationRoutine: Routine with name " + _name + " not found");
	}
	else
		printDebug("stopRotationRoutine: Routine for shape not found");
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

void MovementManager::setDebug(const bool debug)
{
	this->debug = debug;
}
