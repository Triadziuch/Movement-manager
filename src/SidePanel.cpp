#pragma once
#include "SidePanel.h"

// Constructors / Destructors

void SidePanel::instantHide()
{
	m_hidden = true;

	m_backgroundMovementRoutine->reset();
	m_titleMovementRoutine->reset();
	for (auto& movementRoutine : m_textMovementRoutines) 
		movementRoutine->reset();

	m_background.setPosition(-m_width, 0.f);
	m_title.setPosition(-m_width / 2.f, 0.f);
	for (auto& text : m_textObjects) 
		text.setPosition(m_padding - m_width, text.getPosition().y);
}

void SidePanel::recalculateTextPositions()
{
	// Adjusting background position
	m_backgroundMovementRoutine->operator[](0)->getStartingPos().x = -m_width;
	m_backgroundMovementRoutine->operator[](1)->getEndingPos().x   = -m_width;

	// Adjusting title position
	m_title.setOrigin(m_title.getLocalBounds().left + m_title.getLocalBounds().width / 2.f, m_title.getLocalBounds().top + m_title.getLocalBounds().height / 2.f);
	m_title.setPosition(-m_width / 2.f, m_padding + m_title.getGlobalBounds().height / 2.f);

	m_titleMovementShow->getCurrentMovement()->getStartingPos() = sf::Vector2f(m_title.getPosition());
	m_titleMovementShow->getCurrentMovement()->getEndingPos() = sf::Vector2f(m_title.getPosition().x + m_width, m_title.getPosition().y);

	m_titleMovementHide->getCurrentMovement()->getStartingPos() = sf::Vector2f(m_title.getPosition().x + m_width, m_title.getPosition().y);
	m_titleMovementHide->getCurrentMovement()->getEndingPos() = sf::Vector2f(m_title.getPosition());

	// Adjusting text position
	if (!m_textObjects.empty()) {
		m_textObjects[0].first.setOrigin(0.f, m_textObjects[0].first.getLocalBounds().top / 2.f);
		m_textObjects[0].second.setOrigin(0.f, m_textObjects[0].second.getLocalBounds().top / 2.f);

		if (m_title.getString().isEmpty()) {
			m_textObjects[0].first.setPosition(m_background.getPosition().x + m_padding, m_padding + m_textObjects[0].first.getGlobalBounds().height / 2.f);
			m_textObjects[0].second.setPosition(m_textObjects[0].first.getPosition().x + m_textObjects[0].first.getGlobalBounds().width + m_padding, m_textObjects[0].first.getPosition().y);
		}
		else {
			m_textObjects[0].first.setPosition(m_background.getPosition().x + m_padding, m_title.getPosition().y + m_title.getGlobalBounds().height / 2.f + m_padding + m_textObjects[0].first.getGlobalBounds().height / 2.f);
			m_textObjects[0].second.setPosition(m_textObjects[0].first.getPosition().x + m_textObjects[0].first.getGlobalBounds().width + m_padding, m_textObjects[0].first.getPosition().y);
		}

		m_MovementRoutinesShow[0].first->getCurrentMovement()->getStartingPos() = sf::Vector2f(m_padding - m_width, m_textObjects[0].first.getPosition().y);
		m_MovementRoutinesShow[0].first->getCurrentMovement()->getEndingPos() = sf::Vector2f(m_padding, m_textObjects[0].first.getPosition().y);
		m_MovementRoutinesShow[0].second->getCurrentMovement()->getStartingPos() = sf::Vector2f(m_padding - m_width + m_textObjects[0].first.getGlobalBounds().width, m_textObjects[0].second.getPosition().y);
		m_MovementRoutinesShow[0].second->getCurrentMovement()->getEndingPos() = sf::Vector2f(m_padding + m_textObjects[0].first.getGlobalBounds().width, m_textObjects[0].second.getPosition().y);

		m_MovementRoutinesHide[0].first->getCurrentMovement()->getStartingPos() = sf::Vector2f(m_padding, m_textObjects[0].first.getPosition().y);
		m_MovementRoutinesHide[0].first->getCurrentMovement()->getEndingPos() = sf::Vector2f(m_padding - m_width, m_textObjects[0].first.getPosition().y);
		m_MovementRoutinesHide[0].second->getCurrentMovement()->getStartingPos() = sf::Vector2f(m_padding + m_textObjects[0].first.getGlobalBounds().width, m_textObjects[0].second.getPosition().y);
		m_MovementRoutinesHide[0].second->getCurrentMovement()->getEndingPos() = sf::Vector2f(m_padding - m_width + m_textObjects[0].first.getGlobalBounds().width, m_textObjects[0].second.getPosition().y);

		for (size_t i = 1; i < m_textObjects.size(); ++i) {
			m_textObjects[i].first.setOrigin(0.f, m_textObjects[i].first.getLocalBounds().top / 2.f);
			m_textObjects[i].second.setOrigin(0.f, m_textObjects[i].second.getLocalBounds().top / 2.f);
			m_textObjects[i].first.setPosition(m_background.getPosition().x + m_padding, m_textObjects[i - 1].second.getPosition().y + m_textObjects[i - 1].second.getGlobalBounds().height / 2.f + m_textObjects[i].first.getGlobalBounds().height / 2.f);
			m_textObjects[i].second.setPosition(m_textObjects[i].first.getPosition().x + m_textObjects[i].first.getGlobalBounds().width + m_padding, m_textObjects[i].first.getPosition().y);

			m_MovementRoutinesShow[i].first->getCurrentMovement()->getStartingPos() = sf::Vector2f(m_padding - m_width, m_textObjects[i].first.getPosition().y);
			m_MovementRoutinesShow[i].first->getCurrentMovement()->getEndingPos() = sf::Vector2f(m_padding, m_textObjects[i].first.getPosition().y);
			m_MovementRoutinesShow[i].second->getCurrentMovement()->getStartingPos() = sf::Vector2f(2.f * m_padding - m_width + m_textObjects[i].first.getGlobalBounds().width, m_textObjects[i].second.getPosition().y);
			m_MovementRoutinesShow[i].second->getCurrentMovement()->getEndingPos() = sf::Vector2f(2.f * m_padding + m_textObjects[i].first.getGlobalBounds().width, m_textObjects[i].second.getPosition().y);

			m_MovementRoutinesHide[i].first->getCurrentMovement()->getStartingPos() = sf::Vector2f(m_padding, m_textObjects[i].first.getPosition().y);
			m_MovementRoutinesHide[i].first->getCurrentMovement()->getEndingPos() = sf::Vector2f(m_padding - m_width, m_textObjects[i].first.getPosition().y);
			m_MovementRoutinesHide[i].second->getCurrentMovement()->getStartingPos() = sf::Vector2f(2.f * m_padding + m_textObjects[i].first.getGlobalBounds().width, m_textObjects[i].second.getPosition().y);
			m_MovementRoutinesHide[i].second->getCurrentMovement()->getEndingPos() = sf::Vector2f(2.f * m_padding - m_width + m_textObjects[i].first.getGlobalBounds().width, m_textObjects[i].second.getPosition().y);
		}
	}

	instantHide();
}

