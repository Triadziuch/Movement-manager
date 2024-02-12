#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>

// Movement info struct
struct transformationInfo {
	bool		 repeat = false;
	float		 current_time{};
	float		 wait_before_repeating{};
	double (*used_function)(double) {};

	transformationInfo() = default;

	transformationInfo(bool _repeat, float _wait_before_repeating, double (*_used_function)(double)) :
		repeat(_repeat), wait_before_repeating(_wait_before_repeating), used_function(_used_function) {}

	transformationInfo(bool _repeat, float _current_time, float _wait_before_repeating, double (*_used_function)(double)) :
		repeat(_repeat), current_time(_current_time), wait_before_repeating(_wait_before_repeating), used_function(_used_function) {}

	transformationInfo(const transformationInfo& obj) :
		repeat(obj.repeat), current_time(obj.current_time), wait_before_repeating(obj.wait_before_repeating), used_function(obj.used_function) {}

	virtual ~transformationInfo() = default;

	virtual const bool isDone() const = 0;
};

struct movementInfo : public transformationInfo {
	sf::Vector2f starting_pos{};
	sf::Vector2f ending_pos{};
	float		 movement_time{};

	movementInfo(sf::Vector2f _starting_pos, sf::Vector2f _ending_pos, float _movement_time, double (*_used_function)(double), bool _repeat, float _wait_before_repeating) :
		transformationInfo{ _repeat, _wait_before_repeating, _used_function }, starting_pos(_starting_pos), ending_pos(_ending_pos), movement_time(_movement_time) {}

	movementInfo(const movementInfo& obj) :
		transformationInfo{ obj.repeat, obj.current_time, obj.wait_before_repeating, obj.used_function }, starting_pos(obj.starting_pos), ending_pos(obj.ending_pos), movement_time(obj.movement_time) {}

	const bool isDone() const override { return this->current_time >= this->movement_time; }
};

struct scalingInfo : public transformationInfo {
	sf::Vector2f starting_scale{};
	sf::Vector2f ending_scale{};
	float		 scaling_time{};

	scalingInfo(sf::Vector2f _starting_scale, sf::Vector2f _ending_scale, float _scaling_time, double(*_used_function)(double), bool _repeat, float _wait_before_repeating) :
		transformationInfo{ _repeat, _wait_before_repeating, _used_function }, starting_scale(_starting_scale), ending_scale(_ending_scale), scaling_time(_scaling_time) {}

	scalingInfo(const scalingInfo& obj) :
		transformationInfo{ obj.repeat, obj.current_time, obj.wait_before_repeating, obj.used_function }, starting_scale(obj.starting_scale), ending_scale(obj.ending_scale), scaling_time(obj.scaling_time) {}

	const bool isDone() const override { return current_time >= scaling_time; }
};

struct rotationInfo : public transformationInfo {
	float starting_rotation{};
	float ending_rotation{};
	float rotation_time{};
	bool  clockwise = true;

	rotationInfo(float _starting_rotation, float _ending_rotation, float _rotation_time, double(*_used_function)(double), bool _repeat, float _wait_before_repeating, bool _clockwise) :
		transformationInfo{ _repeat, _wait_before_repeating, _used_function }, starting_rotation(_starting_rotation), ending_rotation(_ending_rotation), rotation_time(_rotation_time), clockwise(_clockwise) {}

	rotationInfo(const rotationInfo& obj) :
		transformationInfo{ obj.repeat, obj.current_time, obj.wait_before_repeating, obj.used_function }, starting_rotation(obj.starting_rotation), ending_rotation(obj.ending_rotation), rotation_time(obj.rotation_time), clockwise(obj.clockwise) {}

	const bool isDone() const override { return current_time >= rotation_time; }

	const float getRotation() const { 
		if (this->clockwise)
			return static_cast<float>(this->used_function(static_cast<double>(this->current_time / this->rotation_time))) * (this->ending_rotation - this->starting_rotation) + this->starting_rotation;
		else {
			float new_rotation = static_cast<float>(this->used_function(static_cast<double>(this->current_time / this->rotation_time))) * (this->ending_rotation - this->starting_rotation) + this->starting_rotation;
			new_rotation = 360.f - new_rotation;
			new_rotation = fmod(new_rotation, 360.f);
			return new_rotation;
		}
	}

	const float getStartingRotation() const {
		if (this->clockwise)
			return this->starting_rotation;
		else {
			float new_rotation = 360.f - this->starting_rotation;
			new_rotation = fmod(new_rotation, 360.f);
			return new_rotation;
		}
	}

	const float getEndingRotation() const {
		if (this->clockwise)
			return this->ending_rotation;
		else {
			float new_rotation = 360.f - this->ending_rotation;
			new_rotation = fmod(new_rotation, 360.f);
			return new_rotation;
		}
	}
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

	transformationInfoVA(bool _repeat, float _current_time, float _wait_before_repeating, double (*_used_function)(double), sf::VertexArray* _vertexarray) :
		transformationInfo{ _repeat, _current_time, _wait_before_repeating, _used_function }, originalVertex(*_vertexarray) {

		for (size_t i = 0; i < _vertexarray->getVertexCount(); i++)
			centroid += _vertexarray->operator[](i).position;
		centroid /= static_cast<float>(_vertexarray->getVertexCount());
	}

