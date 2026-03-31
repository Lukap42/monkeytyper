#include <iostream>
#include <fstream>
#include <filesystem>
#include <SFML/graphics.hpp>
#include <vector>
#include <fmt/core.h>
#include "Word.h"
#include "UserText.h"
#include "GameState.h"
#include "DisplayObject.h"

void saveGame(const int &font_num, const int &speed, const int &size, const int &score, const int &lives,
              std::vector<Word> &words) {
    auto saveFile = std::fstream();
    saveFile.open("../assets/quicksave.txt", std::ios::out | std::ios::trunc);

    // 1 - cal, 0 - arial, 2 - helvetica

    saveFile << font_num << " " << speed << " " << size << " " << score << " " << lives << " " << words.size() << '\n';
    for (auto &e: words) {
        saveFile << e.getWord() << " " << e.getTextPos().first << " " << e.getTextPos().second << '\n';
    }
}

void loadGame(sf::Font &font, int &speed, int &size, int &score, int &lives, std::vector<Word> &words, GameState &state,
              sf::Font &callibri, sf::Font &arial, sf::Font &helvetica, const int& screenW, const int& screenH) {
    auto loadFile = std::fstream();
    if (std::filesystem::exists("../assets/quicksave.txt")) {
        loadFile.open("../assets/quicksave.txt", std::ios::in);
        state = GameState::Game;
        int font_num;
        loadFile >> font_num;
        switch (font_num) {
            case 0:
                font = arial;
                break;
            case 1:
                font = callibri;
                break;
            case 2:
                font = helvetica;
                break;
            default:
                font = arial;
                break;
        }

        loadFile >> speed;
        loadFile >> size;
        loadFile >> score;
        loadFile >> lives;

        auto wordCount = 0;
        loadFile >> wordCount;

        auto temp = std::vector<Word>();

        for (int i=0;i<wordCount;i++){
            auto text = std::string();
            auto x = int(0);
            auto y = int(0);
            loadFile >> text;
            loadFile >> x;
            loadFile >> y;

            temp.push_back(Word(text,font,size,screenW,screenH,x,y));
        }

        words = temp;
    }

}

void loadScores(std::vector<std::pair<int, std::string>> &scores) {
    auto scoreReader = std::fstream();
    scoreReader.open("../assets/scores.txt");
    for (int i = 0; i < 5; i++) {
        auto helper = std::string();
        scoreReader >> helper;
        scores[i].first = std::stoi(helper);
        scoreReader >> scores[i].second;
    }

    for (int i = 0; i < 5; i++) {
        std::cout << scores[i].first << " " << scores[i].second;
    }
}

void insertScore(std::vector<std::pair<int, std::string>> &scores, const int &newScore, const std::string &newName) {
    for (int i = 0; i < scores.size(); i++) {
        if (scores[i].first < newScore) {
            for (int j = scores.size() - 1; j > i; j--) {
                scores[j] = scores[j - 1];
            }
            scores[i].first = newScore;
            scores[i].second = newName;
            return;
        }
    }
    return;
}

auto createScoreboard(const std::vector<std::pair<int, std::string>> &scores, const int &winWidth, const int &winHeight,
                      sf::Font &font) {
    auto scoreboard_objects = std::vector<DisplayObject>();
    int score_helper = 0;
    for (const auto &e: scores) {
        scoreboard_objects.push_back(
                DisplayObject(0, 0, "score: " + std::to_string(e.first) + " by: " + e.second, font, 30));
        scoreboard_objects[score_helper].setTextPos(0.5f * winWidth, winHeight * (0.3f + (0.1f * score_helper)));
        score_helper++;
    }
    return scoreboard_objects;
}

auto updateScoreFile(const std::vector<std::pair<int, std::string>> &scores) {
    auto filler = std::fstream();
    filler.open("../assets/scores.txt", std::ios::out | std::ios::trunc);
    for (const auto &e: scores) {
        filler << std::to_string(e.first) << " " << e.second << "\n";
    }
}

