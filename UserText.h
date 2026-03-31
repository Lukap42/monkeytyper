#pragma once
#include <string>
#include <sfml/graphics.hpp>

class UserText{
    std::string word;
    sf::Text text;
    int fontSize;
    int horizontalLimit;
    int verticalLimit;

public:
    UserText(const int &width,const int& height, const sf::Font& f, const int& fs);
    auto display(sf::RenderTarget& window) -> void;
    auto onEvent(const sf::Event& event) -> void;
    auto getWord() -> std::string;
    auto clear() -> void;

};