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
};


// Базовый класс для выражений
class Expression{
public:
    virtual float express() = 0;
};


class float_expression: public Expression {
    std::list<float> numbers;
    std::list<char> operators;

public:
    float_expression(const std::list<float>& numbers, const std::list<char>& operators) {}
    void addNumber(float number) {
        numbers.push_back(number);
    }

    void addOperator(char op) {
        if (op != '+' && op != '-' && op != '*' && op != '/') {
            throw std::invalid_argument("Невидома операція, добрий казак");
        }
        operators.push_back(op);
    }

    virtual float express() override {
        if (operators.size() != numbers.size() - 1) {
            throw std::runtime_error("Невірна кількість чисел та операцій, добрий казак");
        }

        // Создаем копии списков чисел и операторов
        std::list<float> tempNumbers = numbers;
        std::list<char> tempOperators = operators;

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
};

#endif
