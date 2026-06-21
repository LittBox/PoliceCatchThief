#include "Game.h"
#include <cmath>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <exception>

Game::Game()
    : m_window(
          sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
          "Police Catch Thief"
      ),
      m_fontLoaded(false),
      m_cursorTimer(0.f),
      m_cursorVisible(true),
      m_gameState(GameState::Menu),
      m_currentDifficulty("Medium"),
      m_timeLimit(75.f),
      m_remainingTime(75.f),
      m_captureDistance(35.f) {
    std::cout << "[TEST] Game created." << std::endl;

    m_window.setFramerateLimit(60);

    if (m_font.openFromFile("assets/font.ttf")) {
        m_fontLoaded = true;
        std::cout << "[TEST] Font loaded successfully." << std::endl;
    } else {
        std::cerr << "[ERROR] Failed to load font: assets/font.ttf" << std::endl;
    }

    initLayout();
    initMenuButtons();
    initDifficulties();
    initCharacters();

    applyDifficulty("Medium");

    std::cout << "[TEST] Route total length: "
            << m_route.getTotalLength()
            << std::endl;
}

void Game::run() {
    std::cout << "[TEST] Game loop started." << std::endl;

    sf::Clock clock;

    while (m_window.isOpen()) {
        float dt = clock.restart().asSeconds();

        processEvents();
        update(dt);
        render();
    }

    std::cout << "[TEST] Game loop finished." << std::endl;
}

void Game::processEvents() {
    while (auto event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            std::cout << "[TEST] Window closed." << std::endl;
            m_window.close();
        }

        if (event->is<sf::Event::MouseButtonPressed>()) {
            const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();

            if (mouseEvent &&
                mouseEvent->button == sf::Mouse::Button::Left &&
                m_gameState == GameState::Menu) {

                sf::Vector2f mousePosition =
                    m_window.mapPixelToCoords(mouseEvent->position);

                handleMenuClick(mousePosition);
            }
        }

        if (event->is<sf::Event::KeyPressed>()) {
            const auto* keyEvent = event->getIf<sf::Event::KeyPressed>();

            if (keyEvent && keyEvent->scancode == sf::Keyboard::Scancode::Escape) {
                std::cout << "[TEST] Escape pressed. Window closed." << std::endl;
                m_window.close();
            }
            if (keyEvent && keyEvent->scancode == sf::Keyboard::Scancode::F2) {
                if (m_gameState == GameState::Playing ||
                    m_gameState == GameState::Win ||
                    m_gameState == GameState::Lose) {
                    m_gameState = GameState::Menu;
                    std::cout << "[TEST] Back to menu by F2." << std::endl;
                }
            }

            if (keyEvent && keyEvent->scancode == sf::Keyboard::Scancode::Tab) {
                if (m_gameState == GameState::Playing) {
                    m_typingManager.reset();
                    std::cout << "[TEST] Typing text reset." << std::endl;
                }
            }

            if (keyEvent && keyEvent->scancode == sf::Keyboard::Scancode::Enter) {
                if (m_gameState != GameState::Playing) {
                    resetGame();
                    std::cout << "[TEST] Game restarted." << std::endl;
                }
            }
        }

        if (event->is<sf::Event::TextEntered>()) {
            const auto* textEvent = event->getIf<sf::Event::TextEntered>();

            if (!textEvent) {
                continue;
            }

            char32_t unicode = textEvent->unicode;

            if (m_gameState == GameState::Menu) {
                if (unicode == U'1') {
                    startGame("Easy");
                } else if (unicode == U'2') {
                    startGame("Medium");
                } else if (unicode == U'3') {
                    startGame("Hard");
                }

                continue;
            }

            if (m_gameState != GameState::Playing) {
                continue;
            }

            if (unicode >= 32 && unicode <= 126) {
                m_typingManager.handleChar(static_cast<char>(unicode));
            }
        }
    }
}

