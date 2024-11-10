#include "BinTree.h" 
#include <queue>
#include <unordered_map>
#include <vector>

class HuffTree {
public:
    BinTreeNode<char>* root;

    // 构造函数使用哈夫曼树构建
    HuffTree(const std::unordered_map<char, int>& frequencies) {
        // 定义优先队列的比较函数
        auto cmp = [](BinTreeNode<char>* left, BinTreeNode<char>* right) { 
            return left->freq > right->freq; 
        };
        
        // 定义优先队列并应用自定义的比较函数
        std::priority_queue<BinTreeNode<char>*, std::vector<BinTreeNode<char>*>, decltype(cmp)> pq(cmp);

        // 遍历频率表并创建初始节点
        for (auto it = frequencies.begin(); it != frequencies.end(); ++it) {
            char ch = it->first;
            int freq = it->second;
            pq.push(new BinTreeNode<char>(ch, freq));
        }

        // 构建哈夫曼树
        while (pq.size() > 1) {
            auto left = pq.top(); pq.pop();
            auto right = pq.top(); pq.pop();
            auto merged = new BinTreeNode<char>('\0', left->freq + right->freq);
            merged->left = left;
            merged->right = right;
            pq.push(merged);
        }

        root = pq.top();
    }

    ~HuffTree() { delete root; }
};



