#pragma once
#include "transformationInfo.h"

class MovementContainerBase;
class MovementRoutineEngine;

struct TransformationRoutine {
public:
	MovementRoutineEngine*  movementRoutineEngine;
	std::string			routine_name{};
	size_t				current{};
	size_t				count{};
	bool				is_active{};
	bool				is_looping{};
	bool				is_paused{};

	TransformationRoutine() {}
	TransformationRoutine(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr) { this->routine_name = name; this->movementRoutineEngine = _movementRoutineEnginePtr; }
	TransformationRoutine(const TransformationRoutine& obj) :
		movementRoutineEngine(obj.movementRoutineEngine), routine_name(obj.routine_name), current(obj.current), count(obj.count), is_active(obj.is_active), is_looping(obj.is_looping), is_paused(obj.is_paused) {}

	void setLooping(bool _looping) { this->is_looping = _looping; }

	// Pause routine
	void pause() { this->is_paused = true; }

	// Resume routine
	void resume() { this->is_paused = false; }
};

struct MovementRoutine : public TransformationRoutine {
	std::vector <movementInfo*> routine_movements;

	MovementRoutine() {}
	MovementRoutine(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr) : TransformationRoutine{ name, _movementRoutineEnginePtr } {};
	MovementRoutine(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr, movementInfo* movement) : TransformationRoutine{ name, _movementRoutineEnginePtr } { this->routine_movements.emplace_back(movement); }
	MovementRoutine(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr, std::vector<movementInfo*> movements) : TransformationRoutine{ name, _movementRoutineEnginePtr } { this->routine_movements = movements; }
	MovementRoutine(const MovementRoutine& obj) : TransformationRoutine{ obj }, routine_movements(obj.routine_movements) {}
	~MovementRoutine() { routine_movements.clear(); }

public:
	// Add movement to routine
	void addMovement(movementInfo* movement);

	// Remove movement from routine
	void removeMovement(movementInfo* movement);

	// Clear routine
	void clear();

	// Reset routine
	void reset();

	// Start routine
	const bool start(sf::Shape* shape);
	const bool start(sf::Sprite* sprite);

	// Stop routine
	void stop(sf::Shape* shape);
	void stop(sf::Sprite* sprite);

	// Get current movement pointer
	movementInfo* getCurrentMovement();

	const bool goToNextMovement();
};

struct MovementRoutineVA : public TransformationRoutine {
	std::vector <movementInfoVA*> routine_movements;

	MovementRoutineVA() {}
	MovementRoutineVA(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr) : TransformationRoutine{ name, _movementRoutineEnginePtr } {};
	MovementRoutineVA(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr, movementInfoVA* movement) : TransformationRoutine{ name, _movementRoutineEnginePtr } { this->routine_movements.emplace_back(movement); }
	MovementRoutineVA(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr, std::vector<movementInfoVA*> movements) : TransformationRoutine{ name, _movementRoutineEnginePtr } { this->routine_movements = movements; }
	MovementRoutineVA(const MovementRoutineVA& obj) : TransformationRoutine{ obj }, routine_movements(obj.routine_movements) {}
	~MovementRoutineVA() { routine_movements.clear(); }

public:
	// Add movement to routine
	void addMovement(movementInfoVA* movement);

	// Remove movement from routine
	void removeMovement(movementInfoVA* movement);

	// Clear routine
	void clear();

	// Reset routine
	void reset();

	// Start routine
	const bool start(sf::VertexArray* vertexarray);

	// Stop routine
	void stop(sf::VertexArray* vertexarray);

	// Get current movement pointer
	movementInfoVA* getCurrentMovement();

	const bool goToNextMovement();
};

struct ScalingRoutine : public TransformationRoutine {
	std::vector <scalingInfo*> routine_scalings;

