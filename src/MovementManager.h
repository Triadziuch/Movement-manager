#pragma once
#include "MovementContainer.h"
#include "RoutineContainer.h"

class MovementManager {
private:
	// Singleton instance
	static MovementManager* sInstance;

	
	// Contains active routines linked to shapes
	std::map<sf::Shape*,  MovementRoutine*> m_routineMovementShapeActive;
	std::map<sf::Sprite*, MovementRoutine*> m_routineMovementSpriteActive;
	std::map<VertexArray2*, MovementRoutine*> m_routineMovementVertexArrayActive;

	// Contains copies of all linked routines to shapes. Contains COPY of LINKED routines from m_movementRoutineContainer
	std::map<sf::Shape*,  MovementRoutineContainer*>  m_routineMovementShape;
	std::map<sf::Sprite*, MovementRoutineContainer*> m_routineMovementSprite;
	std::map<VertexArray2*, MovementRoutineContainer*> m_routineMovementVertexArray;

	MovementRoutineContainer* m_movementRoutineContainer;



	std::map<sf::Shape*, ScalingRoutine*> m_routineScalingShape_Active;
	std::map<sf::Sprite*, ScalingRoutine*> m_routineScalingSprite_Active;
	std::map<VertexArray2*, ScalingRoutine*> m_routineScalingVertexArray_Active;

	std::map<sf::Shape*, ScalingRoutineContainer*>  m_routineScalingShape;
	std::map<sf::Sprite*, ScalingRoutineContainer*> m_routineScalingSprite;
	std::map<VertexArray2*, ScalingRoutineContainer*> m_routineScalingVertexArray;

	ScalingRoutineContainer* m_scalingRoutineContainer;



	std::map<sf::Shape*, RotationRoutine*> m_routineRotationShape_Active;
	std::map<sf::Sprite*, RotationRoutine*> m_routineRotationSprite_Active;
	std::map<VertexArray2*, RotationRoutine*> m_routineRotationVertexArray_Active;

	std::map<sf::Shape*, RotationRoutineContainer*>  m_routineRotationShape;
	std::map<sf::Sprite*, RotationRoutineContainer*> m_routineRotationSprite;
	std::map<VertexArray2*, RotationRoutineContainer*> m_routineRotationVertexArray;

	RotationRoutineContainer* m_rotationRoutineContainer;

	// Constructors / Destructors
	MovementManager();
	~MovementManager();

public:
	MovementManager(const MovementManager&) = delete;
	MovementManager& operator=(const MovementManager&) = delete;

	static MovementManager* getInstance();

	// Movement container
	MovementRoutineEngine* movementRoutineEngine;

	// Update functions
	sf::Vector3i update(const float dt);

	// Universal public functions
	void setFunction(sf::Shape& _shape, easeFunctions::Tmovement_function usedFunctionType);
	void setFunction(sf::Sprite& _sprite, easeFunctions::Tmovement_function usedFunctionType);
	void setFunction(VertexArray2& _vertexarray, easeFunctions::Tmovement_function usedFunctionType);

	void resetRoutines(sf::Shape& _shape);
	void resetRoutines(sf::Sprite& _sprite);
	void resetRoutines(VertexArray2& _vertexarray);

	// === Movement ===
	MovementRoutine* createMovementRoutine(const std::string& _name);

	MovementRoutine* getMovementRoutine(const std::string& _name);
	MovementRoutine* getMovementRoutine(sf::Shape& _shape, const std::string& _name);
	MovementRoutine* getMovementRoutine(sf::Sprite& _sprite, const std::string& _name);
	MovementRoutine* getMovementRoutine(VertexArray2& _vertexarray, const std::string& _name);

	MovementRoutine* linkMovementRoutine(sf::Shape& _shape, const std::string& _name);
	MovementRoutine* linkMovementRoutine(sf::Sprite& _sprite, const std::string& _name);
	MovementRoutine* linkMovementRoutine(VertexArray2& _vertexarray, const std::string& _name);

	MovementRoutine* linkMovementRoutine(sf::Shape* _shape, MovementRoutine* routine);
	MovementRoutine* linkMovementRoutine(sf::Sprite* _sprite, MovementRoutine* routine);
	MovementRoutine* linkMovementRoutine(VertexArray2* _vertexarray, MovementRoutine* routine);

	void unlinkMovementRoutine(sf::Shape* _shape, const std::string& _name);
	void unlinkMovementRoutine(sf::Sprite* _sprite, const std::string& _name);
	void unlinkMovementRoutine(VertexArray2* _vertexarray, const std::string& _name);

	void startMovementRoutine(sf::Shape& _shape, const std::string& _name);
	void startMovementRoutine(sf::Sprite& _sprite, const std::string& _name);
	void startMovementRoutine(VertexArray2& _vertexarray, const std::string& _name);



	void pauseMovementRoutine(sf::Shape& _shape, const std::string& _name);
	void pauseMovementRoutine(sf::Sprite& _sprite, const std::string& _name);
	void pauseMovementRoutine(VertexArray2& _vertexarray, const std::string& _name);

	void resumeMovementRoutine(sf::Shape& _shape, const std::string& _name);
	void resumeMovementRoutine(sf::Sprite& _sprite, const std::string& _name);
	void resumeMovementRoutine(VertexArray2& _vertexarray, const std::string& _name);

	void resetMovementRoutine(sf::Shape& _shape, const std::string& _name);
	void resetMovementRoutine(sf::Sprite& _sprite, const std::string& _name);
	void resetMovementRoutine(VertexArray2& _vertexarray, const std::string& _name);

	void stopMovementRoutine(sf::Shape* _shape, const std::string& _name);
	void stopMovementRoutine(sf::Sprite* _sprite, const std::string& _name);
	void stopMovementRoutine(VertexArray2* _vertexarray, const std::string& _name);

