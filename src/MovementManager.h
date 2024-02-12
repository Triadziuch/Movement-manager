#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include "easeFunctions.cpp"

class MovementManager {
private:
	constexpr static double M_RAD = 3.14159265358979323846 / 180.0;

	// Movement info struct
	struct transformationInfo {
		bool		 repeat = false;
		float		 currentTime{};
		float		 wait_before_repeating{};
		double (*used_function)(double) {};

		transformationInfo() = default;

		transformationInfo(bool _repeat, float _wait_before_repeating, double (*_used_function)(double)) : 
			repeat(_repeat), wait_before_repeating(_wait_before_repeating), used_function(_used_function) {}

		transformationInfo(bool _repeat, float _currentTime, float _wait_before_repeating, double (*_used_function)(double)) : 
			repeat(_repeat), currentTime(_currentTime), wait_before_repeating(_wait_before_repeating), used_function(_used_function) {}

		transformationInfo(const transformationInfo& obj) : 
			repeat(obj.repeat), currentTime(obj.currentTime), wait_before_repeating(obj.wait_before_repeating), used_function(obj.used_function) {}

		virtual ~transformationInfo() = default;

		virtual const bool isDone() const = 0;
	};

	struct movementInfo : public transformationInfo {
		sf::Vector2f startingPos{};
		sf::Vector2f endingPos{};
		float		 movementTime{};

		movementInfo(sf::Vector2f _startingPos, sf::Vector2f _endingPos, float _movementTime, double (*_used_function)(double), bool _repeat, float _wait_before_repeating) : 
			transformationInfo{ _repeat, _wait_before_repeating, _used_function }, startingPos(_startingPos), endingPos(_endingPos), movementTime(_movementTime) {}

		movementInfo(const movementInfo& obj) : 
			transformationInfo{obj.repeat, obj.currentTime, obj.wait_before_repeating, obj.used_function}, startingPos(obj.startingPos), endingPos(obj.endingPos), movementTime(obj.movementTime) {}

		const bool isDone() const override { return this->currentTime >= this->movementTime; }
	};

	struct scalingInfo : public transformationInfo {
		sf::Vector2f startingScale{};
		sf::Vector2f endingScale{};
		float		 scalingTime{};

		scalingInfo(sf::Vector2f _startingScale, sf::Vector2f _endingScale, float _scalingTime, double(*_used_function)(double), bool _repeat, float _wait_before_repeating) :
			transformationInfo{ _repeat, _wait_before_repeating, _used_function }, startingScale(_startingScale), endingScale(_endingScale), scalingTime(_scalingTime) {}

		scalingInfo(const scalingInfo& obj) : 
			transformationInfo{obj.repeat, obj.currentTime, obj.wait_before_repeating, obj.used_function}, startingScale(obj.startingScale), endingScale(obj.endingScale), scalingTime(obj.scalingTime) {}

		const bool isDone() const override { return currentTime >= scalingTime; }
	};

	struct rotationInfo : public transformationInfo {
		float startingRotation{};
		float endingRotation{};
		float rotationTime{};
		bool  clockwise = true;

		rotationInfo(float _startingRotation, float _endingRotation, float _rotationTime, double(*_used_function)(double), bool _repeat, float _wait_before_repeating, bool _clockwise) :
			transformationInfo{ _repeat, _wait_before_repeating, _used_function }, startingRotation(_startingRotation), endingRotation(_endingRotation), rotationTime(_rotationTime), clockwise(_clockwise) {}

		rotationInfo(const rotationInfo& obj) :
			transformationInfo{obj.repeat, obj.currentTime, obj.wait_before_repeating, obj.used_function}, startingRotation(obj.startingRotation), endingRotation(obj.endingRotation), rotationTime(obj.rotationTime), clockwise(obj.clockwise) {}

		const bool isDone() const override { return currentTime >= rotationTime; }
	};

	struct transformationInfoVA : public transformationInfo {
		sf::VertexArray	 originalVertex{};
		sf::Vector2f	 centroid{};

		transformationInfoVA() = default;

		transformationInfoVA(bool _repeat, float _wait_before_repeating, double (*_used_function)(double), sf::VertexArray* _vertexarray) :
			transformationInfo{ _repeat, _wait_before_repeating, _used_function }, originalVertex(*_vertexarray) {

			for (size_t i = 0; i < _vertexarray->getVertexCount(); i++)
				centroid += _vertexarray->operator[](i).position;
			centroid /= static_cast<float>(_vertexarray->getVertexCount());
		}

