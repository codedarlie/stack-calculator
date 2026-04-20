#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <cstdlib>
#include <cmath>

class Expression {
private:
    std::string asString;
    std::stack<std::string> asRpn;
    std::stack<std::string> hisDerivative;
    std::string hisDerivativeString;
    std::string hisDerivativeAsString;

    bool hasRPN = false;
    bool hasReduce = false;
    bool hasDerivative = false;
    bool hasDerivativeAsString = false;

    void checkStackAndPush(char a, std::stack<char>& st, std::stack<std::string>& out) {
        if (st.size() != 0) {
            while (pr(st.top()) >= pr(a)) {
                std::string add;
                add += st.top();
                out.push(add);
                st.pop();
                if (st.size() == 0) break;
            }
        }
        st.push(a);
    }
    int pr(char p) {
        switch (p) {
        case '+': case '-': return 0;
        case '*': case '/': return 1;
        case '^': return 2;
        case 's': case 'c': case 'l': return 3;
        default: return -1;
        }
    }
    bool isNumber(const std::string& str) {
        // for (auto i = str.begin(); i != str.end(); ++i) {
            // if (!isdigit(*i)) return false;
        // }
        // return true;
        return std::all_of(str.begin(), str.end(), [](const char& ch) { return (isdigit(ch) || ch == '.'); });
    }
    bool checkHasX(std::pair<int, int>& coords, std::vector<std::string>& DER) {
        for (int i = coords.first; i <= coords.second; ++i) {
            if (DER[i] == "x") {
                return true;
            }
        }
        return false;
    }
    std::pair<int, int> giveOneCoords(std::pair<int, int> someCoords, std::vector<std::string>& DER) {
        std::pair<int, int> coords = { -1, -1 };
        for (int i = someCoords.second; i >= someCoords.first; --i) {
            if (DER[i] == "~" || DER[i] == "c" || DER[i] == "s" || DER[i] == "l") { // унарная
                std::pair<int, int> c = giveOneCoords({ someCoords.first, someCoords.second - 1 }, DER);
                coords = { c.first, c.second + 1 };
                return coords;
            }
            else if (DER[i] == "+" || DER[i] == "-" || DER[i] == "*" || DER[i] == "/" || DER[i] == "^") {
                std::pair<std::pair<int, int>, std::pair<int, int>> c = giveTwoCoords({ someCoords.first, someCoords.second - 1 }, DER);
                coords = { c.first.first, c.second.second + 1 };
                return coords;
            }
            else if (isNumber(DER[i]) || DER[i] == "e" || DER[i] == "p" || DER[i] == "x") {
                coords = { i, i };
                return coords;
            }
            else {
                throw "ошибка в giveTwoCoords";
            }
        }
        return coords;
    }
    std::pair<std::pair<int, int>, std::pair<int, int>> giveTwoCoords(std::pair<int, int> someCoords, std::vector<std::string>& DER) {
        int firstSize = 0;
        std::pair<std::pair<int, int>, std::pair<int, int>> coords = { {-1, -1}, {-1, -1} };
        for (int i = someCoords.second; i >= someCoords.first; --i) {
            if (DER[i] == "~" || DER[i] == "c" || DER[i] == "s" || DER[i] == "l") { // унарная
                std::pair<int, int> c = giveOneCoords({ someCoords.first, someCoords.second - 1 - firstSize }, DER);
                if (coords.second.first == -1 && coords.second.second == -1) {
                    coords.second = { c.first, c.second + 1 };
                    i -= coords.second.second - coords.second.first;
                    firstSize = coords.second.second - coords.second.first + 1;
                }
                else if (coords.first.first == -1 && coords.first.second == -1) {
                    coords.first = { c.first, c.second + 1 };
                    return coords;
                }
                else {
                    throw "ошибка в giveTwoCoords";
                }
            }
            else if (DER[i] == "+" || DER[i] == "-" || DER[i] == "*" || DER[i] == "/" || DER[i] == "^") {
                std::pair<std::pair<int, int>, std::pair<int, int>> c = giveTwoCoords({ someCoords.first, someCoords.second - 1 - firstSize }, DER);
                if (coords.second.first == -1 && coords.second.second == -1) {
                    coords.second = { c.first.first, c.second.second + 1 };
                    i -= coords.second.second - coords.second.first;
                    firstSize = coords.second.second - coords.second.first + 1;
                }
                else if (coords.first.first == -1 && coords.first.second == -1) {
                    coords.first = { c.first.first, c.second.second + 1 };
                    return coords;
                }
                else {
                    throw "ошибка в giveTwoCoords";
                }

            }
            else if (isNumber(DER[i]) || DER[i] == "e" || DER[i] == "p" || DER[i] == "x") {
                if (coords.second.first == -1 && coords.second.second == -1) {
                    coords.second = { i, i };
                    firstSize = 1;
                }
                else if (coords.first.first == -1 && coords.first.second == -1) {
                    coords.first = { i, i };
                    return coords;
                }
                else {
                    throw "ошибка в giveTwoCoords";
                }
            }
            else {
                throw "ошибка в giveTwoCoords";
            }
        }
        return coords;
    }

public:
    Expression() = default;
    Expression(std::string& givedString) {
        asString = givedString;
        hasRPN = false;
        hasReduce = false;
        hasDerivative = false;
        hasDerivativeAsString = false;
    }
    Expression(const char* givedString) {
        asString = givedString;
        hasRPN = false;
        hasReduce = false;
        hasDerivative = false;
        hasDerivativeAsString = false;
    }
    Expression(std::stack<std::string>& givedRPN) {
        asRpn = givedRPN;
        hasRPN = true;
        hasReduce = false;
        hasDerivative = false;
        hasDerivativeAsString = false;
    }
    void operator=(std::string& ex) {
        while (!asRpn.empty()) asRpn.pop();
        while (!hisDerivative.empty()) hisDerivative.pop();
        asString = ex;
        hasRPN = false;
        hasReduce = false;
        hasDerivative = false;
    }
    void createRPN() {
        std::stack<char> st;
        std::stack<std::string> out;
        if (!asRpn.empty()) { throw "RPN уже посчитан!\n"; return; }
        if (asString.size() == 0) { throw "Empty expression! (Вы ничего не передали)\n"; return; }
        for (int i = 0; i < asString.size(); ++i) {
            switch (asString[i]) {
            case '-': case '+': {
                if (i == 0 || asString[i - 1] == '(') {
                    if (asString[i] == '-') st.push('~'); // унарная
                }
                else checkStackAndPush(asString[i], st, out); // бинарная
                break;
            }
            case '*': case '/': {
                checkStackAndPush(asString[i], st, out);
                break;
            }
            case '^': {
                if (st.size() != 0) {
                    while (pr(st.top()) > pr(asString[i])) {
                        std::string add;
                        add += st.top();
                        out.push(add);
                        st.pop();
                        if (st.size() == 0) break;
                    }
                }
                st.push(asString[i]);
                break;
            }
            case 's': {
                if ((i + 2 <= asString.size()) && (asString[i + 1] == 'i') && (asString[i + 2] == 'n')) { // Если вообще до конца не дошли
                    checkStackAndPush('s', st, out);
                    i += 2;
                }
                else throw "Wrong operator or operand format! (Синус введен неправильно)\n";
                break;
            }
            case 'c': {
                if ((i + 2 <= asString.size()) && (asString[i + 1] == 'o') && (asString[i + 2] == 's')) { // Если вообще до конца не дошли
                    checkStackAndPush('c', st, out);
                    i += 2;
                }
                else throw "Wrong operator or operand format! (Косинус введен неправильно)\n";
                break;
            }
            case 'l': {
                if ((i + 1 <= asString.size()) && (asString[i + 1] == 'n')) { // Если вообще до конца не дошли
                    checkStackAndPush('l', st, out);
                    ++i;
                }
                else throw "Wrong operator or operand format! (Натуральный логарифм введен неправильно)\n";
                break;
            }
            case 'p': {
                if ((i + 1 <= asString.size()) && (asString[i + 1] == 'i')) { // Если вообще до конца не дошли
                    out.push("p");
                    ++i;
                }
                else throw "Wrong operator or operand format! (Пи введен неправильно)\n";
                break;
            }
            case 'e': case 'x': {
                std::string add;
                add += asString[i];
                out.push(add);
                break;
            }
            case '(': {
                st.push(asString[i]);
                break;
            }
            case ')': {
                if (st.size() != 0) {
                    while (st.top() != '(') {
                        std::string add;
                        add += st.top();;
                        out.push(add);
                        st.pop();
                        if (st.size() == 0) {
                            throw "Wrong operator or operand format! (В стеке нет открывающейся скобки)\n";
                            // break;
                        }
                    }
                    st.pop(); // встретили и удалили (
                }
                else {
                    throw "Wrong operator or operand format! (Стек пустой)\n";
                }
                break; // чтобы дальше не смотреть switch
            }
            default: {
                if (isdigit(asString[i])) {
                    int size = 1;
                    std::string num;
                    num += asString[i];
                    for (++i; i < asString.size(); ++i) {
                        if (isdigit(asString[i])) { ++size; num += asString[i]; }
                        else break;
                    }
                    std::string add;
                    add += num;
                    out.push(add);
                    --i;
                }
                else {
                    throw "Wrong operator or operand format! (Введен не тот символ)\n";
                    break;
                }
            }
            }
        }
        while (st.size() != 0) {
            if (st.top() == '(') throw "Wrong operator or operand format (Написана лишняя скобка)\n";
            std::string add;
            add += st.top();
            out.push(add);
            st.pop();
        }
        while (!out.empty()) {
            asRpn.push(out.top());
            out.pop();
        }
        hasRPN = true;
    }
    void calculateDerivative() {
        if (hasDerivative) return;

        std::vector<std::pair<int, int>> M{ {0, asRpn.size() - 1} }; // отрезки производных
        std::stack<std::string> buffer = asRpn;
        std::vector<std::string> DER; // выражение изначально = рпн но потом будет равен производной его рпн

        while (!buffer.empty()) {
            std::string add;
            add += buffer.top();
            DER.push_back(add);
            buffer.pop();
        }

        while (!M.empty()) { // пока все отрезки не посмотрели
            if (M.back().first == M.back().second) { // если размер отрезка == 1
                if (isNumber(DER[M.back().first]) || DER[M.back().first] == "p" || DER[M.back().first] == "e") {
                    DER[M.back().first] = "0";
                    M.pop_back();
                }
                else if (DER[M.back().first] == "x") {
                    DER[M.back().first] = "1";
                    M.pop_back();
                }
                else {
                    throw "error"; return;
                }
            }
            else { // если > 1
                for (int i = M.back().second; i >= M.back().first; --i) { // проходим циклом по каждому значению этого отрезка
                    if (DER[i] == "~" || DER[i] == "c" || DER[i] == "s" || DER[i] == "l") {
                        std::pair<int, int> crds = giveOneCoords({ M.back().first, M.back().second - 1 }, DER); 
                        if (DER[i] == "~") { // если ~ то отрезок можно просто на один сократить
                            M.pop_back();
                            M.push_back(crds); 
                            break;
                        }
                        else if (DER[i] == "c") {
                            /*
                            (x)     c
                            3-7
                            (x)     s     (x)     *      ~
                            3-7     8     9-13    14     15
                                          ___
                            Выражение: -(sin(10+1))*(10+1)
                            RPN: 10 1 + s 10 1 + * ~
                            */
                            std::vector<std::string> v;
                            for (int j = crds.first; j <= crds.second; ++j) {
                                v.push_back(DER[j]);
                            }
                            v.push_back("s");
                            for (int j = crds.first; j <= crds.second; ++j) {
                                v.push_back(DER[j]);
                            }
                            v.push_back("*");
                            v.push_back("~");
                            DER.erase(DER.begin() + M.back().first, DER.begin() + M.back().second + 1); // заменяем то к чему этот косинус относится 
                            DER.insert(DER.begin() + M.back().first, v.begin(), v.end()); // на то к чему относится его производная
                            M.pop_back();
                            int size = (crds.second - crds.first + 1) + 1;
                            M.push_back({ crds.first + size, crds.second + size }); // не просто удаляем отрезок, а заменяем на другой отрезок
                            break;
                        }
                        else if (DER[i] == "s") {
                            /*
                                (x)     s
                                3-7
                                (x)     c     (x)     *
                                3-7     8     9-13    14
                                              ___
                                Выражение: (cos(10+1))*(10+1)
                                RPN: 10 1 + c 10 1 + * ~
                            */
                            std::vector<std::string> v;
                            for (int j = crds.first; j <= crds.second; ++j) {
                                v.push_back(DER[j]);
                            }
                            v.push_back("c");
                            for (int j = crds.first; j <= crds.second; ++j) {
                                v.push_back(DER[j]);
                            }
                            v.push_back("*");
                            DER.erase(DER.begin() + M.back().first, DER.begin() + M.back().second + 1);
                            DER.insert(DER.begin() + M.back().first, v.begin(), v.end());
                            M.pop_back();
                            int size = (crds.second - crds.first + 1) + 1;
                            M.push_back({ crds.first + size, crds.second + size });
                            break;
                        }
                        else if (DER[i] == "l") {
                            /*
                                (x)     l
                                3-7

                                1     (x)     /     (x)     *
                                3     4-8     9     10-14   15
                                                    ___
                                Выражение: (1/(10+1))*(10+1)
                                RPN: 1 10 1 + / 10 1 + *
                            */
                            std::vector<std::string> v;
                            v.push_back("1");
                            for (int j = crds.first; j <= crds.second; ++j) {
                                v.push_back(DER[j]);
                            }
                            v.push_back("/");
                            for (int j = crds.first; j <= crds.second; ++j) {
                                v.push_back(DER[j]);
                            }
                            v.push_back("*");
                            DER.erase(DER.begin() + M.back().first, DER.begin() + M.back().second + 1);
                            DER.insert(DER.begin() + M.back().first, v.begin(), v.end());
                            M.pop_back();
                            int size = 1 + (crds.second - crds.first + 1) + 1;
                            M.push_back({ crds.first + size, crds.second + size });
                            break;
                        }
                    }
                    else if (DER[i] == "+" || DER[i] == "-" || DER[i] == "*" || DER[i] == "/" || DER[i] == "^") {
                        std::pair<std::pair<int, int>, std::pair<int, int>> crds = giveTwoCoords({ M.back().first, M.back().second - 1 }, DER);
                        if (DER[i] == "+" || DER[i] == "-") {
                            M.pop_back();
                            M.push_back(crds.first);
                            M.push_back(crds.second);
                            break;
                        }
                        else if (DER[i] == "*") {
                            /*
                            x y * [0, 2]
                            (x*y)`
                            
                            (x)`*y+x*(y)`
                            x y * x y * + [0, 0], [4, 4]
                            */
                            
                            std::vector<std::string> v;

                            for (int j = crds.first.first; j <= crds.first.second; ++j) {
                                v.push_back(DER[j]);
                            }
                            for (int j = crds.second.first; j <= crds.second.second; ++j) {
                                v.push_back(DER[j]);
                            }
                            v.push_back("*");
                            for (int j = crds.first.first; j <= crds.first.second; ++j) {
                                v.push_back(DER[j]);
                            }
                            for (int j = crds.second.first; j <= crds.second.second; ++j) {
                                v.push_back(DER[j]);
                            }
                            v.push_back("*"); 
                            v.push_back("+"); 
                            DER.erase(DER.begin() + M.back().first, DER.begin() + M.back().second + 1); 
                            DER.insert(DER.begin() + M.back().first, v.begin(), v.end());
                            M.pop_back();
                            M.push_back(crds.first);
                            int size = (crds.second.second - crds.second.first + 1) + 1 + (crds.first.second - crds.first.first + 1);
                            M.push_back({ crds.second.first + size, crds.second.second + size }); 
                            break;
                        }
                        else if (DER[i] == "/") {
                            /*
                                x' y * y' x * - y 2 ^ /
                                (n)   (x)   *
                                1-3   4-8   9

                                (n)   (x)   *   (n)   (x)   *   -   (x)   2   ^   /
                                1-3   4-8   9  10-13 14-18  19
                                1-3                   ___
                            */
                            std::vector<std::string> v;
                            for (int j = crds.first.first; j <= crds.first.second; ++j) {
                                v.push_back(DER[j]);
                            }
                            for (int j = crds.second.first; j <= crds.second.second; ++j) {
                                v.push_back(DER[j]);
                            }
                            v.push_back("*");
                            for (int j = crds.first.first; j <= crds.first.second; ++j) {
                                v.push_back(DER[j]);
                            }
                            for (int j = crds.second.first; j <= crds.second.second; ++j) {
                                v.push_back(DER[j]);
                            }
                            v.push_back("*");
                            v.push_back("-");
                            for (int j = crds.second.first; j <= crds.second.second; ++j) {
                                v.push_back(DER[j]);
                            }
                            v.push_back("2");
                            v.push_back("^");
                            v.push_back("/");
                            DER.erase(DER.begin() + M.back().first, DER.begin() + M.back().second + 1);
                            DER.insert(DER.begin() + M.back().first, v.begin(), v.end());
                            M.pop_back();
                            M.push_back(crds.first);
                            int size = (crds.second.second - crds.second.first + 1) + 1 + (crds.first.second - crds.first.first + 1);
                            M.push_back({ crds.second.first + size, crds.second.second + size });
                            break;
                        }
                        else if (DER[i] == "^") {
                            if (checkHasX(crds.second, DER) && !(checkHasX(crds.first, DER))) { // если ^x
                                // (n)   (x)   ^
                                // (n)   (x)   ^   (n)   l   *   (x)   *
                                // 0-2   3-7   8   9-11  12  13 14-18 
                                // производные                   ___ 

                                std::vector<std::string> v;
                                for (int j = crds.first.first; j <= crds.first.second; ++j) {
                                    v.push_back(DER[j]);
                                }
                                for (int j = crds.second.first; j <= crds.second.second; ++j) {
                                    v.push_back(DER[j]);
                                }
                                v.push_back("^");
                                for (int j = crds.first.first; j <= crds.first.second; ++j) {
                                    v.push_back(DER[j]);
                                }
                                v.push_back("l");
                                v.push_back("*");
                                for (int j = crds.second.first; j <= crds.second.second; ++j) {
                                    v.push_back(DER[j]);
                                }
                                v.push_back("*");
                                DER.erase(DER.begin() + M.back().first, DER.begin() + M.back().second + 1);
                                DER.insert(DER.begin() + M.back().first, v.begin(), v.end());
                                M.pop_back();
                                int size = (crds.second.second - crds.second.first + 1) + 1 + (crds.first.second - crds.first.first + 1) + 2;
                                M.push_back({ crds.second.first + size, crds.second.second + size });
                                break;
                            }
                            else if (checkHasX(crds.first, DER) && !(checkHasX(crds.second, DER))) { // если x^
                                
                                /*
                                    (x)   (n)   ^ ->
                                    3-6   8-15
                                    (n)   (x)    (n)    1    -    ^    *    (x)    *
                                    3-10 11-14  15-22  23    24   25   26  27-30   31
                                    производные                            _____

                                */

                                std::vector<std::string> v;
                                for (int j = crds.second.first; j <= crds.second.second; ++j) {
                                    v.push_back(DER[j]);
                                }
                                for (int j = crds.first.first; j <= crds.first.second; ++j) {
                                    v.push_back(DER[j]);
                                }
                                for (int j = crds.second.first; j <= crds.second.second; ++j) {
                                    v.push_back(DER[j]);
                                }
                                v.push_back("1");
                                v.push_back("-");
                                v.push_back("^");
                                v.push_back("*");
                                for (int j = crds.first.first; j <= crds.first.second; ++j) {
                                    v.push_back(DER[j]);
                                }
                                v.push_back("*");
                                DER.erase(DER.begin() + M.back().first, DER.begin() + M.back().second + 1);
                                DER.insert(DER.begin() + M.back().first, v.begin(), v.end());
                                M.pop_back();
                                int size = (crds.second.second - crds.second.first + 1) * 2 + (crds.first.second - crds.first.first + 1) + 4;
                                M.push_back({ crds.first.first + size, crds.first.second + size });
                                break;
                            }
                            else if (checkHasX(crds.second, DER) && (checkHasX(crds.first, DER))) {
                                /*
                                    (n)   (x)   ^
                                    (n)   (x)   ^   (n)   l   (x)   *   *
                                    0-2   3-7   8   9-11  12 13-17  18
                                    производные     _________________
                                */
                                std::vector<std::string> v;
                                for (int j = crds.first.first; j <= crds.first.second; ++j) {
                                    v.push_back(DER[j]);
                                }
                                for (int j = crds.second.first; j <= crds.second.second; ++j) {
                                    v.push_back(DER[j]);
                                }
                                v.push_back("^");
                                for (int j = crds.first.first; j <= crds.first.second; ++j) {
                                    v.push_back(DER[j]);
                                }
                                v.push_back("l");
                                for (int j = crds.second.first; j <= crds.second.second; ++j) {
                                    v.push_back(DER[j]);
                                }
                                v.push_back("*");
                                v.push_back("*");
                                DER.erase(DER.begin() + M.back().first, DER.begin() + M.back().second + 1);
                                DER.insert(DER.begin() + M.back().first, v.begin(), v.end());
                                M.pop_back();
                                int size1 = (crds.first.second - crds.first.first + 1) + (crds.second.second - crds.second.first + 1) + 1;
                                int size2 = size1 + 1 + (crds.second.second - crds.second.first + 1) + 1;
                                M.push_back({ crds.first.first + size1, crds.first.second + size2 });
                                break;

                            }
                            else { // значит это выражение целиком число, и его можно заменить на 0
                                std::vector<std::string> v = { "0" };
                                DER.erase(DER.begin() + M.back().first, DER.begin() + M.back().second + 1);
                                DER.insert(DER.begin() + M.back().first, v.begin(), v.end());
                                M.pop_back();
                            }
                        }
                    }
                }
            }
        }
        
        hasDerivative = true;
        for (int i = DER.size() - 1; i >= 0; --i) {
            hisDerivative.push(DER[i]);
        }
    }
    void normalizeDerivative() {
        if (!hasDerivative) {
            std::cout << "error" << std::endl; return;
        };
        std::stack<std::string> rpn;
        std::stack<std::string> out2 = hisDerivative;

        while (!out2.empty()) {
            if (isNumber(out2.top()) || out2.top() == "x" || out2.top() == "p" || out2.top() == "e") {
                rpn.push(out2.top());
                out2.pop();
            }
            else {
                if (out2.top() == "~") {
                    if (rpn.top().size() == 1) {
                        std::string s = "~";
                        s += rpn.top();
                        rpn.pop();
                        rpn.push(s);
                        out2.pop();
                    }
                    else {
                        std::string s = "~(";
                        s += rpn.top();
                        rpn.pop();
                        s += ")";
                        rpn.push(s);
                        out2.pop();

                    }
                }
                else if (out2.top() == "c" || out2.top() == "s" || out2.top() == "l") {
                    std::string s = out2.top() + "(";
                    s += rpn.top();
                    rpn.pop();
                    s += ")";
                    rpn.push(s);
                    out2.pop();
                }
                else if (out2.top() == "+" || out2.top() == "-" || out2.top() == "*" || out2.top() == "/" || out2.top() == "^") {
                    std::string s1;
                    std::string s2;
                    if (rpn.top().size() == 1) {
                        s1 += rpn.top();
                        rpn.pop();
                    }
                    else {
                        s1 += "(" + rpn.top() + ")";
                        rpn.pop();
                    }

                    if (rpn.top().size() == 1) {
                        s2 += rpn.top();
                        rpn.pop();
                    }
                    else {
                        s2 += "(" + rpn.top() + ")";
                        rpn.pop();
                    }
                    rpn.push(s2 + out2.top() + s1);
                    out2.pop();
                }
            }
        }

        hisDerivativeAsString = rpn.top();
        hasDerivativeAsString = true;
    }

    std::stack<std::string>& getAsRpn() { return asRpn; }
    std::stack<std::string>& getHisDerivative() { return hisDerivative; }
    std::string& getHisDerivativeAsString() { return hisDerivativeAsString; }

    bool isHasRpn() { return hasRPN; }
    bool isHasReduced() { return hasReduce; }
    bool isHasDerivative() { return hasDerivative; }
};
