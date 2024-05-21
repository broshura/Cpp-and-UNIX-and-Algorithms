#ifndef TARAS_COMMANDS_HPP
#define TARAS_COMMANDS_HPP

#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <fstream>

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
    console_out(const std::string& msg) : message(msg) {}
    // Переопределяем метод execute
    void execute() override {
        cout << message << endl;
    }
};
// Класс для вывода в файл
class file_out : public Command {
    std::string message;
    std::string filename;
public:
    file_out(const std::string& msg, const std::string& fname) : message(msg), filename(fname) {}
    void execute() override {
        ofstream file(filename, ios::app);
        file << message << endl;
    }
};

// Класс для цикла
class loop : public Command {
    int count;
    std::list<std::unique_ptr<Command>> commands;
public:
    loop(int cnt) : count(cnt) {}
    void addCommand(std::unique_ptr<Command> cmd) {
        commands.push_back(std::move(cmd));
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