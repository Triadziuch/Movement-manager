#pragma once
#include "MovementContainer.h"
#include "RoutineContainer.h"

class MovementManager {
private:
	// Singleton instance
	static MovementManager* sInstance;

	
	// Contains active routines linked to shapes
	std::map<sf::Shape*,  MovementRoutine*> m_Routine_Movement_Shape_Active;
	std::map<sf::Sprite*, MovementRoutine*> m_Routine_Movement_Sprite_Active;
	std::map<VertexArray2*, MovementRoutine*> m_Routine_Movement_VertexArray_Active;

	// Contains copies of all linked routines to shapes. Contains COPY of LINKED routines from movementRoutineContainer
	std::map<sf::Shape*,  MovementRoutineContainer*>  m_Routine_Movement_Shape;
	std::map<sf::Sprite*, MovementRoutineContainer*> m_Routine_Movement_Sprite;
	std::map<VertexArray2*, MovementRoutineContainer*> m_Routine_Movement_VertexArray;

	MovementRoutineContainer* movementRoutineContainer;



	std::map<sf::Shape*, ScalingRoutine*> m_Routine_Scaling_Shape_Active;
	std::map<sf::Sprite*, ScalingRoutine*> m_Routine_Scaling_Sprite_Active;
	std::map<VertexArray2*, ScalingRoutine*> m_Routine_Scaling_VertexArray_Active;

	std::map<sf::Shape*, ScalingRoutineContainer*>  m_Routine_Scaling_Shape;
	std::map<sf::Sprite*, ScalingRoutineContainer*> m_Routine_Scaling_Sprite;
	std::map<VertexArray2*, ScalingRoutineContainer*> m_Routine_Scaling_VertexArray;

	ScalingRoutineContainer* scalingRoutineContainer;



	std::map<sf::Shape*, RotationRoutine*> m_Routine_Rotation_Shape_Active;
	std::map<sf::Sprite*, RotationRoutine*> m_Routine_Rotation_Sprite_Active;
	std::map<VertexArray2*, RotationRoutine*> m_Routine_Rotation_VertexArray_Active;

	std::map<sf::Shape*, RotationRoutineContainer*>  m_Routine_Rotation_Shape;
	std::map<sf::Sprite*, RotationRoutineContainer*> m_Routine_Rotation_Sprite;
	std::map<VertexArray2*, RotationRoutineContainer*> m_Routine_Rotation_VertexArray;

	RotationRoutineContainer* rotationRoutineContainer;

public:
	// Movement container
	MovementRoutineEngine* movementRoutineEngine;

	// Constructors / Destructors
	MovementManager();
	~MovementManager();

	// Update functions
	void update(const float dt);

	// === Movement ===
	MovementRoutine* createMovementRoutine(const std::string& _name);

	MovementRoutine* getMovementRoutine(const std::string& _name);

	MovementRoutine* linkMovementRoutine(sf::Shape& _shape, const std::string& _name); // Dodaæ mo¿liwoœæ linkowania po wskaŸniku do routine
	MovementRoutine* linkMovementRoutine(sf::Sprite& _sprite, const std::string& _name);
	MovementRoutine* linkMovementRoutine(VertexArray2& _vertexarray, const std::string& _name);

	void unlinkMovementRoutine(sf::Shape& _shape, const std::string& _name);
	void unlinkMovementRoutine(sf::Sprite& _sprite, const std::string& _name);
	void unlinkMovementRoutine(VertexArray2& _vertexarray, const std::string& _name);

	void startMovementRoutine(sf::Shape& _shape, const std::string& _name);
	void startMovementRoutine(sf::Sprite& _sprite, const std::string& _name);
	void startMovementRoutine(VertexArray2& _vertexarray, const std::string& _name);

	void pauseMovementRoutine(sf::Shape& _shape, const std::string& _name);
	void pauseMovementRoutine(sf::Sprite& _sprite, const std::string& _name);
	void pauseMovementRoutine(VertexArray2& _vertexarray, const std::string& _name);

	void resumeMovementRoutine(sf::Shape& _shape, const std::string& _name);
	void resumeMovementRoutine(sf::Sprite& _sprite, const std::string& _name);
	void resumeMovementRoutine(VertexArray2& _vertexarray, const std::string& _name);

