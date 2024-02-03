#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include "easeFunctions.cpp"

class MovementManager {
private:
	// Movement info struct
	struct movementInfo {
		sf::Vector2f startingPos{};
		sf::Vector2f endingPos{};
		float		 currentTime{};
		float		 movementTime{};
		bool		 repeat = false;
		float		 repeat_timer{};
		float		 wait_before_repeating{};
		double (*used_function)(double) {};

		movementInfo(sf::Vector2f _startingPos, sf::Vector2f _endingPos, float _movementTime, double(*_used_function)(double), bool _repeat, bool _wait_before_repeating) :
			startingPos(_startingPos), endingPos(_endingPos), movementTime(_movementTime), currentTime(0.f), used_function(_used_function), repeat(_repeat), wait_before_repeating(_wait_before_repeating) {};

		movementInfo(const movementInfo& _movementInfo) {
			startingPos = _movementInfo.startingPos;
			endingPos = _movementInfo.endingPos;
			movementTime = _movementInfo.movementTime;
			currentTime =_movementInfo.currentTime;
			used_function = _movementInfo.used_function;
		}

		const bool isDone() { return currentTime >= movementTime; }
	};

	struct scalingInfo {
		sf::Vector2f startingScale{};
		sf::Vector2f endingScale{};
		float		 currentTime{};
		float		 scalingTime{};
		bool		 repeat = false;
		float		 repeat_timer{};
		float		 wait_before_repeating{};
		double (*used_function)(double) {};

		scalingInfo(sf::Vector2f _startingScale, sf::Vector2f _endingScale, float _scalingTime, double(*_used_function)(double), bool _repeat, bool _wait_before_repeating) :
			startingScale(_startingScale), endingScale(_endingScale), scalingTime(_scalingTime), currentTime(0.f), used_function(_used_function), repeat(_repeat), wait_before_repeating(_wait_before_repeating) {};

		scalingInfo(const scalingInfo& _scalingInfo) {
			startingScale = _scalingInfo.startingScale;
			endingScale = _scalingInfo.endingScale;
			scalingTime = _scalingInfo.scalingTime;
			currentTime = _scalingInfo.currentTime;
			used_function = _scalingInfo.used_function;
		}

		const bool isDone() { return currentTime >= scalingTime; }
	};

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
	
	std::map<sf::CircleShape*, movementInfo*>		m_Movements_CS;
	std::map<sf::VertexArray*, movementInfo*>		m_Movements_VA;
	std::map<sf::Sprite*, movementInfo*>			m_Movements_S;
	std::map<sf::RectangleShape*, movementInfo*>	m_Movements_RS;

	std::map<sf::CircleShape*, scalingInfo*>		m_Scalings_CS;
	std::map<sf::Sprite*, scalingInfo*>				m_Scalings_S;
	std::map<sf::RectangleShape*, scalingInfo*>		m_Scalings_RS;

	// Singleton instance
	static MovementManager* sInstance;

	// Private update functions
	void updateCircleShape(float dt);
	void updateVertexArray(float dt);
	void updateSprite(float dt);
	void updateRectangleShape(float dt);

public:
	// Default constructor
	MovementManager();

	// Update functions
	void update(float dt);

	// Public functions

	// Movement functions
	const bool addMovement(sf::CircleShape* _circleshape, sf::Vector2f endingPos, float movementTime, movement_type _used_function, bool _repeat = false, float _wait_before_repeating = 0.f);
	const bool addMovement(sf::CircleShape* _circleshape, sf::Vector2f startingPos, sf::Vector2f endingPos, float movementTime, movement_type _used_function, bool _repeat = false, float _wait_before_repeating = 0.f);

	const bool addMovement(sf::VertexArray* _vertexarray, sf::Vector2f endingPos, float movementTime, movement_type _used_function, bool _repeat = false, float _wait_before_repeating = 0.f);
	const bool addMovement(sf::VertexArray* _vertexarray, sf::Vector2f startingPos, sf::Vector2f endingPos, float movementTime, movement_type _used_function, bool _repeat = false, float _wait_before_repeating = 0.f);