void Game::update(float dt) {
    m_cursorTimer += dt;
    if (m_cursorTimer >= 0.5f) {
        m_cursorVisible = !m_cursorVisible;
        m_cursorTimer = 0.f;
    }

    if (m_gameState != GameState::Playing) {
        return;
    }

    m_remainingTime -= dt;
    if (m_remainingTime < 0.f) {
        m_remainingTime = 0.f;
    }

    m_typingManager.update(dt);

    if (m_police) {
        m_police->updateSpeedByTyping(
            m_typingManager.getCPM(),
            m_typingManager.getAccuracy()
        );
    }

    for (auto& object : m_gameObjects) {
        object->update(dt);
    }

    if (m_police) {
        m_police->setPosition(
            m_route.getPositionAt(m_police->getRouteDistance())
        );
    }

    if (m_thief) {
        m_thief->setPosition(
            m_route.getPositionAt(m_thief->getRouteDistance())
        );
    }

    bool thiefTurned = m_thief->checkTurnAround(
        m_police->getRouteDistance(),
        m_route.getTotalLength()
    );

    if (thiefTurned) {
        m_police->setDirection(m_thief->getDirection());
    }

    checkGameEnd();
}

void Game::render() {
    m_window.clear(sf::Color(25, 25, 30));
    if (m_gameState == GameState::Menu) {
        drawMenuScreen();
        m_window.display();
        return;
    }
    m_window.draw(m_gameArea);

    m_route.draw(m_window);

    for (const auto& object : m_gameObjects) {
        object->draw(m_window);
    }

    m_window.draw(m_inputPanel);
    m_window.draw(m_infoCard);
    m_window.draw(m_helpCard);

    drawText(
        "Difficulty: " + m_currentDifficulty,
        {35.f, 35.f},
        20,
        sf::Color::White
    );
    drawText(
        "CPM: " + std::to_string(static_cast<int>(m_typingManager.getCPM())),
        {35.f, 65.f},
        20,
        sf::Color::White
    );
    drawText(
        "Accuracy: " +
            std::to_string(static_cast<int>(m_typingManager.getAccuracy() * 100.f)) +
            "%",
        {35.f, 95.f},
        20,
        sf::Color::White
    );

    drawText(
        "Time: " + std::to_string(static_cast<int>(m_remainingTime)) + "s",
        {35.f, 125.f},
        20,
        sf::Color::White
    );
    
    drawText(
        "Speed: " + std::to_string(static_cast<int>(m_police->getSpeed())),
        {35.f, 155.f},
        20,
        sf::Color::White
    );
    
    drawText(
        "Controls",
        {575.f, 47.f},
        18,
        sf::Color(220, 220, 220)
    );

    drawText(
        "Tab: Reset typing",
        {575.f, 73.f},
        17,
        sf::Color(210, 210, 210)
    );

    drawText(
        "F2: Return to menu",
        {575.f, 98.f},
        17,
        sf::Color(210, 210, 210)
    );


    drawTypingText();

    drawGameResultOverlay();

    m_window.display();
}


void Game::initLayout() {
    m_gameArea.setSize({860.f, 510.f});
    m_gameArea.setPosition({20.f, 20.f});
    m_gameArea.setFillColor(sf::Color(38, 42, 48));
    m_gameArea.setOutlineThickness(2.f);
    m_gameArea.setOutlineColor(sf::Color(70, 75, 85));

    m_inputPanel.setSize({860.f, 80.f});
    m_inputPanel.setPosition({20.f, 550.f});
    m_inputPanel.setFillColor(sf::Color(35, 38, 45));
    m_inputPanel.setOutlineThickness(2.f);
    m_inputPanel.setOutlineColor(sf::Color(90, 95, 105));

    m_infoCard.setSize({240.f, 160.f});
    m_infoCard.setPosition({30.f, 30.f});
    m_infoCard.setFillColor(sf::Color(0, 0, 0, 130));
    m_infoCard.setOutlineThickness(1.f);
    m_infoCard.setOutlineColor(sf::Color(150, 150, 150, 120));

    m_helpCard.setSize({280.f, 85.f});
    m_helpCard.setPosition({555.f, 35.f});
    m_helpCard.setFillColor(sf::Color(0, 0, 0, 120));
    m_helpCard.setOutlineThickness(1.f);
    m_helpCard.setOutlineColor(sf::Color(150, 150, 150, 120));
}