SidePanel::SidePanel() :
	m_titleFontSize(32u),
	m_textFontSize(16u),
	m_padding(12.f),
	m_movementManager(MovementManager::getInstance())
	{}

SidePanel::SidePanel(const sf::RenderTarget& window, const std::string& fontPath, unsigned titleFontSize, unsigned textFontSize, float padding) :
	m_titleFontSize(titleFontSize),
	m_textFontSize(textFontSize),
	m_padding(padding),
	m_fontPath(fontPath),
	m_window(&window),
	m_width{static_cast<float>(window.getSize().x) / 4.f },
	m_height{static_cast<float>(window.getSize().y) },
	m_movementManager(MovementManager::getInstance()),
	m_backgroundColor{sf::Color(0, 0, 0, 200)},
	m_hidden{true},
	m_duration{0.5f},
	m_showing{false},
	m_backgroundMovementName{"BackgroundMovement"},
	m_titleMovementName{"TitleMovement"},
	m_textMovementNames{"TextMovement"}
{
	// Font initialization
	if (!m_font.loadFromFile(m_fontPath)) 
		throw std::runtime_error("Error: SidePanel::SidePanel() - Font failed to load.");
	
	// Background initialization
	m_background.setSize(sf::Vector2f(m_width, m_height));
	m_background.setFillColor(m_backgroundColor);
	m_background.setPosition(-m_width, 0.f);

	auto m_backgroundMovement = m_movementManager->createMovementRoutine(m_backgroundMovementName);
	m_backgroundMovement->addMovement(new movementInfo(sf::Vector2f(-m_width, 0.f), sf::Vector2f(0.f, 0.f), m_duration, easeFunctions::getFunction(easeFunctions::IN_SINE), 0.f, 0.f, 0.f));
	m_backgroundMovement->addMovement(new movementInfo(sf::Vector2f(0.f, 0.f), sf::Vector2f(-m_width, 0.f), m_duration, easeFunctions::getFunction(easeFunctions::IN_SINE), 0.f, 0.f, 0.f));
	m_backgroundMovement->setPauseAfterChangingMovements(true);
	m_backgroundMovement->setLooping(true);
	m_movementManager->linkMovementRoutine(m_background, m_backgroundMovementName);
	m_movementManager->startMovementRoutine(m_background, m_backgroundMovementName);
	m_showing = &m_backgroundMovement->m_isPaused;

	// Title initialization
	m_title.setFont(m_font);
	m_title.setCharacterSize(m_titleFontSize);
	m_title.setFillColor(sf::Color::White);

	auto m_titleMovement = m_movementManager->createMovementRoutine(m_titleMovementName);
	m_titleMovement->addMovement(new movementInfo(sf::Vector2f(-m_width / 2.f, 0.f), sf::Vector2f(m_width / 2.f, 0.f), m_duration, easeFunctions::getFunction(easeFunctions::IN_SINE), 0.f, 0.f, 0.f));
	m_titleMovement->addMovement(new movementInfo(sf::Vector2f(m_width / 2.f, 0.f), sf::Vector2f(-m_width / 2.f, 0.f), m_duration, easeFunctions::getFunction(easeFunctions::IN_SINE), 0.f, 0.f, 0.f));
	m_titleMovement->setPauseAfterChangingMovements(true);
	m_titleMovement->setLooping(true);
	m_titleMovementRoutine = m_movementManager->linkMovementRoutine(m_title, m_titleMovementName);
	m_movementManager->startMovementRoutine(m_title, m_titleMovementName);
}

