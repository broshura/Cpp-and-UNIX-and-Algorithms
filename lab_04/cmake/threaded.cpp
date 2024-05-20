#include <iostream>
#include <chrono>       // Для измерения времени
#include <vector>       // Для хранения данных
#include <thread>       // Для создания потоков

using namespace std;

/* 
Если внимательно посмотреть на функцию, то на самом
деле все слагаемые сокращаются. Поэтому функция названа Taras - 
я тебя породил, я тебя и убью.
*/
float Taras(float x){
    return x * x - x * x + x * 4 - x * 5 + x + x;
}

float Twice(float x){
    return x + x;
}

float Tarased_twice(float taras, float twice){
    return taras + twice - taras;
}
 int main(){   
    // Значение для вычислений
    float x = 11;
    vector<int> n = {10000, 100000};

    float taras, twice, tarased_twice;

    // Проводим вычисления
    
    cout << "Параллельные вычисления Taras, twice, затем Tarased_twice:\n";
    for(int repeat : n){
        // Измеряем время выполнения функции
        auto start = chrono::high_resolution_clock::now();
        for (int i = 0; i < repeat; i++){
            thread taras_thread([&taras, x](){ taras = Taras(x); });
            thread twice_thread([&twice, x](){ twice = Twice(x); });
            taras_thread.join();
            twice_thread.join();
            tarased_twice = Tarased_twice(taras, twice);
        }
        auto end = chrono::high_resolution_clock::now();
        auto diff = end - start;
        // Выводим время выполнения и количество итераций
        cout << "Время для " << repeat << " итераций: ";
        cout << std::chrono::duration<double>(diff).count() << " s\n";
    }

    return 0;
}