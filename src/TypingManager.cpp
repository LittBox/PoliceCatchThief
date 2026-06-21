#include "TypingManager.h"

#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>

TypingManager::TypingManager()
    : m_targetText(""),
      m_currentIndex(0),
      m_correctChars(0),
      m_totalKeyPresses(0),
      m_elapsedTime(0.f),
      m_started(false) {
}

void TypingManager::setTargetText(const std::string& text) {
    m_targetText = normalizeText(text);
    reset();
}

void TypingManager::loadTextsFromFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open text file: " + filename);
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();

    std::string processedText = normalizeText(buffer.str());

    if (processedText.empty()) {
        throw std::runtime_error("Text file is empty: " + filename);
    }

    m_targetText = processedText;
    reset();
}

std::string TypingManager::normalizeText(const std::string& rawText) const {
    std::string result;
    bool previousIsSpace = false;

    for (unsigned char ch : rawText) {
        if (std::isspace(ch)) {
            if (!previousIsSpace && !result.empty()) {
                result.push_back(' ');
                previousIsSpace = true;
            }
        } else {
            result.push_back(static_cast<char>(ch));
            previousIsSpace = false;
        }
    }

    if (!result.empty() && result.back() == ' ') {
        result.pop_back();
    }

    return result;
}

void TypingManager::reset() {
    m_currentIndex = 0;
    m_correctChars = 0;
    m_totalKeyPresses = 0;
    m_elapsedTime = 0.f;
    m_started = false;
}

void TypingManager::update(float dt) {
    if (m_started && !isFinished()) {
        m_elapsedTime += dt;
    }
}

void TypingManager::handleChar(char c) {
    if (m_targetText.empty() || isFinished()) {
        return;
    }

    m_started = true;
    m_totalKeyPresses++;

    if (c == m_targetText[m_currentIndex]) {
        m_currentIndex++;
        m_correctChars++;
    }
}

const std::string& TypingManager::getTargetText() const {
    return m_targetText;
}

std::string TypingManager::getTypedPart() const {
    return m_targetText.substr(0, m_currentIndex);
}

std::string TypingManager::getRemainingPart() const {
    return m_targetText.substr(m_currentIndex);
}

std::size_t TypingManager::getCurrentIndex() const {
    return m_currentIndex;
}

int TypingManager::getCorrectChars() const {
    return m_correctChars;
}

int TypingManager::getTotalKeyPresses() const {
    return m_totalKeyPresses;
}

float TypingManager::getCPM() const {
    if (m_elapsedTime <= 0.f) {
        return 0.f;
    }

    return static_cast<float>(m_correctChars) / m_elapsedTime * 60.f;
}

float TypingManager::getAccuracy() const {
    if (m_totalKeyPresses <= 0) {
        return 1.f;
    }

    return static_cast<float>(m_correctChars) /
           static_cast<float>(m_totalKeyPresses);
}

bool TypingManager::isFinished() const {
    return m_currentIndex >= m_targetText.size();
}