#pragma once
#include "MovementContainer.h"

struct TransformationRoutine {
public:
	MovementContainer*	movementContainer{};
	std::string			routine_name{};
	size_t				current{};
	size_t				count{};
	bool				is_active{};
	bool				is_looping{};
	bool				is_paused{};

	TransformationRoutine() {}
	TransformationRoutine(std::string name, MovementContainer* _movementContainerPtr) { this->routine_name = name; this->movementContainer = _movementContainerPtr }
	TransformationRoutine(const TransformationRoutine& obj) : 
		movementContainer(obj.movementContainer), routine_name(obj.routine_name), current(obj.current), count(obj.count), is_active(obj.is_active), is_looping(obj.is_looping), is_paused(obj.is_paused) {}

	void setLooping(bool _looping) { this->is_looping = _looping; }
};

struct MovementRoutine : public TransformationRoutine{
	std::vector <movementInfo*> routine_movements;

	MovementRoutine() {}
	MovementRoutine(std::string name, MovementContainer* _movementContainerPtr) : TransformationRoutine{ name, _movementContainerPtr } {};
	MovementRoutine(std::string name, MovementContainer* _movementContainerPtr, movementInfo* movement) : TransformationRoutine{ name, _movementContainerPtr } { this->routine_movements.emplace_back(movement); }
	MovementRoutine(std::string name, MovementContainer* _movementContainerPtr, std::vector<movementInfo*> movements) : TransformationRoutine{ name, _movementContainerPtr } { this->routine_movements = movements; }
	MovementRoutine(const MovementRoutine& obj) : TransformationRoutine{ obj }, routine_movements(obj.routine_movements) {}
	~MovementRoutine() { routine_movements.clear(); }

public:
	// Add movement to routine
	void addMovement(movementInfo* movement) {
		this->routine_movements.emplace_back(movement);
		++this->count;
	}

	// Remove movement from routine
	void removeMovement(movementInfo* movement) {
		auto it = std::find(this->routine_movements.begin(), this->routine_movements.end(), movement);
		if (it != this->routine_movements.end()) { this->routine_movements.erase(it); --this->count; }
	}

	// Clear routine
	void clear() {
		this->routine_movements.clear();
		this->current = 0;
		this->count = 0;
		this->is_active = false;
		this->is_paused = false;
		this->is_looping = false;
	}

	/*void start(sf::Shape* _shape) {
		if (this->routine_movements.size() != 0) {
			this->current = 0;
			this->is_active = true;
			this->is_paused = false;

			movementContainer->addMovement(_shape, this->routine_movements[this->current]);
		}
	}

	void update(sf::Shape* _shape) {
		if (this->is_active) {
			if (!this->is_paused) {
				movementInfo *movement = this->routine_movements[this->current];

				if (movement->isFinished()) {
					movement->reset();

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


				// W tym miejscu wywo³ujemy update dla konkretnego movementInfo
				// Wiêc trzeba dodaæ funkcjê sk³adow¹ update do klasy transformationInfo

				// Dodatkowo trzeba zaimplementowaæ jakiœ pojemnik na TransformationRoutine który bêdzie pozwala³ na sprawdzenie np czy 
				// dany movementInfo o okreœlonej nazwie istnieje czy nie
			}
		}
	}*/
};

struct MovementRoutineContainer {
private:
	std::map<std::string, MovementRoutine*> movementRoutines;
	MovementContainer* movementContainer;

public:
	MovementRoutineContainer() {}
	MovementRoutineContainer(MovementContainer* _movementContainerPtr) { this->movementContainer = _movementContainerPtr; }

	// Check if routine with given name already exists
	MovementRoutine* exists(const std::string& _name) {
		auto movementRoutineFound = movementRoutines.find(_name);

		if (movementRoutineFound != movementRoutines.end())
			return movementRoutineFound->second;
		else
			return nullptr;
	}

	// Get routine value by name
	MovementRoutine getRoutine(const std::string& _name) {
		if (exists(_name) != nullptr)
			return *movementRoutines[_name];
		else {
			printf("MovementRoutineContainer::getRoutine: Routine with given name does not exist\n");
			return MovementRoutine();
		}
	}

	// Get routine pointer by name
	MovementRoutine* getRoutinePtr(const std::string& _name) {
		auto* movementRoutineExists = exists(_name);

		if (movementRoutineExists == nullptr)
			printf("MovementRoutineContainer::getRoutine: Routine with given name does not exist\n");

		return movementRoutineExists;
	}

	// Create routine with given name. If routine with given name already exists, return pointer to it. Else, create new routine.
	MovementRoutine* createRoutine(const std::string& _name) {
		auto* movementRoutineExists = exists(_name);

		if (movementRoutineExists != nullptr) {
			printf("MovementRoutineContainer::createRoutine: Routine with given name already exists 1\n");
			return movementRoutineExists;
		}
		else {
			MovementRoutine* newMovementRoutine = new MovementRoutine(_name, movementContainer);
			movementRoutines.insert(std::make_pair(_name, newMovementRoutine));
			return newMovementRoutine;
		}
	}

	MovementRoutine* createRoutine(const std::string& _name, MovementRoutine* _movement_routine) {
		auto* movementRoutineExists = exists(_name);

		if (movementRoutineExists != nullptr) {
			printf("MovementRoutineContainer::createRoutine: Routine with given name already exists 2\n");
			return movementRoutineExists;
		}
		else {
			movementRoutines.insert(std::make_pair(_name, _movement_routine));
			return _movement_routine;
		}
	}

	// Remove routine with given name
	void removeRoutine(const std::string& _name) {
		auto* movementRoutineExists = exists(_name);

		if (movementRoutineExists != nullptr) 
			movementRoutines.erase(_name);
		else
			printf("MovementRoutineContainer::removeRoutine: Routine with given name does not exist\n");
	}
};

class MovementManager {
private:
	// Singleton instance
	static MovementManager* sInstance;

	// Movement container
	MovementContainer* movementContainer;

	std::map<sf::Shape*, MovementRoutineContainer*>  m_Routine_Movement_Shape_Active;
	std::map<sf::Sprite*, MovementRoutineContainer*> m_Routine_Movement_Sprite_Active;

	std::map<sf::Shape*,  MovementRoutineContainer*>  m_Routine_Movement_Shape;
	std::map<sf::Sprite*, MovementRoutineContainer*> m_Routine_Movement_Sprite;

	MovementRoutineContainer* movementRoutineContainer;

public:
	MovementManager();

	MovementRoutine* createMovementRoutine(const std::string& _name) { return this->movementRoutineContainer->createRoutine(_name); }
	MovementRoutine* linkMovementRoutine(sf::Shape* _shape, const std::string& _name);
	MovementRoutine* linkMovementRoutine(sf::Sprite* _sprite, const std::string& _name);

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