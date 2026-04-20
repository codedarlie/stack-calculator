#include <iostream>
#include <string>
#include <stack>
#include <cstdlib>
#include <cmath>

class Expression {
private:
  std::stack<char> st;
  std::stack<std::string> out; // string чтобы удобно считать, и не стэк
  std::string RPNString;
  std::stack<std::string> buffer;
  std::string s;

  std::stack<double> results;
  double answer;
  
  bool isCalculated = false;
  bool RPNed = false;

public:
  Expression(): st{std::stack<char>{}}, out{std::stack<std::string>{}}, buffer{std::stack<std::string>{}}, s{std::string{}}, RPNString{std::string{}}, results{std::stack<double>{}}, answer{} {}
  Expression(std::string& someStr): st{std::stack<char>{}}, out{std::stack<std::string>{}}, buffer{std::stack<std::string>{}}, s{someStr}, RPNString{std::string{}}, results{std::stack<double>{}}, answer{} {}

  void checkStackAndPush(char a) {
    if (st.size() != 0) {
      while (pr(st.top()) >= pr(a)) {
        std::string add;
        add += st.top();        
        out.push(add);
        RPNString += add + " ";
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
  void operator=(std::string& ex) {
    while (!st.empty()) st.pop();
    while (!out.empty()) out.pop();
    while (!buffer.empty()) buffer.pop();
    while (!results.empty()) results.pop();
    int answer;
    s.clear();
    RPNString.clear();
    s = ex;
    isCalculated = false;
    RPNed = false;
  }
  void allToOut() {
    while (st.size() != 0) {
      if (st.top() == '(') throw "Wrong operator or operand format (Написана лишняя скобка)\n";
      std::string add;
      add += st.top();  
      out.push(add);
      RPNString += add + " ";
      st.pop();
    }
  }
  void doRPN() {
    if (!out.empty() || isCalculated) {throw "RPN уже посчитан!\n"; return;}
    if (s.size() == 0) {throw "Empty expression! (Вы ничего не передали)\n"; return;}
    for (int i = 0; i < s.size(); ++i) {
      switch (s[i]) {
        case '-': case '+': {
          if (i == 0 || s[i-1] == '(') {
            if (s[i] == '-') st.push('~'); // унарная
          } else checkStackAndPush(s[i]); // бинарная
          break;
        }
        case '*': case '/': {checkStackAndPush(s[i]); break;}
        case '^': {
          if (st.size() != 0) {
            while (pr(st.top()) > pr(s[i])) {
              std::string add;
              add += st.top();        
              out.push(add);
              RPNString += add + " ";
              st.pop();
              if (st.size() == 0) break;
            }
          }
          st.push(s[i]);
          break;
        }
        case 's': {
          if ((i + 2 <= s.size()) && (s[i+1] == 'i') && (s[i+2] == 'n')) { // Если вообще до конца не дошли
            checkStackAndPush('s');
            i += 2;
          } else throw "Wrong operator or operand format! (Синус введен неправильно)\n";
          break;
        }
        case 'c': {
          if ((i + 2 <= s.size()) && (s[i+1] == 'o') && (s[i+2] == 's')) { // Если вообще до конца не дошли
            checkStackAndPush('c');
            i += 2;
          } else throw "Wrong operator or operand format! (Косинус введен неправильно)\n";
          break;
        }
        case 'l': {
          if ((i + 1 <= s.size()) && (s[i+1] == 'n')) { // Если вообще до конца не дошли
            checkStackAndPush('l');
            ++i;
          } else throw "Wrong operator or operand format! (Натуральный логарифм введен неправильно)\n";
          break;
        }
        case '(': {
          st.push('(');
          break;
        }
        case ')': {
          if (st.size() != 0) {
            while (st.top() != '(') {
              std::string add;
              add += st.top();
              out.push(add);
              RPNString += add + " ";
              st.pop();
              if (st.size() == 0) {
                throw "Wrong operator or operand format! (В стеке нет открывающейся скобки)\n"; 
              }
            }
            st.pop(); // встретили и удалили (
          } else {
            throw "Wrong operator or operand format! (Стек пустой)\n";
          }
          break; // чтобы дальше не смотреть switch
        }
        default: {
          if (isdigit(s[i])) {
            int size = 1;
            std::string num;
            num += s[i];
            for (++i; i < s.size(); ++i) {
              if (isdigit(s[i])) {++size; num += s[i];}
              else break;
            }
            out.push(num);
            RPNString += num + " ";
            --i;
          } else {
            throw "Wrong operator or operand format! (Введен не тот символ)\n"; 
            break;
          }
        }
      }
    }
    allToOut();
    RPNed = true;
  }
  void calculate() {
    if (isCalculated) {throw "Ответ уже найден!"; return;}
    if (out.empty()) {throw "Не посчитан RPN!"; return;}
    while (!out.empty()) {
      buffer.push(out.top());
      out.pop();
    }

    bool noOperands = true;
    while (!buffer.empty()) {
      const char* a = buffer.top().c_str();
      if (isdigit(*a)) {
        noOperands = false;
        results.push(std::stod(a));
        buffer.pop();
      } else {
        if (noOperands) {throw "No operands entered!!!\n"; return;}
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
          if (results.size() == 0) throw "Wrong operator or operand format! (Оператор - стоит неправильно, нет первого слагаемого)\n";
          double s = results.top(); results.pop();
          if (results.size() == 0) throw "Wrong operator or operand format! (Оператор - стоит неправильно, нет второго слагаемого)\n";
          double f = results.top(); results.pop();
          results.push(f - s);
          buffer.pop();
          break;
        }
        case '*': {
          if (results.size() == 0) throw "Wrong operator or operand format! (Оператор * стоит неправильно, нет первого слагаемого)\n";
          double s = results.top(); results.pop();
          if (results.size() == 0) throw "Wrong operator or operand format! (Оператор * стоит неправильно, нет второго слагаемого)\n";
          double f = results.top(); results.pop();
          results.push(f * s);
          buffer.pop();
          break;
        }
        case '/': {
          if (results.size() == 0) throw "Wrong operator or operand format! (Оператор / стоит неправильно, нет первого слагаемого)\n";
          double s = results.top(); results.pop();
          if (!s) {throw "Zero Division!!!\n"; break;}
          if (results.size() == 0) throw "Wrong operator or operand format! (Оператор / стоит неправильно, нет второго слагаемого)\n";
          double f = results.top(); results.pop();
          results.push(f / s);
          buffer.pop();
          break;
        }
        case '^': {
          if (results.size() == 0) throw "Wrong operator or operand format! (Оператор ^ стоит неправильно, нет первого слагаемого)\n";
          double s = results.top(); results.pop();
          if (results.size() == 0) throw "Wrong operator or operand format! (Оператор ^ стоит неправильно, нет второго слагаемого)\n";
          double f = results.top(); results.pop();
          results.push(pow(f, s));
          buffer.pop();
          break;
        }
        case 'c': {
          if (results.size() == 0) throw "Wrong operator or operand format! (Оператор cos стоит неправильно, нет числа, которое нужно преобразовать)\n";
          double f = results.top(); results.pop();
          results.push(std::cos(f));
          buffer.pop();
          break;
        }
        case 's': {
          if (results.size() == 0) throw "Wrong operator or operand format! (Оператор sin стоит неправильно, нет числа, которое нужно преобразовать)\n";
          double f = results.top(); results.pop();
          results.push(std::sin(f));
          buffer.pop();
          break;
        }
        case 'l': {
          if (results.size() == 0) throw "Wrong operator or operand format! (Оператор ln стоит неправильно, нет числа, которое нужно преобразовать)\n";
          double f = results.top(); results.pop();
          results.push(std::log(f));
          buffer.pop();
          break;
        }
        case '~': {
          if (results.size() == 0) throw "Wrong operator or operand format! (Оператор ~ стоит неправильно, нет числа, которое нужно преобразовать)\n";
          double f = results.top(); results.pop();
          results.push(-f);
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
    answer = results.top();
    isCalculated = true;
  }
  double getAnswer() {if (!isCalculated) throw "Вы не ввели выражение или не посчитали RPN!\n";return answer;}

  std::string& getOut() {return RPNString;}
  std::string& getS() {return s;}

  bool isSEmpty() {return s.empty();}
  bool isAnswerCalculated() {return isCalculated;}
  bool isRPNed() {return RPNed;}
};

int main() {
  Expression ex;
  std::string str;
  std::cout << "Введите новое выражение: ";
  std::cin >> str;
  try {
    ex = str;
    ex.doRPN();
    ex.calculate();
    std::cout << "Выражение: ";
    std::string s = ex.getS();
    for (int i = 0; i < s.size(); ++i) {
      std::cout << s[i];
    }
    std::cout << "\nRPN: ";
    std::string out = ex.getOut();
    for (int i = 0; i < out.size(); ++i) {
      std::cout << out[i];
    }
    double answer = ex.getAnswer();
    std::cout << "\nОтвет: " << answer << std::endl;
  } catch (const char* err) {
    std::cout << "\nОшибка: " << err << std::endl;
  }
  return 0;
}
