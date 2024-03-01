#include "RoutineContainer.h"

// - - - - - - - - - - - - - - - - - - - - MovementRoutineContainer - - - - - - - - - - - - - - - - - - - - \\

MovementRoutine* MovementRoutineContainer::exists(const std::string& _name)
{
	auto movementRoutineFound = movementRoutines.find(_name);

	if (movementRoutineFound != movementRoutines.end())
		return movementRoutineFound->second;
	else
		return nullptr;
}

MovementRoutine MovementRoutineContainer::getRoutine(const std::string& _name)
{
	if (exists(_name) != nullptr)
		return *movementRoutines[_name];
	else {
		printf("MovementRoutineContainer::getRoutine: Routine with given name does not exist\n");
		return MovementRoutine();
	}
}

MovementRoutine* MovementRoutineContainer::getRoutinePtr(const std::string& _name)
{
	auto* movementRoutineExists = exists(_name);

	if (movementRoutineExists == nullptr)
		printf("MovementRoutineContainer::getRoutine: Routine with given name does not exist\n");

	return movementRoutineExists;
}

MovementRoutine* MovementRoutineContainer::createRoutine(const std::string& _name)
{
	auto* movementRoutineExists = exists(_name);

	if (movementRoutineExists != nullptr) {
		printf("MovementRoutineContainer::createRoutine: Routine with given name already exists\n");
		return movementRoutineExists;
	}
	else {
		MovementRoutine* newMovementRoutine = new MovementRoutine(_name, movementRoutineEngine);
		movementRoutines.insert(std::make_pair(_name, newMovementRoutine));
		return newMovementRoutine;
	}
}

MovementRoutine* MovementRoutineContainer::createRoutine(const std::string& _name, MovementRoutine* _movement_routine)
{
	auto* movementRoutineExists = exists(_name);

	if (movementRoutineExists != nullptr) {
		printf("MovementRoutineContainer::createRoutine: Routine with given name already exists\n");
		return movementRoutineExists;
	}
	else {
		movementRoutines.insert(std::make_pair(_name, _movement_routine));
		return _movement_routine;
	}
}

void MovementRoutineContainer::clear()
{
	for (auto& routine : movementRoutines) delete routine.second;
	movementRoutines.clear();
}

void MovementRoutineContainer::deleteRoutine(const std::string& _name)
{
	auto movementRoutineFound = this->movementRoutines.find(_name);

	if (movementRoutineFound != this->movementRoutines.end()) {
		delete movementRoutineFound->second;
		this->movementRoutines.erase(movementRoutineFound);
	}
}

//// - - - - - - - - - - - - - - - - - - - - MovementRoutineVAContainer - - - - - - - - - - - - - - - - - - - - \\
//
//MovementRoutineVA* MovementRoutineVAContainer::exists(const std::string& _name)
//{
//	auto movementRoutineFound = movementRoutines.find(_name);
//
//	if (movementRoutineFound != movementRoutines.end())
//		return movementRoutineFound->second;
//	else
//		return nullptr;
//}
//
//MovementRoutineVA MovementRoutineVAContainer::getRoutine(const std::string& _name)
//{
//	if (exists(_name) != nullptr)
//		return *movementRoutines[_name];
//	else {
//		printf("MovementRoutineVAContainer::getRoutine: Routine with given name does not exist\n");
//		return MovementRoutineVA();
//	}
//}
//
//MovementRoutineVA* MovementRoutineVAContainer::getRoutinePtr(const std::string& _name)
//{
//	auto* movementRoutineExists = exists(_name);
//
//	if (movementRoutineExists == nullptr)
//		printf("MovementRoutineVAContainer::getRoutine: Routine with given name does not exist\n");
//
//	return movementRoutineExists;
//}
//
//MovementRoutineVA* MovementRoutineVAContainer::createRoutine(const std::string& _name)
//{
//	auto* movementRoutineExists = exists(_name);
//
//	if (movementRoutineExists != nullptr) {
//		printf("MovementRoutineVAContainer::createRoutine: Routine with given name already exists\n");
//		return movementRoutineExists;
//	}
//	else {
//		MovementRoutineVA* newMovementRoutine = new MovementRoutineVA(_name, movementRoutineEngine);
//		movementRoutines.insert(std::make_pair(_name, newMovementRoutine));
//		return newMovementRoutine;
//	}
//}
//
//MovementRoutineVA* MovementRoutineVAContainer::createRoutine(const std::string& _name, MovementRoutineVA* _movement_routine)
//{
//	auto* movementRoutineExists = exists(_name);
//
//	if (movementRoutineExists != nullptr) {
//		printf("MovementRoutineVAContainer::createRoutine: Routine with given name already exists\n");
//		return movementRoutineExists;
//	}
//	else {
//		movementRoutines.insert(std::make_pair(_name, _movement_routine));
//		return _movement_routine;
//	}
//}
//
//void MovementRoutineVAContainer::clear()
//{
//	for (auto& routine : movementRoutines) delete routine.second;
//	movementRoutines.clear();
//}
//
//void MovementRoutineVAContainer::deleteRoutine(const std::string& _name)
//{
//	auto movementRoutineFound = this->movementRoutines.find(_name);
//
//	if (movementRoutineFound != this->movementRoutines.end()){
//		delete movementRoutineFound->second;
//		this->movementRoutines.erase(movementRoutineFound);
//	}
//}

