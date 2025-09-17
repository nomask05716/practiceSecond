#include <iostream>
#include <random>
#include <limits>
#include <string>
#include "math.h"
#include "crypto.h"
using namespace std;
int generatePrime(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(min, max);
    
    int prime;
    do {
        prime = dis(gen);
    } while (!isPrime(prime));
    
    return prime;
}

int generateSecretKey(int p) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(2, p - 2);  
    int key;
    do {
        key = dis(gen);
    } while (gcd(key, p - 1) != 1); 
    return key;
}


void shamirProtocol() {
    int p = generatePrime(1000, 10000);
    cout << "Простое число p = " << p << endl;
    int Ca = generateSecretKey(p);
    int Da = searchReciprocalNum(Ca, p - 1);
    cout << "Ключи Алисы: Ca = " << Ca << ", Da = " << Da << endl;
    int Cb = generateSecretKey(p);
    int Db = searchReciprocalNum(Cb, p - 1);
    cout << "Ключи Боба: Cb = " << Cb << ", Db = " << Db << endl;
    string message;
    cout << "\nВведите сообщение для шифрования: ";
    
    getline(cin, message);
    vector<int> encrypted;
    cout << "\nПроцесс шифрования:" << endl;
    for (char c : message) {
        int m = static_cast<int>(c);
        int x1 = powMod(m, Ca, p);
        cout << "Символ '" << c << "' -> Алиса: " << m << "^" << Ca << " mod " << p << " = " << x1 << endl;
        int x2 = powMod(x1, Cb, p);
        cout << "Боб: " << x1 << "^" << Cb << " mod " << p << " = " << x2 << endl;
        int x3 = powMod(x2, Da, p);
        cout << "Алиса: " << x2 << "^" << Da << " mod " << p << " = " << x3 << endl;
        encrypted.push_back(x3);
    }
    cout << "\nЗашифрованное сообщение: ";
    for (int num : encrypted) {
        cout << num << " ";
    }
    cout << endl;
    string decrypted;
    cout << "\nПроцесс дешифрования:" << endl;
    for (int x : encrypted) {
        int m = powMod(x, Db, p);
        cout << "Боб: " << x << "^" << Db << " mod " << p << " = " << m << " -> '" << static_cast<char>(m) << "'" << endl;
        decrypted += static_cast<char>(m);
    }
    cout << "\nРасшифрованное сообщение: " << decrypted << endl;
    if (message == decrypted) {
        cout << "✓ Шифрование и дешифрование прошли успешно!" << endl;
    } else {
        cout << "✗ Ошибка в процессе шифрования/дешифрования" << endl;
    }
}
void mitmAttack() {
    int p = generatePrime(1000, 10000);
    cout << "Простое число p = " << p << endl;
    int Ca = generateSecretKey(p);
    int Da = searchReciprocalNum(Ca, p - 1);
    cout << "Ключи Алисы: Ca = " << Ca << ", Da = " << Da << endl;
    int Cb = generateSecretKey(p);
    int Db = searchReciprocalNum(Cb, p - 1);
    cout << "Ключи Боба: Cb = " << Cb << ", Db = " << Db << endl;
    int Cm = generateSecretKey(p);
    int Dm = searchReciprocalNum(Cm, p - 1);
    cout << "Ключи Мэллари: Cm = " << Cm << ", Dm = " << Dm << endl;
    
    string message;
    cout << "\nВведите сообщение для шифрования: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, message);
    
    cout << "\n=== ЭМУЛЯЦИЯ АТАКИ MitM ===" << endl;
    cout << "1. Мэллари перехватывает сообщение от Алисы к Бобу" << endl;
    cout << "2. Мэллари подменяет открытые ключи своими" << endl;
    vector<int> intercepted;
    vector<int> malloryDecrypted;
    cout << "\nПроцесс перехвата и подмены:" << endl;
    for (char c : message) {
        int m = static_cast<int>(c);
        int x1 = powMod(m, Ca, p);
        cout << "Алиса -> Мэллари: " << m << "^" << Ca << " mod " << p << " = " << x1 << endl;
        int x2 = powMod(x1, Dm, p); 
        int malloryMessage = powMod(x2, Cm, p); 
        cout << "Мэллари расшифровал: " << x1 << "^" << Dm << " mod " << p << " = " << x2 << endl;
        cout << "Мэллари знает исходное сообщение: " << static_cast<char>(x2) << endl;
        malloryDecrypted.push_back(x2);
        int x3 = powMod(malloryMessage, Cb, p);
        intercepted.push_back(x3);
        cout << "Мэллари -> Бобу: " << malloryMessage << "^" << Cb << " mod " << p << " = " << x3 << endl;
    }
    cout << "\nМэллари перехватил и расшифровал сообщение: ";
    for (int num : malloryDecrypted) {
        cout << static_cast<char>(num);
    }
    cout << endl;
    string bobDecrypted;
    cout << "\nБоб расшифровывает (думая, что это от Алисы):" << endl;
    for (int x : intercepted) {
        int m = powMod(x, Db, p);
        bobDecrypted += static_cast<char>(m);
        cout << "Боб: " << x << "^" << Db << " mod " << p << " = " << m << " -> '" << static_cast<char>(m) << "'" << endl;
    }
    cout << "\nБоб получил (искаженное) сообщение: " << bobDecrypted << endl;
    cout << "Мэллари успешно перехватил и прочитал сообщение!" << endl;
    string modifiedMessage = "hack";
    cout << "Мэллари изменяет сообщение на: " << modifiedMessage << endl;
    vector<int> modifiedEncrypted;
    for (char c : modifiedMessage) {
        int m = static_cast<int>(c);
        int encrypted = powMod(m, Cm, p); 
        encrypted = powMod(encrypted, Cb, p); 
        modifiedEncrypted.push_back(encrypted);
    }
    
    string finalDecrypted;
    for (int x : modifiedEncrypted) {
        int m = powMod(x, Db, p);
        finalDecrypted += static_cast<char>(m);
    }
    
    cout << "Боб получает подмененное сообщение: " << finalDecrypted << endl;
    cout << "✓ Атака MITM завершена успешно!" << endl;
}
