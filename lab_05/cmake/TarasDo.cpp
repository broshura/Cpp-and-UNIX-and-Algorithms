#include "TarasCommands.h"
#include "TarasExpressions.h"
#include <sstream>  
#include <fstream>
#include <iostream>
#include <thread>
#include <regex>

using namespace std;

list<unique_ptr<Command>> commands;
list<float_variable> variables;
floats floats_variables(variables);

// Функция для удаления пробелов в строке
string removeSpaces(const std::string& str)
{
    std::string s = str;
    s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());
    return s;
}

// Функция для удаления пробелов в начале и конце строки
string trim(const std::string& str)
{
    std::string s = str;
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
    return s;
}

// Функция для удаления символов новой строки в начале и конце строки
string trimNewline(const std::string& str)
{
    std::string s = str;
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return ch != '\n';
    }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return ch != '\n';
    }).base(), s.end());
    return s;
}

// Функция для проверки корректности имени переменной
bool isValidVariableName(const string& name) {
    if (name.empty()) {
        return false;
    }
    return true;
}


//Команда для присвоения значения переменной
class assign_float : public Command {
    string Name;
    float_expression expression;
public:
    assign_float(const string& var, float_expression& exp) : Name(var), expression(exp) {}
    assign_float(const string& var, string strExp) : Name(var), expression(strExp) {}
    void execute(floats floats_variables) override {
        cout << "Присвоєння значення змінній " << Name << " = " << expression.express(floats_variables) << endl;
        cout << "Вираз: " << expression.getStrExpression() << endl;
        floats_variables.addVariable(float_variable(Name, expression.express(floats_variables)));
    }
    string name() override { return "#assign_float"; }
    string getVariable() { return Name; }
    string getStrExpession() { return expression.getStrExpression(); }
    list<string> getStrNumbers() { return expression.getStrNumbers(); }
    list<char> getOperators() { return expression.getOperators(); }
    float_expression getExpression() { return expression; }
};

