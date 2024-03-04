#pragma once
#include "easeFunctions.h"
#include "transformationInfo.h"
#include <map>
#include <assert.h>
#include <chrono>

#include "Instrumentor.h"
#define PROFILING 1
#if PROFILING
#define PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)
#else
#define PROFILE_SCOPE(name)
#endif

using namespace std::chrono;

class MovementManager;
class TransformationRoutine;
class MovementRoutine;
class ScalingRoutine;
class RotationRoutine;

class MovementContainerBase {
protected:
	constexpr static double M_RAD = 3.14159265358979323846 / 180.0;
};

class MovementContainer : public MovementContainerBase{
private:
	std::map<sf::Shape*, movementInfo*>			    m_Movements_Shape;
	std::map<VertexArray2*, movementInfo*>			m_Movements_VA;
	std::map<sf::Sprite*, movementInfo*>			m_Movements_S;

	std::map<sf::Shape*, scalingInfo*>				m_Scalings_Shape;
	std::map<VertexArray2*, scalingInfo*>			m_Scalings_VA;
	std::map<sf::Sprite*, scalingInfo*>				m_Scalings_S;

	std::map<sf::Shape*, rotationInfo*>				m_Rotations_Shape;
	std::map<VertexArray2*, rotationInfo*>			m_Rotations_VA;
	std::map<sf::Sprite*, rotationInfo*>			m_Rotations_S;

	// Singleton instance
	static MovementContainer* sInstance;

	// Private update functions
	void updateShape(float dt);
	void updateVertexArray(float dt);
	void updateSprite(float dt);
	
public:
	// Default constructor
	MovementContainer();

	// Update functions
	void update(float dt);

	// Public functions

	// Movement functions
	const movementInfo* addMovement(sf::Shape* _shape, movementInfo* _movementInfo);
	const movementInfo* addMovement(sf::Shape* _shape, sf::Vector2f _ending_pos, float _movement_time, easeFunctions::Tmovement_function _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);
	const movementInfo* addMovement(sf::Shape* _shape, sf::Vector2f _starting_pos, sf::Vector2f _ending_pos, float _movement_time, easeFunctions::Tmovement_function _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);
	const movementInfo* addMovement(sf::Shape* _shape, float _offset_x, float _offset_y, float _movement_time, easeFunctions::Tmovement_function _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);

	const movementInfo* addMovement(VertexArray2* _vertexarray, movementInfo* _movementInfo);
	const movementInfo* addMovement(VertexArray2* _vertexarray, sf::Vector2f _ending_pos, float _movement_time, easeFunctions::Tmovement_function _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);
	const movementInfo* addMovement(VertexArray2* _vertexarray, sf::Vector2f _starting_pos, sf::Vector2f _ending_pos, float _movement_time, easeFunctions::Tmovement_function _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);
	const movementInfo* addMovement(VertexArray2* _vertexarray, float _offset_x, float _offset_y, float _movement_time, easeFunctions::Tmovement_function _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);

	const movementInfo* addMovement(sf::Sprite* _sprite, movementInfo* _movementInfo);
	const movementInfo* addMovement(sf::Sprite* _sprite, sf::Vector2f _ending_pos, float _movement_time, easeFunctions::Tmovement_function _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);
	const movementInfo* addMovement(sf::Sprite* _sprite, sf::Vector2f _starting_pos, sf::Vector2f _ending_pos, float _movement_time, easeFunctions::Tmovement_function _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);
	const movementInfo* addMovement(sf::Sprite* _sprite, float _offset_x, float _offset_y, float _movement_time, easeFunctions::Tmovement_function _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);
	
	void undoMovement();
	void undoMovement(sf::Shape* _shape);
	void undoMovement(VertexArray2* _vertexarray);
	void undoMovement(sf::Sprite* _sprite);

	void resetMovement();
	void resetMovement(sf::Shape* _shape);
	void resetMovement(VertexArray2* _vertexarray);
	void resetMovement(sf::Sprite* _sprite);

	void stopMovement();
	void stopMovement(sf::Shape* _shape);
	void stopMovement(VertexArray2* _vertexarray);
	void stopMovement(sf::Sprite* _sprite);

	void swapMovement(sf::Shape* _shape, movementInfo* _newMovementInfo);

	// Scaling functions
	const bool addScaling(sf::Shape* _shape, sf::Vector2f _ending_scale, float _scaling_time, easeFunctions::Tmovement_function _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);
	const bool addScaling(sf::Shape* _shape, sf::Vector2f _starting_scale, sf::Vector2f _ending_scale, float _scaling_time, easeFunctions::Tmovement_function _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);

	const bool addScaling(VertexArray2* _vertexarray, sf::Vector2f _ending_scale, float _scaling_time, easeFunctions::Tmovement_function _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);
	const bool addScaling(VertexArray2* _vertexarray, sf::Vector2f _starting_scale, sf::Vector2f _ending_scale, float _scaling_time, easeFunctions::Tmovement_function _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);

	const bool addScaling(sf::Sprite* _sprite, sf::Vector2f _ending_scale, float _scaling_time, easeFunctions::Tmovement_function _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);
	const bool addScaling(sf::Sprite* _sprite, sf::Vector2f _starting_scale, sf::Vector2f _ending_scale, float _scaling_time, easeFunctions::Tmovement_function _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);

