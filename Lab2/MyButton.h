#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

// Цвета
#define LIGHTBLUEPRESSED sf::Color(138, 165, 227)
#define LIGHTBLUERELEASED sf::Color(125, 156, 219)
#define LIGHTBLUE sf::Color(110, 133, 202)
#define BLUE sf::Color(83, 112, 229) 
#define DARKBLUE sf::Color(27, 63, 170)

#define LIGHTVIOLETPRESSED sf::Color(169, 138, 227)
#define LIGHTVIOLETRELEASED sf::Color(158, 125, 219)
#define LIGHTVIOLET sf::Color(142, 110, 202)
#define VIOLET sf::Color(133, 83, 229) 
#define DARKVIOLET sf::Color(75, 27, 170)

#define LIGHTGRAY sf::Color(127, 126, 130)
#define GRAY sf::Color(119, 118, 122)
#define DARKGRAY sf::Color(67, 66, 69)

class MyButton {
private:
  sf::Text text;
  sf::RectangleShape button;
  sf::RectangleShape buttonShadow;
  bool isPressed = false;
  bool isActive = false;
  bool isViolet = false;
public:
  MyButton() {}
  MyButton(sf::String t, sf::Vector2f size, bool active, bool pressed, bool violet) : isPressed{ pressed }, isViolet{ violet }, isActive{ active } {
    // Окрашивание кнопки в нужный цвет
    if (active) {
      if (violet) {
        text.setFillColor(DARKVIOLET);
        button.setFillColor(VIOLET);
        buttonShadow.setFillColor(DARKVIOLET);
      }
      else {
        text.setFillColor(DARKBLUE);
        button.setFillColor(BLUE);
        buttonShadow.setFillColor(DARKBLUE);
      }
    }
    else {
      text.setFillColor(DARKGRAY);
      button.setFillColor(GRAY);
      buttonShadow.setFillColor(DARKGRAY);
    }

    text.setString(t);
    text.setCharacterSize(14);

    button.setSize(size);
    buttonShadow.setSize({ size.x, 6 });
  }

  void setFont(sf::Font& font) {
    text.setFont(font);
  }
  void setBackColor(sf::Color color) {
    button.setFillColor(color);
  }
  void setTextColor(sf::Color color) {
    text.setFillColor(color);
  }
  void setPosition(sf::Vector2f pos) {
    button.setPosition(pos);
    float buttonY = button.getSize().y;
    buttonShadow.setPosition({ pos.x, pos.y + buttonY});
    float xPos = (pos.x + button.getGlobalBounds().width / 2) - (text.getGlobalBounds().width / 2);
    float yPos = (pos.y + button.getGlobalBounds().height / 2) - (text.getCharacterSize() / 2);
    text.setPosition({ xPos, yPos });
  }
  void setActive() {
    isActive = !isActive;
    if (isActive) {
      if (isViolet) {
        text.setFillColor(DARKVIOLET);
        button.setFillColor(VIOLET);
        buttonShadow.setFillColor(DARKVIOLET);
      }
      else {
        text.setFillColor(DARKBLUE);
        button.setFillColor(BLUE);
        buttonShadow.setFillColor(DARKBLUE);
      }
    } else {
      text.setFillColor(DARKGRAY);
      button.setFillColor(GRAY);
      buttonShadow.setFillColor(DARKGRAY);
    }
  }
  void drawTo(sf::RenderWindow& window) {
    window.draw(buttonShadow);
    window.draw(button);
    window.draw(text);
  }

  // Возвращает, находится ли курсор в пределах кнопки
  bool isMouseOver(sf::RenderWindow& window) {
    float mouseX = sf::Mouse::getPosition(window).x;
    float mouseY = sf::Mouse::getPosition(window).y;
    
    float btnPosX = button.getPosition().x;
    float btnPosY = button.getPosition().y;
    
    float btnXPosWidth = button.getPosition().x + button.getLocalBounds().width;
    float btnYPosHeight = button.getPosition().y + button.getLocalBounds().height;

    if (mouseX < btnXPosWidth && mouseX > btnPosX && mouseY < btnYPosHeight && mouseY > btnPosY) {
      return true;
    }
    return false;
  }

  // Анимация при нажатии и изменение состояния
  void buttonPressed(int x, int y) {
    if (isActive) {
      if (button.getGlobalBounds().contains(x, y)) {
        isPressed = true;
        if (isViolet) button.setFillColor(LIGHTVIOLETPRESSED);
        else button.setFillColor(LIGHTBLUEPRESSED);
        button.move(0, 3);
        text.move(0, 3);
      }
    }
  }

  // Анимация при отжатии и изменение состояния
  void buttonReleased(int x, int y) {
    if (isActive) {
      if (button.getGlobalBounds().contains(x, y)) {
        isPressed = false;
        if (isViolet) button.setFillColor(LIGHTVIOLETRELEASED);
        else button.setFillColor(LIGHTBLUERELEASED);
        button.move(0, -3);
        text.move(0, -3);
      }
    }
  }

  // Анимация при наводе
  void ifWantToClick(sf::RenderWindow& window) {
    if (!isPressed) {
      if (isActive) {
        if (isMouseOver(window)) {
          if (isViolet) setBackColor(LIGHTVIOLET);
          else setBackColor(LIGHTBLUE);
        }
        else {
          if (isViolet) setBackColor(VIOLET);
          else setBackColor(BLUE);
        }
      }
      else {
        if (isMouseOver(window)) setBackColor(LIGHTGRAY);
        else setBackColor(GRAY);
      }
    }
  }

  bool getActive() {
    return isActive;
  }
  bool getPressed() {
    return isPressed;
  }
};
