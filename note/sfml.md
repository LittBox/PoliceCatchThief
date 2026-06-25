# PoliceCatchThief 项目中的 SFML 类型笔记

## 1. 总览

本项目中涉及到的 SFML 类型主要集中在以下几类：

| 类型类别 | 相关 SFML 类型 | 主要用途 |
|---|---|---|
| 窗口与显示 | `sf::RenderWindow`、`sf::VideoMode` | 创建窗口、刷新画面、绘制对象 |
| 坐标与区域 | `sf::Vector2f`、`sf::Vector2u`、`sf::FloatRect` | 表示位置、尺寸、边界范围 |
| 图形对象 | `sf::RectangleShape`、`sf::CircleShape` | 绘制矩形界面、角色图形 |
| 颜色与文字 | `sf::Color`、`sf::Font`、`sf::Text` | 设置颜色、加载字体、显示文字 |
| 事件输入 | `sf::Event`、`sf::Mouse`、`sf::Keyboard` | 处理窗口事件、鼠标点击、键盘输入 |
| 时间控制 | `sf::Clock` | 计算帧间隔时间，控制游戏更新 |

这些类型共同构成了项目的图形界面、事件响应、角色显示和游戏循环基础。

---

## 2. `sf::RenderWindow`

### 2.1 作用

`sf::RenderWindow` 表示一个可以显示图形内容的窗口。

在本项目中，它是主窗口对象：

```cpp
sf::RenderWindow m_window;
```

在 `Game` 构造函数中创建窗口：

```cpp
m_window(
    sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
    "Police Catch Thief"
)
```

它主要负责：

- 创建游戏窗口；
- 接收窗口事件；
- 清空窗口画面；
- 绘制图形对象；
- 显示最终画面。

### 2.2 常用方法

```cpp
m_window.isOpen();              // 判断窗口是否仍然打开
m_window.close();               // 关闭窗口
m_window.clear(color);          // 清空窗口，并设置背景色
m_window.draw(object);          // 绘制对象
m_window.display();             // 显示当前帧画面
m_window.pollEvent();           // 读取窗口事件
m_window.setFramerateLimit(60); // 设置帧率上限
m_window.mapPixelToCoords(...); // 将鼠标像素坐标转换为窗口坐标
```

### 2.3 项目中的典型用法

```cpp
while (m_window.isOpen()) {
    processEvents();
    update(dt);
    render();
}
```

### 2.4 汇报时可以这样说

> `sf::RenderWindow` 是 SFML 中的窗口类，本项目使用它创建游戏窗口、接收输入事件，并在每一帧中绘制路线、角色、文字和界面组件。

---

## 3. `sf::VideoMode`

### 3.1 作用

`sf::VideoMode` 用来描述窗口大小。

项目中的代码：

```cpp
sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT})
```

其中：

```cpp
WINDOW_WIDTH = 900;
WINDOW_HEIGHT = 650;
```

表示创建一个 `900 × 650` 的游戏窗口。

### 3.2 常见含义

`sf::VideoMode({width, height})` 中：

| 参数 | 含义 |
|---|---|
| `width` | 窗口宽度 |
| `height` | 窗口高度 |

### 3.3 汇报时可以这样说

> `sf::VideoMode` 用来设置窗口大小，本项目窗口大小为 900×650。

---

## 4. `sf::Vector2f`

### 4.1 作用

`sf::Vector2f` 是二维浮点向量，可以理解为一个包含 `x` 和 `y` 的坐标结构。

可以简单理解为：

```cpp
struct Vector2f {
    float x;
    float y;
};
```

它常用于表示：

- 坐标位置；
- 图形尺寸；
- 移动方向；
- 路线点；
- 鼠标转换后的窗口坐标。

### 4.2 项目中的使用示例

```cpp
sf::Vector2f getPosition() const;
void setPosition(const sf::Vector2f& position);

std::vector<sf::Vector2f> m_points;

sf::Vector2f mousePosition;
```

### 4.3 字段

```cpp
position.x
position.y
```

例如：

```cpp
sf::Vector2f pos{100.f, 200.f};
```

表示：

```text
x = 100
y = 200
```

### 4.4 常见操作

```cpp
sf::Vector2f a{100.f, 200.f};
sf::Vector2f b{300.f, 250.f};

float dx = b.x - a.x;
float dy = b.y - a.y;
```

在项目中，可用于计算警察和小偷之间的距离：

```cpp
float dx = policePosition.x - thiefPosition.x;
float dy = policePosition.y - thiefPosition.y;

return std::sqrt(dx * dx + dy * dy);
```

### 4.5 汇报时可以这样说

> `sf::Vector2f` 用来表示二维坐标，包含 `x` 和 `y` 两个浮点值。本项目中它主要用于角色位置、路线点、按钮位置和鼠标坐标。

