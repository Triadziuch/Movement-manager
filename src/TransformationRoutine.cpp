#pragma once
#include "TransformationRoutine.h"
#include "MovementContainer.h"

// - - - - - - - - - - - - - - - - - - - - MovementRoutine - - - - - - - - - - - - - - - - - - - - \\

void MovementRoutine::adjustStartToCurrent(sf::Vector2f current_position)
{
	if (this->routine_movements.size() == 0) return;
	
	const sf::Vector2f offset = current_position - this->routine_movements[0]->getStartingPos();
	this->routine_movements[0]->adjustStartingPositionByOffset(offset);
}

void MovementRoutine::adjustAllToCurrent(sf::Vector2f current_position)
{
	if (this->routine_movements.size() == 0) return;

	const sf::Vector2f offset = current_position - this->routine_movements[0]->getStartingPos();
	for (auto& movement : this->routine_movements) 
		movement->adjustMovementByOffset(offset);
}

void MovementRoutine::addMovement(movementInfo* movement)
{
	this->routine_movements.emplace_back(movement);
	++this->count;
}

void MovementRoutine::removeMovement(movementInfo* movement) 
{
	auto it = std::find(this->routine_movements.begin(), this->routine_movements.end(), movement);

	if (it != this->routine_movements.end()) {
		this->routine_movements.erase(it);
		--this->count;
	}
}

void MovementRoutine::clear() 
{
	this->routine_movements.clear();
	this->current = 0;
	this->count = 0;
	this->is_active = false;
	this->is_paused = false;
	this->is_looping = false;
}

void MovementRoutine::reset() 
{
	for (auto& movement : this->routine_movements)
		movement->reset();

	this->current = 0;
	this->is_active = false;
	this->is_paused = false;
}

const bool MovementRoutine::start(sf::Shape* shape) 
{
	if (this->routine_movements.size() != 0) {
		if (this->adjust_all_to_current_transform)		  { this->adjustAllToCurrent(shape->getPosition()); }
		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(shape->getPosition()); }

		this->reset();

		this->current = 0;
		this->is_active = true;
		this->movementRoutineEngine->addMovement(shape, this);
		return true;
	}

	printf("MovementRoutine::start: Routine is empty\n");
	return false;
}

const bool MovementRoutine::start(sf::Sprite* sprite) 
{
	if (this->routine_movements.size() != 0) {
		if (this->adjust_all_to_current_transform)		  { this->adjustAllToCurrent(sprite->getPosition()); }
		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(sprite->getPosition()); }

		this->reset();

		this->current = 0;
		this->is_active = true;
		this->movementRoutineEngine->addMovement(sprite, this);
		return true;
	}

	printf("MovementRoutine::start: Routine is empty\n");
	return false;
}

const bool MovementRoutine::start(VertexArray2* vertexArray)
{
	if (this->routine_movements.size() != 0) {
		if (this->adjust_all_to_current_transform)		  { this->adjustAllToCurrent(vertexArray->getPosition()); }
		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(vertexArray->getPosition()); }

		this->reset();

		this->current = 0;
		this->is_active = true;
		this->movementRoutineEngine->addMovement(vertexArray, this);
		return true;
	}

	printf("MovementRoutine::start: Routine is empty\n");
	return false;
}

void MovementRoutine::stop(sf::Shape* shape) 
{
	this->reset();
	this->movementRoutineEngine->stopMovement(shape);
}

void MovementRoutine::stop(sf::Sprite* sprite) {
	this->reset();
	this->movementRoutineEngine->stopMovement(sprite);
}

void MovementRoutine::stop(VertexArray2* vertexArray)
{
	this->reset();
	this->movementRoutineEngine->stopMovement(vertexArray);
}

movementInfo* MovementRoutine::getCurrentMovement() 
{
	if (this->current < this->count)
		return this->routine_movements[this->current];
	else {
		printf("MovementRoutine::getCurrentMovement: Current movement index out of range\n");
		return nullptr;
	}
}

const bool MovementRoutine::goToNextMovement(sf::Shape* shape)
{
	if (this->current < this->count - 1) {
		++this->current;
		return true;
	}
	else {
		if (this->adjust_all_to_current_transform)		  { this->adjustAllToCurrent(shape->getPosition()); }
		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(shape->getPosition()); }
		this->reset();

		if (this->is_looping) {
			this->is_active = true;
			return true;
		}

		return false;
	}
}

const bool MovementRoutine::goToNextMovement(sf::Sprite* sprite)
{
	if (this->current < this->count - 1) {
		++this->current;
		return true;
	}
	else {
		if (this->adjust_all_to_current_transform)		  { this->adjustAllToCurrent(sprite->getPosition()); }
		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(sprite->getPosition()); }
		this->reset();

		if (this->is_looping) {
			this->is_active = true;
			return true;
		}

		return false;
	}
}

