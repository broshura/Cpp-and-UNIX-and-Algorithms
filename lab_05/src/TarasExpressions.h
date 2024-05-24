#ifndef TARAS_EXPRESSIONS_H
#define TARAS_EXPRESSIONS_H

#include <sstream>
#include <list>
#include <algorithm>
#include <stdexcept>

using namespace std;
/*
На данный момент в языке поддерживаются только вычисления для чисел
с плавающей точкой и хранение переменных, их содержащих.
*/

// Класс для переменной с плавающей точкой
class float_variable {
    string name;
    float value;
public:
    // Создаем переменную зная значение
    float_variable(const string& name, float value) : name(name), value(value) {}
    // Создаем переменную не зная значение (фиксируем ноль)
    float_variable(const std::string& name) : name(name), value(0.0f) {}
    
    string getName() const { return name; }
    string valueToString(const float_variable& var) {
        return to_string(var.getValue());
    }
    float getValue() const { return value; }
    void setValue(float newValue) { value = newValue; }
    
};

// Класс для хранения всех переменных
class floats {
    list<float_variable> variables;
public:
    floats(list<float_variable> vars) : variables(vars) {}
    void addVariable(const float_variable& var) {
        auto it = std::find_if(variables.begin(), variables.end(), 
            [&var](const float_variable& existingVar) { return existingVar.getName() == var.getName(); });
        if (it != variables.end()) {
            // Переменная с таким именем уже существует, переназначаем значение
            it->setValue(var.getValue());
        } else {
            // Переменная с таким именем не найдена, добавляем новую
            variables.push_back(var);
        }
    }

    float getVariableValue(const string& name) const {
        auto it = std::find_if(variables.begin(), variables.end(), 
            [&name](const float_variable& var) { return var.getName() == name; });
        if (it != variables.end()) {
            return it->getValue();
        } else {
            throw std::runtime_error("Невідома змінна, добрий казак");
        }
    }

    void setVariableValue(const std::string& name, float value) {
        auto it = std::find_if(variables.begin(), variables.end(), 
            [&name](const float_variable& var) { return var.getName() == name; });
        if (it != variables.end()) {
            it->setValue(value);
        } else {
            float_variable var(name, value);
            addVariable(var);
        }
    }
    list<float_variable> getVariables() const { return variables; }
    bool isVariableExists(const string& name) const {
        auto it = std::find_if(variables.begin(), variables.end(), 
            [&name](const float_variable& var) { return var.getName() == name; });
        return it != variables.end();
    }
};

// Перевод строки, которая может быть переменной, в число
float toFloat(const string& str, floats& floats_variables) {
    // Выводим все переменные из списка переменных
    // cout << "Переменные: ";
    // for (const float_variable& var : floats_variables.getVariables()) {
    //    cout << var.getName() << " = " << var.getValue() << " ";
    // }
    // cout << endl;
    if (floats_variables.isVariableExists(str)) {
        return floats_variables.getVariableValue(str);
    }
    try {
        return stof(str);
    } catch (const invalid_argument& e) {
        throw runtime_error("Невірний формат числа, добрий казак");
        return 0;
    }
}


// Базовый класс для выражений
class Expression{
public:
    virtual float express(floats& floats_variables) = 0;
    virtual ~Expression() {}
    virtual string getStrExpression() = 0;
};


class float_expression: public Expression {
    list<string> strNumbers;
    list<float> numbers;
    list<char> operators;
public:
    float_expression(string& strExp) {
        size_t pos = strExp.find_first_of("+-*/");
        string strNumber;
        char op;
        if (pos == string::npos) {
            strNumbers.push_back(strExp);
        } else {
        while (pos != string::npos) {
            // Получаем оператор
            op = strExp[pos];
            operators.push_back(op);
            // Получаем число
            strNumber = strExp.substr(0, pos);
            strNumbers.push_back(strNumber);
            // Удаляем число и оператор из строки
            strExp = strExp.substr(pos + 1);
            pos = strExp.find_first_of("+-*/");
        }
        strNumbers.push_back(strExp);
        }
    }
    float_expression(const list<float>& numbers, const list<char>& operators): numbers(numbers), operators(operators) {}
    float_expression() {}
    void addNumber(float number) {
        numbers.push_back(number);
    }

    void addOperator(char op) {
        if (op != '+' && op != '-' && op != '*' && op != '/') {
            throw std::invalid_argument("Невидома операція, добрий казак");
        }
        operators.push_back(op);
    }

    virtual float express(floats& floats_variables) override {
        //Выводим все числа и операторы
        // cout << endl;
        // cout << "Числа: ";
        // for (const float& num : numbers) {
        //     cout << num << " ";
        // }
        // cout << endl;
        // cout << "Оператори: ";
        // for (const char& op : operators) {
        //     cout << op << " ";
        // }
        // cout << endl;

        if (strNumbers.size() != 0 && numbers.size() == 0){
            for (const string& strNum : strNumbers) {
                numbers.push_back(toFloat(strNum, floats_variables));
            }
        }
        if (operators.size() != numbers.size() - 1) {
            throw std::runtime_error("Невірна кількість чисел та операцій, добрий казак");
        }
        // Создаем копии списков чисел и операторов
        list<float> tempNumbers = numbers;
        list<char> tempOperators = operators;

        auto numIt = tempNumbers.begin();
        auto opIt = tempOperators.begin();

        // Первый проход: выполняем операции умножения и деления
        while (opIt != tempOperators.end()) {
            if (*opIt == '*' || *opIt == '/') {
                auto nextNumIt = std::next(numIt);
                if (*opIt == '*') {
                    *numIt *= *nextNumIt;
                } else {
                    if (*nextNumIt == 0) {
                        throw std::runtime_error("Ділення на нуль, добрий казак");
                    }
                    *numIt /= *nextNumIt;
                }
                tempNumbers.erase(nextNumIt);
                opIt = tempOperators.erase(opIt);
            } else {
                ++numIt;
                ++opIt;
            }
        }

        // Второй проход: выполняем операции сложения и вычитания
        numIt = tempNumbers.begin();
        opIt = tempOperators.begin();
        float result = *numIt;
        ++numIt;

        while (numIt != tempNumbers.end()) {
            if (*opIt == '+') {
                result += *numIt;
            } else if (*opIt == '-'){
                result -= *numIt;
            } else {
                throw std::invalid_argument("Невидома операція, добрий казак");
            }
            ++numIt;
            ++opIt;
        }
        return result;
    }
    virtual string getStrExpression() override {
        std::stringstream ss;
        auto numIt = numbers.begin();
        auto opIt = operators.begin();
        ss << *numIt;
        ++numIt;
        while (numIt != numbers.end()) {
            ss << *opIt << *numIt;
            ++numIt;
            ++opIt;
        }
        return ss.str();
    }
    list<string> getStrNumbers() { 
        if (strNumbers.size() == 0) {
            for (const float& num : numbers) {
                strNumbers.push_back(to_string(num));
            }
        }
        return strNumbers; 
        }
    list<float> getNumbers() { return numbers; }
    list<char> getOperators() { return operators; }
};



#endif
