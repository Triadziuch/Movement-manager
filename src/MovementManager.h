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

	void addMovement(movementInfo* movement) { this->routine_movements.emplace_back(movement); ++this->count; }
	void removeMovement(movementInfo* movement) {
		auto it = std::find(this->routine_movements.begin(), this->routine_movements.end(), movement);
		if (it != this->routine_movements.end()) { this->routine_movements.erase(it); --this->count; }
	}

	void start(MovementContainer* movementContainer, sf::Shape* _shape) {
		if (this->routine_movements.size() != 0) {
			this->current = 0;
			this->is_active = true;
			this->is_paused = false;

			movementContainer->addMovement(_shape, this->routine_movements[this->current]);
		}
	}

	void update(MovementContainer* movementContainer, sf::Shape* _shape) {
		if (this->is_active) {
			if (!this->is_paused) {
				if (this->routine_movements[this->current]->isFinished()) {
					if (this->current < this->count - 1) {
						++this->current;
						movementContainer->addMovement(_shape, this->routine_movements[this->current]);
					}
					else {
						if (this->is_looping) {
							this->current = 0;
							movementContainer->addMovement(_shape, this->routine_movements[this->current]);
						}
						else {
							this->current = 0;
							this->is_active = false;
						}
					}
				}
			}
		}
	}
};

class MovementManager {
private:
	// Singleton instance
	static MovementManager* sInstance;

	// Movement container
	MovementContainer* movementContainer;

	std::map<sf::Shape*, MovementRoutine*> m_Routine_Movement_Shape_Active;
	std::map<sf::Shape*, std::vector<MovementRoutine*>> m_Routine_Movement_Shape;

public:
	MovementManager();

	const MovementRoutine* createMovementRoutine(sf::Shape* _shape, const std::string& _name);

	void update(float dt);

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