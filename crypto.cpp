#include <iostream>
#include <random>
#include <limits>
#include <fstream>
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


void shamirProtocol(string& inNameFile) {
    string text;
    ifstream infile(inNameFile, ios::binary);
    if (!infile) {
        cerr << "Ошибка открытия файла!" << endl;
        return;
    }
    char symbol;
    while (infile.get(symbol)) {
        text += symbol;
    }
    infile.close();
    int p = generatePrime(1000, 10000);
    cout << "Простое число p = " << p << endl;
    int Ca = generateSecretKey(p);
    int Da = searchReciprocalNum(Ca, p - 1);
    cout << "Ключи Алисы: Ca = " << Ca << ", Da = " << Da << endl;
    int Cb = generateSecretKey(p);
    int Db = searchReciprocalNum(Cb, p - 1);
    cout << "Ключи Боба: Cb = " << Cb << ", Db = " << Db << endl;
    vector<int> x1Value, x2Values, x3Values,x4Values;
    cout << "\nШаг 1: Алиса вычисляет x1 = m^Ca mod p" << endl;
    for (char c : text) {
        int m = static_cast<int>(c);
        int x1 = powMod(m, Ca, p);
        x1Value.push_back(x1);
    }
    string x1NameFile;
    cout << "Введите имя файла для сохранения x1: ";
    cin >> x1NameFile;
    ofstream outputX1(x1NameFile, ios::binary);
    if (!outputX1) {
        cerr << "Ошибка создания файла x1" << endl;
        return;
    }
    for (int x1 : x1Value) {
        outputX1.write(reinterpret_cast<const char*>(&x1), sizeof(x1));
    }
    outputX1.close();
    cout << "Промежуточный результат x1 сохранен в файл: " << x1NameFile << endl;
    cout << "Шаг 2: Боб вычисляет x2 = x1^Cb mod p" << endl;
    for (int x1 : x1Value) {
        int x2 = powMod(x1, Cb, p);
        x2Values.push_back(x2);
    }
    string x2NameFile;
    cout << "Введите имя файла для сохранения x2: ";
    cin >> x2NameFile;
    ofstream outputX2(x2NameFile, ios::binary);
    if (!outputX2) {
        cerr << "Ошибка создания файла x2!" << endl;
        return;
    }
    for (int x2 : x2Values) {
        outputX2.write(reinterpret_cast<const char*>(&x2), sizeof(x2));
    }
    outputX2.close();
    cout << "Промежуточный результат x2 сохранен в файл: " << x2NameFile << endl;
    cout << "Шаг 3: Алиса вычисляет x3 = x2^Da mod p" << endl;
    for (int x2 : x2Values) {
        int x3 = powMod(x2, Da, p);
        x3Values.push_back(x3);
    }
    string x3NameFile;
    cout << "Введите имя файла для сохранения x3: ";
    cin >> x3NameFile;
    
    ofstream outputX3(x3NameFile, ios::binary);
    if (!outputX3) {
        cerr << "Ошибка создания файла x3!" << endl;
        return;
    }
    for (int x3 : x3Values) {
        outputX3.write(reinterpret_cast<const char*>(&x3), sizeof(x3));
    }
    outputX3.close();
    cout << "Промежуточный результат x3 сохранен в файл: " << x3NameFile << endl;
    cout << "Шаг 4: Боб вычисляет x4 = x3^Db mod p" << endl;
    string decryptedText;
    for (int x3 : x3Values) {
        int x4 = powMod(x3, Db, p);
        x4Values.push_back(x4);
        decryptedText += static_cast<char>(x4);
    }
    string x4NameFile;
    cout << "Введите имя файла для сохранения x4: ";
    cin >> x4NameFile;
    
    ofstream outputX4(x4NameFile, ios::binary);
    if (!outputX4) {
        cerr << "Ошибка создания файла x4!" << endl;
        return;
    }
    for (int x4 : x4Values) {
        outputX4.write(reinterpret_cast<const char*>(&x4), sizeof(x4));
    }
    outputX4.close();
    cout << "Итоговый результат x4 сохранен в файл: " << x4NameFile << endl;
    string decryptedFile;
    cout << "Введите имя файла для сохранения расшифрованного текста: ";
    cin >> decryptedFile;
    
    ofstream decOutput(decryptedFile, ios::binary);
    if (!decOutput) {
        cerr << "Ошибка создания файла с расшифрованным текстом!" << endl;
        return;
    }
    decOutput.write(decryptedText.c_str(), decryptedText.size());
    decOutput.close();
    cout << "Расшифрованный текст сохранен в файл: " << decryptedFile << endl;
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