SidePanel::SidePanel(const SidePanel& obj) :
	m_titleFontSize(obj.m_titleFontSize),
	m_textFontSize(obj.m_textFontSize),
	m_padding(obj.m_padding),
	m_fontPath(obj.m_fontPath),
	m_window(obj.m_window),
	m_font(obj.m_font),
	m_title(obj.m_title),
	m_textObjects(obj.m_textObjects),
	m_width(obj.m_width),
	m_height(obj.m_height),
	m_background(obj.m_background),
	m_backgroundColor(obj.m_backgroundColor),
	m_hidden(obj.m_hidden),
	m_movementManager(obj.m_movementManager),
	m_duration(obj.m_duration),
	m_showing(obj.m_showing),
	m_backgroundMovementName(obj.m_backgroundMovementName),
	m_titleMovementName(obj.m_titleMovementName),
	m_textMovementNames(obj.m_textMovementNames)
	{}

SidePanel::~SidePanel()
{
	instantHide();

	m_textObjects.clear();

	m_movementManager->deleteMovementRoutine(m_backgroundMovementName);
	m_backgroundMovementRoutine = nullptr;

	m_movementManager->deleteMovementRoutine(m_titleMovementName);
	m_titleMovementRoutine = nullptr;

	for (size_t i = 0; i < m_textMovementRoutines.size(); ++i) {
		m_movementManager->deleteMovementRoutine(m_textMovementNames + std::to_string(i));
		m_textMovementRoutines[i] = nullptr;
	}

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
	for (auto& text : m_textObjects)
		text.setFont(m_font);

	recalculateTextPositions();
}

void SidePanel::setFont(const sf::Font& font)
{
	m_font = font;
	m_title.setFont(m_font);
	for (auto& text : m_textObjects)
		text.setFont(m_font);

	recalculateTextPositions();
}

void SidePanel::setTitleFontSize(unsigned fontSize)
{
	m_titleFontSize = fontSize;
	m_title.setCharacterSize(m_titleFontSize);

	recalculateTextPositions();
}

void SidePanel::setTextFontSize(unsigned fontSize)
{
	m_textFontSize = fontSize;

	for (auto& text : m_textObjects) 
		text.setCharacterSize(m_textFontSize);

	recalculateTextPositions();
}

void SidePanel::setPadding(float padding)
{
	const float padding_offset = padding - m_padding;
	m_padding = padding;

	// Adjusting title position and movement routine
	m_title.move(0.f, padding_offset);
	m_titleMovementRoutine->move(sf::Vector2f(0.f, padding_offset));

	// Adjusting text position
	float it = 1.f;
	if (!m_title.getString().isEmpty())
		it = 3.f;

	for (size_t i = 0; it < m_textObjects.size(); ++it, it += 2.f) {
		m_textObjects[i].move(padding_offset, it * padding_offset);
		m_textMovementRoutines[i]->move(sf::Vector2f(padding_offset, it * padding_offset));
	}
}

