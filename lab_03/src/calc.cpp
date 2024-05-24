#include <iostream>
#include <string>
using namespace std;

// Функция для сложения двух чисел
double add(double a, double b) {
    return a + b;
}

// Функция для вычитания двух чисел
double subtract(double a, double b) {
    return a - b;
}

// Функция для возведения числа a в степень b
double power(double a, double b) {
    double result = 1;
    for(int i = 0; i < b; i++) {
        result *= a;
    }
    return result;
}

// Основная часть программы
int main(int argc, char *argv[]) {
    // Проверка количества аргументов
    if(argc != 4) {
        cout << "Ну это не пример а ужас какой-то" << endl;
        return 1;
    }
    // Получаем аргументы
    double operand1 = stod(argv[1]);
    string operation = argv[2];
    double operand2 = std::stod(argv[3]);
    double result;
    // Выбираем операцию
    if(operation == "plus" || operation == "+") {
        result = add(operand1, operand2);
    } else if(operation == "minus" || operation == "-") {
        result = subtract(operand1, operand2);
    } else if(operation == "power" || operation == "^") {
        result = power(operand1, operand2);
    } else {
        cout << "Я пока не научился пользоваться таким оператором :(" << endl;
        return 1;
    }
    // Выводим результат
    cout << "Result: " << result << endl;

    return 0;
}