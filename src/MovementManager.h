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
	std::map<sf::VertexArray*, MovementRoutineVA*> m_Routine_Movement_VertexArray_Active;

	// Contains copies of all linked routines to shapes. Contains COPY of SELECTED routines from movementRoutineContainer
	std::map<sf::Shape*,  MovementRoutineContainer*>  m_Routine_Movement_Shape;
	std::map<sf::Sprite*, MovementRoutineContainer*> m_Routine_Movement_Sprite;
	std::map<sf::VertexArray*, MovementRoutineVAContainer*> m_Routine_Movement_VertexArray;

	MovementRoutineContainer* movementRoutineContainer;
	MovementRoutineVAContainer* movementRoutineVAContainer;

public:
	// Movement container
	MovementRoutineEngine* movementRoutineEngine;

	MovementManager();

	MovementRoutine* createMovementRoutine(const std::string& _name) { return this->movementRoutineContainer->createRoutine(_name); }
	MovementRoutineVA* createMovementRoutineVA(const std::string& _name) { return this->movementRoutineVAContainer->createRoutine(_name); }

	MovementRoutine* getMovementRoutine(const std::string& _name)    { return this->movementRoutineContainer->getRoutinePtr(_name); }
	MovementRoutineVA* getMovementRoutineVA(const std::string& _name) { return this->movementRoutineVAContainer->getRoutinePtr(_name); }

	MovementRoutine* linkMovementRoutine(sf::Shape* _shape, const std::string& _name); // Dodaæ mo¿liwoœæ linkowania po wskaŸniku do routine
	MovementRoutine* linkMovementRoutine(sf::Sprite* _sprite, const std::string& _name);
	MovementRoutineVA* linkMovementRoutine(sf::VertexArray* _vertexarray, const std::string& _name);

	void unlinkMovementRoutine(sf::Shape* _shape, const std::string& _name);
	void unlinkMovementRoutine(sf::Sprite* _sprite, const std::string& _name);
	void unlinkMovementRoutine(sf::VertexArray* _vertexarray, const std::string& _name);

	void startMovementRoutine(sf::Shape* _shape, const std::string& _name);
	void startMovementRoutine(sf::Sprite* _sprite, const std::string& _name);
	void startMovementRoutine(sf::VertexArray* _vertexarray, const std::string& _name);

	void pauseMovementRoutine(sf::Shape* _shape, const std::string& _name);
	void pauseMovementRoutine(sf::Sprite* _sprite, const std::string& _name);
	void pauseMovementRoutine(sf::VertexArray* _vertexarray, const std::string& _name);

	void resumeMovementRoutine(sf::Shape* _shape, const std::string& _name);
	void resumeMovementRoutine(sf::Sprite* _sprite, const std::string& _name);
	void resumeMovementRoutine(sf::VertexArray* _vertexarray, const std::string& _name);

	void stopMovementRoutine(sf::Shape* _shape, const std::string& _name);
	void stopMovementRoutine(sf::Sprite* _sprite, const std::string& _name);
	void stopMovementRoutine(sf::VertexArray* _vertexarray, const std::string& _name);

	void deleteMovementRoutine();
	void deleteMovementRoutine(const std::string& _name);

	long long int getSizeShapeActive() {
		long long int size = 0;
		for (auto& routine : m_Routine_Movement_Shape_Active) size += routine.second->size();
		return size + sizeof(m_Routine_Movement_Shape_Active);
	}

	long long int getSizeShape() {
		long long int size = 0;
		for (auto& routineContainer : m_Routine_Movement_Shape) size += routineContainer.second->size();
		return size + sizeof(m_Routine_Movement_Shape);
	}

	long long int getSizeContainer() {
		long long int size = 0;
		size += movementRoutineContainer->size();
		return size + sizeof(movementRoutineContainer);
	}

	void update(float dt);
};