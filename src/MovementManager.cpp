#pragma once
#include "MovementManager.h"

// Singleton initialization
MovementManager* MovementManager::sInstance = nullptr;

// Constructors / Destructors
MovementManager::MovementManager()
{
	assert(this->sInstance == nullptr);
	sInstance = this;

	movementRoutineEngine = new MovementRoutineEngine(this);

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

// Update functions
void MovementManager::update(const float dt)
{
	this->movementRoutineEngine->update(dt);
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

MovementRoutine* MovementManager::linkMovementRoutine(sf::Shape& _shape, const std::string& _name)
{
	// Check if routine with given name exists
	MovementRoutine* movementRoutineOriginal = m_movementRoutineContainer->getRoutinePtr(_name);

	if (movementRoutineOriginal == nullptr) {
		printf("MovementManager::linkMovementRoutine: Routine with name %s not found\n", _name.c_str());
		return nullptr;
	}

	// Check if we linked a movement routine to this shape before
	auto m_movementRoutineContainerFound = m_routineMovementShape.find(&_shape);
	if (m_movementRoutineContainerFound != m_routineMovementShape.end()){

		auto* movementRoutineFound = m_movementRoutineContainerFound->second->exists(_name);
		if (movementRoutineFound != nullptr){
			printf("MovementManager::linkMovementRoutine: Routine with name %s already linked to shape\n", _name.c_str());
			return movementRoutineFound;
		}
	}
	else {
		// If not, create a new movement routine container for this shape and link the movement routine to it
		MovementRoutineContainer* newMovementRoutineContainer = new MovementRoutineContainer(this->movementRoutineEngine);
		m_routineMovementShape.insert(std::make_pair(&_shape, newMovementRoutineContainer));

		MovementRoutine* newMovementRoutine = new MovementRoutine(*movementRoutineOriginal);
		return newMovementRoutineContainer->createRoutine(_name, newMovementRoutine);
	}

	// If yes, create a new movement routine and link it to the existing movement routine container
	MovementRoutine* newMovementRoutine = new MovementRoutine(*movementRoutineOriginal);
	return m_movementRoutineContainerFound->second->createRoutine(_name, newMovementRoutine);
}

MovementRoutine* MovementManager::linkMovementRoutine(sf::Sprite& _sprite, const std::string& _name)
{
	// Check if routine with given name exists
	MovementRoutine* movementRoutineOriginal = m_movementRoutineContainer->getRoutinePtr(_name);

	if (movementRoutineOriginal == nullptr) {
		printf("MovementManager::linkMovementRoutine: Routine with name %s not found\n", _name.c_str());
		return nullptr;
	}

	// Check if we linked a movement routine to this shape before
	auto m_movementRoutineContainerFound = m_routineMovementSprite.find(&_sprite);
	if (m_movementRoutineContainerFound != m_routineMovementSprite.end()){

		auto* movementRoutineFound = m_movementRoutineContainerFound->second->exists(_name);
		if (movementRoutineFound != nullptr){
			printf("MovementManager::linkMovementRoutine: Routine with name %s already linked to shape\n", _name.c_str());
			return movementRoutineFound;
		}
	}
	else {
		// If not, create a new movement routine container for this shape and link the movement routine to it
		MovementRoutineContainer* newMovementRoutineContainer = new MovementRoutineContainer(this->movementRoutineEngine);
		m_routineMovementSprite.insert(std::make_pair(&_sprite, newMovementRoutineContainer));

		MovementRoutine* newMovementRoutine = new MovementRoutine(*movementRoutineOriginal);
		return newMovementRoutineContainer->createRoutine(_name, newMovementRoutine);
	}

	// If yes, create a new movement routine and link it to the existing movement routine container
	MovementRoutine* newMovementRoutine = new MovementRoutine(*movementRoutineOriginal);
	return m_movementRoutineContainerFound->second->createRoutine(_name, newMovementRoutine);
}

MovementRoutine* MovementManager::linkMovementRoutine(VertexArray2& _vertexarray, const std::string& _name)
{
	// Check if routine with given name exists
	MovementRoutine* movementRoutineOriginal = m_movementRoutineContainer->getRoutinePtr(_name);

	if (movementRoutineOriginal == nullptr) {
		printf("MovementManager::linkMovementRoutine: Routine with name %s not found\n", _name.c_str());
		return nullptr;
	}

	// Check if we linked a movement routine to this vertexarray before
	auto m_movementRoutineContainerFound = m_routineMovementVertexArray.find(&_vertexarray);
	if (m_movementRoutineContainerFound != m_routineMovementVertexArray.end()) {

		auto* movementRoutineFound = m_movementRoutineContainerFound->second->exists(_name);
		if (movementRoutineFound != nullptr) {
			printf("MovementManager::linkMovementRoutine: Routine with name %s already linked to vertexarray\n", _name.c_str());
			return movementRoutineFound;
		}
	}
	else {
		// If not, create a new movement routine container for this vertexarray and link the movement routine to it
		MovementRoutineContainer* newMovementRoutineContainer = new MovementRoutineContainer(this->movementRoutineEngine);
		m_routineMovementVertexArray.insert(std::make_pair(&_vertexarray, newMovementRoutineContainer));

		MovementRoutine* newMovementRoutine = new MovementRoutine(*movementRoutineOriginal);
		return newMovementRoutineContainer->createRoutine(_name, newMovementRoutine);
	}

	// If yes, create a new movement routine and link it to the existing movement routine container
	MovementRoutine* newMovementRoutine = new MovementRoutine(*movementRoutineOriginal);
	return m_movementRoutineContainerFound->second->createRoutine(_name, newMovementRoutine);

}

void MovementManager::unlinkMovementRoutine(sf::Shape* _shape, const std::string& _name)
{
	// Stop the routine if it's active
	this->stopMovementRoutine(_shape, _name);

	// Delete the routine from m_routineMovementShape
	auto m_movementRoutineContainerFound = m_routineMovementShape.find(_shape);
	if (m_movementRoutineContainerFound != m_routineMovementShape.end()) {

		auto* movementRoutineFound = m_movementRoutineContainerFound->second->exists(_name);
		if (movementRoutineFound != nullptr) 
			m_movementRoutineContainerFound->second->deleteRoutine(_name);
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

	// Delete the routine from m_routineMovementSprite
	auto m_movementRoutineContainerFound = m_routineMovementSprite.find(_sprite);
	if (m_movementRoutineContainerFound != m_routineMovementSprite.end()) {

		auto* movementRoutineFound = m_movementRoutineContainerFound->second->exists(_name);
		if (movementRoutineFound != nullptr) 
			m_movementRoutineContainerFound->second->deleteRoutine(_name);
		else
			printf("MovementManager::unlinkMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::unlinkMovementRoutine: Routine for sprite not found\n");
}

void MovementManager::unlinkMovementRoutine(VertexArray2* _vertexarray, const std::string& _name)
{
	// Stop the routine if it's active
	this->stopMovementRoutine(_vertexarray, _name);

	// Delete the routine from m_routineMovementVertexArray
	auto m_movementRoutineContainerFound = m_routineMovementVertexArray.find(_vertexarray);
	if (m_movementRoutineContainerFound != m_routineMovementVertexArray.end()) {

		auto* movementRoutineFound = m_movementRoutineContainerFound->second->exists(_name);
		if (movementRoutineFound != nullptr) 
			m_movementRoutineContainerFound->second->deleteRoutine(_name);
		else
			printf("MovementManager::unlinkMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::unlinkMovementRoutine: Routine for vertexarray not found\n");

}

void MovementManager::startMovementRoutine(sf::Shape& _shape, const std::string& _name)
{
	auto m_movementRoutineContainerFound = m_routineMovementShape.find(&_shape);
	if (m_movementRoutineContainerFound != m_routineMovementShape.end()) {

		auto* movementRoutineFound = m_movementRoutineContainerFound->second->exists(_name);
		if (movementRoutineFound != nullptr) {
			if (movementRoutineFound->start(_shape))
				m_routineMovementShapeActive.insert(std::make_pair(&_shape, movementRoutineFound));
		}
		else
			printf("MovementManager::startMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::startMovementRoutine: Routine for shape not found\n");
}

void MovementManager::startMovementRoutine(sf::Sprite& _sprite, const std::string& _name)
{
	auto m_movementRoutineContainerFound = m_routineMovementSprite.find(&_sprite);
	if (m_movementRoutineContainerFound != m_routineMovementSprite.end()) {

		auto* movementRoutineFound = m_movementRoutineContainerFound->second->exists(_name);
		if (movementRoutineFound != nullptr) {
			if (movementRoutineFound->start(_sprite))
				m_routineMovementSpriteActive.insert(std::make_pair(&_sprite, movementRoutineFound));
		}
		else
			printf("MovementManager::startMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::startMovementRoutine: Routine for sprite not found\n");
}

void MovementManager::startMovementRoutine(VertexArray2& _vertexarray, const std::string& _name)
{
	auto m_movementRoutineContainerFound = m_routineMovementVertexArray.find(&_vertexarray);
	if (m_movementRoutineContainerFound != m_routineMovementVertexArray.end()) {

		auto* movementRoutineFound = m_movementRoutineContainerFound->second->exists(_name);
		if (movementRoutineFound != nullptr) {
			if (movementRoutineFound->start(_vertexarray))
				m_routineMovementVertexArrayActive.insert(std::make_pair(&_vertexarray, movementRoutineFound));
		}
		else
			printf("MovementManager::startMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::startMovementRoutine: Routine for vertexarray not found\n");
}

void MovementManager::pauseMovementRoutine(sf::Shape& _shape, const std::string& _name)
{
	auto movementRoutineFound = m_routineMovementShapeActive.find(&_shape);
	if (movementRoutineFound != m_routineMovementShapeActive.end()) {
		if (movementRoutineFound->second->getName() == _name) 
			movementRoutineFound->second->pause();
		else
			printf("MovementManager::pauseMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::pauseMovementRoutine: Routine for shape not found\n");
}

void MovementManager::pauseMovementRoutine(sf::Sprite& _sprite, const std::string& _name)
{
	auto movementRoutineFound = m_routineMovementSpriteActive.find(&_sprite);
	if (movementRoutineFound != m_routineMovementSpriteActive.end()) {
		if (movementRoutineFound->second->getName() == _name) 
			movementRoutineFound->second->pause();
		else
			printf("MovementManager::pauseMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::pauseMovementRoutine: Routine for sprite not found\n");
}

void MovementManager::pauseMovementRoutine(VertexArray2& _vertexarray, const std::string& _name)
{
	auto movementRoutineFound = m_routineMovementVertexArrayActive.find(&_vertexarray);
	if (movementRoutineFound != m_routineMovementVertexArrayActive.end()) {
		if (movementRoutineFound->second->getName() == _name) 
			movementRoutineFound->second->pause();
		else
			printf("MovementManager::pauseMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::pauseMovementRoutine: Routine for vertexarray not found\n");
}

void MovementManager::resumeMovementRoutine(sf::Shape& _shape, const std::string& _name)
{
	auto movementRoutineFound = m_routineMovementShapeActive.find(&_shape);
	if (movementRoutineFound != m_routineMovementShapeActive.end()) {
		if (movementRoutineFound->second->getName() == _name) 
			movementRoutineFound->second->resume();
		else
			printf("MovementManager::resumeMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::resumeMovementRoutine: Routine for shape not found\n");
}

void MovementManager::resumeMovementRoutine(sf::Sprite& _sprite, const std::string& _name)
{
	auto movementRoutineFound = m_routineMovementSpriteActive.find(&_sprite);
	if (movementRoutineFound != m_routineMovementSpriteActive.end()) {
		if (movementRoutineFound->second->getName() == _name) 
			movementRoutineFound->second->resume();
		else
			printf("MovementManager::resumeMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::resumeMovementRoutine: Routine for sprite not found\n");
}

void MovementManager::resumeMovementRoutine(VertexArray2& _vertexarray, const std::string& _name)
{
	auto movementRoutineFound = m_routineMovementVertexArrayActive.find(&_vertexarray);
	if (movementRoutineFound != m_routineMovementVertexArrayActive.end()) {
		if (movementRoutineFound->second->getName() == _name) 
			movementRoutineFound->second->resume();
		else
			printf("MovementManager::resumeMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::resumeMovementRoutine: Routine for vertexarray not found\n");
}

void MovementManager::stopMovementRoutine(sf::Shape* _shape, const std::string& _name)
{
	auto movementRoutineFound = m_routineMovementShapeActive.find(_shape);
	if (movementRoutineFound != m_routineMovementShapeActive.end()) {
		if (movementRoutineFound->second->getName() == _name) {
			movementRoutineFound->second->stop(_shape);
			m_routineMovementShapeActive.erase(movementRoutineFound);
		}
		else
			printf("MovementManager::stopMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::stopMovementRoutine: Routine for shape not found\n");
}

void MovementManager::stopMovementRoutine(sf::Sprite* _sprite, const std::string& _name)
{
	auto movementRoutineFound = m_routineMovementSpriteActive.find(_sprite);
	if (movementRoutineFound != m_routineMovementSpriteActive.end()) {
		if (movementRoutineFound->second->getName() == _name) {
			movementRoutineFound->second->stop(_sprite);
			m_routineMovementSpriteActive.erase(movementRoutineFound);
		}
		else
			printf("MovementManager::stopMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::stopMovementRoutine: Routine for sprite not found\n");
}

void MovementManager::stopMovementRoutine(VertexArray2* _vertexarray, const std::string& _name)
{
	auto movementRoutineFound = m_routineMovementVertexArrayActive.find(_vertexarray);
	if (movementRoutineFound != m_routineMovementVertexArrayActive.end()) {
		if (movementRoutineFound->second->getName() == _name) {
			movementRoutineFound->second->stop(_vertexarray);
			m_routineMovementVertexArrayActive.erase(movementRoutineFound);
		}
		else
			printf("MovementManager::stopMovementRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::stopMovementRoutine: Routine for vertexarray not found\n");
}

void MovementManager::deleteMovementRoutine()
{
	for (auto routine_active = m_routineMovementShapeActive.begin(); routine_active != m_routineMovementShapeActive.end();) {
		routine_active->second->stop(routine_active->first);
		routine_active = m_routineMovementShapeActive.erase(routine_active);
	}

	for (auto routine_active = m_routineMovementSpriteActive.begin(); routine_active != m_routineMovementSpriteActive.end();) {
		routine_active->second->stop(routine_active->first);
		routine_active = m_routineMovementSpriteActive.erase(routine_active);
	}

	for (auto routine_active = m_routineMovementVertexArrayActive.begin(); routine_active != m_routineMovementVertexArrayActive.end();) {
		routine_active->second->stop(routine_active->first);
		routine_active = m_routineMovementVertexArrayActive.erase(routine_active);
	}

	for (auto routineContainer = m_routineMovementShape.begin(); routineContainer != m_routineMovementShape.end();) {
		routineContainer->second->clear();
		delete routineContainer->second;
		routineContainer = m_routineMovementShape.erase(routineContainer);
	}

	for (auto routineContainer = m_routineMovementSprite.begin(); routineContainer != m_routineMovementSprite.end();) {
		routineContainer->second->clear();
		delete routineContainer->second;
		routineContainer = m_routineMovementSprite.erase(routineContainer);
	}

	for (auto routineContainer = m_routineMovementVertexArray.begin(); routineContainer != m_routineMovementVertexArray.end();) {
		routineContainer->second->clear();
		delete routineContainer->second;
		routineContainer = m_routineMovementVertexArray.erase(routineContainer);
	}

	m_movementRoutineContainer->clear();
}

void MovementManager::deleteMovementRoutine(const std::string& _name)
{
	for (auto routine_active = m_routineMovementShapeActive.begin(); routine_active != m_routineMovementShapeActive.end();) {
		if (routine_active->second->getName() == _name) {
			routine_active->second->stop(routine_active->first);
			routine_active = m_routineMovementShapeActive.erase(routine_active);
		}
		else
			++routine_active;
	}

	for (auto routine_active = m_routineMovementSpriteActive.begin(); routine_active != m_routineMovementSpriteActive.end();) {
		if (routine_active->second->getName() == _name) {
			routine_active->second->stop(routine_active->first);
			routine_active = m_routineMovementSpriteActive.erase(routine_active);
		}
		else
			++routine_active;
	}

	for (auto routine_active = m_routineMovementVertexArrayActive.begin(); routine_active != m_routineMovementVertexArrayActive.end();) {
		if (routine_active->second->getName() == _name) {
			routine_active->second->stop(routine_active->first);
			routine_active = m_routineMovementVertexArrayActive.erase(routine_active);
		}
		else
			++routine_active;
	}

	for (auto routineContainer = m_routineMovementShape.begin(); routineContainer != m_routineMovementShape.end();) {
		routineContainer->second->deleteRoutine(_name);
		if (routineContainer->second->getRoutineCount() == 0) {
			delete routineContainer->second;
			routineContainer = m_routineMovementShape.erase(routineContainer);
		}
		else
			++routineContainer;
	}

	for (auto routineContainer = m_routineMovementSprite.begin(); routineContainer != m_routineMovementSprite.end();) {
		routineContainer->second->deleteRoutine(_name);
		if (routineContainer->second->getRoutineCount() == 0) {
			delete routineContainer->second;
			routineContainer = m_routineMovementSprite.erase(routineContainer);
		}
		else
			++routineContainer;
	}

	for (auto routineContainer = m_routineMovementVertexArray.begin(); routineContainer != m_routineMovementVertexArray.end();) {
		routineContainer->second->deleteRoutine(_name);
		if (routineContainer->second->getRoutineCount() == 0) {
			delete routineContainer->second;
			routineContainer = m_routineMovementVertexArray.erase(routineContainer);
		}
		else
			++routineContainer;
	}

	m_movementRoutineContainer->deleteRoutine(_name);
}

const long long int& MovementManager::getSizeMovement() const
{
	long long int size{};
	size += m_movementRoutineContainer->size() + sizeof(m_movementRoutineContainer);

	for (const auto& routineShape : m_routineMovementShape)
		size += routineShape.second->size() + sizeof(routineShape.second);

	for (const auto& routineSprite : m_routineMovementSprite)
		size += routineSprite.second->size() + sizeof(routineSprite.second);

	for (const auto& routineVertexArray : m_routineMovementVertexArray)
		size += routineVertexArray.second->size() + sizeof(routineVertexArray.second);

	for (const auto& routineActive : m_routineMovementShapeActive)
		size += sizeof(routineActive.second);

	for (const auto& routineActive : m_routineMovementSpriteActive)
		size += sizeof(routineActive.second);

	for (const auto& routineActive : m_routineMovementVertexArrayActive)
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

ScalingRoutine* MovementManager::linkScalingRoutine(sf::Shape& _shape, const std::string& _name)
{
	// Check if routine with given name exists
	ScalingRoutine* scalingRoutineOriginal = m_scalingRoutineContainer->getRoutinePtr(_name);

	if (scalingRoutineOriginal == nullptr) {
		printf("MovementManager::linkScalingRoutine: Routine with name %s not found\n", _name.c_str());
		return nullptr;
	}

	// Check if we linked a scaling routine to this shape before
	auto m_scalingRoutineContainerFound = m_routineScalingShape.find(&_shape);
	if (m_scalingRoutineContainerFound != m_routineScalingShape.end()) {

		auto* scalingRoutineFound = m_scalingRoutineContainerFound->second->exists(_name);
		if (scalingRoutineFound != nullptr) {
			printf("MovementManager::linkScalingRoutine: Routine with name %s already linked to shape\n", _name.c_str());
			return scalingRoutineFound;
		}
	}
	else {
		// If not, create a new scaling routine container for this shape and link the scaling routine to it
		ScalingRoutineContainer* newScalingRoutineContainer = new ScalingRoutineContainer(this->movementRoutineEngine);
		m_routineScalingShape.insert(std::make_pair(&_shape, newScalingRoutineContainer));

		ScalingRoutine* newScalingRoutine = new ScalingRoutine(*scalingRoutineOriginal);
		return newScalingRoutineContainer->createRoutine(_name, newScalingRoutine);
	}

	// If yes, create a new scaling routine and link it to the existing scaling routine container
	ScalingRoutine* newScalingRoutine = new ScalingRoutine(*scalingRoutineOriginal);
	return m_scalingRoutineContainerFound->second->createRoutine(_name, newScalingRoutine);
}

ScalingRoutine* MovementManager::linkScalingRoutine(sf::Sprite& _sprite, const std::string& _name)
{
	// Check if routine with given name exists
	ScalingRoutine* scalingRoutineOriginal = m_scalingRoutineContainer->getRoutinePtr(_name);

	if (scalingRoutineOriginal == nullptr) {
		printf("MovementManager::linkScalingRoutine: Routine with name %s not found\n", _name.c_str());
		return nullptr;
	}

	// Check if we linked a scaling routine to this shape before
	auto m_scalingRoutineContainerFound = m_routineScalingSprite.find(&_sprite);
	if (m_scalingRoutineContainerFound != m_routineScalingSprite.end()) {

		auto* scalingRoutineFound = m_scalingRoutineContainerFound->second->exists(_name);
		if (scalingRoutineFound != nullptr) {
			printf("MovementManager::linkScalingRoutine: Routine with name %s already linked to shape\n", _name.c_str());
			return scalingRoutineFound;
		}
	}
	else {
		// If not, create a new scaling routine container for this shape and link the scaling routine to it
		ScalingRoutineContainer* newScalingRoutineContainer = new ScalingRoutineContainer(this->movementRoutineEngine);
		m_routineScalingSprite.insert(std::make_pair(&_sprite, newScalingRoutineContainer));

		ScalingRoutine* newScalingRoutine = new ScalingRoutine(*scalingRoutineOriginal);
		return newScalingRoutineContainer->createRoutine(_name, newScalingRoutine);
	}

	// If yes, create a new scaling routine and link it to the existing scaling routine container
	ScalingRoutine* newScalingRoutine = new ScalingRoutine(*scalingRoutineOriginal);
	return m_scalingRoutineContainerFound->second->createRoutine(_name, newScalingRoutine);
}

ScalingRoutine* MovementManager::linkScalingRoutine(VertexArray2& _vertexarray, const std::string& _name)
{
	// Check if routine with given name exists
	ScalingRoutine* scalingRoutineOriginal = m_scalingRoutineContainer->getRoutinePtr(_name);

	if (scalingRoutineOriginal == nullptr) {
		printf("MovementManager::linkScalingRoutine: Routine with name %s not found\n", _name.c_str());
		return nullptr;
	}

	// Check if we linked a scaling routine to this vertexarray before
	auto m_scalingRoutineContainerFound = m_routineScalingVertexArray.find(&_vertexarray);
	if (m_scalingRoutineContainerFound != m_routineScalingVertexArray.end()) {

		auto* scalingRoutineFound = m_scalingRoutineContainerFound->second->exists(_name);
		if (scalingRoutineFound != nullptr) {
			printf("MovementManager::linkScalingRoutine: Routine with name %s already linked to vertexarray\n", _name.c_str());
			return scalingRoutineFound;
		}
	}
	else {
		// If not, create a new scaling routine container for this vertexarray and link the scaling routine to it
		ScalingRoutineContainer* newScalingRoutineContainer = new ScalingRoutineContainer(this->movementRoutineEngine);
		m_routineScalingVertexArray.insert(std::make_pair(&_vertexarray, newScalingRoutineContainer));

		ScalingRoutine* newScalingRoutine = new ScalingRoutine(*scalingRoutineOriginal);
		return newScalingRoutineContainer->createRoutine(_name, newScalingRoutine);
	}

	// If yes, create a new scaling routine and link it to the existing scaling routine container
	ScalingRoutine* newScalingRoutine = new ScalingRoutine(*scalingRoutineOriginal);
	return m_scalingRoutineContainerFound->second->createRoutine(_name, newScalingRoutine);
}

void MovementManager::unlinkScalingRoutine(sf::Shape* _shape, const std::string& _name)
{
	// Stop the routine if it's active
	this->stopScalingRoutine(_shape, _name);

	// Delete the routine from m_routineScalingShape
	auto m_scalingRoutineContainerFound = m_routineScalingShape.find(_shape);
	if (m_scalingRoutineContainerFound != m_routineScalingShape.end()) {

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

void MovementManager::unlinkScalingRoutine(sf::Sprite* _sprite, const std::string& _name)
{
	// Stop the routine if it's active
	this->stopScalingRoutine(_sprite, _name);

	// Delete the routine from m_routineScalingSprite
	auto m_scalingRoutineContainerFound = m_routineScalingSprite.find(_sprite);
	if (m_scalingRoutineContainerFound != m_routineScalingSprite.end()) {

		auto* scalingRoutineFound = m_scalingRoutineContainerFound->second->exists(_name);
		if (scalingRoutineFound != nullptr) {
			m_scalingRoutineContainerFound->second->deleteRoutine(_name);
		}
		else
			printf("MovementManager::unlinkScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::unlinkScalingRoutine: Routine for sprite not found\n");
}

void MovementManager::unlinkScalingRoutine(VertexArray2* _vertexarray, const std::string& _name)
{
	// Stop the routine if it's active
	this->stopScalingRoutine(_vertexarray, _name);

	// Delete the routine from m_routineScalingVertexArray
	auto m_scalingRoutineContainerFound = m_routineScalingVertexArray.find(_vertexarray);
	if (m_scalingRoutineContainerFound != m_routineScalingVertexArray.end()) {

		auto* scalingRoutineFound = m_scalingRoutineContainerFound->second->exists(_name);
		if (scalingRoutineFound != nullptr) {
			m_scalingRoutineContainerFound->second->deleteRoutine(_name);
		}
		else
			printf("MovementManager::unlinkScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::unlinkScalingRoutine: Routine for vertexarray not found\n");
}

void MovementManager::startScalingRoutine(sf::Shape& _shape, const std::string& _name)
{
	auto m_scalingRoutineContainerFound = m_routineScalingShape.find(&_shape);
	if (m_scalingRoutineContainerFound != m_routineScalingShape.end()) {

		auto* scalingRoutineFound = m_scalingRoutineContainerFound->second->exists(_name);
		if (scalingRoutineFound != nullptr) {

			if (scalingRoutineFound->start(_shape))
				m_routineScalingShape_Active.insert(std::make_pair(&_shape, scalingRoutineFound));
		}
		else
			printf("MovementManager::startScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::startScalingRoutine: Routine for shape not found\n");
}

void MovementManager::startScalingRoutine(sf::Sprite& _sprite, const std::string& _name)
{
	auto m_scalingRoutineContainerFound = m_routineScalingSprite.find(&_sprite);
	if (m_scalingRoutineContainerFound != m_routineScalingSprite.end()) {

		auto* scalingRoutineFound = m_scalingRoutineContainerFound->second->exists(_name);
		if (scalingRoutineFound != nullptr) {

			if (scalingRoutineFound->start(_sprite))
				m_routineScalingSprite_Active.insert(std::make_pair(&_sprite, scalingRoutineFound));
		}
		else
			printf("MovementManager::startScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::startScalingRoutine: Routine for sprite not found\n");
}

void MovementManager::startScalingRoutine(VertexArray2& _vertexarray, const std::string& _name)
{
	auto m_scalingRoutineContainerFound = m_routineScalingVertexArray.find(&_vertexarray);
	if (m_scalingRoutineContainerFound != m_routineScalingVertexArray.end()) {

		auto* scalingRoutineFound = m_scalingRoutineContainerFound->second->exists(_name);
		if (scalingRoutineFound != nullptr) {
			if (scalingRoutineFound->start(_vertexarray)) 
				m_routineScalingVertexArray_Active.insert(std::make_pair(&_vertexarray, scalingRoutineFound));
		}
		else
			printf("MovementManager::startScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::startScalingRoutine: Routine for vertexarray not found\n");
}

void MovementManager::pauseScalingRoutine(sf::Shape& _shape, const std::string& _name)
{
	auto scalingRoutineFound = m_routineScalingShape_Active.find(&_shape);
	if (scalingRoutineFound != m_routineScalingShape_Active.end()) {
		if (scalingRoutineFound->second->getName() == _name) 
			scalingRoutineFound->second->pause();
		else
			printf("MovementManager::pauseScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::pauseScalingRoutine: Routine for shape not found\n");
}

void MovementManager::pauseScalingRoutine(sf::Sprite& _sprite, const std::string& _name)
{
	auto scalingRoutineFound = m_routineScalingSprite_Active.find(&_sprite);
	if (scalingRoutineFound != m_routineScalingSprite_Active.end()) {
		if (scalingRoutineFound->second->getName() == _name) 
			scalingRoutineFound->second->pause();
		else
			printf("MovementManager::pauseScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::pauseScalingRoutine: Routine for sprite not found\n");
}

void MovementManager::pauseScalingRoutine(VertexArray2& _vertexarray, const std::string& _name)
{
	auto scalingRoutineFound = m_routineScalingVertexArray_Active.find(&_vertexarray);
	if (scalingRoutineFound != m_routineScalingVertexArray_Active.end()) {
		if (scalingRoutineFound->second->getName() == _name) 
			scalingRoutineFound->second->pause();
		else
			printf("MovementManager::pauseScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::pauseScalingRoutine: Routine for vertexarray not found\n");
}

void MovementManager::resumeScalingRoutine(sf::Shape& _shape, const std::string& _name)
{
	auto scalingRoutineFound = m_routineScalingShape_Active.find(&_shape);
	if (scalingRoutineFound != m_routineScalingShape_Active.end()) {
		if (scalingRoutineFound->second->getName() == _name) 
			scalingRoutineFound->second->resume();
		else
			printf("MovementManager::resumeScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::resumeScalingRoutine: Routine for shape not found\n");
}

void MovementManager::resumeScalingRoutine(sf::Sprite& _sprite, const std::string& _name)
{
	auto scalingRoutineFound = m_routineScalingSprite_Active.find(&_sprite);
	if (scalingRoutineFound != m_routineScalingSprite_Active.end()) {
		if (scalingRoutineFound->second->getName() == _name) 
			scalingRoutineFound->second->resume();
		else
			printf("MovementManager::resumeScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::resumeScalingRoutine: Routine for sprite not found\n");
}

void MovementManager::resumeScalingRoutine(VertexArray2& _vertexarray, const std::string& _name)
{
	auto scalingRoutineFound = m_routineScalingVertexArray_Active.find(&_vertexarray);
	if (scalingRoutineFound != m_routineScalingVertexArray_Active.end()) {
		if (scalingRoutineFound->second->getName() == _name) 
			scalingRoutineFound->second->resume();
		else
			printf("MovementManager::resumeScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::resumeScalingRoutine: Routine for vertexarray not found\n");
}

void MovementManager::stopScalingRoutine(sf::Shape* _shape, const std::string& _name)
{
	auto scalingRoutineFound = m_routineScalingShape_Active.find(_shape);
	if (scalingRoutineFound != m_routineScalingShape_Active.end()) {
		if (scalingRoutineFound->second->getName() == _name) {
			scalingRoutineFound->second->stop(_shape);
			m_routineScalingShape_Active.erase(scalingRoutineFound);
		}
		else
			printf("MovementManager::stopScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::stopScalingRoutine: Routine for shape not found\n");
}

void MovementManager::stopScalingRoutine(sf::Sprite* _sprite, const std::string& _name)
{
	auto scalingRoutineFound = m_routineScalingSprite_Active.find(_sprite);
	if (scalingRoutineFound != m_routineScalingSprite_Active.end()) {
		if (scalingRoutineFound->second->getName() == _name) {
			scalingRoutineFound->second->stop(_sprite);
			m_routineScalingSprite_Active.erase(scalingRoutineFound);
		}
		else
			printf("MovementManager::stopScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::stopScalingRoutine: Routine for sprite not found\n");
}

void MovementManager::stopScalingRoutine(VertexArray2* _vertexarray, const std::string& _name)
{
	auto scalingRoutineFound = m_routineScalingVertexArray_Active.find(_vertexarray);
	if (scalingRoutineFound != m_routineScalingVertexArray_Active.end()) {
		if (scalingRoutineFound->second->getName() == _name) {
			scalingRoutineFound->second->stop(_vertexarray);
			m_routineScalingVertexArray_Active.erase(scalingRoutineFound);
		}
		else
			printf("MovementManager::stopScalingRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::stopScalingRoutine: Routine for vertexarray not found\n");
}

void MovementManager::deleteScalingRoutine()
{
	for (auto routine_active = m_routineScalingShape_Active.begin(); routine_active != m_routineScalingShape_Active.end();) {
		routine_active->second->stop(routine_active->first);
		routine_active = m_routineScalingShape_Active.erase(routine_active);
	}

	for (auto routine_active = m_routineScalingSprite_Active.begin(); routine_active != m_routineScalingSprite_Active.end();) {
		routine_active->second->stop(routine_active->first);
		routine_active = m_routineScalingSprite_Active.erase(routine_active);
	}

	for (auto routine_active = m_routineScalingVertexArray_Active.begin(); routine_active != m_routineScalingVertexArray_Active.end();) {
		routine_active->second->stop(routine_active->first);
		routine_active = m_routineScalingVertexArray_Active.erase(routine_active);
	}

	for (auto routineContainer = m_routineScalingShape.begin(); routineContainer != m_routineScalingShape.end();) {
		routineContainer->second->clear();
		delete routineContainer->second;
		routineContainer = m_routineScalingShape.erase(routineContainer);
	}

	for (auto routineContainer = m_routineScalingSprite.begin(); routineContainer != m_routineScalingSprite.end();) {
		routineContainer->second->clear();
		delete routineContainer->second;
		routineContainer = m_routineScalingSprite.erase(routineContainer);
	}

	for (auto routineContainer = m_routineScalingVertexArray.begin(); routineContainer != m_routineScalingVertexArray.end();) {
		routineContainer->second->clear();
		delete routineContainer->second;
		routineContainer = m_routineScalingVertexArray.erase(routineContainer);
	}

	m_scalingRoutineContainer->clear();
}

void MovementManager::deleteScalingRoutine(const std::string& _name)
{
	for (auto routine_active = m_routineScalingShape_Active.begin(); routine_active != m_routineScalingShape_Active.end();) {
		if (routine_active->second->getName() == _name) {
			routine_active->second->stop(routine_active->first);
			routine_active = m_routineScalingShape_Active.erase(routine_active);
		}
		else
			++routine_active;
	}

	for (auto routine_active = m_routineScalingSprite_Active.begin(); routine_active != m_routineScalingSprite_Active.end();) {
		if (routine_active->second->getName() == _name) {
			routine_active->second->stop(routine_active->first);
			routine_active = m_routineScalingSprite_Active.erase(routine_active);
		}
		else
			++routine_active;
	}

	for (auto routine_active = m_routineScalingVertexArray_Active.begin(); routine_active != m_routineScalingVertexArray_Active.end();) {
		if (routine_active->second->getName() == _name) {
			routine_active->second->stop(routine_active->first);
			routine_active = m_routineScalingVertexArray_Active.erase(routine_active);
		}
		else
			++routine_active;
	}

	for (auto routineContainer = m_routineScalingShape.begin(); routineContainer != m_routineScalingShape.end();) {
		routineContainer->second->deleteRoutine(_name);
		if (routineContainer->second->getRoutineCount() == 0) {
			delete routineContainer->second;
			routineContainer = m_routineScalingShape.erase(routineContainer);
		}
		else
			++routineContainer;
	}

	for (auto routineContainer = m_routineScalingSprite.begin(); routineContainer != m_routineScalingSprite.end();) {
		routineContainer->second->deleteRoutine(_name);
		if (routineContainer->second->getRoutineCount() == 0) {
			delete routineContainer->second;
			routineContainer = m_routineScalingSprite.erase(routineContainer);
		}
		else
			++routineContainer;
	}

	for (auto routineContainer = m_routineScalingVertexArray.begin(); routineContainer != m_routineScalingVertexArray.end();) {
		routineContainer->second->deleteRoutine(_name);
		if (routineContainer->second->getRoutineCount() == 0) {
			delete routineContainer->second;
			routineContainer = m_routineScalingVertexArray.erase(routineContainer);
		}
		else
			++routineContainer;
	}

	m_scalingRoutineContainer->deleteRoutine(_name);
}

const long long int& MovementManager::getSizeScaling() const
{
	long long int size{};
	size += m_scalingRoutineContainer->size() + sizeof(m_scalingRoutineContainer);

	for (const auto& routineShape : m_routineScalingShape)
		size += routineShape.second->size() + sizeof(routineShape.second);

	for (const auto& routineSprite : m_routineScalingSprite)
		size += routineSprite.second->size() + sizeof(routineSprite.second);

	for (const auto& routineVertexArray : m_routineScalingVertexArray)
		size += routineVertexArray.second->size() + sizeof(routineVertexArray.second);

	for (const auto& routineActive : m_routineScalingShape_Active)
		size += sizeof(routineActive.second);

	for (const auto& routineActive : m_routineScalingSprite_Active)
		size += sizeof(routineActive.second);

	for (const auto& routineActive : m_routineScalingVertexArray_Active)
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

RotationRoutine* MovementManager::linkRotationRoutine(sf::Shape& _shape, const std::string& _name)
{
	// Check if routine with given name exists
	RotationRoutine* rotationRoutineOriginal = m_rotationRoutineContainer->getRoutinePtr(_name);

	if (rotationRoutineOriginal == nullptr) {
		printf("MovementManager::linkRotationRoutine: Routine with name %s not found\n", _name.c_str());
		return nullptr;
	}

	// Check if we linked a rotation routine to this shape before
	auto m_rotationRoutineContainerFound = m_routineRotationShape.find(&_shape);
	if (m_rotationRoutineContainerFound != m_routineRotationShape.end()) {

		auto* rotationRoutineFound = m_rotationRoutineContainerFound->second->exists(_name);
		if (rotationRoutineFound != nullptr) {
			printf("MovementManager::linkRotationRoutine: Routine with name %s already linked to shape\n", _name.c_str());
			return rotationRoutineFound;
		}
	}
	else {
		// If not, create a new rotation routine container for this shape and link the rotation routine to it
		RotationRoutineContainer* newRotationRoutineContainer = new RotationRoutineContainer(this->movementRoutineEngine);
		m_routineRotationShape.insert(std::make_pair(&_shape, newRotationRoutineContainer));

		RotationRoutine* newRotationRoutine = new RotationRoutine(*rotationRoutineOriginal);
		return newRotationRoutineContainer->createRoutine(_name, newRotationRoutine);
	}

	// If yes, create a new rotation routine and link it to the existing rotation routine container
	RotationRoutine* newRotationRoutine = new RotationRoutine(*rotationRoutineOriginal);
	return m_rotationRoutineContainerFound->second->createRoutine(_name, newRotationRoutine);
}

RotationRoutine* MovementManager::linkRotationRoutine(sf::Sprite& _sprite, const std::string& _name)
{
	// Check if routine with given name exists
	RotationRoutine* rotationRoutineOriginal = m_rotationRoutineContainer->getRoutinePtr(_name);

	if (rotationRoutineOriginal == nullptr) {
		printf("MovementManager::linkRotationRoutine: Routine with name %s not found\n", _name.c_str());
		return nullptr;
	}

	// Check if we linked a rotation routine to this shape before
	auto m_rotationRoutineContainerFound = m_routineRotationSprite.find(&_sprite);
	if (m_rotationRoutineContainerFound != m_routineRotationSprite.end()) {

		auto* rotationRoutineFound = m_rotationRoutineContainerFound->second->exists(_name);
		if (rotationRoutineFound != nullptr) {
			printf("MovementManager::linkRotationRoutine: Routine with name %s already linked to shape\n", _name.c_str());
			return rotationRoutineFound;
		}
	}
	else {
		// If not, create a new rotation routine container for this shape and link the rotation routine to it
		RotationRoutineContainer* newRotationRoutineContainer = new RotationRoutineContainer(this->movementRoutineEngine);
		m_routineRotationSprite.insert(std::make_pair(&_sprite, newRotationRoutineContainer));

		RotationRoutine* newRotationRoutine = new RotationRoutine(*rotationRoutineOriginal);
		return newRotationRoutineContainer->createRoutine(_name, newRotationRoutine);
	}

	// If yes, create a new rotation routine and link it to the existing rotation routine container
	RotationRoutine* newRotationRoutine = new RotationRoutine(*rotationRoutineOriginal);
	return m_rotationRoutineContainerFound->second->createRoutine(_name, newRotationRoutine);
}

RotationRoutine* MovementManager::linkRotationRoutine(VertexArray2& _vertexarray, const std::string& _name)
{
	// Check if routine with given name exists
	RotationRoutine* rotationRoutineOriginal = m_rotationRoutineContainer->getRoutinePtr(_name);

	if (rotationRoutineOriginal == nullptr) {
		printf("MovementManager::linkRotationRoutine: Routine with name %s not found\n", _name.c_str());
		return nullptr;
	}

	// Check if we linked a rotation routine to this vertexarray before
	auto m_rotationRoutineContainerFound = m_routineRotationVertexArray.find(&_vertexarray);
	if (m_rotationRoutineContainerFound != m_routineRotationVertexArray.end()) {

		auto* rotationRoutineFound = m_rotationRoutineContainerFound->second->exists(_name);
		if (rotationRoutineFound != nullptr) {
			printf("MovementManager::linkRotationRoutine: Routine with name %s already linked to vertexarray\n", _name.c_str());
			return rotationRoutineFound;
		}
	}
	else {
		// If not, create a new rotation routine container for this vertexarray and link the rotation routine to it
		RotationRoutineContainer* newRotationRoutineContainer = new RotationRoutineContainer(this->movementRoutineEngine);
		m_routineRotationVertexArray.insert(std::make_pair(&_vertexarray, newRotationRoutineContainer));

		RotationRoutine* newRotationRoutine = new RotationRoutine(*rotationRoutineOriginal);
		return newRotationRoutineContainer->createRoutine(_name, newRotationRoutine);
	}

	// If yes, create a new rotation routine and link it to the existing rotation routine container
	RotationRoutine* newRotationRoutine = new RotationRoutine(*rotationRoutineOriginal);
	return m_rotationRoutineContainerFound->second->createRoutine(_name, newRotationRoutine);
}

void MovementManager::unlinkRotationRoutine(sf::Shape* _shape, const std::string& _name)
{
	// Stop the routine if it's active
	this->stopRotationRoutine(_shape, _name);

	// Delete the routine from m_routineRotationShape
	auto m_rotationRoutineContainerFound = m_routineRotationShape.find(_shape);
	if (m_rotationRoutineContainerFound != m_routineRotationShape.end()) {

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

void MovementManager::unlinkRotationRoutine(sf::Sprite* _sprite, const std::string& _name)
{
	// Stop the routine if it's active
	this->stopRotationRoutine(_sprite, _name);

	// Delete the routine from m_routineRotationSprite
	auto m_rotationRoutineContainerFound = m_routineRotationSprite.find(_sprite);
	if (m_rotationRoutineContainerFound != m_routineRotationSprite.end()) {

		auto* rotationRoutineFound = m_rotationRoutineContainerFound->second->exists(_name);
		if (rotationRoutineFound != nullptr) {
			m_rotationRoutineContainerFound->second->deleteRoutine(_name);
		}
		else
			printf("MovementManager::unlinkRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::unlinkRotationRoutine: Routine for sprite not found\n");
}

void MovementManager::unlinkRotationRoutine(VertexArray2* _vertexarray, const std::string& _name)
{
	// Stop the routine if it's active
	this->stopRotationRoutine(_vertexarray, _name);

	// Delete the routine from m_routineRotationVertexArray
	auto m_rotationRoutineContainerFound = m_routineRotationVertexArray.find(_vertexarray);
	if (m_rotationRoutineContainerFound != m_routineRotationVertexArray.end()) {

		auto* rotationRoutineFound = m_rotationRoutineContainerFound->second->exists(_name);
		if (rotationRoutineFound != nullptr) 
			m_rotationRoutineContainerFound->second->deleteRoutine(_name);
		else
			printf("MovementManager::unlinkRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::unlinkRotationRoutine: Routine for vertexarray not found\n");
}

void MovementManager::startRotationRoutine(sf::Shape& _shape, const std::string& _name)
{
	auto m_rotationRoutineContainerFound = m_routineRotationShape.find(&_shape);
	if (m_rotationRoutineContainerFound != m_routineRotationShape.end()) {

		auto* rotationRoutineFound = m_rotationRoutineContainerFound->second->exists(_name);
		if (rotationRoutineFound != nullptr) {

			if (rotationRoutineFound->start(_shape))
				m_routineRotationShape_Active.insert(std::make_pair(&_shape, rotationRoutineFound));
		}
		else
			printf("MovementManager::startRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::startRotationRoutine: Routine for shape not found\n");
}

void MovementManager::startRotationRoutine(sf::Sprite& _sprite, const std::string& _name)
{
	auto m_rotationRoutineContainerFound = m_routineRotationSprite.find(&_sprite);
	if (m_rotationRoutineContainerFound != m_routineRotationSprite.end()) {

		auto* rotationRoutineFound = m_rotationRoutineContainerFound->second->exists(_name);
		if (rotationRoutineFound != nullptr) {

			if (rotationRoutineFound->start(_sprite))
				m_routineRotationSprite_Active.insert(std::make_pair(&_sprite, rotationRoutineFound));
		}
		else
			printf("MovementManager::startRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::startRotationRoutine: Routine for sprite not found\n");
}

void MovementManager::startRotationRoutine(VertexArray2& _vertexarray, const std::string& _name)
{
	auto m_rotationRoutineContainerFound = m_routineRotationVertexArray.find(&_vertexarray);
	if (m_rotationRoutineContainerFound != m_routineRotationVertexArray.end()) {

		auto* rotationRoutineFound = m_rotationRoutineContainerFound->second->exists(_name);
		if (rotationRoutineFound != nullptr) {

			if (rotationRoutineFound->start(_vertexarray)) 
				m_routineRotationVertexArray_Active.insert(std::make_pair(&_vertexarray, rotationRoutineFound));
		}
		else
			printf("MovementManager::startRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::startRotationRoutine: Routine for vertexarray not found\n");
}

void MovementManager::pauseRotationRoutine(sf::Shape& _shape, const std::string& _name)
{
	auto rotationRoutineFound = m_routineRotationShape_Active.find(&_shape);
	if (rotationRoutineFound != m_routineRotationShape_Active.end()) {
		if (rotationRoutineFound->second->getName() == _name) 
			rotationRoutineFound->second->pause();
		else
			printf("MovementManager::pauseRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::pauseRotationRoutine: Routine for shape not found\n");
}

void MovementManager::pauseRotationRoutine(sf::Sprite& _sprite, const std::string& _name)
{
	auto rotationRoutineFound = m_routineRotationSprite_Active.find(&_sprite);
	if (rotationRoutineFound != m_routineRotationSprite_Active.end()) {
		if (rotationRoutineFound->second->getName() == _name) 
			rotationRoutineFound->second->pause();
		else
			printf("MovementManager::pauseRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::pauseRotationRoutine: Routine for sprite not found\n");
}

void MovementManager::pauseRotationRoutine(VertexArray2& _vertexarray, const std::string& _name)
{
	auto rotationRoutineFound = m_routineRotationVertexArray_Active.find(&_vertexarray);
	if (rotationRoutineFound != m_routineRotationVertexArray_Active.end()) {
		if (rotationRoutineFound->second->getName() == _name) 
			rotationRoutineFound->second->pause();
		else
			printf("MovementManager::pauseRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::pauseRotationRoutine: Routine for vertexarray not found\n");
}

void MovementManager::resumeRotationRoutine(sf::Shape& _shape, const std::string& _name)
{
	auto rotationRoutineFound = m_routineRotationShape_Active.find(&_shape);
	if (rotationRoutineFound != m_routineRotationShape_Active.end()) {
		if (rotationRoutineFound->second->getName() == _name) 
			rotationRoutineFound->second->resume();
		else
			printf("MovementManager::resumeRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::resumeRotationRoutine: Routine for shape not found\n");
}

void MovementManager::resumeRotationRoutine(sf::Sprite& _sprite, const std::string& _name)
{
	auto rotationRoutineFound = m_routineRotationSprite_Active.find(&_sprite);
	if (rotationRoutineFound != m_routineRotationSprite_Active.end()) {
		if (rotationRoutineFound->second->getName() == _name) 
			rotationRoutineFound->second->resume();
		else
			printf("MovementManager::resumeRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::resumeRotationRoutine: Routine for sprite not found\n");
}

void MovementManager::resumeRotationRoutine(VertexArray2& _vertexarray, const std::string& _name)
{
	auto rotationRoutineFound = m_routineRotationVertexArray_Active.find(&_vertexarray);
	if (rotationRoutineFound != m_routineRotationVertexArray_Active.end()) {
		if (rotationRoutineFound->second->getName() == _name) 
			rotationRoutineFound->second->resume();
		else
			printf("MovementManager::resumeRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::resumeRotationRoutine: Routine for vertexarray not found\n");
}

void MovementManager::stopRotationRoutine(sf::Shape* _shape, const std::string& _name)
{
	auto rotationRoutineFound = m_routineRotationShape_Active.find(_shape);
	if (rotationRoutineFound != m_routineRotationShape_Active.end()) {
		if (rotationRoutineFound->second->getName() == _name) {
			rotationRoutineFound->second->stop(_shape);
			m_routineRotationShape_Active.erase(rotationRoutineFound);
		}
		else
			printf("MovementManager::stopRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::stopRotationRoutine: Routine for shape not found\n");
}

void MovementManager::stopRotationRoutine(sf::Sprite* _sprite, const std::string& _name)
{
	auto rotationRoutineFound = m_routineRotationSprite_Active.find(_sprite);
	if (rotationRoutineFound != m_routineRotationSprite_Active.end()) {
		if (rotationRoutineFound->second->getName() == _name) {
			rotationRoutineFound->second->stop(_sprite);
			m_routineRotationSprite_Active.erase(rotationRoutineFound);
		}
		else
			printf("MovementManager::stopRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::stopRotationRoutine: Routine for sprite not found\n");
}

void MovementManager::stopRotationRoutine(VertexArray2* _vertexarray, const std::string& _name)
{
	auto rotationRoutineFound = m_routineRotationVertexArray_Active.find(_vertexarray);
	if (rotationRoutineFound != m_routineRotationVertexArray_Active.end()) {
		if (rotationRoutineFound->second->getName() == _name) {
			rotationRoutineFound->second->stop(_vertexarray);
			m_routineRotationVertexArray_Active.erase(rotationRoutineFound);
		}
		else
			printf("MovementManager::stopRotationRoutine: Routine with name %s not found\n", _name.c_str());
	}
	else
		printf("MovementManager::stopRotationRoutine: Routine for vertexarray not found\n");
}

void MovementManager::deleteRotationRoutine()
{
	for (auto routine_active = m_routineRotationShape_Active.begin(); routine_active != m_routineRotationShape_Active.end();) {
		routine_active->second->stop(routine_active->first);
		routine_active = m_routineRotationShape_Active.erase(routine_active);
	}

	for (auto routine_active = m_routineRotationSprite_Active.begin(); routine_active != m_routineRotationSprite_Active.end();) {
		routine_active->second->stop(routine_active->first);
		routine_active = m_routineRotationSprite_Active.erase(routine_active);
	}

	for (auto routine_active = m_routineRotationVertexArray_Active.begin(); routine_active != m_routineRotationVertexArray_Active.end();) {
		routine_active->second->stop(routine_active->first);
		routine_active = m_routineRotationVertexArray_Active.erase(routine_active);
	}

	for (auto routineContainer = m_routineRotationShape.begin(); routineContainer != m_routineRotationShape.end();) {
		routineContainer->second->clear();
		delete routineContainer->second;
		routineContainer = m_routineRotationShape.erase(routineContainer);
	}

	for (auto routineContainer = m_routineRotationSprite.begin(); routineContainer != m_routineRotationSprite.end();) {
		routineContainer->second->clear();
		delete routineContainer->second;
		routineContainer = m_routineRotationSprite.erase(routineContainer);
	}

	for (auto routineContainer = m_routineRotationVertexArray.begin(); routineContainer != m_routineRotationVertexArray.end();) {
		routineContainer->second->clear();
		delete routineContainer->second;
		routineContainer = m_routineRotationVertexArray.erase(routineContainer);
	}

	m_rotationRoutineContainer->clear();
}

void MovementManager::deleteRotationRoutine(const std::string& _name)
{
for (auto routine_active = m_routineRotationShape_Active.begin(); routine_active != m_routineRotationShape_Active.end();) {
		if (routine_active->second->getName() == _name) {
			routine_active->second->stop(routine_active->first);
			routine_active = m_routineRotationShape_Active.erase(routine_active);
		}
		else
			++routine_active;
	}

	for (auto routine_active = m_routineRotationSprite_Active.begin(); routine_active != m_routineRotationSprite_Active.end();) {
		if (routine_active->second->getName() == _name) {
			routine_active->second->stop(routine_active->first);
			routine_active = m_routineRotationSprite_Active.erase(routine_active);
		}
		else
			++routine_active;
	}

	for (auto routine_active = m_routineRotationVertexArray_Active.begin(); routine_active != m_routineRotationVertexArray_Active.end();) {
		if (routine_active->second->getName() == _name) {
			routine_active->second->stop(routine_active->first);
			routine_active = m_routineRotationVertexArray_Active.erase(routine_active);
		}
		else
			++routine_active;
	}

	for (auto routineContainer = m_routineRotationShape.begin(); routineContainer != m_routineRotationShape.end();) {
		routineContainer->second->deleteRoutine(_name);
		if (routineContainer->second->getRoutineCount() == 0) {
			delete routineContainer->second;
			routineContainer = m_routineRotationShape.erase(routineContainer);
		}
		else
			++routineContainer;
	}

	for (auto routineContainer = m_routineRotationSprite.begin(); routineContainer != m_routineRotationSprite.end();) {
		routineContainer->second->deleteRoutine(_name);
		if (routineContainer->second->getRoutineCount() == 0) {
			delete routineContainer->second;
			routineContainer = m_routineRotationSprite.erase(routineContainer);
		}
		else
			++routineContainer;
	}

	for (auto routineContainer = m_routineRotationVertexArray.begin(); routineContainer != m_routineRotationVertexArray.end();) {
		routineContainer->second->deleteRoutine(_name);
		if (routineContainer->second->getRoutineCount() == 0) {
			delete routineContainer->second;
			routineContainer = m_routineRotationVertexArray.erase(routineContainer);
		}
		else
			++routineContainer;
	}

	m_rotationRoutineContainer->deleteRoutine(_name);
}

const long long int& MovementManager::getSizeRotation() const
{
	long long int size{};
	size += m_rotationRoutineContainer->size() + sizeof(m_rotationRoutineContainer);

	for (const auto& routineShape : m_routineRotationShape)
		size += routineShape.second->size() + sizeof(routineShape.second);

	for (const auto& routineSprite : m_routineRotationSprite)
		size += routineSprite.second->size() + sizeof(routineSprite.second);

	for (const auto& routineVertexArray : m_routineRotationVertexArray)
		size += routineVertexArray.second->size() + sizeof(routineVertexArray.second);

	for (const auto& routineActive : m_routineRotationShape_Active)
		size += sizeof(routineActive.second);

	for (const auto& routineActive : m_routineRotationSprite_Active)
		size += sizeof(routineActive.second);

	for (const auto& routineActive : m_routineRotationVertexArray_Active)
		size += sizeof(routineActive.second);

	return size;
}