		transformationInfoVA(bool _repeat, float _currentTime, float _wait_before_repeating, double (*_used_function)(double), sf::VertexArray* _vertexarray) :
			transformationInfo{ _repeat, _currentTime, _wait_before_repeating, _used_function }, originalVertex(*_vertexarray) {

			for (size_t i = 0; i < _vertexarray->getVertexCount(); i++)
				centroid += _vertexarray->operator[](i).position;
			centroid /= static_cast<float>(_vertexarray->getVertexCount());
		}

		transformationInfoVA(const transformationInfoVA& obj) :
			transformationInfo{ obj.repeat, obj.currentTime, obj.wait_before_repeating, obj.used_function }, originalVertex(obj.originalVertex), centroid(obj.centroid) {}
	};

	struct movementInfoVA : public transformationInfoVA {
		sf::Vector2f startingPos{};
		sf::Vector2f endingPos{};
		float		 movementTime{};
		sf::Vector2f originalCentroid{};

		movementInfoVA(sf::Vector2f _startingPos, sf::Vector2f _endingPos, float _movementTime, double(*_used_function)(double), bool _repeat, float _wait_before_repeating, sf::VertexArray* _vertexarray) :
			transformationInfoVA{ _repeat, _wait_before_repeating, _used_function, _vertexarray }, startingPos(_startingPos), endingPos(_endingPos), movementTime(_movementTime) { this->originalCentroid = this->centroid; }

		movementInfoVA(const movementInfoVA& obj) :
			transformationInfoVA{ obj.repeat, obj.currentTime, obj.wait_before_repeating, obj.used_function, const_cast<sf::VertexArray*>(&obj.originalVertex)}, startingPos(obj.startingPos), endingPos(obj.endingPos), movementTime(obj.movementTime), originalCentroid(obj.originalCentroid) {}

		const bool isDone() const override { return currentTime >= movementTime; }
	};

	struct scalingInfoVA : public transformationInfoVA {
		sf::Vector2f startingScale{};
		sf::Vector2f endingScale{};
		sf::Vector2f currentScale{};
		float		 scalingTime{};

		scalingInfoVA(sf::Vector2f _startingScale, sf::Vector2f _endingScale, float _scalingTime, double(*_used_function)(double), bool _repeat, float _wait_before_repeating, sf::VertexArray* _vertexarray) :
			transformationInfoVA{ _repeat, _wait_before_repeating, _used_function, _vertexarray }, startingScale(_startingScale), endingScale(_endingScale), currentScale(_startingScale), scalingTime(_scalingTime) {}

		scalingInfoVA(const scalingInfoVA& obj) :
			transformationInfoVA{ obj.repeat, obj.currentTime, obj.wait_before_repeating, obj.used_function, const_cast<sf::VertexArray*>(&obj.originalVertex) }, startingScale(obj.startingScale), endingScale(obj.endingScale), currentScale(obj.currentScale), scalingTime(obj.scalingTime) {}

		const bool isDone() const override { return currentTime >= scalingTime; }
	};

	struct rotationInfoVA : public transformationInfoVA {
		float startingRotation{};
		float endingRotation{};
		float current_rotation{};
		float rotationTime{};
		bool  clockwise = true;

		rotationInfoVA(float _startingRotation, float _endingRotation, float _rotationTime, double(*_used_function)(double), sf::VertexArray* _vertexarray, bool _repeat, float _wait_before_repeating, bool _clockwise) :
			transformationInfoVA{ _repeat, _wait_before_repeating, _used_function, _vertexarray }, startingRotation(_startingRotation), endingRotation(_endingRotation), current_rotation(_startingRotation), rotationTime(_rotationTime), clockwise(_clockwise) {}

		rotationInfoVA(const rotationInfoVA& obj) :
			transformationInfoVA{ obj.repeat, obj.currentTime, obj.wait_before_repeating, obj.used_function, const_cast<sf::VertexArray*>(&obj.originalVertex) }, startingRotation(obj.startingRotation), endingRotation(obj.endingRotation), current_rotation(obj.current_rotation), rotationTime(obj.rotationTime), clockwise(obj.clockwise) {}

		const bool isDone() const override { return currentTime >= rotationTime; }
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
	
	std::map<sf::Shape*, movementInfo*>			    m_Movements_Shape;
	std::map<sf::VertexArray*, movementInfoVA*>		m_Movements_VA;
	std::map<sf::Sprite*, movementInfo*>			m_Movements_S;

	std::map<sf::Shape*, scalingInfo*>				m_Scalings_Shape;
	std::map<sf::VertexArray*, scalingInfoVA*>		m_Scalings_VA;
	std::map<sf::Sprite*, scalingInfo*>				m_Scalings_S;

	std::map<sf::Shape*, rotationInfo*>				m_Rotations_Shape;
	std::map<sf::VertexArray*, rotationInfoVA*>		m_Rotations_VA;
	std::map<sf::Sprite*, rotationInfo*>			m_Rotations_S;

