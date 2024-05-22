#include <TarasCommands.h>
#include <TarasExpressions.h>
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
std::string removeSpaces(const std::string& str)
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

    if (!isalpha(name[0]) && name[0] != '_') {
        return false;
    }

    for (char c : name) {
        if (!isalnum(c) && c != '_') {
            return false;
        }
    }

    return true;
}
// Перевод строки, которая может быть переменной, в число
float toFloat(const string& str) {
    if (isValidVariableName(str)) {
        return floats_variables.getVariableValue(str);
    }
    try {
        return stof(str);
    } catch (const std::invalid_argument& e) {
        throw runtime_error("Невірний формат числа, добрий казак");
        return 0;
    }
}

//Команда для присвоения значения переменной
class assign_float : public Command {
    string name;
    string strExpression;
public:
    assign_float(const string& var, string exp) : name(var), strExpression(exp) {}
    void execute() override {
        size_t pos = strExpression.find_first_of("+-*/");

        list<float> numbers = {};
        list<char> operators = {};
        if (pos == string::npos) {
            float number = toFloat(strExpression);
            floats_variables.addVariable(float_variable(name, number));
            return;
        }
        while (pos != string::npos) {
            // Получаем оператор
            char op = strExpression[pos];
            operators.push_back(op);
            // Получаем число
            float number = toFloat(strExpression.substr(0, pos));
            numbers.push_back(number);
            // Удаляем число и оператор из строки
            strExpression = strExpression.substr(pos + 1);
            pos = strExpression.find_first_of("+-*/");
        }
        float_expression expression = float_expression(numbers, operators);
        floats_variables.addVariable(float_variable(name, expression.express()));
    }
};

