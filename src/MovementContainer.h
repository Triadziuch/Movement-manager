#pragma once
#include "easeFunctions.cpp"
#include "transformationInfo.h"
#include <map>
#include <assert.h>

class MovementContainer {
private:
	constexpr static double M_RAD = 3.14159265358979323846 / 180.0;

	std::map<movement_type, double(*)(double)>		movement_functions = {
		{movement_type::IN_SINE, inSine},
		{movement_type::OUT_SINE, outSine},
		{movement_type::IN_OUT_SINE, inOutSine},
		{movement_type::IN_QUAD, inQuad},
		{movement_type::OUT_QUAD, outQuad},
		{movement_type::IN_OUT_QUAD, inOutQuad},
		{movement_type::IN_CUBIC, inCubic},
		{movement_type::OUT_CUBIC, outCubic},
		{movement_type::IN_OUT_CUBIC, inOutCubic},
		{movement_type::IN_QUART, inQuart},
		{movement_type::OUT_QUART, outQuart},
		{movement_type::IN_OUT_QUART, inOutQuart},
		{movement_type::IN_QUINT, inQuint},
		{movement_type::OUT_QUINT, outQuint},
		{movement_type::IN_OUT_QUINT, inOutQuint},
		{movement_type::IN_EXPO, inExpo},
		{movement_type::OUT_EXPO, outExpo},
		{movement_type::IN_OUT_EXPO, inOutExpo},
		{movement_type::IN_CIRC, inCirc},
		{movement_type::OUT_CIRC, outCirc},
		{movement_type::IN_OUT_CIRC, inOutCirc},
		{movement_type::IN_BACK, inBack},
		{movement_type::OUT_BACK, outBack},
		{movement_type::IN_OUT_BACK, inOutBack},
		{movement_type::IN_ELASTIC, inElastic},
		{movement_type::OUT_ELASTIC, outElastic},
		{movement_type::IN_OUT_ELASTIC, inOutElastic},
		{movement_type::IN_BOUNCE, inBounce},
		{movement_type::OUT_BOUNCE, outBounce},
		{movement_type::IN_OUT_BOUNCE, inOutBounce}
	};
	
	std::map<sf::Shape*, movementInfo*>			    m_Movements_Shape;
	std::map<sf::VertexArray*, movementInfoVA*>		m_Movements_VA;
	std::map<sf::Sprite*, movementInfo*>			m_Movements_S;

	std::map<sf::Shape*, scalingInfo*>				m_Scalings_Shape;
	std::map<sf::VertexArray*, scalingInfoVA*>		m_Scalings_VA;
	std::map<sf::Sprite*, scalingInfo*>				m_Scalings_S;

	std::map<sf::Shape*, rotationInfo*>				m_Rotations_Shape;
	std::map<sf::VertexArray*, rotationInfoVA*>		m_Rotations_VA;
	std::map<sf::Sprite*, rotationInfo*>			m_Rotations_S;

	// Singleton instance
	static MovementContainer* sInstance;

	// Private update functions
	void updateMovementInfoVA(sf::VertexArray* _vertexarray, movementInfoVA* _movementInfo, sf::Vector2f _offset);
	void updateScalingInfoVA(sf::VertexArray* _vertexarray, scalingInfoVA* _scalingInfo);
	void updateRotationInfoVA(sf::VertexArray* _vertexarray, rotationInfoVA* _rotationInfo);
	
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
	const movementInfo* addMovement(sf::Shape* _shape, sf::Vector2f _ending_pos, float _movement_time, movement_type _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);
	const movementInfo* addMovement(sf::Shape* _shape, sf::Vector2f _starting_pos, sf::Vector2f _ending_pos, float _movement_time, movement_type _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);
	const movementInfo* addMovement(sf::Shape* _shape, float _offset_x, float _offset_y, float _movement_time, movement_type _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);

	const movementInfoVA* addMovement(sf::VertexArray* _vertexarray, sf::Vector2f _ending_pos, float _movement_time, movement_type _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);
	const movementInfoVA* addMovement(sf::VertexArray* _vertexarray, sf::Vector2f _starting_pos, sf::Vector2f _ending_pos, float _movement_time, movement_type _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);
	const movementInfoVA* addMovement(sf::VertexArray* _vertexarray, float _offset_x, float _offset_y, float _movement_time, movement_type _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);

	const movementInfo* addMovement(sf::Sprite* _sprite, sf::Vector2f _ending_pos, float _movement_time, movement_type _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);
	const movementInfo* addMovement(sf::Sprite* _sprite, sf::Vector2f _starting_pos, sf::Vector2f _ending_pos, float _movement_time, movement_type _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);
	const movementInfo* addMovement(sf::Sprite* _sprite, float _offset_x, float _offset_y, float _movement_time, movement_type _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);
	