const bool MovementRoutine::goToNextMovement(VertexArray2* vertexArray)
{
	if (this->current < this->count - 1) {
		++this->current;
		return true;
	}
	else {
		if (this->adjust_all_to_current_transform)		  { this->adjustAllToCurrent(vertexArray->getPosition()); }
		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(vertexArray->getPosition()); }
		this->reset();

		if (this->is_looping) {
			this->is_active = true;
			return true;
		}

		return false;
	}
}

// - - - - - - - - - - - - - - - - - - - - ScalingRoutine - - - - - - - - - - - - - - - - - - - - \\

void ScalingRoutine::adjustStartToCurrent(const sf::Vector2f& current_scale)
{
	if (this->routine_scalings.size() == 0) return;
	this->routine_scalings[0]->starting_scale = current_scale;
}

void ScalingRoutine::adjustAllToCurrent(const sf::Vector2f& current_scale)
{
	if (this->routine_scalings.size() == 0) return;

	const sf::Vector2f proportion{current_scale.x / this->routine_scalings[0]->starting_scale.x, current_scale.y / this->routine_scalings[0]->starting_scale.y};

	if (proportion == sf::Vector2f{1.f, 1.f}) return;

	for (auto& scaling : this->routine_scalings) {
		scaling->starting_scale = sf::Vector2f{ scaling->starting_scale.x * proportion.x, scaling->starting_scale.y * proportion.y };
		scaling->ending_scale   = sf::Vector2f{ scaling->ending_scale.x * proportion.x, scaling->ending_scale.y * proportion.y };
	}
}

void ScalingRoutine::addScaling(scalingInfo* scaling)
{
	this->routine_scalings.emplace_back(scaling);
	++this->count;
}

void ScalingRoutine::removeScaling(scalingInfo* scaling)
{
	auto it = std::find(this->routine_scalings.begin(), this->routine_scalings.end(), scaling);

	if (it != this->routine_scalings.end()) {
		this->routine_scalings.erase(it); 
		--this->count;
	}
}

void ScalingRoutine::clear()
{
	this->routine_scalings.clear();
	this->current = 0;
	this->count = 0;
	this->is_active = false;
	this->is_paused = false;
	this->is_looping = false;
}

void ScalingRoutine::reset()
{
	for (auto& scaling : this->routine_scalings)
		scaling->reset();

	this->current = 0;
	this->is_active = false;
	this->is_paused = false;
}

const bool ScalingRoutine::start(sf::Shape* shape)
{
	if (this->routine_scalings.size() != 0) {
		if (this->adjust_all_to_current_transform)		  { this->adjustAllToCurrent(shape->getScale()); }
		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(shape->getScale()); }

		this->reset();

		this->current = 0;
		this->is_active = true;
		this->movementRoutineEngine->addScaling(shape, this);
		return true;
	}

	printf("ScalingRoutine::start: Routine is empty\n");
	return false;
}

const bool ScalingRoutine::start(sf::Sprite* sprite)
{
	if (this->routine_scalings.size() != 0) {
		if (this->adjust_all_to_current_transform)		  { this->adjustAllToCurrent(sprite->getPosition()); }
		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(sprite->getPosition()); }
		this->reset();

		this->current = 0;
		this->is_active = true;
		this->movementRoutineEngine->addScaling(sprite, this);
		return true;
	}

	printf("ScalingRoutine::start: Routine is empty\n");
	return false;
}

const bool ScalingRoutine::start(VertexArray2* vertexArray)
{
	if (this->routine_scalings.size() != 0) {
		if (this->adjust_all_to_current_transform)		  { this->adjustAllToCurrent(vertexArray->getPosition()); }
		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(vertexArray->getPosition()); }
		this->reset();

		this->current = 0;
		this->is_active = true;
		this->movementRoutineEngine->addScaling(vertexArray, this);
		return true;
	}

	printf("ScalingRoutine::start: Routine is empty\n");
	return false;
}

void ScalingRoutine::stop(sf::Shape* shape)
{
	this->reset();
	this->movementRoutineEngine->stopScaling(shape);
}

void ScalingRoutine::stop(sf::Sprite* sprite)
{
	this->reset();
	this->movementRoutineEngine->stopScaling(sprite);
}

void ScalingRoutine::stop(VertexArray2* vertexArray)
{
	this->reset();
	this->movementRoutineEngine->stopScaling(vertexArray);
}

scalingInfo* ScalingRoutine::getCurrentScaling()
{
	if (this->current < this->count)
		return this->routine_scalings[this->current];
	else {
		printf("ScalingRoutine::getCurrentScaling: Current scaling index out of range\n");
		return nullptr;
	}
}