	const bool addMovement(sf::Sprite* _sprite, sf::Vector2f endingPos, float movementTime, movement_type _used_function, bool _repeat = false, float _wait_before_repeating = 0.f);
	const bool addMovement(sf::Sprite* _sprite, sf::Vector2f startingPos, sf::Vector2f endingPos, float movementTime, movement_type _used_function, bool _repeat = false, float _wait_before_repeating = 0.f);
	
	const bool addMovement(sf::RectangleShape* _rectangleshape, sf::Vector2f endingPos, float movementTime, movement_type _used_function, bool _repeat = false, float _wait_before_repeating = 0.f);
	const bool addMovement(sf::RectangleShape* _rectangleshape, sf::Vector2f startingPos, sf::Vector2f endingPos, float movementTime, movement_type _used_function, bool _repeat = false, float _wait_before_repeating = 0.f);

	void undoMovement();
	void undoMovement(sf::CircleShape* _circleshape);
	void undoMovement(sf::VertexArray* _vertexarray);
	void undoMovement(sf::Sprite* _sprite);
	void undoMovement(sf::RectangleShape* _rectangleshape);

	void resetMovement();
	void resetMovement(sf::CircleShape* _circleshape);
	void resetMovement(sf::VertexArray* _vertexarray);
	void resetMovement(sf::Sprite* _sprite);
	void resetMovement(sf::RectangleShape* _rectangleshape);

	void stopMovement();
	void stopMovement(sf::CircleShape* _circleshape);
	void stopMovement(sf::VertexArray* _vertexarray);
	void stopMovement(sf::Sprite* _sprite);
	void stopMovement(sf::RectangleShape* _rectangleshape);

	// Scaling functions
	const bool addScaling(sf::CircleShape* _circleshape, sf::Vector2f endingScale, float scalingTime, movement_type _used_function, bool _repeat = false, float _wait_before_repeating = 0.f);
	const bool addScaling(sf::CircleShape* _circleshape, sf::Vector2f startingScale, sf::Vector2f endingScale, float scalingTime, movement_type _used_function, bool _repeat = false, float _wait_before_repeating = 0.f);

	const bool addScaling(sf::Sprite* _sprite, sf::Vector2f endingScale, float scalingTime, movement_type _used_function, bool _repeat = false, float _wait_before_repeating = 0.f);
	const bool addScaling(sf::Sprite* _sprite, sf::Vector2f startingScale, sf::Vector2f endingScale, float scalingTime, movement_type _used_function, bool _repeat = false, float _wait_before_repeating = 0.f);

	const bool addScaling(sf::RectangleShape* _rectangleshape, sf::Vector2f endingScale, float scalingTime, movement_type _used_function, bool _repeat = false, float _wait_before_repeating = 0.f);
	const bool addScaling(sf::RectangleShape* _rectangleshape, sf::Vector2f startingScale, sf::Vector2f endingScale, float scalingTime, movement_type _used_function, bool _repeat = false, float _wait_before_repeating = 0.f);

	void undoScaling();
	void undoScaling(sf::CircleShape* _circleshape);
	void undoScaling(sf::Sprite* _sprite);
	void undoScaling(sf::RectangleShape* _rectangleshape);

	void resetScaling();
	void resetScaling(sf::CircleShape* _circleshape);
	void resetScaling(sf::Sprite* _sprite);
	void resetScaling(sf::RectangleShape* _rectangleshape);

	void stopScaling();
	void stopScaling(sf::CircleShape* _circleshape);
	void stopScaling(sf::Sprite* _sprite);
	void stopScaling(sf::RectangleShape* _rectangleshape);


	// Accessors / Mutators
	int getMovementCount() { return m_Movements_CS.size(); }
	double (*getFunctionPointer(movement_type _movement_type))(double) { return movement_functions[_movement_type]; }
};