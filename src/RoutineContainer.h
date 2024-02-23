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

class MovementRoutineVAContainer : protected RoutineContainer {
private:
	std::map<std::string, MovementRoutineVA*> movementRoutines{};

public:
	MovementRoutineVAContainer() {}
	MovementRoutineVAContainer(MovementRoutineEngine* _movementRoutineEnginePtr) : RoutineContainer{ _movementRoutineEnginePtr } {}
	~MovementRoutineVAContainer() {
		for (auto& routine : movementRoutines) delete routine.second;
		movementRoutines.clear();
	}

	// Check if routine with given name already exists
	MovementRoutineVA* exists(const std::string& _name);

	// Get routine value by name
	MovementRoutineVA getRoutine(const std::string& _name);

	// Get routine pointer by name
	MovementRoutineVA* getRoutinePtr(const std::string& _name);

	// Create routine with given name. If routine with given name already exists, return pointer to it. Else, create new routine.
	MovementRoutineVA* createRoutine(const std::string& _name);
	MovementRoutineVA* createRoutine(const std::string& _name, MovementRoutineVA* _movement_routine);

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
	ScalingRoutineContainer(MovementRoutineEngine* _movementRoutineEnginePtr) { this->movementRoutineEngine = _movementRoutineEnginePtr; }
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

class ScalingRoutineVAContainer : protected RoutineContainer {
private:
	std::map<std::string, ScalingRoutineVA*> scalingRoutines{};
	MovementRoutineEngine* movementRoutineEngine{};

public:
	ScalingRoutineVAContainer() {}
	ScalingRoutineVAContainer(MovementRoutineEngine* _movementRoutineEnginePtr) : movementRoutineEngine(_movementRoutineEnginePtr) {}
	~ScalingRoutineVAContainer() {
		for (auto& routine : scalingRoutines) delete routine.second;
		scalingRoutines.clear();
	}

	// Check if routine with given name already exists
	ScalingRoutineVA* exists(const std::string& _name);

	// Get routine value by name
	ScalingRoutineVA getRoutine(const std::string& _name);

	// Get routine pointer by name
	ScalingRoutineVA* getRoutinePtr(const std::string& _name);

	// Create routine with given name. If routine with given name already exists, return pointer to it. Else, create new routine.
	ScalingRoutineVA* createRoutine(const std::string& _name);
	ScalingRoutineVA* createRoutine(const std::string& _name, ScalingRoutineVA* _scaling_routine);

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
	MovementRoutineEngine* movementRoutineEngine{};

public:
	RotationRoutineContainer() {}
	RotationRoutineContainer(MovementRoutineEngine* _movementRoutineEnginePtr) : movementRoutineEngine(_movementRoutineEnginePtr) {}
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

class RotationRoutineVAContainer : protected RoutineContainer {
private:
	std::map<std::string, RotationRoutineVA*> rotationRoutines{};
	MovementRoutineEngine* movementRoutineEngine{};

public:
	RotationRoutineVAContainer() {}
	RotationRoutineVAContainer(MovementRoutineEngine* _movementRoutineEnginePtr) : movementRoutineEngine(_movementRoutineEnginePtr) {}
	~RotationRoutineVAContainer() { rotationRoutines.clear(); }

	// Check if routine with given name already exists
	RotationRoutineVA* exists(const std::string& _name);

	// Get routine value by name
	RotationRoutineVA getRoutine(const std::string& _name);

	// Get routine pointer by name
	RotationRoutineVA* getRoutinePtr(const std::string& _name);

	// Create routine with given name. If routine with given name already exists, return pointer to it. Else, create new routine.
	RotationRoutineVA* createRoutine(const std::string& _name);
	RotationRoutineVA* createRoutine(const std::string& _name, RotationRoutineVA* _rotation_routine);

	// Clear all routines
	void clear();

	// Remove routine with given name
	void deleteRoutine(const std::string& _name);
};