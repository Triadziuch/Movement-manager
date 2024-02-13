#pragma once
#include "MovementContainer.h"

struct MovementRoutine {
	std::string routine_name{};
	std::vector <movementInfo*> routine_movements;
	size_t current_movement{};
	size_t movement_count{};
	bool is_active{};
	bool is_looping{};
	bool is_paused{};
};

class MovementManager {
private:
	// Singleton instance
	static MovementManager* sInstance;

	MovementContainer* movementContainer;

	std::map<sf::Shape*, std::vector<movementInfo*>> m_Routine_Movement;

public:
	MovementManager();

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