---

## 5. `sf::Color`

### 5.1 作用

`sf::Color` 用来表示颜色。

本项目中它用于设置：

- 背景色；
- 按钮颜色；
- 文字颜色；
- 边框颜色；
- 角色颜色。

### 5.2 使用示例

```cpp
sf::Color::White
sf::Color(25, 25, 30)
sf::Color(0, 0, 0, 130)
```

### 5.3 字段含义

#### RGB 颜色

```cpp
sf::Color(25, 25, 30)
```

表示：

| 字段 | 含义 | 数值 |
|---|---|---|
| `R` | 红色分量 | 25 |
| `G` | 绿色分量 | 25 |
| `B` | 蓝色分量 | 30 |

#### RGBA 颜色

```cpp
sf::Color(0, 0, 0, 130)
```

表示：

| 字段 | 含义 | 数值 |
|---|---|---|
| `R` | 红色分量 | 0 |
| `G` | 绿色分量 | 0 |
| `B` | 蓝色分量 | 0 |
| `A` | 透明度 | 130 |

其中 `A` 表示透明度：

| A 值 | 含义 |
|---|---|
| `0` | 完全透明 |
| `255` | 完全不透明 |

### 5.4 常用颜色

```cpp
sf::Color::White
sf::Color::Black
sf::Color::Red
sf::Color::Green
sf::Color::Blue
sf::Color(r, g, b)
sf::Color(r, g, b, a)
```

### 5.5 汇报时可以这样说

> `sf::Color` 用于设置界面元素颜色，例如背景、文字、按钮和角色颜色。第四个参数表示透明度。

---

## 6. `sf::RectangleShape`

### 6.1 作用

`sf::RectangleShape` 表示矩形图形。

本项目中它用于绘制：

- 游戏区域；
- 输入面板；
- 信息卡片；
- 帮助卡片；
- 菜单按钮；
- 结果弹窗；
- 输入光标。

### 6.2 项目中的使用示例

```cpp
sf::RectangleShape m_gameArea;
sf::RectangleShape m_inputPanel;
sf::RectangleShape m_easyButton;
```

### 6.3 常用方法

```cpp
setSize({width, height});       // 设置矩形大小
setPosition({x, y});            // 设置位置
setFillColor(color);            // 设置填充色
setOutlineThickness(value);     // 设置边框粗细
setOutlineColor(color);         // 设置边框颜色
getPosition();                  // 获取位置
getSize();                      // 获取尺寸
getGlobalBounds();              // 获取边界范围
```

### 6.4 项目中的典型用法

用于判断鼠标是否点击到按钮：

```cpp
if (m_easyButton.getGlobalBounds().contains(mousePosition)) {
    startGame("Easy");
}
```

### 6.5 汇报时可以这样说

> `sf::RectangleShape` 用来绘制矩形界面元素。本项目中的菜单按钮、输入区域、信息面板和结果提示框都是通过矩形实现的。

---

## 7. `sf::CircleShape`

### 7.1 作用

`sf::CircleShape` 表示圆形图形。

在本项目中，`Character` 类中封装了圆形对象：

```cpp
sf::CircleShape m_shape;
```

因此，警察和小偷的图形外观可以基于圆形绘制。

### 7.2 常用方法

```cpp
setRadius(radius);          // 设置半径
setFillColor(color);        // 设置填充颜色
setPosition({x, y});        // 设置位置
getPosition();              // 获取位置
setOrigin({x, y});          // 设置原点
setOutlineThickness(value); // 设置边框粗细
setOutlineColor(color);     // 设置边框颜色
```

### 7.3 汇报时可以这样说

> `sf::CircleShape` 用来表示角色图形。我在 `Character` 类中封装了圆形对象，使 `Police` 和 `Thief` 可以继承角色的基础绘制能力。

---

## 8. `sf::Font`

### 8.1 作用

`sf::Font` 表示字体资源。

项目中定义了字体对象：

```cpp
sf::Font m_font;
```

加载字体文件：

```cpp
m_font.openFromFile("assets/font.ttf");
```

### 8.2 常用方法

```cpp
openFromFile("assets/font.ttf"); // 从文件加载字体
```

### 8.3 项目中的处理逻辑

```cpp
if (m_font.openFromFile("assets/font.ttf")) {
    m_fontLoaded = true;
} else {
    std::cerr << "[ERROR] Failed to load font: assets/font.ttf" << std::endl;
}
```

### 8.4 汇报时可以这样说

> `sf::Font` 用于加载字体文件。本项目从 `assets/font.ttf` 加载字体，用于菜单、状态信息和输入文本显示。

---

## 9. `sf::Text`

### 9.1 作用

`sf::Text` 用来显示文字。

