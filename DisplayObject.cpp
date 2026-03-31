#include "DisplayObject.h"

DisplayObject::DisplayObject(const int &sizeX, const int &sizeY, const std::string &text,
                             const sf::Font &font, const int &size) {
    drawableObject = sf::RectangleShape();
    drawableObject.setSize(sf::Vector2f(sizeX, sizeY));
    drawableObject.setFillColor(sf::Color::Blue);
    textObject = sf::Text(text, font, size);
}

auto DisplayObject::setTextPos(const int &w, const int &h) -> void {
    textObject.setPosition(sf::Vector2f(w-textObject.getGlobalBounds().width*0.5f,h-textObject.getGlobalBounds().height*0.5f));
}

auto DisplayObject::display(sf::RenderTarget &window) -> void {
    window.draw(drawableObject);
    window.draw(textObject);
}

auto DisplayObject::setTextSize(const int &n) -> void {
    textObject.setCharacterSize(n);
}

auto DisplayObject::setTextFont(const sf::Font& font) -> void {
    textObject.setFont(font);
}

auto DisplayObject::getText() -> sf::Text& {
    return textObject;
}

