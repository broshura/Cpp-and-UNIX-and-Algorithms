#include <iostream>
#include <chrono>       // Для измерения времени
#include <sstream>       // Для проверки ввода
using namespace std;

/* 
Если внимательно посмотреть на функцию, то на самом
деле все слагаемые сокращаются. Поэтому функция названа Taras - 
я тебя породил, я тебя и убью.
*/
float Taras(float x){
    return x * x - x * x + x * 4 - x * 5 + x + x;
}
 int main(int argc, char* argv[]){   
    // Объявим переменную х и проверим, что данные корректные
    float x = 11;
    // Проверка на количество аргументов
    if (argc < 2) {
        std::cout << "Недостаточно аргументов, goodbye \n";
        return 1;
    }
    // Проверка на корректность ввода
    std::stringstream ss(argv[1]);
    int n;
    if ((ss >> noskipws >> n).eof() && !ss.fail()) {
        // Ваш код здесь
    } else {
        std::cout << "Некорректный формат данных, goodbye \n";
        return 1;
    }

    // Теперь когда все чисто запускаем там - считаем изменение времени
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n ; i++)
        Taras(x); 
    auto end = chrono::high_resolution_clock::now();
    std::chrono::duration<float> diff = end - start;
    // Выводим время и спрашиваем, будут ли дальнейшие итерации
    std::cout << "Время " << std::chrono::duration<double>(diff).count() << " s\n";

    // Запрашиваем еще итерации
    std::cout << "Хотите продолжить? (y/n) \n";
    char answer;
    std::cin >> answer;
    while (answer == 'y') {
        std::cout << "Введите количество итераций: \n";
        std::cin >> n;
        // Проверка на корректность ввода
        if (std::cin.fail()) {
            std::cin.clear(); // очистить флаг ошибки
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // очистить ввод
            std::cout << "Некорректный ввод, goodbye \n";
            return 1;
        }
        std::cout << "Введите значение x: \n";
        std::cin >> x;
        // Проверка на корректность ввода
        if (std::cin.fail()) {
            std::cin.clear(); // очистить флаг ошибки
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // очистить ввод
            std::cout << "Некорректный ввод, goodbye \n";
            return 1;
        }

        // Теперь когда все чисто измеряем время

        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < n ; i++)
            Taras(x); 
        end = chrono::high_resolution_clock::now();
        diff = end - start;
        // Выводим время и спрашиваем, будут ли дальнейшие итерации
        std::cout << "Время " << std::chrono::duration<double>(diff).count() << " s\n";
        std::cout << "Хотите продолжить? (y/n) \n";
        std::cin >> answer;
    }
    std::cout << "Goodbye \n";
    return 0;
}