#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <stack>
#include <vector>

constexpr auto PI = 3.1415;
constexpr auto E = 2.7182;

class MyGraphicFunction {
private:
  bool isMain = false; // главная ли? если да, то работает с мышкой
  const int w; // длина графика
  const int h; // высота графика
  const int luw; // левый верхний угол x
  const int luh; // левый верхний угол y
  int mass; // количество точек
  double c;
  int sc; // масштабирование
  int o1; // левая граница
  int o2; // правая граница
    std::stack<std::string> der;
    sf::Color color;

public:
  MyGraphicFunction() = delete;
    MyGraphicFunction(bool ismain, const int W, const int H, const int LUW, const int LUH, int o1, int o2, int mass, float c, int sc) : isMain{ ismain }, w { W }, h{ H }, luw{ LUW }, luh{ LUH }, mass{ mass }, c{ c }, sc{ sc }, o1{ o1 }, o2{ o2 } {
        if (ismain) {
            color = DARKVIOLET;
        }
        else {
            color = BLUE;
        }
    }

  double calculate(double x, std::stack<std::string> rpn) {
    if (rpn.empty()) { throw "Не посчитан RPN!"; return 0.f;}
    std::stack<double> results;
        std::vector<std::string> out3;
        while (!rpn.empty()) {
            out3.push_back(rpn.top());
            rpn.pop();
        }
    std::stack<std::string> buffer;
        for (int i = out3.size() - 1; i >= 0; --i) {
            buffer.push(out3[i]);
        }
    bool noOperands = true;
    while (!buffer.empty()) {
      const char* a = buffer.top().c_str();
      if (isdigit(*a)) {
        noOperands = false;
        results.push(std::stod(a));
        buffer.pop();
      }
      else if (*a == 'x') {
        noOperands = false;
        results.push(x);
        buffer.pop();
            }
            else if (*a == 'p') {
                noOperands = false;
                results.push(3.1415);
                buffer.pop();
            }
            else if (*a == 'e') {
                noOperands = false;
                results.push(2.7182);
                buffer.pop();
            }
      else {
        if (noOperands) { throw "No operands entered!!!\n"; return 0.0; }
        switch (*a) {
          case '+': {
            if (results.size() == 0) throw "Wrong operator or operand format! (Оператор + стоит неправильно, нет первого слагаемого)\n";
            double s = results.top(); results.pop();
            if (results.size() == 0) throw "Wrong operator or operand format! (Оператор + стоит неправильно, нет второго слагаемого)\n";
            double f = results.top(); results.pop();
            results.push(f + s);
            buffer.pop();
            break;
          }
                    case '-': {
                        if (results.size() == 0) throw "Wrong operator or operand format! (Оператор + стоит неправильно, нет первого слагаемого)\n";
                        double s = results.top(); results.pop();
                        if (results.size() == 0) throw "Wrong operator or operand format! (Оператор + стоит неправильно, нет второго слагаемого)\n";
                        double f = results.top(); results.pop();
                        results.push(f - s);
                        buffer.pop();
                        break;
                    }
                    case '*': {
                        if (results.size() == 0) throw "Wrong operator or operand format! (Оператор + стоит неправильно, нет первого слагаемого)\n";
                        double s = results.top(); results.pop();
                        if (results.size() == 0) throw "Wrong operator or operand format! (Оператор + стоит неправильно, нет второго слагаемого)\n";
                        double f = results.top(); results.pop();
                        results.push(f * s);
                        buffer.pop();
                        break;
                    }
                    case '/': {
                        if (results.size() == 0) throw "Wrong operator or operand format! (Оператор + стоит неправильно, нет первого слагаемого)\n";
                        double s = results.top(); results.pop();
                        if (s == 0) { return -1000000; }
                        if (results.size() == 0) throw "Wrong operator or operand format! (Оператор + стоит неправильно, нет второго слагаемого)\n";
                        double f = results.top(); results.pop();
                        results.push(f / s);
                        buffer.pop();
                        break;
                    }
                    case '^': {
                        if (results.size() == 0) throw "Wrong operator or operand format! (Оператор + стоит неправильно, нет первого слагаемого)\n";
                        double s = results.top(); results.pop();
                        if (results.size() == 0) throw "Wrong operator or operand format! (Оператор + стоит неправильно, нет второго слагаемого)\n";
                        double f = results.top(); results.pop();
                        results.push(pow(f, s));
                        buffer.pop();
                        break;
                    }
                    case 'c': {
                        if (results.size() == 0) throw "Wrong operator or operand format! (Оператор + стоит неправильно, нет числа, которое нужно преобразовать)\n";
                        double f = results.top(); results.pop();
                        results.push(std::cos(f));
                        buffer.pop();
                        break;
                    }
                    case 's': {
                        if (results.size() == 0) throw "Wrong operator or operand format! (Оператор + стоит неправильно, нет числа, которое нужно преобразовать)\n";
                        double f = results.top(); results.pop();
                        results.push(std::sin(f));
                        buffer.pop();
                        break;
                    }
                    case 'l': {
                        if (results.size() == 0) throw "Wrong operator or operand format! (Оператор + стоит неправильно, нет числа, которое нужно преобразовать)\n";
                        double f = results.top(); results.pop();
                        results.push(std::log(f));
                        buffer.pop();
                        break;
                    }
                    case '~': {
                        if (results.size() == 0) throw "Wrong operator or operand format! (Оператор + стоит неправильно, нет числа, которое нужно преобразовать)\n";
                        double f = results.top(); results.pop();
                        results.push(-(f));
                        buffer.pop();
                        break;
                    }
                    default: {
                        throw "ошибка\n";
                        break;
                    }
                }
      }
    }
        return results.top();
  }

