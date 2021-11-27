#include <vector>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include "pTriangle.h"
#include <iostream>

// Start window size
constexpr unsigned int startW{ 600 }, startH{ 800 };

// Keyboard processing
void processKeyboard(sf::View& view, sf::Vector2f bounds, float deltaMove, sf::Clock& deltaClock)
{
    float cDeltaMove = deltaMove * deltaClock.restart().asMilliseconds();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && view.getCenter().y - view.getSize().y / 2 > 0)
        view.move(sf::Vector2f(0, -cDeltaMove));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && view.getCenter().y + view.getSize().y / 2 < bounds.y)
        view.move(sf::Vector2f(0, cDeltaMove));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && view.getCenter().x - view.getSize().x / 2 > startW / 2 - bounds.x / 2)
        view.move(sf::Vector2f(-cDeltaMove, 0));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && view.getCenter().x + view.getSize().x / 2 < startW / 2 + bounds.x / 2)
        view.move(sf::Vector2f(cDeltaMove, 0));
}

// Checking functions (for colorizing)
namespace colorizeFcn
{
    bool isEven(int x)
    {
        if (x % 2 == 0) return true;
        else return false;
    }
    bool isOdd(int x)
    {
        if (x % 2 != 0) return true;
        else return false;
    }
    bool isMultipleOfThree(int x)
    {
        if (x % 3 == 0) return true;
        else return false;
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(startW, startH), "Pascal Triangle");
    ImGui::SFML::Init(window);
    ImGui::StyleColorsClassic();

    //Triangle Settings
    int rows{ 50 };
    float deltaMove{ 1.f };
    float viewScale{ 1.f };
    bool showValues{ true };

    // Generating triangle
    PTriangle triangle;
    triangle.generate(startW, rows);

    std::vector<PTriangle::ColorSetting> colorSettings // Coloring settings
    {
        {"Even", colorizeFcn::isEven, sf::Color::Blue, false},
        {"Odd", colorizeFcn::isOdd, sf::Color::Magenta, false},
        {"Multiple of 3", colorizeFcn::isMultipleOfThree, sf::Color::Green, false},
    };

    //Triangle [local] bounds
    sf::Vector2f bounds = triangle.getBounds();

    // View (for moving and for scaling)
    sf::View view = window.getDefaultView();
    
    // For scaling
    float unscaledSizeX = view.getSize().x, unscaledSizeY = view.getSize().y;

    sf::Clock deltaClockImGui, deltaClock;
    while (window.isOpen())
    {
        // Processing events
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::Resized) {
                view.setSize(                           // resize view
                    static_cast<float>(event.size.width),
                    static_cast<float>(event.size.height)
                );
                unscaledSizeX = view.getSize().x, unscaledSizeY = view.getSize().y;
                window.setView(view);
            }
        }

        ImGui::SFML::Update(window, deltaClockImGui.restart());

        // ImGui window
        ImGui::Begin("Settings");

        ImGui::Text("View settings:");
        ImGui::SliderFloat("Moving velocity", &deltaMove, 0.3f, 5.f, "%.1f");
        if (ImGui::SliderFloat("View scale", &viewScale, 0.01, 5, "%.2f")) // Zoom view, if slider have benn used
        {
            view.setSize(unscaledSizeX, unscaledSizeY);
            view.zoom(1 / viewScale);
        }

        ImGui::NewLine();

        ImGui::Text("Triangle settings:");
        ImGui::SliderInt("Rows number", &rows, 3, 300);
        if (ImGui::Button("Regenerate")) // Regenerate triangle with new rows num, if button have been pressed 
        {
            triangle.regenerate(startW, rows);
            bounds = triangle.getBounds();
        }

        ImGui::Separator();

        ImGui::Checkbox("Show values", &showValues); // Show / hide values

        ImGui::Separator();

        for (int i = 0; i < colorSettings.size(); i++) // Colors
        {
            ImGui::Checkbox(colorSettings[i].label, &colorSettings[i].active);
            ImGui::SameLine();
            if (ImGui::ColorEdit3(colorSettings[i].label, colorSettings[i].colorNum))
            {
                colorSettings[i].color.r = static_cast<sf::Uint8>(colorSettings[i].colorNum[0] * 255.f);
                colorSettings[i].color.g = static_cast<sf::Uint8>(colorSettings[i].colorNum[1] * 255.f);
                colorSettings[i].color.b = static_cast<sf::Uint8>(colorSettings[i].colorNum[2] * 255.f);
            }
        }
        ImGui::End();

        // Processing keyboard
        processKeyboard(view, bounds, deltaMove, deltaClock);

        // Applying changes
        window.setView(view);

        // Rendering and drawing
        window.clear();
        triangle.draw(window, showValues, colorSettings);
        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();

    return 0;
}