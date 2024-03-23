#include "RoutineContainer.h"

// - - - - - - - - - - - - - - - - - - - - RoutineContainer - - - - - - - - - - - - - - - - - - - - \\

// Constructors / Destructors
RoutineContainer::RoutineContainer() :
	m_movementRoutineEngine(nullptr)
	{}

RoutineContainer::RoutineContainer(MovementRoutineEngine* movementRoutineEnginePtr) :
	m_movementRoutineEngine(movementRoutineEnginePtr)
	{}

RoutineContainer::RoutineContainer(const RoutineContainer & obj) :
	m_movementRoutineEngine(obj.m_movementRoutineEngine),
	m_routines(obj.m_routines)
	{}

RoutineContainer::~RoutineContainer()
{
	m_movementRoutineEngine = nullptr;
	m_routines.clear();
}

void RoutineContainer::setDelayBefore(const float delay, const bool reset)
{
	for (auto& routine : m_routines)
		routine.second->setDelayBefore(delay, reset);
}

void RoutineContainer::setDelayBefore(std::string& name, const float delay, const bool reset)
{
	auto routine = m_routines.find(&name);

	if (routine != m_routines.end())
		routine->second->setDelayBefore(delay, reset);
}

void RoutineContainer::setMotionDuration(const float time, const bool reset)
{
	for (auto& routine : m_routines)
		routine.second->setMotionDuration(time, reset);
}

void RoutineContainer::setMotionDuration(std::string& name, const float time, const bool reset)
{
	auto routine = m_routines.find(&name);

	if (routine != m_routines.end())
		routine->second->setMotionDuration(time, reset);
}

void RoutineContainer::setDelayAfter(const float delay, const bool reset)
{
	for (auto& routine : m_routines)
		routine.second->setDelayAfter(delay, reset);
}

void RoutineContainer::setDelayAfter(std::string& name, const float delay, const bool reset)
{
	auto routine = m_routines.find(&name);

	if (routine != m_routines.end())
		routine->second->setDelayAfter(delay, reset);
}

const size_t& RoutineContainer::getRoutineCount() const
{
	return m_routines.size();
}

// - - - - - - - - - - - - - - - - - - - - MovementRoutineContainer - - - - - - - - - - - - - - - - - - - - \\

// Constructors / Destructors
MovementRoutineContainer::MovementRoutineContainer() :
	RoutineContainer{}
	{}

MovementRoutineContainer::MovementRoutineContainer(MovementRoutineEngine * movementRoutineEnginePtr) :
	RoutineContainer{ movementRoutineEnginePtr }
	{}

MovementRoutineContainer::~MovementRoutineContainer()
{
	for (auto& routine : m_movementRoutines) 
		delete routine.second;
	m_movementRoutines.clear();
	m_routines.clear();
}

// Public functions
MovementRoutine* MovementRoutineContainer::exists(const std::string& name)
{
	auto movementRoutineFound = m_movementRoutines.find(name);

	if (movementRoutineFound != m_movementRoutines.end())
		return movementRoutineFound->second;
	else
		return nullptr;
}

MovementRoutine* MovementRoutineContainer::exists(const MovementRoutine* movementRoutine)
{
	for (auto& routine : m_movementRoutines) 
		if (routine.second == movementRoutine)
			return routine.second;
	
	return nullptr;
}

MovementRoutine MovementRoutineContainer::getRoutine(const std::string& name)
{
	if (exists(name) != nullptr)
		return *m_movementRoutines[name];
	else {
		printf("MovementRoutineContainer::getRoutine: Routine with given name does not exist\n");
		return MovementRoutine();
	}
}

MovementRoutine* MovementRoutineContainer::getRoutinePtr(const std::string& name)
{
	auto* movementRoutineExists = exists(name);

	if (movementRoutineExists == nullptr)
		printf("MovementRoutineContainer::getRoutine: Routine with given name does not exist\n");

	return movementRoutineExists;
}

MovementRoutine* MovementRoutineContainer::createRoutine(const std::string& name)
{
	auto* movementRoutineExists = exists(name);

	if (movementRoutineExists != nullptr) {
		printf("MovementRoutineContainer::createRoutine: Routine with given name already exists\n");
		return movementRoutineExists;
	}
	else {
		MovementRoutine* newMovementRoutine = new MovementRoutine(name, m_movementRoutineEngine);
		m_movementRoutines.insert(std::make_pair(name, newMovementRoutine));
		m_routines.insert(std::make_pair(&newMovementRoutine->getName(), static_cast<TransformationRoutine*>(newMovementRoutine)));

		return newMovementRoutine;
	}
}

