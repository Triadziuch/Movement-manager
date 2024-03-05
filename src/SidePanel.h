#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class SidePanel {
private:
	unsigned	m_titleFontSize{};
	unsigned	m_textFontSize{};
	float		m_padding{};
	std::string m_fontPath{};

	float m_width{};
	float m_height{};

	const sf::RenderTarget* m_window{};
	sf::Font		  m_font{};
	sf::Text		  m_title{};

	std::vector<std::pair<std::string, std::string>> m_textString{};
	std::vector<std::pair<sf::Text, sf::Text>>		 m_textObjects{};

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

	void setTitle(const std::string& title);
	void addText(const std::string& label, const std::string& description);
	void clearText();
};