本项目中它用于：

- 显示菜单文字；
- 显示难度、CPM、正确率、剩余时间；
- 显示帮助提示；
- 显示输入文本；
- 显示胜负结果。

### 9.2 项目中的典型用法

在 `drawText()` 中封装文字绘制：

```cpp
sf::Text text(m_font);

text.setString(content);
text.setCharacterSize(size);
text.setFillColor(color);
text.setPosition(position);

m_window.draw(text);
```

### 9.3 常用方法

```cpp
setString("text");              // 设置显示内容
setCharacterSize(24);           // 设置字号
setFillColor(sf::Color::White); // 设置颜色
setPosition({x, y});            // 设置位置
getLocalBounds();               // 获取文字局部边界
findCharacterPos(index);        // 获取某个字符的位置
```

### 9.4 项目中的高级用法

```cpp
measureText.findCharacterPos(i).x
```

它用于计算每个字符的位置，从而实现：

- 输入文本滚动；
- 光标位置显示；
- 当前输入位置定位。

### 9.5 汇报时可以这样说

> `sf::Text` 用于显示菜单文字、状态信息和输入文本。本项目还使用 `findCharacterPos()` 计算字符坐标，实现输入光标和文本滚动效果。

---

## 10. `sf::Event`

### 10.1 作用

`sf::Event` 表示窗口事件。

常见事件包括：

- 关闭窗口；
- 鼠标点击；
- 键盘按下；
- 文本输入。

### 10.2 项目中的事件循环

```cpp
while (auto event = m_window.pollEvent()) {
    ...
}
```

### 10.3 项目中用到的事件类型

#### 关闭窗口

```cpp
event->is<sf::Event::Closed>();
```

#### 鼠标点击

```cpp
event->is<sf::Event::MouseButtonPressed>();
```

取出鼠标事件：

```cpp
const auto* mouseEvent =
    event->getIf<sf::Event::MouseButtonPressed>();
```

#### 键盘按下

```cpp
event->is<sf::Event::KeyPressed>();
```

项目中用于检测：

- `Escape`
- `F2`
- `Tab`
- `Enter`

#### 文本输入

```cpp
event->is<sf::Event::TextEntered>();
```

用于接收玩家输入的字符：

```cpp
char32_t unicode = textEvent->unicode;
```

### 10.4 汇报时可以这样说

> `sf::Event` 用来处理用户输入和窗口行为。本项目中使用它处理窗口关闭、鼠标点击菜单按钮、快捷键控制和文本输入。

---

## 11. `sf::Mouse`

### 11.1 作用

`sf::Mouse` 用来表示鼠标相关信息。

### 11.2 项目中的使用示例

```cpp
mouseEvent->button == sf::Mouse::Button::Left
```

表示判断用户是否点击了鼠标左键。

然后通过：

```cpp
m_window.mapPixelToCoords(mouseEvent->position)
```

将鼠标点击的像素坐标转换为窗口坐标，再判断是否点击到按钮。

### 11.3 汇报时可以这样说

> `sf::Mouse` 用于判断鼠标按钮。本项目中主要用于菜单界面点击 `Easy`、`Medium`、`Hard` 三个难度按钮。

---

## 12. `sf::Keyboard`

### 12.1 作用

`sf::Keyboard` 用来表示键盘按键。

本项目使用的是 SFML 3 的扫描码写法：

```cpp
sf::Keyboard::Scancode::Escape
sf::Keyboard::Scancode::F2
sf::Keyboard::Scancode::Tab
sf::Keyboard::Scancode::Enter
```

### 12.2 按键功能对应表

| 按键 | 功能 |
|---|---|
| `Escape` | 退出游戏 |
| `F2` | 返回菜单 |
| `Tab` | 重置输入 |
| `Enter` | 重新开始游戏 |

### 12.3 汇报时可以这样说

> `sf::Keyboard` 用于处理功能键输入，例如 `Esc` 退出、`F2` 返回菜单、`Tab` 重置输入、`Enter` 重新开始。

---

## 13. `sf::Clock`

### 13.1 作用

`sf::Clock` 是计时器，用来计算每一帧经过的时间。

### 13.2 项目中的典型用法

```cpp
sf::Clock clock;

while (m_window.isOpen()) {
    float dt = clock.restart().asSeconds();

    processEvents();
    update(dt);
    render();
}
```

其中，`dt` 表示上一帧到当前帧之间经过的秒数。

### 13.3 常用方法

```cpp
restart();   // 重启计时器，并返回上一次计时经过的时间
asSeconds(); // 将时间转换成秒
```

### 13.4 汇报时可以这样说

> `sf::Clock` 用于计算帧间隔时间 `dt`，这样角色移动可以根据真实时间更新，而不是依赖循环次数。

---