	void undoScaling();
	void undoScaling(sf::Shape* _shape);
	void undoScaling(VertexArray2* _vertexarray);
	void undoScaling(sf::Sprite* _sprite);

	void resetScaling();
	void resetScaling(sf::Shape* _shape);
	void resetScaling(VertexArray2* _vertexarray);
	void resetScaling(sf::Sprite* _sprite);

	void stopScaling();
	void stopScaling(sf::Shape* _shape);
	void stopScaling(VertexArray2* _vertexarray);
	void stopScaling(sf::Sprite* _sprite);

	// Rotation functions
	const bool addRotation(sf::Shape* _shape, float _ending_rotation, float _rotation_time, easeFunctions::Tmovement_function _used_function, bool _clockwise = true, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);
	const bool addRotation(sf::Shape* _shape, float _starting_rotation, float _ending_rotation, float _rotation_time, easeFunctions::Tmovement_function _used_function, bool _clockwise = true, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);

	const bool addRotation(VertexArray2* _vertexarray, float _ending_rotation, float _rotation_time, easeFunctions::Tmovement_function _used_function, bool _clockwise = true, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);
	const bool addRotation(VertexArray2* _vertexarray, float _starting_rotation, float _ending_rotation, float _rotation_time, easeFunctions::Tmovement_function _used_function, bool _clockwise = true, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);

	const bool addRotation(sf::Sprite* _sprite, float _ending_rotation, float _rotation_time, easeFunctions::Tmovement_function _used_function, bool _clockwise = true, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);
	const bool addRotation(sf::Sprite* _sprite, float _starting_rotation, float _ending_rotation, float _rotation_time, easeFunctions::Tmovement_function _used_function, bool _clockwise = true, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);
	
	void undoRotation();
	void undoRotation(sf::Shape* _shape);
	void undoRotation(VertexArray2* _vertexarray);
	void undoRotation(sf::Sprite* _sprite);

	void resetRotation();
	void resetRotation(sf::Shape* _shape);
	void resetRotation(VertexArray2* _vertexarray);
	void resetRotation(sf::Sprite* _sprite);

	void stopRotation();
	void stopRotation(sf::Shape* _shape);
	void stopRotation(VertexArray2* _vertexarray);
	void stopRotation(sf::Sprite* _sprite);

	// Accessors / Mutators
	int getMovementCount() { return m_Movements_Shape.size() + m_Movements_VA.size() + m_Movements_S.size(); }
	int getScalingCount() { return m_Scalings_Shape.size() + m_Scalings_VA.size() + m_Scalings_S.size(); }
	int getRotationCount() { return m_Rotations_Shape.size() + m_Rotations_VA.size() + m_Rotations_S.size(); }

	const bool isMoving(sf::Shape* _shape);
	const bool isMoving(VertexArray2* _vertexarray);
	const bool isMoving(sf::Sprite* _sprite);

	const bool isScaling(sf::Shape* _shape);
	const bool isScaling(VertexArray2* _vertexarray);
	const bool isScaling(sf::Sprite* _sprite);

	const bool isRotating(sf::Shape* _shape);
	const bool isRotating(VertexArray2* _vertexarray);
	const bool isRotating(sf::Sprite* _sprite);
};

class MovementRoutineEngine : protected MovementContainerBase {
private:
	std::map<sf::Transformable*, std::tuple<MovementRoutine*, ScalingRoutine*, RotationRoutine*>*>	m_Routines;

	MovementManager* movementManager{};

	// Singleton instance
	static MovementRoutineEngine* sInstance;

	// Constructors / Destructors
	MovementRoutineEngine() {};

public:
	MovementRoutineEngine(const MovementRoutineEngine&) = delete;
	MovementRoutineEngine& operator=(const MovementRoutineEngine&) = delete;

	static MovementRoutineEngine* getInstance();

	void setMovementManager(MovementManager* _movementManager);

	// Update functions
	void update(float dt);

	// Public functions

	// Movement functions
	const MovementRoutine* addMovement(sf::Transformable* transformable, MovementRoutine* _movementRoutine);

	void undoMovement();
	void undoMovement(sf::Transformable* transformable);

	void resetMovement();

	void stopMovement();
	void stopMovement(sf::Transformable* transformable);

	// Scaling functions
	const ScalingRoutine* addScaling(sf::Transformable* transformable, ScalingRoutine* _scalingRoutine);

	void undoScaling();
	void undoScaling(sf::Transformable* transformable);

	void resetScaling();

	void stopScaling();
	void stopScaling(sf::Transformable* transformable);

	// Rotation functions
	const RotationRoutine* addRotation(sf::Transformable* transformable, RotationRoutine* _rotationRoutine);

	void undoRotation();
	void undoRotation(sf::Transformable* transformable);

	void resetRotation();

	void stopRotation();
	void stopRotation(sf::Transformable* transformable);

	// Accessors / Mutators
	const bool isMoving(sf::Transformable* transformable);
	const bool isScaling(sf::Transformable* transformable);
	const bool isRotating(sf::Transformable* transformable);
};