const bool ScalingRoutine::goToNextScaling(sf::Shape* shape)
{
	if (this->current < this->count - 1) {
		++this->current;
		return true;
	}
	else {
		if (this->adjust_all_to_current_transform)		  { this->adjustAllToCurrent(shape->getScale()); }
		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(shape->getScale()); }
		this->reset();

		if (this->is_looping) {
			this->is_active = true;
			return true;
		}

		return false;
	}
}

const bool ScalingRoutine::goToNextScaling(sf::Sprite* sprite)
{
	if (this->current < this->count - 1) {
		++this->current;
		return true;
	}
	else {
		if (this->adjust_all_to_current_transform)		  { this->adjustAllToCurrent(sprite->getScale()); }
		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(sprite->getScale()); }
		this->reset();

		if (this->is_looping) {
			this->is_active = true;
			return true;
		}

		return false;
	}
}

const bool ScalingRoutine::goToNextScaling(VertexArray2* vertexArray)
{
	if (this->current < this->count - 1) {
		++this->current;
		return true;
	}
	else {
		if (this->adjust_all_to_current_transform)		  { this->adjustAllToCurrent(vertexArray->getScale()); }
		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(vertexArray->getScale()); }
		this->reset();

		if (this->is_looping) {
			this->is_active = true;
			return true;
		}

		return false;
	}
}

// - - - - - - - - - - - - - - - - - - - - RotationRoutine - - - - - - - - - - - - - - - - - - - - \\

void RotationRoutine::adjustStartToCurrent(const float& current_rotation)
{
	if (this->routine_rotations.size() == 0) return;
	
	rotationInfo* rotation = this->routine_rotations[0];
	float new_starting_rotation = current_rotation;

	if (!this->was_last_clockwise) {
		if (rotation->clockwise) {
			if (current_rotation > rotation->ending_rotation)
				new_starting_rotation -= 360.f;
		}
		else {
			new_starting_rotation -= 360.f;
			new_starting_rotation *= -1.f;

			if (new_starting_rotation > rotation->ending_rotation) 
				new_starting_rotation -= 360.f;
		}
	}
	else {
		if (rotation->clockwise) {
			if (current_rotation > rotation->ending_rotation)
				new_starting_rotation -= 360.f;
		}
		else if (rotation->clockwise == false){
			new_starting_rotation -= 360.f;
			new_starting_rotation *= -1.f;

			if (new_starting_rotation > rotation->ending_rotation)
				new_starting_rotation -= 360.f;
		}
	}

	if (rotation->clockwise)
		this->routine_rotations[0]->starting_rotation = new_starting_rotation;
	else 
		this->routine_rotations[0]->starting_rotation = 360.f - new_starting_rotation;
}

void RotationRoutine::adjustAllToCurrent(const float& current_rotation)
{
	if (this->routine_rotations.size() == 0) return;

	float rotation_offset = current_rotation - this->routine_rotations[0]->starting_rotation;

	if (!this->routine_rotations[0]->clockwise)
		rotation_offset = 360.f + rotation_offset;

	if (rotation_offset == 0.f || rotation_offset == 360.f) return;

	for (auto& rotation : this->routine_rotations) {
		float& new_starting_rotation = rotation->starting_rotation;
		float& new_ending_rotation   = rotation->ending_rotation;

		new_starting_rotation += rotation_offset;
		new_ending_rotation   += rotation_offset;
	}
}

void RotationRoutine::addRotation(rotationInfo* rotation)
{
	this->was_last_clockwise = rotation->clockwise;
	this->routine_rotations.emplace_back(rotation);
	++this->count;
}

void RotationRoutine::removeRotation(rotationInfo* rotation)
{
	auto it = std::find(this->routine_rotations.begin(), this->routine_rotations.end(), rotation);
	if (it != this->routine_rotations.end()) { 
		this->routine_rotations.erase(it); 
		--this->count;
	}
}

void RotationRoutine::clear()
{
	this->routine_rotations.clear();
	this->current = 0;
	this->count = 0;
	this->is_active = false;
	this->is_paused = false;
	this->is_looping = false;
}

void RotationRoutine::reset()
{
	for (auto& rotation : this->routine_rotations)
		rotation->reset();

	this->current = 0;
	this->is_active = false;
	this->is_paused = false;
}

const bool RotationRoutine::start(sf::Shape* shape)
{
	if (this->routine_rotations.size() != 0) {
		if (this->adjust_all_to_current_transform)		  { this->adjustAllToCurrent(shape->getRotation()); }
		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(shape->getRotation()); }
		this->reset();

		this->current = 0;
		this->is_active = true;
		this->movementRoutineEngine->addRotation(shape, this);
		return true;
	}

	printf("RotationRoutine::start: Routine is empty\n");
	return false;
}