	ScalingRoutine() {}
	ScalingRoutine(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr) : TransformationRoutine{ name, _movementRoutineEnginePtr } {};
	ScalingRoutine(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr, scalingInfo* scaling) : TransformationRoutine{ name, _movementRoutineEnginePtr } { this->routine_scalings.emplace_back(scaling); }
	ScalingRoutine(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr, std::vector<scalingInfo*> scalings) : TransformationRoutine{ name, _movementRoutineEnginePtr } { this->routine_scalings = scalings; }
	ScalingRoutine(const ScalingRoutine& obj) : TransformationRoutine{ obj }, routine_scalings(obj.routine_scalings) {}
	~ScalingRoutine() { routine_scalings.clear(); }

public:
	// Add scaling to routine
	void addScaling(scalingInfo* scaling);

	// Remove scaling from routine
	void removeScaling(scalingInfo* scaling);

	// Clear routine
	void clear();

	// Reset routine
	void reset();

	// Start routine
	const bool start(sf::Shape* shape);
	const bool start(sf::Sprite* sprite);

	// Stop routine
	void stop(sf::Shape* shape);
	void stop(sf::Sprite* sprite);

	// Get current scaling pointer
	scalingInfo* getCurrentScaling();

	const bool goToNextScaling();
};

struct ScalingRoutineVA : public TransformationRoutine {
	std::vector <scalingInfoVA*> routine_scalings;

	ScalingRoutineVA() {}
	ScalingRoutineVA(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr) : TransformationRoutine{ name, _movementRoutineEnginePtr } {};
	ScalingRoutineVA(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr, scalingInfoVA* scaling) : TransformationRoutine{ name, _movementRoutineEnginePtr } { this->routine_scalings.emplace_back(scaling); }
	ScalingRoutineVA(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr, std::vector<scalingInfoVA*> scalings) : TransformationRoutine{ name, _movementRoutineEnginePtr } { this->routine_scalings = scalings; }
	ScalingRoutineVA(const ScalingRoutineVA& obj) : TransformationRoutine{ obj }, routine_scalings(obj.routine_scalings) {}
	~ScalingRoutineVA() { routine_scalings.clear(); }

public:
	// Add scaling to routine
	void addScaling(scalingInfoVA* scaling);

	// Remove scaling from routine
	void removeScaling(scalingInfoVA* scaling);

	// Clear routine
	void clear();

	// Reset routine
	void reset();

	// Start routine
	const bool start(sf::VertexArray* vertexarray);

	// Stop routine
	void stop(sf::VertexArray* vertexarray);

	// Get current scaling pointer
	scalingInfoVA* getCurrentScaling();

	const bool goToNextScaling();
};

struct RotationRoutine : public TransformationRoutine {
	std::vector <rotationInfo*> routine_rotations;

	RotationRoutine() {}
	RotationRoutine(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr) : TransformationRoutine{ name, _movementRoutineEnginePtr } {};
	RotationRoutine(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr, rotationInfo* rotation) : TransformationRoutine{ name, _movementRoutineEnginePtr } { this->routine_rotations.emplace_back(rotation); }
	RotationRoutine(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr, std::vector<rotationInfo*> rotations) : TransformationRoutine{ name, _movementRoutineEnginePtr } { this->routine_rotations = rotations; }
	RotationRoutine(const RotationRoutine& obj) : TransformationRoutine{ obj }, routine_rotations(obj.routine_rotations) {}
	~RotationRoutine() { routine_rotations.clear(); }

public:
	// Add rotation to routine
	void addRotation(rotationInfo* rotation);

	// Remove rotation from routine
	void removeRotation(rotationInfo* rotation);

	// Clear routine
	void clear();

	// Reset routine
	void reset();

	// Start routine
	const bool start(sf::Shape* shape);
	const bool start(sf::Sprite* sprite);

	// Stop routine
	void stop(sf::Shape* shape);
	void stop(sf::Sprite* sprite);

	// Get current rotation pointer
	rotationInfo* getCurrentRotation();

	const bool goToNextRotation();
};

struct RotationRoutineVA : public TransformationRoutine {
	std::vector <rotationInfoVA*> routine_rotations;

