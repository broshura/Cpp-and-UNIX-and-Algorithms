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
        // Измеряем время выполнения пустого цикла
        auto start = chrono::high_resolution_clock::now();
        for (int i = 0; i < repeat; i++){}
        auto end = chrono::high_resolution_clock::now();
        auto zero = end - start;
        // Измеряем время выполнения функции

        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < repeat; i++){
            thread taras_thread([&taras, x](){ taras = Taras(x); });
            taras_thread.join();
        }
        end = chrono::high_resolution_clock::now();
        auto diff_taras = end - start - zero;

        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < repeat; i++){
            thread twice_thread([&twice, x](){ twice = Twice(x); });
            twice_thread.join();
        }
        end = chrono::high_resolution_clock::now();
        auto diff_twice = end - start - zero;

        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < repeat; i++){
            tarased_twice = Tarased_twice(taras, twice);
        }
        end = chrono::high_resolution_clock::now();
        auto diff_tarased_twice = end - start - zero;

        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < repeat; i++){
            thread taras_thread([&taras, x](){ taras = Taras(x); });
            thread twice_thread([&twice, x](){ twice = Twice(x); });
            taras_thread.join();
            twice_thread.join();
            tarased_twice = Tarased_twice(taras, twice);
        }
        end = chrono::high_resolution_clock::now();
        auto diff_full = end - start - zero;
        // Выводим время выполнения и количество итераций
        cout << "Время для " << repeat << " итераций для Taras: ";
        cout << std::chrono::duration<double>(diff_taras).count() << " s\n";
    
        cout << "Время для " << repeat << " итераций для twice: ";
        cout << std::chrono::duration<double>(diff_twice).count() << " s\n";

        cout << "Время для " << repeat << " итераций для Tarased_twice: ";
        cout << std::chrono::duration<double>(diff_tarased_twice).count() << " s\n";

        cout << "Время для " << repeat << " итераций для полного алгоритма: ";
        cout << std::chrono::duration<double>(diff_full).count() << " s\n";
    }
    return 0;
}