void Game::drawText(
    const std::string& content,
    sf::Vector2f position,
    unsigned int size,
    sf::Color color
) {
    if (!m_fontLoaded) {
        return;
    }

    sf::Text text(m_font);
    text.setString(content);
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setPosition(position);

    m_window.draw(text);
}

void Game::initCharacters() {
    m_police = std::make_shared<Police>();
    m_thief = std::make_shared<Thief>();

    m_police->setRouteDistance(0.f);
    m_police->setPosition(m_route.getPositionAt(0.f));

    const float thiefStartDistance = m_route.getTotalLength() * 0.5f;
    m_thief->setRouteDistance(thiefStartDistance);
    m_thief->setPosition(m_route.getPositionAt(thiefStartDistance));

    m_gameObjects.push_back(m_police);
    m_gameObjects.push_back(m_thief);

    std::cout << "[TEST] Characters initialized." << std::endl;
}

void Game::drawTypingText() {
    if (!m_fontLoaded) {
        return;
    }

    const std::string& targetText = m_typingManager.getTargetText();
    const std::size_t currentIndex = m_typingManager.getCurrentIndex();

    const float startX = 65.f;
    const float startY = 568.f;
    const float visibleLeft = 65.f;
    const float visibleRight = 835.f;
    const float visibleWidth = visibleRight - visibleLeft;

    const unsigned int fontSize = 34;

    if (targetText.empty()) {
        return;
    }

    sf::Text measureText(m_font);
    measureText.setCharacterSize(fontSize);
    measureText.setString(targetText);

    std::vector<float> characterPositions;
    characterPositions.reserve(targetText.size() + 1);

    for (std::size_t i = 0; i <= targetText.size(); ++i) {
        characterPositions.push_back(measureText.findCharacterPos(i).x);
    }

    float cursorLocalX = characterPositions[currentIndex];

    float scrollOffset = 0.f;
    const float cursorKeepPosition = visibleWidth * 0.65f;

    if (cursorLocalX > cursorKeepPosition) {
        scrollOffset = cursorLocalX - cursorKeepPosition;
    }

    sf::Vector2f cursorPosition{
        startX + cursorLocalX - scrollOffset,
        startY
    };

    for (std::size_t i = 0; i < targetText.size(); ++i) {
        float charLocalX = characterPositions[i];
        float nextCharLocalX = characterPositions[i + 1];

        float drawX = startX + charLocalX - scrollOffset;
        float nextDrawX = startX + nextCharLocalX - scrollOffset;

        if (nextDrawX < visibleLeft || drawX > visibleRight) {
            continue;
        }

        std::string charString(1, targetText[i]);

        sf::Text charText(m_font);
        charText.setString(charString);
        charText.setCharacterSize(fontSize);

        if (i < currentIndex) {
            charText.setFillColor(sf::Color(90, 220, 130));
        } else {
            charText.setFillColor(sf::Color(150, 150, 150));
        }

        charText.setPosition({drawX, startY});
        m_window.draw(charText);
    }

    if (m_cursorVisible && m_gameState == GameState::Playing) {
        if (cursorPosition.x >= visibleLeft && cursorPosition.x <= visibleRight) {
            sf::RectangleShape cursor;
            cursor.setSize({3.f, 34.f});
            cursor.setFillColor(sf::Color::White);
            cursor.setPosition({cursorPosition.x + 2.f, cursorPosition.y + 4.f});

            m_window.draw(cursor);
        }
    }
}

void Game::checkGameEnd() {
    if (m_gameState != GameState::Playing) {
        return;
    }

    float distance = getDistanceBetweenCharacters();

    if (distance <= m_captureDistance) {
        m_gameState = GameState::Win;
        std::cout << "[TEST] You win. Distance: " << distance << std::endl;
        return;
    }

    if (m_remainingTime <= 0.f) {
        m_gameState = GameState::Lose;
        std::cout << "[TEST] Game over. Time is up." << std::endl;
    }
}

