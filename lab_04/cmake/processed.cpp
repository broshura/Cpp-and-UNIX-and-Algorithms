#include <iostream>
// Для создания процессов
#include <unistd.h>  
#include <sys/wait.h>
#include <chrono>       // Для измерения времени
#include <vector>       // Для хранения данных
#include <thread>

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

    // Создаем каналы для обмена данными
    int Taras_pipe[2], Twice_pipe[2];

    pipe(Taras_pipe);
    pipe(Twice_pipe);
    // Проводим вычисления
    
    cout << "Параллельные вычисления Taras, twice, затем Tarased_twice:\n";
    for(int repeat : n){
        // Измеряем время выполнения функции
        auto start = chrono::high_resolution_clock::now();
        for (int i = 0; i < repeat; i++){
            // Создаем процессы
            if (fork() == 0) {
                // Дочерний процесс для Taras
                close(Taras_pipe[0]);
                taras = Taras(x);
                write(Taras_pipe[1], &taras, sizeof(taras));
                close(Taras_pipe[1]);
                exit(0);
            }

            if (fork() == 0) {
                // Дочерний процесс для Twice
                close(Twice_pipe[0]);
                twice = Twice(x);
                write(Twice_pipe[1], &twice, sizeof(twice));
                close(Twice_pipe[1]);
                exit(0);
            }

            // Родительский процесс ждет завершения дочерних процессов
            wait(NULL);
            wait(NULL);

            close(Taras_pipe[1]);
            close(Twice_pipe[1]);

             // Читаем результаты из каналов
            read(Taras_pipe[0], &taras, sizeof(taras));
            read(Twice_pipe[0], &twice, sizeof(twice));

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