// - - - - - - - - - - - - - - - - - - - - ScalingRoutineContainer - - - - - - - - - - - - - - - - - - - - \\

ScalingRoutine* ScalingRoutineContainer::exists(const std::string& _name)
{
	auto scalingRoutineFound = scalingRoutines.find(_name);

	if (scalingRoutineFound != scalingRoutines.end())
		return scalingRoutineFound->second;
	else
		return nullptr;
}

ScalingRoutine ScalingRoutineContainer::getRoutine(const std::string& _name)
{
	if (exists(_name) != nullptr)
		return *scalingRoutines[_name];
	else {
		printf("ScalingRoutineContainer::getRoutine: Routine with given name does not exist\n");
		return ScalingRoutine();
	}
}

ScalingRoutine* ScalingRoutineContainer::getRoutinePtr(const std::string& _name)
{
	auto* scalingRoutineExists = exists(_name);

	if (scalingRoutineExists == nullptr)
		printf("ScalingRoutineContainer::getRoutine: Routine with given name does not exist\n");

	return scalingRoutineExists;
}

ScalingRoutine* ScalingRoutineContainer::createRoutine(const std::string& _name)
{
	auto* scalingRoutineExists = exists(_name);

	if (scalingRoutineExists != nullptr) {
		printf("ScalingRoutineContainer::createRoutine: Routine with given name already exists\n");
		return scalingRoutineExists;
	}
	else {
		ScalingRoutine* newScalingRoutine = new ScalingRoutine(_name, movementRoutineEngine);
		scalingRoutines.insert(std::make_pair(_name, newScalingRoutine));
		return newScalingRoutine;
	}
}

ScalingRoutine* ScalingRoutineContainer::createRoutine(const std::string& _name, ScalingRoutine* _scaling_routine)
{
	auto* scalingRoutineExists = exists(_name);

	if (scalingRoutineExists != nullptr) {
		printf("ScalingRoutineContainer::createRoutine: Routine with given name already exists\n");
		return scalingRoutineExists;
	}
	else {
		scalingRoutines.insert(std::make_pair(_name, _scaling_routine));
		return _scaling_routine;
	}
}

void ScalingRoutineContainer::clear()
{
	for (auto& routine : scalingRoutines) delete routine.second;
	scalingRoutines.clear();
}

void ScalingRoutineContainer::deleteRoutine(const std::string& _name)
{
	auto scalingRoutineFound = this->scalingRoutines.find(_name);

	if (scalingRoutineFound != this->scalingRoutines.end()) {
		delete scalingRoutineFound->second;
		this->scalingRoutines.erase(scalingRoutineFound);
	}
	else
		printf("ScalingRoutineContainer::deleteRoutine: Routine with given name does not exist\n");
}

