
游戏运行逻辑：
```Mermaid
flowchart TD
    A[main.cpp 程序入口] --> B[创建 Game 对象]
    B --> C[调用 Game::run]
    C --> D[进入游戏主循环]

    D --> E[processEvents 处理事件]
    E --> F{当前是否在菜单}
    F -- 是 --> G[鼠标或数字键选择难度]
    G --> H[startGame 开始游戏]
    H --> I[applyDifficulty 应用难度配置]
    I --> J[TypingManager 加载对应词库]
    J --> K[resetGame 初始化角色和时间]

    F -- 否 --> L[处理玩家键盘输入]
    L --> M[TypingManager 记录输入字符]

    K --> N[update 更新游戏逻辑]
    M --> N
    N --> O[计算 CPM 和 Accuracy]
    O --> P[Police 根据打字表现更新速度]
    P --> Q[遍历 m_gameObjects 调用 update]
    Q --> R[Route 根据路线距离更新坐标]
    R --> S[Thief 判断是否掉头]
    S --> T[checkGameEnd 判断胜负]

    T --> U{警察是否抓到小偷}
    U -- 是 --> V[进入 Win 状态]
    U -- 否 --> W{时间是否耗尽}
    W -- 是 --> X[进入 Lose 状态]
    W -- 否 --> Y[render 绘制画面]
    V --> Y
    X --> Y
    Y --> D
```

完整项目类图：
```Mermaid
classDiagram
    class Game {
        -RenderWindow m_window
        -Font m_font
        -Route m_route
        -shared_ptr~Police~ m_police
        -shared_ptr~Thief~ m_thief
        -vector~shared_ptr~GameObject~~ m_gameObjects
        -TypingManager m_typingManager
        -map~string,DifficultyConfig~ m_difficulties
        -string m_currentDifficulty
        -float m_timeLimit
        -float m_remainingTime
        -float m_captureDistance
        +Game()
        +run()
        -processEvents()
        -update(float dt)
        -render()
        -initCharacters()
        -resetGame()
        -startGame(string difficultyName)
        -initDifficulties()
        -applyDifficulty(string difficultyName)
        -checkGameEnd()
    }

    class GameObject {
        <<abstract>>
        +update(float dt)*
        +draw(RenderWindow& window) const*
    }

    class Character {
        #CircleShape m_shape
        #float m_speed
        #int m_direction
        #float m_routeDistance
        +moveAlongRoute(float dt)
        +setPosition(Vector2f)
        +getPosition() Vector2f
        +setSpeed(float)
        +getSpeed() float
        +setDirection(int)
        +getDirection() int
        +setRouteDistance(float)
        +getRouteDistance() float
    }

    class Police {
        +update(float dt)
        +updateSpeedByTyping(float cpm, float accuracy)
    }

    class Thief {
        -float m_alertDistance
        -float m_turnCooldown
        -float m_currentCooldown
        +update(float dt)
        +draw(RenderWindow& window) const
        +checkTurnAround(float policeRouteDistance, float routeLength) bool
        +setAlertDistance(float)
        +getAlertDistance() float
    }

    class TypingManager {
        -string m_targetText
        -size_t m_currentIndex
        -int m_correctChars
        -int m_totalKeyPresses
        -float m_elapsedTime
        -bool m_started
        +loadTextsFromFile(string filename)
        +reset()
        +update(float dt)
        +handleChar(char c)
        +getCPM() float
        +getAccuracy() float
        +isFinished() bool
    }

    class Route {
        -vector~Vector2f~ m_points
        -float m_totalLength
        +draw(RenderWindow& window) const
        +getTotalLength() float
        +getPositionAt(float distance) Vector2f
    }

    class DifficultyConfig {
        +string name
        +float timeLimit
        +float thiefSpeed
        +float alertDistance
        +float captureDistance
        +string wordFile
    }

    GameObject <|-- Character
    Character <|-- Police
    Character <|-- Thief

    Game --> Route
    Game --> Police
    Game --> Thief
    Game --> TypingManager
    Game --> DifficultyConfig
    Game --> GameObject
```