int main() {
    auto windowWidth = int(1600);
    auto windowHeight = int(1200);
    auto font = sf::Font();
    if (!font.loadFromFile("../assets/calibri.ttf")) fmt::println("font not found");

    auto arial = sf::Font();
    if (!arial.loadFromFile("../assets/arial.ttf")) fmt::println("arial not found");
    auto calibri = sf::Font();
    if (!calibri.loadFromFile("../assets/calibri.ttf")) fmt::println("calibri not found");
    auto helvetica = sf::Font();
    if (!helvetica.loadFromFile("../assets/helvetica.ttf")) fmt::println("helvetica not found");

    //Menu related variables and objects

    auto menu = DisplayObject(windowWidth, windowHeight, "Main Menu", font, 100);
    menu.setTextPos(windowWidth * 0.5f, windowHeight * 0.5f);

    auto menu_options = std::vector<DisplayObject>();
    menu_options.push_back(DisplayObject(0, 0, "Start Game", font, 20));
    menu_options.push_back(DisplayObject(0, 0, "Scoreboard", font, 20));
    menu_options.push_back(DisplayObject(0, 0, "Settings", font, 20));
    for (int i = 0; i < menu_options.size(); i++) {
        menu_options[i].setTextPos(windowWidth * 0.5f, windowHeight * (0.65f + i * 0.1f));
    }

    auto menu_position = int(50);

    auto state = GameState::Menu;

    //Setting related object creation

    auto settings = DisplayObject(windowWidth, windowHeight, "Settings", font, 100);
    settings.setTextPos(windowWidth * 0.5f, windowHeight * 0.1f);

    auto setting_options = std::vector<DisplayObject>();
    setting_options.push_back(DisplayObject(0, 0, "Arial", font, 20));
    setting_options.push_back(DisplayObject(0, 0, "Calibri", font, 20));
    setting_options.push_back(DisplayObject(0, 0, "Helvetica", font, 20));
    setting_options.push_back(DisplayObject(0, 0, "Slow speed", font, 20));
    setting_options.push_back(DisplayObject(0, 0, "Medium speed", font, 20));
    setting_options.push_back(DisplayObject(0, 0, "High speed", font, 20));

    for (int i = 0; i < setting_options.size(); i++) {
        setting_options[i].setTextPos(windowWidth * (0.4f + 0.1f * (i % 3)), windowHeight * (0.5f + 0.1f * (i / 3)));
    }

    auto font_num = int(1);
    auto font_t = font;
    auto speed_t = int(100);
    auto size_t = int(20);

    auto sampleText = DisplayObject(0, 0, "Sample Text", font_t, size_t);
    sampleText.setTextPos(windowWidth * 0.5f, windowHeight * 0.3f);

    auto settings_position = int(10);

    /*auto cursor_s = sf::RectangleShape();
    cursor_s.setFillColor(sf::Color::Red);
    cursor_s.setSize(sf::Vector2f(20, 20));*/

    //Highscore related objects
    auto scoreVector = std::vector<std::pair<int, std::string>>(5);
    if (!std::filesystem::exists("../assets/scores.txt")) {
        std::cout << "creating new scorelist";
        auto filler = std::fstream();
        filler.open("../assets/scores.txt", std::ios::out | std::ios::trunc);
        for (int i = 0; i < 5; i++) {
            filler << "0 null\n";
        }
    }
    loadScores(scoreVector);

    auto scoreboard_objects = createScoreboard(scoreVector, windowWidth, windowHeight, font);

    //Postgame objects
    auto nameInput = UserText(windowWidth * 0.5f, windowHeight * 0.5f, font, 20);
    auto postgameText = sf::Text("Enter name", font, 20);
    postgameText.setPosition((windowWidth - postgameText.getGlobalBounds().width) * 0.5f, windowHeight * 0.1f);



    //General objects

    auto window = sf::RenderWindow(sf::VideoMode(windowWidth, windowHeight), "MonkeyTyper");
    window.setFramerateLimit(60);

    auto wordListReader = std::fstream();
    wordListReader.open("../assets/wordlist.txt");
    auto wordList = std::vector<std::string>();
    auto temp = std::string();
    while (wordListReader >> temp) {
        wordList.push_back(temp);
    }

    //game logic variables and objects
    auto clock = sf::Clock(); //used for counting time to generate text
    auto dt = float();

    auto size = int(24);
    auto speed = int(100);

    auto lives = int(0);

    auto userInput = UserText(windowWidth * 0.5f, windowHeight * 0.9f, font, 40); //user writing

    auto score = int(0);
    auto scoreDisplay = sf::Text();
    scoreDisplay.setString("score: " + std::to_string(score));
    scoreDisplay.setCharacterSize(size);
    scoreDisplay.setPosition(0, windowHeight - size * 1.5f);
    scoreDisplay.setFont(font);

    auto lifeDisplay = sf::Text();
    lifeDisplay.setString("lifes left: " + std::to_string(lives));
    lifeDisplay.setCharacterSize(size);
    lifeDisplay.setPosition(0, windowHeight - size * 2.5f);
    lifeDisplay.setFont(font);

    auto wordsOnScreen = std::vector<Word>();
    auto generateWord = [&wordList]() { return wordList[std::rand() % wordList.size()]; };

    while (window.isOpen()) {

        window.clear();

        auto event = sf::Event();

        if (state == GameState::Menu) {
            menu.display(window);
            for (auto e: menu_options) {
                e.display(window);
            }
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }

                if (event.type == sf::Event::KeyPressed) { //Loading
                    if(event.key.control==true && event.key.code == sf::Keyboard::L) {
                        clock.restart();
                        loadGame(font,speed,size,score, lives, wordsOnScreen, state, calibri, arial,helvetica, windowWidth,windowHeight);
                    }
                }

                if (event.type == sf::Event::MouseButtonPressed) { //Picking an option
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        bool hit = false;
                        auto click_pos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
                        for (int i = 0; i < menu_options.size(); i++) {
                            if (menu_options[i].getText().getGlobalBounds().contains(click_pos)) {
                                menu_position = i;
                                hit = true;
                            }
                        }
                        if (hit) { //if clicked on some option
                            switch (menu_position) {
                                case 0:
                                    clock.restart(); //restart word generation timer
                                    state = GameState::Game;
                                    lives = 3;
                                    break;
                                case 1:
                                    state = GameState::Scoreboard;
                                    break;
                                case 2:
                                    state = GameState::Settings;
                                    break;
                                default:
                                    state = GameState::Menu;
                                    break;
                            }
                        }
                    }
                }

            }
        }
        if (state == GameState::Scoreboard) {
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                if (event.type = sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        state = GameState::Menu;
                    }
                }
            }

            for (auto &e: scoreboard_objects) {
                e.display(window);
            }

        }
        if (state == GameState::Settings) {
            settings.display(window);
            for (auto e: setting_options) {
                e.display(window);
            }
            sampleText.display(window);


            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }

                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        auto click_pos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
                        for (int i = 0; i < setting_options.size(); i++) {
                            if (setting_options[i].getText().getGlobalBounds().contains(click_pos))
                                settings_position = i;
                        }
                        switch (settings_position) {
                            case 0:
                                font_num = 0;
                                font_t = arial;
                                break;
                            case 1:
                                font_num = 1;
                                font_t = calibri;
                                break;
                            case 2:
                                font_num = 2;
                                font_t = helvetica;
                                break;
                            case 3:
                                speed_t = 50;
                                break;
                            case 4:
                                speed_t = 100;
                                break;
                            case 5:
                                speed_t = 150;
                                break;
                            default:
                                break;
                        }
                        std::cout << settings_position << std::endl;
                        sampleText.setTextFont(font_t);
                    }
                }

                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        font = font_t;
                        size = size_t;
                        speed = speed_t;
                        state = GameState::Menu;
                    }
                    if (event.key.code == sf::Keyboard::Add) size_t += 4;
                    if (event.key.code == sf::Keyboard::Subtract) size_t -= 4;

                }
            }
            sampleText.setTextSize(size_t);

        }
        if (state == GameState::Game) {
            dt += static_cast<float>(clock.getElapsedTime().asMilliseconds()) / 1000.f;
            if (dt > 0.5) {
                auto word_t = Word(generateWord(), font, size, windowWidth, windowHeight);
                wordsOnScreen.push_back(word_t);
                dt -= 0.5;
            }

            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed) {
                    if(event.key.control==true && event.key.code == sf::Keyboard::S) {
                        std::cout << "Game saved";
                        saveGame(font_num, speed, size, score, lives, wordsOnScreen);
                    }
                    else if(event.key.control==true && event.key.code == sf::Keyboard::L) {
                        clock.restart();
                        loadGame(font,speed,size,score, lives, wordsOnScreen, state, calibri, arial,helvetica, windowWidth,windowHeight);
                    }
                    else
                        userInput.onEvent(event);
                }

            }

            for (auto iter = wordsOnScreen.begin(); iter != wordsOnScreen.end();) {
                iter->update(speed, clock, userInput, window); //clock works here as "time between frames"
                if (iter->getUserTyped()) { //if word is same as one types by user
                    score++;
                    fmt::println("rzuw");
                    iter = wordsOnScreen.erase(iter);
                } else if (iter->getExpired()) { //if word is past the screen
                    lives--;
                    fmt::println("rzuw1");
                    iter = wordsOnScreen.erase(iter);
                } else {
                    iter++;
                }
            }

            lifeDisplay.setString("lifes left: " + std::to_string(lives));
            scoreDisplay.setString("score: " + std::to_string(score));

            clock.restart();

            if (lives == 0) {
                state = GameState::Postgame;
                clock.restart();
                wordsOnScreen.clear();
                userInput.clear();
            }

            for (auto &e: wordsOnScreen) {
                e.display(window);
            }

            userInput.display(window);
            window.draw(scoreDisplay);
            window.draw(lifeDisplay);

        }

        if (state == GameState::Postgame) {
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Enter) {
                        auto name = nameInput.getWord();
                        nameInput.clear();
                        insertScore(scoreVector, score, name);
                        scoreboard_objects = createScoreboard(scoreVector, windowWidth, windowHeight, font);
                        updateScoreFile(scoreVector);
                        state = GameState::Menu;
                        score = 0;

                    } else {
                        nameInput.onEvent(event);
                    }
                }
            }

            window.draw(postgameText);
            nameInput.display(window);
        }

        window.display();

    }
}
