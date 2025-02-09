#include "../inc/AES.h"
#include "../inc/OFB.h"

/*
 *              nk      nb      nr
 * AES-128      4       4       10
 * AES-192      6       4       12
 * AES-256      8       4       14
 */
int main() {
    AES aes(4, 4, 10);
    vector<uint8_t> iv = {16, 15, 14, 13, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

    OFB ofb(aes, iv);

    /*
     * текстовые сообщения
     */
    // string keyForStr = "ffffffffffffffffbbbbbbbbbbbbbbbb";
    // // string message = "ffffffffffffffffaaaaaaaaaaaaaaaa";
    //   string message = "a reader will be distracted by the readable content of a page when looking at it";
    // cout << "message: " << message << endl;
    // cout << message.length() << endl;
    // string encryptMessage = ofb.encryptString(message, keyForStr);
    // cout << "encryptMessage: " << encryptMessage << endl;
    // cout << encryptMessage.length() << endl;

    // string decryptMessage = ofb.decryptString(encryptMessage, keyForStr);
    // cout << "decryptMessage: " << decryptMessage << endl;
    // cout << decryptMessage.size() << endl;

    /*
     * произвольные файлы
     */
    // string keyForFiles = "asdfghjklqwertyuasdfghjklqwertyu";
    string keyForFiles = "asdfghjklqwertaa";

    // string filepath = "../data/img.png";
    // string encryptFilepath = "../data/encrypt_img.png";
    // string decryptFilepath = "../data/decrypt_img.png";

    string filepath = "../data/a.pdf";
    string encryptFilepath = "../data/a.enc";
    string decryptFilepath = "../data/new_a.pdf";

    ofb.encryptFile(filepath, encryptFilepath, keyForFiles);
    ofb.decryptFile(encryptFilepath, decryptFilepath, keyForFiles);


    return 0;
}
