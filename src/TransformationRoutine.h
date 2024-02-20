#pragma once
#include "transformationInfo.h"
#include "MovementContainer.h"

struct TransformationRoutine {
public:
	MovementRoutineEngine*  movementRoutineEngine{};
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
	void addMovement(movementInfo* movement) {
		this->routine_movements.emplace_back(movement);
		++this->count;
	}

	// Remove movement from routine
	void removeMovement(movementInfo* movement) {
		auto it = std::find(this->routine_movements.begin(), this->routine_movements.end(), movement);
		if (it != this->routine_movements.end()) { this->routine_movements.erase(it); --this->count; }
	}

	// Clear routine
	void clear() {
		this->routine_movements.clear();
		this->current = 0;
		this->count = 0;
		this->is_active = false;
		this->is_paused = false;
		this->is_looping = false;
	}

	// Reset routine
	void reset() {
		for (auto& movement : this->routine_movements)
			movement->reset();

		this->current = 0;
		this->is_active = false;
		this->is_paused = false;
	}

	// Start routine
	void start() {
		if (this->routine_movements.size() != 0) {
			this->current = 0;
			this->is_active = true;
			this->is_paused = false;
		}
		else
			printf("MovementRoutine::start: Routine is empty\n");
	}

	// Get current movement pointer
	movementInfo* getCurrentMovement() {
		if (this->current < this->count)
			return this->routine_movements[this->current];
		else {
			printf("MovementRoutine::getCurrentMovement: Current movement index out of range\n");
			return nullptr;
		}
	}

	// Returns true if routine is active, false if it should be deleted from m_Routine_Movements_Shape_Active
	bool update(sf::Shape* _shape) {
		if (this->is_active) {
			if (!this->is_paused) {
				movementInfo* movement = this->routine_movements[this->current];

				if (movement->isFinished()) {
					printf("Movement finished\n");
					movement->reset();

					if (this->current < this->count - 1) {
						printf("Changed movement to next\n");
						++this->current;
						this->movementRoutineEngine->addMovement(_shape, this->routine_movements[this->current]);
					}
					else {
						printf("Resetting routine\n");
						this->reset();
						if (this->is_looping) {
							printf("Looping routine\n");
							this->start();
							this->movementRoutineEngine->addMovement(_shape, this->routine_movements[this->current]);
						}
						else {
							printf("Stopping movement\n");
							this->movementRoutineEngine->stopMovement(_shape);
							return false;
						}
					}
				}


				// W tym miejscu wywo³ujemy update dla konkretnego movementInfo
				// Wiêc trzeba dodaæ funkcjê sk³adow¹ update do klasy transformationInfo

				// Dodatkowo trzeba zaimplementowaæ jakiœ pojemnik na TransformationRoutine który bêdzie pozwala³ na sprawdzenie np czy 
				// dany movementInfo o okreœlonej nazwie istnieje czy nie
			}
		}
		return true;
	}
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
	void addScaling(scalingInfo* scaling) {
		this->routine_scalings.emplace_back(scaling);
		++this->count;
	}

	// Remove scaling from routine
	void removeScaling(scalingInfo* scaling) {
		auto it = std::find(this->routine_scalings.begin(), this->routine_scalings.end(), scaling);
		if (it != this->routine_scalings.end()) { this->routine_scalings.erase(it); --this->count; }
	}

	// Clear routine
	void clear() {
		this->routine_scalings.clear();
		this->current = 0;
		this->count = 0;
		this->is_active = false;
		this->is_paused = false;
		this->is_looping = false;
	}

	// Reset routine
	void reset() {
		for (auto& scaling : this->routine_scalings)
			scaling->reset();

		this->current = 0;
		this->is_active = false;
		this->is_paused = false;
	}

	// Start routine
	void start() {
		if (this->routine_scalings.size() != 0) {
			this->current = 0;
			this->is_active = true;
			this->is_paused = false;
		}
		else
			printf("ScalingRoutine::start: Routine is empty\n");
	}

	// Get current scaling pointer
	scalingInfo* getCurrentScaling() {
		if (this->current < this->count)
			return this->routine_scalings[this->current];
		else {
			printf("ScalingRoutine::getCurrentScaling: Current scaling index out of range\n");
			return nullptr;
		}
	}

	// Returns true if routine is active, false if it should be deleted from m_Routine_Scalings_Shape_Active
	bool update(sf::Shape* _shape) {
		if (this->is_active) {
			if (!this->is_paused) {
				scalingInfo* scaling = this->routine_scalings[this->current];

				if (scaling->isFinished()) {
					printf("Scaling finished\n");
					scaling->reset();

					if (this->current < this->count - 1) {
						printf("Changed scaling to next\n");
						++this->current;
						this->movementRoutineEngine->addScaling(_shape, this->routine_scalings[this->current]);
					}
					else {
						printf("Resetting routine\n");
						this->reset();
						if (this->is_looping) {
							printf("Looping routine\n");
							this->start();
							this->movementRoutineEngine->addScaling(_shape, this->routine_scalings[this->current]);
						}
						else {
							printf("Stopping scaling\n");
							this->movementRoutineEngine->stopScaling(_shape);
							return false;
						}
					}
				}
			}
		}
		return true;
	}
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
	void addRotation(rotationInfo* rotation) {
		this->routine_rotations.emplace_back(rotation);
		++this->count;
	}

	// Remove rotation from routine
	void removeRotation(rotationInfo* rotation) {
		auto it = std::find(this->routine_rotations.begin(), this->routine_rotations.end(), rotation);
		if (it != this->routine_rotations.end()) { this->routine_rotations.erase(it); --this->count; }
	}

	// Clear routine
	void clear() {
		this->routine_rotations.clear();
		this->current = 0;
		this->count = 0;
		this->is_active = false;
		this->is_paused = false;
		this->is_looping = false;
	}

	// Reset routine
	void reset() {
		for (auto& rotation : this->routine_rotations)
			rotation->reset();

		this->current = 0;
		this->is_active = false;
		this->is_paused = false;
	}

	// Start routine
	void start() {
		if (this->routine_rotations.size() != 0) {
			this->current = 0;
			this->is_active = true;
			this->is_paused = false;
		}
		else
			printf("RotationRoutine::start: Routine is empty\n");
	}

	// Get current rotation pointer
	rotationInfo* getCurrentRotation() {
		if (this->current < this->count)
			return this->routine_rotations[this->current];
		else {
			printf("RotationRoutine::getCurrentRotation: Current rotation index out of range\n");
			return nullptr;
		}
	}

	// Returns true if routine is active, false if it should be deleted from m_Routine_Rotations_Shape_Active
	bool update(sf::Shape* _shape) {
		if (this->is_active) {
			if (!this->is_paused) {
				rotationInfo* rotation = this->routine_rotations[this->current];

				if (rotation->isFinished()) {
					printf("Rotation finished\n");
					rotation->reset();

					if (this->current < this->count - 1) {
						printf("Changed rotation to next\n");
						++this->current;
						this->movementRoutineEngine->addRotation(_shape, this->routine_rotations[this->current]);
					}
					else {
						printf("Resetting routine\n");
						this->reset();
						if (this->is_looping) {
							printf("Looping routine\n");
							this->start();
							this->movementRoutineEngine->addRotation(_shape, this->routine_rotations[this->current]);
						}
						else {
							printf("Stopping rotation\n");
							this->movementRoutineEngine->stopRotation(_shape);
							return false;
						}
					}
				}
			}
		}
		return true;
	}
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
};