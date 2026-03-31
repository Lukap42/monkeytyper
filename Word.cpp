#include "Word.h"
#include <fmt/core.h>

int Word::last_pos = 0;

Word::Word(const std::string &w, const sf::Font &f, const int &size, const int &width, const int &height) { //usual constructor
    word = w;
    verticalLimit = height;
    horizontalLimit = width;
    text.setString(word);
    text.setFont(f);
    text.setCharacterSize(size);
    auto noRepeat = [&height, &size]() { //this lambda attempts to prevent overlap
        int n = rand() % ((height - size * 3) / size) * size;
        while(n==last_pos){
            n=rand() % (height / size) * size;
        }
        last_pos=n;
        return n;
    };
    text.setPosition(0, noRepeat());
    fmt::println("{} created, pos: {},{}, size {}", w, text.getPosition().x, text.getPosition().y,
                 text.getCharacterSize());
    expired = false;
    user_typed = false;
}

auto Word::update(const int &speed, const sf::Clock &clock, UserText& userWord, sf::RenderTarget& window) -> void {
    text.setPosition(text.getPosition() +
                     sf::Vector2f(speed * static_cast<float>(clock.getElapsedTime().asMilliseconds()) / 1000.f, 0));
    if (text.getPosition().x > 0.5 * horizontalLimit && text.getPosition().x < 0.75 * horizontalLimit)
        text.setFillColor(sf::Color::Yellow);
    if (text.getPosition().x > 0.75 * horizontalLimit && text.getPosition().x < horizontalLimit)
        text.setFillColor(sf::Color::Red);
    if (text.getPosition().x > horizontalLimit) {
        expired = true;
    }

    if (text.getString() == userWord.getWord()) {
        user_typed = true;
        userWord.clear();
    }

}

auto Word::display(sf::RenderTarget &window) -> void {
    window.draw(text);

}

Word::~Word() {
}

auto Word::getExpired() -> bool {
    return expired;
}

auto Word::getUserTyped() -> bool{
    return user_typed;
}

auto Word::getWord() -> std::string {
    return word;
}

auto Word::getTextPos() -> std::pair<int, int> {
    return {text.getPosition().x,text.getPosition().y};
}

Word::Word(const std::string &w, const sf::Font &f, const int &size, const int &width, const int &height,
           const int &xpos, const int &ypos) { //overload needed for loading the state of game from file
    word = w;
    verticalLimit = height;
    horizontalLimit = width;
    text.setString(word);
    text.setFont(f);
    text.setCharacterSize(size);
    text.setPosition(xpos, ypos);
    expired = false;
    user_typed = false;

}