float Game::getDistanceBetweenCharacters() const {
    if (!m_police || !m_thief) {
        return 999999.f;
    }

    sf::Vector2f policePosition = m_police->getPosition();
    sf::Vector2f thiefPosition = m_thief->getPosition();

    float dx = policePosition.x - thiefPosition.x;
    float dy = policePosition.y - thiefPosition.y;

    return std::sqrt(dx * dx + dy * dy);
}

void Game::resetGame() {
    m_gameState = GameState::Playing;
    m_remainingTime = m_timeLimit;

    m_typingManager.reset();

    if (m_police) {
        m_police->setRouteDistance(0.f);
        m_police->setDirection(1);
        m_police->setSpeed(0.f);
        m_police->setPosition(m_route.getPositionAt(0.f));
    }

    if (m_thief) {
        const float thiefStartDistance = m_route.getTotalLength() * 0.5f;
        m_thief->setRouteDistance(thiefStartDistance);
        m_thief->setDirection(1);
        m_thief->setPosition(m_route.getPositionAt(thiefStartDistance));
    }

    applyDifficulty(m_currentDifficulty);

    m_cursorTimer = 0.f;
    m_cursorVisible = true;
}

void Game::drawGameResultOverlay() {
    if (!m_fontLoaded || m_gameState == GameState::Playing) {
        return;
    }

    sf::RectangleShape overlay;
    overlay.setSize({520.f, 220.f});
    overlay.setPosition({190.f, 190.f});
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    overlay.setOutlineThickness(2.f);
    overlay.setOutlineColor(sf::Color(220, 220, 220));

    m_window.draw(overlay);

    if (m_gameState == GameState::Win) {
        drawText(
            "You Win!",
            {360.f, 230.f},
            42,
            sf::Color(100, 240, 140)
        );

        drawText(
            "The thief has been caught.",
            {275.f, 295.f},
            24,
            sf::Color::White
        );
    } else if (m_gameState == GameState::Lose) {
        drawText(
            "Game Over",
            {330.f, 230.f},
            42,
            sf::Color(240, 100, 100)
        );

        drawText(
            "Time is up. The thief escaped.",
            {250.f, 295.f},
            24,
            sf::Color::White
        );
    }

    drawText(
        "Press Enter to restart. Press Esc to quit.",
        {250.f, 355.f},
        22,
        sf::Color(220, 220, 220)
    );
}

void Game::startGame(const std::string& difficultyName) {
    try {
        applyDifficulty(difficultyName);
        resetGame();

        std::cout << "[TEST] Start game. Difficulty: "
                  << m_currentDifficulty
                  << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Failed to start game: "
                  << e.what()
                  << std::endl;

        m_gameState = GameState::Menu;
    }
}

void Game::drawMenuScreen() {
    if (!m_fontLoaded) {
        return;
    }

    sf::RectangleShape background;
    background.setSize({860.f, 570.f});
    background.setPosition({20.f, 40.f});
    background.setFillColor(sf::Color(32, 36, 42));
    background.setOutlineThickness(2.f);
    background.setOutlineColor(sf::Color(90, 95, 110));

    m_window.draw(background);

    drawText(
        "Police Catch Thief",
        {250.f, 110.f},
        46,
        sf::Color(100, 220, 255)
    );

    drawText(
        "Typing faster and more accurately makes the police run faster.",
        {135.f, 190.f},
        22,
        sf::Color(230, 230, 230)
    );

    drawText(
        "Choose difficulty",
        {330.f, 255.f},
        30,
        sf::Color::White
    );

    drawMenuButton(
        m_easyButton,
        "1. Easy",
        sf::Color(150, 255, 180)
    );

    drawMenuButton(
        m_mediumButton,
        "2. Medium",
        sf::Color(255, 230, 150)
    );

    drawMenuButton(
        m_hardButton,
        "3. Hard",
        sf::Color(255, 160, 160)
    );

    drawText(
        "Esc: Quit",
        {390.f, 525.f},
        20,
        sf::Color(190, 190, 190)
    );
}