MovementRoutine* MovementRoutineContainer::createRoutine(const std::string& name, MovementRoutine* movementRoutine)
{
	auto* movementRoutineExists = exists(name);

	if (movementRoutineExists != nullptr) {
		printf("MovementRoutineContainer::createRoutine: Routine with given name already exists\n");
		return movementRoutineExists;
	}
	else {
		m_movementRoutines.insert(std::make_pair(name, movementRoutine));
		m_routines.insert(std::make_pair(&movementRoutine->getName(), static_cast<TransformationRoutine*>(movementRoutine)));
		return movementRoutine;
	}
}

void MovementRoutineContainer::clear()
{
	for (auto& routine : m_movementRoutines) 
		delete routine.second;
	m_movementRoutines.clear();
	m_routines.clear();
}

void MovementRoutineContainer::deleteRoutine(const std::string& name)
{
	auto movementRoutineFound = this->m_movementRoutines.find(name);

	if (movementRoutineFound != this->m_movementRoutines.end()) {
		auto routineFound = this->m_routines.find(&movementRoutineFound->second->getName());
		if (routineFound != this->m_routines.end())
			this->m_routines.erase(routineFound);

		delete movementRoutineFound->second;
		this->m_movementRoutines.erase(movementRoutineFound);
	}

	
}

const long long int& MovementRoutineContainer::size() const
{
	long long int size{ sizeof(this) };
	for (const auto& routine : m_movementRoutines) 
		size += routine.second->size();
	size += sizeof(m_movementRoutines);
	size += sizeof(m_routines);
	return size;
}

// - - - - - - - - - - - - - - - - - - - - ScalingRoutineContainer - - - - - - - - - - - - - - - - - - - - \\

// Constructors / Destructors
ScalingRoutineContainer::ScalingRoutineContainer() :
	RoutineContainer{}
	{}

ScalingRoutineContainer::ScalingRoutineContainer(MovementRoutineEngine* movementRoutineEnginePtr) :
	RoutineContainer{ movementRoutineEnginePtr }
	{}

ScalingRoutineContainer::~ScalingRoutineContainer()
{
	for (auto& routine : scalingRoutines)
		delete routine.second;
	scalingRoutines.clear();
	m_routines.clear();
}

ScalingRoutine* ScalingRoutineContainer::exists(const std::string& name)
{
	auto scalingRoutineFound = scalingRoutines.find(name);

	if (scalingRoutineFound != scalingRoutines.end())
		return scalingRoutineFound->second;
	else
		return nullptr;
}

ScalingRoutine* ScalingRoutineContainer::exists(const ScalingRoutine* scalingRoutine)
{
	for (auto& routine : scalingRoutines)
		if (routine.second == scalingRoutine)
			return routine.second;

	return nullptr;
}

ScalingRoutine ScalingRoutineContainer::getRoutine(const std::string& name)
{
	if (exists(name) != nullptr)
		return *scalingRoutines[name];
	else {
		printf("ScalingRoutineContainer::getRoutine: Routine with given name does not exist\n");
		return ScalingRoutine();
	}
}

ScalingRoutine* ScalingRoutineContainer::getRoutinePtr(const std::string& name)
{
	auto* scalingRoutineExists = exists(name);

	if (scalingRoutineExists == nullptr)
		printf("ScalingRoutineContainer::getRoutine: Routine with given name does not exist\n");

	return scalingRoutineExists;
}

ScalingRoutine* ScalingRoutineContainer::createRoutine(const std::string& name)
{
	auto* scalingRoutineExists = exists(name);

	if (scalingRoutineExists != nullptr) {
		printf("ScalingRoutineContainer::createRoutine: Routine with given name already exists\n");
		return scalingRoutineExists;
	}
	else {
		ScalingRoutine* newScalingRoutine = new ScalingRoutine(name, m_movementRoutineEngine);
		scalingRoutines.insert(std::make_pair(name, newScalingRoutine));
		m_routines.insert(std::make_pair(&newScalingRoutine->getName(), static_cast<TransformationRoutine*>(newScalingRoutine)));
		return newScalingRoutine;
	}
}

ScalingRoutine* ScalingRoutineContainer::createRoutine(const std::string& name, ScalingRoutine* scalingRoutine)
{
	auto* scalingRoutineExists = exists(name);

	if (scalingRoutineExists != nullptr) {
		printf("ScalingRoutineContainer::createRoutine: Routine with given name already exists\n");
		return scalingRoutineExists;
	}
	else {
		scalingRoutines.insert(std::make_pair(name, scalingRoutine));
		m_routines.insert(std::make_pair(&scalingRoutine->getName(), static_cast<TransformationRoutine*>(scalingRoutine)));
		return scalingRoutine;
	}
}

void ScalingRoutineContainer::clear()
{
	for (auto& routine : scalingRoutines)
		delete routine.second;
	scalingRoutines.clear();	
	m_routines.clear();
}

