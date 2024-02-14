#pragma once
#include "MovementContainer.h"

struct MovementRoutine {
	std::string routine_name{}; // base
	std::vector <movementInfo*> routine_movements;
	size_t current_movement{}; //base
	size_t movement_count{}; //base
	bool is_active{}; //base
	bool is_looping{}; //base
	bool is_paused{}; //base

	MovementRoutine() {}
	MovementRoutine(std::string name) { this->routine_name = name; }
	MovementRoutine(std::string name, movementInfo* movement) {
		this->routine_name = name;
		this->routine_movements.emplace_back(movement);
	}
	MovementRoutine(std::string name, std::vector<movementInfo*> movements) {
		this->routine_name = name;
		this->routine_movements = movements;
	}
	MovementRoutine(const MovementRoutine& obj) {
		this->routine_name		 = obj.routine_name;
		this->routine_movements	 = obj.routine_movements;
		this->current_movement	 = obj.current_movement;
		this->movement_count	 = obj.movement_count;
		this->is_active			 = obj.is_active;
		this->is_looping		 = obj.is_looping;
		this->is_paused			 = obj.is_paused;
	}
	~MovementRoutine() { routine_movements.clear(); }
};

class MovementManager {
private:
	// Singleton instance
	static MovementManager* sInstance;

	MovementContainer* movementContainer;

	std::map<sf::Shape*, std::vector<MovementRoutine*>> m_Routine_Movement;

public:
	MovementManager();

	const MovementRoutine* createMovementRoutine(const sf::Shape* _shape, const std::string& _name);

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