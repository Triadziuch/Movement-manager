#pragma once
#include "transformationInfo.h"

class MovementContainerBase;
class MovementRoutineEngine;

class TransformationRoutine {
protected:
	MovementRoutineEngine* m_movementRoutineEngine{};
	std::string			   m_routineName{};
	size_t				   m_current{};
	size_t				   m_count{};
	bool				   m_isActive{};
	bool				   m_isLooping{};
	bool				   m_isPaused{};
	bool				   m_adjustStartToCurrentTransform{};
	bool				   m_adjustAllToCurrentTransform{};

public:
	// Constructors / Destructors
	TransformationRoutine();
	TransformationRoutine(const std::string& name, MovementRoutineEngine* movementRoutineEnginePtr);
	TransformationRoutine(const TransformationRoutine& obj);
	virtual ~TransformationRoutine();

	// Mutators
	void setLooping(bool looping);
	void adjustStartToCurrentTransform(bool adjust);
	void adjustAllToCurrentTransform(bool adjust);

	void pause();
	void resume();

	// Accessors
	const std::string& getName() const;
};

class MovementRoutine : public TransformationRoutine {
private:
	std::vector <movementInfo*> m_routineMovements;

	// Private functions
	void adjustStartToCurrent(const sf::Vector2f& currentPosition);
	void adjustAllToCurrent(const sf::Vector2f& currentPosition);

public:
	// Constructors / Destructors
	MovementRoutine();
	MovementRoutine(const std::string& name, MovementRoutineEngine* movementRoutineEnginePtr);
	MovementRoutine(const std::string& name, MovementRoutineEngine* movementRoutineEnginePtr, const movementInfo& movement);
	MovementRoutine(const std::string& name, MovementRoutineEngine* movementRoutineEnginePtr, const std::vector<movementInfo*> movements);
	MovementRoutine(const MovementRoutine& obj);
	~MovementRoutine();

	// Public functions
	void addMovement(const movementInfo& movement);
	void removeMovement(const movementInfo& movement);

	void clear();
	void reset();

	const bool start(sf::Shape& shape);
	const bool start(sf::Sprite& sprite);
	const bool start(VertexArray2& vertexArray);

	void stop(sf::Shape& shape);
	void stop(sf::Sprite& sprite);
	void stop(VertexArray2& vertexArray);

	movementInfo* getCurrentMovement();

	const bool goToNextMovement(const sf::Shape& shape);
	const bool goToNextMovement(const sf::Sprite& sprite);
	const bool goToNextMovement(VertexArray2& vertexArray);

	const long long int& size() const;
};

class ScalingRoutine : public TransformationRoutine {
private:
	std::vector <scalingInfo*> m_routineScalings;

	// Private functions
	void adjustStartToCurrent(const sf::Vector2f& m_current_scale);
	void adjustAllToCurrent(const sf::Vector2f& m_current_scale);

public:
	// Constructors / Destructors
	ScalingRoutine();
	ScalingRoutine(const std::string& name, MovementRoutineEngine* movementRoutineEnginePtr);
	ScalingRoutine(const std::string& name, MovementRoutineEngine* movementRoutineEnginePtr, const scalingInfo& scaling);
	ScalingRoutine(const std::string& name, MovementRoutineEngine* movementRoutineEnginePtr, const std::vector<scalingInfo*> scalings);
	ScalingRoutine(const ScalingRoutine& obj);
	~ScalingRoutine();

	// Public functions
	void addScaling(const scalingInfo& scaling);
	void removeScaling(const scalingInfo& scaling);

	void clear();
	void reset();

	const bool start(sf::Shape& shape);
	const bool start(sf::Sprite& sprite);
	const bool start(VertexArray2& vertexArray);

	void stop(sf::Shape& shape);
	void stop(sf::Sprite& sprite);
	void stop(VertexArray2& vertexArray);

	scalingInfo* getCurrentScaling();

	const bool goToNextScaling(const sf::Shape& shape);
	const bool goToNextScaling(const sf::Sprite& sprite);
	const bool goToNextScaling(VertexArray2& vertexArray);

	const long long int& size() const;
};

class RotationRoutine : public TransformationRoutine {
private:
	std::vector <rotationInfo*> m_routineRotations;
	bool was_last_clockwise{};

	void adjustStartToCurrent(float current_rotation);
	void adjustAllToCurrent(const float current_rotation);

public:
	// Constructors / Destructors
	RotationRoutine();
	RotationRoutine(const std::string& name, MovementRoutineEngine* movementRoutineEnginePtr);
	RotationRoutine(const std::string& name, MovementRoutineEngine* movementRoutineEnginePtr, const rotationInfo& rotation);
	RotationRoutine(const std::string& name, MovementRoutineEngine* movementRoutineEnginePtr, const std::vector<rotationInfo*> rotations);
	RotationRoutine(const RotationRoutine& obj);
	~RotationRoutine();

	// Public functions
	void addRotation(const rotationInfo& rotation);
	void removeRotation(const rotationInfo& rotation);

	void clear();
	void reset();

	const bool start(sf::Shape& shape);
	const bool start(sf::Sprite& sprite);
	const bool start(VertexArray2& vertexArray);

	void stop(sf::Shape& shape);
	void stop(sf::Sprite& sprite);
	void stop(VertexArray2& vertexArray);

	rotationInfo* getCurrentRotation();

	const bool goToNextRotation(const sf::Shape& shape);
	const bool goToNextRotation(const sf::Sprite& sprite);
	const bool goToNextRotation(VertexArray2& vertexArray);

	const long long int& size() const;
};