  void drawFunction(sf::RenderWindow& window, std::stack<std::string>& out) {
    sf::CircleShape point(1.f);
    point.setFillColor(color);
        
    int xc = luw + w / 2; // начало координат
    int yc = luh + h / 2; // начало координат

    for (int i = 0; i < mass; ++i) {
      double x = o1 + i / c;
            double y = calculate(x, out);
            if (y == -1000000) continue;

            double x1 = xc + x * sc;
            double y1 = yc - y * sc;
            if (((x1 - 1) < luw || (x1 - 1) > (luw + w)) || ((y1 - 1) < luh || (y1 - 1) > (luh + h))) continue;
      point.setPosition(x1 - 1, y1 - 1);
      window.draw(point);
    }
  }

    void drawPointIfMouseMoved(double coords, sf::RenderWindow& window, std::stack<std::string>& out) {
        if (isMain) {
            int xc = luw + w / 2;
            int yc = luh + h / 2;

            double x = ((coords + 1 - xc) / sc);
            double y = calculate(x, out);

            double x1 = (xc + x * sc) - 1;
            double y1 = (yc - y * sc) - 1;

            sf::CircleShape point(3.f);
            if (((x1 - 1) < luw || (x1 - 1) > (luw + w)) || ((y1 - 1) < luh || (y1 - 1) > (luh + h))) return;
            point.setFillColor(sf::Color::Green);
            point.setPosition(x1 - 2, y1 - 2);
            window.draw(point);
        }
    }

    void drawTangents(double coords, sf::RenderWindow& window, std::stack<std::string>& out, std::stack<std::string>& der, sf::Font& font) {
        int xc = luw + w / 2; // центры
        int yc = luh + h / 2; // центры

        double x0 = ((coords - xc + 1) / sc);
        double y0 = calculate(x0, out);
        double k = calculate(x0, der);
        double b = y0 - x0*k;

        sf::Text yGray("", font, 10);
        yGray.setFillColor(LIGHTGRAY);

        sf::Text xGray("", font, 10);
        xGray.setFillColor(LIGHTGRAY);

        sf::Text xCoord("", font, 10);
        xCoord.setFillColor(LIGHTGRAY);

        sf::CircleShape point(1.f);
        point.setFillColor(sf::Color(156, 156, 156));

        for (int i = 0; i < mass; ++i) {
            double x = o1 + i / c;
            double y = k*x + b;
            double x1 = xc + x * sc;
            double y1 = yc - y * sc;
            if (((x1 - 1) < luw || (x1 - 1) > (luw + w)) || ((y1 - 1) < luh || (y1 - 1) > (luh + h))) continue;
            
            point.setPosition(x1 - 1, y1 - 1);
            window.draw(point);

        }
        yGray.setString("y = " + std::to_string((std::round(y0 * 100)) / 100));
        xGray.setString("x = " + std::to_string((std::round(x0 * 100)) / 100));
        yGray.setPosition(luw + w - 120, luh + 10);
        xGray.setPosition(luw + w - 120, luh + 20);
        window.draw(yGray);
        window.draw(xGray);

        xCoord.setString("x = " + std::to_string(coords));
        xCoord.setPosition(luw + 30, luh + 20);
        window.draw(xCoord);
    }
    
    std::vector<double> createTangent(double coords, sf::RenderWindow& window, std::stack<std::string>& out, std::stack<std::string>& der) {
        std::vector<double> tangent;
        
        int xc = luw + w / 2; // центры
        int yc = luh + h / 2; // центры

        double x0 = ((coords + 1 - xc) / sc);
        tangent.push_back(x0);
        double y0 = calculate(x0, out);

        tangent.push_back(y0);
        double k = calculate(x0, der);
        tangent.push_back(k);

        return tangent;
    }
}; 
