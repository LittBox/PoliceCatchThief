#pragma once

#include <cstddef>
#include <string>

class TypingManager {
public:
    TypingManager();

    void setTargetText(const std::string& text);
    void loadTextsFromFile(const std::string& filename);

    void reset();

    void update(float dt);
    void handleChar(char c);

    const std::string& getTargetText() const;
    std::string getTypedPart() const;
    std::string getRemainingPart() const;

    std::size_t getCurrentIndex() const;
    int getCorrectChars() const;
    int getTotalKeyPresses() const;

    float getCPM() const;
    float getAccuracy() const;

    bool isFinished() const;

private:
    std::string normalizeText(const std::string& rawText) const;

private:
    std::string m_targetText;
    std::size_t m_currentIndex;

    int m_correctChars;
    int m_totalKeyPresses;

    float m_elapsedTime;
    bool m_started;
};