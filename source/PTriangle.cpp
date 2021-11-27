#include "pTriangle.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include <iostream>

PTriangle::PTriangleCell::PTriangleCell(int val, float size, sf::Font& font)
{
    this->val = val;
    float halfSize{ size / 2 };

    hex.setPointCount(6);
    hex.setPoint(0, sf::Vector2f(0.f, -halfSize));
    hex.setPoint(1, sf::Vector2f(halfSize * static_cast<float>(sqrt(3)) / 2, -0.5 * halfSize));
    hex.setPoint(2, sf::Vector2f(halfSize * static_cast<float>(sqrt(3)) / 2, 0.5 * halfSize));
    hex.setPoint(3, sf::Vector2f(0.f, halfSize));
    hex.setPoint(4, sf::Vector2f(-halfSize * static_cast<float>(sqrt(3)) / 2, 0.5 * halfSize));
    hex.setPoint(5, sf::Vector2f(-halfSize * static_cast<float>(sqrt(3)) / 2, -0.5 * halfSize));
    hex.setOutlineThickness(2);
    hex.setOutlineColor(sf::Color::Black);

    text.setFont(font);
    text.setStyle(sf::Text::Regular);
    text.setFillColor(sf::Color::Black);
    text.setCharacterSize(size / 4);
    text.setString(std::to_string(val));
    text.setOrigin(text.getGlobalBounds().width * .5, text.getGlobalBounds().height * .5);
}

void PTriangle::generate(float startWidth, int rows)
{
    font.loadFromFile("resources\\arial.ttf");
    for (int i = 0; i < rows; i++)
    {
        drawbleTriangle.push_back(std::vector<PTriangleCell>());
        for (int j = 0; j <= i; j++)
        {
            if (j == 0 || j == i)
                drawbleTriangle[i].push_back(PTriangleCell(1, hexSize, font));
            else
                drawbleTriangle[i].push_back(PTriangleCell(drawbleTriangle[i - 1][j - 1].val + drawbleTriangle[i - 1][j].val, hexSize, font));
        }
    }

    float halfHexSize{ hexSize / 2 };
    for (int i = 0; i < drawbleTriangle.size(); i++)
    {
        for (int j = 0; j < drawbleTriangle[i].size(); j++)
        {
            if (i == 0)
            {
                drawbleTriangle[i][j].hex.setPosition(startWidth / 2, halfHexSize);
                drawbleTriangle[i][j].text.setPosition(startWidth / 2, halfHexSize);
            }
            else if (j == 0)
            {
                drawbleTriangle[i][j].hex.setPosition(
                    drawbleTriangle[i - 1][j].hex.getPosition().x - halfHexSize * sqrt(3) / 2.f,
                    drawbleTriangle[i - 1][j].hex.getPosition().y + halfHexSize * 1.5f
                );
                drawbleTriangle[i][j].text.setPosition(
                    drawbleTriangle[i - 1][j].text.getPosition().x - halfHexSize * sqrt(3) / 2.f,
                    drawbleTriangle[i - 1][j].text.getPosition().y + halfHexSize * 1.5f
                );
            }
            else
            {
                drawbleTriangle[i][j].hex.setPosition(
                    drawbleTriangle[i][j - 1].hex.getPosition().x + halfHexSize * sqrt(3),
                    drawbleTriangle[i][j - 1].hex.getPosition().y
                );
                drawbleTriangle[i][j].text.setPosition(
                    drawbleTriangle[i][j - 1].text.getPosition().x + halfHexSize * sqrt(3),
                    drawbleTriangle[i][j - 1].text.getPosition().y
                );
            }
        }
    }
}

void PTriangle::regenerate(float startWidth, int rows)
{
    drawbleTriangle.clear();
    generate(startWidth, rows);
}

void PTriangle::draw(sf::RenderWindow& window, bool& showValues, std::vector<ColorSetting> settings) const
{
    sf::Vector2f cViewPos{ window.getView().getCenter() };
    sf::Vector2f cViewSize{ window.getView().getSize() };
    float halfVerHexSize{ hexSize / 2 };
    float halfHorHexSize{ hexSize * static_cast<float>(sqrt(3)) / 4 };
    for (std::vector<PTriangleCell> row : drawbleTriangle)
    {
        for (PTriangleCell cell : row)
        {
            if (cell.hex.getPosition().x + halfHorHexSize > cViewPos.x - cViewSize.x / 2 &&
                cell.hex.getPosition().x - halfHorHexSize < cViewPos.x + cViewSize.x / 2 &&
                cell.hex.getPosition().y - halfVerHexSize < cViewPos.y + cViewSize.y / 2 &&
                cell.hex.getPosition().y + halfVerHexSize > cViewPos.y - cViewSize.y / 2)
            {
                for (ColorSetting st : settings)
                    if (st.active && st.checkFcn(cell.val)) cell.hex.setFillColor(st.color);

                window.draw(cell.hex);
                if (showValues) window.draw(cell.text);
            }
        }
    }
}

sf::Vector2f PTriangle::getBounds() const
{
    if (!drawbleTriangle.size())
        return sf::Vector2f();

    return sf::Vector2f(hexSize * (static_cast<float>(sqrt(3)) / 2.f) * drawbleTriangle.size(), 0.75f * hexSize * drawbleTriangle.size() + 0.25f * hexSize);
}
