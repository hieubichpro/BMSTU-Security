// lab_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <unordered_map>
using namespace std;

// Определяем мощность алфавита
const int ALPHABET_SIZE = 64;

// Алфавит из 64 символов
const string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,";

// Класс для одного ротора
class Rotor {
public:
    Rotor(const string wiring, int position = 0)
    {
        string part1 = wiring.substr(0, position);
        string part2 = wiring.substr(position, ALPHABET_SIZE - position);
        wiring_ = part2 + part1;
        //wiring_ = wiring;
    }

    // Проход символа через ротор в прямом направлении
    char encryptForward(char c) {
        int index = wiring_.find(c);
        return ALPHABET[index];
    }

    // Проход символа через ротор в обратном направлении
    char encryptBackward(char c) {
        size_t index = ALPHABET.find(c);
        return wiring_[index];
    }

    // Сдвиг ротора на один шаг
    void rotate() {
        char tmp = wiring_[0];
        for (int i = 0; i < ALPHABET_SIZE - 1; i++)
        {
            wiring_[i] = wiring_[i + 1];
        }
        wiring_[ALPHABET_SIZE - 1] = tmp;
    }

private:
    string wiring_;
};

// Класс для рефлектора
class Reflector {
public:
    Reflector(const string wiring)
    {
        wiring_ = wiring;
        for (int i = 0; i < wiring.size(); i += 1)
        {
            if (i % 2 == 0)
            {
                wiring_[i] = wiring[i + 1];
            }
            else
            {
                wiring_[i] = wiring[i - 1];
            }
        }
    }

    // Проход символа через рефлектор
    char reflect(char c) {
        return wiring_[ALPHABET.find(c)];
    }

private:
    string wiring_;
};

// Класс для шифровальной машины Enigma
class Enigma {
public:
    Enigma(const vector<Rotor>& rotors, const Reflector& reflector)
        : rotors_(rotors), reflector_(reflector) {}

    // Функция для шифрования/дешифрования текстового сообщения
    string encryptMessage(const string& message) {
        string encryptedMessage;
        for (char c : message) {
            if (isInAlphabet(c)) { // Проверяем, что символ в нашем алфавите
                encryptedMessage += encryptChar(c);
                rotateRotors();
            }
            else {
                encryptedMessage += c; // Пропуск символов вне алфавита
            }
        }
        return encryptedMessage;
    }

    // Функция для обработки двоичных файлов
    void encryptFile(const string& inputFile, const string& outputFile) {
        ifstream inFile(inputFile, ios::binary);
        ofstream outFile(outputFile, ios::binary);

        if (!inFile.is_open() || !outFile.is_open()) {
            cerr << "Не удалось открыть файл!" << endl;
            return;
        }

        char buffer;
        while (inFile.get(buffer)) {
            // Шифрование/дешифрование байтов
            if (isInAlphabet(buffer)) {
                buffer = encryptChar(buffer);
                rotateRotors();
            }
            outFile.put(buffer);
        }

        inFile.close();
        outFile.close();
    }

private:
    vector<Rotor> rotors_;
    Reflector reflector_;
    int cnt_ = 0;

    // Шифрование одного символа через все роторы, рефлектор и обратно
    char encryptChar(char c) {
        // Проход вперед через роторы
        for (Rotor& rotor : rotors_) {
            c = rotor.encryptForward(c);
        }

        // Проход через рефлектор
        c = reflector_.reflect(c);

        // Проход обратно через роторы
        for (auto it = rotors_.rbegin(); it != rotors_.rend(); ++it) {
            c = it->encryptBackward(c);
        }
        return c;
    }

    // Поворот роторов при каждом символе
    void rotateRotors() {
        cnt_++;
        rotors_[0].rotate();
        if (cnt_ % ALPHABET_SIZE == 0)
            rotors_[1].rotate();
        if (cnt_ % (ALPHABET_SIZE * ALPHABET_SIZE) == 0)
            rotors_[2].rotate();

    }

    // Проверка, что символ находится в алфавите
    bool isInAlphabet(char c) {
        return ALPHABET.find(c) != string::npos;
    }
};

// Точка входа
int main() {
    Rotor rotor1("BDK3O6heLTY5gsGIEd.tCy2H4rJ7o8nP,z9bxQNqZpjwiUfvaSmWMAVXckR10lFu", 10);
    Rotor rotor2("JrECz18.7kds3Q,u4Y6vBqZXWIjyRoGPFNgMchAimV0fw2UOtpxnDaL5HS9lebKT", 25);
    Rotor rotor3("FE9OaQkSnJ3bUA4iVhTZNYjDK5p.8sztfxcWMCdolyGIq6,m7XePwHgB0RLr1v2u", 2);

    Reflector reflector("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,");

    vector<Rotor> rotors = { rotor1, rotor2, rotor3 };
    Enigma enigma(rotors, reflector);
    Enigma enigma1(rotors, reflector);

    //// Пример шифрования текстового сообщения

    //string message = "HelloEnigmaFromMoskva123.";
    //string encryptedMessage = enigma.encryptMessage(message);
    //cout << "Encrypted message: " << encryptedMessage << endl;

    //cout << "Encrypted message: " << enigma1.encryptMessage(encryptedMessage) << endl;

     //Пример шифрования файла
    enigma.encryptFile("input.txt", "output.txt");
    enigma1.encryptFile("output.txt", "output1.txt");


    return 0;
}

//#include <iostream>
//#include <algorithm> // Для shuffle
//#include <random>    // Для default_random_engine
//#include <string>    // Для string
//#include <ctime>     // Для time
//
//int main() {
//    // Исходная строка
//    string input = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,";
//
//    // Создание случайного генератора на основе текущего времени
//    default_random_engine generator(static_cast<unsigned>(time(0)));
//
//    // Перемешивание строки
//    shuffle(input.begin(), input.end(), generator);
//
//    // Вывод перемешанной строки
//    cout << "Перемешанная строка: " << input << endl;
//
//    return 0;
//}