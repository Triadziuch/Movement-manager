#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>

// Movement info class
class transformationInfo {
public:
	bool		 repeat = false;
	float		 current_time{};
	float		 motion_duration{};
	float		 delay_before{};
	float		 delay_after{};
	float		 total_duration{};
	double (*used_function)(double) {};

	transformationInfo() = default;

	transformationInfo(bool _repeat, float _motion_duration, float _delay_before, float _delay_after, double (*_used_function)(double)) :
		repeat(_repeat), motion_duration(_motion_duration), delay_before(_delay_before), delay_after(_delay_after), used_function(_used_function), total_duration(_delay_before + _motion_duration + _delay_after) {}

	transformationInfo(bool _repeat, float _current_time, float _motion_duration, float _delay_before, float _delay_after, double (*_used_function)(double)) :
		repeat(_repeat), current_time(_current_time), motion_duration(_motion_duration), delay_before(_delay_before), delay_after(_delay_after), used_function(_used_function), total_duration(_delay_before + _motion_duration + _delay_after) {}

	transformationInfo(const transformationInfo& obj) :
		repeat(obj.repeat), current_time(obj.current_time), motion_duration(obj.motion_duration), delay_before(obj.delay_before), delay_after(obj.delay_after), used_function(obj.used_function), total_duration(delay_before + motion_duration + delay_after) {}

	virtual ~transformationInfo() = default;

	void reset() { this->current_time = 0.f; }

	const bool isDone() const { return (this->current_time - this->delay_before >= this->motion_duration); }

	const bool isFinished() const { return (this->current_time >= this->total_duration); }
};

class movementInfo : public transformationInfo {
public:
	sf::Vector2f starting_pos{};
	sf::Vector2f ending_pos{};


	movementInfo(sf::Vector2f _starting_pos, sf::Vector2f _ending_pos, float _motion_duration, double (*_used_function)(double), bool _repeat, float _delay_before, float _delay_after) :
		transformationInfo{ _repeat, _motion_duration, _delay_before, _delay_after, _used_function }, starting_pos(_starting_pos), ending_pos(_ending_pos) {}

	movementInfo(const movementInfo& obj) :
		transformationInfo{ obj.repeat, obj.current_time, obj.motion_duration, obj.delay_before, obj.delay_after, obj.used_function }, starting_pos(obj.starting_pos), ending_pos(obj.ending_pos) {}

	const sf::Vector2f getStartingPos() const { return this->starting_pos; }
	const sf::Vector2f getEndingPos()	const { return this->ending_pos; }

	void adjustStartingPositionByOffset(const sf::Vector2f& _offset) { this->starting_pos += _offset; }
	void adjustEndingPositionByOffset(const sf::Vector2f& _offset)	 { this->ending_pos += _offset; }
	void adjustMovementByOffset(const sf::Vector2f& _offset)		 { this->starting_pos += _offset; this->ending_pos += _offset; }
};

class scalingInfo : public transformationInfo {
public:
	sf::Vector2f starting_scale{};
	sf::Vector2f ending_scale{};


	scalingInfo(sf::Vector2f _starting_scale, sf::Vector2f _ending_scale, float _motion_duration, double(*_used_function)(double), bool _repeat, float _delay_before, float _delay_after) :
		transformationInfo{ _repeat, _motion_duration, _delay_before, _delay_after, _used_function }, starting_scale(_starting_scale), ending_scale(_ending_scale) {}

	scalingInfo(const scalingInfo& obj) :
		transformationInfo{ obj.repeat, obj.current_time, obj.motion_duration, obj.delay_before, obj.delay_after, obj.used_function }, starting_scale(obj.starting_scale), ending_scale(obj.ending_scale) {}
};

class rotationInfo : public transformationInfo {
public:
	float starting_rotation{};
	float ending_rotation{};
	bool  clockwise = true;


	rotationInfo(float _starting_rotation, float _ending_rotation, float _motion_duration, double(*_used_function)(double), bool _repeat, float _delay_before, float _delay_after, bool _clockwise) :
		transformationInfo{ _repeat, _motion_duration, _delay_before, _delay_after, _used_function }, starting_rotation(_starting_rotation), ending_rotation(_ending_rotation), clockwise(_clockwise) {}

