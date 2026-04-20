#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>

#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27

class MyTextBox {
private:
  sf::Text textbox;
  std::ostringstream text;
  bool isSelected = false;
  bool hasLimit = false;
  int limit;

  // Корректный ввод символов в выражение
  void inputLogic(int charTyped) {
    if (charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY) {
      text << static_cast<char>(charTyped);
    }
    else if (charTyped == DELETE_KEY) {
      if (text.str().length() > 0) {
        deleteLastChar();
      }
    }
    textbox.setString(text.str() + "_"); // Для красивого отображения
    std::cout << "String: " << text.str() << std::endl;
  }

  // Корректное удаление символов из выражения
  void deleteLastChar() {
    std::string t = text.str();
    std::string newT = "";
    for (int i = 0; i < t.length() - 1; ++i) {
      newT += t[i];
    }
    text.str("");
    text << newT;

    textbox.setString(text.str());
  }

public:
  MyTextBox() { }
  MyTextBox(int size, sf::Color color, bool sel) {
    textbox.setCharacterSize(size);
    textbox.setFillColor(color);
    isSelected = sel;
    if (sel) {
      textbox.setString("_");
    }
    else {
      textbox.setString("");
    }
}

  void setFont(sf::Font& font) {
    textbox.setFont(font);
  }
  void setPosition(sf::Vector2f pos) {
    textbox.setPosition(pos);
  }
  void setLimit(bool ToF) {
    hasLimit = ToF;
  }
  void setLimit(bool ToF, int lim) {
    hasLimit = ToF;
    limit = lim - 1;
  }
  void selSelected(bool sel) {
    isSelected = sel;
    if (!sel) {
      std::string t = text.str();
      std::string newT = "";
      for (int i = 0; i < t.length(); i++) {
        newT += t[i];
      }
      textbox.setString(newT);
    }
  }
  std::string getText() {
    return text.str();
  }
  void drawTo(sf::RenderWindow& window) {
    window.draw(textbox);
  }
  
  // Проверка символов и длины выражения
  void typedOn(sf::Event input) {
    if (isSelected) {
      int charTyped = input.text.unicode;
      if ((charTyped >= 47 && charTyped <= 57) || (charTyped >= 40 && charTyped <= 43) || 
        (charTyped >= 110 && charTyped <= 112) || charTyped == ENTER_KEY || charTyped == ESCAPE_KEY || 
        charTyped == DELETE_KEY || charTyped == 120 || charTyped == 115 || charTyped == 108 || 
        charTyped == 105 || charTyped == 101 || charTyped == 99 || charTyped == 94 || 
        charTyped == 28 || charTyped == 29 || charTyped == 45) {
        if (hasLimit) {
          if (text.str().length() <= limit) {
            inputLogic(charTyped);
          }
          else if (text.str().length() > limit && charTyped == DELETE_KEY) {
            deleteLastChar();
          }
        }
        else {
          inputLogic(charTyped);
        }
      }
      else {
        std::cout << "этот символ нельзя использовать!\n";
      }
    }
  }

};
