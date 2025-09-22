#include <iostream>
#include "validInput.h"
#include <cassert>
using namespace std;
int ValidInputMainMenu()
{
    while (true)
    {
        try
        {
            cout << "Введите номер задания: \n1 - a^x mod p. \n2 - с*d mod m=1 (через u и v). \n3 - с^-1 mod m=d. \n4 - Криптопротокол Шамира. \n5 - Цепная дробь && диофантово уравнение. \n6 - МitM \nВаш выбор >>";
            string input;
            getline(cin, input);
            size_t pos = 0;
            int numAlgo = stoi(input, &pos);
            if (pos != input.length())
            {
                throw runtime_error("Ошибка: введите целое число без лишних символов.");
            }
            if (numAlgo < 1)
                throw runtime_error("Ошибка: Номер алгоритма начинается с 1.");
            if (numAlgo > 6)
                throw runtime_error("Ошибка: Всего 6 алгоритмов.");
            return numAlgo; 
        }
        catch (const invalid_argument &)
        {
            cout << "Ошибка: введено не число. Попробуйте ещё раз." << endl;
        }
        catch (const runtime_error &e)
        {
            cout << e.what() << endl;
        }
    }
}
int exp()
{
    while (true)
    {
        try
        {
            cout << "Введите число>>";
            string input;
            getline(cin, input);
            size_t pos = 0;
            int numAlgo = stoi(input, &pos);
            if (pos != input.length())
            {
                throw runtime_error("Ошибка: введите целое число без лишних символов.");
            }
            return numAlgo; 
        }
        catch (const invalid_argument &)
        {
            cout << "Ошибка: введено не число. Попробуйте ещё раз." << endl;
        }
        catch (const runtime_error &e)
        {
            cout << e.what() << endl;
        }
    }
}
int degree()
{
    while (true)
    {
        try
        {
            cout << "Введите степень числа >>";
            string input;
            getline(cin, input);
            size_t pos = 0;
            int numAlgo = stoi(input, &pos);
            if (pos != input.length())
            {
                throw runtime_error("Ошибка: введите целое число без лишних символов.");
            }

            return numAlgo; 
        }
        catch (const invalid_argument &)
        {
            cout << "Ошибка: введено не число. Попробуйте ещё раз." << endl;
        }
        catch (const runtime_error &e)
        {
            cout << e.what() << endl;
        }
    }
}
int modul()
{
    while (true)
    {
        try
        {
            cout << "Введите модуль >>";
            string input;
            getline(cin, input);
            size_t pos = 0;
            int numAlgo = stoi(input, &pos);
            if (pos != input.length())
            {
                throw runtime_error("Ошибка: введите целое число без лишних символов.");
            }
            return numAlgo; 
        }
        catch (const invalid_argument &)
        {
            cout << "Ошибка: введено не число. Попробуйте ещё раз." << endl;
        }
        catch (const runtime_error &e)
        {
            cout << e.what() << endl;
        }
    }
}
string validInputPathFile()
{
    while (true)
    {
        try
        {
            cout << "Введите полный путь до файла(если в одной директории,то достаточно названия файла) >>";
            string input;
            getline(cin, input);
            for (auto symbol : input){
                if (symbol == '/'){
                    throw runtime_error("Ошибка. В пути не должно содержаться '/'");
                } 
            }
            return input;
            
        }
        catch (const runtime_error &e)
        {
            cout << e.what() << endl;
        }
    }
}