	rotationInfo(const rotationInfo& obj) :
		transformationInfo{ obj.repeat, obj.current_time, obj.motion_duration, obj.delay_before, obj.delay_after, obj.used_function }, starting_rotation(obj.starting_rotation), ending_rotation(obj.ending_rotation), clockwise(obj.clockwise) {}

	const float updateRotation() const { 
		if (this->clockwise)
			return static_cast<float>(this->used_function(static_cast<double>((this->current_time - this->delay_before) / this->motion_duration))) * (this->ending_rotation - this->starting_rotation) + this->starting_rotation;
		else {
			float new_rotation = static_cast<float>(this->used_function(static_cast<double>((this->current_time - this->delay_before) / this->motion_duration))) * (this->ending_rotation - this->starting_rotation) + this->starting_rotation;
			//new_rotation = 360.f - new_rotation;
			//new_rotation = fmod(new_rotation, 360.f);
			return -new_rotation;
		}
	}

	const float getStartingRotation() const {
		if (this->clockwise)
			return this->starting_rotation;
		else 
			return -starting_rotation;
	}

	const float getEndingRotation() const {
		if (this->clockwise)
			return this->ending_rotation;
		else 
			return -this->ending_rotation;
	}

	void adjustRotationByOffset(const float _offset) { 
		this->starting_rotation += _offset; 
		this->ending_rotation += _offset; 
	}
};

// TODO: Dodaæ sprawdzanie, czy przypadkiem nie zmieniliœmy liczby wierzcho³ków i w razie potrzeby zaktualizowaæ centroid i originalVertex

class transformationInfoVA : public transformationInfo {
public:
	sf::VertexArray	 originalVertex{};
	sf::Vector2f	 centroid{};


	transformationInfoVA() = default;

	transformationInfoVA(bool _repeat, float _motion_duration, float _delay_before, float _delay_after, double (*_used_function)(double), sf::VertexArray* _vertexarray) :
		transformationInfo{ _repeat, _motion_duration, _delay_before, _delay_after, _used_function }, originalVertex(*_vertexarray) {

		for (size_t i = 0; i < _vertexarray->getVertexCount(); i++)
			centroid += _vertexarray->operator[](i).position;
		centroid /= static_cast<float>(_vertexarray->getVertexCount());
	}

	transformationInfoVA(bool _repeat, float _current_time, float _motion_duration, float _delay_before, float _delay_after, double (*_used_function)(double), sf::VertexArray* _vertexarray) :
		transformationInfo{ _repeat, _current_time, _motion_duration, _delay_before, _delay_after, _used_function }, originalVertex(*_vertexarray) {

		for (size_t i = 0; i < _vertexarray->getVertexCount(); i++)
			centroid += _vertexarray->operator[](i).position;
		centroid /= static_cast<float>(_vertexarray->getVertexCount());
	}

	transformationInfoVA(const transformationInfoVA& obj) :
		transformationInfo{ obj.repeat, obj.current_time, obj.motion_duration, obj.delay_before, obj.delay_after, obj.used_function }, originalVertex(obj.originalVertex), centroid(obj.centroid) {}
};

class movementInfoVA : public transformationInfoVA {
public:
	sf::Vector2f starting_pos{};
	sf::Vector2f ending_pos{};
	sf::Vector2f originalCentroid{};


	movementInfoVA(sf::Vector2f _starting_pos, sf::Vector2f _ending_pos, float _motion_duration, double(*_used_function)(double), bool _repeat, float _delay_before, float _delay_after, sf::VertexArray* _vertexarray) :
		transformationInfoVA{ _repeat, _motion_duration, _delay_before, _delay_after, _used_function, _vertexarray }, starting_pos(_starting_pos), ending_pos(_ending_pos) {
		this->originalCentroid = this->centroid;
	}

	movementInfoVA(const movementInfoVA& obj) :
		transformationInfoVA{ obj.repeat, obj.current_time, obj.motion_duration, obj.delay_before, obj.delay_after, obj.used_function, const_cast<sf::VertexArray*>(&obj.originalVertex) }, starting_pos(obj.starting_pos), ending_pos(obj.ending_pos), originalCentroid(obj.originalCentroid) {}

	void moveCentroidsToStartingPosition() {
		sf::Vector2f offset = this->starting_pos - this->centroid;
		for (size_t i = 0; i < this->originalVertex.getVertexCount(); i++)
			this->originalVertex[i].position += offset;
		this->centroid += offset;
	}
};