void ScalingRoutineContainer::deleteRoutine(const std::string& name)
{
	auto scalingRoutineFound = this->scalingRoutines.find(name);

	if (scalingRoutineFound != this->scalingRoutines.end()) {

		auto routineFound = this->m_routines.find(&scalingRoutineFound->second->getName());
		if (routineFound != this->m_routines.end())
			this->m_routines.erase(routineFound);

		delete scalingRoutineFound->second;
		this->scalingRoutines.erase(scalingRoutineFound);
	}
}

const long long int& ScalingRoutineContainer::size() const
{
	long long int size{ sizeof(this) };
	for (const auto& routine : scalingRoutines)
		size += routine.second->size();
	size += sizeof(scalingRoutines);
	size += sizeof(m_routines);
	return size;
}

// - - - - - - - - - - - - - - - - - - - - RotationRoutineContainer - - - - - - - - - - - - - - - - - - - - \\

// Constructors / Destructors
RotationRoutineContainer::RotationRoutineContainer() :
	RoutineContainer{}
	{}

RotationRoutineContainer::RotationRoutineContainer(MovementRoutineEngine* movementRoutineEnginePtr) :
	RoutineContainer{ movementRoutineEnginePtr }
	{}

RotationRoutineContainer::~RotationRoutineContainer()
{
	for (auto& routine : rotationRoutines)
		delete routine.second;
	rotationRoutines.clear();
	m_routines.clear();
}

RotationRoutine* RotationRoutineContainer::exists(const std::string& name)
{
	auto rotationRoutineFound = rotationRoutines.find(name);

	if (rotationRoutineFound != rotationRoutines.end())
		return rotationRoutineFound->second;
	else
		return nullptr;
}

RotationRoutine* RotationRoutineContainer::exists(const RotationRoutine* rotationRoutine)
{
	for (auto& routine : rotationRoutines)
		if (routine.second == rotationRoutine)
			return routine.second;

	return nullptr;
}

RotationRoutine RotationRoutineContainer::getRoutine(const std::string& name)
{
	if (exists(name) != nullptr)
		return *rotationRoutines[name];
	else {
		printf("RotationRoutineContainer::getRoutine: Routine with given name does not exist\n");
		return RotationRoutine();
	}
}

RotationRoutine* RotationRoutineContainer::getRoutinePtr(const std::string& name)
{
	auto* rotationRoutineExists = exists(name);

	if (rotationRoutineExists == nullptr)
		printf("RotationRoutineContainer::getRoutine: Routine with given name does not exist\n");

	return rotationRoutineExists;
}

RotationRoutine* RotationRoutineContainer::createRoutine(const std::string& name)
{
	auto* rotationRoutineExists = exists(name);

	if (rotationRoutineExists != nullptr) {
		printf("RotationRoutineContainer::createRoutine: Routine with given name already exists\n");
		return rotationRoutineExists;
	}
	else {
		RotationRoutine* newRotationRoutine = new RotationRoutine(name, m_movementRoutineEngine);
		rotationRoutines.insert(std::make_pair(name, newRotationRoutine));
		m_routines.insert(std::make_pair(&newRotationRoutine->getName(), static_cast<TransformationRoutine*>(newRotationRoutine)));
		return newRotationRoutine;
	}
}

RotationRoutine* RotationRoutineContainer::createRoutine(const std::string& name, RotationRoutine* rotationRoutine)
{
	auto* rotationRoutineExists = exists(name);

	if (rotationRoutineExists != nullptr) {
		printf("RotationRoutineContainer::createRoutine: Routine with given name already exists\n");
		return rotationRoutineExists;
	}
	else {
		rotationRoutines.insert(std::make_pair(name, rotationRoutine));
		m_routines.insert(std::make_pair(&rotationRoutine->getName(), static_cast<TransformationRoutine*>(rotationRoutine)));
		return rotationRoutine;
	}
}

void RotationRoutineContainer::clear()
{
	for (auto& routine : rotationRoutines)
		delete routine.second;
	rotationRoutines.clear();
	m_routines.clear();
}

void RotationRoutineContainer::deleteRoutine(const std::string& name)
{
	auto rotationRoutineFound = this->rotationRoutines.find(name);

	if (rotationRoutineFound != this->rotationRoutines.end()) {
		auto routineFound = this->m_routines.find(&rotationRoutineFound->second->getName());
		if (routineFound != this->m_routines.end())
			this->m_routines.erase(routineFound);

		delete rotationRoutineFound->second;
		this->rotationRoutines.erase(rotationRoutineFound);
	}
}

const long long int& RotationRoutineContainer::size() const
{
	long long int size{ sizeof(this) };
	for (const auto& routine : rotationRoutines)
		size += routine.second->size();
	size += sizeof(rotationRoutines);
	size += sizeof(m_routines);
	return size;
}