void SidePanel::setWidth(float width)
{
	m_hidden = true;

	m_width = width;
	m_background.setSize(sf::Vector2f(m_width, m_height));
	m_backgroundMovementRoutine->operator[](0)->getStartingPos().x = -m_width;
	m_backgroundMovementRoutine->operator[](1)->getEndingPos().x   = -m_width;

	instantHide();

	m_titleMovementRoutine->operator[](0)->getStartingPos().x = m_title.getPosition().x;
	m_titleMovementRoutine->operator[](1)->getEndingPos().x   = m_titleMovementRoutine->operator[](0)->getStartingPos().x;

	for (auto& movementRoutine : m_textMovementRoutines) {
		movementRoutine->operator[](0)->getStartingPos().x = m_padding - m_width;
		movementRoutine->operator[](1)->getEndingPos().x   = movementRoutine->operator[](0)->getStartingPos().x;
	}
}

void SidePanel::setHeight(float height)
{
	m_height = height;
	sf::Vector2f old_pos = m_background.getPosition();
	m_background.setSize(sf::Vector2f(m_width, m_height));

	instantHide();
}

void SidePanel::setBackgroundColor(const sf::Color& color)
{
	m_backgroundColor = color;
	m_background.setFillColor(m_backgroundColor);
}

unsigned SidePanel::getTitleFontSize() const
{
	return m_titleFontSize;
}

void SidePanel::setTitle(const std::string& title)
{
	m_title.setString(title);
	
	recalculateTextPositions();
}

void SidePanel::addText(const std::string& label)
{
	instantHide();

	sf::Text text;
	text.setFont(m_font);
	text.setCharacterSize(m_textFontSize);
	text.setFillColor(sf::Color::White);
	text.setString(label);
	text.setOrigin(0.f, text.getLocalBounds().top / 2.f);

	if (m_textObjects.empty()) {
		if (m_title.getString().isEmpty()) 
			text.setPosition(m_padding - m_width, m_padding + text.getGlobalBounds().height / 2.f);
		else 
			text.setPosition(m_padding - m_width, 2.f * m_padding + m_title.getPosition().y + m_title.getGlobalBounds().height / 2.f + text.getGlobalBounds().height / 2.f);
	}
	else 
		text.setPosition(m_padding - m_width, 2.f * m_padding + m_textObjects.back().getPosition().y + m_textObjects.back().getGlobalBounds().height / 2.f + text.getGlobalBounds().height / 2.f);
	

	auto label_movement = m_movementManager->createMovementRoutine(m_textMovementNames + std::to_string(m_textObjects.size()));
	label_movement->addMovement(new movementInfo(text.getPosition(), text.getPosition() + sf::Vector2f(m_width, 0.f), m_duration, easeFunctions::getFunction(easeFunctions::IN_SINE), 0.f, 0.f, 0.f));
	label_movement->addMovement(new movementInfo(text.getPosition() + sf::Vector2f(m_width, 0.f), text.getPosition(), m_duration, easeFunctions::getFunction(easeFunctions::IN_SINE), 0.f, 0.f, 0.f));
	label_movement->setPauseAfterChangingMovements(true);
	label_movement->setLooping(true);
	m_textMovementRoutines.emplace_back(m_movementManager->linkMovementRoutine(text, m_textMovementNames + std::to_string(m_textObjects.size())));
	m_movementManager->startMovementRoutine(text, m_textMovementNames + std::to_string(m_textObjects.size()));

	m_textObjects.push_back(text);
}

void SidePanel::clearText()
{
	for (auto& movementRoutine : m_textMovementRoutines) {
		m_movementManager->deleteMovementRoutine(movementRoutine->getName());
		movementRoutine = nullptr;
	}

	m_textObjects.clear();
}

void SidePanel::show()
{
	if (m_hidden == true && !*m_showing) {
		m_hidden = false;

		m_backgroundMovementRoutine->resume();
		m_titleMovementRoutine->resume();
		for (auto& movementRoutine : m_textMovementRoutines) 
			movementRoutine->resume();
	}
}

void SidePanel::hide()
{
	if (m_hidden == false && !*m_showing) {
		m_hidden = true;

		m_backgroundMovementRoutine->resume();
		m_titleMovementRoutine->resume();
		for (auto& movementRoutine : m_textMovementRoutines) 
			movementRoutine->resume();
	}
}

void SidePanel::draw(sf::RenderTarget& window)
{
	window.draw(m_background);
	window.draw(m_title);
	for (const auto& text : m_textObjects) 
		window.draw(text);
}
