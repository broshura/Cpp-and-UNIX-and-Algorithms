from timeit import default_timer as timer
from tqdm import tqdm 
import numba as nb
import py_compile

py_compile.compile('/Users/shuramakarenko/LocalDocs/Studying/Cpp-and-UNIX/lab_02/cmake/main.py', cfile='../build/main.pyc')

'''
Если внимательно посмотреть на функцию, то на самом
деле все слагаемые сокращаются. Поэтому функция названа Taras - 
я тебя породил, я тебя и убью.
'''
@nb.jit(nb.float64(nb.float64), nopython=True, fastmath=True)
def Taras(x):
    return x * x - x * x + x * 4 - x * 5 + x + x

def main(n):
    # Зададим переменные и проверим, что полученный аргумент корректен

    x = 11
    try:
        n = int(n)
    except:
        print("Некорректный формат данных, goodbye")
        return 1
    
    # Повторяем цикл нужное количество раз, фиксируя время
    start = timer()
    for i in range(n):
        Taras(x)
    end = timer()
    print(f'Время: {end - start} s')
    return 0

print("Данная программа повторяет функцию Taras n раз, а затем выводит время выполнения")
n = input("Введите количество повторений: ")
if not main(n):
    print('Хотите повторить эксперимент? (y/n)')
    repeat = input()
    while repeat == 'y':
        n = input("Введите количество повторений: ")
        if not main(n):
            print('Хотите повторить эксперимент? (y/n)')
            repeat = input()
        else:
            break