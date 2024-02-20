#pragma once
#include "MovementContainer.h"

class MovementManager {
private:
	// Singleton instance
	static MovementManager* sInstance;

	

	std::map<sf::Shape*,  MovementRoutine*> m_Routine_Movement_Shape_Active;
	std::map<sf::Sprite*, MovementRoutine*> m_Routine_Movement_Sprite_Active;

	std::map<sf::Shape*,  MovementRoutineContainer*>  m_Routine_Movement_Shape;
	std::map<sf::Sprite*, MovementRoutineContainer*> m_Routine_Movement_Sprite;

	MovementRoutineContainer* movementRoutineContainer;

public:
	// Movement container
	MovementRoutineEngine* movementRoutineEngine;

	MovementManager();

	MovementRoutine* createMovementRoutine(const std::string& _name) { return this->movementRoutineContainer->createRoutine(_name); }
	MovementRoutine* getMovementRoutine(const std::string& _name)    { return this->movementRoutineContainer->getRoutinePtr(_name); }

	MovementRoutine* linkMovementRoutine(sf::Shape* _shape, const std::string& _name); // Dodaæ mo¿liwoœæ linkowania po wskaŸniku do routine
	MovementRoutine* linkMovementRoutine(sf::Sprite* _sprite, const std::string& _name);

	void startMovementRoutine(sf::Shape* _shape, const std::string& _name);
	void startMovementRoutine(sf::Sprite* _sprite, const std::string& _name);

	void pauseMovementRoutine(sf::Shape* _shape, const std::string& _name);
	void pauseMovementRoutine(sf::Sprite* _sprite, const std::string& _name);

	void resumeMovementRoutine(sf::Shape* _shape, const std::string& _name);
	void resumeMovementRoutine(sf::Sprite* _sprite, const std::string& _name);

	void update(float dt);
	void updateShape();

	std::map<sf::Shape*, movementInfo*>			    m_Movements_Shape;
	std::map<sf::VertexArray*, movementInfoVA*>		m_Movements_VA;
	std::map<sf::Sprite*, movementInfo*>			m_Movements_S;

	std::map<sf::Shape*, scalingInfo*>				m_Scalings_Shape;
	std::map<sf::VertexArray*, scalingInfoVA*>		m_Scalings_VA;
	std::map<sf::Sprite*, scalingInfo*>				m_Scalings_S;

	std::map<sf::Shape*, rotationInfo*>				m_Rotations_Shape;
	std::map<sf::VertexArray*, rotationInfoVA*>		m_Rotations_VA;
	std::map<sf::Sprite*, rotationInfo*>			m_Rotations_S;
};