class scalingInfoVA : public transformationInfoVA {
public:
	sf::Vector2f starting_scale{};
	sf::Vector2f ending_scale{};
	sf::Vector2f current_scale{};


	scalingInfoVA(sf::Vector2f _starting_scale, sf::Vector2f _ending_scale, float _motion_duration, double(*_used_function)(double), bool _repeat, float _delay_before, float _delay_after, sf::VertexArray* _vertexarray) :
		transformationInfoVA{ _repeat, _motion_duration, _delay_before, _delay_after, _used_function, _vertexarray }, starting_scale(_starting_scale), ending_scale(_ending_scale), current_scale(_starting_scale) {}

	scalingInfoVA(const scalingInfoVA& obj) :
		transformationInfoVA{ obj.repeat, obj.current_time, obj.motion_duration, obj.delay_before, obj.delay_after, obj.used_function, const_cast<sf::VertexArray*>(&obj.originalVertex) }, starting_scale(obj.starting_scale), ending_scale(obj.ending_scale), current_scale(obj.current_scale) {}

	void reset() {
		this->current_time = 0.f;
		this->current_scale = this->starting_scale;
	}

	void recalculateCentroidsOriginalVertex(const sf::VertexArray* vertexArray) {
		sf::Vector2f current_centroid{};
		for (size_t i = 0; i < vertexArray->getVertexCount(); i++)
			current_centroid += vertexArray->operator[](i).position;
		current_centroid /= static_cast<float>(vertexArray->getVertexCount());

		sf::Vector2f offset = current_centroid - this->centroid;
		this->centroid = current_centroid;
		for (size_t i = 0; i < vertexArray->getVertexCount(); i++)
			this->originalVertex[i].position += offset;
	}
};

class rotationInfoVA : public transformationInfoVA {
public:
	float starting_rotation{};
	float ending_rotation{};
	float current_rotation{};
	bool  clockwise = true;


	rotationInfoVA(float _starting_rotation, float _ending_rotation, float _motion_duration, double(*_used_function)(double), sf::VertexArray* _vertexarray, bool _repeat, float _delay_before, float _delay_after, bool _clockwise) :
		transformationInfoVA{ _repeat, _motion_duration, _delay_before, _delay_after, _used_function, _vertexarray } {
		this->clockwise = _clockwise;

		if (this->clockwise) {
			this->starting_rotation = _starting_rotation;
			this->ending_rotation = _ending_rotation;
		}
		else {
			this->starting_rotation = 360.f - _starting_rotation;
			this->ending_rotation = 360.f - _ending_rotation;
		}

		this->current_rotation = this->starting_rotation;
	}

	rotationInfoVA(const rotationInfoVA& obj) :
		transformationInfoVA{ obj.repeat, obj.current_time, obj.motion_duration, obj.delay_before, obj.delay_after, obj.used_function, const_cast<sf::VertexArray*>(&obj.originalVertex) }, starting_rotation(obj.starting_rotation), ending_rotation(obj.ending_rotation), current_rotation(obj.current_rotation), clockwise(obj.clockwise) {}

	const float updateRotation()  {
		float new_rotation = static_cast<float>(this->used_function(static_cast<double>((this->current_time - this->delay_before) / this->motion_duration))) * (this->ending_rotation - this->starting_rotation) + this->starting_rotation;
		return new_rotation;
	}

	void reset() {
		this->current_time = 0.f;

		this->current_rotation = this->starting_rotation;
	}

	void recalculateCentroidsOriginalVertex(const sf::VertexArray* vertexArray) {
		sf::Vector2f current_centroid{};
		for (size_t i = 0; i < vertexArray->getVertexCount(); i++)
			current_centroid += vertexArray->operator[](i).position;
		current_centroid /= static_cast<float>(vertexArray->getVertexCount());

		sf::Vector2f offset = current_centroid - this->centroid;
		this->centroid = current_centroid;
		for (size_t i = 0; i < vertexArray->getVertexCount(); i++)
			this->originalVertex[i].position += offset;
	}

	const float getStartingRotation() const {
		return this->starting_rotation;
	}

	const float getCurrentRotation() const {
		return this->current_rotation;
	}

	const float getEndingRotation() const {
		return this->ending_rotation;
	}

	void setCurrentRotation(const float rotation) {
		this->current_rotation = rotation;
	}
};