	RotationRoutineVA() {}
	RotationRoutineVA(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr) : TransformationRoutine{ name, _movementRoutineEnginePtr } {};
	RotationRoutineVA(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr, rotationInfoVA* rotation) : TransformationRoutine{ name, _movementRoutineEnginePtr } { this->routine_rotations.emplace_back(rotation); }
	RotationRoutineVA(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr, std::vector<rotationInfoVA*> rotations) : TransformationRoutine{ name, _movementRoutineEnginePtr } { this->routine_rotations = rotations; }
	RotationRoutineVA(const RotationRoutineVA& obj) : TransformationRoutine{ obj }, routine_rotations(obj.routine_rotations) {}
	~RotationRoutineVA() { routine_rotations.clear(); }

public:
	// Add rotation to routine
	void addRotation(rotationInfoVA* rotation);

	// Remove rotation from routine
	void removeRotation(rotationInfoVA* rotation);

	// Clear routine
	void clear();

	// Reset routine
	void reset();

	// Start routine
	const bool start(sf::VertexArray* vertexarray);

	// Stop routine
	void stop(sf::VertexArray* vertexarray);

	// Get current rotation pointer
	rotationInfoVA* getCurrentRotation();

	const bool goToNextRotation();
};

struct MovementRoutineContainer {
private:
	std::map<std::string, MovementRoutine*> movementRoutines{};
	MovementRoutineEngine* movementRoutineEngine{};

public:
	MovementRoutineContainer() {}
	MovementRoutineContainer(MovementRoutineEngine* _movementRoutineEnginePtr) { this->movementRoutineEngine = _movementRoutineEnginePtr; }
	~MovementRoutineContainer() { movementRoutines.clear(); movementRoutineEngine = nullptr; }

	// Check if routine with given name already exists
	MovementRoutine* exists(const std::string& _name) {
		auto movementRoutineFound = movementRoutines.find(_name);

		if (movementRoutineFound != movementRoutines.end())
			return movementRoutineFound->second;
		else
			return nullptr;
	}

	// Get routine value by name
	MovementRoutine getRoutine(const std::string& _name) {
		if (exists(_name) != nullptr)
			return *movementRoutines[_name];
		else {
			printf("MovementRoutineContainer::getRoutine: Routine with given name does not exist\n");
			return MovementRoutine();
		}
	}

	// Get routine pointer by name
	MovementRoutine* getRoutinePtr(const std::string& _name) {
		auto* movementRoutineExists = exists(_name);

		if (movementRoutineExists == nullptr)
			printf("MovementRoutineContainer::getRoutine: Routine with given name does not exist\n");

		return movementRoutineExists;
	}

	// Create routine with given name. If routine with given name already exists, return pointer to it. Else, create new routine.
	MovementRoutine* createRoutine(const std::string& _name) {
		auto* movementRoutineExists = exists(_name);

		if (movementRoutineExists != nullptr) {
			printf("MovementRoutineContainer::createRoutine: Routine with given name already exists 1\n");
			return movementRoutineExists;
		}
		else {
			MovementRoutine* newMovementRoutine = new MovementRoutine(_name, movementRoutineEngine);
			movementRoutines.insert(std::make_pair(_name, newMovementRoutine));
			return newMovementRoutine;
		}
	}

	MovementRoutine* createRoutine(const std::string& _name, MovementRoutine* _movement_routine) {
		auto* movementRoutineExists = exists(_name);

		if (movementRoutineExists != nullptr) {
			printf("MovementRoutineContainer::createRoutine: Routine with given name already exists 2\n");
			return movementRoutineExists;
		}
		else {
			movementRoutines.insert(std::make_pair(_name, _movement_routine));
			return _movement_routine;
		}
	}

	// Remove routine with given name
	void removeRoutine(const std::string& _name) {
		auto* movementRoutineExists = exists(_name);

		if (movementRoutineExists != nullptr)
			movementRoutines.erase(_name);
		else
			printf("MovementRoutineContainer::removeRoutine: Routine with given name does not exist\n");
	}
};

struct MovementRoutineVAContainer {
private:
	std::map<std::string, MovementRoutineVA*> movementRoutines{};
	MovementRoutineEngine* movementRoutineEngine{};
public:
	MovementRoutineVAContainer() {}
	MovementRoutineVAContainer(MovementRoutineEngine* _movementRoutineEnginePtr) { this->movementRoutineEngine = _movementRoutineEnginePtr; }
	~MovementRoutineVAContainer() { movementRoutines.clear(); movementRoutineEngine = nullptr; }

