#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <unordered_map>
#include "easeFunctions.cpp"

class MovementManager {
private:
	// Movement info struct
	struct movementInfo {
		sf::Vector2f startingPos{};
		sf::Vector2f endingPos{};
		float		 movementTime{};
		float		 currentTime{};
		double (*used_function)(double) {};

		movementInfo(sf::Vector2f _startingPos, sf::Vector2f _endingPos, float _movementTime, double(*_used_function)(double)) :
			startingPos(_startingPos), endingPos(_endingPos), movementTime(_movementTime), currentTime(0.f), used_function(_used_function) {};

		movementInfo(const movementInfo& _movementInfo) {
			startingPos = _movementInfo.startingPos;
			endingPos = _movementInfo.endingPos;
			movementTime = _movementInfo.movementTime;
			currentTime =_movementInfo.currentTime;
			used_function = _movementInfo.used_function;
		}

		const bool isDone() { return currentTime >= movementTime; }
	};

	std::map<movement_type, double(*)(double)> movement_functions = {
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
	std::map<sf::CircleShape*, movementInfo*>  m_Movements;

	// Singleton
	static MovementManager* sInstance;

public:
	// Default constructor
	MovementManager();

	// Update functions
	void update(float dt);

	// Public functions
	static void addMovement(sf::Vector2f startingPos, sf::Vector2f endingPos, float movementTime, sf::CircleShape* _sprite, movement_type _used_function);
	void resetMovement();

	// Accessors / Mutators
	int getMovementCount() { return m_Movements.size(); }
	double (*getFunctionPointer(movement_type _movement_type))(double) { return movement_functions[_movement_type]; }
};