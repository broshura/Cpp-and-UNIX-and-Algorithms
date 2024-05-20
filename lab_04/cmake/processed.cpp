#include <iostream>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>
#include <chrono>

using namespace std;

float Taras(float x){
    return x * x;
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
    pid_t pid;
    cout << "Параллельные вычисления Taras, twice, затем Tarased_twice:\n";
    for(int repeat : n){
        // Измеряем время выполнения функции
        auto start = chrono::high_resolution_clock::now();
        for (int i = 0; i < repeat; i++){
            // Создаем процессы
            pid = fork();
            if (pid == 0) {
                // Первый процесс
                taras = Taras(x);
                write(Taras_pipe[1], &taras, sizeof(taras));
                close(Taras_pipe[1]);
                exit(0);
            } 
            pid = fork();
            if (pid == 0) {
                // Второй процесс
                twice = Twice(x);
                write(Twice_pipe[1], &twice, sizeof(twice));
                close(Twice_pipe[1]);
                exit(0);
            }
            // Ждем завершения процессов
            wait(NULL);
            wait(NULL);
            // Читаем результаты
            read(Taras_pipe[0], &taras, sizeof(taras));
            read(Twice_pipe[0], &twice, sizeof(twice));
            // Вычисляем tarased_twice
            tarased_twice = Tarased_twice(taras, twice);
            // Закрываем каналы
            close(Taras_pipe[0]);
            close(Twice_pipe[0]);
            close(Taras_pipe[1]);
            close(Twice_pipe[1]);
        }
        auto end = chrono::high_resolution_clock::now();
        auto diff = end - start;
        // Выводим время выполнения и количество итераций
        cout << "Время для " << repeat << " итераций: ";
        cout << std::chrono::duration<double>(diff).count() << " s\n";
    }

    return 0;
}

// Количество процессов в пуле
// const int POOL_SIZE = 2;

// int main(){   
//     // Значение для вычислений
//     float x = 11;
//     std::vector<int> n = {10000, 100000};

//     float taras, twice, tarased_twice;

//     // Создаем каналы для обмена данными
//     int Taras_pipe[2], Twice_pipe[2];
//     int parents_cnt = 0;
//     int taras_cnt = 1;
//     int twice_cnt = 1;
//     int repeat = 0;

//     pipe(Taras_pipe);
//     pipe(Twice_pipe);

//     std::vector<pid_t> pool;

//     // Создаем пул процессов
//     for (int i = 0; i < POOL_SIZE; ++i) {
//         // получаем индекс процесса
//         pid_t pid = fork();
//         if (pid == 0) {
//             // Дочерний процесс
//             if (i == 0) {
//                 // Первый процесс
//                 while(parents_cnt < repeat){
//                     read(Taras_pipe[0], &parents_cnt, sizeof(parents_cnt));
//                     read(Taras_pipe[0], &repeat, sizeof(repeat));
//                     if(parents_cnt == taras_cnt){
//                         taras_cnt++;
//                         taras = Taras(x);
//                         write(Taras_pipe[1], &taras, sizeof(taras));
//                     }
//                 }
//             }
//         }   else if (i == 1) {
//                 // Второй процесс
//                 while(parents_cnt < repeat){
//                     read(Taras_pipe[0], &parents_cnt, sizeof(parents_cnt));
//                     read(Taras_pipe[0], &repeat, sizeof(repeat));
//                     if(parents_cnt == twice_cnt){
//                         twice_cnt++;
//                         twice = Twice(x);
//                         write(Twice_pipe[1], &twice, sizeof(twice));
//                     }
//                 }
//         } else if (pid > 0) {
//             // Родительский процесс
//             pool.push_back(pid);
//         } else {
//             std::cerr << "Ошибка при создании процесса" << std::endl;
//             return 1;
//         }
//     }

//     std::cout << "Параллельные вычисления Taras, twice, затем Tarased_twice:\n";
//     for(int repeat : n){
//         // Здесь код для вычисления taras и twice
//         int cnt = 0;
//         while(cnt < repeat){
//             write(Taras_pipe[1], &cnt, sizeof(taras));

//             read(Taras_pipe[0], &taras, sizeof(taras));
//             read(Twice_pipe[0], &twice, sizeof(twice));
//             tarased_twice = Tarased_twice(taras, twice);
//             cnt++;
//         }
//         std::cout << "Tarased_twice: " << tarased_twice << std::endl;
//     }

//     // Ждем завершения всех процессов в пуле
//     for (pid_t pid : pool) {
//         waitpid(pid, nullptr, 0);
//     }

//     return 0;
// }