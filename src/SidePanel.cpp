#pragma once
#include "SidePanel.h"

// Constructors / Destructors

SidePanel::SidePanel() :
	m_titleFontSize(32u),
	m_textFontSize(16u),
	m_padding(12.f)
	{}

SidePanel::SidePanel(const sf::RenderTarget& window, const std::string& fontPath, unsigned titleFontSize, unsigned textFontSize, float padding) :
	m_titleFontSize(titleFontSize),
	m_textFontSize(textFontSize),
	m_padding(padding),
	m_fontPath(fontPath),
	m_window(&window)
{
	if (!m_font.loadFromFile(m_fontPath)) 
		throw std::runtime_error("Error: SidePanel::SidePanel() - Font failed to load.");
	
	m_title.setFont(m_font);
	m_title.setCharacterSize(m_titleFontSize);
	m_title.setFillColor(sf::Color::White);
}

SidePanel::SidePanel(const SidePanel& obj) :
	m_titleFontSize(obj.m_titleFontSize),
	m_textFontSize(obj.m_textFontSize),
	m_padding(obj.m_padding),
	m_fontPath(obj.m_fontPath),
	m_window(obj.m_window),
	m_font(obj.m_font),
	m_title(obj.m_title),
	m_textString(obj.m_textString),
	m_textObjects(obj.m_textObjects)
	{}

SidePanel::~SidePanel()
{
	m_textString.clear();
	m_textObjects.clear();
	m_window = nullptr;
}

// Accessors / Mutators
void SidePanel::setWindow(const sf::RenderTarget& window)
{
	m_window = &window;
}

void SidePanel::setFont(const std::string& fontPath)
{
	m_fontPath = fontPath;
	if (!m_font.loadFromFile(m_fontPath)) 
		throw std::runtime_error("Error: SidePanel::setFont() - Font failed to load.");

	m_title.setFont(m_font);
	for (auto& text : m_textObjects) {
		text.first.setFont(m_font);
		text.second.setFont(m_font);
	}
}

void SidePanel::setFont(const sf::Font& font)
{
	m_font = font;
	m_title.setFont(m_font);
	for (auto& text : m_textObjects) {
		text.first.setFont(m_font);
		text.second.setFont(m_font);
	}
}

void SidePanel::setTitleFontSize(unsigned fontSize)
{
	m_titleFontSize = fontSize;
	m_title.setCharacterSize(m_titleFontSize);
}

void SidePanel::setTextFontSize(unsigned fontSize)
{
	m_textFontSize = fontSize;
	for (auto& text : m_textObjects) {
		text.first.setCharacterSize(m_textFontSize);
		text.second.setCharacterSize(m_textFontSize);
	}
}

void SidePanel::setPadding(float padding)
{
	m_padding = padding;
}

void SidePanel::setWidth(float width)
{
	m_width = width;
}

void SidePanel::setHeight(float height)
{
	m_height = height;
}

void SidePanel::setTitle(const std::string& title)
{
	m_title.setString(title);
}

void SidePanel::addText(const std::string& label, const std::string& description)
{
	m_textString.push_back(std::make_pair(label, description));
	sf::Text label_text;
	label_text.setFont(m_font);
	label_text.setCharacterSize(m_textFontSize);
	label_text.setFillColor(sf::Color::White);
	label_text.setString(label);
	label_text.setOrigin(0.f, label_text.getLocalBounds().top / 2.f);
	// setposition

	sf::Text description_text;
	description_text.setFont(m_font);
	description_text.setCharacterSize(m_textFontSize);
	description_text.setFillColor(sf::Color::White);
	description_text.setString(description);
	description_text.setOrigin(0.f, description_text.getLocalBounds().top / 2.f);

	m_textObjects.push_back(std::make_pair(label_text, description_text));
}

void SidePanel::clearText()
{
	m_textString.clear();
	m_textObjects.clear();
}
