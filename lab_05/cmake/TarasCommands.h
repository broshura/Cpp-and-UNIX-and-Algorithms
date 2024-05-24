#ifndef TARAS_COMMANDS_H
#define TARAS_COMMANDS_H

#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <fstream>
#include "TarasExpressions.h"

using namespace std;

// Создаем методы для выполнения команд
// базовая команда вызываемая при выполнении - execute
class Command {
public:
    virtual void execute(floats floats_variables) = 0;
    virtual string name() { return "@Command";}
    virtual ~Command() {}
};

// Класс для вывода в консоль
class console_out : public Command {
    // Создаем приватный атрибут message
    string message;
    // Создаем приватный атрибут expMessage
    Expression* expMessage;
public:
    // Конструктор класса 
    console_out(const std::variant<std::string, float_expression>& msg) {
        if (std::holds_alternative<std::string>(msg)) {
            message = std::get<std::string>(msg);
            expMessage = nullptr;
        } else {
            expMessage = new float_expression(std::get<float_expression>(msg));
        }
    }
    console_out(const string& msg) : message(msg), expMessage(nullptr) {}
    // Конструктор класса с использованием выражения
    console_out(Expression* expr) : expMessage(expr) {}
    // Переопределяем метод execute
    void execute(floats floats_variables) override {
        // Если expMessage не равно nullptr
        if (expMessage != nullptr) {
            // Выводим результат выражения
            cout << expMessage->express(floats_variables) << endl;
        } else {
            // Выводим сообщение
        cout << message << endl;
        }
    }
    string name() override { return "@console_out";}
    string getMessage() {
        if (expMessage != nullptr) {
            cout << expMessage->getStrExpression() << endl;
            return expMessage->getStrExpression();
        }
        return message;
    }
};
// Класс для вывода в файл
class file_out : public Command {
    string message;
    string filename;
    Expression* expMessage;
    Expression* expFilename;
public:
    file_out(const std::variant<std::string, float_expression>& msg, string& fname) : filename(fname)  {
        if (std::holds_alternative<std::string>(msg)) {
            message = std::get<std::string>(msg);
            expMessage = nullptr;
        } else {
            expMessage = new float_expression(std::get<float_expression>(msg));
        }
    }
    file_out(const string& msg, std::variant<std::string, float_expression>& fname) : message(msg) {
        if (std::holds_alternative<std::string>(fname)) {
            message = std::get<std::string>(fname);
            expMessage = nullptr;
        } else {
            expMessage = new float_expression(std::get<float_expression>(fname));
        }
    }
    file_out(const std::variant<std::string, float_expression>& msg, const std::variant<std::string, float_expression>& fname) {
        if (std::holds_alternative<std::string>(msg)) {
            message = std::get<std::string>(msg);
            expMessage = nullptr;
        } else {
            expMessage = new float_expression(std::get<float_expression>(msg));
        }

        if (std::holds_alternative<std::string>(fname)) {
            filename = std::get<std::string>(fname);
            expFilename = nullptr;
        } else {
            expFilename = new float_expression(std::get<float_expression>(fname));
        }
    }
    file_out(const string& msg, const string& fname) : message(msg), filename(fname), expMessage(nullptr), expFilename(nullptr) {}
    file_out(const string& msg, Expression* expr): message(msg), expFilename(expr), expMessage(nullptr) {}
    file_out(Expression* expr, const string& fname) : expMessage(expr), filename(fname), expFilename(nullptr) {}
    file_out(Expression* expr1, Expression* expr2) : expMessage(expr1), expFilename(expr2) {}

    void execute(floats floats_variables) override {
        if (expMessage != nullptr) {
            message = to_string(expMessage->express(floats_variables));
        }
        if (expFilename != nullptr) {
            filename = to_string(expFilename->express(floats_variables));
        }
        ofstream file(filename, ios::app);
        file << message << endl;
    }
    string name() override { return "@file_out";}
    string getMessage() {
        if (expMessage != nullptr) {
            return expMessage->getStrExpression();
        }
        return message;
    }
    string getFilename() {
        if (expFilename != nullptr) {
            return expFilename->getStrExpression();
        }
        return filename;
    }
};

// Класс для цикла
class loop : public Command {
    int count;
    float_expression expCount;
    list<unique_ptr<Command>> commands;
public:
    loop(int cnt) : count(cnt) {}
    loop(float_expression expr) : expCount(expr) {}
    void addCommand(unique_ptr<Command> cmd) {
        commands.push_back(std::move(cmd));
    }
    void execute(floats floats_variables) override {
        if (expCount.getNumbers().size() > 0) {
            count = expCount.express(floats_variables);
        }
        for (int i = 0; i < count; ++i) {
            for (auto& cmd : commands) {
                cmd->execute(floats_variables);
            }
        }
    }
    string name() override {return "@loop";}
    string getCount() {
        if (expCount.getNumbers().size() > 0){
            return expCount.getStrExpression();
        }
        return to_string(count);
    }
    string getCommands() {
        string result = "";
        for (auto& cmd : commands) {
            result += cmd->name() + " ";
        }
        return result;
    }
};

#endif 