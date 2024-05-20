#include <iostream>
#include <list>
#include <cstdlib>
using namespace std;

// Создаем структуру ход - откуда и куда перемещаем кольцо
struct Move {
    int from, to;
};

/*
Сделаем функцию, которая будет заполнять список moves
ходами для перемещения n колец с стержня from на 
стержень to через вспомогателый стержень help
*/
void hanoi(int n, int from, int to, int help, std::list<Move>& moves) {
    if (n > 0) {
        // Перемещаем n-1 кольцо на вспомогательный стержень
        hanoi(n - 1, from, help, to, moves);
        // Перемещаем самое большое кольцо на конечный стержень
        moves.push_back({from, to});
        // Перемещаем n-1 кольцо с вспомогательного стержня на конечный
        hanoi(n - 1, help, to, from, moves);
    }
}

int main(int argc, char *argv[]) {
    // Проверка количества аргументов
    if(argc != 2) {
        cout << "Какоеå то странное количество колец" << endl;
        return 1;
    }

    int n = atoi(argv[1]); // количество колец
    list<Move> moves;   // создаем двусвязный список для хранения ходов
    hanoi(n, 1, 3, 2, moves);
    // Выводим все ходы
    for (const Move move : moves) {
        cout << "Move from " << move.from << " to " << move.to << endl;
    }

    moves.clear(); // освобождение памяти списка перед завершением программы

    return 0;
}