	void stopMovementRoutine(sf::Shape& _shape, const std::string& _name);
	void stopMovementRoutine(sf::Sprite& _sprite, const std::string& _name);
	void stopMovementRoutine(VertexArray2& _vertexarray, const std::string& _name);

	void deleteMovementRoutine();
	void deleteMovementRoutine(const std::string& _name);

	const long long int& getSizeMovement() const;

	// === Scaling ===
	ScalingRoutine* createScalingRoutine(const std::string& _name);

	ScalingRoutine* getScalingRoutine(const std::string& _name);

	ScalingRoutine* linkScalingRoutine(sf::Shape& _shape, const std::string& _name); // Dodaæ mo¿liwoœæ linkowania po wskaŸniku do routine
	ScalingRoutine* linkScalingRoutine(sf::Sprite& _sprite, const std::string& _name);
	ScalingRoutine* linkScalingRoutine(VertexArray2& _vertexarray, const std::string& _name);

	void unlinkScalingRoutine(sf::Shape& _shape, const std::string& _name);
	void unlinkScalingRoutine(sf::Sprite& _sprite, const std::string& _name);
	void unlinkScalingRoutine(VertexArray2& _vertexarray, const std::string& _name);

	void startScalingRoutine(sf::Shape& _shape, const std::string& _name);
	void startScalingRoutine(sf::Sprite& _sprite, const std::string& _name);
	void startScalingRoutine(VertexArray2& _vertexarray, const std::string& _name);

	void pauseScalingRoutine(sf::Shape& _shape, const std::string& _name);
	void pauseScalingRoutine(sf::Sprite& _sprite, const std::string& _name);
	void pauseScalingRoutine(VertexArray2& _vertexarray, const std::string& _name);

	void resumeScalingRoutine(sf::Shape& _shape, const std::string& _name);
	void resumeScalingRoutine(sf::Sprite& _sprite, const std::string& _name);
	void resumeScalingRoutine(VertexArray2& _vertexarray, const std::string& _name);

	void stopScalingRoutine(sf::Shape& _shape, const std::string& _name);
	void stopScalingRoutine(sf::Sprite& _sprite, const std::string& _name);
	void stopScalingRoutine(VertexArray2& _vertexarray, const std::string& _name);

	void deleteScalingRoutine();
	void deleteScalingRoutine(const std::string& _name);

	const long long int& getSizeScaling() const;

	// === Rotation ===
	RotationRoutine* createRotationRoutine(const std::string& _name);

	RotationRoutine* getRotationRoutine(const std::string& _name);

	RotationRoutine* linkRotationRoutine(sf::Shape& _shape, const std::string& _name); 
	RotationRoutine* linkRotationRoutine(sf::Sprite& _sprite, const std::string& _name);
	RotationRoutine* linkRotationRoutine(VertexArray2& _vertexarray, const std::string& _name);

	void unlinkRotationRoutine(sf::Shape& _shape, const std::string& _name);
	void unlinkRotationRoutine(sf::Sprite& _sprite, const std::string& _name);
	void unlinkRotationRoutine(VertexArray2& _vertexarray, const std::string& _name);

	void startRotationRoutine(sf::Shape& _shape, const std::string& _name);
	void startRotationRoutine(sf::Sprite& _sprite, const std::string& _name);
	void startRotationRoutine(VertexArray2& _vertexarray, const std::string& _name);

	void pauseRotationRoutine(sf::Shape& _shape, const std::string& _name);
	void pauseRotationRoutine(sf::Sprite& _sprite, const std::string& _name);
	void pauseRotationRoutine(VertexArray2& _vertexarray, const std::string& _name);

	void resumeRotationRoutine(sf::Shape& _shape, const std::string& _name);
	void resumeRotationRoutine(sf::Sprite& _sprite, const std::string& _name);
	void resumeRotationRoutine(VertexArray2& _vertexarray, const std::string& _name);

	void stopRotationRoutine(sf::Shape& _shape, const std::string& _name);
	void stopRotationRoutine(sf::Sprite& _sprite, const std::string& _name);
	void stopRotationRoutine(VertexArray2& _vertexarray, const std::string& _name);

	void deleteRotationRoutine();
	void deleteRotationRoutine(const std::string& _name);

	const long long int& getSizeRotation() const;
};