	// Check if routine with given name already exists
	MovementRoutineVA* exists(const std::string& _name) {
		auto movementRoutineFound = movementRoutines.find(_name);

		if (movementRoutineFound != movementRoutines.end())
			return movementRoutineFound->second;
		else
			return nullptr;
	}

	// Get routine value by name
	MovementRoutineVA getRoutine(const std::string& _name) {
		if (exists(_name) != nullptr)
			return *movementRoutines[_name];
		else {
			printf("MovementRoutineVAContainer::getRoutine: Routine with given name does not exist\n");
			return MovementRoutineVA();
		}
	}

	// Get routine pointer by name
	MovementRoutineVA* getRoutinePtr(const std::string& _name) {
		auto* movementRoutineExists = exists(_name);

		if (movementRoutineExists == nullptr)
			printf("MovementRoutineVAContainer::getRoutine: Routine with given name does not exist\n");

		return movementRoutineExists;
	}

	// Create routine with given name. If routine with given name already exists, return pointer to it. Else, create new routine.
	MovementRoutineVA* createRoutine(const std::string& _name) {
		auto* movementRoutineExists = exists(_name);

		if (movementRoutineExists != nullptr) {
			printf("MovementRoutineVAContainer::createRoutine: Routine with given name already exists 1\n");
			return movementRoutineExists;
		}
		else {
			MovementRoutineVA* newMovementRoutine = new MovementRoutineVA(_name, movementRoutineEngine);
			movementRoutines.insert(std::make_pair(_name, newMovementRoutine));
			return newMovementRoutine;
		}
	}

	MovementRoutineVA* createRoutine(const std::string& _name, MovementRoutineVA* _movement_routine) {
		auto* movementRoutineExists = exists(_name);

		if (movementRoutineExists != nullptr) {
			printf("MovementRoutineVAContainer::createRoutine: Routine with given name already exists 2\n");
			return movementRoutineExists;
		}
		else {
			movementRoutines.insert(std::make_pair(_name, _movement_routine));
			return _movement_routine;
		}
	}
};

struct ScalingRoutineContainer {
private:
	std::map<std::string, ScalingRoutine*> scalingRoutines{};
	MovementRoutineEngine* movementRoutineEngine{};

public:
	ScalingRoutineContainer() {}
	ScalingRoutineContainer(MovementRoutineEngine* _movementRoutineEnginePtr) { this->movementRoutineEngine = _movementRoutineEnginePtr; }
	~ScalingRoutineContainer() { scalingRoutines.clear(); movementRoutineEngine = nullptr; }

	// Check if routine with given name already exists
	ScalingRoutine* exists(const std::string& _name) {
		auto scalingRoutineFound = scalingRoutines.find(_name);

		if (scalingRoutineFound != scalingRoutines.end())
			return scalingRoutineFound->second;
		else
			return nullptr;
	}

	// Get routine value by name
	ScalingRoutine getRoutine(const std::string& _name) {
		if (exists(_name) != nullptr)
			return *scalingRoutines[_name];
		else {
			printf("ScalingRoutineContainer::getRoutine: Routine with given name does not exist\n");
			return ScalingRoutine();
		}
	}

	// Get routine pointer by name
	ScalingRoutine* getRoutinePtr(const std::string& _name) {
		auto* scalingRoutineExists = exists(_name);

		if (scalingRoutineExists == nullptr)
			printf("ScalingRoutineContainer::getRoutine: Routine with given name does not exist\n");

		return scalingRoutineExists;
	}

	// Create routine with given name. If routine with given name already exists, return pointer to it. Else, create new routine.
	ScalingRoutine* createRoutine(const std::string& _name) {
		auto* scalingRoutineExists = exists(_name);

		if (scalingRoutineExists != nullptr) {
			printf("ScalingRoutineContainer::createRoutine: Routine with given name already exists 1\n");
			return scalingRoutineExists;
		}
		else {
			ScalingRoutine* newScalingRoutine = new ScalingRoutine(_name, movementRoutineEngine);
			scalingRoutines.insert(std::make_pair(_name, newScalingRoutine));
			return newScalingRoutine;
		}
	}