const bool RotationRoutine::start(sf::Sprite* sprite)
{
	if (this->routine_rotations.size() != 0) {
		if (this->adjust_all_to_current_transform)		  { this->adjustAllToCurrent(sprite->getRotation()); }
		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(sprite->getRotation()); }
		this->reset();

		this->current = 0;
		this->is_active = true;
		this->movementRoutineEngine->addRotation(sprite, this);
		return true;
	}

	printf("RotationRoutine::start: Routine is empty\n");
	return false;
}

const bool RotationRoutine::start(VertexArray2* vertexArray)
{
	if (this->routine_rotations.size() != 0) {
		if (this->adjust_all_to_current_transform)		  { this->adjustAllToCurrent(vertexArray->getRotation()); }
		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(vertexArray->getRotation()); }
		this->reset();

		this->current = 0;
		this->is_active = true;
		this->movementRoutineEngine->addRotation(vertexArray, this);
		return true;
	}

	printf("RotationRoutine::start: Routine is empty\n");
	return false;
}

void RotationRoutine::stop(sf::Shape* shape)
{
	this->reset();
	this->movementRoutineEngine->stopRotation(shape);
}

void RotationRoutine::stop(sf::Sprite* sprite)
{
	this->reset();
	this->movementRoutineEngine->stopRotation(sprite);
}

void RotationRoutine::stop(VertexArray2* vertexArray)
{
	this->reset();
	this->movementRoutineEngine->stopRotation(vertexArray);
}

rotationInfo* RotationRoutine::getCurrentRotation()
{
	if (this->current < this->count)
		return this->routine_rotations[this->current];
	else {
		printf("RotationRoutine::getCurrentRotation: Current rotation index out of range\n");
		return nullptr;
	}
}

const bool RotationRoutine::goToNextRotation(sf::Shape* shape)
{
	if (this->current < this->count - 1) {
		this->was_last_clockwise = this->routine_rotations[this->current]->clockwise;
		++this->current;
		return true;
	}
	else {
		this->was_last_clockwise = this->routine_rotations[this->current]->clockwise;
		if (this->adjust_all_to_current_transform) { this->adjustAllToCurrent(shape->getRotation()); }
		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(shape->getRotation()); }
		this->reset();

		if (this->is_looping) {
			this->is_active = true;
			return true;
		}

		return false;
	}
}

const bool RotationRoutine::goToNextRotation(sf::Sprite* sprite)
{
	if (this->current < this->count - 1) {
		this->was_last_clockwise = this->routine_rotations[this->current]->clockwise;
		++this->current;
		return true;
	}
	else {
		this->was_last_clockwise = this->routine_rotations[this->current]->clockwise;
		if (this->adjust_all_to_current_transform) { this->adjustAllToCurrent(sprite->getRotation()); }
		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(sprite->getRotation()); }
		this->reset();

		if (this->is_looping) {
			this->is_active = true;
			return true;
		}

		return false;
	}
}

const bool RotationRoutine::goToNextRotation(VertexArray2* vertexArray)
{
	if (this->current < this->count - 1) {
		this->was_last_clockwise = this->routine_rotations[this->current]->clockwise;
		++this->current;
		return true;
	}
	else {
		this->was_last_clockwise = this->routine_rotations[this->current]->clockwise;
		if (this->adjust_all_to_current_transform)		  { this->adjustAllToCurrent(vertexArray->getRotation()); }
		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(vertexArray->getRotation()); }
		this->reset();

		if (this->is_looping) {
			this->is_active = true;
			return true;
		}

		return false;
	}
}

