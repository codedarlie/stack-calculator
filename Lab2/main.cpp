#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "MyTextBox.h"
#include "MyButton.h"
#include "MyGraphicFunction.h"
#include "RPN.h"

using namespace sf;

constexpr auto x_textboxes = 50;
constexpr auto y_expr = 65;
constexpr auto y_rpn = 200;
constexpr auto y_der = 470;
constexpr auto y_der_rpn = 335;

int main() {
    // ~~~~~~~~~~~~~~~~~~~~~~~~ШРИФТ~~~~~~~~~~~~~~~~~~~~~~~~`
    
    Font joystick;
    joystick.loadFromFile("fonts/joystix.ttf");

    // ~~~~~~~~~~~~~~~~~~~~~~~~ОКНО~~~~~~~~~~~~~~~~~~~~~~~~`
    
    RenderWindow window(VideoMode(1100, 570), "OPN"); // Создание окна: Указываем размер и заголовок

    // ~~~~~~~~~~~~~~~~~~~~~~~~КНОПКИ~~~~~~~~~~~~~~~~~~~~~~~~
    
    MyButton btnDoRpn("Do rpn", {90, 40}, false, false, true);
    btnDoRpn.setFont(joystick);
    btnDoRpn.setPosition({ 410, 105 });

    MyButton btnReduce("Reduce", { 90, 40 }, false, false, true);
    btnReduce.setFont(joystick);
    btnReduce.setPosition({ 310, 105 });

    MyButton btnDraw("Draw Function", { 180, 40 }, false, false, true);
    btnDraw.setFont(joystick);
    btnDraw.setPosition({ 320, 240 });

    MyButton btnCalcDer("Calculate Derivative", { 255, 40 }, false, false, false);
    btnCalcDer.setFont(joystick);
    btnCalcDer.setPosition({ 50, 240 });

    MyButton btnDrawDer("Draw Derivative", { 200, 40 }, false, false, false);
    btnDrawDer.setFont(joystick);
    btnDrawDer.setPosition({ 300, 375 });

    // ~~~~~~~~~~~~~~~~~~~~~~~~СТРОКИ~~~~~~~~~~~~~~~~~~~~~~~~
    
    Text textExpression;
    textExpression.setString("Expression");
    textExpression.setCharacterSize(16);
    textExpression.setFont(joystick);
    textExpression.setFillColor(GRAY);
    textExpression.setPosition({ x_textboxes, y_expr - 25 });

    MyTextBox textBoxExpression(18, DARKVIOLET, false);
    textBoxExpression.setPosition({ x_textboxes, y_expr });
    textBoxExpression.setFont(joystick);
    textBoxExpression.setLimit(true, 29);
    
    RectangleShape textBoxExpressionBack(Vector2f(450, 24));
    textBoxExpressionBack.setOutlineColor(DARKGRAY);
    textBoxExpressionBack.setOutlineThickness(4.f);
    textBoxExpressionBack.setPosition({ x_textboxes, y_expr });


    Text textRPN;
    textRPN.setString("RPN");
    textRPN.setCharacterSize(16);
    textRPN.setFont(joystick);
    textRPN.setFillColor(GRAY);
    textRPN.setPosition({ x_textboxes, y_rpn - 25 });

    Text textBoxRPN("", joystick, 18);
    textBoxRPN.setFillColor(DARKBLUE);
    textBoxRPN.setPosition({ x_textboxes, y_rpn });

    RectangleShape textBoxRPNBack(Vector2f(450, 24));
    textBoxRPNBack.setOutlineColor(DARKGRAY);
    textBoxRPNBack.setOutlineThickness(4.f);
    textBoxRPNBack.setPosition({x_textboxes, y_rpn });
    

    Text textDerRPN;
    textDerRPN.setString("RPN Derivative");
    textDerRPN.setCharacterSize(16);
    textDerRPN.setFont(joystick);
    textDerRPN.setFillColor(GRAY);
    textDerRPN.setPosition({ x_textboxes, y_der_rpn - 25 });

    Text textBoxDer("", joystick, 18);
    textBoxDer.setFillColor(DARKBLUE);
    textBoxDer.setPosition({ x_textboxes, y_der_rpn });

    RectangleShape textBoxDerRPNBack(Vector2f(450, 24));
    textBoxDerRPNBack.setOutlineColor(DARKGRAY);
    textBoxDerRPNBack.setOutlineThickness(4.f);
    textBoxDerRPNBack.setPosition({ x_textboxes, y_der_rpn });


    Text yEqual("", joystick, 10);
    yEqual.setFillColor(DARKVIOLET);

    Text xEqual("", joystick, 10);
    xEqual.setFillColor(DARKVIOLET);

    
    // ~~~~~~~~~~~~~~~~~~~~~~~~ГРАФИК~И~ОСИ~~~~~~~~~~~~~~~~~~~~~~~~
    // Размеры графика
    const int W = 500;
    const int H = 500;

    // Левый верхний угол
    const int luw = 550; // left up - wight
    const int luh = 35; // left up - height

    RectangleShape area(Vector2f(W, H));
    area.setOutlineColor(Color::Black);
    area.setOutlineThickness(2.f);
    area.setPosition({ luw, luh }); 
    
    // начало координат
    int x0 = luw + W / 2; 
    int y0 = luh + H / 2;

    // int o1 = -5; // левая граница
    int o1 = -11;
    // int o2 = 5; // правая граница
    int o2 = 11;

    double c = 150; // увеличение числа точек
    int mass = ((o1) * (-1) + o2) * c + 1; // количество точек
    int sc = 22; // масштабирование линии
    

    RectangleShape line[40];
    for (int i = 0; i < 40; i++) {
        line[i].setSize(Vector2f(1, 10));
        line[i].setFillColor(Color::Black);
        if (i < 20) {
            if (i < 10) line[i].setPosition(x0 - (i + 1) * sc, y0 - 5);
            else line[i].setPosition(x0 + (i - 9) * sc, y0 - 5);
        }
        else {
            line[i].setRotation(90);
            if (i < 30) line[i].setPosition(x0 + 5, y0 + (i - 30) * sc);
            else line[i].setPosition(x0 + 5, y0 + (i - 29) * sc);
        }
    }

    RectangleShape OsX(Vector2f(W, 1)); // ось абцисс
    OsX.setFillColor(Color::Black);
    OsX.setPosition(luw, y0);

    RectangleShape OsY(Vector2f(1, H)); // ось ординат
    OsY.setFillColor(Color::Black);
    OsY.setPosition(x0, luh);

    RectangleShape strel[4];
    for (int i = 0; i < 4; i++) {
        strel[i].setSize(Vector2f(1, 25));
        strel[i].setFillColor(Color::Black);

        if (i < 2)
            strel[i].setPosition(x0, luh);
        else
            strel[i].setPosition(W + luw, y0);
    }
    strel[0].setRotation(25);
    strel[1].setRotation(-25);
    strel[2].setRotation(68);
    strel[3].setRotation(-250);

    Text xText("x", joystick, 12);
    xText.setFillColor(Color::Black);
    xText.setPosition(W + luw - 12, y0 + 12);

    Text yText("y", joystick, 12);
    yText.setFillColor(Color::Black);
    yText.setPosition(x0 + 14, luh + 3);


    // ~~~~~~~~~~~~~~~~~~~~~~~~RPN~~~~~~~~~~~~~~~~~~~~~~~~

    Expression expression; // Выражение, RPN, Производная, RPN производной

    MyGraphicFunction graphicMain(true, W, H, luw, luh, o1, o2, mass, c, sc); // основной график
    MyGraphicFunction graphicDer(false, W, H, luw, luh, o1, o2, mass, c, sc); // график производной
    bool drawFunction = false;
    bool drawDerivative = false;
    bool seeRPN = false;
    bool seeDer = false;
   
    std::vector<std::vector<double>> tangents; // массив касательных с основными значениями

    while (window.isOpen()) { // Каждая итерация цикла эта отрисовка отдельного кадра
        Vector2i mousePozition = Mouse::getPosition(window); // Получаем местоположение мышки в окне
        
        Event event; // Создание объекта ивента - когда что-то случилось
        window.clear(Color(231, 230, 242));

        if (Keyboard::isKeyPressed(Keyboard::Return)) { // Если нажали ENTER
            textBoxExpression.selSelected(true);
            // Логика кнопок
            if (btnDoRpn.getActive()) btnDoRpn.setActive();
            if (btnDraw.getActive()) btnDraw.setActive();
            if (btnCalcDer.getActive()) btnCalcDer.setActive();
            if (btnDrawDer.getActive()) btnDrawDer.setActive();
            if (btnReduce.getActive()) btnReduce.setActive();
        }
        else if (Keyboard::isKeyPressed(Keyboard::Escape)) { // Если нажали ESCAPE
            textBoxExpression.selSelected(false);
            // Логика кнопок
            if (textBoxExpression.getText() != "" && !btnDoRpn.getActive()) btnDoRpn.setActive();
            if (btnDraw.getActive()) btnDraw.setActive();
            if (btnCalcDer.getActive()) btnCalcDer.setActive();
            if (btnDrawDer.getActive()) btnDrawDer.setActive();
            if (btnReduce.getActive()) btnReduce.setActive();
        }
        while (window.pollEvent(event)) { // Обработка ивента
            if (event.type == Event::Closed) window.close(); // Нажали на закрыть
            if (event.type == Event::MouseButtonPressed) { // Еси нажали кнопкой мыши
                if (event.key.code == Mouse::Left) { // Левая кнопка мыши
                    
                    // Сообщаем об этом кнопкам (для анимации)
                    btnDoRpn.buttonPressed(mousePozition.x, mousePozition.y);
                    btnDraw.buttonPressed(mousePozition.x, mousePozition.y);
                    btnCalcDer.buttonPressed(mousePozition.x, mousePozition.y);
                    btnDrawDer.buttonPressed(mousePozition.x, mousePozition.y);
                    btnReduce.buttonPressed(mousePozition.x, mousePozition.y);

                    // Если мышка находится внутри рамки координатных осей, создаем касательную и получаем ее основные значения (y, k, x, b)
                    if (mousePozition.x < luw + W && mousePozition.x > luw && mousePozition.y < luh + H && mousePozition.y > luh) {
                        if (drawFunction && expression.isHasRpn() && expression.isHasDerivative()) {
                            std::vector<double> tangent = graphicMain.createTangent(mousePozition.x, window, expression.getAsRpn(), expression.getHisDerivative());
                            tangents.push_back(tangent);
                        }
                    }
                }

                // Логика кнопки DO RPN
                if (btnDoRpn.getPressed()) {
                    std::string newExpr = textBoxExpression.getText();
                    expression = newExpr;
                    expression.createRPN();
                    textBoxRPN.setString("");
                    textBoxDer.setString("");

                    // Логика кнопок
                    if (!btnDraw.getActive()) btnDraw.setActive();
                    if (!btnCalcDer.getActive()) btnCalcDer.setActive();
                    if (btnDrawDer.getActive()) btnDrawDer.setActive();
                    if (btnReduce.getActive()) btnReduce.setActive();
                    drawFunction = false;
                    drawDerivative = false;
                    seeDer = false;
                    tangents.clear();
                    seeRPN = true;
                }

                // Логика кнопки CALCULATE DERIVATIVE
                if (btnCalcDer.getPressed()) {
                    expression.calculateDerivative();
                    expression.normalizeDerivative();
                    if (!btnDrawDer.getActive()) btnDrawDer.setActive();
                    seeDer = true;
                }

                // Логика кнопки DRAW FUNCTION
                if (btnDraw.getPressed()) {
                    drawFunction = true;
                }
                
                // Логика кнопки DRAW DERIVATIVE
                if (btnDrawDer.getPressed()) {
                    drawDerivative = true;
                }
            }

            // Если отпустили кнопку мыши, то сообщаем об этом кнопкам (для анимации)
            if (event.type == Event::MouseButtonReleased) {
                if (event.key.code == Mouse::Left) {
                    btnDoRpn.buttonReleased(mousePozition.x, mousePozition.y);
                    btnDraw.buttonReleased(mousePozition.x, mousePozition.y);
                    btnCalcDer.buttonReleased(mousePozition.x, mousePozition.y);
                    btnDrawDer.buttonReleased(mousePozition.x, mousePozition.y);
                    btnReduce.buttonReleased(mousePozition.x, mousePozition.y);
                }
            }

            // Если ввели какой-либо символ, сообщаем об этом главному выражению
            if (event.type == Event::TextEntered) {
                textBoxExpression.typedOn(event);
            }

            // Если мышка передвинулась, то сообщаем об этом кнопкам (для анимации)
            if (event.type == Event::MouseMoved) {
                btnDoRpn.ifWantToClick(window);
                btnDraw.ifWantToClick(window);
                btnCalcDer.ifWantToClick(window);
                btnDrawDer.ifWantToClick(window);
                btnReduce.ifWantToClick(window);
            }
        }

        // ОТРИСОВКА ВСЕХ ОСНОВНЫХ ЭЛЕМЕНТОВ
        window.draw(area);
        window.draw(OsX);
        window.draw(OsY);
        for (int i = 0; i < 4; i++) window.draw(strel[i]);
        for (int i = 0; i < 40; i++) if (i != 19 && i != 20) window.draw(line[i]);
        window.draw(xText);
        window.draw(yText);

        window.draw(textBoxExpressionBack);
        window.draw(textBoxRPNBack);
        window.draw(textBoxDerRPNBack);

        textBoxExpression.drawTo(window);

        btnDoRpn.drawTo(window);
        btnReduce.drawTo(window);
        btnDraw.drawTo(window);
        btnCalcDer.drawTo(window);
        btnDrawDer.drawTo(window);

        // Отрисовка главной функции при соблюдении условий
        if (drawFunction && expression.isHasRpn()) {
            graphicMain.drawFunction(window, expression.getAsRpn());
        }
        
        // Отрисовка функции производной при соблюдении условий
        if (drawDerivative && expression.isHasDerivative()) {
            graphicDer.drawFunction(window, expression.getHisDerivative());
        }

        // Вывод записи rpn главного выражения при соблюдении условий
        if (seeRPN && expression.isHasRpn()) {
            textBoxRPN.setString("");
            std::stack<std::string> rpn = expression.getAsRpn();
            std::string buffer;
            while (!rpn.empty()) {
                buffer += rpn.top() + " ";
                rpn.pop();
            }
            textBoxRPN.setString(buffer);
            std::string s = textBoxRPN.getString();
            seeRPN = false;
        }

        // Вывод записи производной главного выражения при соблюдении условий
        if (seeDer && expression.isHasDerivative()) {
            textBoxDer.setString("");
            std::string der = expression.getHisDerivativeAsString();
            textBoxDer.setString(der);
            seeDer = false;

        }
        

        window.draw(textExpression);
        window.draw(textRPN);
        window.draw(textDerRPN);
        window.draw(textBoxRPN);
        window.draw(textBoxDer);

        // Отрисовка точки и касательной, если мышка внутри границ
        if (mousePozition.x < luw + W && mousePozition.x > luw && mousePozition.y < luh + H && mousePozition.y > luh) {
            if (drawFunction && expression.isHasRpn()) {
                graphicMain.drawPointIfMouseMoved(mousePozition.x, window, expression.getAsRpn());
            }
            if (drawFunction && expression.isHasRpn() && expression.isHasDerivative()) {
                graphicMain.drawTangents(mousePozition.x, window, expression.getAsRpn(), expression.getHisDerivative(), joystick);
            }
        }

        // Отрисовка созданных касательных
        CircleShape tangentPoint(1.f);
        for (int i = 0; i < tangents.size(); ++i) {
            tangentPoint.setFillColor(Color((155 + rand()%(55)), (110+rand()%(60)), 255));
            double x00 = tangents[i][0];
            double y00 = tangents[i][1];
            double k = tangents[i][2];
            double b = y00 - x00 * k;

            for (int j = 0; j < mass; ++j) {
                double x = o1 + j / c;
                double y = k * x + b;
                double x1 = x0 + x * sc;
                double y1 = y0 - y * sc;
                
                if (((x1 - 1) < luw || (x1 - 1) > (luw + W)) || ((y1 - 1) < luh || (y1 - 1) > (luh + H))) continue;

                tangentPoint.setPosition(x1 - 1, y1 - 1);
                window.draw(tangentPoint);
            }
            yEqual.setString("y = " + std::to_string(static_cast<int>(y00)));
            xEqual.setString("x = " + std::to_string(static_cast<int>(x00)));
            yEqual.setPosition(x00 * sc + x0 - 1 - 25, 550);
            xEqual.setPosition(x00 * sc + x0 - 1 - 25, 540);
            window.draw(yEqual);
            window.draw(xEqual);
        }

        // Отображение главного окна
        window.display();
    }

    return 0;
}