	transformationInfoVA(const transformationInfoVA& obj) :
		transformationInfo{ obj.repeat, obj.current_time, obj.wait_before_repeating, obj.used_function }, originalVertex(obj.originalVertex), centroid(obj.centroid) {}
};

struct movementInfoVA : public transformationInfoVA {
	sf::Vector2f starting_pos{};
	sf::Vector2f ending_pos{};
	float		 movement_time{};
	sf::Vector2f originalCentroid{};

	movementInfoVA(sf::Vector2f _starting_pos, sf::Vector2f _ending_pos, float _movement_time, double(*_used_function)(double), bool _repeat, float _wait_before_repeating, sf::VertexArray* _vertexarray) :
		transformationInfoVA{ _repeat, _wait_before_repeating, _used_function, _vertexarray }, starting_pos(_starting_pos), ending_pos(_ending_pos), movement_time(_movement_time) {
		this->originalCentroid = this->centroid;
	}

	movementInfoVA(const movementInfoVA& obj) :
		transformationInfoVA{ obj.repeat, obj.current_time, obj.wait_before_repeating, obj.used_function, const_cast<sf::VertexArray*>(&obj.originalVertex) }, starting_pos(obj.starting_pos), ending_pos(obj.ending_pos), movement_time(obj.movement_time), originalCentroid(obj.originalCentroid) {}

	const bool isDone() const override { return current_time >= movement_time; }
};

struct scalingInfoVA : public transformationInfoVA {
	sf::Vector2f starting_scale{};
	sf::Vector2f ending_scale{};
	sf::Vector2f current_scale{};
	float		 scaling_time{};

	scalingInfoVA(sf::Vector2f _starting_scale, sf::Vector2f _ending_scale, float _scaling_time, double(*_used_function)(double), bool _repeat, float _wait_before_repeating, sf::VertexArray* _vertexarray) :
		transformationInfoVA{ _repeat, _wait_before_repeating, _used_function, _vertexarray }, starting_scale(_starting_scale), ending_scale(_ending_scale), current_scale(_starting_scale), scaling_time(_scaling_time) {}

	scalingInfoVA(const scalingInfoVA& obj) :
		transformationInfoVA{ obj.repeat, obj.current_time, obj.wait_before_repeating, obj.used_function, const_cast<sf::VertexArray*>(&obj.originalVertex) }, starting_scale(obj.starting_scale), ending_scale(obj.ending_scale), current_scale(obj.current_scale), scaling_time(obj.scaling_time) {}

	const bool isDone() const override { return current_time >= scaling_time; }
};

struct rotationInfoVA : public transformationInfoVA {
	float starting_rotation{};
	float ending_rotation{};
	float current_rotation{};
	float rotation_time{};
	bool  clockwise = true;

	rotationInfoVA(float _starting_rotation, float _ending_rotation, float _rotation_time, double(*_used_function)(double), sf::VertexArray* _vertexarray, bool _repeat, float _wait_before_repeating, bool _clockwise) :
		transformationInfoVA{ _repeat, _wait_before_repeating, _used_function, _vertexarray }, starting_rotation(_starting_rotation), ending_rotation(_ending_rotation), current_rotation(_starting_rotation), rotation_time(_rotation_time), clockwise(_clockwise) {}

	rotationInfoVA(const rotationInfoVA& obj) :
		transformationInfoVA{ obj.repeat, obj.current_time, obj.wait_before_repeating, obj.used_function, const_cast<sf::VertexArray*>(&obj.originalVertex) }, starting_rotation(obj.starting_rotation), ending_rotation(obj.ending_rotation), current_rotation(obj.current_rotation), rotation_time(obj.rotation_time), clockwise(obj.clockwise) {}

	const bool isDone() const override { return current_time >= rotation_time; }

	const float getRotation() const {
		if (this->clockwise)
			return static_cast<float>(this->used_function(static_cast<double>(this->current_time / this->rotation_time))) * (this->ending_rotation - this->starting_rotation) + this->starting_rotation;
		else {
			float new_rotation = static_cast<float>(this->used_function(static_cast<double>(this->current_time / this->rotation_time))) * (this->ending_rotation - this->starting_rotation) + this->starting_rotation;
			new_rotation = 360.f - new_rotation;
			new_rotation = fmod(new_rotation, 360.f);
			return new_rotation;
		}
	}

	const float getStartingRotation() const {
		if (this->clockwise)
			return this->starting_rotation;
		else {
			float new_rotation = 360.f - this->starting_rotation;
			new_rotation = fmod(new_rotation, 360.f);
			return new_rotation;
		}
	}

	const float getEndingRotation() const {
		if (this->clockwise)
			return this->ending_rotation;
		else {
			float new_rotation = 360.f - this->ending_rotation;
			new_rotation = fmod(new_rotation, 360.f);
			return new_rotation;
		}
	}
};