## 14. `sf::FloatRect`

### 14.1 作用

`sf::FloatRect` 表示一个矩形边界区域。

### 14.2 项目中的使用示例

```cpp
sf::FloatRect textBounds = text.getLocalBounds();
```

以及：

```cpp
m_easyButton.getGlobalBounds().contains(mousePosition);
```

### 14.3 常见含义

`sf::FloatRect` 通常表示：

- 左上角位置；
- 宽度；
- 高度；
- 边界范围。

### 14.4 常用方法

```cpp
contains(mousePosition); // 判断某个点是否在矩形区域内
```

### 14.5 汇报时可以这样说

> `sf::FloatRect` 用于表示对象的边界范围。本项目中用它判断鼠标点击是否落在按钮区域内，也用于计算文字居中位置。

---

## 15. `sf::Vector2u`

### 15.1 作用

项目代码中没有直接写出 `sf::Vector2u`，但 `sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT})` 这类窗口尺寸参数本质上属于无符号整数尺寸语义。

### 15.2 与 `sf::Vector2f` 的区别

| 类型 | 字段类型 | 常见用途 |
|---|---|---|
| `sf::Vector2f` | `float x, y` | 位置、移动、绘图坐标 |
| `sf::Vector2u` | `unsigned int x, y` | 窗口大小、纹理大小、像素尺寸 |

### 15.3 汇报时可以这样说

> `sf::Vector2f` 更常用于角色位置和绘图坐标，而 `sf::Vector2u` 更常用于窗口大小、纹理大小等像素尺寸。

---

## 16. SFML 类型速查表

| SFML 类型 | 在项目中的作用 | 核心字段/参数 | 常用方法 |
|---|---|---|---|
| `sf::RenderWindow` | 游戏窗口 | 窗口尺寸、标题 | `isOpen()`、`close()`、`clear()`、`draw()`、`display()`、`pollEvent()` |
| `sf::VideoMode` | 设置窗口大小 | `{width, height}` | 常用于窗口构造 |
| `sf::Vector2f` | 二维坐标、尺寸、方向 | `x`、`y` | 直接访问 `.x`、`.y` |
| `sf::Color` | 颜色 | `r`、`g`、`b`、`a` | `sf::Color::White`、`sf::Color(r,g,b,a)` |
| `sf::RectangleShape` | 矩形 UI | 位置、大小、颜色 | `setSize()`、`setPosition()`、`setFillColor()`、`getGlobalBounds()` |
| `sf::CircleShape` | 角色图形 | 半径、位置、颜色 | `setRadius()`、`setPosition()`、`setFillColor()` |
| `sf::Font` | 字体资源 | 字体文件 | `openFromFile()` |
| `sf::Text` | 文字显示 | 内容、字号、颜色、位置 | `setString()`、`setCharacterSize()`、`setFillColor()`、`findCharacterPos()` |
| `sf::Event` | 事件处理 | 事件类型 | `is<>()`、`getIf<>()` |
| `sf::Mouse` | 鼠标输入 | 鼠标按钮、位置 | `Button::Left` |
| `sf::Keyboard` | 键盘输入 | 扫描码 | `Scancode::Escape`、`Scancode::Tab` |
| `sf::Clock` | 计时 | 时间间隔 | `restart()`、`asSeconds()` |
| `sf::FloatRect` | 边界框 | 矩形范围 | `contains()` |

---

## 17. 汇报时的总结话术

> 本项目使用 SFML 作为图形和输入处理库。`sf::RenderWindow` 用来创建游戏窗口并完成绘制；`sf::RectangleShape` 用来绘制菜单按钮、输入面板和提示框；`sf::CircleShape` 用来表示警察和小偷等角色；`sf::Text` 和 `sf::Font` 用来显示文字和加载字体；`sf::Event` 用来处理窗口关闭、鼠标点击、键盘控制和文本输入；`sf::Clock` 用来计算每一帧的时间差，使角色移动和倒计时能够按照真实时间变化。`sf::Vector2f` 则贯穿整个项目，用于表示角色坐标、路线点、按钮位置和界面元素尺寸。

---

## 18. 重点记忆版

如果时间紧，优先记住下面这几句：

1. `sf::RenderWindow`：创建窗口、绘制画面、接收事件。
2. `sf::Vector2f`：表示二维坐标，核心字段是 `x` 和 `y`。
3. `sf::RectangleShape`：绘制矩形按钮、输入框、信息面板。
4. `sf::CircleShape`：绘制角色图形。
5. `sf::Text` + `sf::Font`：加载字体并显示文字。
6. `sf::Event`：处理窗口关闭、鼠标点击、键盘输入和文本输入。
7. `sf::Clock`：计算每一帧的时间差 `dt`，让移动和倒计时更稳定。