	void undoMovement();
	void undoMovement(sf::Shape* _shape);
	void undoMovement(sf::VertexArray* _vertexarray);
	void undoMovement(sf::Sprite* _sprite);

	void resetMovement();
	void resetMovement(sf::Shape* _shape);
	void resetMovement(sf::VertexArray* _vertexarray);
	void resetMovement(sf::Sprite* _sprite);

	void stopMovement();
	void stopMovement(sf::Shape* _shape);
	void stopMovement(sf::VertexArray* _vertexarray);
	void stopMovement(sf::Sprite* _sprite);

	// Scaling functions
	const bool addScaling(sf::Shape* _shape, sf::Vector2f _ending_scale, float _scaling_time, movement_type _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);
	const bool addScaling(sf::Shape* _shape, sf::Vector2f _starting_scale, sf::Vector2f _ending_scale, float _scaling_time, movement_type _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);

	const bool addScaling(sf::VertexArray* _vertexarray, sf::Vector2f _ending_scale, float _scaling_time, movement_type _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);
	const bool addScaling(sf::VertexArray* _vertexarray, sf::Vector2f _starting_scale, sf::Vector2f _ending_scale, float _scaling_time, movement_type _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);

	const bool addScaling(sf::Sprite* _sprite, sf::Vector2f _ending_scale, float _scaling_time, movement_type _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);
	const bool addScaling(sf::Sprite* _sprite, sf::Vector2f _starting_scale, sf::Vector2f _ending_scale, float _scaling_time, movement_type _used_function, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);

	void undoScaling();
	void undoScaling(sf::Shape* _shape);
	void undoScaling(sf::VertexArray* _vertexarray);
	void undoScaling(sf::Sprite* _sprite);

	void resetScaling();
	void resetScaling(sf::Shape* _shape);
	void resetScaling(sf::VertexArray* _vertexarray);
	void resetScaling(sf::Sprite* _sprite);

	void stopScaling();
	void stopScaling(sf::Shape* _shape);
	void stopScaling(sf::VertexArray* _vertexarray);
	void stopScaling(sf::Sprite* _sprite);

	// Rotation functions
	const bool addRotation(sf::Shape* _shape, float _ending_rotation, float _rotation_time, movement_type _used_function, bool _clockwise = true, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);
	const bool addRotation(sf::Shape* _shape, float _starting_rotation, float _ending_rotation, float _rotation_time, movement_type _used_function, bool _clockwise = true, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);

	const bool addRotation(sf::VertexArray* _vertexarray, float _ending_rotation, float _rotation_time, movement_type _used_function, bool _clockwise = true, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);
	const bool addRotation(sf::VertexArray* _vertexarray, float _starting_rotation, float _ending_rotation, float _rotation_time, movement_type _used_function, bool _clockwise = true, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);

	const bool addRotation(sf::Sprite* _sprite, float _ending_rotation, float _rotation_time, movement_type _used_function, bool _clockwise = true, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);
	const bool addRotation(sf::Sprite* _sprite, float _starting_rotation, float _ending_rotation, float _rotation_time, movement_type _used_function, bool _clockwise = true, bool _repeat = false, float _delay_before = 0.f, float _delay_after = 0.f);
	
	void undoRotation();
	void undoRotation(sf::Shape* _shape);
	void undoRotation(sf::VertexArray* _vertexarray);
	void undoRotation(sf::Sprite* _sprite);

	void resetRotation();
	void resetRotation(sf::Shape* _shape);
	void resetRotation(sf::VertexArray* _vertexarray);
	void resetRotation(sf::Sprite* _sprite);

	void stopRotation();
	void stopRotation(sf::Shape* _shape);
	void stopRotation(sf::VertexArray* _vertexarray);
	void stopRotation(sf::Sprite* _sprite);

	// Accessors / Mutators
	int getMovementCount() { return m_Movements_Shape.size() + m_Movements_VA.size() + m_Movements_S.size(); }
	int getScalingCount() { return m_Scalings_Shape.size() + m_Scalings_VA.size() + m_Scalings_S.size(); }
	int getRotationCount() { return m_Rotations_Shape.size() + m_Rotations_VA.size() + m_Rotations_S.size(); }

	const bool isMoving(sf::Shape* _shape);
	const bool isMoving(sf::VertexArray* _vertexarray);
	const bool isMoving(sf::Sprite* _sprite);

	const bool isScaling(sf::Shape* _shape);
	const bool isScaling(sf::VertexArray* _vertexarray);
	const bool isScaling(sf::Sprite* _sprite);

	const bool isRotating(sf::Shape* _shape);
	const bool isRotating(sf::VertexArray* _vertexarray);
	const bool isRotating(sf::Sprite* _sprite);

	double (*getFunctionPointer(movement_type _movement_type))(double) { return movement_functions[_movement_type]; }
};