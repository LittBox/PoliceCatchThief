#pragma once

#include "Route.h"
#include "GameObject.h"
#include "Police.h"
#include "Thief.h"
#include "TypingManager.h"
#include "Difficulty.h"

#include <SFML/Graphics.hpp>
#include <map>
#include <memory>
#include <string>
#include <vector>


class Game {
public:
    Game();
    void run();

private:
    enum class GameState {
        Menu,
        Playing,
        Win,
        Lose
    };

private:
    void processEvents();
    void update(float dt);
    void render();

    void initLayout();
    void initCharacters();
    void resetGame();
    void startGame(const std::string& difficultyName);

    void initDifficulties();
    void applyDifficulty(const std::string& difficultyName);    

    void checkGameEnd();
    float getDistanceBetweenCharacters() const;

    void drawText(
        const std::string& content,
        sf::Vector2f position,
        unsigned int size,
        sf::Color color
    );

    void drawTypingText();
    void drawGameResultOverlay();
    void drawMenuScreen();
    void initMenuButtons();
    void handleMenuClick(sf::Vector2f mousePosition);
    void drawMenuButton(
        const sf::RectangleShape& button,
        const std::string& label,
        sf::Color textColor
    );

private:
    static constexpr unsigned int WINDOW_WIDTH = 900;
    static constexpr unsigned int WINDOW_HEIGHT = 650;

    sf::RenderWindow m_window;

    sf::Font m_font;
    bool m_fontLoaded;

    sf::RectangleShape m_gameArea;
    sf::RectangleShape m_inputPanel;
    sf::RectangleShape m_infoCard;
    sf::RectangleShape m_helpCard;
    sf::RectangleShape m_easyButton;
    sf::RectangleShape m_mediumButton;
    sf::RectangleShape m_hardButton;
    

    Route m_route;

    std::shared_ptr<Police> m_police;
    std::shared_ptr<Thief> m_thief;
    std::vector<std::shared_ptr<GameObject>> m_gameObjects;

    TypingManager m_typingManager;

    float m_cursorTimer;
    bool m_cursorVisible;

    GameState m_gameState;
    std::map<std::string, DifficultyConfig> m_difficulties;
    std::string m_currentDifficulty;
    float m_timeLimit;
    float m_remainingTime;
    float m_captureDistance;

    
};