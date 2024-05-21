#include <TarasCommands.h>
#include <sstream>  

using namespace std;

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
    string line;
    loop mainLoop(1);
    return 0
}