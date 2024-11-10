#include <iostream>
#include <fstream>
#include <unordered_map>
#include "HuffmanEncoder.h"

// �����ַ�Ƶ��
std::unordered_map<char, int> calculateFrequency(const std::string& text) {
    std::unordered_map<char, int> freq;
    for (char ch : text) {
        if (isalpha(ch)) freq[tolower(ch)]++;
    }
    return freq;
}

int main() {
    // ���ļ�����ȡ����
    std::ifstream file("text.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file 'text.txt'." << std::endl;
        return 1;
    }
    
    // ���ļ����ݴ洢���ַ��� text ��
    std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close(); // �ر��ļ�

    // �����ַ�Ƶ�ʲ����� Huffman ������
    auto frequencies = calculateFrequency(text);
    HuffmanEncoder encoder(frequencies);

    // ��ָ�����ʽ��б���
    std::string word = "dream";
    std::cout << "Encoding for '" << word << "': " << encoder.encode(word) << std::endl;

    // ���ÿ���ַ��ı���
    for (auto it = encoder.getCodes().begin(); it != encoder.getCodes().end(); ++it) {
        char ch = it->first;
        std::string code = it->second;
        std::cout << ch << ": " << code << std::endl;
    }

    return 0;
}

