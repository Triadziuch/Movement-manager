#pragma once
#include <SFML/Graphics.hpp>
#include "MovementManager.h"
#include "easeFunctions.h"
#include <vector>
#include <string>

class SidePanel {
private:
	MovementManager* m_movementManager{};

	unsigned	m_titleFontSize{};
	unsigned	m_textFontSize{};
	float		m_padding{};
	std::string m_fontPath{};

	bool  m_hidden{}, m_showing{}, m_hiding{};
	float m_width{};
	float m_height{};
	float m_duration{};
	sf::RectangleShape m_background{};
	sf::Color		   m_backgroundColor{};

	const sf::RenderTarget* m_window{};
	sf::Font		  m_font{};
	sf::Text		  m_title{};

	std::vector<sf::Text>		  m_textObjects{};

	std::string					  m_backgroundMovementName{};
	MovementRoutine*			  m_backgroundMovementRoutine{};

	std::string					  m_titleMovementName{};
	MovementRoutine*			  m_titleMovementRoutine{};

	std::string					  m_textMovementNames{};
	std::vector<MovementRoutine*> m_textMovementRoutines{};

	void instantHide();
	void recalculateTextPositions();

public:
	// Constructors / Destructors
	SidePanel();
	SidePanel(const sf::RenderTarget& window, const std::string& fontPath, unsigned titleFontSize = 32u, unsigned textFontSize = 16u, float padding = 12.f);
	SidePanel(const SidePanel& obj);
	~SidePanel();

	// Accessors / Mutators
	void setWindow(const sf::RenderTarget& window);
	void setFont(const std::string& fontPath);
	void setFont(const sf::Font& font);

	void setTitleFontSize(unsigned fontSize);
	void setTextFontSize(unsigned fontSize);
	void setPadding(float padding);
	void setWidth(float width);
	void setHeight(float height);
	void setBackgroundColor(const sf::Color& color);

	unsigned getTitleFontSize() const;

	void setTitle(const std::string& title);
	void addText(const std::string& text);
	void clearText();

	void show();
	void hide();

	void draw(sf::RenderTarget& window);
};