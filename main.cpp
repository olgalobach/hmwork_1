// Ольга Лобач. ИУ8-13. ДЗ 2. Вариант 12
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main(int argc, const char *argv[]) {
    if ((argc != 4) && (argc != 3)) {
        std::cerr << "Error: Use three or two parameters\n";
        return 5;
    }
    const std::string mode(argv[1]); // Режим работы
    const std::string filename(argv[2]);
    int key = 156;
    srand(key);
    unsigned int leftShift = 4;
    if (mode == "encryption") {
        const std::string old_string(argv[3]);
        const char *data = old_string.data();
        int size = old_string.size();

        std::vector<char> bytes(size);
        for (int i = 0; i < size; i++) {
            bytes[i] = data[i];
        }
        std::ofstream outputFile;
        outputFile.open(filename, std::ios::binary);
        for (int i = 0; i < bytes.size(); i += 4) {
            unsigned int gamma = rand();
            unsigned char b0 = bytes[i];
            unsigned char b1 = i + 1 < bytes.size() ? bytes[i + 1] : 0u;
            unsigned char b2 = i + 2 < bytes.size() ? bytes[i + 2] : 0u;
            unsigned char b3 = i + 3 < bytes.size() ? bytes[i + 3] : 0u;
            unsigned int message_to_int =
                    ((static_cast<unsigned int>(b0) << 24u) | (static_cast<unsigned int>(b1) << 16u) |
                     (static_cast<unsigned int>(b2) << 8u) | (static_cast<unsigned int>(b3)));
            unsigned int message_xored_with_gamma = message_to_int ^ gamma;
            unsigned int tmp = (message_xored_with_gamma & 0xFFFFFFFF) >> (32 - leftShift);
            unsigned int encrypted_block =
                    (message_xored_with_gamma << leftShift) | tmp;
            char r1 = encrypted_block >> 24;
            char r2 = encrypted_block >> 16;
            char r3 = encrypted_block >> 8;
            char r4 = encrypted_block;
            outputFile.write(&r1, sizeof(r1));
            outputFile.write(&r2, sizeof(r2));
            outputFile.write(&r3, sizeof(r3));
            outputFile.write(&r4, sizeof(r4));
        }
        outputFile.close();
    } else if (mode == "decryption") {
        std::ifstream readFile;
        readFile.open(filename, std::ios::binary);
        std::vector<char> encryptedData((std::istreambuf_iterator<char>(readFile)),
                                        std::istreambuf_iterator<char>());
        readFile.close();
        std::vector<char> decryptedData(encryptedData.size());
        for (int i = 0; i < encryptedData.size(); i += 4) {
            unsigned int gamma = rand();
            unsigned char b0 = encryptedData[i];
            unsigned char b1 = i + 1 < encryptedData.size() ? encryptedData[i + 1] : 0u;
            unsigned char b2 = i + 2 < encryptedData.size() ? encryptedData[i + 2] : 0u;
            unsigned char b3 = i + 3 < encryptedData.size() ? encryptedData[i + 3] : 0u;
            unsigned int encrypted_block = ((static_cast<unsigned int>(b0) << 24u) |
                                            (static_cast<unsigned int>(b1) << 16u) |
                                            (static_cast<unsigned int>(b2) << 8u) | (static_cast<unsigned int>(b3)));
            unsigned int shifted_encrypted_block =
                    ((encrypted_block & 0xFFFFFFFF) >> leftShift) | encrypted_block << (32 - leftShift);
            unsigned int result =
                    shifted_encrypted_block ^ gamma;
            unsigned char r0 = result >> 24;
            unsigned char r1 = result >> 16;
            unsigned char r2 = result >> 8;
            unsigned char r3 = result;
            decryptedData[i] = r0;
            decryptedData[i + 1] = r1;
            decryptedData[i + 2] = r2;
            decryptedData[i + 3] = r3;
        }
        for (unsigned int i = 0; i < decryptedData.size(); i++) {
            if (decryptedData[i] != 0) {
                std::cout << decryptedData[i];
            }
        }
    }
    return 0;
}