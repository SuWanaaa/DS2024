#include "HuffTree.h"
#include "Bitmap.h"
#include <unordered_map>

class HuffmanEncoder {
private:
    std::unordered_map<char, std::string> codes;

    void generateCodes(BinTreeNode<char>* node, const std::string& code) {
        if (!node) return;
        if (node->data != '\0') codes[node->data] = code;
        generateCodes(node->left, code + "0");
        generateCodes(node->right, code + "1");
    }

public:
    HuffmanEncoder(const std::unordered_map<char, int>& frequencies) {
        HuffTree tree(frequencies);
        generateCodes(tree.root, "");
    }

    std::string encode(const std::string& text) {
        std::string encoded;
        for (char ch : text) encoded += codes[ch];
        return encoded;
    }

    const std::unordered_map<char, std::string>& getCodes() const { return codes; }
};