//// - - - - - - - - - - - - - - - - - - - - ScalingRoutineVAContainer - - - - - - - - - - - - - - - - - - - - \\
//
//ScalingRoutineVA* ScalingRoutineVAContainer::exists(const std::string& _name)
//{
//	auto scalingRoutineFound = scalingRoutines.find(_name);
//
//	if (scalingRoutineFound != scalingRoutines.end())
//		return scalingRoutineFound->second;
//	else
//		return nullptr;
//}
//
//ScalingRoutineVA ScalingRoutineVAContainer::getRoutine(const std::string& _name)
//{
//	if (exists(_name) != nullptr)
//		return *scalingRoutines[_name];
//	else {
//		printf("ScalingRoutineVAContainer::getRoutine: Routine with given name does not exist\n");
//		return ScalingRoutineVA();
//	}
//}
//
//ScalingRoutineVA* ScalingRoutineVAContainer::getRoutinePtr(const std::string& _name)
//{
//	auto* scalingRoutineExists = exists(_name);
//
//	if (scalingRoutineExists == nullptr)
//		printf("ScalingRoutineVAContainer::getRoutine: Routine with given name does not exist\n");
//
//	return scalingRoutineExists;
//}
//
//ScalingRoutineVA* ScalingRoutineVAContainer::createRoutine(const std::string& _name)
//{
//	auto* scalingRoutineExists = exists(_name);
//
//	if (scalingRoutineExists != nullptr) {
//		printf("ScalingRoutineVAContainer::createRoutine: Routine with given name already exists\n");
//		return scalingRoutineExists;
//	}
//	else {
//		ScalingRoutineVA* newScalingRoutine = new ScalingRoutineVA(_name, movementRoutineEngine);
//		scalingRoutines.insert(std::make_pair(_name, newScalingRoutine));
//		return newScalingRoutine;
//	}
//}
//
//ScalingRoutineVA* ScalingRoutineVAContainer::createRoutine(const std::string& _name, ScalingRoutineVA* _scaling_routine)
//{
//	auto* scalingRoutineExists = exists(_name);
//
//	if (scalingRoutineExists != nullptr) {
//		printf("ScalingRoutineVAContainer::createRoutine: Routine with given name already exists\n");
//		return scalingRoutineExists;
//	}
//	else {
//		scalingRoutines.insert(std::make_pair(_name, _scaling_routine));
//		return _scaling_routine;
//	}
//}
//
//void ScalingRoutineVAContainer::clear()
//{
//	for (auto& routine : scalingRoutines) delete routine.second;
//	scalingRoutines.clear();
//}
//
//void ScalingRoutineVAContainer::deleteRoutine(const std::string& _name)
//{
//	auto scalingRoutineFound = this->scalingRoutines.find(_name);
//
//	if (scalingRoutineFound != this->scalingRoutines.end()) {
//		delete scalingRoutineFound->second;
//		this->scalingRoutines.erase(scalingRoutineFound);
//	}
//	else
//		printf("ScalingRoutineVAContainer::deleteRoutine: Routine with given name does not exist\n");
//}

// - - - - - - - - - - - - - - - - - - - - RotationRoutineContainer - - - - - - - - - - - - - - - - - - - - \\

RotationRoutine* RotationRoutineContainer::exists(const std::string& _name)
{
	auto rotationRoutineFound = rotationRoutines.find(_name);

	if (rotationRoutineFound != rotationRoutines.end())
		return rotationRoutineFound->second;
	else
		return nullptr;
}

RotationRoutine RotationRoutineContainer::getRoutine(const std::string& _name)
{
	if (exists(_name) != nullptr)
		return *rotationRoutines[_name];
	else {
		printf("RotationRoutineContainer::getRoutine: Routine with given name does not exist\n");
		return RotationRoutine();
	}
}

RotationRoutine* RotationRoutineContainer::getRoutinePtr(const std::string& _name)
{
	auto* rotationRoutineExists = exists(_name);

	if (rotationRoutineExists == nullptr)
		printf("RotationRoutineContainer::getRoutine: Routine with given name does not exist\n");

	return rotationRoutineExists;
}

RotationRoutine* RotationRoutineContainer::createRoutine(const std::string& _name)
{
	auto* rotationRoutineExists = exists(_name);

	if (rotationRoutineExists != nullptr) {
		printf("RotationRoutineContainer::createRoutine: Routine with given name already exists\n");
		return rotationRoutineExists;
	}
	else {
		RotationRoutine* newRotationRoutine = new RotationRoutine(_name, movementRoutineEngine);
		rotationRoutines.insert(std::make_pair(_name, newRotationRoutine));
		return newRotationRoutine;
	}
}

