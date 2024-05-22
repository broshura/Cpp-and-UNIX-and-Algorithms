#ifndef TARAS_COMMANDS_H
#define TARAS_COMMANDS_H

#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <fstream>
#include <TarasExpressions.h>

using namespace std;

// Создаем методы для выполнения команд
// базовая команда вызываемая при выполнении - execute
class Command {
public:
    virtual void execute() = 0;
};

// Класс для вывода в консоль
class console_out : public Command {
    // Создаем приватный атрибут message
    std::string message;
public:
    // Конструктор класса 
    console_out(const string& msg) : message(msg) {}
    // Переопределяем метод execute
    void execute() override {
        cout << message << endl;
    }
};
// Класс для вывода в файл
class file_out : public Command {
    string message;
    string filename;
public:
    file_out(const string& msg, const string& fname) : message(msg), filename(fname) {}
    void execute() override {
        ofstream file(filename, ios::app);
        file << message << endl;
    }
};

// Класс для цикла
class loop : public Command {
    int count;
    std::list<unique_ptr<Command>> commands;
public:
    loop(int cnt) : count(cnt) {}
    void addCommand(unique_ptr<Command> cmd) {
        commands.push_back(move(cmd));
    }
    void execute() override {
        for (int i = 0; i < count; ++i) {
            for (auto& cmd : commands) {
                cmd->execute();
            }
        }
    }
};

#endif 