//// - - - - - - - - - - - - - - - - - - - - RotationVARoutine - - - - - - - - - - - - - - - - - - - - \\
//
//void RotationRoutineVA::adjustVertexarrayToStartingRotation(sf::VertexArray* vertexarray)
//{
//	const rotationInfoVA& current_rotation = *this->getCurrentRotation();
//
//	float sine = static_cast<float>(sin(static_cast<double>(current_rotation.current_rotation) * M_RAD)),
//		cosine = static_cast<float>(cos(static_cast<double>(current_rotation.current_rotation) * M_RAD));
//
//	for (size_t i = 0; i < vertexarray->getVertexCount(); ++i) {
//		float x = current_rotation.originalVertex.operator[](i).position.x - current_rotation.centroid.x;
//		float y = current_rotation.originalVertex.operator[](i).position.y - current_rotation.centroid.y;
//		vertexarray->operator[](i).position.x = current_rotation.centroid.x + x * cosine - y * sine;
//		vertexarray->operator[](i).position.y = current_rotation.centroid.y + x * sine + y * cosine;
//	}
//}
//
//void RotationRoutineVA::adjustStartToCurrent()
//{
//	if (this->routine_rotations.size() == 0) return;
//
//	rotationInfoVA* rotation = this->routine_rotations[0];
//
//	float current_rotation_temp = 0.f;
//	if (this->current_rotation != nullptr) 
//		current_rotation_temp = *this->current_rotation;
//	else {
//		if (!rotation->clockwise) {
//			this->routine_rotations[0]->starting_rotation = 360.f - current_rotation_temp;
//			this->routine_rotations[0]->current_rotation = 360.f - current_rotation_temp;
//			this->current_rotation = &this->routine_rotations[0]->current_rotation;
//		}
//	}
//
//	float new_starting_rotation = current_rotation_temp;
//
//	printf("\nVertexArray: \n");
//	printf("Current rotation: %f\n", current_rotation_temp);
//	printf("Was last clockwise: %d\n", this->was_last_clockwise);
//	printf("Is current clockwise: %d\n", rotation->clockwise);
//
//	if (!this->was_last_clockwise) {
//		if (rotation->clockwise) {
//			if (current_rotation_temp > rotation->ending_rotation)
//				new_starting_rotation -= 360.f;
//		}
//		else {
//			new_starting_rotation -= 360.f;
//			new_starting_rotation *= -1.f;
//
//			if (new_starting_rotation > rotation->ending_rotation)
//				new_starting_rotation -= 360.f;
//		}
//	}
//	else {
//		if (rotation->clockwise) {
//			if (current_rotation_temp > rotation->ending_rotation)
//				new_starting_rotation -= 360.f;
//		}
//		else if (rotation->clockwise == false) {
//			new_starting_rotation -= 360.f;
//			new_starting_rotation *= -1.f;
//
//			if (new_starting_rotation > rotation->ending_rotation)
//				new_starting_rotation -= 360.f;
//		}
//	}
//
//	printf("New starting rotation: %f\n", new_starting_rotation);
//	printf("Ending rotation: %f\n\n", rotation->ending_rotation);
//	//new_starting_rotation = fmod(new_starting_rotation, 360.f);
//
//	if (rotation->clockwise) {
//		this->routine_rotations[0]->starting_rotation = new_starting_rotation;
//		this->routine_rotations[0]->current_rotation = new_starting_rotation;
//	}
//	else {
//		this->routine_rotations[0]->starting_rotation = 360.f - new_starting_rotation;
//		this->routine_rotations[0]->current_rotation = 360.f - new_starting_rotation;
//	}
//
//	this->current_rotation = &this->routine_rotations[0]->current_rotation;
//}
//
//void RotationRoutineVA::adjustAllToCurrent()
//{
//	if (this->routine_rotations.size() == 0) return;
//
//	float rotation_offset{};
//
//	if (this->current_rotation != nullptr) 
//		rotation_offset = *this->current_rotation - this->routine_rotations[0]->starting_rotation;
//	else 
//		rotation_offset =  -this->routine_rotations[0]->starting_rotation;
//
//	if (!this->routine_rotations[0]->clockwise)
//		rotation_offset = 360.f + rotation_offset;
//
//	if (rotation_offset == 0.f || rotation_offset == 360.f) return;
//	
//	for (auto& rotation : this->routine_rotations) {
//		float& new_starting_rotation = rotation->starting_rotation;
//		float& new_current_rotation = rotation->current_rotation;
//		float& new_ending_rotation = rotation->ending_rotation;
//
//		new_starting_rotation += rotation_offset;
//		new_current_rotation  += rotation_offset;
//		new_ending_rotation   += rotation_offset;
//
//		if (new_starting_rotation > 360.f || new_ending_rotation > 360.f) {
//			new_starting_rotation -= 360.f;
//			new_current_rotation  -= 360.f;
//			new_ending_rotation   -= 360.f;
//		}
//	}
//
//	this->current_rotation = &this->routine_rotations[0]->current_rotation;
//}
//
//void RotationRoutineVA::addRotation(rotationInfoVA* rotation)
//{
//	this->was_last_clockwise = rotation->clockwise;
//	this->routine_rotations.emplace_back(rotation);
//	++this->count;
//}
//
//void RotationRoutineVA::removeRotation(rotationInfoVA* rotation)
//{
//	auto it = std::find(this->routine_rotations.begin(), this->routine_rotations.end(), rotation);
//	if (it != this->routine_rotations.end()) { 
//		this->routine_rotations.erase(it);
//		--this->count;
//	}
//}
//
//void RotationRoutineVA::clear()
//{
//	this->routine_rotations.clear();
//	this->current = 0;
//	this->count = 0;
//	this->is_active = false;
//	this->is_paused = false;
//	this->is_looping = false;
//}
//
//void RotationRoutineVA::reset(const sf::VertexArray* vertexArray)
//{
//	for (auto& rotation : this->routine_rotations) {
//		rotation->reset();
//		rotation->recalculateCentroidsOriginalVertex(vertexArray);
//	}
//
//	this->current = 0;
//	this->is_active = false;
//	this->is_paused = false;
//}
//
//const bool RotationRoutineVA::start(sf::VertexArray* vertexArray)
//{
//	if (this->routine_rotations.size() != 0) {
//		//this->updateScalingInfoVA(*vertexArray);
//		if (this->adjust_all_to_current_transform)		  { this->adjustAllToCurrent(); }
//		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(); }
//		this->current_rotation = &this->routine_rotations[0]->current_rotation;
//
//		this->reset(vertexArray);
//		//this->adjustVertexarrayToStartingRotation(vertexArray);
//
//		this->current = 0;
//		this->is_active = true;
//		this->movementRoutineEngine->addRotation(vertexArray, this);
//		return true;
//	}
//
//	printf("RotationRoutineVA::start: Routine is empty\n");
//	return false;
//}
//
//void RotationRoutineVA::stop(sf::VertexArray* vertexArray)
//{
//	this->reset(vertexArray);
//	this->movementRoutineEngine->stopRotation(vertexArray);
//}
//
//rotationInfoVA* RotationRoutineVA::getCurrentRotation()
//{
//	if (this->current < this->count)
//		return this->routine_rotations[this->current];
//	else {
//		printf("RotationRoutineVA::getCurrentRotation: Current rotation index out of range\n");
//		return nullptr;
//	}
//}
//
//const bool RotationRoutineVA::goToNextRotation(sf::VertexArray* vertexArray)
//{
//	if (this->current < this->count - 1) {
//		this->was_last_clockwise = this->routine_rotations[this->current]->clockwise;
//		++this->current;
//		this->current_rotation = &this->routine_rotations[this->current]->current_rotation;
//		return true;
//	}
//	else {
//		this->was_last_clockwise = this->routine_rotations[this->current]->clockwise;
//		this->current_rotation = &this->routine_rotations[this->current]->ending_rotation;
//		if (this->adjust_all_to_current_transform) { this->adjustAllToCurrent(); }
//		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(); }
//		
//
//		this->reset(vertexArray);
//		if (this->is_looping) {
//			this->is_active = true;
//			return true;
//		}
//
//		return false;
//	}
//}
//
//void RotationRoutineVA::updateMovementInfoVA(const sf::Vector2f& offset)
//{
//	for (auto& rotation : this->routine_rotations) {
//		rotation->centroid += offset;
//
//		for (size_t i = 0; i < rotation->originalVertex.getVertexCount(); ++i)
//			rotation->originalVertex.operator[](i).position += offset;
//	}
//}
//
//void RotationRoutineVA::updateScalingInfoVA(const sf::VertexArray& vertexArray)
//{
//	for (auto& rotation : this->routine_rotations) 
//		rotation->setOriginalVertex(&vertexArray);
//}
//
//// - - - - - - - - - - - - - - - - - - - - ScalingRoutineVA - - - - - - - - - - - - - - - - - - - - \\
//
//void ScalingRoutineVA::adjustVertexarrayToStartingScale(sf::VertexArray* vertexarray)
//{
//	scalingInfoVA* current_scaling = this->getCurrentScaling();
//	if (current_scaling == nullptr) return;
//
//	for (size_t i = 0; i < vertexarray->getVertexCount(); i++) {
//		vertexarray->operator[](i).position.x = current_scaling->centroid.x + (current_scaling->originalVertex.operator[](i).position.x - current_scaling->centroid.x) * current_scaling->current_scale.x;
//		vertexarray->operator[](i).position.y = current_scaling->centroid.y + (current_scaling->originalVertex.operator[](i).position.y - current_scaling->centroid.y) * current_scaling->current_scale.y;
//	}
//}
//
//void ScalingRoutineVA::adjustStartToCurrent()
//{
//	if (this->routine_scalings.size() == 0) return;
//
//	if (this->current_scale == nullptr) {
//		this->routine_scalings[0]->starting_scale = sf::Vector2f{ 1.f, 1.f };
//		this->routine_scalings[0]->current_scale = sf::Vector2f{ 1.f, 1.f };
//	}
//	else {
//		this->routine_scalings[0]->starting_scale = *this->current_scale;
//		this->routine_scalings[0]->current_scale = *this->current_scale;
//	}
//	
//}
//
//void ScalingRoutineVA::adjustAllToCurrent()
//{
//	if (this->routine_scalings.size() == 0) return;
//
//	sf::Vector2f current_scale_temp{ 1.f, 1.f };
//	if (this->current_scale != nullptr) current_scale_temp = *this->current_scale;
//
//	const sf::Vector2f proportion{ current_scale_temp.x / this->routine_scalings[0]->starting_scale.x, current_scale_temp.y / this->routine_scalings[0]->starting_scale.y };
//	for (auto& scaling : this->routine_scalings) {
//		scaling->starting_scale = sf::Vector2f{ scaling->starting_scale.x * proportion.x, scaling->starting_scale.y * proportion.y };
//		scaling->ending_scale   = sf::Vector2f{ scaling->ending_scale.x * proportion.x, scaling->ending_scale.y * proportion.y };
//		scaling->current_scale  = sf::Vector2f{ scaling->current_scale.x * proportion.x, scaling->current_scale.y * proportion.y };
//	}
//
//	this->current_scale = &this->routine_scalings[0]->current_scale;
//}
//
//void ScalingRoutineVA::addScaling(scalingInfoVA* scaling)
//{
//	this->routine_scalings.emplace_back(scaling);
//	++this->count;
//}
//
//void ScalingRoutineVA::removeScaling(scalingInfoVA* scaling)
//{
//	auto it = std::find(this->routine_scalings.begin(), this->routine_scalings.end(), scaling);
//
//	if (it != this->routine_scalings.end()) {
//		this->routine_scalings.erase(it);
//		--this->count;
//	}
//}
//
//void ScalingRoutineVA::clear()
//{
//	this->routine_scalings.clear();
//	this->current = 0;
//	this->count = 0;
//	this->is_active = false;
//	this->is_paused = false;
//	this->is_looping = false;
//}
//
//void ScalingRoutineVA::reset(const sf::VertexArray* vertexArray)
//{
//	for (auto& scaling : this->routine_scalings) {
//		scaling->reset();
//		scaling->recalculateCentroidsOriginalVertex(vertexArray);
//	}
//	
//	this->current = 0;
//	this->is_active = false;
//	this->is_paused = false;
//}
//
//const bool ScalingRoutineVA::start(sf::VertexArray* vertexArray)
//{
//	if (this->routine_scalings.size() != 0) {
//		if (this->adjust_all_to_current_transform)		  { this->adjustAllToCurrent(); }
//		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(); }
//		this->current_scale = &this->routine_scalings[0]->current_scale;
//
//		this->reset(vertexArray);
//
//		//this->adjustVertexarrayToStartingScale(vertexArray);
//
//		this->current = 0;
//		this->is_active = true;
//		this->movementRoutineEngine->addScaling(vertexArray, this);
//		return true;
//	}
//
//	printf("ScalingRoutineVA::start: Routine is empty\n");
//	return false;
//}
//
//void ScalingRoutineVA::stop(sf::VertexArray* vertexArray)
//{
//	this->reset(vertexArray);
//	this->movementRoutineEngine->stopScaling(vertexArray);
//}
//
//scalingInfoVA* ScalingRoutineVA::getCurrentScaling()
//{
//	if (this->current < this->count)
//		return this->routine_scalings[this->current];
//	else {
//		printf("ScalingRoutineVA::getCurrentScaling: Current scaling index out of range\n");
//		return nullptr;
//	}
//}
//
//const bool ScalingRoutineVA::goToNextScaling(sf::VertexArray* vertexArray)
//{
//	if (this->current < this->count - 1) {
//		++this->current;
//		this->current_scale = &this->routine_scalings[this->current]->current_scale;
//		return true;
//	}
//	else {
//		this->current_scale = &this->routine_scalings[this->current]->ending_scale;
//		if (this->adjust_all_to_current_transform)		  { this->adjustAllToCurrent(); }
//		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(); }
//
//		this->reset(vertexArray);
//
//		if (this->is_looping) {
//			this->is_active = true;
//			return true;
//		}
//
//		return false;
//	}
//}
//
//void ScalingRoutineVA::updateMovementInfoVA(const sf::Vector2f& offset)
//{
//	for (auto& scaling : this->routine_scalings) {
//		scaling->centroid += offset;
//		
//		for (size_t i = 0; i < scaling->originalVertex.getVertexCount(); ++i) 
//			scaling->originalVertex.operator[](i).position += offset;
//	}
//}
//
//void ScalingRoutineVA::updateRotationInfoVA(const sf::VertexArray& vertexArray)
//{
//	for (auto& scaling : this->routine_scalings) 
//		scaling->setOriginalVertex(&vertexArray);
//}
//
//
//// - - - - - - - - - - - - - - - - - - - - MovementRoutineVA - - - - - - - - - - - - - - - - - - - - \\
//
//void MovementRoutineVA::adjustStartToCurrent(sf::VertexArray* vertexArray)
//{
//	if (this->routine_movements.size() == 0) return;
//
//	sf::Vector2f centroid{};
//	for (size_t i = 0; i < vertexArray->getVertexCount(); ++i)
//		centroid += vertexArray->operator[](i).position;
//	centroid /= static_cast<float>(vertexArray->getVertexCount());
//
//	this->routine_movements[0]->starting_pos = centroid;
//	this->routine_movements[0]->moveCentroidsToStartingPosition();
//}
//
//void MovementRoutineVA::adjustAllToCurrent(sf::VertexArray* vertexArray)
//{
//	if (this->routine_movements.size() == 0) return;
//
//	sf::Vector2f centroid{};
//	for (size_t i = 0; i < vertexArray->getVertexCount(); ++i)
//		centroid += vertexArray->operator[](i).position;
//	centroid /= static_cast<float>(vertexArray->getVertexCount());
//
//	const sf::Vector2f offset = this->routine_movements[0]->starting_pos - centroid;
//	for (auto& movement : this->routine_movements) {
//		movement->starting_pos -= offset;
//		movement->ending_pos -= offset;
//	}
//}
//
//void MovementRoutineVA::adjustVertexarrayToStartingPosition(sf::VertexArray* vertexarray, ScalingRoutineVA* scalingRoutine)
//{
//	movementInfoVA* current_movement = this->getCurrentMovement();
//	if (current_movement == nullptr) return;
//
//	sf::Vector2f centroid{};
//	for (size_t i = 0; i < vertexarray->getVertexCount(); ++i)
//		centroid += vertexarray->operator[](i).position;
//	centroid /= static_cast<float>(vertexarray->getVertexCount());
//
//	sf::Vector2f offset = current_movement->starting_pos - centroid;
//
//
//	if (scalingRoutine != nullptr) {
//		scalingRoutine->updateMovementInfoVA(offset);
//	}
//	else {
//		for (size_t i = 0; i < vertexarray->getVertexCount(); ++i)
//			vertexarray->operator[](i).position += offset;
//	}
//}
//
//void MovementRoutineVA::addMovement(movementInfoVA* movement)
//{
//	this->routine_movements.emplace_back(movement);
//	++this->count;
//}
//
//void MovementRoutineVA::removeMovement(movementInfoVA* movement)
//{
//	auto it = std::find(this->routine_movements.begin(), this->routine_movements.end(), movement);
//	if (it != this->routine_movements.end()) { this->routine_movements.erase(it); --this->count; }
//}
//
//void MovementRoutineVA::clear()
//{
//	this->routine_movements.clear();
//	this->current = 0;
//	this->count = 0;
//	this->is_active = false;
//	this->is_paused = false;
//	this->is_looping = false;
//}
//
//void MovementRoutineVA::reset()
//{
//	for (auto& movement : this->routine_movements) {
//		movement->reset();
//		movement->moveCentroidsToStartingPosition();
//	}
//
//	this->current = 0;
//	this->is_active = false;
//	this->is_paused = false;
//}
//
//const bool MovementRoutineVA::start(sf::VertexArray* vertexArray)
//{
//	if (this->routine_movements.size() != 0) {
//		if (this->adjust_all_to_current_transform) { this->adjustAllToCurrent(vertexArray); }
//		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(vertexArray); }
//
//		this->reset();
//
//		this->adjustVertexarrayToStartingPosition(vertexArray);
//
//		this->current = 0;
//		this->is_active = true;
//		this->movementRoutineEngine->addMovement(vertexArray, this);
//		return true;
//	}
//
//	printf("MovementRoutineVA::start: Routine is empty\n");
//	return false;
//}
//
//void MovementRoutineVA::stop(sf::VertexArray* vertexArray)
//{
//	this->reset();
//	this->movementRoutineEngine->stopMovement(vertexArray);
//}
//
//movementInfoVA* MovementRoutineVA::getCurrentMovement()
//{
//	if (this->current < this->count)
//		return this->routine_movements[this->current];
//	else {
//		printf("MovementRoutineVA::getCurrentMovement: Current movement index out of range\n");
//		return nullptr;
//	}
//}
//
//const bool MovementRoutineVA::goToNextMovement(sf::VertexArray* vertexArray)
//{
//	if (this->current < this->count - 1) {
//		++this->current;
//		return true;
//	}
//	else {
//		if (this->adjust_all_to_current_transform) { this->adjustAllToCurrent(vertexArray); }
//		else if (this->adjust_start_to_current_transform) { this->adjustStartToCurrent(vertexArray); }
//		this->reset();
//
//		if (this->is_looping) {
//			this->is_active = true;
//			return true;
//		}
//
//		return false;
//	}
//}
//
//const sf::Vector2f MovementRoutineVA::getGoToNextMovementOffset(const sf::VertexArray* vertexArray)
//{
//	movementInfoVA* current_movement = this->getCurrentMovement();
//	if (current_movement == nullptr) return sf::Vector2f{};
//
//	sf::Vector2f centroid{};
//	for (size_t i = 0; i < vertexArray->getVertexCount(); ++i)
//		centroid += vertexArray->operator[](i).position;
//	centroid /= static_cast<float>(vertexArray->getVertexCount());
//
//	const sf::Vector2f offset = current_movement->starting_pos - centroid;
//	return offset;
//}