	ScalingRoutine* createRoutine(const std::string& _name, ScalingRoutine* _scaling_routine) {
		auto* scalingRoutineExists = exists(_name);

		if (scalingRoutineExists != nullptr) {
			printf("ScalingRoutineContainer::createRoutine: Routine with given name already exists 2\n");
			return scalingRoutineExists;
		}
		else {
			scalingRoutines.insert(std::make_pair(_name, _scaling_routine));
			return _scaling_routine;
		}
	}
};

struct ScalingRoutineVAContainer {
private:
	std::map<std::string, ScalingRoutineVA*> scalingRoutines{};
	MovementRoutineEngine* movementRoutineEngine{};

public:
	ScalingRoutineVAContainer() {}
	ScalingRoutineVAContainer(MovementRoutineEngine* _movementRoutineEnginePtr) { this->movementRoutineEngine = _movementRoutineEnginePtr; }
	~ScalingRoutineVAContainer() { scalingRoutines.clear(); movementRoutineEngine = nullptr; }

	// Check if routine with given name already exists
	ScalingRoutineVA* exists(const std::string& _name) {
		auto scalingRoutineFound = scalingRoutines.find(_name);

		if (scalingRoutineFound != scalingRoutines.end())
			return scalingRoutineFound->second;
		else
			return nullptr;
	}

	// Get routine value by name
	ScalingRoutineVA getRoutine(const std::string& _name) {
		if (exists(_name) != nullptr)
			return *scalingRoutines[_name];
		else {
			printf("ScalingRoutineVAContainer::getRoutine: Routine with given name does not exist\n");
			return ScalingRoutineVA();
		}
	}

	// Get routine pointer by name
	ScalingRoutineVA* getRoutinePtr(const std::string& _name) {
		auto* scalingRoutineExists = exists(_name);

		if (scalingRoutineExists == nullptr)
			printf("ScalingRoutineVAContainer::getRoutine: Routine with given name does not exist\n");

		return scalingRoutineExists;
	}

	// Create routine with given name. If routine with given name already exists, return pointer to it. Else, create new routine.
	ScalingRoutineVA* createRoutine(const std::string& _name) {
		auto* scalingRoutineExists = exists(_name);

		if (scalingRoutineExists != nullptr) {
			printf("ScalingRoutineVAContainer::createRoutine: Routine with given name already exists 1\n");
			return scalingRoutineExists;
		}
		else {
			ScalingRoutineVA* newScalingRoutine = new ScalingRoutineVA(_name, movementRoutineEngine);
			scalingRoutines.insert(std::make_pair(_name, newScalingRoutine));
			return newScalingRoutine;
		}
	}

	ScalingRoutineVA* createRoutine(const std::string& _name, ScalingRoutineVA* _scaling_routine) {
		auto* scalingRoutineExists = exists(_name);

		if (scalingRoutineExists != nullptr) {
			printf("ScalingRoutineVAContainer::createRoutine: Routine with given name already exists 2\n");
			return scalingRoutineExists;
		}
		else {
			scalingRoutines.insert(std::make_pair(_name, _scaling_routine));
			return _scaling_routine;
		}
	}
};

struct RotationRoutineContainer {
private:
	std::map<std::string, RotationRoutine*> rotationRoutines{};
	MovementRoutineEngine* movementRoutineEngine{};

public:
	RotationRoutineContainer() {}
	RotationRoutineContainer(MovementRoutineEngine* _movementRoutineEnginePtr) { this->movementRoutineEngine = _movementRoutineEnginePtr; }
	~RotationRoutineContainer() { rotationRoutines.clear(); movementRoutineEngine = nullptr; }

	// Check if routine with given name already exists
	RotationRoutine* exists(const std::string& _name) {
		auto rotationRoutineFound = rotationRoutines.find(_name);

		if (rotationRoutineFound != rotationRoutines.end())
			return rotationRoutineFound->second;
		else
			return nullptr;
	}

	// Get routine value by name
	RotationRoutine getRoutine(const std::string& _name) {
		if (exists(_name) != nullptr)
			return *rotationRoutines[_name];
		else {
			printf("RotationRoutineContainer::getRoutine: Routine with given name does not exist\n");
			return RotationRoutine();
		}
	}

