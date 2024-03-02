#pragma once
#include "TransformationRoutine.h"

class RoutineContainer {
protected:
	MovementRoutineEngine* movementRoutineEngine{};

	RoutineContainer() {};
	RoutineContainer(MovementRoutineEngine* _movementRoutineEnginePtr) : movementRoutineEngine(_movementRoutineEnginePtr) {}
	~RoutineContainer() { movementRoutineEngine = nullptr; }
};

class MovementRoutineContainer : protected RoutineContainer {
private:
	std::map<std::string, MovementRoutine*> movementRoutines{};

public:
	MovementRoutineContainer() {}
	MovementRoutineContainer(MovementRoutineEngine* _movementRoutineEnginePtr) : RoutineContainer{ _movementRoutineEnginePtr } {}
	~MovementRoutineContainer() { 
		for (auto routine : movementRoutines) delete routine.second;
		movementRoutines.clear(); 
	}

	// Check if routine with given name already exists
	MovementRoutine* exists(const std::string& _name);

	// Get routine value by name
	MovementRoutine getRoutine(const std::string& _name);

	// Get routine pointer by name
	MovementRoutine* getRoutinePtr(const std::string& _name);

	// Create routine with given name. If routine with given name already exists, return pointer to it. Else, create new routine.
	MovementRoutine* createRoutine(const std::string& _name);
	MovementRoutine* createRoutine(const std::string& _name, MovementRoutine* _movement_routine);

	// Clear all routines
	void clear();

	// Remove routine with given name
	void deleteRoutine(const std::string& _name);

	// Get routine count
	size_t routineCount() { return movementRoutines.size(); }

	// Get size
	long long int size() {
		long long int size = 0;
		for (auto& routine : movementRoutines) size += routine.second->size();
		return size + sizeof(movementRoutines);
	}
};

class ScalingRoutineContainer : protected RoutineContainer {
private:
	std::map<std::string, ScalingRoutine*> scalingRoutines{};

public:
	ScalingRoutineContainer() {}
	ScalingRoutineContainer(MovementRoutineEngine* _movementRoutineEnginePtr) : RoutineContainer{ _movementRoutineEnginePtr } {}
	~ScalingRoutineContainer() {
		for (auto& routine : scalingRoutines) delete routine.second;
		scalingRoutines.clear();
	}

	// Check if routine with given name already exists
	ScalingRoutine* exists(const std::string& _name);

	// Get routine value by name
	ScalingRoutine getRoutine(const std::string& _name);

	// Get routine pointer by name
	ScalingRoutine* getRoutinePtr(const std::string& _name);

	// Create routine with given name. If routine with given name already exists, return pointer to it. Else, create new routine.
	ScalingRoutine* createRoutine(const std::string& _name);
	ScalingRoutine* createRoutine(const std::string& _name, ScalingRoutine* _scaling_routine);

	// Clear all routines
	void clear();

	// Remove routine with given name
	void deleteRoutine(const std::string& _name);

	// Get routine count
	size_t routineCount() { return scalingRoutines.size(); }

	// Get size
	long long int size() {
		long long int size = 0;
		for (auto& routine : scalingRoutines) size += routine.second->size();
		return size + sizeof(scalingRoutines);
	}
};

class RotationRoutineContainer : protected RoutineContainer {
private:
	std::map<std::string, RotationRoutine*> rotationRoutines{};

public:
	RotationRoutineContainer() {}
	RotationRoutineContainer(MovementRoutineEngine* _movementRoutineEnginePtr) : RoutineContainer{ _movementRoutineEnginePtr } {}
	~RotationRoutineContainer() {
		for (auto& routine : rotationRoutines) delete routine.second;
		rotationRoutines.clear();
	}

	// Check if routine with given name already exists
	RotationRoutine* exists(const std::string& _name);

	// Get routine value by name
	RotationRoutine getRoutine(const std::string& _name);

	// Get routine pointer by name
	RotationRoutine* getRoutinePtr(const std::string& _name);

	// Create routine with given name. If routine with given name already exists, return pointer to it. Else, create new routine.
	RotationRoutine* createRoutine(const std::string& _name);
	RotationRoutine* createRoutine(const std::string& _name, RotationRoutine* _rotation_routine);

	// Clear all routines
	void clear();

	// Remove routine with given name
	void deleteRoutine(const std::string& _name);

	// Get routine count
	size_t routineCount() { return rotationRoutines.size(); }

	// Get size
	long long int size() {
		long long int size = 0;
		for (auto& routine : rotationRoutines) size += routine.second->size();
		return size + sizeof(rotationRoutines);
	}
};