void Game::initMenuButtons() {
    const sf::Vector2f buttonSize{220.f, 52.f};
    const float x = 340.f;

    m_easyButton.setSize(buttonSize);
    m_easyButton.setPosition({x, 315.f});
    m_easyButton.setFillColor(sf::Color(45, 80, 55));
    m_easyButton.setOutlineThickness(2.f);
    m_easyButton.setOutlineColor(sf::Color(120, 240, 150));

    m_mediumButton.setSize(buttonSize);
    m_mediumButton.setPosition({x, 380.f});
    m_mediumButton.setFillColor(sf::Color(85, 70, 35));
    m_mediumButton.setOutlineThickness(2.f);
    m_mediumButton.setOutlineColor(sf::Color(255, 220, 120));

    m_hardButton.setSize(buttonSize);
    m_hardButton.setPosition({x, 445.f});
    m_hardButton.setFillColor(sf::Color(90, 45, 45));
    m_hardButton.setOutlineThickness(2.f);
    m_hardButton.setOutlineColor(sf::Color(255, 130, 130));
}

//判断鼠标点到了哪个按钮
void Game::handleMenuClick(sf::Vector2f mousePosition) {
    if (m_easyButton.getGlobalBounds().contains(mousePosition)) {
        startGame("Easy");
    } else if (m_mediumButton.getGlobalBounds().contains(mousePosition)) {
        startGame("Medium");
    } else if (m_hardButton.getGlobalBounds().contains(mousePosition)) {
        startGame("Hard");
    }
}

void Game::drawMenuButton(
    const sf::RectangleShape& button,
    const std::string& label,
    sf::Color textColor
) {
    if (!m_fontLoaded) {
        return;
    }

    m_window.draw(button);

    sf::Text text(m_font);
    text.setString(label);
    text.setCharacterSize(24);
    text.setFillColor(textColor);

    sf::FloatRect textBounds = text.getLocalBounds();
    sf::Vector2f buttonPosition = button.getPosition();
    sf::Vector2f buttonSize = button.getSize();

    float textX = buttonPosition.x + (buttonSize.x - textBounds.size.x) / 2.f;
    float textY = buttonPosition.y + (buttonSize.y - textBounds.size.y) / 2.f - 5.f;

    text.setPosition({textX, textY});

    m_window.draw(text);
}

void Game::initDifficulties() {
    m_difficulties["Easy"] = DifficultyConfig{
        "Easy",
        90.f,
        75.f,
        95.f,
        42.f,
        "data/words_easy.txt"
    };

    m_difficulties["Medium"] = DifficultyConfig{
        "Medium",
        75.f,
        95.f,
        130.f,
        35.f,
        "data/words_medium.txt"
    };

    m_difficulties["Hard"] = DifficultyConfig{
        "Hard",
        60.f,
        125.f,
        165.f,
        28.f,
        "data/words_hard.txt"
    };

    std::cout << "[TEST] Difficulties initialized." << std::endl;
}

void Game::applyDifficulty(const std::string& difficultyName) {
    auto it = m_difficulties.find(difficultyName);

    if (it == m_difficulties.end()) {
        std::cerr << "[ERROR] Difficulty not found: "
                  << difficultyName
                  << std::endl;
        return;
    }

    const DifficultyConfig& config = it->second;

    m_currentDifficulty = config.name;
    m_timeLimit = config.timeLimit;
    m_remainingTime = config.timeLimit;
    m_captureDistance = config.captureDistance;

    m_typingManager.loadTextsFromFile(config.wordFile);

    if (m_thief) {
        m_thief->setSpeed(config.thiefSpeed);
        m_thief->setAlertDistance(config.alertDistance);
    }

    std::cout << "[TEST] Applied difficulty: "
              << config.name
              << " | Time: " << config.timeLimit
              << " | Thief speed: " << config.thiefSpeed
              << " | Alert distance: " << config.alertDistance
              << " | Capture distance: " << config.captureDistance
              << std::endl;
}