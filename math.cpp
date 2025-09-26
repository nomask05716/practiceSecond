#include <iostream>
#include <cmath>
#include<vector>
#include "math.h"
#include <cassert>
using namespace std;
bool isPrime(int number){
    if (number <= 1){
        return false;
    }
    if (number == 2){
        return true;
    }
    if (number % 2 ==0){
        return false;
    }
    for (int num=3;num <= sqrt(number);num += 2){
        if (number % num ==0){
            return false;
        }
    }
    return true;
}

int powMod(int a,int b,int m){
    int result =1;
    a = a %m;
    while (b>0){
        //cout << "Степень = "<<b<<endl;
        if (b % 2 ==1){
            result = (a *result)%m; 
        }
        
        a = (a*a)%m;
        //cout << "a = "<<a<<endl;
        b /= 2;
        //cout << "Промежуточный результат = "<<result<<endl;

        
    }
    return result;
}
int gcd(int a,int b){
    while(b !=0){
        int tmp =b;
        b = a %b;
        a =tmp;
    }
    return a;
}
bool threoremFerma(int a,int b,int p){
    if (isPrime(p)==1 && (gcd(a,p)==1) && (p-b ==1)){
        return 1;
    }else if (gcd(a,p)!=1)
    {
        cout << "Число a должно быть взаимно простым с p"<<endl;
        return 0;
    }else{
        return 0;
    }
    
}
int extendedGcd(int a, int b, int &u, int &v) {
    if (b == 0) {
        u = 1;
        v = 0;
        return a;
    }
    int u1, v1;
    cout <<"extendedGcd(" << b << ", " << a % b << ")" << endl;
    int gcd = extendedGcd(b, a % b, u1, v1);
    cout <<"u1 = " << u1 << ", v1 = " << v1 << endl;
    u = v1;
    v = u1 - (a / b) * v1;
    cout <<"Вычисляем новые коэффициенты:" << endl;
    cout <<"u = v1 = " << v1 << endl;
    cout <<"v = u1 - (a/b)*v1 = " << u1 << " -(" << a << "/" << b << ")* " << v1 << " = " << u1 << " - " << (a/b) << "*" << v1 << " = " << v << endl;;
    return gcd;
}
int searchReciprocalNum(int c, int m) {
    if (m <= 0) {
        throw "Модуль должен быть положительным";
    }
    int u, v;
    int gcd = extendedGcd(c, m, u, v);
    if (gcd != 1) {
        throw "Уравнение не имеет решения: числа не взаимно просты";
    }
    int d = (u % m + m) % m;
    return d;
}
vector<int> chainShotes(int a, int b) {
    vector<int> coefficients;
    while (b != 0) {
        coefficients.push_back(a / b);
        cout << "Целая часть: " << a << " / " << b << " = " << a / b << endl;
        int remainder = a % b;
        cout << "Остаток: " << a << " % " << b << " = " << remainder << endl;
        int temp = b;
        b = remainder;
        a = temp;
        cout << "Новые значения: a = " << a << ", b = " << b << endl;
    }
    return coefficients;
}
void continuedFractionValues(const vector<int>& cf, vector<int>& P, vector<int>& Q) {
    P.push_back(1);
    Q.push_back(0);
    P.push_back(cf[0]);
    Q.push_back(1);
    for (size_t k = 1; k < cf.size(); k++) {
        P.push_back(cf[k] * P[k] + P[k-1]);
        Q.push_back(cf[k] * Q[k] + Q[k-1]);
    }
}
void solveDiophantine(int a, int b, int c) {
    cout << "Решаем диофантово уравнение: " << a << " * x + " << b << " * y = " << c << endl;
    vector<int> cf = chainShotes(a, b);
    cout << "Цепная дробь для " << a << "/" << b << ": [";
    bool first = true;
    for (const auto& q : cf) {
        if (!first) cout << ", ";
        cout << q;
        first = false;
    }
    cout << "]" << endl;
    vector<int> P, Q;
    continuedFractionValues(cf, P, Q);
    size_t n = cf.size();
    int Qn_1 = Q[n-2];
    int Pn_1 = P[n-2];
    int x0, y0;
    if (n % 2 == 0) {
        x0 = c * Qn_1;
        y0 = -c * Pn_1;
    } else {
        x0 = -c * Qn_1;
        y0 = c * Pn_1;
    }
    cout << "Частное решение: x0 = " << x0 << ", y0 = " << y0 << endl
         << "Общее решение: " << endl
         << "x = " << x0 << " + " << b << " * t" << endl
         << "y = " << y0 << " - " << a << " * t" << endl
         << "где t ∈ ℤ" << endl;
}

void fractionAndEquation() {
    int a =275, b = 145, c = 10;
    solveDiophantine(a, b, c);
}