void lineToCommands(stringstream& ss, string item, int& num_of_threads) {
    // Проверяем, создаем ли мы переменную
    if (item[0] == '#') {
        size_t typePos = item.find_first_of(" ");
        if (typePos == string::npos) {
            throw runtime_error("Не можу визначити тип змiнної, добрий казак");
        } 
        string varType = item.substr(1, typePos - 1);
        if (varType == "нехай") {
            size_t namePos = item.find_first_of("=", typePos + 1);
            if (namePos == string::npos) {
                throw runtime_error("Не можу визначити ім'я змінної, добрий казак");
            }
            string varName = trim(item.substr(typePos + 1, namePos - typePos - 1));
            if (!isValidVariableName(varName)) {
                throw runtime_error("Невірне ім'я змінної, добрий казак");
            }
            string strExpression = removeSpaces(item.substr(namePos + 1));
            commands.push_back(make_unique<assign_float>(varName, strExpression));
            return;
        } else {
            throw runtime_error("Невідомий тип змінної, добрий казак");
        }
    }
    // Проверяем, создаем ли мы команду
    else if (item[0] == '@') {
        // Проверяем синтаксис
        item = removeSpaces(item);
        size_t typePos = item.find_first_of("[");
        if (typePos == string::npos) {
            throw runtime_error("Не можу визначити команду, добрий казак");
        }
        if (item.size() < typePos + 2) {
            throw runtime_error("Невірний формат команди, добрий казак");
        }
        if (item[typePos]!='[' || item[item.size()-1]!=']') {
            throw runtime_error("Невірний формат команди, добрий казак");
        }
        string commandType = item.substr(1, typePos - 1);
        string commandArgs = item.substr(typePos + 1, item.size() - typePos - 2);
        // Проверяем, какая именно команда используется
        if (commandType == "покажи_у_консолi") {
            if (commandArgs[0] != '|' || commandArgs[commandArgs.size() - 1] != '|') {
                throw runtime_error("Невірний формат аргументiв команди, добрий казак");
            }
            string commandArg = commandArgs.substr(1, commandArgs.size() - 2);
            if (commandArg[0] == '"' && commandArg[commandArg.size() - 1] == '"') {
                string message = commandArgs.substr(1, commandArg.size() - 2);
                commands.push_back(make_unique<console_out>(message));
            } else {
                string strExpression = removeSpaces(commandArg);
                float_expression message = float_expression(strExpression);
                commands.push_back(make_unique<console_out>(message));
                return;
            }
        }
        if (commandType == "запиши_до_файлу") {
            if (commandArgs[0] != '|' || commandArgs[commandArgs.size() - 1] != '|') {
                throw runtime_error("Невірний формат аргументiв команди, добрий казак");
            }
            commandArgs = commandArgs.substr(1, commandArgs.size() - 2);
            size_t messagePos = commandArgs.find_first_of("||");
            if (messagePos == string::npos) {
                throw runtime_error("Невірний формат аргументiв команди, добрий казак");
            }
            string arg1 = commandArgs.substr(0, messagePos);
            string arg2 = commandArgs.substr(messagePos + 2);

            std::variant<std::string, float_expression> message;
            std::variant<std::string, float_expression> filename;
            if (arg1[0] == '"' && arg1[arg1.size() - 1] == '"') {
                message = arg1.substr(1, arg1.size() - 2);
            } else {
                string strExpression = removeSpaces(arg1);
                message = float_expression(strExpression);
            } 
            if (arg2[0] == '"' && arg2[arg2.size() - 1] == '"') {
                filename = arg2.substr(1, arg2.size() - 2);
                commands.push_back(make_unique<file_out>(message, filename));
                return;
            } else {
                string strExpression = removeSpaces(arg2);
                filename = float_expression(strExpression);
                commands.push_back(make_unique<file_out>(message, filename));
                return;
            }

        }
        if (commandType == "повтори") {
            if (commandArgs[0] != '|' || commandArgs[commandArgs.size() - 1] != '|') {
                throw runtime_error("Невірний формат аргументiв команди, добрий казак");
            }
            string commandArg = commandArgs.substr(1, commandArgs.size() - 2);

            string strExpression = removeSpaces(commandArg);
            float_expression count = float_expression(strExpression);;
            loop loopCommand(count);
            getline(ss, item, '\n');
            if (item.size() < 4) {
                throw runtime_error("Цикл без команди, добрий казак");
            } else if (item.substr(0, 4) != "    ") {
                throw runtime_error("Цикл без команди, добрий казак");
            } else {
                //cout << "Первая строка цикла: " << item << endl;
                item = trim(item);
                lineToCommands(ss, item, num_of_threads);
                auto it = commands.end();
                it--;
                loopCommand.addCommand(std::move(*it));
                commands.pop_back();
            }
            while (getline(ss, item, '\n')) {
                if (item.substr(0, 4) != "    ") {
                    //cout << "Строка: " << item << endl;
                    if (item.empty()) {
                        continue;
                    }
                    num_of_threads++;
                    lineToCommands(ss, item, num_of_threads);
                    break;
                }
                //cout << "Строка цикла: " << item << endl;
                item = trim(item);
                lineToCommands(ss, item, num_of_threads);
                auto it = commands.end();
                it--;
                loopCommand.addCommand(std::move(*it));
                commands.pop_back();
            }
            commands.push_back(make_unique<loop>(std::move(loopCommand)));
            return;
            }
        else {
                throw runtime_error("Невідома команда, добрий казак");
        }
    }  
    else {
        throw runtime_error("Невідомий тип команди, добрий казак");
    }
}
// Функция для компиляции файла
int main(int argc, char* argv[]){
    if (argc < 2) {
        cout << "Нема чого компiлирувати, добрий казак \n";
        return 1;
    }
    if (argc > 2) {
        cout << "Не розумiю який файл треба компiлируваты, добрий казак \n";
        return 1;
    }
    cout << "Компiляцiя файлу " << argv[1] << endl;
    string filename = argv[1];
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Не можу вiдкрити файл, добрий казак \n";
        return 1;
    }

    string parallel_line;
    string item;
    int number_of_strings = 0;
    vector<int> number_of_threads;
    // Создаем вектор потоков
    vector<thread> threads;
    // Читаем файл по строкам, разделяя строки по символу '\\'
    while (getline(file, parallel_line, '\\')) {
        stringstream ss(parallel_line);
        int num_of_threads = 0;
        // Читаем строку по строке, разделяя строки по символу '\n'
        while (getline(ss, item, '\n')) {
            //cout << "Строка: " << item << endl;
            // Проверяем пустую строку
            if (item.empty()) {
                continue;
            }
            lineToCommands(ss, item, num_of_threads);
            num_of_threads++;
        }
        number_of_threads.push_back(num_of_threads);
        number_of_strings++ ;
    }
    cout << endl;
    // Закрываем файл
    file.close();
    // Запускаем потоки
    cout << "Виконання потокiв: " << endl;
    for (int i = 0; i < number_of_strings; i++) {
        // Запускаем каждую отделенную \n строку в отдельном потоке
        for (int j = 0; j < number_of_threads[i]; j++) {
            auto it = commands.begin();
            advance(it, j);
            threads.push_back(thread([it]() { 
                cout << (*it)->name() << endl;
                (*it)->execute(floats_variables);
            }));
        }
        // Ждем завершения всех потоков
        for (auto& th : threads) {
            if (th.joinable()){
                th.join();
            }
        }
        // Удаляем команды из  списка команд
        for (int j = 0; j < number_of_threads[i]; j++) {
            commands.pop_front();
        }
    }
    return 0;
}