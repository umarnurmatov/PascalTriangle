#pragma once
#include <vector>
#include <functional>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class PTriangle
{
    // Hexagon cell of triangle. Constists of hexagon and value
    struct PTriangleCell
    {
        int val;
        sf::ConvexShape hex;
        sf::Text text;
        PTriangleCell(int val, float size, sf::Font& font);
    };

    std::vector<std::vector<PTriangleCell>> drawbleTriangle;
    sf::Font font;
    const float hexSize{ 50.f };

public:
    struct ColorSetting
    {
        const char* label;
        std::function<bool(int)> checkFcn;
        sf::Color color;
        bool active;
        float colorNum[3];
        ColorSetting(const char* label, std::function<bool(int)> checkFcn, sf::Color color, bool active) :
            label{ label }, checkFcn{ checkFcn }, color{ color }, active{ active }
        {
            colorNum[0] = static_cast<float>(color.r) / 255.f;
            colorNum[1] = static_cast<float>(color.g) / 255.f;
            colorNum[2] = static_cast<float>(color.b) / 255.f;
        }
    };

    void generate(float startWidth, int rows);
    void regenerate(float startWidth, int rows);
    void draw(sf::RenderWindow& window, bool& showValues, std::vector<ColorSetting> settings) const;
    sf::Vector2f getBounds() const;
};