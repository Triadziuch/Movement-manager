#pragma once
#include "MovementContainer.h"



struct TransformationRoutine {
public:
	std::string	 routine_name{};
	size_t		 current{};
	size_t		 count{};
	bool		 is_active{};
	bool		 is_looping{};
	bool		 is_paused{};

	TransformationRoutine() {}
	TransformationRoutine(std::string name) { this->routine_name = name; }
	TransformationRoutine(const TransformationRoutine& obj) : 
		routine_name(obj.routine_name), current(obj.current), count(obj.count), is_active(obj.is_active), is_looping(obj.is_looping), is_paused(obj.is_paused) {}
};

struct MovementRoutine : public TransformationRoutine{
	std::vector <movementInfo*> routine_movements;

	MovementRoutine() {}
	MovementRoutine(std::string name) { this->routine_name = name; }
	MovementRoutine(std::string name, movementInfo* movement) : TransformationRoutine{ name } { this->routine_movements.emplace_back(movement); }
	MovementRoutine(std::string name, std::vector<movementInfo*> movements) : TransformationRoutine{ name } { this->routine_movements = movements; }
	MovementRoutine(const MovementRoutine& obj) : TransformationRoutine{ obj }, routine_movements(obj.routine_movements) {}
	~MovementRoutine() { routine_movements.clear(); }

	void addMovement(movementInfo* movement) { this->routine_movements.emplace_back(movement); }
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