	void deleteMovementRoutine();
	void deleteMovementRoutine(const std::string& _name);

	const long long int getSizeMovement() const;

	// === Scaling ===
	ScalingRoutine* createScalingRoutine(const std::string& _name);

	ScalingRoutine* getScalingRoutine(const std::string& _name);
	ScalingRoutine* getScalingRoutine(sf::Shape& _shape, const std::string& _name);
	ScalingRoutine* getScalingRoutine(sf::Sprite& _sprite, const std::string& _name);
	ScalingRoutine* getScalingRoutine(VertexArray2& _vertexarray, const std::string& _name);

	ScalingRoutine* linkScalingRoutine(sf::Shape& _shape, const std::string& _name);
	ScalingRoutine* linkScalingRoutine(sf::Sprite& _sprite, const std::string& _name);
	ScalingRoutine* linkScalingRoutine(VertexArray2& _vertexarray, const std::string& _name);

	ScalingRoutine* linkScalingRoutine(sf::Shape* _shape, ScalingRoutine* routine);
	ScalingRoutine* linkScalingRoutine(sf::Sprite* _sprite, ScalingRoutine* routine);
	ScalingRoutine* linkScalingRoutine(VertexArray2* _vertexarray, ScalingRoutine* routine);

	void unlinkScalingRoutine(sf::Shape* _shape, const std::string& _name);
	void unlinkScalingRoutine(sf::Sprite* _sprite, const std::string& _name);
	void unlinkScalingRoutine(VertexArray2* _vertexarray, const std::string& _name);

	void startScalingRoutine(sf::Shape& _shape, const std::string& _name);
	void startScalingRoutine(sf::Sprite& _sprite, const std::string& _name);
	void startScalingRoutine(VertexArray2& _vertexarray, const std::string& _name);

	void pauseScalingRoutine(sf::Shape& _shape, const std::string& _name);
	void pauseScalingRoutine(sf::Sprite& _sprite, const std::string& _name);
	void pauseScalingRoutine(VertexArray2& _vertexarray, const std::string& _name);

	void resumeScalingRoutine(sf::Shape& _shape, const std::string& _name);
	void resumeScalingRoutine(sf::Sprite& _sprite, const std::string& _name);
	void resumeScalingRoutine(VertexArray2& _vertexarray, const std::string& _name);

	void resetScalingRoutine(sf::Shape& _shape, const std::string& _name);
	void resetScalingRoutine(sf::Sprite& _sprite, const std::string& _name);
	void resetScalingRoutine(VertexArray2& _vertexarray, const std::string& _name);

	void stopScalingRoutine(sf::Shape* _shape, const std::string& _name);
	void stopScalingRoutine(sf::Sprite* _sprite, const std::string& _name);
	void stopScalingRoutine(VertexArray2* _vertexarray, const std::string& _name);

	void deleteScalingRoutine();
	void deleteScalingRoutine(const std::string& _name);

	const long long int getSizeScaling() const;

	// === Rotation ===
	RotationRoutine* createRotationRoutine(const std::string& _name);

	RotationRoutine* getRotationRoutine(const std::string& _name);
	RotationRoutine* getRotationRoutine(sf::Shape& _shape, const std::string& _name);
	RotationRoutine* getRotationRoutine(sf::Sprite& _sprite, const std::string& _name);
	RotationRoutine* getRotationRoutine(VertexArray2& _vertexarray, const std::string& _name);

	RotationRoutine* linkRotationRoutine(sf::Shape& _shape, const std::string& _name); 
	RotationRoutine* linkRotationRoutine(sf::Sprite& _sprite, const std::string& _name);
	RotationRoutine* linkRotationRoutine(VertexArray2& _vertexarray, const std::string& _name);

	RotationRoutine* linkRotationRoutine(sf::Shape* _shape, RotationRoutine* routine);
	RotationRoutine* linkRotationRoutine(sf::Sprite* _sprite, RotationRoutine* routine);
	RotationRoutine* linkRotationRoutine(VertexArray2* _vertexarray, RotationRoutine* routine);

	void unlinkRotationRoutine(sf::Shape* _shape, const std::string& _name);
	void unlinkRotationRoutine(sf::Sprite* _sprite, const std::string& _name);
	void unlinkRotationRoutine(VertexArray2* _vertexarray, const std::string& _name);

	void startRotationRoutine(sf::Shape& _shape, const std::string& _name);
	void startRotationRoutine(sf::Sprite& _sprite, const std::string& _name);
	void startRotationRoutine(VertexArray2& _vertexarray, const std::string& _name);

	void pauseRotationRoutine(sf::Shape& _shape, const std::string& _name);
	void pauseRotationRoutine(sf::Sprite& _sprite, const std::string& _name);
	void pauseRotationRoutine(VertexArray2& _vertexarray, const std::string& _name);

	void resumeRotationRoutine(sf::Shape& _shape, const std::string& _name);
	void resumeRotationRoutine(sf::Sprite& _sprite, const std::string& _name);
	void resumeRotationRoutine(VertexArray2& _vertexarray, const std::string& _name);

	void resetRotationRoutine(sf::Shape& _shape, const std::string& _name);
	void resetRotationRoutine(sf::Sprite& _sprite, const std::string& _name);
	void resetRotationRoutine(VertexArray2& _vertexarray, const std::string& _name);

	void stopRotationRoutine(sf::Shape* _shape, const std::string& _name);
	void stopRotationRoutine(sf::Sprite* _sprite, const std::string& _name);
	void stopRotationRoutine(VertexArray2* _vertexarray, const std::string& _name);

	void deleteRotationRoutine();
	void deleteRotationRoutine(const std::string& _name);

	const long long int getSizeRotation() const;
};