	void updateMovementCentroidOriginalVertex(sf::VertexArray* _vertexarray, movementInfoVA* _movementInfo, sf::Vector2f _offset);
	void updateScalingCentroidOriginalVertex(sf::VertexArray* _vertexarray, scalingInfoVA* _scalingInfo);
	void updateRotationCentroidOriginalVertex(sf::VertexArray* _vertexarray, rotationInfoVA* _rotationInfo);

	// Singleton instance
	static MovementManager* sInstance;

	// Private update functions
	void updateShape(float dt);
	void updateVertexArray(float dt);
	void updateSprite(float dt);

public:
	// Default constructor
	MovementManager();

	// Update functions
	void update(float dt);

	// Public functions

	// Movement functions
	const bool addMovement(sf::Shape* _shape, sf::Vector2f endingPos, float movementTime, movement_type _used_function, bool _repeat = false, float _wait_before_repeating = 0.f);
	const bool addMovement(sf::Shape* _shape, sf::Vector2f startingPos, sf::Vector2f endingPos, float movementTime, movement_type _used_function, bool _repeat = false, float _wait_before_repeating = 0.f);

	const bool addMovement(sf::VertexArray* _vertexarray, sf::Vector2f endingPos, float movementTime, movement_type _used_function, bool _repeat = false, float _wait_before_repeating = 0.f);
	const bool addMovement(sf::VertexArray* _vertexarray, sf::Vector2f startingPos, sf::Vector2f endingPos, float movementTime, movement_type _used_function, bool _repeat = false, float _wait_before_repeating = 0.f);

	const bool addMovement(sf::Sprite* _sprite, sf::Vector2f endingPos, float movementTime, movement_type _used_function, bool _repeat = false, float _wait_before_repeating = 0.f);
	const bool addMovement(sf::Sprite* _sprite, sf::Vector2f startingPos, sf::Vector2f endingPos, float movementTime, movement_type _used_function, bool _repeat = false, float _wait_before_repeating = 0.f);
	
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
	const bool addScaling(sf::Shape* _shape, sf::Vector2f endingScale, float scalingTime, movement_type _used_function, bool _repeat = false, float _wait_before_repeating = 0.f);
	const bool addScaling(sf::Shape* _shape, sf::Vector2f startingScale, sf::Vector2f endingScale, float scalingTime, movement_type _used_function, bool _repeat = false, float _wait_before_repeating = 0.f);

	const bool addScaling(sf::VertexArray* _vertexarray, sf::Vector2f endingScale, float scalingTime, movement_type _used_function, bool _repeat = false, float _wait_before_repeating = 0.f);
	const bool addScaling(sf::VertexArray* _vertexarray, sf::Vector2f startingScale, sf::Vector2f endingScale, float scalingTime, movement_type _used_function, bool _repeat = false, float _wait_before_repeating = 0.f);

	const bool addScaling(sf::Sprite* _sprite, sf::Vector2f endingScale, float scalingTime, movement_type _used_function, bool _repeat = false, float _wait_before_repeating = 0.f);
	const bool addScaling(sf::Sprite* _sprite, sf::Vector2f startingScale, sf::Vector2f endingScale, float scalingTime, movement_type _used_function, bool _repeat = false, float _wait_before_repeating = 0.f);

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
	const bool addRotation(sf::Shape* _shape, float endingRotation, float rotationTime, movement_type _used_function, bool _clockwise = true, bool _repeat = false, float _wait_before_repeating = 0.f);
	const bool addRotation(sf::Shape* _shape, float startingRotation, float endingRotation, float rotationTime, movement_type _used_function, bool _clockwise = true, bool _repeat = false, float _wait_before_repeating = 0.f);

	const bool addRotation(sf::VertexArray* _vertexarray, float endingRotation, float rotationTime, movement_type _used_function, bool _clockwise = true, bool _repeat = false, float _wait_before_repeating = 0.f);
	const bool addRotation(sf::VertexArray* _vertexarray, float startingRotation, float endingRotation, float rotationTime, movement_type _used_function, bool _clockwise = true, bool _repeat = false, float _wait_before_repeating = 0.f);

	const bool addRotation(sf::Sprite* _sprite, float endingRotation, float rotationTime, movement_type _used_function, bool _clockwise = true, bool _repeat = false, float _wait_before_repeating = 0.f);
	const bool addRotation(sf::Sprite* _sprite, float startingRotation, float endingRotation, float rotationTime, movement_type _used_function, bool _clockwise = true, bool _repeat = false, float _wait_before_repeating = 0.f);

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

	double (*getFunctionPointer(movement_type _movement_type))(double) { return movement_functions[_movement_type]; }
};