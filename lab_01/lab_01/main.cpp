// lab_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <unordered_map>
using namespace std;

int8_t find(vector<int8_t> nums, int8_t num)
{
    for (int i = 0; i < nums.size(); i++)
    {
        if (num == nums[i])
            return i;
    }
}

// Определяем мощность алфавита
const int ALPHABET_SIZE = 16;
//base64
const vector<int8_t> ALPHABET = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
//const string ALPHABET = "0123456789ABCDEF";

class Rotor {
public:
    Rotor(const vector<int8_t> wiring, int position = 0)
    {
        for (int i = position; i < ALPHABET_SIZE; i++)
        {
            wiring_.push_back(wiring[i]);
        }
        for (int i = 0; i < position; i++)
        {
            wiring_.push_back(wiring[i]);
        }
    }

    int8_t encryptForward(int8_t c) {
        int index = find(wiring_, c);
        return ALPHABET[index];
    }

    int8_t encryptBackward(char c) {
        size_t index = find(ALPHABET, c);
        return wiring_[index];
    }

    void rotate() {
        int8_t tmp = wiring_[0];
        for (int i = 0; i < ALPHABET_SIZE - 1; i++)
        {
            wiring_[i] = wiring_[i + 1];
        }
        wiring_[ALPHABET_SIZE - 1] = tmp;
    }

private:
    vector<int8_t> wiring_;

};

class CommutationPanel
{
private:
    vector<int8_t> wiring_;
    vector<int8_t> wiring1;
public:
    CommutationPanel(vector<int8_t> wiring)
    {
        wiring1 = wiring;
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
    int8_t commutation(int8_t c)
    {
        bool flag = false;
        for (int i = 0; i < wiring1.size(); i++)
        {
            if (c == wiring1[i])
            {
                flag = true;
            }
        }
        if (flag)
        {
            return wiring_[find(wiring1, c)];
        }
        else
            return c;
    }
};

class Reflector {
public:
    Reflector(vector<int8_t> wiring)
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

    int8_t reflect(int8_t c) {
        return wiring_[find(ALPHABET, c)];
    }

private:
    vector<int8_t> wiring_;
};

class Enigma {
public:
    Enigma(const vector<Rotor>& rotors, const Reflector& reflector, const CommutationPanel &panel)
        : rotors_(rotors), reflector_(reflector), panel_(panel) {}

    string encryptMessage(const string& message) {
        string encryptedMessage;
        for (char c : message) {
            encryptedMessage += encryptChar(c);
            rotateRotors();
        }
        return encryptedMessage;
    }

    void encryptFile(const string& inputFile, const string& outputFile) {
        ifstream inFile(inputFile, ios::binary);
        ofstream outFile(outputFile, ios::binary);

        if (!inFile.is_open() || !outFile.is_open()) {
            cerr << "Не удалось открыть файл!" << endl;
            return;
        }

        char buffer;
        while (inFile.get(buffer)) {
            buffer = encryptChar(buffer);
            rotateRotors();
            outFile.put(buffer);
        }

        inFile.close();
        outFile.close();
    }

private:
    vector<Rotor> rotors_;
    Reflector reflector_;
    CommutationPanel panel_;
    int cnt_ = 0;

    char encryptChar(char c) {
        int8_t p1 = (c & 0xf0) >> 4;
        int8_t p2 = (c & 0x0f);
        p1 = panel_.commutation(p1);
        for (Rotor& rotor : rotors_) {
            p1 = rotor.encryptForward(p1);
        }
        p1 = reflector_.reflect(p1);

        for (auto it = rotors_.rbegin(); it != rotors_.rend(); ++it) {
            p1 = it->encryptBackward(p1);
        }
        p1 = panel_.commutation(p1);

        p2 = panel_.commutation(p2);

        for (Rotor& rotor : rotors_) {
            p2 = rotor.encryptForward(p2);
        }
        p2 = reflector_.reflect(p2);

        for (auto it = rotors_.rbegin(); it != rotors_.rend(); ++it) {
            p2 = it->encryptBackward(p2);
        }
        p2 = panel_.commutation(p2);

        char res = (p1 << 4) | p2;
        return res;
    }

    void rotateRotors() {
        cnt_++;
        rotors_[0].rotate();
        if (cnt_ % ALPHABET_SIZE == 0)
            rotors_[1].rotate();
        if (cnt_ % (ALPHABET_SIZE * ALPHABET_SIZE) == 0)
            rotors_[2].rotate();

    }
};

// Точка входа
int main() {
    vector<int8_t> r1 = { 11, 1, 2 , 3, 8, 5, 6, 7, 4, 14, 10, 0, 12, 13, 9, 15 };
    vector<int8_t> r2 = { 4, 1, 13 , 3, 0, 5, 6, 10, 8, 9, 7, 11, 12, 2, 14, 15 };
    vector<int8_t> r3 = { 5, 1, 2 , 3, 4, 0, 8, 7, 6, 10, 9, 14, 12, 13, 11, 15 };
    vector<int8_t> refl = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9 , 10, 11, 12, 13, 14, 15};
    vector<int8_t> p= { 1, 4, 7, 3 };

    Rotor rotor1(r1, 0);
    Rotor rotor2(r2, 0);
    Rotor rotor3(r3, 0);


    Reflector reflector(refl);
    CommutationPanel panel(p);

    vector<Rotor> rotors = { rotor1, rotor2, rotor3 };
    Enigma enigma(rotors, reflector, panel);
    Enigma enigma1(rotors, reflector, panel);

    // Пример шифрования текстового сообщения

    string message = "HelloEnigmaFromMoskva";
    string encryptedMessage = enigma.encryptMessage(message);
    cout << "Encrypted message: " << encryptedMessage << endl;

    cout << "Encrypted message: " << enigma1.encryptMessage(encryptedMessage) << endl;

    // //Пример шифрования файла
    //enigma.encryptFile("input.txt", "output.txt");
    //enigma1.encryptFile("output.txt", "output1.txt");

    ////Пример шифрования файла
    //enigma.encryptFile("test.zip", "test1.pdf");
    //enigma1.encryptFile("test1.pdf", "test1.zip");
    //enigma.encryptFile("a.pdf", "test2.enc");
    //enigma.encryptFile("test2.enc", "b.pdf");



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
//    vector<int> input = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
//
//    // Создание случайного генератора на основе текущего времени
//    default_random_engine generator(static_cast<unsigned>(time(0)));
//
//    // Перемешивание строки
//    shuffle(input.begin(), input.end(), generator);
//
//    // Вывод перемешанной строки
//    for (int i = 0; i < input.size(); i++)
//    {
//        cout << input[z
//    }
//
//    return 0;
//}