RotationRoutine* RotationRoutineContainer::createRoutine(const std::string& _name, RotationRoutine* _rotation_routine)
{
	auto* rotationRoutineExists = exists(_name);

	if (rotationRoutineExists != nullptr) {
		printf("RotationRoutineContainer::createRoutine: Routine with given name already exists\n");
		return rotationRoutineExists;
	}
	else {
		rotationRoutines.insert(std::make_pair(_name, _rotation_routine));
		return _rotation_routine;
	}
}

void RotationRoutineContainer::clear()
{
	for (auto& routine : rotationRoutines) delete routine.second;
	rotationRoutines.clear();
}

void RotationRoutineContainer::deleteRoutine(const std::string& _name)
{
	auto rotationRoutineFound = this->rotationRoutines.find(_name);

	if (rotationRoutineFound != this->rotationRoutines.end()) {
		delete rotationRoutineFound->second;
		this->rotationRoutines.erase(rotationRoutineFound);
	}
}


//// - - - - - - - - - - - - - - - - - - - - RotationRoutineVAContainer - - - - - - - - - - - - - - - - - - - - \\
//
//RotationRoutineVA* RotationRoutineVAContainer::exists(const std::string& _name)
//{
//	auto rotationRoutineFound = rotationRoutines.find(_name);
//
//	if (rotationRoutineFound != rotationRoutines.end())
//		return rotationRoutineFound->second;
//	else
//		return nullptr;
//}
//
//RotationRoutineVA RotationRoutineVAContainer::getRoutine(const std::string& _name)
//{
//	if (exists(_name) != nullptr)
//		return *rotationRoutines[_name];
//	else {
//		printf("RotationRoutineVAContainer::getRoutine: Routine with given name does not exist\n");
//		return RotationRoutineVA();
//	}
//}
//
//RotationRoutineVA* RotationRoutineVAContainer::getRoutinePtr(const std::string& _name)
//{
//	auto* rotationRoutineExists = exists(_name);
//
//	if (rotationRoutineExists == nullptr)
//		printf("RotationRoutineVAContainer::getRoutine: Routine with given name does not exist\n");
//
//	return rotationRoutineExists;
//}
//
//RotationRoutineVA* RotationRoutineVAContainer::createRoutine(const std::string& _name)
//{
//	auto* rotationRoutineExists = exists(_name);
//
//	if (rotationRoutineExists != nullptr) {
//		printf("RotationRoutineVAContainer::createRoutine: Routine with given name already exists\n");
//		return rotationRoutineExists;
//	}
//	else {
//		RotationRoutineVA* newRotationRoutine = new RotationRoutineVA(_name, movementRoutineEngine);
//		rotationRoutines.insert(std::make_pair(_name, newRotationRoutine));
//		return newRotationRoutine;
//	}
//}
//
//RotationRoutineVA* RotationRoutineVAContainer::createRoutine(const std::string& _name, RotationRoutineVA* _rotation_routine)
//{
//	auto* rotationRoutineExists = exists(_name);
//
//	if (rotationRoutineExists != nullptr) {
//		printf("RotationRoutineVAContainer::createRoutine: Routine with given name already exists\n");
//		return rotationRoutineExists;
//	}
//	else {
//		rotationRoutines.insert(std::make_pair(_name, _rotation_routine));
//		return _rotation_routine;
//	}
//}
//
//void RotationRoutineVAContainer::clear()
//{
//	for (auto& routine : rotationRoutines) delete routine.second;
//	rotationRoutines.clear();
//}
//
//void RotationRoutineVAContainer::deleteRoutine(const std::string& _name)
//{
//	auto rotationRoutineFound = this->rotationRoutines.find(_name);
//
//	if (rotationRoutineFound != this->rotationRoutines.end()) {
//		delete rotationRoutineFound->second;
//		this->rotationRoutines.erase(rotationRoutineFound);
//	}
//	else
//		printf("RotationRoutineVAContainer::deleteRoutine: Routine with given name does not exist\n");
//}