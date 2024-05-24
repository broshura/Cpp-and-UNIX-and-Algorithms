#include <iostream>
#include <cmath>        // Для возведения в степень
#include <chrono>       // Для измерения времени
#include <vector>       // Для хранения данных

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

    float taras;
    float twice;
    float tarased_twice;

    // Проводим вычисления

    cout << "Последовательные вычисления Taras, twice и Tarased_twice:\n";
    for(int repeat : n){
        // Измеряем время выполнения пустого цикла
        auto start = chrono::high_resolution_clock::now();
        for (int i = 0; i < repeat; i++){}
        auto end = chrono::high_resolution_clock::now();
        auto zero = end - start;
        // Измеряем время выполнения функции
        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < repeat; i++){
            taras = Taras(x);
            twice = Twice(x);
            tarased_twice = Tarased_twice(taras, twice);
        }
        end = chrono::high_resolution_clock::now();
        auto diff_full = end - start - zero;

        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < repeat; i++){
            taras = Taras(x);
        }
        end = chrono::high_resolution_clock::now();
        auto diff_taras = end - start - zero;

        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < repeat; i++){
            twice = Twice(x);
        }
        end = chrono::high_resolution_clock::now();
        auto diff_twice = end - start - zero;

        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < repeat; i++){
            tarased_twice = Tarased_twice(taras, twice);
        }
        end = chrono::high_resolution_clock::now();
        auto diff_tarased_twice = end - start - zero;

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

//  int main(){   
//     // Значение для вычислений
//     float x = 11;
//     vector<int> n = {10000, 100000};

//     float taras;
//     float twice;
//     float tarased_twice;

//     // Проводим вычисления

//     cout << "Последовательные вычисления Taras, twice и Tarased_twice:\n";
//     for(int repeat : n){
//         // Измеряем время выполнения функции
//         auto start = chrono::high_resolution_clock::now();
//         auto end = chrono::high_resolution_clock::now();

//         auto diff_taras = start - start;
//         auto diff_twice = start - start;
//         auto diff_tarased_twice = start - start;

//         auto start_full = chrono::high_resolution_clock::now();
//         for (int i = 0; i < repeat; i++){
//             // Измеряем время выполнения функций
//             start = chrono::high_resolution_clock::now();
//             taras = Taras(x);
//             end = chrono::high_resolution_clock::now();

//             diff_taras = diff_taras +  end - start;

//             start = chrono::high_resolution_clock::now();
//             twice = Twice(x);
//             end = chrono::high_resolution_clock::now();

//             diff_twice = diff_twice + end - start;

//             start = chrono::high_resolution_clock::now();
//             tarased_twice = Tarased_twice(taras, twice);
//             end = chrono::high_resolution_clock::now();

//             diff_tarased_twice = diff_tarased_twice + end - start;

//         }
//         auto end_full = chrono::high_resolution_clock::now();
//         auto diff_full = end_full - start_full;
//         // Выводим время выполнения и количество итераций
//         cout << "Время для " << repeat << " итераций для Taras: ";
//         cout << std::chrono::duration<double>(diff_taras).count() << " s\n";
    
//         cout << "Время для " << repeat << " итераций для twice: ";
//         cout << std::chrono::duration<double>(diff_twice).count() << " s\n";

//         cout << "Время для " << repeat << " итераций для Tarased_twice: ";
//         cout << std::chrono::duration<double>(diff_tarased_twice).count() << " s\n";

//         cout << "Время для " << repeat << " итераций для полного алгоритма: ";
//         cout << std::chrono::duration<double>(diff_full).count() << " s\n";
//     }

//     return 0;
// }