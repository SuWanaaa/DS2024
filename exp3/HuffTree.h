#include "BinTree.h" 
#include <queue>
#include <unordered_map>
#include <vector>

class HuffTree {
public:
    BinTreeNode<char>* root;

    // ���캯��ʹ�ù�����������
    HuffTree(const std::unordered_map<char, int>& frequencies) {
        // �������ȶ��еıȽϺ���
        auto cmp = [](BinTreeNode<char>* left, BinTreeNode<char>* right) { 
            return left->freq > right->freq; 
        };
        
        // �������ȶ��в�Ӧ���Զ���ıȽϺ���
        std::priority_queue<BinTreeNode<char>*, std::vector<BinTreeNode<char>*>, decltype(cmp)> pq(cmp);

        // ����Ƶ�ʱ�������ʼ�ڵ�
        for (auto it = frequencies.begin(); it != frequencies.end(); ++it) {
            char ch = it->first;
            int freq = it->second;
            pq.push(new BinTreeNode<char>(ch, freq));
        }

        // ������������
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