void lineToCommands(string item) {
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
                string name = "@console_out";
                assign_float(name, strExpression).execute();
                float number = floats_variables.getVariableValue(name);
                string message = to_string(number);
                commands.push_back(make_unique<console_out>(message));
            }
        }
        if (commandType == "запиши_до_файлу") {
            if (commandArgs[0] != '|' || commandArgs[commandArgs.size() - 1] != '|') {
                throw runtime_error("Невірний формат аргументiв команди, добрий казак");
            }
            string commandArg = commandArgs.substr(1, commandArgs.size() - 2);
            if (commandArg[0] == '"' && commandArg[commandArg.size() - 1] == '"') {
                string message = commandArgs.substr(1, commandArg.size() - 2);
                commands.push_back(make_unique<file_out>(message));
            } else {
                string strExpression = removeSpaces(commandArg);
                string name = "@file_out";
                assign_float(name, strExpression).execute();
                float number = floats_variables.getVariableValue(name);
                string message = to_string(number);
                commands.push_back(make_unique<file_out>(message));
            }
        }
        if (commandType == "повтори") {
            if (commandArgs[0] != '|' || commandArgs[commandArgs.size() - 1] != '|') {
                throw runtime_error("Невірний формат аргументiв команди, добрий казак");
            }
            string commandArg = commandArgs.substr(1, commandArgs.size() - 2);
            if (commandArg[0] == '"' && commandArg[commandArg.size() - 1] == '"') {
                string message = commandArgs.substr(1, commandArg.size() - 2);
                commands.push_back(make_unique<file_out>(message));
            } else {
                string strExpression = removeSpaces(commandArg);
                string name = "@loop";
                assign_float(name, strExpression).execute();
                float number = floats_variables.getVariableValue(name);
                int count = (int)number;
                loop loopCommand(count);
            }
        }
    
    } else {
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
    // Создаем вектор потоков
    vector<thread> threads;
    // Читаем файл по строкам, разделяя строки по символу '\\'
    while (getline(file, parallel_line, '\\')) {
        // Запускаем каждую отделенную \n строку в отдельном потоке

        stringstream ss(parallel_line);
        while (getline(ss, item, '\n')) {
            item = trim(item);
            // Проверяем пустую строку
            if (item.empty()) {
                continue;
            }
            
            threads.push_back(thread([&]() { 
                // Проверяем, создаем ли мы переменную
                if (item[0] == '#') {
                    size_t typePos = item.find_first_of(" ");
                    if (typePos == string::npos) {
                        cout << "Не можу визначити тип змiнної, добрий казак \n";
                        return 1;
                    } 
                    string varType = item.substr(1, typePos - 1);
                    if (varType == "нехай") {
                        size_t namePos = item.find_first_of("=", typePos + 1);
                        if (namePos == string::npos) {
                            cout << "Не можу визначити ім'я змінної, добрий казак \n";
                            return 1;
                        }
                        string varName = trim(item.substr(typePos + 1, namePos - typePos - 1));
                        if (!isValidVariableName(varName)) {
                            cout << "Невірне ім'я змінної, добрий казак \n";
                            return 1;
                        }
                        string strExpression = removeSpaces(item.substr(namePos + 1));
                        commands.push_back(make_unique<assign_float>(varName, strExpression));
                    } else {
                        cout << "Невідомий тип змінної, добрий казак \n";
                        return 1;
                    }
                }
                else if (item[0] == '@') {
                    // Проверяем синтаксис
                    item = removeSpaces(item);
                    size_t typePos = item.find_first_of("[");
                    if (typePos == string::npos) {
                        cout << "Не можу визначити команду, добрий казак \n";
                        return 1;
                    }
                    if (item.size() < typePos + 2) {
                        cout << "Невірний формат команди, добрий казак \n";
                        return 1;
                    }
                    if (item[typePos]!='[' || item[item.size()-1]!=']') {
                        cout << "Невірний формат команди, добрий казак \n";
                        return 1;
                    }
                    string commandType = item.substr(1, typePos - 1);
                    string commandArgs = item.substr(typePos + 1, item.size() - typePos - 2);
                    // Проверяем, какая именно команда используется
                    if (commandType == "покажи_у_консолi") {
                        if (commandArgs[0] != '|' || commandArgs[commandArgs.size() - 1] != '|') {
                            cout << "Невірний формат аргументiв команди, добрий казак \n";
                            return 1;
                        }
                        string commandArg = commandArgs.substr(1, commandArgs.size() - 2);
                        if (commandArg[0] == '"' && commandArg[commandArg.size() - 1] == '"') {
                            string message = commandArgs.substr(1, commandArg.size() - 2);
                            commands.push_back(make_unique<console_out>(message));
                        } else {
                            string strExpression = removeSpaces(commandArg);
                            string name = "console_out";
                            assign_float(name, strExpression).execute();
                            float number = floats_variables.getVariableValue(name);
                            string message = to_string(number);
                            commands.push_back(make_unique<console_out>(message));
                        }
                    }
                    if (commandType == "запиши_до_файлу") {
                        if (commandArgs[0] != '|' || commandArgs[commandArgs.size() - 1] != '|') {
                            cout << "Невірний формат аргументiв команди, добрий казак \n";
                            return 1;
                        }
                        string commandArg = commandArgs.substr(1, commandArgs.size() - 2);
                        if (commandArg[0] == '"' && commandArg[commandArg.size() - 1] == '"') {
                            string message = commandArgs.substr(1, commandArg.size() - 2);
                            commands.push_back(make_unique<file_out>(message));
                        } else {
                            string strExpression = removeSpaces(commandArg);
                            string name = "console_out";
                            assign_float(name, strExpression).execute();
                            float number = floats_variables.getVariableValue(name);
                            string message = to_string(number);
                            commands.push_back(make_unique<file_out>(message));
                        }
                    }
                    if (commandType == "повтори") {
                        if (commandArgs[0] != '|' || commandArgs[commandArgs.size() - 1] != '|') {
                            cout << "Невірний формат аргументiв команди, добрий казак \n";
                            return 1;
                        }
                        string commandArg = commandArgs.substr(1, commandArgs.size() - 2);
                        if (commandArg[0] == '"' && commandArg[commandArg.size() - 1] == '"') {
                            string message = commandArgs.substr(1, commandArg.size() - 2);
                            commands.push_back(make_unique<file_out>(message));
                        } else {
                            string strExpression = removeSpaces(commandArg);
                            string name = "console_out";
                            assign_float(name, strExpression).execute();
                            float number = floats_variables.getVariableValue(name);
                            string message = to_string(number);
                            commands.push_back(make_unique<file_out>(message));
                        }
                    }


                } else {
                    cout << "Невідомий тип команди, добрий казак \n";
                    return 1;
                }
            }));
        }
        // Ждем завершения всех потоков
        for (auto& th : threads) {
            th.join();
        }
        
    }
    return 0;
}