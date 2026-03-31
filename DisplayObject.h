#pragma once
#include <sfml/graphics.hpp>

class DisplayObject {
    sf::RectangleShape drawableObject;
    sf::Text textObject;

public:
    DisplayObject(const int &sizeX, const int &sizeY, const std::string &text, const sf::Font &font,
                  const int &size);

    auto setTextPos(const int& w, const int& h) -> void;

    auto display (sf::RenderTarget& window) -> void;

    auto setTextSize(const int& n) -> void;
    auto setTextFont(const sf::Font& font) -> void;

    auto getText() -> sf::Text&;
};