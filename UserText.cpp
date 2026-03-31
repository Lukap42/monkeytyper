#include "UserText.h"

UserText::UserText(const int &width,const int& height, const sf::Font& f, const int& fs) {
    horizontalLimit=width;
    verticalLimit = height;
    text=sf::Text();
    text.setFont(f);
    text.setString("_");
    text.setCharacterSize(fs);
    text.setPosition(sf::Vector2f(horizontalLimit,verticalLimit));
}

auto UserText::display(sf::RenderTarget &window) -> void {
    window.draw(text);
}

auto UserText::onEvent(const sf::Event &event) -> void {
    if(event.key.code == sf::Keyboard::Backspace){
        if(word.size()>0){
            auto word_t = std::string();
            for(int i=0;i<word.size()-1;i++){
                word_t+=word[i];
            }
            word=word_t;
        }
    }
    else if(event.key.code >= sf::Keyboard::A and event.key.code <= sf::Keyboard::Z){
        word+=event.key.code+'a';
    }

    else if(event.key.code == sf::Keyboard::Enter){
        word = "";
    }
    text.setString(word+"_");
    text.setPosition((horizontalLimit-text.getGlobalBounds().width*0.5f),verticalLimit);
}

auto UserText::getWord() -> std::string {
    return word;
}

auto UserText::clear() -> void {
    word = "";
    text.setString(word);
}


