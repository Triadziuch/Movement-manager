#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <unordered_map>
#include "easeFunctions.cpp"

class MovementManager {
private:

	struct movementInfo {
		sf::Vector2f startingPos{};
		sf::Vector2f endingPos{};
		float		 movementTime{};
		float		 currentTime{};
		float (*used_function)(float, float) {};

		movementInfo(sf::Vector2f _startingPos, sf::Vector2f _endingPos, float _movementTime, float(*_used_function)(float, float)) :
			startingPos(_startingPos), endingPos(_endingPos), movementTime(_movementTime), currentTime(0.f), used_function(_used_function) {};

		movementInfo(const movementInfo& _movementInfo) {
			startingPos = _movementInfo.startingPos;
			endingPos = _movementInfo.endingPos;
			movementTime = _movementInfo.movementTime;
			currentTime =_movementInfo.currentTime;
			used_function = _movementInfo.used_function;
		}

		bool isDone() { return currentTime >= movementTime; }
	};

	//float (*return_movement_function(movement_type type))(float, float); madafaka

	std::map<movement_type, float(*)(float, float)> movement_functions = {
		{movement_type::LINEAR, linear},
		{movement_type::IN_POW, inPow},
		{movement_type::OUT_POW, outPow},
		{movement_type::IN_OUT_POW, inOutPow},
		{movement_type::IN_SIN, inSin},
		{movement_type::OUT_SIN, outSin},
		{movement_type::IN_OUT_SIN, inOutSin},
		{movement_type::IN_EXP, inExp},
		{movement_type::OUT_EXP, outExp},
		{movement_type::IN_OUT_EXP, inOutExp},
		{movement_type::IN_CIRC, inCirc},
		{movement_type::OUT_CIRC, outCirc},
		{movement_type::IN_OUT_CIRC, inOutCirc},
		{movement_type::REBOUND, rebound},
		{movement_type::IN_BACK, inBack},
		{movement_type::OUT_BACK, outBack},
		{movement_type::IN_OUT_BACK, inOutBack}
	};

	std::map<sf::CircleShape*, movementInfo*>  m_Movements;

	static MovementManager* sInstance;
public:
	

	MovementManager();

	void update(float dt);

	static void addMovement(sf::Vector2f startingPos, sf::Vector2f endingPos, float movementTime, sf::CircleShape* _sprite, movement_type _used_function);

	int getMovementCount() { return m_Movements.size(); }
};