	// Get routine pointer by name
	RotationRoutine* getRoutinePtr(const std::string& _name) {
		auto* rotationRoutineExists = exists(_name);

		if (rotationRoutineExists == nullptr)
			printf("RotationRoutineContainer::getRoutine: Routine with given name does not exist\n");

		return rotationRoutineExists;
	}

	// Create routine with given name. If routine with given name already exists, return pointer to it. Else, create new routine.
	RotationRoutine* createRoutine(const std::string& _name) {
		auto* rotationRoutineExists = exists(_name);

		if (rotationRoutineExists != nullptr) {
			printf("RotationRoutineContainer::createRoutine: Routine with given name already exists 1\n");
			return rotationRoutineExists;
		}
		else {
			RotationRoutine* newRotationRoutine = new RotationRoutine(_name, movementRoutineEngine);
			rotationRoutines.insert(std::make_pair(_name, newRotationRoutine));
			return newRotationRoutine;
		}
	}

	RotationRoutine* createRoutine(const std::string& _name, RotationRoutine* _rotation_routine) {
		auto* rotationRoutineExists = exists(_name);

		if (rotationRoutineExists != nullptr) {
			printf("RotationRoutineContainer::createRoutine: Routine with given name already exists 2\n");
			return rotationRoutineExists;
		}
		else {
			rotationRoutines.insert(std::make_pair(_name, _rotation_routine));
			return _rotation_routine;
		}
	}
};

struct RotationRoutineVAContainer {
private:
	std::map<std::string, RotationRoutineVA*> rotationRoutines{};
	MovementRoutineEngine* movementRoutineEngine{};

public:
	RotationRoutineVAContainer() {}
	RotationRoutineVAContainer(MovementRoutineEngine* _movementRoutineEnginePtr) { this->movementRoutineEngine = _movementRoutineEnginePtr; }
	~RotationRoutineVAContainer() { rotationRoutines.clear(); movementRoutineEngine = nullptr; }

	// Check if routine with given name already exists
	RotationRoutineVA* exists(const std::string& _name) {
		auto rotationRoutineFound = rotationRoutines.find(_name);

		if (rotationRoutineFound != rotationRoutines.end())
			return rotationRoutineFound->second;
		else
			return nullptr;
	}

	// Get routine value by name
	RotationRoutineVA getRoutine(const std::string& _name) {
		if (exists(_name) != nullptr)
			return *rotationRoutines[_name];
		else {
			printf("RotationRoutineVAContainer::getRoutine: Routine with given name does not exist\n");
			return RotationRoutineVA();
		}
	}

	// Get routine pointer by name
	RotationRoutineVA* getRoutinePtr(const std::string& _name) {
		auto* rotationRoutineExists = exists(_name);

		if (rotationRoutineExists == nullptr)
			printf("RotationRoutineVAContainer::getRoutine: Routine with given name does not exist\n");

		return rotationRoutineExists;
	}

	// Create routine with given name. If routine with given name already exists, return pointer to it. Else, create new routine.
	RotationRoutineVA* createRoutine(const std::string& _name) {
		auto* rotationRoutineExists = exists(_name);

		if (rotationRoutineExists != nullptr) {
			printf("RotationRoutineVAContainer::createRoutine: Routine with given name already exists 1\n");
			return rotationRoutineExists;
		}
		else {
			RotationRoutineVA* newRotationRoutine = new RotationRoutineVA(_name, movementRoutineEngine);
			rotationRoutines.insert(std::make_pair(_name, newRotationRoutine));
			return newRotationRoutine;
		}
	}

	RotationRoutineVA* createRoutine(const std::string& _name, RotationRoutineVA* _rotation_routine) {
		auto* rotationRoutineExists = exists(_name);

		if (rotationRoutineExists != nullptr) {
			printf("RotationRoutineVAContainer::createRoutine: Routine with given name already exists 2\n");
			return rotationRoutineExists;
		}
		else {
			rotationRoutines.insert(std::make_pair(_name, _rotation_routine));
			return _rotation_routine;
		}
	}
};