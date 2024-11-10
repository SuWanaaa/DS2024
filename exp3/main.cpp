#include <iostream>
#include <fstream>
#include <unordered_map>
#include "HuffmanEncoder.h"

// 计算字符频率
std::unordered_map<char, int> calculateFrequency(const std::string& text) {
    std::unordered_map<char, int> freq;
    for (char ch : text) {
        if (isalpha(ch)) freq[tolower(ch)]++;
    }
    return freq;
}

int main() {
    // 打开文件并读取内容
    std::ifstream file("text.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file 'text.txt'." << std::endl;
        return 1;
    }
    
    // 将文件内容存储到字符串 text 中
    std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close(); // 关闭文件

    // 计算字符频率并构建 Huffman 编码器
    auto frequencies = calculateFrequency(text);
    HuffmanEncoder encoder(frequencies);

    // 对指定单词进行编码
    std::string word = "dream";
    std::cout << "Encoding for '" << word << "': " << encoder.encode(word) << std::endl;

    // 输出每个字符的编码
    for (auto it = encoder.getCodes().begin(); it != encoder.getCodes().end(); ++it) {
        char ch = it->first;
        std::string code = it->second;
        std::cout << ch << ": " << code << std::endl;
    }

    return 0;
}

