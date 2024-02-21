#pragma once
#include "transformationInfo.h"

class MovementContainerBase;
class MovementRoutineEngine;

class TransformationRoutine {
public:
	MovementRoutineEngine*  movementRoutineEngine;
	std::string			routine_name{};
	size_t				current{};
	size_t				count{};
	bool				is_active{};
	bool				is_looping{};
	bool				is_paused{};
	bool				adjust_start_to_current_transform{};
	bool				adjust_all_to_current_transform{};

public:
	TransformationRoutine() {}
	TransformationRoutine(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr) { this->routine_name = name; this->movementRoutineEngine = _movementRoutineEnginePtr; }
	TransformationRoutine(const TransformationRoutine& obj) :
		movementRoutineEngine(obj.movementRoutineEngine), routine_name(obj.routine_name), current(obj.current), count(obj.count), is_active(obj.is_active), is_looping(obj.is_looping), is_paused(obj.is_paused), adjust_start_to_current_transform(obj.adjust_start_to_current_transform), adjust_all_to_current_transform(obj.adjust_all_to_current_transform) {}

	void setLooping(bool _looping) { this->is_looping = _looping; }

	void adjustStartToCurrentTransform(bool _adjust) { this->adjust_start_to_current_transform = _adjust; }
	void adjustAllToCurrentTransform(bool _adjust) { this->adjust_all_to_current_transform = _adjust; }

	// Pause routine
	void pause() { this->is_paused = true; }

	// Resume routine
	void resume() { this->is_paused = false; }
};

class MovementRoutine : public TransformationRoutine {
private:
	std::vector <movementInfo*> routine_movements;

	void adjustStartToCurrent(sf::Vector2f current_position);
	void adjustAllToCurrent(sf::Vector2f current_position);

public:
	MovementRoutine() {}
	MovementRoutine(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr) : TransformationRoutine{ name, _movementRoutineEnginePtr } {};
	MovementRoutine(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr, movementInfo* movement) : TransformationRoutine{ name, _movementRoutineEnginePtr } { this->routine_movements.emplace_back(movement); }
	MovementRoutine(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr, std::vector<movementInfo*> movements) : TransformationRoutine{ name, _movementRoutineEnginePtr } { this->routine_movements = movements; }
	MovementRoutine(const MovementRoutine& obj) : TransformationRoutine{ obj } {
		for (auto movement : obj.routine_movements) 
			this->routine_movements.push_back(new movementInfo(*movement));
	}
	~MovementRoutine() { for (auto movement : routine_movements) delete movement; routine_movements.clear(); }

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

	const bool goToNextMovement(sf::Shape* shape);
	const bool goToNextMovement(sf::Sprite* sprite);

	// get size
	long long int size() {
		long long int size = 0;
		for (auto movement : routine_movements) size += sizeof(*movement);
		return size + sizeof(routine_movements);
	}
};

class MovementRoutineVA : public TransformationRoutine {
private:
	std::vector <movementInfoVA*> routine_movements;

	void adjustVertexarrayToStartingPosition(sf::VertexArray* vertexarray);
	void adjustStartToCurrent(sf::VertexArray* vertexArray);
	void adjustAllToCurrent(sf::VertexArray* vertexArray);

public:
	MovementRoutineVA() {}
	MovementRoutineVA(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr) : TransformationRoutine{ name, _movementRoutineEnginePtr } {};
	MovementRoutineVA(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr, movementInfoVA* movement) : TransformationRoutine{ name, _movementRoutineEnginePtr } { this->routine_movements.emplace_back(movement); }
	MovementRoutineVA(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr, std::vector<movementInfoVA*> movements) : TransformationRoutine{ name, _movementRoutineEnginePtr } { this->routine_movements = movements; }
	MovementRoutineVA(const MovementRoutineVA& obj) : TransformationRoutine{ obj } {
		for (auto movement : obj.routine_movements)
			this->routine_movements.push_back(new movementInfoVA(*movement));
	}

	~MovementRoutineVA() { for (auto& movement : routine_movements) delete movement; routine_movements.clear(); }

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

	const bool goToNextMovement(sf::VertexArray* vertexArray);
};

class ScalingRoutine : public TransformationRoutine {
private:
	std::vector <scalingInfo*> routine_scalings;

public:
	ScalingRoutine() {}
	ScalingRoutine(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr) : TransformationRoutine{ name, _movementRoutineEnginePtr } {};
	ScalingRoutine(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr, scalingInfo* scaling) : TransformationRoutine{ name, _movementRoutineEnginePtr } { this->routine_scalings.emplace_back(scaling); }
	ScalingRoutine(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr, std::vector<scalingInfo*> scalings) : TransformationRoutine{ name, _movementRoutineEnginePtr } { this->routine_scalings = scalings; }
	ScalingRoutine(const ScalingRoutine& obj) : TransformationRoutine{ obj }, routine_scalings(obj.routine_scalings) {}
	~ScalingRoutine() { routine_scalings.clear(); }

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

class ScalingRoutineVA : public TransformationRoutine {
private:
	std::vector <scalingInfoVA*> routine_scalings;

public:
	ScalingRoutineVA() {}
	ScalingRoutineVA(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr) : TransformationRoutine{ name, _movementRoutineEnginePtr } {};
	ScalingRoutineVA(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr, scalingInfoVA* scaling) : TransformationRoutine{ name, _movementRoutineEnginePtr } { this->routine_scalings.emplace_back(scaling); }
	ScalingRoutineVA(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr, std::vector<scalingInfoVA*> scalings) : TransformationRoutine{ name, _movementRoutineEnginePtr } { this->routine_scalings = scalings; }
	ScalingRoutineVA(const ScalingRoutineVA& obj) : TransformationRoutine{ obj }, routine_scalings(obj.routine_scalings) {}
	~ScalingRoutineVA() { routine_scalings.clear(); }

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

class RotationRoutine : public TransformationRoutine {
private:
	std::vector <rotationInfo*> routine_rotations;

public:
	RotationRoutine() {}
	RotationRoutine(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr) : TransformationRoutine{ name, _movementRoutineEnginePtr } {};
	RotationRoutine(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr, rotationInfo* rotation) : TransformationRoutine{ name, _movementRoutineEnginePtr } { this->routine_rotations.emplace_back(rotation); }
	RotationRoutine(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr, std::vector<rotationInfo*> rotations) : TransformationRoutine{ name, _movementRoutineEnginePtr } { this->routine_rotations = rotations; }
	RotationRoutine(const RotationRoutine& obj) : TransformationRoutine{ obj }, routine_rotations(obj.routine_rotations) {}
	~RotationRoutine() { routine_rotations.clear(); }

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

class RotationRoutineVA : public TransformationRoutine {
private:
	std::vector <rotationInfoVA*> routine_rotations;

public:
	RotationRoutineVA() {}
	RotationRoutineVA(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr) : TransformationRoutine{ name, _movementRoutineEnginePtr } {};
	RotationRoutineVA(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr, rotationInfoVA* rotation) : TransformationRoutine{ name, _movementRoutineEnginePtr } { this->routine_rotations.emplace_back(rotation); }
	RotationRoutineVA(std::string name, MovementRoutineEngine* _movementRoutineEnginePtr, std::vector<rotationInfoVA*> rotations) : TransformationRoutine{ name, _movementRoutineEnginePtr } { this->routine_rotations = rotations; }
	RotationRoutineVA(const RotationRoutineVA& obj) : TransformationRoutine{ obj }, routine_rotations(obj.routine_rotations) {}
	~RotationRoutineVA() { routine_rotations.clear(); }

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