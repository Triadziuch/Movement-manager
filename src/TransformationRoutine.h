#pragma once
#include "transformationInfo.h"
class MovementContainerBase;
class MovementRoutineEngine;

// - - - - - - - - - - - - - - - - - - - - TransformationRoutine - - - - - - - - - - - - - - - - - - - - \\

class TransformationRoutine {
protected:
	MovementRoutineEngine* m_movementRoutineEngine{};
	bool				   m_adjustStartToCurrentTransform{};
	bool				   m_adjustAllToCurrentTransform{};
	bool				   m_isLooping{};

public:
	std::string			   m_routineName{};
	size_t				   m_current{};
	size_t				   m_count{};
	bool				   m_isActive{};
	bool				   m_isPaused{};

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



// - - - - - - - - - - - - - - - - - - - - MovementRoutine - - - - - - - - - - - - - - - - - - - - \\

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
	MovementRoutine(const std::string& name, MovementRoutineEngine* movementRoutineEnginePtr, movementInfo* movement);
	MovementRoutine(const std::string& name, MovementRoutineEngine* movementRoutineEnginePtr, std::vector<movementInfo*> movements);
	MovementRoutine(const MovementRoutine& obj);
	~MovementRoutine();

	// Public functions
	void addMovement(movementInfo* movement);
	void removeMovement(movementInfo* movement);

	void clear();
	void reset();

	const bool start(sf::Shape& shape);
	const bool start(sf::Sprite& sprite);
	const bool start(VertexArray2& vertexArray);

	void reset(sf::Shape& shape);
	void reset(sf::Sprite& sprite);
	void reset(VertexArray2& vertexArray);

	void stop(sf::Shape* shape);
	void stop(sf::Sprite* sprite);
	void stop(VertexArray2* vertexArray);

	movementInfo* getCurrentMovement() const;

	const bool goToNextMovement(const sf::Shape& shape);
	const bool goToNextMovement(const sf::Sprite& sprite);
	const bool goToNextMovement(VertexArray2& vertexArray);

	void setFunction(easeFunctions::Tmovement_function usedFunctionType);
	void setFunction(easeFunctions::Tmovement_function usedFunctionType, const size_t movement_id);
	void setFunction(double(*usedFunctionPtr)(double));
	void setFunction(double(*usedFunctionPtr)(double), const size_t movement_id);

	const long long int& size() const;
};

// - - - - - - - - - - - - - - - - - - - - ScalingRoutine - - - - - - - - - - - - - - - - - - - - \\

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
	ScalingRoutine(const std::string& name, MovementRoutineEngine* movementRoutineEnginePtr, scalingInfo* scaling);
	ScalingRoutine(const std::string& name, MovementRoutineEngine* movementRoutineEnginePtr, std::vector<scalingInfo*> scalings);
	ScalingRoutine(const ScalingRoutine& obj);
	~ScalingRoutine();

	// Public functions
	void addScaling(scalingInfo* scaling);
	void removeScaling(scalingInfo* scaling);

	void clear();
	void reset();

	const bool start(sf::Shape& shape);
	const bool start(sf::Sprite& sprite);
	const bool start(VertexArray2& vertexArray);

	void reset(sf::Shape& shape);
	void reset(sf::Sprite& sprite);
	void reset(VertexArray2& vertexArray);

	void stop(sf::Shape* shape);
	void stop(sf::Sprite* sprite);
	void stop(VertexArray2* vertexArray);

	scalingInfo* getCurrentScaling() const;

	const bool goToNextScaling(const sf::Shape& shape);
	const bool goToNextScaling(const sf::Sprite& sprite);
	const bool goToNextScaling(VertexArray2& vertexArray);

	void setFunction(easeFunctions::Tmovement_function usedFunctionType);
	void setFunction(easeFunctions::Tmovement_function usedFunctionType, const size_t scaling_id);
	void setFunction(double(*usedFunctionPtr)(double));
	void setFunction(double(*usedFunctionPtr)(double), const size_t scaling_id);

	const long long int& size() const;
};



// - - - - - - - - - - - - - - - - - - - - RotationRoutine - - - - - - - - - - - - - - - - - - - - \\

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
	RotationRoutine(const std::string& name, MovementRoutineEngine* movementRoutineEnginePtr, rotationInfo* rotation);
	RotationRoutine(const std::string& name, MovementRoutineEngine* movementRoutineEnginePtr, std::vector<rotationInfo*> rotations);
	RotationRoutine(const RotationRoutine& obj);
	~RotationRoutine();

	// Public functions
	void addRotation(rotationInfo* rotation);
	void removeRotation(rotationInfo* rotation);

	void clear();
	void reset();

	const bool start(sf::Shape& shape);
	const bool start(sf::Sprite& sprite);
	const bool start(VertexArray2& vertexArray);

	void reset(sf::Shape& shape);
	void reset(sf::Sprite& sprite);
	void reset(VertexArray2& vertexArray);

	void stop(sf::Shape* shape);
	void stop(sf::Sprite* sprite);
	void stop(VertexArray2* vertexArray);

	rotationInfo* getCurrentRotation() const;

	const bool goToNextRotation(const sf::Shape& shape);
	const bool goToNextRotation(const sf::Sprite& sprite);
	const bool goToNextRotation(VertexArray2& vertexArray);

	void setFunction(easeFunctions::Tmovement_function usedFunctionType);
	void setFunction(easeFunctions::Tmovement_function usedFunctionType, const size_t rotation_id);
	void setFunction(double(*usedFunctionPtr)(double));
	void setFunction(double(*usedFunctionPtr)(double), const size_t rotation_id);

	const long long int& size() const;
};