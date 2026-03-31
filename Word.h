#pragma once

#include <string>
#include <sfml/graphics.hpp>
#include "UserText.h"

class Word {
    std::string word;
    sf::Text text;
    int verticalLimit;
    int horizontalLimit;
    bool expired;
    bool user_typed;

    static int last_pos;
public:

    Word(const std::string &word, const sf::Font &f, const int &size, const int &width, const int &height);

    Word(const std::string &w, const sf::Font &f, const int &size, const int &width, const int &height,
         const int &xpos, const int &ypos);

    auto update(const int &speed, const sf::Clock &clock, UserText &userWord, sf::RenderTarget &window) -> void;

    auto display(sf::RenderTarget &window) -> void;

    auto getExpired() -> bool;

    auto getUserTyped() -> bool;

    auto getWord() -> std::string;

    auto getTextPos() -> std::pair<int, int>;

    ~Word();
};